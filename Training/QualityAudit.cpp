#include "AutoFit.h"
#include "PresetTraits.h"
#include "QualityMeter.h"
#include <iostream>
#include <iomanip>
int main(){nova::Engine engine;engine.prepare(22050);nova::FeatureExtractor ex;std::cout<<std::setprecision(9)<<"[\n";
 for(int i=0;i<812;++i){auto p=nova::preset(i);int base=nova::presetRole(i)==0?40:60;int gate=static_cast<int>(22050*std::clamp(p.attack+.6f,1.f,5.6f)),n=gate+17640;double peak=0,sum=0,centralRms=0;std::array<float,161> feature{};
  for(int condition=0;condition<8;++condition){engine.configure(p);engine.reset();int note=base+(condition==0?-12:condition==2||condition>=6?12:0);float vel=condition==3?.35f:condition==4?.6f:condition>=6?1.f:.8f;bool chord=condition>=5;for(int v=0;v<(chord?3:1);++v)engine.noteOn(1,note+std::array<int,3>{0,4,7}[v],vel);std::vector<float> l(n),r(n);engine.render(l.data(),r.data(),gate);for(int v=0;v<(chord?3:1);++v)engine.noteOff(1,note+std::array<int,3>{0,4,7}[v]);engine.render(l.data()+gate,r.data()+gate,n-gate);auto level=nova::qualityLevel(l,r,22050);peak=std::max(peak,level.peak);if(condition==1)centralRms=level.rms;if(condition<3){sum+=std::log(std::max(1e-10,level.weightedRms));auto f=ex.extract(l,r,22050);for(int j=0;j<128;++j)feature[j]+=f.spectrum[j]/3;for(int j=0;j<24;++j)feature[128+j]+=f.envelope[j]/3;for(int j=0;j<8;++j)feature[152+j]+=f.attack[j]/3;feature[160]+=f.width/3;}}
  std::cout<<(i?",\n":"")<<"{\"id\":"<<i<<",\"role\":"<<nova::presetRole(i)<<",\"weighted\":"<<std::exp(sum/3)<<",\"rms\":"<<centralRms<<",\"peak\":"<<peak<<",\"features\":[";for(int j=0;j<161;++j)std::cout<<(j?",":"")<<feature[j];std::cout<<"]}";if(i%80==0)std::cerr<<"Measured "<<i<<"/812\n";
 }std::cout<<"\n]\n";}
