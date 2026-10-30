#pragma once
#include "AutoFit.h"
#include "MultiNeural.h"
namespace nova {
inline const char* engineName(int id){static constexpr const char* names[]={"FM / subtractive","Parciales + ruido","Wavetable procedural","Cuerda / guia de ondas","Soplo / resonador"};return names[std::clamp(id,0,4)];}
struct InstrumentCandidate {Params parameters;std::shared_ptr<SpectralModel> model;std::shared_ptr<ZoneBank> bank;std::string label;double error=1e9,rms=0,pitchErrorCents=0;int pitchChecks=0,tonalReferences=0;double attackDistance=0,bodyDistance=0,tailDistance=0;bool valid=false,pitchMeasured=false;};
struct InstrumentReport {
 std::array<InstrumentCandidate,5> candidates{};
 FitReport fit;std::array<double,5> engineErrors{1e9,1e9,1e9,1e9,1e9};std::array<double,4> stageErrors{};bool multiNeuralUsed=false;
};
inline Params cleanCandidate(Params p,int note,bool dry=true){p.root=note;p.fineTune=0;p.tracking=0;p.detune=0;p.sub=0;p.unison=false;p.chorus=0;p.lfoDepth=0;p.transpose=std::clamp(static_cast<int>(std::round(p.transpose/12.))*12,0,24);if(dry){p.drive=p.delay=p.reverb=0;p.eqLow=p.eqHigh=0;}return p;}
inline InstrumentReport fitInstrument(const FitTarget& source,Params initial,int budget,std::atomic<bool>& cancel,const std::function<void(int,int,double)>& progress={},const std::function<void(int)>& stage={},bool neural=true,bool clean=true){
 InstrumentReport report;budget=std::max(100,budget);auto target=centredTarget(source);if(stage)stage(0);FeatureExtractor ex;auto ref=ex.extract(target.left,target.right,target.rate);if(ref.rms<1e-7||target.left.size()<128){report.fit.error="Referencia silenciosa o demasiado breve.";return report;}
 int baseBudget=budget*2/5;if(stage)stage(1);report.fit=fitParameters(target,cleanCandidate(initial,target.note),baseBudget,cancel,[&](int n,int,double loss){if(progress)progress(n,budget,loss);},0,{},neural,clean,true);int trials=report.fit.trials;
 if(report.fit.valid){auto& c=report.candidates[report.fit.parameters.engine];c.parameters=report.fit.parameters;c.model=report.fit.model;c.error=report.fit.bestError;c.valid=true;}
 if(report.fit.valid){report.engineErrors[0]=report.fit.nativeError;report.engineErrors[1]=report.fit.spectralError;}report.stageErrors[0]=report.fit.bestError;
 if(cancel.load()){report.fit.cancelled=true;return report;}
 Engine renderer;renderer.prepare(target.rate);std::vector<float> l(target.left.size()),r(l.size());double best=report.fit.valid?report.fit.bestError:1e9,bestRms=ref.rms;
 auto evaluate=[&](Params candidate,std::shared_ptr<SpectralModel> model){
  if(cancel.load())return 1e9;candidate=cleanCandidate(candidate,target.note,false);renderer.setModel(model.get());renderCandidate(renderer,candidate,target,l,r);auto features=ex.extract(l,r,target.rate);double loss=features.rms>1e-8?featureDistance(ref,features):1000;
  auto& option=report.candidates[candidate.engine];if(features.rms>1e-8&&loss<option.error){option.parameters=candidate;option.model=model;option.error=loss;option.rms=features.rms;option.valid=true;}
  ++trials;report.engineErrors[candidate.engine]=std::min(report.engineErrors[candidate.engine],loss);
  if(loss<best){best=loss;bestRms=features.rms;report.fit.parameters=candidate;report.fit.model=model;report.fit.bestError=loss;report.fit.valid=true;}
  if(progress)progress(trials,budget,best);return loss;
 };
 if(report.fit.valid){renderer.setModel(report.fit.model.get());renderCandidate(renderer,report.fit.parameters,target,l,r);bestRms=ex.extract(l,r,target.rate).rms;report.candidates[report.fit.parameters.engine].rms=bestRms;}
 MultiProposal proposal;if(neural){proposal=multiProposal(target);report.multiNeuralUsed=true;}
 std::array<int,4> order{0,1,2,3};if(neural)std::sort(order.begin(),order.end(),[&](int a,int b){return proposal.priority[a]>proposal.priority[b];});
 std::array<Params,4> winners{};std::array<double,4> losses{1e9,1e9,1e9,1e9};std::mt19937 rng(0x4e533130);std::normal_distribution<float> normal(0,1);
 for(int cls:order){int engine=std::array<int,4>{0,2,3,4}[cls];Params p=neural?fromGenes(proposal.genes[cls],target.note):Params{};p.engine=engine;p=cleanCandidate(p,target.note);if(engine>=3){p.attack=engine==3?.001f:.025f;p.sustain=1;p.cutoff=14000;p.resonance=.8f;p.envAmount=0;p.noise=.02f;}
  if(report.fit.parameters.engine==engine)p=report.fit.parameters;winners[cls]=p;losses[cls]=evaluate(p,nullptr);
 }
 int bodyEnd=budget*3/4;
 while(trials<bodyEnd&&!cancel.load()){
  int cls=order[static_cast<size_t>(trials)%4],engine=std::array<int,4>{0,2,3,4}[cls];Params p=winners[cls];auto g=toGenes(p);int knob=std::array<int,10>{0,1,4,8,9,10,11,7,14,13}[rng()%10];g[knob]+=normal(rng)*.16f;g[knob]=std::clamp(g[knob],0.f,1.f);p=cleanCandidate(fromGenes(g,target.note),target.note);p.engine=engine;double loss=evaluate(p,nullptr);if(loss<losses[cls]){winners[cls]=p;losses[cls]=loss;}
 }
 report.stageErrors[1]=best;if(stage)stage(2);
 int envelopeEnd=budget*9/10;
 while(trials<envelopeEnd&&!cancel.load()){
  auto p=report.fit.parameters;auto model=report.fit.model;int knob=rng()%5;float delta=normal(rng)*.12f;
  if(knob==0)p.attack=std::clamp(p.attack*std::exp(delta*2),.001f,2.f);
  if(knob==1)p.decay=std::clamp(p.decay*std::exp(delta*2),.005f,3.f);
  if(knob==2)p.sustain=std::clamp(p.sustain+delta,0.f,1.f);
  if(knob==3)p.release=std::clamp(p.release*std::exp(delta*2),.005f,4.f);
  if(knob==4&&p.engine==1)p.spectralTime=std::clamp(p.spectralTime+delta,.7f,1.4f);
  evaluate(p,model);
 }
 report.stageErrors[2]=best;if(stage)stage(3);
 while(trials<budget&&!cancel.load()){
  auto p=report.fit.parameters;auto model=report.fit.model;float delta=normal(rng)*.08f;switch(rng()%6){case 0:p.drive=std::clamp(p.drive+delta,0.f,.5f);break;case 1:p.reverb=std::clamp(p.reverb+delta,0.f,.4f);break;case 2:p.delay=std::clamp(p.delay+delta,0.f,.3f);break;case 3:p.eqLow=std::clamp(p.eqLow+delta*15,-9.f,9.f);break;case 4:p.eqHigh=std::clamp(p.eqHigh+delta*15,-9.f,9.f);break;case 5:p.residual=std::clamp(p.residual+delta,0.f,1.5f);}
  p.driveOn=p.delayOn=p.reverbOn=p.eqOn=true;evaluate(p,model);
 }
 report.stageErrors[3]=best;report.fit.trials=trials;report.fit.cancelled=cancel.load();report.fit.neuralUsed=report.fit.neuralUsed||report.multiNeuralUsed;
 if(report.fit.valid&&!report.fit.cancelled){auto& p=report.fit.parameters;p.master=static_cast<float>(std::clamp(p.master+20*std::log10(std::max(ref.rms,1e-9f)/std::max(bestRms,1e-9)),-36.,0.));renderer.setModel(report.fit.model.get());renderCandidate(renderer,p,target,l,r);report.fit.comparison=compareSignals(target,l,r);}
 if(!report.fit.cancelled)for(auto& c:report.candidates)if(c.valid){
  if(cancel.load()){report.fit.cancelled=true;break;}
  c.parameters.master=static_cast<float>(std::clamp(c.parameters.master+20*std::log10(std::max(ref.rms,1e-9f)/std::max(c.rms,1e-9)),-36.,0.));
  if(target.tonal){renderer.setModel(c.model.get());renderCandidate(renderer,c.parameters,target,l,r);Sound probe;probe.sampleRate=target.rate;probe.left=l;probe.right=r;analyse(probe);
   if(probe.tonal&&probe.confidence>.85f){c.pitchMeasured=true;c.pitchErrorCents=1200*std::log2(probe.estimatedHz/(440*std::pow(2.,(target.note-69)/12.)));if(std::abs(c.pitchErrorCents)>65)c.valid=false;}
  }
 }
 if(report.fit.valid&&!report.fit.cancelled&&!report.candidates[report.fit.parameters.engine].valid){report.fit.valid=false;report.fit.error="La verificacion final detecto una nota/octava incorrecta. Se conserva el patch anterior. Revisa la nota de referencia o selecciona una nota mas estable.";}

 return report;
}
}
