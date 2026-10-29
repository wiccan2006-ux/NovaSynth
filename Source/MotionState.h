#pragma once
#include <algorithm>
#include <cmath>
namespace nova {
// UI-only exponential easing. No timer, allocation or access to the audio engine.
inline float easeMotion(float value,float target,double seconds,double timeConstant){
    if(!std::isfinite(seconds)||seconds<=0)return value;
    return target+(value-target)*static_cast<float>(std::exp(-std::min(seconds,.25)/timeConstant));
}
struct MotionState {
    float level=0,activity=0;double phase=0;
    void advance(double seconds,float peak,bool busy,bool enabled){
        peak=std::isfinite(peak)?std::clamp(peak,0.f,1.f):0;
        if(!enabled){level=peak;activity=busy?1.f:0.f;phase=0;return;}
        level=easeMotion(level,peak,seconds,peak>level?.035:.28);
        activity=easeMotion(activity,busy?1.f:0.f,seconds,.14);
        if(std::isfinite(seconds)&&seconds>0)phase=std::fmod(phase+std::min(seconds,.25)*.22,1.);
    }
};
}
