#pragma once
#include "ContextAnalysis.h"
#include <string>
namespace nova {
struct ReviewRow {
    ContextNote note;double clipped=0;bool recommended=false;
    std::array<float,24> envelope{};
    bool stable()const{return note.confidence>=.87f&&note.centsSpread<=75&&clipped<=.001;}
};
struct ReferenceReview {
    ContextAnalysis analysis;std::vector<ReviewRow> rows;
    std::array<float,256> waveform{};double rms=0,clipped=0;bool cancelled=false;
    std::string summary;
};
inline ReferenceReview reviewReference(const Sound& source,std::atomic<bool>& cancel,const std::function<void(float)>& progress={}){
    ReferenceReview review;review.analysis=analyseContext(source,cancel,[&](float v){if(progress)progress(v*.8f);});
    if(cancel.load()||review.analysis.cancelled){review.cancelled=true;return review;}
    const auto size=std::min(source.left.size(),source.right.size());if(!size||source.sampleRate<=0){review.summary="Referencia vacia.";return review;}
    double energy=0;for(size_t i=0;i<size;++i){energy+=(source.left[i]*source.left[i]+source.right[i]*source.right[i])*.5;review.clipped+=std::max(std::abs(source.left[i]),std::abs(source.right[i]))>=.999f;size_t bin=std::min<size_t>(255,i*256/size);review.waveform[bin]=std::max(review.waveform[bin],std::max(std::abs(source.left[i]),std::abs(source.right[i])));}
    review.rms=std::sqrt(energy/size);review.clipped/=size;
    for(const auto& n:review.analysis.regions){
        if(cancel.load()){review.cancelled=true;return review;}
        ReviewRow row;row.note=n;auto span=sampleSpan(n.range,source.sampleRate,static_cast<int64_t>(size));if(!span.count)continue;
        for(int64_t j=0;j<span.count;++j)row.clipped+=std::max(std::abs(source.left[span.first+j]),std::abs(source.right[span.first+j]))>=.999f;
        row.clipped/=span.count;float maximum=1e-8f;
        for(int bin=0;bin<24;++bin){int64_t first=span.first+bin*span.count/24,last=span.first+(bin+1)*span.count/24;double e=0;for(auto i=first;i<last;++i)e+=(source.left[i]*source.left[i]+source.right[i]*source.right[i])*.5;row.envelope[bin]=static_cast<float>(std::sqrt(e/std::max<int64_t>(1,last-first)));maximum=std::max(maximum,row.envelope[bin]);}
        for(auto& value:row.envelope)value/=maximum;
        for(const auto& picked:review.analysis.picked)if(std::abs(picked.range.start-n.range.start)<1e-6&&std::abs(picked.range.end-n.range.end)<1e-6&&row.stable())row.recommended=true;
        review.rows.push_back(row);
    }
    // Recommend from the reviewable pool after clipping checks, so a bad row
    // cannot occupy a slot that could teach another register.
    for(auto& row:review.rows)row.recommended=false;
    std::vector<int> picked;
    while(picked.size()<maxZones){int best=-1;double score=-1;for(size_t i=0;i<review.rows.size();++i){const auto& row=review.rows[i];if(!row.stable()||row.recommended)continue;double diversity=picked.empty()?18:1e9;bool duplicate=false;for(int j:picked){diversity=std::min(diversity,static_cast<double>(std::abs(row.note.note-review.rows[j].note.note)+18*std::abs(row.note.velocity-review.rows[j].note.velocity)));if(row.note.note==review.rows[j].note.note&&std::abs(row.note.velocity-review.rows[j].note.velocity)<.09f)duplicate=true;}if(duplicate)continue;double value=row.note.quality*(1+std::min(18.,diversity));if(value>score){score=value;best=static_cast<int>(i);}}if(best<0)break;review.rows[best].recommended=true;picked.push_back(best);}
    review.summary=review.rows.empty()?"No se encontraron regiones tonales fiables. Prueba una nota aislada sin mezcla.":"Escucha las regiones antes de elegirlas. La intensidad es relativa y no una velocidad MIDI recuperada.";
    if(review.clipped>.001)review.summary+=" Hay picos recortados: algunas notas no se recomiendan.";
    if(review.analysis.usableFraction<.5)review.summary+=" Baja cobertura tonal: puede incluir silencios, ruido, acordes o efectos; el analisis no distingue esas causas con certeza.";
    review.summary+=" No separa instrumentos ni elimina reverb automaticamente.";
    if(progress)progress(1);return review;
}
inline std::vector<int> recommendedRows(const ReferenceReview& review){std::vector<int> out;for(size_t i=0;i<review.rows.size()&&out.size()<maxZones;++i)if(review.rows[i].recommended)out.push_back(static_cast<int>(i));return out;}
inline ContextAnalysis chosenReviewNotes(const ReferenceReview& review,const std::vector<int>& indices){
    auto analysis=review.analysis;analysis.picked.clear();if(indices.size()>maxZones)return analysis;
    std::set<int> seen;for(int i:indices)if(i>=0&&i<static_cast<int>(review.rows.size())&&seen.insert(i).second)analysis.picked.push_back(review.rows[i].note);return analysis;
}
}
