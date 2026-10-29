#pragma once
#include "SynthCore.h"
#include <array>
#include <algorithm>
#include <cmath>
namespace nova {
constexpr int arpSteps=16,arpRest=-100;
struct ArpAnalysis {std::array<int,arpSteps> notes{};int root=60,count=0;float bpm=120,rootHz=0;bool valid=false;ArpAnalysis(){notes.fill(arpRest);}};
inline ArpAnalysis detectArpeggio(const Sound& sound){
    ArpAnalysis out;const auto& input=sound.left;double sr=sound.sampleRate,duration=input.size()/sr;if(duration<.4||sr<8000||input.size()!=sound.right.size())return out;
    float peak=0;for(float x:input)peak=std::max(peak,std::abs(x));if(peak<1e-5f)return out;std::array<int,arpSteps> absolute{};std::array<float,arpSteps> frequencies{};absolute.fill(arpRest);
    for(int step=0;step<arpSteps;++step){double midpoint=(step+.5)*duration/arpSteps;int centre=static_cast<int>(midpoint*sr);int half=static_cast<int>(std::min(.045,duration/arpSteps*.38)*sr);if(half<128)continue;int start=std::max(0,centre-half),length=std::min(static_cast<int>(input.size())-start,half*2);if(length<256)continue;double energy=0;for(int i=0;i<length;++i)energy+=input[start+i]*input[start+i];if(std::sqrt(energy/length)<peak*.018)continue;
      int minLag=std::max(2,static_cast<int>(sr/1800)),maxLag=std::min(length/3,static_cast<int>(sr/55));if(maxLag<=minLag+2)continue;double cumulative=0;std::vector<double> cmnd(static_cast<size_t>(maxLag+1),1);
      for(int lag=1;lag<=maxLag;++lag){double diff=0;for(int i=0;i<length-maxLag;++i){double d=input[start+i]-input[start+i+lag];diff+=d*d;}cumulative+=diff;cmnd[lag]=cumulative>1e-14?diff*lag/cumulative:1;}
      int best=0;for(int lag=minLag+1;lag<maxLag;++lag)if(cmnd[lag]<.18&&cmnd[lag]<=cmnd[lag-1]&&cmnd[lag]<cmnd[lag+1]){best=lag;break;}if(!best)continue;
      double denom=cmnd[best-1]-2*cmnd[best]+cmnd[best+1];double shift=std::abs(denom)>1e-12?.5*(cmnd[best-1]-cmnd[best+1])/denom:0;double hz=sr/(best+std::clamp(shift,-.5,.5));int note=static_cast<int>(std::lround(69+12*std::log2(hz/440)));if(note<24||note>108)continue;absolute[step]=note;frequencies[step]=static_cast<float>(hz);++out.count;
    }
    if(out.count<2)return out;for(int i=0;i<arpSteps;++i)if(absolute[i]!=arpRest){out.root=absolute[i];out.rootHz=frequencies[i];break;}int different=0;for(int i=0;i<arpSteps;++i){if(absolute[i]!=arpRest){out.notes[i]=std::clamp(absolute[i]-out.root,-24,24);if(absolute[i]!=out.root)++different;}}if(different==0)return out;out.bpm=std::clamp(static_cast<float>(60*4/duration),30.f,300.f);out.valid=true;return out;
}
}
namespace nova {
struct ArpClock {
 bool held=false,gateOff=true;int root=60,channel=1,sounding=-1,stepIndex=0;float velocity=1;int64_t position=0;
 template<class Off> void start(int ch,int note,float vel,Off off){if(sounding>=0)off(channel,sounding);root=note;channel=ch;velocity=vel;held=true;sounding=-1;stepIndex=0;position=0;gateOff=true;}
 template<class Off> void stop(int ch,int note,Off off){if(held&&ch==channel&&note==root){held=false;if(sounding>=0)off(channel,sounding);sounding=-1;}}
 void reset(){held=false;gateOff=true;sounding=-1;position=0;stepIndex=0;}
 template<class Get,class On,class Off,class Render> void render(int count,double rate,float bpm,Get get,On on,Off off,Render output){
  int offset=0;if(!held){output(0,count);return;}while(offset<count){int64_t length=std::max<int64_t>(1,static_cast<int64_t>(rate*60/(std::clamp(bpm,30.f,300.f)*4)));int64_t gate=std::max<int64_t>(1,length*4/5);if(position>=length){position=0;stepIndex=(stepIndex+1)%arpSteps;}if(position==0){if(sounding>=0)off(channel,sounding);int pitch=get(stepIndex);sounding=pitch==arpRest?-1:std::clamp(root+pitch,0,127);if(sounding>=0)on(channel,sounding,velocity);gateOff=false;}if(position>=gate&&!gateOff){if(sounding>=0)off(channel,sounding);sounding=-1;gateOff=true;}int64_t boundary=gateOff?length:gate;int chunk=static_cast<int>(std::min<int64_t>(count-offset,std::max<int64_t>(1,boundary-position)));output(offset,chunk);offset+=chunk;position+=chunk;}
 }
};
}
