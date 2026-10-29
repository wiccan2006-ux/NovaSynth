#pragma once
#include <array>
#include <cstdint>
#include <cmath>
#include <algorithm>
namespace nova {
constexpr int partialCount=48,modelFrames=64,noiseBands=12;
struct Partial {
    float frequency=0,phase=0;
    std::array<float,modelFrames> amplitude{},frequencyRatio{};
};
struct SpectralModel {
    int count=0,root=60;float duration=1,analysisRate=22050;bool tracked=false;
    std::array<float,modelFrames> times{};
    std::array<Partial,partialCount> partials{};
    std::array<std::array<float,modelFrames>,noiseBands> noise{};
    bool valid()const{
        if(count<1||count>partialCount||root<0||root>127||!std::isfinite(duration)||duration<=0||duration>5||!std::isfinite(analysisRate)||analysisRate<8000||analysisRate>96000)return false;
        float previous=-1;for(float t:times){if(!std::isfinite(t)||t<0||t<=previous||t>5)return false;previous=t;}
        if(std::abs(times.back()-duration)>.001f)return false;
        for(int i=0;i<count;++i){const auto& p=partials[i];if(!std::isfinite(p.frequency)||p.frequency<20||p.frequency>20000||!std::isfinite(p.phase))return false;for(float a:p.amplitude)if(!std::isfinite(a)||a<0||a>8)return false;if(tracked)for(float r:p.frequencyRatio)if(!std::isfinite(r)||r<.5f||r>2)return false;}
        for(const auto& b:noise)for(float a:b)if(!std::isfinite(a)||a<0||a>8)return false;
        return true;
    }
};
}
