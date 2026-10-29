#pragma once
#include "SynthCore.h"
#include "SignatureBank.h"
#include <string>
namespace nova {
constexpr int legacyPresetCount=64,newPresetCount=300,extraPresetCount=400,factoryPresetCount=legacyPresetCount+newPresetCount+extraPresetCount+signaturePresetCount;
inline const std::array<const char*,64>& legacyNames() {
    static const std::array<const char*,64> n={
      "Bass / Sub Round","Bass / Punch","Bass / Rubber","Bass / Acid Line","Bass / Wide Reese","Bass / Hollow","Bass / Dirty Pulse","Bass / Deep Motion",
      "Lead / Neon","Lead / Glass Edge","Lead / Twin Saw","Lead / Retro Pulse","Lead / Air Flight","Lead / Razor","Lead / Soft Solo","Lead / Night Drive",
      "Pad / Aurora","Pad / Velvet","Pad / Slow Clouds","Pad / Ocean","Pad / Distant Light","Pad / Warm Horizon","Pad / Lunar","Pad / Frozen Air",
      "Pluck / Dew","Pluck / Short Circuit","Pluck / Wood","Pluck / Wire","Pluck / Bright Steps","Pluck / Night Rain","Pluck / Rubber String","Pluck / Tiny Stars",
      "Keys / Soft Electric","Keys / Digital Ivory","Keys / Dust","Keys / Mellow","Keys / Crystal Room","Keys / Retro Chord","Keys / Night Piano","Keys / Dream Steps",
      "Bell / Clear Glass","Bell / Copper","Bell / Orbit","Bell / Ice","Bell / Temple","Bell / Silver Rain","Bell / Small Chime","Bell / Starlight",
      "Brass / Analog Horn","Brass / Warm Section","Brass / Bright Stack","Brass / Velvet Horn","Brass / Synth Fanfare","Brass / Low Section","Brass / Air Section","Brass / Bold Motion",
      "FX / Rising Dust","FX / Space Radio","FX / Metal Current","FX / Alien Pulse","FX / Fog Signal","FX / Wind Tunnel","FX / Moving Grid","FX / Broken Satellite"};
    return n;
}
inline Params legacyPreset(int index) {
    index=std::clamp(index,0,63);int group=index/8,v=index%8;
    Params p;p.detune=4+v*1.7f;p.mix=.15f+v*.07f;p.spread=.3f+v*.08f;
    switch(group) {
    case 0:p.waveA=v%3;p.waveB=(v+1)%3;p.cutoff=180+v*130.f;p.envAmount=1.2f+v*.35f;p.attack=.001f;p.decay=.12f+v*.025f;p.sustain=.55f;p.release=.08f;p.sub=.5f;p.drive=v*.07f;p.reverb=0;p.delay=0;p.unison=v>=4;break;
    case 1:p.waveA=1+v%2;p.waveB=v%4;p.cutoff=1800+v*650.f;p.attack=.004f;p.decay=.2f;p.sustain=.75f;p.release=.17f+v*.03f;p.delay=.13f+v*.02f;p.reverb=.12f;p.lfoDepth=v*.015f;p.envAmount=.5f;break;
    case 2:p.waveA=v%4;p.waveB=(v+1)%4;p.cutoff=1200+v*340.f;p.attack=.6f+v*.16f;p.decay=1.2f;p.sustain=.7f;p.release=1.5f+v*.2f;p.delay=.2f;p.reverb=.45f;p.lfoRate=.12f+v*.04f;p.lfoDepth=.25f;p.envAmount=.5f;p.sub=.05f;break;
    case 3:p.waveA=v%4;p.waveB=(v+2)%4;p.cutoff=700+v*350.f;p.attack=.001f;p.decay=.13f+v*.035f;p.sustain=.04f;p.release=.12f+v*.02f;p.envAmount=3.7f;p.delay=.18f;p.delayTime=.19f+v*.03f;p.reverb=.2f;p.sub=.05f;break;
    case 4:p.waveA=v%2?3:0;p.waveB=v%3;p.transpose=v%2?12:0;p.cutoff=2500+v*420.f;p.attack=.004f;p.decay=.45f+v*.05f;p.sustain=.25f;p.release=.35f;p.fm=v*.025f;p.reverb=.22f;p.delay=.08f;p.unison=v>=4;break;
    case 5:p.waveA=0;p.waveB=0;p.transpose=12+v;p.mix=.15f;p.fm=.08f+v*.07f;p.cutoff=7000;p.attack=.001f;p.decay=1.0f+v*.13f;p.sustain=0;p.release=1.0f;p.reverb=.35f;p.delay=.15f;p.unison=false;p.sub=0;break;
    case 6:p.waveA=1;p.waveB=v%2?2:1;p.cutoff=650+v*210.f;p.attack=.045f+v*.008f;p.decay=.22f;p.sustain=.7f;p.release=.2f;p.envAmount=2.5f;p.reverb=.18f;p.delay=0;p.sub=.13f;p.noise=.01f;break;
    case 7:p.waveA=v%5;p.waveB=(v+2)%5;p.transpose=-12+v*4;p.cutoff=500+v*600.f;p.attack=.06f+v*.08f;p.decay=.3f;p.sustain=.5f;p.release=.7f;p.noise=v*.018f;p.fm=.08f+v*.04f;p.lfoRate=.2f+v*.8f;p.lfoDepth=1.4f;p.delay=.35f;p.feedback=.45f;p.reverb=.4f;break;
    }
    return p;
}
} // namespace nova
#include "DanceBank.h"
namespace nova {
inline const std::array<std::string,factoryPresetCount>& names(){
    static const auto all=[] {std::array<std::string,factoryPresetCount> a{};for(int i=0;i<legacyPresetCount;++i)a[i]=legacyNames()[i];for(int i=0;i<newPresetCount;++i)a[i+legacyPresetCount]=std::string(danceCategories[i/25])+" / "+danceNames[i/25][i%25];for(int i=0;i<400;++i){static const char* categories[]={"Halloween","Synthwave 80s","Trap","Hip Hop 2K"};static const char* adjectives[]={"Shadow","Electric","Midnight","Neon","Cold","Golden","Moon","Dark","Velvet","Digital"};static const char* shapes[]={"Pulse","Keys","Pluck","Bass","Pad","Bell","Lead","Brass","Texture","Motion"};a[364+i]=std::string(categories[i/100])+" / "+adjectives[(i%100)/10]+" "+shapes[i%10];}for(int i=0;i<signaturePresetCount;++i)a[signaturePresetStart+i]=std::string("Signature / ")+signaturePatches()[i].name;return a;}();return all;
}
inline int presetCategory(int index){return index>=signaturePresetStart?24+(index-signaturePresetStart)/8:index<64?std::clamp(index,0,63)/8:index<364?8+(index-64)/25:20+(std::clamp(index,364,factoryPresetCount-1)-364)/100;}
inline const char* presetCategoryName(int category){static const char* old[]={"Original / Bass","Original / Lead","Original / Pad","Original / Pluck","Original / Keys","Original / Bell","Original / Brass","Original / FX"};static const char* extra[]={"Halloween","Synthwave 80s","Trap","Hip Hop 2K"};return category>=24?signatureFamily((category-24)*8):category<8?old[std::clamp(category,0,7)]:category<20?danceCategories[category-8]:extra[std::clamp(category-20,0,3)];}
inline Params preset(int index){index=std::clamp(index,0,factoryPresetCount-1);if(index>=signaturePresetStart)return signaturePreset(index-signaturePresetStart);if(index<64)return legacyPreset(index);if(index<364)return dancePreset(index-64);
    int n=index-364,group=n/100,v=n%100;static const int seed[4][10]={{6,9,11,5,6,9,0,8,6,11},{0,10,1,2,5,9,4,8,10,11},{2,3,1,2,5,9,4,8,6,11},{2,10,1,2,5,9,0,8,3,11}};
    Params p=dancePreset(seed[group][v%10]*25+(v/10)*2+(v%2));float x=(v/10)/9.f,y=(v%10)/9.f;
    if(group==0){p.fm=std::min(1.f,p.fm+.2f*x);p.reverb=std::min(.7f,p.reverb+.15f+.1f*y);p.cutoff=std::max(100.f,p.cutoff*(.45f+.5f*x));p.lfoDepth=std::min(4.f,p.lfoDepth+.45f*y);}
    if(group==1){p.chorus=std::min(.8f,p.chorus+.15f+.25f*x);p.detune=std::min(35.f,p.detune+3+5*y);p.reverb=std::min(.7f,p.reverb+.1f);}
    if(group==2){p.drive=std::min(1.f,p.drive+.08f+.25f*x);p.sub=std::min(1.f,p.sub+.2f*y);p.reverb*=.5f;p.delay*=.5f;}
    if(group==3){p.cutoff=std::max(80.f,p.cutoff*(.55f+.35f*y));p.eqHigh=std::clamp(p.eqHigh-3+4*x,-12.f,12.f);p.delay=std::min(.7f,p.delay+.07f*y);p.chorus=std::min(.8f,p.chorus+.15f*x);}
    p.mix=std::clamp(p.mix+.012f*(v/10),0.f,1.f);return p;}
}
