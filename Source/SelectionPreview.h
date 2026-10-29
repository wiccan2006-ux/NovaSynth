#pragma once
#include "SynthCore.h"
#include <atomic>
#include <memory>

namespace nova {
// Reference audition only. It never feeds the instrument's oscillator engine.
class SelectionPreview {
public:
    struct Clip { Sound sound; std::shared_ptr<const Sound> sharedSound; double start=0; uint64_t generation=0; };
private:
    std::atomic<Clip*> incoming{nullptr},retired{nullptr};
    Clip* active=nullptr;
    std::atomic<uint64_t> generation{0};
    double cursor=0;
public:
    std::atomic<bool> playing{false};
    std::atomic<double> position{-1};
    ~SelectionPreview(){delete incoming.exchange(nullptr);delete retired.exchange(nullptr);delete active;}
    uint64_t stop(){auto id=generation.fetch_add(1)+1;playing.store(false);position.store(-1);delete incoming.exchange(nullptr);return id;}
    bool pending()const{return incoming.load()!=nullptr;}
    bool current(uint64_t id)const{return generation.load()==id;}
    void publish(std::unique_ptr<Clip> clip){if(current(clip->generation))delete incoming.exchange(clip.release());}
    void collect(){delete retired.exchange(nullptr);}
    void render(float* l,float* r,int count,double outputRate){
        if(retired.load()==nullptr)if(auto* clip=incoming.exchange(nullptr)){
            auto* old=active;active=clip;cursor=0;retired.store(old);
        }
        if(!active||!current(active->generation)||outputRate<=0){playing.store(false);position.store(-1);return;}
        const auto& s=active->sharedSound?*active->sharedSound:active->sound;size_t size=std::min(s.left.size(),s.right.size());
        if(size<2||cursor>=size){playing.store(false);position.store(-1);return;}
        playing.store(true);double step=s.sampleRate/outputRate;
        for(int i=0;i<count&&cursor<size;++i,cursor+=step){
            size_t a=static_cast<size_t>(cursor),b=std::min(a+1,size-1);float t=static_cast<float>(cursor-a);
            // Short boundary ramps suppress clicks without changing reference pitch.
            float fade=static_cast<float>(std::clamp(std::min(cursor,(size-1)-cursor)/(s.sampleRate*.003),0.,1.));
            l[i]+=(s.left[a]*(1-t)+s.left[b]*t)*fade;
            r[i]+=(s.right[a]*(1-t)+s.right[b]*t)*fade;
        }
        bool running=cursor<size;playing.store(running);position.store(running?active->start+cursor/s.sampleRate:-1);
    }
};
}
