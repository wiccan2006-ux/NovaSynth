#pragma once
#include <juce_audio_formats/juce_audio_formats.h>
#include "Selection.h"
#include "SynthCore.h"
#include <atomic>
#include <memory>
#include <stdexcept>
struct ReferenceFile {
    juce::File file;double rate=48000,duration=0;juce::int64 frames=0,fileSize=0,modified=0;
    std::vector<nova::PeakPair> peaks;std::vector<std::vector<nova::PeakPair>> mips;
};
inline std::shared_ptr<ReferenceFile> readReferenceOverview(juce::AudioFormatManager& formats,const juce::File& file,std::atomic<bool>& cancel,std::atomic<float>& progress){
    std::unique_ptr<juce::AudioFormatReader> reader(formats.createReaderFor(file));
    if(!reader||!std::isfinite(reader->sampleRate)||reader->sampleRate<8000||reader->sampleRate>96000||reader->lengthInSamples<64)throw std::runtime_error("Usa WAV, AIFF, FLAC u OGG de 8 a 96 kHz.");
    auto result=std::make_shared<ReferenceFile>();result->file=file;result->rate=reader->sampleRate;result->frames=reader->lengthInSamples;result->duration=result->frames/result->rate;result->fileSize=file.getSize();result->modified=file.getLastModificationTime().toMilliseconds();
    if(result->duration<.05)throw std::runtime_error("La referencia debe durar al menos 0.05 segundos.");
    if(result->duration>3600)throw std::runtime_error("El archivo supera el limite de 60 minutos. Exporta una parte mas corta.");
    nova::PeakBuilder builder;builder.peaks.reserve(static_cast<size_t>((result->frames+255)/256));constexpr int block=65536;juce::AudioBuffer<float> buffer(2,block);
    for(juce::int64 pos=0;pos<result->frames&&!cancel.load();pos+=block){int count=static_cast<int>(std::min<juce::int64>(block,result->frames-pos));if(!reader->read(&buffer,0,count,pos,true,true))throw std::runtime_error("No se pudo leer la forma de onda completa.");auto* l=buffer.getReadPointer(0);auto* r=buffer.getReadPointer(reader->numChannels>1?1:0);for(int i=0;i<count;++i)builder.push(l[i],r[i]);progress.store(static_cast<float>(double(pos+count)/result->frames));}
    if(cancel.load())return {};builder.finish();result->peaks=std::move(builder.peaks);
    const auto* current=&result->peaks;while(current->size()>512){std::vector<nova::PeakPair> next;next.reserve((current->size()+7)/8);for(size_t i=0;i<current->size();i+=8){nova::PeakPair pair;for(size_t j=i;j<std::min(current->size(),i+8);++j){pair.low=std::min(pair.low,(*current)[j].low);pair.high=std::max(pair.high,(*current)[j].high);}next.push_back(pair);}result->mips.push_back(std::move(next));current=&result->mips.back();}if(cancel.load())return {};return result;
}
inline std::shared_ptr<nova::Sound> readSelectedReference(juce::AudioFormatManager& formats,const ReferenceFile& reference,nova::SelectionRange range,bool analysePitch=true,double maximumSeconds=3){
    if(reference.file.getSize()!=reference.fileSize||reference.file.getLastModificationTime().toMilliseconds()!=reference.modified)throw std::runtime_error("El archivo cambio o ya no existe. Vuelve a cargarlo.");
    if(!std::isfinite(maximumSeconds)||maximumSeconds<.05||maximumSeconds>60)throw std::runtime_error("Limite de contexto invalido.");auto span=nova::sampleSpan(range,reference.rate,reference.frames,maximumSeconds);if(span.count<1||span.count>static_cast<juce::int64>(reference.rate*maximumSeconds)+1)throw std::runtime_error("Seleccion invalida o demasiado larga para este modo.");
    std::unique_ptr<juce::AudioFormatReader> reader(formats.createReaderFor(reference.file));if(!reader||reader->sampleRate!=reference.rate||reader->lengthInSamples!=reference.frames)throw std::runtime_error("No se pudo abrir el fragmento seleccionado.");
    int count=static_cast<int>(span.count);juce::AudioBuffer<float> data(2,count);if(!reader->read(&data,0,count,span.first,true,true))throw std::runtime_error("No se pudo leer el fragmento.");
    auto sound=std::make_shared<nova::Sound>();sound->sampleRate=reference.rate;sound->left.assign(data.getReadPointer(0),data.getReadPointer(0)+count);int channel=reader->numChannels>1?1:0;sound->right.assign(data.getReadPointer(channel),data.getReadPointer(channel)+count);
    for(auto* values:{&sound->left,&sound->right})for(auto& x:*values)x=std::isfinite(x)?std::clamp(x,-4.f,4.f):0;
    if(analysePitch)nova::analyse(*sound);return sound;
}
