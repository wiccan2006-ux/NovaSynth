#pragma once
#include "Parameters.h"
#include <algorithm>
#include <cmath>
namespace nova {
struct ModValues {float cutoff=0,morph=0,amplitude=1,pan=0;};
inline ModValues modulation(const Params& p,float lfo1,float lfo2,float envelope,float velocity,int note,float random){
 ModValues out;
 for(const auto& r:p.routes){if(r.source<1||r.source>6||r.destination<1||r.destination>4||!std::isfinite(r.amount))continue;
 float value=0;switch(r.source){case 1:value=lfo1;break;case 2:value=lfo2;break;case 3:value=envelope;break;case 4:value=2*velocity-1;break;case 5:value=std::clamp((note-60)/24.f,-1.f,1.f);break;case 6:value=random;break;}
 value*=std::clamp(r.amount,-1.f,1.f);switch(r.destination){case 1:out.cutoff+=value*4;break;case 2:out.morph+=value;break;case 3:out.amplitude+=value*.8f;break;case 4:out.pan+=value;break;}
 }out.cutoff=std::clamp(out.cutoff,-8.f,8.f);out.morph=std::clamp(out.morph,-1.f,1.f);out.amplitude=std::clamp(out.amplitude,0.f,2.f);out.pan=std::clamp(out.pan,-.95f,.95f);return out;
}
class OutputGuard {
 float gain=1,release=.001f;
public:
 void prepare(double rate){gain=1;release=static_cast<float>(1-std::exp(-1/(std::max(8000.,rate)*.04)));}
 float reduction()const{return -20*std::log10(std::max(.000001f,gain));}
 void tick(float& l,float& r,bool enabled){l=std::isfinite(l)?l:0;r=std::isfinite(r)?r:0;if(!enabled){gain=1;return;}float peak=std::max(std::abs(l),std::abs(r)),wanted=peak>.95f?.95f/peak:1.f;if(wanted<gain)gain=wanted;else gain+=release*(wanted-gain);l*=gain;r*=gain;}
};
}
