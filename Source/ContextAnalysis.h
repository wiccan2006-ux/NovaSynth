#pragma once
#include "SynthCore.h"
#include "Selection.h"
#include <atomic>
#include <functional>
#include <set>

namespace nova {
struct ContextNote {
    SelectionRange range;int note=60;float velocity=.8f,confidence=0,rms=0;double quality=0,centsSpread=0,detuning=0;bool manual=false;
};
struct ContextAnalysis {
    double duration=0,usableFraction=0;int events=0,uniqueNotes=0,rejectedUnstable=0;bool cancelled=false;
    std::vector<ContextNote> picked,regions,sequence;
};
// Context is an analysis source, never an audio source for Engine.
inline ContextAnalysis analyseContext(const Sound& sound,std::atomic<bool>& cancel,const std::function<void(float)>& progress={}){
    ContextAnalysis result;if(sound.left.size()!=sound.right.size()||sound.left.size()<128||sound.sampleRate<8000||sound.sampleRate>96000)return result;
    result.duration=sound.left.size()/sound.sampleRate;if(result.duration>60.0001)return result;
    constexpr double rate=12000;constexpr int window=960,hop=240,maxLag=300,compare=window-maxLag;
    size_t count=static_cast<size_t>(sound.left.size()*rate/sound.sampleRate);if(count<window)return result;
    double le=0,re=0;for(size_t i=0;i<sound.left.size();i+=16){le+=sound.left[i]*sound.left[i];re+=sound.right[i]*sound.right[i];}const auto& source=le>=re?sound.left:sound.right;
    std::vector<float> mono(count);double peak=0;
    for(size_t i=0;i<count;++i){double pos=i*sound.sampleRate/rate;size_t a=std::min(source.size()-2,static_cast<size_t>(pos));float t=static_cast<float>(pos-a);mono[i]=source[a]*(1-t)+source[a+1]*t;peak=std::max(peak,static_cast<double>(std::abs(mono[i])));}
    if(peak<1e-5)return result;
    struct Frame {double start=0,rms=0;int note=-1;float confidence=0;double cents=0;};std::vector<Frame> frames;
    for(size_t start=0;start+window<=count;start+=hop){
        if(cancel.load()){result.cancelled=true;return result;}
        Frame frame;frame.start=start/rate;double energy=0;for(int j=0;j<window;++j)energy+=mono[start+j]*mono[start+j];frame.rms=std::sqrt(energy/window);
        if(frame.rms>peak*.012){
            std::array<double,maxLag+1> cmnd{};cmnd[0]=1;double sum=0;
            for(int lag=1;lag<=maxLag;++lag){double diff=0;for(int j=0;j<compare;++j){double d=mono[start+j]-mono[start+j+lag];diff+=d*d;}sum+=diff;cmnd[lag]=sum>1e-20?diff*lag/sum:1;}
            int best=0;for(int lag=5;lag<maxLag-1;++lag)if(cmnd[lag]<.13&&cmnd[lag]<=cmnd[lag-1]&&cmnd[lag]<cmnd[lag+1]){best=lag;break;}
            if(best){double den=cmnd[best-1]-2*cmnd[best]+cmnd[best+1],shift=std::abs(den)>1e-12?.5*(cmnd[best-1]-cmnd[best+1])/den:0;double hz=rate/(best+std::clamp(shift,-.5,.5));int note=static_cast<int>(std::lround(69+12*std::log2(hz/440)));if(note>=24&&note<=108){frame.cents=1200*std::log2(hz/(440*std::pow(2.,(note-69)/12.)));frame.note=note;frame.confidence=static_cast<float>(1-cmnd[best]);}}
        }
        frames.push_back(frame);if(progress&&start%(hop*16)==0)progress(static_cast<float>(start)/count);
    }
    // Remove isolated pitch flips, without filling silence or spanning real pauses.
    auto original=frames;for(size_t i=1;i+1<frames.size();++i)if(original[i-1].note>=0&&original[i-1].note==original[i+1].note&&original[i].rms>peak*.012)frames[i].note=original[i-1].note;
    std::vector<double> energies;for(const auto& f:frames)if(f.note>=0)energies.push_back(f.rms);
    if(energies.empty())return result;std::sort(energies.begin(),energies.end());double level=energies[energies.size()*9/10];result.usableFraction=static_cast<double>(energies.size())/std::max<size_t>(1,frames.size());
    std::vector<ContextNote> candidates;std::set<int> notes;
    for(size_t first=0;first<frames.size();){
        if(frames[first].note<0){++first;continue;}size_t last=first+1;while(last<frames.size()&&frames[last].note==frames[first].note)++last;
        double begin=frames[first].start,end=std::min(result.duration,frames[last-1].start+window/rate);if(first>0&&frames[first-1].note>=0)begin=(frames[first-1].start+frames[first].start)*.5+window/(rate*2);if(last<frames.size()&&frames[last].note>=0)end=(frames[last-1].start+frames[last].start)*.5+window/(rate*2);int n=static_cast<int>(last-first);
        if(n>=3&&end-begin>=.10){
            ++result.events;notes.insert(frames[first].note);
            int variants=end-begin>3?3:1;
            for(int v=0;v<variants;++v){double length=std::min(2.,end-begin),start=begin+(end-begin-length)*(variants==1?0:v/2.);double rms=0,confidence=0;int used=0;std::vector<double> deviations;
                for(size_t i=first;i<last;++i)if(frames[i].start>=start-.04&&frames[i].start<=start+length){rms+=frames[i].rms;confidence+=frames[i].confidence;deviations.push_back(frames[i].cents);++used;}
                if(!used)continue;std::sort(deviations.begin(),deviations.end());double spread=deviations[deviations.size()*9/10]-deviations[deviations.size()/10];if(spread>75)++result.rejectedUnstable;rms/=used;confidence/=used;float velocity=static_cast<float>(std::clamp(.2+.75*std::sqrt(rms/std::max(level,1e-9)),.2,1.));
                ContextNote c;c.centsSpread=spread;c.detuning=deviations[deviations.size()/2];c.range={start,start+length};c.note=frames[first].note;c.rms=static_cast<float>(rms);c.velocity=velocity;c.confidence=static_cast<float>(confidence);c.quality=(1./(1+spread/35))*confidence*std::min(1.,length/.35)*std::sqrt(rms/std::max(level,1e-9));if(v==0&&spread<=75&&result.sequence.size()<512){auto event=c;event.range={begin,end};result.sequence.push_back(event);}if(result.regions.size()<512)result.regions.push_back(c);if(spread<=75)candidates.push_back(c);
            }
        }first=last;
    }
    result.uniqueNotes=static_cast<int>(notes.size());
    // Greedily cover register and relative dynamics rather than eight copies of
    // the loudest repeated note. Eight is the synthesised bank's current limit.
    while(result.picked.size()<8&&!candidates.empty()){
        size_t best=0;double score=-1;
        for(size_t i=0;i<candidates.size();++i){double diversity=1;if(!result.picked.empty()){diversity=1e9;for(const auto& p:result.picked){double distance=std::abs(p.note-candidates[i].note)+18*std::abs(p.velocity-candidates[i].velocity);diversity=std::min(diversity,distance);}if(diversity<1.7)continue;}
            double value=candidates[i].quality*(1+std::min(18.,diversity));if(value>score){score=value;best=i;}}
        if(score<0)break;result.picked.push_back(candidates[best]);candidates.erase(candidates.begin()+static_cast<ptrdiff_t>(best));
    }
    if(progress)progress(1);return result;
}
inline Sound extractContextNote(const Sound& source,const ContextNote& note){
    Sound result;result.sampleRate=source.sampleRate;auto span=sampleSpan(note.range,source.sampleRate,static_cast<int64_t>(source.left.size()));if(span.count<1)return result;
    result.left.assign(source.left.begin()+span.first,source.left.begin()+span.first+span.count);result.right.assign(source.right.begin()+span.first,source.right.begin()+span.first+span.count);analyse(result);return result;
}
}
