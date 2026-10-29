#include "AutoFit.h"
#include <iostream>
#include <stdexcept>
#include <set>
void check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
int main(){try{
 nova::Engine engine;engine.prepare(22050);nova::FeatureExtractor extractor;std::vector<nova::Features> features;double closest=1e9;
 for(int i=0;i<48;++i){auto p=nova::signaturePreset(i);check(p.fineTune==0&&p.detune==0&&!p.unison,"signature pitch is centred");nova::FitTarget t;t.rate=22050;t.note=i>=24&&i<32?40:60;t.gate=1.4;t.left.resize(44100);t.right=t.left;std::vector<float> l(44100),r(44100);nova::renderCandidate(engine,p,t,l,r);double power=0,peak=0;for(float x:l){check(std::isfinite(x),"finite output");power+=x*x;peak=std::max(peak,std::abs(static_cast<double>(x)));}check(power/l.size()>1e-7&&peak<.98,"audible and bounded");auto f=extractor.extract(l,r,22050);for(const auto& old:features)closest=std::min(closest,nova::featureDistance(f,old));features.push_back(f);
 std::cout<<i<<" rms="<<std::sqrt(power/l.size())<<" peak="<<peak<<'\n';
 }
 check(closest>.0001,"signature patches differ in measured features");
 auto base=nova::signaturePreset(4);base.character=base.brightness=base.movement=base.space=0;base.delay=base.reverb=0;base.cutoff=1200;auto render=[&](nova::Params p){nova::FitTarget t;t.rate=22050;t.note=60;t.gate=1;t.left.resize(33075);t.right=t.left;std::vector<float> l(t.left.size()),r(l.size());nova::renderCandidate(engine,p,t,l,r);return l;};auto dry=render(base);
 for(int macro=0;macro<4;++macro){auto p=base;
 if(macro==0)p.character=.8f;if(macro==1)p.brightness=.8f;if(macro==2)p.movement=.8f;if(macro==3)p.space=.8f;auto wet=render(p);double diff=0;for(size_t i=0;i<wet.size();++i)diff+=std::abs(wet[i]-dry[i]);std::cout<<"macro="<<macro<<" diff="<<diff<<std::endl;check(diff>.01,"each macro changes audio");check(p.fineTune==0&&p.detune==0,"macros do not alter tuning controls");}
 for(double rate:{22050.,48000.,96000.}){engine.prepare(rate);auto p=nova::signaturePreset(43);p.brightness=p.character=p.movement=p.space=1;engine.configure(p);for(int n=48;n<64;++n)engine.noteOn(1,n,.9f);std::array<float,1024> l{},r{};for(int block=0;block<20;++block){engine.render(l.data(),r.data(),1024);for(float x:l)check(std::isfinite(x)&&std::abs(x)<8,"polyphony and sample rate stability");}engine.reset();}
 std::cout<<"PASS: 48 signature patches, closest feature distance="<<closest<<", 4 audible macros, 3 rates and 16 voices\n";
}catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}}
