// Reproducible offline measurements, not a perceptual quality rating.
#include "AutoFit.h"
#include "PresetTraits.h"
#include <fstream>
#include <iostream>
int main(int argc,char** argv){if(argc!=2)return 1;std::ofstream out(argv[1]);out<<"[\n";nova::FeatureExtractor ex;nova::Engine engine;engine.prepare(22050);
 for(int i=0;i<nova::factoryPresetCount;++i){auto p=nova::preset(i);int note=nova::presetRole(i)==0?40:60;int gate=static_cast<int>(22050*std::clamp(p.attack+.6f,1.f,5.6f)),n=gate+17640;double rms[3]{},peaks[3]{};nova::Features feature;
 for(int condition=0;condition<3;++condition){engine.configure(p);engine.reset();int voices=condition?3:1;for(int v=0;v<voices;++v)engine.noteOn(1,note+(condition==2?12:0)+std::array<int,3>{0,4,7}[v],condition==2?1.f:condition?.65f:.8f);std::vector<float> l(n),r(n);engine.render(l.data(),r.data(),gate);for(int v=0;v<voices;++v)engine.noteOff(1,note+(condition==2?12:0)+std::array<int,3>{0,4,7}[v]);engine.render(l.data()+gate,r.data()+gate,n-gate);double energy=0;for(int k=0;k<n;++k){if(!std::isfinite(l[k])||!std::isfinite(r[k]))return 2;energy+=(l[k]*l[k]+r[k]*r[k])*.5;peaks[condition]=std::max(peaks[condition],static_cast<double>(std::max(std::abs(l[k]),std::abs(r[k]))));}rms[condition]=std::sqrt(energy/n);if(!condition)feature=ex.extract(l,r,22050);}
 out<<(i?",\n":"")<<"{\"id\":"<<i<<",\"rms\":"<<rms[0]<<",\"chord\":"<<rms[1]<<",\"peak\":"<<std::max({peaks[0],peaks[1],peaks[2]})<<",\"category\":"<<nova::presetCategory(i)<<",\"role\":"<<nova::presetRole(i)<<",\"features\":[";
 bool first=true;auto values=[&](const auto& array){for(auto v:array){out<<(first?"":",")<<v;first=false;}};values(feature.spectrum);values(feature.envelope);values(feature.attack);out<<","<<feature.width<<"]}";if(i%100==0)std::cerr<<i<<"/812\n";
 }out<<"\n]\n";
}
