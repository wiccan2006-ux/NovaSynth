#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>
namespace nova {
struct SelectionRange {
    double start=0,end=0;
    static constexpr double minimum=.05,maximum=3;
    double length()const{return end-start;}
    bool valid(double duration,double limit=maximum)const{return std::isfinite(start)&&std::isfinite(end)&&start>=0&&end<=duration+1e-8&&length()>=std::min(minimum,duration)-1e-8&&length()<=limit+1e-8;}
    static SelectionRange between(double a,double b,double duration,double limit=maximum){
        if(!std::isfinite(duration)||duration<=0||!std::isfinite(a)||!std::isfinite(b))return {};
        double low=std::min(minimum,duration);if(a>b)std::swap(a,b);a=std::clamp(a,0.,duration);b=std::clamp(b,a,duration);b=std::min(b,a+limit);if(b-a<low){b=std::min(duration,a+low);a=std::max(0.,b-low);}return {a,b};
    }
    void move(double position,double duration,double limit=maximum){if(!std::isfinite(position))return;double size=std::clamp(length(),std::min(minimum,duration),std::min(limit,duration));start=std::clamp(position,0.,std::max(0.,duration-size));end=start+size;}
    void resizeStart(double position,double duration,double limit=maximum){if(std::isfinite(position))start=std::clamp(position,std::max(0.,end-limit),end-std::min(minimum,duration));}
    void resizeEnd(double position,double duration,double limit=maximum){if(std::isfinite(position))end=std::clamp(position,start+std::min(minimum,duration),std::min(duration,start+limit));}
};
struct SampleSpan {int64_t first=0,count=0;};
inline SampleSpan sampleSpan(SelectionRange selection,double rate,int64_t length,double limit=SelectionRange::maximum){
    if(!std::isfinite(rate)||rate<=0||length<1||!selection.valid(length/rate,limit))return {};
    int64_t a=std::clamp(static_cast<int64_t>(std::llround(selection.start*rate)),int64_t(0),length-1),b=std::clamp(static_cast<int64_t>(std::llround(selection.end*rate)),a+1,length);return {a,b-a};
}
struct PeakPair {float low=0,high=0;};
// One extrema pair per 256 source frames: bounded memory, no dropped transients.
class PeakBuilder {
    int filled=0;PeakPair current{};
public:
    static constexpr int stride=256;
    std::vector<PeakPair> peaks;
    void push(float l,float r){if(!std::isfinite(l))l=0;if(!std::isfinite(r))r=0;l=std::clamp(l,-4.f,4.f);r=std::clamp(r,-4.f,4.f);current.low=std::min(current.low,std::min(l,r));current.high=std::max(current.high,std::max(l,r));if(++filled==stride){peaks.push_back(current);current={};filled=0;}}
    void finish(){if(filled){peaks.push_back(current);current={};filled=0;}}
};
}
