#include "AutoFit.h"
#include <iostream>
#include <set>
#include <stdexcept>
#include <cstring>
void check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
int main(){try{
 check(nova::factoryPresetCount==812&&nova::newPresetCount==300,"bank counts");std::set<std::string> names;std::set<uint64_t> hashes;std::vector<nova::Features> features;nova::Engine engine;engine.prepare(22050);nova::FeatureExtractor extractor;double nearest=1000;int close=0;
 for(int i=0;i<nova::factoryPresetCount;++i){check(names.insert(nova::names()[i]).second,"unique name");auto p=nova::preset(i);check(p.waveA>=0&&p.waveA<5&&p.waveB>=0&&p.waveB<5&&(i<764?p.engine==0:p.engine>=0&&p.engine<=4),"procedural waveforms");check(p.cutoff>=30&&p.cutoff<=20000&&p.transpose>=-24&&p.transpose<=24&&p.detune<=35,"valid preset ranges");if(i<64)check(nova::toGenes(p)==nova::toGenes(nova::legacyPreset(i)),"original program IDs preserved");
  nova::FitTarget t;t.rate=22050;t.note=i>=114&&i<139?40:60;t.gate=1;t.left.resize(33075);t.right=t.left;std::vector<float> l(t.left.size()),r(l.size());nova::renderCandidate(engine,p,t,l,r);double energy=0;uint64_t hash=1469598103934665603ull;for(float v:l){check(std::isfinite(v)&&std::abs(v)<4,"finite and bounded patch");energy+=v*v;uint32_t bits;std::memcpy(&bits,&v,4);hash=(hash^bits)*1099511628211ull;}check(energy>1e-5,"audible patch");check(hashes.insert(hash).second,"distinct factory generated audio");if(i>=64){auto f=extractor.extract(l,r,t.rate);for(const auto& previous:features){double d=nova::featureDistance(f,previous);nearest=std::min(nearest,d);if(d<.003)++close;}features.push_back(f);}
 }
 std::cout<<"Nearest feature distance="<<nearest<<" pairs under 0.003="<<close<<"\n";check(nearest>0.00001,"nonidentical spectral features");std::cout<<"PASS: 812 unique names, 748 distinct finite/audible patches, legacy IDs retained; closest new-preset feature distance="<<nearest<<'\n';return 0;
}catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}}
