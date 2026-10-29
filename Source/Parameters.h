#pragma once
#include "SpectralModel.h"
namespace nova {
struct ModRoute {int source=0,destination=0;float amount=0;};
struct Params {
    float presetTrim=0,touch=0,modRate=1;
    std::array<ModRoute,4> routes{};
    // Performance macros are neutral at zero; old patches retain their sound.
    float character=0,brightness=0,movement=0,space=0;
    int palette=0;float velocityTone=0;
    int engine=0;bool useZones=false;
    int waveA=1,waveB=0,transpose=0,mode=0,root=60;
    bool unison=true,loop=false;
    bool driveOn=true,chorusOn=true,delayOn=true,reverbOn=true,eqOn=true;
    float mix=.25f,detune=8,sub=.15f,noise=0,fm=0,fineTune=0;
    float cutoff=5000,resonance=.15f,envAmount=2,attack=.01f,decay=.3f,sustain=.6f,release=.4f;
    float lfoRate=2,lfoDepth=0,drive=0,delay=.12f,delayTime=.3f,feedback=.25f,reverb=.12f,spread=.7f,master=-12;
    float chorus=0,chorusRate=.8f,room=.5f,eqLow=0,eqHigh=0;
    float boost=0;
    float spectralTime=1,spectralTone=0,residual=1,tracking=1;
    std::array<float,partialCount> partialGains=[]{std::array<float,partialCount> a{};a.fill(1);return a;}();
};
}
