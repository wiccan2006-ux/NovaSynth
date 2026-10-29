#pragma once
#include "ContextAnalysis.h"
#include "Diagnostics.h"
namespace nova {
struct MusicalComparison {
    Sound original,recreated;std::array<float,256> waveA{},waveB{},envelopeA{},envelopeB{};
    int events=0,measured=0,pitchMeasured=0;double attack=0,body=0,envelope=0,maxPitchCents=0;bool valid=false,cancelled=false;std::string error;
};
inline MusicalComparison compareSequence(Sound source,std::vector<ContextNote> notes,Params patch,const SpectralModel* model,const ZoneBank* bank,bool match,std::atomic<bool>& cancel,const std::function<void(float)>& progress={}){
    MusicalComparison out;if(source.left.size()!=source.right.size()||source.left.empty()||source.sampleRate<8000||source.sampleRate>96000||source.left.size()/source.sampleRate>60.001){out.error="Referencia invalida para comparar.";return out;}
    if(notes.empty()||notes.size()>512){out.error="Revisa primero un fragmento con notas tonales estables.";return out;}
    struct Event{int64_t sample;int note;float velocity;bool on;};std::vector<Event> events;const int64_t length=static_cast<int64_t>(source.left.size());
    for(const auto& n:notes){if(!std::isfinite(n.range.start)||!std::isfinite(n.range.end)||n.note<24||n.note>108||n.range.end<=n.range.start||n.centsSpread>75||n.confidence<.87f)continue;auto a=std::clamp<int64_t>(static_cast<int64_t>(std::llround(n.range.start*source.sampleRate)),0,length),b=std::clamp<int64_t>(static_cast<int64_t>(std::llround(n.range.end*source.sampleRate)),0,length);if(b<=a)continue;events.push_back({a,n.note,n.velocity,true});events.push_back({b,n.note,0,false});++out.events;}
    if(events.empty()){out.error="No hay eventos tonales aprovechables.";return out;}
    std::sort(events.begin(),events.end(),[](const Event& a,const Event& b){return a.sample==b.sample?a.on<b.on:a.sample<b.sample;});out.original=std::move(source);out.recreated.sampleRate=out.original.sampleRate;out.recreated.left.resize(length);out.recreated.right.resize(length);Engine engine;engine.configure(patch);engine.prepare(out.original.sampleRate);engine.setModel(model);engine.setZoneBank(bank);int64_t cursor=0;size_t next=0;
    while(cursor<length){if(cancel.load()){out.cancelled=true;return out;}while(next<events.size()&&events[next].sample<=cursor){const auto& e=events[next++];if(e.on)engine.noteOn(1,e.note,e.velocity);else engine.noteOff(1,e.note);}
        auto boundary=next<events.size()?events[next].sample:length;int count=static_cast<int>(std::min<int64_t>({boundary-cursor,2048,length-cursor}));if(count<=0)continue;engine.render(out.recreated.left.data()+cursor,out.recreated.right.data()+cursor,count);cursor+=count;if(progress)progress(static_cast<float>(cursor)/length*.8f);}
    for(size_t i=0;i<notes.size()&&out.measured<8;++i){if(cancel.load()){out.cancelled=true;return out;}const auto& note=notes[i];if(note.centsSpread>75||note.confidence<.87f)continue;auto range=note.range;range.end=std::min(range.end,range.start+2.);auto span=sampleSpan(range,out.original.sampleRate,length);if(!span.count)continue;Sound a,b;a.sampleRate=b.sampleRate=out.original.sampleRate;a.left.assign(out.original.left.begin()+span.first,out.original.left.begin()+span.first+span.count);a.right.assign(out.original.right.begin()+span.first,out.original.right.begin()+span.first+span.count);b.left.assign(out.recreated.left.begin()+span.first,out.recreated.left.begin()+span.first+span.count);b.right.assign(out.recreated.right.begin()+span.first,out.recreated.right.begin()+span.first+span.count);auto d=diagnose(std::move(a),std::move(b),note.note);out.attack+=d.attackError;out.body+=d.bodyError;out.envelope+=d.envelopeError;if(d.resultTonal){++out.pitchMeasured;out.maxPitchCents=std::max(out.maxPitchCents,std::abs(d.targetErrorCents));}++out.measured;}
    if(out.measured){out.attack/=out.measured;out.body/=out.measured;out.envelope/=out.measured;}if(match){normaliseAudition(out.original);normaliseAudition(out.recreated);}
    auto plots=[](const Sound& sound,auto& wave,auto& env){for(int bin=0;bin<256;++bin){size_t a=bin*sound.left.size()/256,b=(bin+1)*sound.left.size()/256;double energy=0;for(size_t i=a;i<b;++i){wave[bin]=std::max(wave[bin],std::max(std::abs(sound.left[i]),std::abs(sound.right[i])));energy+=(sound.left[i]*sound.left[i]+sound.right[i]*sound.right[i])*.5;}env[bin]=static_cast<float>(std::sqrt(energy/std::max<size_t>(1,b-a)));}};
    plots(out.original,out.waveA,out.envelopeA);plots(out.recreated,out.waveB,out.envelopeB);out.valid=true;if(progress)progress(1);return out;
}
}
