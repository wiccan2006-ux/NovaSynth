#pragma once
#include "SynthCore.h"
#include "Presets.h"
#include <atomic>
#include <complex>
#include <functional>
#include <limits>
#include <random>
#include <string>
#include <memory>

namespace nova {
struct Features {
    std::array<float,128> spectrum{};
    std::array<float,24> envelope{};
    std::array<float,8> attack{};
    float rms=0,width=0;
};
class FeatureExtractor {
    std::array<std::complex<float>,4096> bins;
    std::array<float,1024> shortWindow;
    std::array<float,4096> longWindow;
    void fft(int n) {
        for(int i=1,j=0;i<n;++i){int bit=n>>1;for(;j&bit;bit>>=1)j^=bit;j^=bit;if(i<j)std::swap(bins[i],bins[j]);}
        for(int len=2;len<=n;len<<=1){auto step=std::polar(1.f,static_cast<float>(-2*pi/len));for(int base=0;base<n;base+=len){std::complex<float> w=1;for(int j=0;j<len/2;++j){auto a=bins[base+j],b=w*bins[base+j+len/2];bins[base+j]=a+b;bins[base+j+len/2]=a-b;w*=step;}}}
    }
public:
    FeatureExtractor(){for(int i=0;i<1024;++i)shortWindow[i]=static_cast<float>(.5-.5*std::cos(2*pi*i/1023));for(int i=0;i<4096;++i)longWindow[i]=static_cast<float>(.5-.5*std::cos(2*pi*i/4095));}
    Features extract(const std::vector<float>& l,const std::vector<float>& r,double sr) {
        Features f;double energy=0,mid=0,side=0;
        for(size_t i=0;i<l.size();++i){energy+=(l[i]*l[i]+r[i]*r[i])*.5;mid+=(l[i]+r[i])*(l[i]+r[i]);side+=(l[i]-r[i])*(l[i]-r[i]);}
        f.rms=static_cast<float>(std::sqrt(energy/std::max<size_t>(1,l.size())));f.width=static_cast<float>(side/std::max(1e-12,4*energy));
        const bool chooseLeft=mid<energy*.05;float scale=std::max(f.rms,1e-7f);
        for(size_t j=0;j<f.envelope.size();++j){size_t a=j*l.size()/f.envelope.size(),b=(j+1)*l.size()/f.envelope.size();double e=0;for(size_t i=a;i<b;++i)e+=(l[i]*l[i]+r[i]*r[i])*.5;f.envelope[j]=static_cast<float>(std::sqrt(e/std::max<size_t>(1,b-a)))/scale;}
        for(int j=0;j<8;++j){size_t a=std::min(l.size(),static_cast<size_t>(j*sr*.004)),b=std::min(l.size(),static_cast<size_t>((j+1)*sr*.004));double e=0;for(size_t k=a;k<b;++k)e+=(l[k]*l[k]+r[k]*r[k])*.5;f.attack[j]=static_cast<float>(std::sqrt(e/std::max<size_t>(1,b-a)))/scale;}
        for(int pass=0;pass<10;++pass){
            int n=pass<8?1024:4096,bandCount=pass<8?24:32;
            double fraction=pass<8?pass/7.0:(pass==8?.2:.65);
            size_t start=static_cast<size_t>(std::max(0.,static_cast<double>(l.size())-n)*fraction);
            for(int j=0;j<n;++j){size_t i=start+static_cast<size_t>(j);float x=i<l.size()?(chooseLeft?l[i]:(l[i]+r[i])*.5f):0;bins[j]={x*(n==1024?shortWindow[j]:longWindow[j]),0};}
            fft(n);
            for(int b=0;b<bandCount;++b){
                double lo=35*std::pow(sr*.45/35,b/static_cast<double>(bandCount)),hi=35*std::pow(sr*.45/35,(b+1.)/bandCount);
                int a=std::clamp(static_cast<int>(lo*n/sr),1,n/2-1),z=std::clamp(static_cast<int>(hi*n/sr),a,n/2-1);
                double power=0;for(int k=a;k<=z;++k)power+=std::norm(bins[k]);
                float value=static_cast<float>(std::log1p(100*std::sqrt(power/(z-a+1))/(n*scale)));
                int destination=pass<8?(pass/2)*24+b:96+b;f.spectrum[destination]+=value*.5f;
            }
        }
        return f;
    }
};
inline double featureDistance(const Features& a,const Features& b) {
    double spectral=0,envelope=0,attack=0;
    for(size_t i=0;i<a.spectrum.size();++i)spectral+=std::abs(a.spectrum[i]-b.spectrum[i]);
    for(size_t i=0;i<a.envelope.size();++i)envelope+=std::abs(a.envelope[i]-b.envelope[i]);
    for(size_t i=0;i<a.attack.size();++i)attack+=std::abs(a.attack[i]-b.attack[i]);
    return attack/a.attack.size()*.06+spectral/a.spectrum.size()*.72+envelope/a.envelope.size()*.25+std::abs(a.width-b.width)*.08;
}
struct FitTarget {
    double rate=22050;
    std::vector<float> left,right;
    int note=60;float fine=0;double gate=.5;
    bool tonal=false;
};
inline FitTarget makeTarget(const Sound& sound,int fallbackNote,double analysisRate=22050,bool trimOnset=true) {
    FitTarget t;t.rate=analysisRate;t.note=std::clamp(fallbackNote,24,108);t.tonal=sound.tonal;
    if(sound.tonal&&std::isfinite(sound.estimatedHz)&&sound.estimatedHz>0){double detected=69+12*std::log2(sound.estimatedHz/440.);t.fine=std::clamp(static_cast<float>((detected-std::round(detected))*100),-100.f,100.f);}
    else t.tonal=false;
    float peak=0;for(float x:sound.left)peak=std::max(peak,std::abs(x));for(float x:sound.right)peak=std::max(peak,std::abs(x));
    if(peak<1e-6f)return t;
    size_t onset=0;while(onset<sound.left.size() && std::max(std::abs(sound.left[onset]),std::abs(sound.right[onset]))<peak*.015f)++onset;
    // Keep 5 ms before the detected attack, then at most 3 s for the search objective.
    onset=onset>sound.sampleRate*.005?onset-static_cast<size_t>(sound.sampleRate*.005):0;
    if(!trimOnset)onset=0;
    double duration=std::clamp((sound.left.size()-onset)/sound.sampleRate,trimOnset?.6:.05,3.);
    size_t count=static_cast<size_t>(duration*t.rate);t.left.assign(count,0);t.right=t.left;
    // Windowed-sinc resampling limits aliasing when references use 44.1/48/96 kHz.
    constexpr int radius=32;double cutoff=.45*std::min(1.,t.rate/sound.sampleRate);
    for(size_t i=0;i<count;++i){
        double pos=onset+i*sound.sampleRate/t.rate;if(pos>=sound.left.size()-1)break;int centre=static_cast<int>(pos);double l=0,r=0,normal=0;
        for(int k=centre-radius+1;k<=centre+radius;++k){if(k<0||static_cast<size_t>(k)>=sound.left.size())continue;double d=pos-k;double weight=(std::abs(d)<1e-10?2*cutoff:std::sin(2*pi*cutoff*d)/(pi*d))*(.5+.5*std::cos(pi*d/radius));l+=sound.left[static_cast<size_t>(k)]*weight;r+=sound.right[static_cast<size_t>(k)]*weight;normal+=weight;}
        if(std::abs(normal)>1e-10){t.left[i]=static_cast<float>(l/normal);t.right[i]=static_cast<float>(r/normal);}
    }
    double le=0,re=0;for(size_t i=0;i<t.left.size();++i){le+=t.left[i]*t.left[i];re+=t.right[i]*t.right[i];}if(re>le)std::swap(t.left,t.right);
    t.gate=duration*.7;return t;
}
} // namespace nova
#include "PartialAnalysis.h"
namespace nova {
// Search compares centred tones, so a clean patch is not penalised for refusing
// to reproduce the reference's global detuning.
inline FitTarget centredTarget(const FitTarget& input){
    FitTarget t=input;if(!t.tonal||!std::isfinite(t.fine)||std::abs(t.fine)<.001f)return t;
    double step=std::pow(2.,-std::clamp(t.fine,-50.f,50.f)/1200.);constexpr int radius=20;
    for(size_t i=0;i<t.left.size();++i){double pos=i*step;int centre=static_cast<int>(pos);double l=0,r=0,sum=0;
        for(int k=centre-radius+1;k<=centre+radius;++k){if(k<0||static_cast<size_t>(k)>=input.left.size())continue;double d=pos-k,w=(std::abs(d)<1e-10?1:std::sin(pi*d)/(pi*d))*(.5+.5*std::cos(pi*d/radius));l+=input.left[k]*w;r+=input.right[k]*w;sum+=w;}
        t.left[i]=std::abs(sum)>1e-10?static_cast<float>(l/sum):0;t.right[i]=std::abs(sum)>1e-10?static_cast<float>(r/sum):0;
    }t.fine=0;return t;
}
using Genes=std::array<float,30>;
inline float logarithmic(float x,float lo,float hi){return lo*std::pow(hi/lo,std::clamp(x,0.f,1.f));}
inline float inverseLog(float x,float lo,float hi){return std::clamp(std::log(std::max(lo,x)/lo)/std::log(hi/lo),0.f,1.f);}
inline Params fromGenes(const Genes& g,int note){
    Params p;p.mode=0;p.root=note;p.waveA=std::clamp(static_cast<int>(std::round(g[0]*4)),0,4);p.waveB=std::clamp(static_cast<int>(std::round(g[1]*4)),0,4);
    p.transpose=std::clamp(static_cast<int>(std::round(g[2]*48-24)),-24,24);p.unison=g[3]>.5f;p.mix=g[4];p.detune=g[5]*35;p.sub=g[6];p.noise=g[7]*.2f;p.fm=g[8];
    p.cutoff=logarithmic(g[9],30,20000);p.resonance=g[10];p.envAmount=g[11]*10-4;p.attack=logarithmic(g[12],.001f,3);p.decay=logarithmic(g[13],.005f,3);p.sustain=g[14];p.release=logarithmic(g[15],.005f,4);
    p.lfoRate=logarithmic(g[16],.03f,12);p.lfoDepth=g[17]*2;p.drive=g[18];p.chorus=g[19]*.6f;p.chorusRate=logarithmic(g[20],.1f,3);
    p.delay=g[21]*.6f;p.delayTime=logarithmic(g[22],.04f,1);p.feedback=g[23]*.7f;p.reverb=g[24]*.6f;p.room=g[25];p.eqLow=g[26]*24-12;p.eqHigh=g[27]*24-12;p.spread=g[28];p.fineTune=g[29]*200-100;p.master=-6;
    return p;
}
inline Genes toGenes(const Params& p){return {p.waveA/4.f,p.waveB/4.f,(p.transpose+24)/48.f,p.unison?1.f:0.f,p.mix,p.detune/35,p.sub,p.noise/.2f,p.fm,inverseLog(p.cutoff,30,20000),p.resonance,(p.envAmount+4)/10,inverseLog(p.attack,.001f,3),inverseLog(p.decay,.005f,3),p.sustain,inverseLog(p.release,.005f,4),inverseLog(p.lfoRate,.03f,12),p.lfoDepth/2,p.driveOn?p.drive:0,p.chorusOn?p.chorus/.6f:0,inverseLog(p.chorusRate,.1f,3),p.delayOn?p.delay/.6f:0,inverseLog(p.delayTime,.04f,1),p.feedback/.7f,p.reverbOn?p.reverb/.6f:0,p.room,(p.eqLow+12)/24,(p.eqHigh+12)/24,p.spread,(p.fineTune+100)/200};}
inline void renderCandidate(Engine& engine,const Params& p,const FitTarget& target,std::vector<float>& l,std::vector<float>& r){
    engine.configure(p);engine.reset();engine.noteOn(1,target.note,1);
    int gate=p.engine==1?static_cast<int>(l.size()):std::clamp(static_cast<int>(target.gate*target.rate),0,static_cast<int>(l.size()));
    engine.render(l.data(),r.data(),gate);engine.noteOff(1,target.note);engine.render(l.data()+gate,r.data()+gate,static_cast<int>(l.size())-gate);
}
} // namespace nova
#include "NeuralSeed.h"
namespace nova {
struct FitReport {Params parameters;std::shared_ptr<SpectralModel> model;Comparison comparison;SoundProfile profile;double nativeError=0,spectralError=0;double initialError=0,bestError=0;int trials=0;bool cancelled=false,valid=false,neuralUsed=false;std::string error;};
inline FitReport fitNativeParameters(const FitTarget& referenceTarget,Params initial,int budget,std::atomic<bool>& cancel,const std::function<void(int,int,double)>& progress={},int family=-1,bool useNeural=true,bool dryOnly=false){
    const auto target=centredTarget(referenceTarget);FitReport report;report.parameters=initial;
    if(target.left.size()<128){report.error="Referencia vacia o silenciosa.";return report;}
    FeatureExtractor extractor;auto reference=extractor.extract(target.left,target.right,target.rate);
    if(reference.rms<1e-6){report.error="La referencia no tiene energia suficiente.";return report;}
    Engine engine;engine.prepare(target.rate);std::vector<float> l(target.left.size()),r(l.size());
    struct Member{Genes genes;double error;float rms;};std::vector<Member> population;
    std::mt19937 random(0x4e6f7661);std::uniform_real_distribution<float> unit(0,1);std::normal_distribution<float> normal(0,1);
    budget=std::max(2,budget);double best=std::numeric_limits<double>::infinity();Member winner{};
    initial.engine=0;initial.mode=0;initial.root=target.note;initial.fineTune=target.fine;initial.master=-6;
    auto evaluate=[&](Genes g){
        for(auto& x:g)x=std::clamp(x,0.f,1.f);
        // A matched patch starts on equal temperament, without deliberate detune/chorus.
        g[29]=.5f;g[3]=0;g[5]=0;g[6]=0;g[19]=0;g[2]=(std::clamp(std::round((g[2]*48-24)/12),0.f,2.f)*12+24)/48.f;
        if(dryOnly){g[18]=g[21]=g[24]=0;g[26]=g[27]=.5f;}auto p=fromGenes(g,target.note);renderCandidate(engine,p,target,l,r);auto f=extractor.extract(l,r,target.rate);
        double distance=f.rms>1e-8?featureDistance(reference,f):1000.;Member member{g,distance,f.rms};
        if(report.trials==0)report.initialError=distance;
        ++report.trials;if(distance<best){best=distance;winner=member;}
        population.push_back(member);std::sort(population.begin(),population.end(),[](const Member& a,const Member& b){return a.error<b.error;});if(population.size()>10)population.pop_back();
        if(progress)progress(report.trials,budget,best);
    };
    evaluate(toGenes(initial));
    if(useNeural&&report.trials<budget&&!cancel.load()){
        auto prediction=neuralSeed(target,extractor);prediction[29]=(target.fine+100)/200;report.neuralUsed=true;
        for(int i=0;i<4&&report.trials<budget&&!cancel.load();++i){auto candidate=prediction;if(i==1)candidate[3]=0;if(i==2)candidate[9]=std::min(1.f,candidate[9]+.12f);if(i==3){candidate[0]=std::round(candidate[0]*4)/4;candidate[4]*=.5f;}evaluate(candidate);}
    }
    // Search both the user's current patch and independent procedural oscillator seeds.
    for(int i=0;i<64 && report.trials<budget && !cancel.load();++i){auto p=preset(family<0?i:(family*8+i)%64);p.root=target.note;p.fineTune=target.fine;evaluate(toGenes(p));}
    for(int a=0;a<5 && report.trials<budget && !cancel.load();++a)for(int b=0;b<5 && report.trials<budget && !cancel.load();++b){Params p;p.waveA=a;p.waveB=b;p.unison=false;p.mix=.2f;p.cutoff=15000;p.envAmount=0;p.resonance=0;p.sub=0;p.attack=.004f;p.sustain=.7f;p.delay=p.reverb=0;p.fineTune=target.fine;evaluate(toGenes(p));}
    while(report.trials<budget && !cancel.load()){
        float stage=static_cast<float>(report.trials)/budget;
        auto child=population[unit(random)<.75f?0:static_cast<size_t>(random()%population.size())].genes;
        int changes=1+static_cast<int>(random()%4);float radius=.28f*(1-stage)+.035f;
        for(int j=0;j<changes;++j){size_t k=random()%child.size();if(k==0||k==1)child[k]=static_cast<float>(random()%5)/4;else if(k==3)child[k]=unit(random)<.5?0.f:1.f;else child[k]+=normal(random)*radius;}
        // Periodic crossover prevents the search being confined to one seed.
        if(report.trials%7==0 && population.size()>1){const auto& other=population[random()%population.size()].genes;for(size_t k=0;k<child.size();++k)if(unit(random)<.18f)child[k]=other[k];}
        evaluate(child);
    }
    report.cancelled=cancel.load();report.bestError=best;report.parameters=fromGenes(winner.genes,target.note);
    report.parameters.master=std::clamp(-6.f+20*std::log10(std::max(reference.rms,1e-9f)/std::max(winner.rms,1e-9f)),-36.f,0.f);
    report.parameters.driveOn=report.parameters.drive>.0001f;report.parameters.chorusOn=report.parameters.chorus>.0001f;
    report.parameters.delayOn=report.parameters.delay>.0001f;report.parameters.reverbOn=report.parameters.reverb>.0001f;report.parameters.eqOn=true;
    report.valid=std::isfinite(best)&&best<999.;return report;
}
inline bool tuneModelToNearestNote(SpectralModel& model,const FitTarget& target,bool clean=true){
    if(!target.tonal||!std::isfinite(target.fine))return false;
    float correction=std::pow(2.f,-std::clamp(target.fine,-50.f,50.f)/1200.f);double base=440*std::pow(2.,(target.note-69)/12.);
    double supported=0,total=0;int supportedCount=0,totalCount=0;double strongest=0;for(int i=0;i<model.count;++i)strongest=std::max(strongest,static_cast<double>(*std::max_element(model.partials[i].amplitude.begin(),model.partials[i].amplitude.end())));
    for(int i=0;i<model.count;++i){auto& part=model.partials[i];part.frequency=std::clamp(part.frequency*correction,20.f,20000.f);double ratio=part.frequency/base;int harmonic=std::max(1,static_cast<int>(std::lround(ratio)));double cents=1200*std::log2(ratio/harmonic);double amp=*std::max_element(part.amplitude.begin(),part.amplitude.end());
        if(ratio<=8.5){total+=amp*amp;if(std::abs(cents)<65)supported+=amp*amp;if(amp>strongest*.08){++totalCount;if(std::abs(cents)<65)++supportedCount;}}
    }
    // Only regularise a convincingly harmonic tone. Bells and inharmonic
    // percussion retain their structure instead of becoming a different instrument.
    if(!clean||total<1e-10||supported<total*.72||(totalCount>=3&&supportedCount<totalCount*.72))return false;
    SpectralModel cleaned=model;cleaned.count=0;cleaned.tracked=false;
    for(int i=0;i<model.count;++i){auto part=model.partials[i];int harmonic=std::max(1,static_cast<int>(std::lround(part.frequency/base)));double cents=1200*std::log2(part.frequency/(base*harmonic));if(std::abs(cents)>65||base*harmonic>20000)continue;
        part.frequency=static_cast<float>(base*harmonic);part.frequencyRatio.fill(1);
        int duplicate=-1;for(int j=0;j<cleaned.count;++j)if(std::abs(cleaned.partials[j].frequency-part.frequency)<.1f)duplicate=j;
        if(duplicate<0)cleaned.partials[cleaned.count++]=part;
        else {auto& existing=cleaned.partials[duplicate];for(int f=0;f<modelFrames;++f)existing.amplitude[f]=std::max(existing.amplitude[f],part.amplitude[f]);}
    }
    if(cleaned.count){model=cleaned;return true;}return false;
}
inline FitReport fitParameters(const FitTarget& referenceTarget,Params initial,int budget,std::atomic<bool>& cancel,const std::function<void(int,int,double)>& progress={},int preference=0,const std::function<void(const SoundProfile&)>& classified={},bool useNeural=true,bool cleanPitch=true,bool dryOnly=false){
    const auto target=centredTarget(referenceTarget);
    if(target.left.empty()||cancel.load())return fitNativeParameters(target,initial,2,cancel,progress);
    FeatureExtractor extractor;auto ref=extractor.extract(target.left,target.right,target.rate);auto profile=classifySound(target,ref);
    std::shared_ptr<SpectralModel> analysisModel;
    if(preference!=1&&!cancel.load()){
        analysisModel=std::make_shared<SpectralModel>(estimatePartials(target,cancel));
        if(analysisModel->valid())tuneModelToNearestNote(*analysisModel,target,cleanPitch);
        if(analysisModel->valid()&&(target.tonal||analysisModel->count<=16)){
            double base=440*std::pow(2.,(target.note-69)/12.),sum=0,deviation=0;if(!target.tonal)base=analysisModel->partials[0].frequency;
            for(int j=0;j<analysisModel->count;++j){const auto& part=analysisModel->partials[j];double a=*std::max_element(part.amplitude.begin(),part.amplitude.end());double ratio=part.frequency/base;deviation+=a*a*std::abs(ratio-std::round(ratio));sum+=a*a;}
            if(sum>0&&deviation/sum>.055){profile.family=5;profile.description="Metalico / campana o parciales inarmonicos";}
        }
    }
    if(classified)classified(profile);
    int nativeBudget=preference==2?0:(preference==1?budget:std::max(12,budget*3/5));nativeBudget=std::min(nativeBudget,budget);
    FitReport best;best.bestError=1e9;best.initialError=1e9;int trials=0;
    if(nativeBudget){best=fitNativeParameters(target,initial,nativeBudget,cancel,[&](int n,int,double loss){if(progress)progress(n,budget,loss);},profile.family,useNeural,dryOnly);trials=best.trials;best.nativeError=best.bestError;}
    if(preference!=1&&!cancel.load()){
        auto model=analysisModel;
        if(model&&model->valid()){
            Engine engine;engine.prepare(target.rate);engine.setModel(model.get());std::vector<float> l(target.left.size()),r(l.size());
            Params seed;seed.engine=1;seed.root=target.note;seed.unison=false;seed.mix=seed.sub=seed.fm=seed.noise=0;seed.cutoff=20000;seed.resonance=seed.envAmount=0;seed.attack=.001f;seed.decay=.005f;seed.sustain=1;seed.release=.6f;seed.delay=seed.reverb=seed.chorus=seed.drive=0;seed.master=-6;
            seed.residual=0;seed.tracking=0;auto fixedModel=std::make_shared<SpectralModel>(estimatePartials(target,cancel,false));if(fixedModel->valid())tuneModelToNearestNote(*fixedModel,target,cleanPitch);
            std::shared_ptr<SpectralModel> spectralModel=model;Params spectralBest=seed;double spectralError=1e9;float spectralRms=1e-6f;
            std::mt19937 rng(0x50617274);std::normal_distribution<float> normal(0,1);
            while(trials<budget&&!cancel.load()){
                int seedIndex=trials-nativeBudget;bool seeded=seedIndex<5;
                auto candidateModel=seedIndex==4&&fixedModel->valid()?fixedModel:(seeded?model:spectralModel);
                Params candidate=seeded?seed:spectralBest;
                if(seedIndex==1)candidate.residual=1;if(seedIndex==2)candidate.residual=.35f;if(seedIndex==3)candidate.residual=.7f;
                if(!seeded){int knob=static_cast<int>(rng()%14);float delta=normal(rng)*(trials<budget*.8?.12f:.04f);
                    if(knob==0)candidate.spectralTime=std::clamp(candidate.spectralTime+delta,.7f,1.4f);
                    if(knob==1)candidate.spectralTone=std::clamp(candidate.spectralTone+delta,-.5f,.5f);
                    if(knob==2)candidate.attack=std::clamp(candidate.attack+delta*.03f,.001f,.2f);
                    
                    
                    if(knob==5)candidate.reverb=std::clamp(candidate.reverb+delta,0.f,.5f);
                    if(knob==6)candidate.eqHigh=std::clamp(candidate.eqHigh+delta*10,-6.f,6.f);
                    if(knob==7)candidate.eqLow=std::clamp(candidate.eqLow+delta*10,-6.f,6.f);
                    if(knob==8)candidate.noise=std::clamp(candidate.noise+delta*.2f,0.f,.12f);
                    if(knob==9)candidate.delay=std::clamp(candidate.delay+delta,0.f,.35f);
                    if(knob==12)candidate.residual=std::clamp(candidate.residual+delta*2,0.f,2.f);
                    
                    if(knob==10||knob==11){int group=static_cast<int>(rng()%8);for(int j=group*6;j<std::min(model->count,(group+1)*6);++j)candidate.partialGains[j]=std::clamp(candidate.partialGains[j]+delta,0.f,2.f);}
                }
                if(dryOnly){candidate.drive=candidate.chorus=candidate.delay=candidate.reverb=0;candidate.eqLow=candidate.eqHigh=0;}engine.setModel(candidateModel.get());renderCandidate(engine,candidate,target,l,r);auto f=extractor.extract(l,r,target.rate);double error=f.rms>1e-8?featureDistance(ref,f):1000.;++trials;
                if(error<spectralError){spectralError=error;spectralBest=candidate;spectralModel=candidateModel;spectralRms=f.rms;}
                if(error<best.bestError){if(best.initialError==1e9)best.initialError=error;best.bestError=error;best.parameters=candidate;best.model=candidateModel;best.valid=true;}
                if(progress)progress(trials,budget,best.bestError);
            }
            best.spectralError=spectralError;
            if(best.model){best.parameters=spectralBest;best.parameters.master=std::clamp(-6.f+20*std::log10(std::max(ref.rms,1e-9f)/std::max(spectralRms,1e-9f)),-36.f,0.f);}
        }
    }
    best.profile=profile;best.trials=trials;best.cancelled=cancel.load();
    if(best.valid&&!best.cancelled){Engine renderer;renderer.configure(best.parameters);renderer.prepare(target.rate);renderer.setModel(best.model.get());std::vector<float> l(target.left.size()),r(l.size());renderCandidate(renderer,best.parameters,target,l,r);best.comparison=compareSignals(target,l,r);}
    return best;
}
}
