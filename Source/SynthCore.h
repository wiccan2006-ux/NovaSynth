#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <vector>
#include "Effects.h"
#include "ZoneBank.h"
#include "WaveTables.h"
#include "Modulation.h"

namespace nova {
constexpr double pi=3.14159265358979323846;
constexpr int tableSize=2048,tableLevels=7;
struct Sound {
    double sampleRate=48000;
    int root=60;
    float estimatedHz=0,confidence=0,attack=0.01f,decay=0.3f;
    bool tonal=false;
    std::vector<float> left,right;
};
inline float blep(double phase,double dt) {
    if(phase<dt) { auto t=phase/dt; return static_cast<float>(t+t-t*t-1); }
    if(phase>1-dt) { auto t=(phase-1)/dt; return static_cast<float>(t*t+t+t+1); }
    return 0;
}
inline float oscillator(int wave,double phase,double dt) {
    switch(wave) {
    case 1: return static_cast<float>(2*phase-1)-blep(phase,dt);
    case 2: return (phase<.5?1.f:-1.f)+blep(phase,dt)-blep(std::fmod(phase+.5,1.),dt);
    case 3: { // A band-limited triangle using the first eight odd partials.
        double sum=0;
        for(int h=1,sign=1;h<=15 && h*dt<.48;h+=2,sign=-sign) sum+=sign*std::sin(2*pi*h*phase)/(h*h);
        return static_cast<float>(sum*8/(pi*pi));
    }
    case 4: return static_cast<float>((std::sin(2*pi*phase)+.45*std::sin(4*pi*phase))/.145*0.1);
    default: return static_cast<float>(std::sin(2*pi*phase));
    }
}
struct Voice {
    bool active=false,held=false,deferred=false;
    int note=60,channel=1,stage=0;
    uint64_t age=0;
    double samplePosition=0;
    float amplitude=0,velocity=1,releaseStep=0,attackScale=1,randomValue=0;
    std::array<double,3> phaseA{},phaseB{},phaseSub{};
    double integrator1=0,integrator2=0,g=.1,k=1;
    double modelTime=0;int modelFrame=0,trajectoryClock=0;
    std::array<double,noiseBands> noiseLow{},noiseHigh{};
    double baseHz=440,bankRatio=1;const SpectralModel* sourceModel=nullptr;int zone=-1;
    int delayWrite=0;double delayLength=100,previousDelay=0;
    std::array<float,partialCount> zoneWeights{};
    std::array<double,partialCount> partialSin{},partialCos{},stepSin{},stepCos{};
};
class Engine {
    double sampleRate=48000,lfoPhase=0,modPhase=0;
    uint64_t counter=0;
    uint32_t randomState=0x12345678;
    std::array<Voice,16> voices{};
    std::array<float,16> bends{};
    std::array<bool,16> pedal{};
    Effects effects;
    float smoothedMaster=.25f,smoothedCutoff=5000;
    Params p;
    const SpectralModel* model=nullptr;const ZoneBank* zones=nullptr;
    static constexpr int delayCapacity=8192;std::vector<float> delayMemory;
    std::array<float,partialCount> partialWeights{};
    std::array<double,noiseBands> noiseLoCoeff{},noiseHiCoeff{},noiseNormal{};
    void prepareNoise(){for(int b=0;b<noiseBands;++b){double upper=std::min(sampleRate*.44,model?model->analysisRate*.44:sampleRate*.44),lo=40*std::pow(upper/40,b/static_cast<double>(noiseBands)),hi=40*std::pow(upper/40,(b+1.)/noiseBands);double a=1-std::exp(-2*pi*lo/sampleRate),c=1-std::exp(-2*pi*hi/sampleRate);noiseLoCoeff[b]=a;noiseHiCoeff[b]=c;double variance=a/(2-a)+c/(2-c)-2*a*c/(a+c-a*c);noiseNormal[b]=std::sqrt(3/std::max(1e-8,variance));}}
    void retune(Voice& v,bool resetPhase){
        v.baseHz=440*std::pow(2.,(v.note-69+bends[v.channel-1]+p.fineTune/100.)/12.);v.bankRatio=std::pow(2.,(v.note-p.root+bends[v.channel-1]+p.fineTune/100.)/12.);
        v.delayLength=std::clamp(sampleRate/v.baseHz-.5,2.,static_cast<double>(delayCapacity-3));double integer=std::floor(v.delayLength),fraction=v.delayLength-integer,w=2*pi*v.baseHz/sampleRate;double aphase=std::sin(w*fraction),bphase=std::sin(w*(1-fraction));if(aphase+bphase>1e-10)v.delayLength=integer+aphase/(aphase+bphase);
        const auto* model=v.sourceModel;if(!model)return;for(int j=0;j<model->count;++j)v.zoneWeights[j]=std::pow(std::max(40.f,model->partials[j].frequency)/500.f,p.spectralTone)*p.partialGains[j];
        v.bankRatio=std::pow(2.,(v.note-(v.zone>=0?model->root:p.root)+bends[v.channel-1]+p.fineTune/100.)/12.);double ratio=v.bankRatio;
        for(int j=0;j<model->count;++j){double frequency=model->partials[j].frequency*ratio;double angle=2*pi*std::min(frequency,sampleRate*.49)/sampleRate;v.stepSin[j]=std::sin(angle);v.stepCos[j]=std::cos(angle);if(resetPhase){v.partialSin[j]=std::sin(model->partials[j].phase);v.partialCos[j]=std::cos(model->partials[j].phase);}}
    }
    float random() { randomState^=randomState<<13; randomState^=randomState>>17; randomState^=randomState<<5; return static_cast<float>(randomState/2147483648.0-1); }
    void releaseVoice(Voice& v) { v.held=false; v.deferred=false; v.stage=3; v.releaseStep=v.amplitude/std::max(1.f,p.release*static_cast<float>(sampleRate)); }
    float envelope(Voice& v) {
        switch(v.stage) {
        case 0: v.amplitude+=1/std::max(1.f,p.attack*v.attackScale*static_cast<float>(sampleRate)); if(v.amplitude>=1) {v.amplitude=1;v.stage=1;} break;
        case 1: v.amplitude-=(1-p.sustain)/std::max(1.f,p.decay*static_cast<float>(sampleRate)); if(v.amplitude<=p.sustain) {v.amplitude=p.sustain;v.stage=2;} break;
        case 2: v.amplitude=p.sustain; break;
        case 3: v.amplitude-=std::max(v.releaseStep,1e-9f); if(v.amplitude<=0) {v.amplitude=0;v.active=false;} break;
        }
        return v.amplitude;
    }
public:
    void prepare(double sr) {
        sampleRate=sr;delayMemory.assign(voices.size()*delayCapacity,0);(void)waveTables();(void)signatureTables();effects.prepare(sr);prepareNoise();
        reset(); smoothedMaster=std::pow(10.f,(p.master+p.boost+p.presetTrim)/20); smoothedCutoff=p.cutoff;
    }
    void reset() { voices={};bends.fill(0);pedal.fill(false);lfoPhase=modPhase=0;counter=0;randomState=0x12345678;
        effects.reset();smoothedMaster=std::pow(10.f,(p.master+p.boost+p.presetTrim)/20);smoothedCutoff=p.cutoff; }
    void configure(const Params& value) {bool tune=p.root!=value.root||p.fineTune!=value.fineTune||p.spectralTone!=value.spectralTone||p.partialGains!=value.partialGains;p=value;
        p.cutoff=std::clamp(p.cutoff*std::pow(2.f,3*value.brightness),30.f,20000.f);
        p.fm=std::clamp(p.fm+value.character*.35f,0.f,1.f);p.spectralTone=std::clamp(p.spectralTone+value.character*.4f,-1.f,1.f);
        p.drive=std::clamp(p.drive+value.character*.22f,0.f,1.f);p.lfoDepth=std::clamp(p.lfoDepth+value.movement*.9f,0.f,4.f);
        p.reverb=std::clamp(p.reverb+value.space*.35f,0.f,.7f);p.delay=std::clamp(p.delay+value.space*.2f,0.f,.7f);
        p.mode=0;p.waveA=std::clamp(p.waveA,0,4);p.waveB=std::clamp(p.waveB,0,4);effects.configure(p);if(model)for(int j=0;j<model->count;++j)partialWeights[j]=std::pow(std::max(40.f,model->partials[j].frequency)/500.f,p.spectralTone)*p.partialGains[j];if(tune)for(auto& v:voices)if(v.active)retune(v,false);}
    void setModel(const SpectralModel* value){model=value&&value->valid()?value:nullptr;prepareNoise();reset();configure(p);}
    void setZoneBank(const ZoneBank* value){zones=value&&value->valid()?value:nullptr;reset();}
    int lastZone(int voice=0)const{return voices[std::clamp(voice,0,15)].zone;}
    int activeVoices() const { int n=0;for(const auto& v:voices) if(v.active) ++n;return n; }
    void noteOn(int channel,int note,float velocity) {
        Voice* chosen=nullptr;
        for(auto& v:voices) if(!v.active) { chosen=&v;break; }
        if(!chosen) { chosen=&voices[0]; for(auto& v:voices) if(v.age<chosen->age) chosen=&v; }
        *chosen=Voice{}; auto& v=*chosen;v.active=v.held=true;v.channel=std::clamp(channel,1,16);
        v.note=std::clamp(note,0,127);v.velocity=std::clamp(velocity,0.f,1.f);v.age=++counter;v.attackScale=1+std::clamp(p.touch,0.f,1.f)*(1-v.velocity)*3;v.randomValue=static_cast<float>(((v.age*2654435761u+note*97u)%65536)/32767.5-1);
        // Small deterministic phase offsets give unison its stereo width.
        v.phaseA={0,.19,.63};v.phaseB={.25,.49,.81};v.phaseSub={0,.19,.63};
        v.sourceModel=model;if(p.useZones&&zones&&p.engine==1){v.zone=zones->select(v.note,v.velocity);if(v.zone>=0)v.sourceModel=&zones->zones[v.zone].model;}
        retune(v,true);
        if((p.engine==3||p.engine==4)&&!delayMemory.empty()){
            auto* data=delayMemory.data()+(chosen-voices.data())*delayCapacity;std::fill(data,data+delayCapacity,0.f);int n=static_cast<int>(std::ceil(v.delayLength))+2;double mean=0;
            for(int i=0;i<n;++i){double phase=std::fmod((i-n)/std::max(2.,sampleRate/v.baseHz)+2.,1.),pluck=.2+.5*p.fm,triangle=phase<pluck?phase/pluck:(1-phase)/(1-pluck);data[delayCapacity-n+i]=p.engine==3?static_cast<float>(.55*(triangle-.5)+random()*.01):0;mean+=data[delayCapacity-n+i];}mean/=n;for(int i=0;i<n;++i)data[delayCapacity-n+i]-=static_cast<float>(mean);
        }
    }
    void noteOff(int channel,int note) {
        for(auto& v:voices) if(v.active && v.channel==channel && v.note==note && v.held) {
            v.held=false; if(pedal[channel-1]) v.deferred=true;else releaseVoice(v);
        }
    }
    void pitchWheel(int channel,int value) { bends[std::clamp(channel,1,16)-1]=(value-8192)/8192.f*2;for(auto& v:voices)if(v.active&&v.channel==channel)retune(v,false); }
    void controller(int channel,int cc,int value) {
        channel=std::clamp(channel,1,16);
        if(cc==64) { pedal[channel-1]=value>=64; if(value<64) for(auto& v:voices) if(v.channel==channel && v.deferred) releaseVoice(v); }
        if(cc==120 || cc==123) for(auto& v:voices) if(v.channel==channel) { if(cc==120) v.active=false;else releaseVoice(v); }
    }
    void render(float* left,float* right,int count) {
        const float masterTarget=std::pow(10.f,(std::clamp(p.master,-36.f,0.f)+std::clamp(p.boost,-12.f,12.f)+std::clamp(p.presetTrim,-12.f,36.f))/20);
        const double detunes[]={1,std::pow(2.,-p.detune/1200.),std::pow(2.,p.detune/1200.)};const double transpose=std::pow(2.,p.transpose/12.);
        for(int i=0;i<count;++i) {
            float l=0,r=0;
            double lfo=std::sin(2*pi*lfoPhase);lfoPhase+=p.lfoRate/sampleRate;lfoPhase-=std::floor(lfoPhase);double lfo2=std::sin(2*pi*modPhase);modPhase+=std::clamp(p.modRate,.03f,15.f)/sampleRate;modPhase-=std::floor(modPhase);
            smoothedCutoff+=.002f*(p.cutoff-smoothedCutoff);
            for(auto& v:voices) if(v.active) {
                float env=envelope(v); if(!v.active) continue;
                auto mods=modulation(p,static_cast<float>(lfo),static_cast<float>(lfo2),env,v.velocity,v.note,v.randomValue);float vl=0,vr=0;
                        const auto* model=v.sourceModel;
                if(p.engine==1 && model){
                    while(v.modelFrame<modelFrames-2 && v.modelTime>model->times[v.modelFrame+1])++v.modelFrame;
                    int frame=v.modelFrame;float t=std::clamp(static_cast<float>((v.modelTime-model->times[frame])/std::max(.0001f,model->times[frame+1]-model->times[frame])),0.f,1.f);
                    double value=0,ratio=v.bankRatio;
                    bool update=v.trajectoryClock++%32==0;
                    for(int j=0;j<model->count;++j){const auto& part=model->partials[j];float amp=part.amplitude[frame]*(1-t)+part.amplitude[frame+1]*t;
                        double motion=model->tracked?1+p.tracking*((part.frequencyRatio[frame]*(1-t)+part.frequencyRatio[frame+1]*t)-1):1;
                        double frequency=part.frequency*ratio*motion;
                        if(frequency<sampleRate*.45)value+=v.partialSin[j]*amp*(v.zone>=0?v.zoneWeights[j]:partialWeights[j]);
                        if(update){double angle=2*pi*std::min(frequency,sampleRate*.49)/sampleRate;v.stepSin[j]=std::sin(angle);v.stepCos[j]=std::cos(angle);double norm=1/std::sqrt(std::max(1e-12,v.partialSin[j]*v.partialSin[j]+v.partialCos[j]*v.partialCos[j]));v.partialSin[j]*=norm;v.partialCos[j]*=norm;}
                        double sine=v.partialSin[j]*v.stepCos[j]+v.partialCos[j]*v.stepSin[j];v.partialCos[j]=v.partialCos[j]*v.stepCos[j]-v.partialSin[j]*v.stepSin[j];v.partialSin[j]=sine;
                    }
                    float white=random();double residual=0;
                    if(p.residual>0)for(int b=0;b<noiseBands;++b){float amp=model->noise[b][frame]*(1-t)+model->noise[b][frame+1]*t;if(amp>1e-8){v.noiseLow[b]+=noiseLoCoeff[b]*(white-v.noiseLow[b]);v.noiseHigh[b]+=noiseHiCoeff[b]*(white-v.noiseHigh[b]);residual+=(v.noiseHigh[b]-v.noiseLow[b])*noiseNormal[b]*amp;}}
                    vl=vr=static_cast<float>(value+residual*p.residual)+white*p.noise*static_cast<float>(std::exp(-v.modelTime/.04));v.modelTime+=1/(sampleRate*std::max(.25f,p.spectralTime));
                }else if(p.engine==2){
                    double dt=std::min(.45,v.baseHz/sampleRate);float scan=std::clamp(p.fm+static_cast<float>(v.modelTime)*p.mix*.25f,0.f,1.f);
                    float a=waveTables().read(p.waveA,v.phaseA[0],dt),b=waveTables().read(p.waveB,v.phaseA[0],dt);
                    if(p.palette>0){a=signatureTables().read(p.palette-1,0,v.phaseA[0],dt);b=signatureTables().read(p.palette-1,1,v.phaseA[0],dt);}
                    if(p.movement>0){scan=std::clamp(p.fm+p.movement*static_cast<float>(.35*lfo+.25*(1-env))+.15f*p.mix*env,0.f,1.f);}
                    scan=std::clamp(scan+mods.morph,0.f,1.f);
                    float side=p.movement*p.spread*.12f*static_cast<float>(std::sin(2*pi*(lfoPhase+.25)));
                    vl=a*(1-std::clamp(scan-side,0.f,1.f))+b*std::clamp(scan-side,0.f,1.f);vr=a*(1-std::clamp(scan+side,0.f,1.f))+b*std::clamp(scan+side,0.f,1.f);v.phaseA[0]+=dt;v.phaseA[0]-=std::floor(v.phaseA[0]);v.modelTime+=1/sampleRate;
                }else if((p.engine==3||p.engine==4)&&!delayMemory.empty()){
                    auto* data=delayMemory.data()+(&v-voices.data())*delayCapacity;
                    double pos=v.delayWrite-v.delayLength;while(pos<0)pos+=delayCapacity;int a=static_cast<int>(pos),b=(a+1)%delayCapacity;double t=pos-a;
                    double delayed=data[a]*(1-t)+data[b]*t,filtered=(delayed+v.previousDelay)*.5;v.previousDelay=delayed;
                    double feedback=.985+.014*std::clamp(static_cast<double>(p.resonance),0.,1.);
                    double drive=0;if(p.engine==4){double phase=v.phaseA[0];drive=v.held?(.018*std::sin(2*pi*phase)+random()*(.002+p.noise*.02)):0;v.phaseA[0]+=v.baseHz/sampleRate;v.phaseA[0]-=std::floor(v.phaseA[0]);}
                    data[v.delayWrite]=static_cast<float>(p.engine==3?filtered*feedback:std::tanh(filtered*feedback+drive));v.delayWrite=(v.delayWrite+1)%delayCapacity;vl=vr=static_cast<float>(delayed*(p.engine==4?2:1.4));
                }else if(p.engine==0){
                    double hz=v.baseHz;
                    int copies=p.unison?3:1;
                    for(int u=0;u<copies;++u) {
                        double dt=std::min(.45,hz*detunes[u]/sampleRate);
                        double dtB=std::min(.45,dt*transpose);
                        float b=oscillator(p.waveB,v.phaseB[u],dtB);
                        double phase=v.phaseA[u]+b*std::clamp(p.fm+mods.morph,0.f,1.f)*.15;phase-=std::floor(phase);
                        float a=oscillator(p.waveA,phase,dt);
                        float x=a*(1-p.mix)+b*p.mix+static_cast<float>(std::sin(2*pi*v.phaseSub[u]))*p.sub;
                        v.phaseA[u]+=dt;v.phaseA[u]-=std::floor(v.phaseA[u]);
                        v.phaseB[u]+=dtB;v.phaseB[u]-=std::floor(v.phaseB[u]);
                        v.phaseSub[u]+=dt*.5;v.phaseSub[u]-=std::floor(v.phaseSub[u]);
                        float pan=u==0?0:(u==1?-p.spread:p.spread);
                        vl+=x*std::sqrt((1-pan)*.5f)/copies;vr+=x*std::sqrt((1+pan)*.5f)/copies;
                    }
                    float n=random()*p.noise;vl+=n;vr+=n;
                }
                vl*=mods.amplitude;vr*=mods.amplitude;if(mods.pan!=0){vl*=std::sqrt(1-mods.pan);vr*=std::sqrt(1+mods.pan);}
                if(p.engine==1&&p.cutoff>=19000&&p.resonance<.001f&&p.envAmount==0&&p.lfoDepth==0&&mods.cutoff==0&&p.touch==0){l+=vl*env*v.velocity*.3f;r+=vr*env*v.velocity*.3f;continue;}
                // One resonant low-pass per voice, then width from the stereo source.
                double cutoff=std::clamp(smoothedCutoff*std::pow(2.,p.envAmount*env+p.lfoDepth*lfo+(p.velocityTone+p.touch)*(v.velocity-.65f)*3+mods.cutoff),20.,sampleRate*.42);
                v.g=std::tan(pi*cutoff/sampleRate);v.k=1/(.55+p.resonance*7.5);
                double input=(vl+vr)*.5;
                double high=(input-(v.k+v.g)*v.integrator1-v.integrator2)/(1+v.k*v.g+v.g*v.g);
                double band=v.g*high+v.integrator1,low=v.g*band+v.integrator2;
                v.integrator1=v.g*high+band;v.integrator2=v.g*band+low;
                // The side signal follows the same overall envelope; attenuate at low cutoff.
                float side=(vl-vr)*.5f*static_cast<float>(std::min(1.,cutoff/6000.));
                if(p.engine==1&&p.cutoff>=19000&&p.resonance<.001f&&p.envAmount==0&&p.lfoDepth==0&&mods.cutoff==0&&p.touch==0){l+=vl*env*v.velocity*.3f;r+=vr*env*v.velocity*.3f;}else{l+=(static_cast<float>(low)+side)*env*v.velocity*.3f;r+=(static_cast<float>(low)-side)*env*v.velocity*.3f;}
            }
            effects.tick(l,r);
            smoothedMaster+=.001f*(masterTarget-smoothedMaster);
            left[i]=l*smoothedMaster;right[i]=r*smoothedMaster;
        }
    }
};

// Pitch-only reference analysis. Reference PCM is never accessible to Engine.
inline void analyse(Sound& sound) {
    sound.tonal=false;sound.estimatedHz=0;sound.confidence=0;
    if(sound.left.size()<64||sound.left.size()!=sound.right.size()||!std::isfinite(sound.sampleRate)||sound.sampleRate<8000)return;
    double le=0,re=0;for(size_t i=0;i<sound.left.size();i+=16){le+=sound.left[i]*sound.left[i];re+=sound.right[i]*sound.right[i];}
    const auto& source=le>=re?sound.left:sound.right;
    constexpr double sr=12000;size_t count=static_cast<size_t>(source.size()*sr/sound.sampleRate);if(count<560)return;
    std::vector<float> mono(count);
    for(size_t i=0;i<count;++i){double pos=i*sound.sampleRate/sr;size_t a=std::min(source.size()-2,static_cast<size_t>(pos));float t=static_cast<float>(pos-a);mono[i]=source[a]*(1-t)+source[a+1]*t;}
    int N=static_cast<int>(std::min<size_t>(2048,count)),maxLag=std::min(400,N/3),length=N-maxLag;
    struct Window{size_t start;double energy;};std::vector<Window> windows;
    size_t hop=std::max<size_t>(128,N/2);
    for(size_t start=count>2400?180:0;start+N<=count;start+=hop){double energy=0;for(int j=0;j<N;++j)energy+=mono[start+j]*mono[start+j];windows.push_back({start,energy/N});}
    if(windows.empty())return;
    std::sort(windows.begin(),windows.end(),[](const Window& a,const Window& b){return a.energy>b.energy;});
    if(windows.front().energy<1e-7)return;
    std::vector<double> estimates;double confidence=0;
    auto sample=[&](double pos){int k=static_cast<int>(pos);double t=pos-k;auto at=[&](int i){return static_cast<double>(source[static_cast<size_t>(std::clamp(i,0,static_cast<int>(source.size())-1))]);};double a=at(k-1),b=at(k),c=at(k+1),d=at(k+2);return b+.5*t*(c-a+t*(2*a-5*b+4*c-d+t*(3*(b-c)+d-a)));};
    for(size_t w=0;w<std::min<size_t>(7,windows.size());++w){
        if(windows[w].energy<windows.front().energy*.15)continue;
        size_t start=windows[w].start;std::array<double,401> cmnd{};cmnd[0]=1;double sum=0;
        for(int lag=1;lag<=maxLag;++lag){double diff=0;for(int j=0;j<length;++j){double d=mono[start+j]-mono[start+j+lag];diff+=d*d;}sum+=diff;cmnd[lag]=sum>1e-20?diff*lag/sum:1;}
        int best=0;for(int lag=5;lag<maxLag-1;++lag)if(cmnd[lag]<.16&&cmnd[lag]<=cmnd[lag-1]&&cmnd[lag]<cmnd[lag+1]){best=lag;break;}if(!best)continue;
        double den=cmnd[best-1]-2*cmnd[best]+cmnd[best+1],shift=std::abs(den)>1e-12?.5*(cmnd[best-1]-cmnd[best+1])/den:0;
        double period=(best+std::clamp(shift,-.5,.5))*sound.sampleRate/sr;
        // Refine against original-rate samples. Low-rate YIN interpolation alone
        // biases high notes by several cents and can miss correct semitone centres.
        int offset=static_cast<int>(start*sound.sampleRate/sr);
        int points=std::min(static_cast<int>(length*sound.sampleRate/sr),static_cast<int>(source.size())-offset-static_cast<int>(std::ceil(period+sound.sampleRate/sr))-3);
        if(points<64)continue;
        auto error=[&](double lag){double xx=0,yy=0,xy=0;for(int j=0;j<points;++j){double x=source[offset+j],y=sample(offset+j+lag);xx+=x*x;yy+=y*y;xy+=x*y;}return 1-xy/std::sqrt(std::max(1e-30,xx*yy));};
        double radius=sound.sampleRate/sr;for(int iteration=0;iteration<12;++iteration){double left=period-radius/2,right=period+radius/2;double e=error(period),a=error(left),b=error(right);if(a<e&&a<=b)period=left;else if(b<e)period=right;radius*=.5;}
        double hz=sound.sampleRate/period;if(hz>=30&&hz<=2400){estimates.push_back(69+12*std::log2(hz/440));confidence+=1-cmnd[best];}
    }
    if(estimates.empty())return;std::sort(estimates.begin(),estimates.end());double midi=estimates[estimates.size()/2];
    // Reject mixed notes/polyphony instead of asserting a reliable pitch.
    int stable=0;for(double x:estimates)if(std::abs(x-midi)<.45)++stable;if(stable*2<static_cast<int>(estimates.size()))return;
    sound.estimatedHz=static_cast<float>(440*std::pow(2.,(midi-69)/12.));sound.confidence=static_cast<float>(confidence/estimates.size());
    sound.root=std::clamp(static_cast<int>(std::lround(midi)),0,127);sound.tonal=true;
    sound.attack=.005f;sound.decay=std::clamp(static_cast<float>(source.size()/sound.sampleRate*.25),.1f,3.f);
}
} // namespace nova
