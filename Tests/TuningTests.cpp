#include "AutoFit.h"
#include "Arpeggio.h"
#include <iostream>
#include <stdexcept>
void check(bool ok,const char* m){if(!ok)throw std::runtime_error(m);}
int main(){try{
 std::atomic<bool> cancel{false};for(float detune:{-37.f,37.f}){
  nova::Sound source;source.sampleRate=48000;source.tonal=true;source.root=69;source.estimatedHz=static_cast<float>(440*std::pow(2.,detune/1200));source.left.resize(48000);source.right=source.left;for(size_t i=0;i<source.left.size();++i)source.left[i]=source.right[i]=static_cast<float>(.35*std::sin(2*nova::pi*source.estimatedHz*i/source.sampleRate));auto shortNote=source;shortNote.left.resize(9600);shortNote.right.resize(9600);shortNote.tonal=false;nova::analyse(shortNote);check(shortNote.tonal&&std::abs(shortNote.estimatedHz-source.estimatedHz)<2,"short 200ms note pitch detection");auto target=nova::makeTarget(source,69,22050,false);check(std::abs(target.fine-detune)<.01,"reference cents");
  auto fit=nova::fitParameters(target,nova::preset(0),35,cancel,{},2);check(fit.valid&&fit.model&&fit.parameters.engine==1,"spectral fit");check(std::abs(fit.parameters.fineTune)<.001&&fit.parameters.tracking==0&&fit.parameters.chorus==0,"no added pitch drift");
  nova::Engine e;e.configure(fit.parameters);e.prepare(48000);e.setModel(fit.model.get());e.noteOn(1,69,1);nova::Sound rendered;rendered.sampleRate=48000;rendered.left.resize(48000);rendered.right=rendered.left;e.render(rendered.left.data(),rendered.right.data(),48000);nova::analyse(rendered);check(rendered.tonal&&std::abs(rendered.estimatedHz-440)<2,"detuned source corrected to A440");std::cout<<"Source "<<detune<<" cents -> "<<rendered.estimatedHz<<" Hz\n";
  auto native=nova::fitNativeParameters(target,nova::preset(0),12,cancel);check(native.valid&&std::abs(native.parameters.fineTune)<.001&&!native.parameters.unison&&native.parameters.detune==0&&native.parameters.chorus==0,"native fitting stays centred");check(native.parameters.transpose%12==0,"oscillator pitch constrained to octaves");
 }
 // Native-rate refinement must be accurate for low and high notes, not just A4.
 for(double rate:{8000.,44100.,48000.,96000.})for(double frequency:{55.,110.,261.625565,880.,1760.}){
  nova::Sound tone;tone.sampleRate=rate;tone.left.resize(static_cast<size_t>(rate*.32));tone.right=tone.left;
  double hz=frequency*std::pow(2.,23./1200);for(size_t i=0;i<tone.left.size();++i)tone.left[i]=tone.right[i]=static_cast<float>(.25*std::sin(2*nova::pi*hz*i/rate));nova::analyse(tone);
  double cents=1200*std::log2(tone.estimatedHz/hz);check(tone.tonal&&std::abs(cents)<1.2,"pitch refinement across rates / notes");
 }
 // Two slightly detuned versions of the same harmonic should not retain beating.
 nova::SpectralModel dirty;dirty.count=6;dirty.root=69;dirty.duration=.6;for(int f=0;f<nova::modelFrames;++f)dirty.times[f]=.6f*f/(nova::modelFrames-1);
 for(int j=0;j<6;++j){auto& part=dirty.partials[j];part.frequency=static_cast<float>(440*(j/2+1)*std::pow(2.,(j%2?18.:-17.)/1200));part.amplitude.fill(.4f/(j/2+1));part.frequencyRatio.fill(1);}
 nova::FitTarget ideal;ideal.tonal=true;ideal.note=69;check(nova::tuneModelToNearestNote(dirty,ideal),"harmonic cleaning recognised");check(dirty.count==3,"near duplicate partials merged");
 for(int j=0;j<dirty.count;++j)check(std::abs(dirty.partials[j].frequency-440*(j+1))<.001,"each harmonic tuned independently");
 nova::Params patch;patch.engine=1;patch.root=69;patch.fineTune=0;patch.attack=.001;patch.sustain=1;patch.cutoff=20000;patch.envAmount=0;patch.lfoDepth=0;patch.residual=0;patch.chorus=patch.delay=patch.reverb=patch.drive=0;
 for(int note:{57,69,81}){nova::Engine engine;engine.configure(patch);engine.prepare(48000);engine.setModel(&dirty);engine.noteOn(1,note,1);nova::Sound out;out.sampleRate=48000;out.left.resize(24000);out.right=out.left;engine.render(out.left.data(),out.right.data(),24000);nova::analyse(out);check(out.tonal&&std::abs(1200*std::log2(out.estimatedHz/(440*std::pow(2.,(note-69)/12.))))<1,"clean model tracks keyboard octaves");}
 auto bell=dirty;bell.count=3;for(int j=0;j<3;++j)bell.partials[j].frequency=static_cast<float>(440*std::array<double,3>{1.41,2.37,3.16}[j]);auto original=bell;check(!nova::tuneModelToNearestNote(bell,ideal),"inharmonic instrument not forced into harmonics");for(int j=0;j<3;++j)check(bell.partials[j].frequency==original.partials[j].frequency,"bell partials preserved");
 nova::Sound arp;arp.sampleRate=8000;arp.left.resize(24000);arp.right=arp.left;int notes[]={60,64,67,72};for(size_t i=0;i<arp.left.size();++i){int n=notes[std::min(15,static_cast<int>(i/1500))%4];double hz=440*std::pow(2.,(n-69)/12.)*std::pow(2.,35./1200);arp.left[i]=arp.right[i]=static_cast<float>(.3*std::sin(2*nova::pi*hz*i/8000));}auto pattern=nova::detectArpeggio(arp);double expected=261.625565*std::pow(2.,35./1200);check(pattern.valid&&std::abs(pattern.rootHz-expected)<2,"arpeggio keeps original measured first pitch");std::cout<<"PASS: automatic tuning for sharp/flat references, native settings, first arpeggio pitch\n";return 0;
}catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}}
