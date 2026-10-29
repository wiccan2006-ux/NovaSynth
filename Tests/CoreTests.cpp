#include "AutoFit.h"
#include <iostream>
#include <set>
#include <stdexcept>
void check(bool ok,const char* m){if(!ok)throw std::runtime_error(m);}
int main(){try{
 std::set<std::string> names;
 for(int index=0;index<64;++index){check(names.insert(nova::names()[index]).second,"unique names");nova::Engine e;e.configure(nova::preset(index));e.prepare(48000);e.noteOn(1,60,.8f);std::array<float,256> l{},r{};double energy=0;for(int b=0;b<150;++b){e.render(l.data(),r.data(),256);for(int i=0;i<256;++i){check(std::isfinite(l[i])&&std::isfinite(r[i]),"finite preset");energy+=l[i]*l[i];}}check(energy>1e-5,"audible preset");}
 nova::Engine e;nova::Params p;p.delay=p.feedback=0;p.release=.02f;e.configure(p);e.prepare(22050);
 for(int n=20;n<50;++n)e.noteOn(1,n,.7f);
 check(e.activeVoices()==16,"voice limit");e.controller(1,120,0);check(e.activeVoices()==0,"all sound off");
 std::array<float,1024> l{},r{};e.noteOn(1,60,1);e.render(l.data(),r.data(),1024);e.controller(1,64,127);e.noteOff(1,60);e.render(l.data(),r.data(),1024);check(e.activeVoices()==1,"pedal hold");e.controller(1,64,0);e.render(l.data(),r.data(),1024);check(e.activeVoices()==0,"release");
 nova::Sound sound;sound.sampleRate=48000;sound.left.resize(24000);sound.right.resize(24000);for(int i=0;i<24000;++i)sound.left[i]=sound.right[i]=static_cast<float>(.4*std::sin(2*nova::pi*220*i/48000));nova::analyse(sound);check(sound.tonal&&std::abs(sound.estimatedHz-220)<3,"pitch detection");auto resampled=nova::makeTarget(sound,69);check(resampled.note==69&&std::abs(resampled.fine)<3&&resampled.left.size()>10000,"manual root and reference resampling");
 nova::FitTarget target;target.note=60;target.left.resize(22050);target.right=target.left;target.gate=.7;
 p=nova::preset(26);p.waveA=2;p.mix=.17f;p.cutoff=1750;p.envAmount=1.3f;p.delay=.18f;p.chorus=.2f;p.eqHigh=-3;p.reverb=.16f;
 nova::renderCandidate(e,p,target,target.left,target.right);auto expected=target.left;p.mode=1;nova::renderCandidate(e,p,target,target.left,target.right);check(target.left==expected,"sampler disabled even with legacy mode flag");
 nova::FeatureExtractor extractor;auto f=extractor.extract(target.left,target.right,target.rate);check(nova::featureDistance(f,f)==0,"identical feature loss");
 std::atomic<bool> stop{false};double last=1000,seedLoss=1000;int updates=0;
 auto fit=nova::fitParameters(target,nova::preset(0),260,stop,[&](int trial,int,double best){if(trial==94)seedLoss=best;check(best<=last+1e-10,"monotonic best error");last=best;++updates;});
 std::cout<<"Search: "<<fit.initialError<<" -> "<<fit.bestError<<" (best seed "<<seedLoss<<") in "<<fit.trials<<" trials\n";
 check(fit.valid&&!fit.cancelled&&fit.trials==260&&updates==260,"search completion");check(fit.bestError<fit.initialError*.85,"off-preset fit improves");check(fit.bestError<seedLoss*.98,"mutation improves beyond bank and oscillator seeds");check(fit.parameters.mode==0&&fit.parameters.waveA<5&&fit.parameters.waveB<5,"procedural patch only");
 stop.store(true);auto cancelled=nova::fitParameters(target,p,500,stop);check(cancelled.cancelled&&cancelled.trials<=2,"cancellation");
 nova::Sound silent;silent.left.assign(10000,0);silent.right=silent.left;check(nova::makeTarget(silent,60).left.empty(),"silence rejected");
 p=nova::preset(1);p.delay=p.reverb=p.drive=p.chorus=p.eqLow=p.eqHigh=0;
 std::vector<float> dry(22050),dryR(22050);nova::renderCandidate(e,p,target,dry,dryR);
 for(int fx=0;fx<5;++fx){auto wet=p;if(fx==0)wet.drive=.6f;if(fx==1)wet.chorus=.5f;if(fx==2)wet.delay=.5f;if(fx==3)wet.reverb=.5f;if(fx==4)wet.eqHigh=8;std::vector<float> a(22050),b(22050);nova::renderCandidate(e,wet,target,a,b);double delta=0;for(size_t i=0;i<a.size();++i){check(std::isfinite(a[i])&&std::isfinite(b[i]),"effects finite");delta+=std::abs(a[i]-dry[i]);}check(delta>.001,"effect path changes audio");wet.driveOn=wet.chorusOn=wet.delayOn=wet.reverbOn=wet.eqOn=false;nova::renderCandidate(e,wet,target,a,b);check(a==dry,"effect bypass");}
 for(double rate:{22050.,44100.,48000.,96000.}){p=nova::preset(63);p.cutoff=20000;p.resonance=1;e.configure(p);e.prepare(rate);e.noteOn(1,120,1);e.render(l.data(),r.data(),1024);for(float x:l)check(std::isfinite(x)&&std::abs(x)<8,"high note stability");}
 std::cout<<"PASS: 64 presets, MIDI, pitch, no PCM playback, optimization, cancellation, 5 effects/bypass, 4 sample rates\n";return 0;
}catch(const std::exception& ex){std::cerr<<"FAIL: "<<ex.what()<<'\n';return 1;}}
