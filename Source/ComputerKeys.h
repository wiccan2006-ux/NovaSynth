#pragma once
#include <array>
#include <algorithm>
namespace nova {
// State machine shared by GUI and native tests. No OS polling inside the audio thread.
class ComputerKeys {
    std::array<int,13> sounding=[]{std::array<int,13> a{};a.fill(-1);return a;}();
public:
    inline static constexpr char mapping[]="AWSEDFTGYHUJK";
    int base=48;
    template<class Off> void release(Off off){for(auto& n:sounding)if(n>=0){off(n);n=-1;}}
    template<class On,class Off> bool update(const std::array<bool,13>& down,bool enabled,On on,Off off){
        bool changed=false;for(int i=0;i<13;++i){bool play=enabled&&down[i];if(play&&sounding[i]<0){sounding[i]=std::clamp(base+i,0,127);on(sounding[i]);changed=true;}else if(!play&&sounding[i]>=0){off(sounding[i]);sounding[i]=-1;changed=true;}}return changed;
    }
};
}
