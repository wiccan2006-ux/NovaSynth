#pragma once
#include "Parameters.h"
#include <array>
#include <vector>
#include <cmath>
#include <algorithm>

namespace nova {
// The same effects code is used by offline parameter search and the live synth.
class Effects {
    struct Comb {std::vector<float> data;size_t position=0;float damp=0;};
    double sr=48000,phase=0;
    std::vector<float> echoL,echoR,chorusL,chorusR;
    std::array<Comb,8> combs;
    size_t echoPosition=0,chorusPosition=0;
    std::array<float,2> low{},upper{};
    Params p;
    int echoSamples=12000;
    float lowCoef=.03f,upperCoef=.3f,lowGain=1,highGain=1,driveGain=1,driveNorm=1;
    static float read(const std::vector<float>& ring,size_t pos,double delay) {
        double x=static_cast<double>(pos)+ring.size()-delay;
        x=std::fmod(x,static_cast<double>(ring.size()));if(x<0)x+=ring.size();
        size_t a=static_cast<size_t>(x),b=(a+1)%ring.size();float t=static_cast<float>(x-a);
        return ring[a]*(1-t)+ring[b]*t;
    }
public:
    void prepare(double rate) {
        sr=rate;echoL.resize(static_cast<size_t>(sr*2)+2);echoR=echoL;
        chorusL.resize(static_cast<size_t>(sr*.06)+2);chorusR=chorusL;
        constexpr int lengths[]={1116,1188,1277,1356,1139,1211,1300,1379};
        for(int i=0;i<8;++i)combs[i].data.resize(std::max(8,static_cast<int>(lengths[i]*sr/44100)));
        lowCoef=static_cast<float>(1-std::exp(-2*3.141592653589793*250/sr));
        upperCoef=static_cast<float>(1-std::exp(-2*3.141592653589793*3500/sr));
        configure(p);reset();
    }
    void configure(const Params& parameters) {
        p=parameters;echoSamples=std::max(1,static_cast<int>(std::clamp(p.delayTime,.04f,1.5f)*sr));
        lowGain=std::pow(10.f,p.eqLow/20);highGain=std::pow(10.f,p.eqHigh/20);
        driveGain=1+p.drive*8;driveNorm=std::sqrt(driveGain);
    }
    void reset() {
        phase=0;echoPosition=chorusPosition=0;low={};upper={};
        for(auto* v:{&echoL,&echoR,&chorusL,&chorusR})std::fill(v->begin(),v->end(),0);
        for(auto& c:combs){std::fill(c.data.begin(),c.data.end(),0);c.position=0;c.damp=0;}
    }
    void tick(float& l,float& r) {
        if(echoL.empty())return;
        if(p.driveOn && p.drive>.0001f){l=std::tanh(l*driveGain)/driveNorm;r=std::tanh(r*driveGain)/driveNorm;}
        // Chorus has independent rate and stereo modulation; no source PCM is read.
        chorusL[chorusPosition]=l;chorusR[chorusPosition]=r;
        if(p.chorusOn && p.chorus>.0001f){
            double modulation=std::sin(phase*2*3.141592653589793);
            float cl=read(chorusL,chorusPosition,sr*(.015+.004*modulation));
            float cr=read(chorusR,chorusPosition,sr*(.015-.004*modulation));
            l=l*(1-p.chorus*.45f)+cl*p.chorus*.7f;r=r*(1-p.chorus*.45f)+cr*p.chorus*.7f;
        }
        chorusPosition=(chorusPosition+1)%chorusL.size();phase+=p.chorusRate/sr;phase-=std::floor(phase);
        if(p.eqOn){
            float input[]={l,r};
            for(int c=0;c<2;++c){low[c]+=lowCoef*(input[c]-low[c]);upper[c]+=upperCoef*(input[c]-upper[c]);input[c]+=(lowGain-1)*low[c]+(highGain-1)*(input[c]-upper[c]);}
            l=input[0];r=input[1];
        }
        size_t readPos=(echoPosition+echoL.size()-static_cast<size_t>(echoSamples))%echoL.size();
        float dl=echoL[readPos],dr=echoR[readPos];
        echoL[echoPosition]=l+dr*std::clamp(p.feedback,0.f,.8f);echoR[echoPosition]=r+dl*std::clamp(p.feedback,0.f,.8f);
        echoPosition=(echoPosition+1)%echoL.size();
        if(p.delayOn){l+=dl*p.delay;r+=dr*p.delay;}
        if(p.reverbOn && p.reverb>.0001f){
            float input=(l+r)*.5f,wetL=0,wetR=0;
            float feedback=.62f+std::clamp(p.room,0.f,1.f)*.25f;
            for(int i=0;i<8;++i){auto& c=combs[i];float delayed=c.data[c.position];c.damp+=.4f*(delayed-c.damp);c.data[c.position]=input+c.damp*feedback;c.position=(c.position+1)%c.data.size();if(i<4)wetL+=delayed;else wetR+=delayed;}
            l+=wetL*p.reverb*.2f;r+=wetR*p.reverb*.2f;
        }
    }
};
}
