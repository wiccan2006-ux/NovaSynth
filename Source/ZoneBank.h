#pragma once
#include "SpectralModel.h"
namespace nova {
constexpr int maxZones=8;
struct ModelZone { SpectralModel model;float velocity=.8f; };
struct ZoneBank {
    int count=0;std::array<ModelZone,maxZones> zones{};
    bool valid()const{if(count<1||count>maxZones)return false;for(int i=0;i<count;++i)if(!zones[i].model.valid()||!std::isfinite(zones[i].velocity)||zones[i].velocity<.05f||zones[i].velocity>1)return false;return true;}
    int select(int note,float velocity)const{
        int best=-1;double distance=1e30;
        for(int i=0;i<count;++i){double n=note-zones[i].model.root,v=(velocity-zones[i].velocity)*18;double score=n*n+v*v;if(score<distance){distance=score;best=i;}}
        return best;
    }
};
}
