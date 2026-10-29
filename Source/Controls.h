#pragma once
struct ControlSpec {const char* id;const char* label;float minimum,maximum,initial,skew,step;bool effect;};
static constexpr ControlSpec controls[]={
 {"mix","OSC B Mix",0,1,.25f,1,.001f,false},
 {"detune","Detune / cents",0,35,8,1,.001f,false},
 {"transpose","OSC B / semis",-24,24,0,1,1,false},
 {"sub","Sub",0,1,.15f,1,.001f,false},
 {"noise","Noise",0,.5f,0,1,.001f,false},
 {"fm","FM",0,1,0,1,.001f,false},
 {"cutoff","Cutoff / Hz",30,20000,5000,.28f,1,false},
 {"resonance","Resonance",0,1,.15f,1,.001f,false},
 {"envamount","Filter Env / oct",-4,6,2,1,.001f,false},
 {"attack","Attack / s",.001f,5,.01f,.3f,.001f,false},
 {"decay","Decay / s",.005f,5,.3f,.35f,.001f,false},
 {"sustain","Sustain",0,1,.6f,1,.001f,false},
 {"release","Release / s",.005f,8,.4f,.35f,.001f,false},
 {"lforate","LFO / Hz",.03f,15,2,.4f,.001f,false},
 {"lfodepth","LFO > Filter",0,4,0,1,.001f,false},
 {"drive","Drive",0,1,0,1,.001f,true},
 {"delay","Delay Mix",0,.7f,.12f,1,.001f,true},
 {"delaytime","Delay Time / s",.04f,1.5f,.3f,.6f,.001f,true},
 {"feedback","Feedback",0,.8f,.25f,1,.001f,true},
 {"reverb","Reverb Mix",0,.7f,.12f,1,.001f,true},
 {"spread","Stereo Width",0,1,.7f,1,.001f,false},
 {"master","Output / dB",-36,0,-12,1,.1f,false},
 {"root","Reference Note",24,108,60,1,1,false},
 {"fine","Tune / cents",-100,100,0,1,.1f,false},
 {"chorus","Chorus Mix",0,.8f,0,1,.001f,true},
 {"chorusrate","Chorus / Hz",.1f,3,.8f,.5f,.001f,true},
 {"room","Reverb Room",0,1,.5f,1,.001f,true},
 {"eqlow","EQ Low / dB",-12,12,0,1,.1f,true},
 {"eqhigh","EQ High / dB",-12,12,0,1,.1f,true},
 {"spectime","Partial Time",.25f,4,1,.6f,.001f,false},
 {"spectone","Partial Tone",-1,1,0,1,.001f,false},
 {"residual","Residual / Air",0,2,1,1,.001f,false},
 {"tracking","Pitch Movement",0,1,1,1,.001f,false}
};
static constexpr size_t controlCount=sizeof(controls)/sizeof(controls[0]);
static constexpr const char* effectIds[]={"driveOn","chorusOn","eqOn","delayOn","reverbOn"};
static constexpr const char* effectNames[]={"Drive","Chorus","EQ","Delay","Reverb"};
