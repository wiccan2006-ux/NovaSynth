#pragma once
#include <algorithm>
#include <cmath>
#include <vector>
namespace nova {
// Offline comparison proxy: 38 Hz high-pass + 4 dB shelf, relative block gate.
// Deliberately not labelled LUFS or a certified BS.1770 measurement.
struct QualityLevel { double weightedRms=0,rms=0,peak=0; };
inline QualityLevel qualityLevel(const std::vector<float>& l,const std::vector<float>& r,double rate){
 QualityLevel out;if(l.empty()||l.size()!=r.size()||rate<8000)return out;
 struct Biquad{double b0,b1,b2,a1,a2,z1=0,z2=0;double tick(double x){double y=b0*x+z1;z1=b1*x-a1*y+z2;z2=b2*x-a2*y;return y;}};
 double w=2*3.141592653589793*1500/rate,A=std::pow(10.,4./40),c=std::cos(w),s=std::sin(w),alpha=s/2*std::sqrt(2.),q=2*std::sqrt(A)*alpha,a0=(A+1)-(A-1)*c+q;
 Biquad shelf{A*((A+1)+(A-1)*c+q)/a0,-2*A*((A-1)+(A+1)*c)/a0,A*((A+1)+(A-1)*c-q)/a0,2*((A-1)-(A+1)*c)/a0,((A+1)-(A-1)*c-q)/a0},filters[2]={shelf,shelf};
 double pole=std::exp(-2*3.141592653589793*38/rate),old[2]={},hp[2]={},energy=0,block=0;int count=0,blockSize=std::max(1,static_cast<int>(rate*.1));std::vector<double> blocks;
 for(size_t i=0;i<l.size();++i){for(int ch=0;ch<2;++ch){double x=ch?r[i]:l[i];if(!std::isfinite(x))return {};out.peak=std::max(out.peak,std::abs(x));energy+=x*x*.5;hp[ch]=x-old[ch]+pole*hp[ch];old[ch]=x;double y=filters[ch].tick(hp[ch]);block+=y*y*.5;}if(++count==blockSize){blocks.push_back(block/count);block=0;count=0;}}
 if(count)blocks.push_back(block/count);out.rms=std::sqrt(energy/l.size());double maximum=blocks.empty()?0:*std::max_element(blocks.begin(),blocks.end()),sum=0;count=0;for(double b:blocks)if(b>maximum*.001&&b>1e-12){sum+=b;++count;}out.weightedRms=std::sqrt(sum/std::max(1,count));return out;
}
}
