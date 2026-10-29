#include "PresetTraits.h"
#include "CatalogData.h"
#include <iostream>
#include <stdexcept>
void check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
int main(){try{nova::Engine engine;engine.prepare(22050);double quietest=0,loudest=-200;
 for(int i=0;i<812;++i){auto p=nova::preset(i);p.presetTrim=nova::catalog[i].trim;int base=nova::presetRole(i)==0?40:60;int gate=static_cast<int>(22050*std::clamp(p.attack+.6f,1.f,5.6f)),n=gate+17640;
 for(int condition=0;condition<3;++condition){engine.configure(p);engine.reset();int voices=condition?3:1;for(int v=0;v<voices;++v)engine.noteOn(1,base+(condition==2?12:0)+std::array<int,3>{0,4,7}[v],condition==2?1.f:condition?.65f:.8f);std::vector<float> l(n),r(n);engine.render(l.data(),r.data(),gate);for(int v=0;v<voices;++v)engine.noteOff(1,base+(condition==2?12:0)+std::array<int,3>{0,4,7}[v]);engine.render(l.data()+gate,r.data()+gate,n-gate);double energy=0,peak=0;for(int k=0;k<n;++k){check(std::isfinite(l[k])&&std::isfinite(r[k]),"finite calibrated preset");energy+=(l[k]*l[k]+r[k]*r[k])*.5;peak=std::max(peak,static_cast<double>(std::max(std::abs(l[k]),std::abs(r[k]))));}check(peak<=.7002,"calibrated reference peak cap");if(!condition){double db=10*std::log10(energy/n);quietest=std::min(quietest,db);loudest=std::max(loudest,db);}}
 }check(loudest-quietest<14,"calibrated solo RMS spread reduced to below 14 dB with peak protection constraint");std::cout<<"PASS: 812 calibrated presets / 2436 renders, peak <= .7002; solo stereo RMS range "<<quietest<<".."<<loudest<<" dBFS (not LUFS)\n";
}catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}}
