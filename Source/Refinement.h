#pragma once
#include "Diagnostics.h"
#include <random>
namespace nova {
enum class RefineGoal { balanced,attack,body,lessMetallic,brighter,darker,shorterTail,lessEffects };
struct RefineExample {FitTarget target;float velocity=.8f;};
struct RefineMetrics {double attack=0,body=0,tail=0,overall=0,metallic=0,brightnessDelta=0;};
struct RefinementReport {RefineGoal goal=RefineGoal::balanced;FitReport fit;RefineMetrics before,after;double objectiveBefore=0,objectiveAfter=0;int examples=0;};
inline RefineMetrics refinementMetrics(const Features& ref,const Features& result){
    RefineMetrics m;for(int i=0;i<8;++i)m.attack+=std::abs(ref.attack[i]-result.attack[i])/8;
    for(int i=0;i<128;++i)m.body+=std::abs(ref.spectrum[i]-result.spectrum[i])/128;
    for(int i=16;i<24;++i)m.tail+=std::abs(ref.envelope[i]-result.envelope[i])/8;
    for(int i=0;i<128;++i)m.brightnessDelta+=(result.spectrum[i]-ref.spectrum[i])*i/(128.*128.);m.overall=featureDistance(ref,result);return m;
}
inline double metallicEnergy(const SpectralModel* model,const Params& p){
    if(!model||!model->valid())return p.fm*.25+p.resonance*.1;
    const double root=440*std::pow(2.,(model->root-69)/12.);double inharmonic=0,total=0;
    for(int i=0;i<model->count;++i){const auto& part=model->partials[i];double a=0;for(float value:part.amplitude)a+=value*value;double weight=std::pow(std::max(40.f,part.frequency)/500.f,p.spectralTone)*p.partialGains[i];a*=weight*weight;double ratio=part.frequency/root;double cents=1200*std::abs(std::log2(ratio/std::max(1.,std::round(ratio))));total+=a;if(cents>35)inharmonic+=a;}
    return inharmonic/std::max(1e-12,total);
}
inline double refinementObjective(const RefineMetrics& m,RefineGoal goal){
    if(goal==RefineGoal::attack)return m.overall*.45+m.attack*.5+m.tail*.08;
    if(goal==RefineGoal::body)return m.overall*.35+m.body*.8+m.tail*.08;
    if(goal==RefineGoal::lessMetallic)return m.overall+m.metallic*.16;
    if(goal==RefineGoal::brighter)return m.overall+std::max(0.,-m.brightnessDelta)*1.2;
    if(goal==RefineGoal::darker)return m.overall+std::max(0.,m.brightnessDelta)*1.2;
    if(goal==RefineGoal::shorterTail)return m.overall*.5+m.tail*.7;
    return m.overall;
}
// Fixed engine and note mapping. Searches locally around the user's current patch.
// References are used only as analysis targets; Engine receives oscillator models.
inline RefinementReport refineInstrument(const std::vector<RefineExample>& examples,Params initial,const std::shared_ptr<SpectralModel>& model,const std::shared_ptr<ZoneBank>& zones,RefineGoal goal,int budget,std::atomic<bool>& cancel,const std::function<void(int,int,double)>& progress={}){
    RefinementReport out;out.goal=goal;out.fit.parameters=initial;out.fit.model=model;out.examples=static_cast<int>(examples.size());
    if(examples.empty()||examples.size()>maxZones){out.fit.error="Elige entre una y ocho referencias.";return out;}
    if(initial.engine==1&&(!model||!model->valid())&&(!zones||!zones->valid())){out.fit.error="Primero aprende el instrumento.";return out;}
    const double rate=examples.front().target.rate;std::vector<RefineExample> targets=examples;std::vector<Features> reference;FeatureExtractor extractor;
    for(auto& e:targets){if(e.target.rate!=rate||e.target.left.size()!=e.target.right.size()||e.target.left.size()<128){out.fit.error="Referencia de refinamiento invalida.";return out;}e.target=centredTarget(e.target);auto f=extractor.extract(e.target.left,e.target.right,rate);if(f.rms<1e-7){out.fit.error="Referencia silenciosa.";return out;}reference.push_back(f);}
    budget=std::clamp(budget,24,600);Engine engine;engine.prepare(rate);engine.setModel(model.get());engine.setZoneBank(zones.get());std::vector<float> l,r;double best=1e9;Params winner=initial;RefineMetrics winnerMetrics;double winnerGain=1;
    auto evaluate=[&](const Params& p){
        RefineMetrics metrics;double gains=0;bool valid=true;
        for(size_t i=0;i<targets.size();++i){if(cancel.load()){valid=false;break;}const auto& t=targets[i].target;l.assign(t.left.size(),0);r.assign(t.right.size(),0);engine.configure(p);engine.reset();engine.noteOn(1,t.note,targets[i].velocity);int gate=p.engine==1?static_cast<int>(l.size()):std::clamp(static_cast<int>(t.gate*rate),0,static_cast<int>(l.size()));engine.render(l.data(),r.data(),gate);engine.noteOff(1,t.note);engine.render(l.data()+gate,r.data()+gate,static_cast<int>(l.size())-gate);
            auto f=extractor.extract(l,r,rate);if(f.rms<1e-8){valid=false;break;}auto m=refinementMetrics(reference[i],f);metrics.attack+=m.attack;metrics.body+=m.body;metrics.tail+=m.tail;metrics.overall+=m.overall;metrics.brightnessDelta+=m.brightnessDelta;gains+=std::log(std::max(1e-9f,reference[i].rms)/f.rms);
            const SpectralModel* used=p.engine==1?model.get():nullptr;if(p.engine==1&&p.useZones&&zones&&zones->valid())used=&zones->zones[zones->select(t.note,targets[i].velocity)].model;metrics.metallic+=metallicEnergy(used,p);
        }
        if(!valid){if(!cancel.load()){++out.fit.trials;if(progress)progress(out.fit.trials,budget,best);}return;}
        for(double* x:{&metrics.attack,&metrics.body,&metrics.tail,&metrics.overall,&metrics.metallic,&metrics.brightnessDelta})*x/=targets.size();
        double loss=refinementObjective(metrics,goal);
        if(goal==RefineGoal::lessEffects)loss+=.3*((p.reverbOn?p.reverb:0)+(p.delayOn?p.delay:0)+(p.chorusOn?p.chorus:0))+.12*(p.driveOn?p.drive:0);
        if(out.fit.trials==0){out.before=metrics;out.objectiveBefore=loss;}
        bool preservesBody=goal!=RefineGoal::body||metrics.body<=out.before.body+1e-7;
        if(goal==RefineGoal::lessEffects)preservesBody=preservesBody&&p.reverb<=initial.reverb&&p.delay<=initial.delay&&p.chorus<=initial.chorus&&p.drive<=initial.drive;
        if(preservesBody&&loss<best){best=loss;winner=p;winnerMetrics=metrics;winnerGain=std::exp(gains/targets.size());}
        ++out.fit.trials;if(progress)progress(out.fit.trials,budget,best);
    };
    evaluate(initial);if(goal==RefineGoal::lessEffects){auto dry=initial;dry.reverb*=.4f;dry.delay*=.4f;dry.chorus*=.5f;dry.drive*=.6f;evaluate(dry);}std::mt19937 random(0x4e533134);std::normal_distribution<float> normal(0,1);
    while(out.fit.trials<budget&&!cancel.load()&&best<1e8){Params p=winner;float change=normal(random)*(.18f*(1.f-out.fit.trials/static_cast<float>(budget))+.025f);int control=random()%16;
        if(control==0)p.attack=std::clamp(p.attack*std::exp(change*3),.001f,3.f);
        if(control==1)p.decay=std::clamp(p.decay*std::exp(change*3),.005f,3.f);
        if(control==2)p.sustain=std::clamp(p.sustain+change,0.f,1.f);
        if(control==3)p.release=std::clamp(p.release*std::exp(change*3),.005f,4.f);
        if(control==4)p.cutoff=std::clamp(p.cutoff*std::exp(change*2),30.f,20000.f);
        if(control==5)p.resonance=std::clamp(p.resonance+change,0.f,.95f);
        if(control==6)p.fm=std::clamp(p.fm+change,0.f,1.f);
        if(control==7)p.noise=std::clamp(p.noise+change*.1f,0.f,.2f);
        if(control==8)p.spectralTime=std::clamp(p.spectralTime*std::exp(change),.25f,4.f);
        if(control==9)p.spectralTone=std::clamp(p.spectralTone+change,-1.f,1.f);
        if(control==10)p.residual=std::clamp(p.residual+change,0.f,2.f);
        if(control==11){p.eqOn=true;p.eqHigh=std::clamp(p.eqHigh+change*12,-12.f,12.f);}
        if(control==12){p.eqOn=true;p.eqLow=std::clamp(p.eqLow+change*12,-12.f,12.f);}
        if(control==13){p.reverbOn=true;p.reverb=std::clamp(p.reverb+change*.25f,0.f,.7f);}
        if(control==14){p.delayOn=true;p.delay=std::clamp(p.delay+change*.2f,0.f,.7f);}
        if(control==15){int partial=random()%partialCount;p.partialGains[partial]=std::clamp(p.partialGains[partial]+change,0.f,2.f);if(goal==RefineGoal::lessMetallic&&model&&partial<model->count){double ratio=model->partials[partial].frequency/(440*std::pow(2.,(model->root-69)/12.));if(std::abs(1200*std::log2(ratio/std::max(1.,std::round(ratio))))>35)p.partialGains[partial]=std::max(0.f,winner.partialGains[partial]-std::abs(change));}}
        evaluate(p);
    }
    out.fit.cancelled=cancel.load();if(out.fit.cancelled||best>=1e8)return out;
    winner.master=static_cast<float>(std::clamp(winner.master+20*std::log10(std::max(1e-9,winnerGain)),-36.,0.));out.fit.parameters=winner;out.after=winnerMetrics;out.objectiveAfter=best;out.fit.initialError=out.objectiveBefore;out.fit.bestError=best;out.fit.valid=true;
    // Verify all tonal examples at their own note/intensity before publishing.
    for(const auto& e:targets){if(cancel.load()){out.fit.cancelled=true;return out;}if(!e.target.tonal)continue;Sound probe;probe.sampleRate=rate;probe.left.resize(e.target.left.size());probe.right=probe.left;engine.configure(winner);engine.reset();engine.noteOn(1,e.target.note,e.velocity);engine.render(probe.left.data(),probe.right.data(),static_cast<int>(probe.left.size()));analyse(probe);if(probe.tonal&&probe.confidence>.85&&std::abs(1200*std::log2(probe.estimatedHz/(440*std::pow(2.,(e.target.note-69+winner.fineTune/100.)/12.))))>65){out.fit.valid=false;out.fit.error="El refinamiento no supera la verificacion tonal. Se conserva el ajuste anterior.";return out;}}
    out.fit.error="Refinado local: compara A/B. La distancia interna no garantiza parecido perceptual.";return out;
}
}
