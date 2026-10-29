#pragma once
#include "InstrumentFit.h"
#include "Arpeggio.h"
#include <sstream>
#include <iomanip>
namespace nova {
struct SoundDiagnostic {
    int sourceNote=-1,resultNote=-1,expectedNote=-1;double targetErrorCents=0;
    double sourceHz=0,resultHz=0,sourceCents=0,resultCents=0,confidence=0;
    double attackError=0,bodyError=0,envelopeError=0,sourceRms=0,resultRms=0,clipped=0;
    bool sourceTonal=false,resultTonal=false;std::string warnings;
};
inline double soundRms(const Sound& s){double e=0;size_t n=std::min(s.left.size(),s.right.size());for(size_t i=0;i<n;++i)e+=(s.left[i]*s.left[i]+s.right[i]*s.right[i])*.5;return std::sqrt(e/std::max<size_t>(1,n));}
inline void normaliseAudition(Sound& s,double rms=.12){
    double energy=soundRms(s),peak=0;for(float x:s.left)peak=std::max(peak,static_cast<double>(std::abs(x)));for(float x:s.right)peak=std::max(peak,static_cast<double>(std::abs(x)));if(energy<1e-9||peak<1e-9)return;
    float gain=static_cast<float>(std::min(rms/energy,.92/peak));for(auto* channel:{&s.left,&s.right})for(float& x:*channel)x*=gain;
}
inline Sound renderRecreation(const Sound& source,Params p,const SpectralModel* model,const ZoneBank* zones,bool arp,const std::array<int,arpSteps>& pattern,float bpm,float velocity=.8f,int noteOverride=-1){
    Sound s;s.sampleRate=source.sampleRate;s.left.resize(source.left.size());s.right=s.left;if(s.left.empty())return s;
    Engine engine;engine.configure(p);engine.prepare(s.sampleRate);engine.setModel(model);engine.setZoneBank(zones);int note=noteOverride>=0?noteOverride:source.tonal?source.root:p.root;
    if(arp){ArpClock clock;clock.start(1,note,velocity,[&](int c,int n){engine.noteOff(c,n);});clock.render(static_cast<int>(s.left.size()),s.sampleRate,bpm,[&](int i){return pattern[i];},[&](int c,int n,float v){engine.noteOn(c,n,v);},[&](int c,int n){engine.noteOff(c,n);},[&](int offset,int count){engine.render(s.left.data()+offset,s.right.data()+offset,count);});}
    else {engine.noteOn(1,note,velocity);int gate=(p.engine==1||p.engine==2)?static_cast<int>(s.left.size()):static_cast<int>(s.left.size()*.75);engine.render(s.left.data(),s.right.data(),gate);engine.noteOff(1,note);engine.render(s.left.data()+gate,s.right.data()+gate,static_cast<int>(s.left.size())-gate);}
    return s;
}
inline SoundDiagnostic diagnose(Sound source,Sound result,int expectedNote=-1){
    SoundDiagnostic d;analyse(source);analyse(result);d.sourceTonal=source.tonal;d.resultTonal=result.tonal;d.sourceHz=source.estimatedHz;d.resultHz=result.estimatedHz;d.confidence=source.confidence;
    auto cents=[](double hz){double note=69+12*std::log2(hz/440);return (note-std::round(note))*100;};if(d.sourceTonal)d.sourceCents=cents(d.sourceHz);if(d.resultTonal)d.resultCents=cents(d.resultHz);
    d.sourceNote=d.sourceTonal?source.root:-1;d.resultNote=d.resultTonal?result.root:-1;d.expectedNote=expectedNote>=0?expectedNote:d.sourceNote;
    if(d.resultTonal&&d.expectedNote>=0)d.targetErrorCents=1200*std::log2(d.resultHz/(440*std::pow(2.,(d.expectedNote-69)/12.)));
    d.sourceRms=soundRms(source);d.resultRms=soundRms(result);for(float x:source.left)if(std::abs(x)>=.999f)d.clipped+=1;d.clipped/=std::max<size_t>(1,source.left.size());
    FeatureExtractor ex;auto target=makeTarget(source,source.root,22050,false),rendered=makeTarget(result,source.root,22050,false);target=centredTarget(target);rendered=centredTarget(rendered);auto a=ex.extract(target.left,target.right,target.rate),b=ex.extract(rendered.left,rendered.right,rendered.rate);
    for(int i=0;i<8;++i)d.attackError+=std::abs(a.attack[i]-b.attack[i])/8;
    for(int i=0;i<128;++i)d.bodyError+=std::abs(a.spectrum[i]-b.spectrum[i])/128;
    for(int i=0;i<24;++i)d.envelopeError+=std::abs(a.envelope[i]-b.envelope[i])/24;
    if(!d.sourceTonal)d.warnings+="Fundamental incierta: prueba una nota aislada. ";
    if(d.resultTonal&&d.expectedNote>=0&&std::abs(d.targetErrorCents)>65)d.warnings+="Diferencia de nota/octava o fundamental ambigua. ";
    if(!d.resultTonal)d.warnings+="Recreacion sin fundamental fiable: no se puede verificar la afinacion. ";
    if(d.resultTonal&&std::abs(d.resultCents)>5)d.warnings+="La recreacion no esta centrada: revisa Tune, MIDI y modulaciones. ";
    if(d.clipped>.001)d.warnings+="La referencia contiene picos recortados. ";
    if(source.left.size()/source.sampleRate<.12)d.warnings+="Fragmento corto: menor fiabilidad tonal. ";
    if(d.sourceRms<1e-5)d.warnings+="Referencia silenciosa. ";
    if(d.warnings.empty())d.warnings="Sin incidencias claras en este analisis; no certifica una copia exacta.";
    return d;
}
inline std::string diagnosticText(const SoundDiagnostic& d){
    std::ostringstream out;out<<std::fixed<<std::setprecision(2);out<<"AFINACION / medicion estimada\nOriginal: ";if(d.sourceTonal)out<<d.sourceHz<<" Hz | "<<d.sourceCents<<" cents";else out<<"sin fundamental estable";out<<"\nRecreado: ";if(d.resultTonal)out<<d.resultHz<<" Hz | "<<d.resultCents<<" cents";else out<<"sin fundamental estable";
    out<<"\nNota original MIDI: "<<d.sourceNote<<" | confianza tonal: "<<d.confidence;
    if(d.expectedNote>=0)out<<"\nNota objetivo MIDI: "<<d.expectedNote;
    if(d.resultTonal&&d.expectedNote>=0)out<<" | error respecto al objetivo: "<<d.targetErrorCents<<" cents";
    out<<"\n\nDISTANCIAS / menor es mejor, no son porcentajes\nAtaque: "<<d.attackError<<"   Cuerpo: "<<d.bodyError<<"   Envolvente: "<<d.envelopeError<<"\n\n"<<d.warnings;return out.str();
}
}
