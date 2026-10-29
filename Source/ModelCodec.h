#pragma once
#include "SpectralModel.h"
#include "ZoneBank.h"
#include <vector>
#include <cstring>
namespace nova {
// Explicit little-endian wire format; bounded and independent of compiler struct layout.
inline std::vector<uint8_t> encodeModel(const SpectralModel& m){
    if(!m.valid())return {};std::vector<uint8_t> out;
    auto integer=[&](uint32_t n){for(int i=0;i<4;++i)out.push_back(static_cast<uint8_t>(n>>(i*8)));};
    auto real=[&](float x){uint32_t n;std::memcpy(&n,&x,4);integer(n);};
    integer(0x4e535032);integer(m.count);integer(m.root);real(m.duration);real(m.analysisRate);integer(modelFrames);integer(noiseBands);integer(m.tracked?1:0);
    for(float t:m.times)real(t);for(int i=0;i<m.count;++i){const auto& p=m.partials[i];real(p.frequency);real(p.phase);for(float a:p.amplitude)real(a);for(float r:p.frequencyRatio)real(r);}for(const auto& b:m.noise)for(float a:b)real(a);return out;
}
inline bool decodeModel(const void* bytes,size_t length,SpectralModel& result){
    if(!bytes||length<16||length>40000)return false;auto* src=static_cast<const uint8_t*>(bytes);size_t pos=0;
    auto integer=[&](){uint32_t n=0;for(int i=0;i<4;++i)if(pos<length)n|=static_cast<uint32_t>(src[pos++])<<(8*i);return n;};
    auto real=[&](){auto n=integer();float f;std::memcpy(&f,&n,4);return f;};
    uint32_t magic=integer();SpectralModel m;m.count=static_cast<int>(integer());m.root=static_cast<int>(integer());m.duration=real();if(m.count<1||m.count>partialCount)return false;
    if(magic==0x4e535032){
        if(length!=32+modelFrames*4+static_cast<size_t>(m.count)*(8+modelFrames*8)+noiseBands*modelFrames*4)return false;
        m.analysisRate=real();if(integer()!=modelFrames||integer()!=noiseBands)return false;auto flags=integer();if(flags>1)return false;m.tracked=flags!=0;
        for(auto& t:m.times)t=real();for(int i=0;i<m.count;++i){auto& p=m.partials[i];p.frequency=real();p.phase=real();for(auto& a:p.amplitude)a=real();for(auto& r:p.frequencyRatio)r=real();}for(auto& b:m.noise)for(auto& a:b)a=real();
    }else if(magic==0x4e535031){
        if(length!=96+static_cast<size_t>(m.count)*88)return false;std::array<float,20> times{};for(auto& t:times)t=real();float prev=-1;for(float t:times){if(!std::isfinite(t)||t<=prev||t<0||t>5)return false;prev=t;}
        for(int f=0;f<modelFrames;++f){float x=f*19.f/(modelFrames-1);int a=std::min(18,static_cast<int>(x));m.times[f]=times[a]+(times[a+1]-times[a])*(x-a);}
        for(int i=0;i<m.count;++i){auto& p=m.partials[i];p.frequency=real();p.phase=real();std::array<float,20> amps{};for(auto& a:amps){a=real();if(!std::isfinite(a)||a<0||a>8)return false;}for(int f=0;f<modelFrames;++f){float x=f*19.f/(modelFrames-1);int a=std::min(18,static_cast<int>(x));p.amplitude[f]=amps[a]+(amps[a+1]-amps[a])*(x-a);p.frequencyRatio[f]=1;}}
    }else return false;
    if(pos!=length||!m.valid())return false;result=m;return true;
}
}
namespace nova {
inline std::vector<uint8_t> encodeZones(const ZoneBank& bank){
 if(!bank.valid())return {};std::vector<uint8_t> out;auto integer=[&](uint32_t n){for(int i=0;i<4;++i)out.push_back(static_cast<uint8_t>(n>>(8*i)));};integer(0x4e535a31);integer(bank.count);
 for(int i=0;i<bank.count;++i){uint32_t velocity;std::memcpy(&velocity,&bank.zones[i].velocity,4);integer(velocity);auto model=encodeModel(bank.zones[i].model);integer(static_cast<uint32_t>(model.size()));out.insert(out.end(),model.begin(),model.end());}return out;
}
inline bool decodeZones(const void* bytes,size_t length,ZoneBank& result){
 if(!bytes||length<8||length>maxZones*40008+8)return false;auto* data=static_cast<const uint8_t*>(bytes);size_t pos=0;auto integer=[&](){uint32_t n=0;for(int i=0;i<4;++i)if(pos<length)n|=static_cast<uint32_t>(data[pos++])<<(8*i);return n;};if(integer()!=0x4e535a31)return false;ZoneBank bank;bank.count=static_cast<int>(integer());if(bank.count<1||bank.count>maxZones)return false;
 for(int i=0;i<bank.count;++i){if(length-pos<8)return false;auto velocity=integer();std::memcpy(&bank.zones[i].velocity,&velocity,4);auto n=integer();if(n>40000||n>length-pos||!decodeModel(data+pos,n,bank.zones[i].model))return false;pos+=n;}if(pos!=length||!bank.valid())return false;result=bank;return true;
}
}
