// Original, balanced procedural corpus for FM, wavetable and two waveguides.
#include "AutoFit.h"
#include <fstream>
#include <iostream>
int main(int argc,char** argv){if(argc!=2)return 1;std::ofstream out(argv[1],std::ios::binary);std::mt19937 rng(0x4e533130);std::uniform_real_distribution<float> u(0,1);nova::Engine e;nova::FeatureExtractor extractor;e.prepare(22050);
 for(int row=0;row<8192;++row){int cls=row%4,engine=std::array<int,4>{0,2,3,4}[cls];nova::FitTarget t;t.note=40+rng()%41;t.rate=22050;double seconds=.4+u(rng)*.7;t.left.resize(static_cast<size_t>(seconds*t.rate));t.right=t.left;t.gate=seconds*.75;
  auto p=nova::preset(rng()%64);p.engine=engine;p.root=t.note;p.fineTune=0;p.unison=false;p.detune=p.chorus=p.delay=p.reverb=p.drive=0;p.lfoDepth=0;p.envAmount=0;p.master=-6;p.sub=0;p.noise=0;
  p.attack=.001f+u(rng)*.07f;p.decay=.05f+u(rng)*1.2f;p.sustain=.2f+u(rng)*.75f;p.release=.03f+u(rng)*.5f;p.cutoff=700*std::pow(20.f,u(rng));p.resonance=.05f+u(rng)*.7f;p.eqLow=p.eqHigh=0;
  if(engine==2){p.waveA=rng()%5;p.waveB=rng()%5;p.fm=u(rng);p.mix=u(rng);}
  if(engine>=3){p.waveA=p.waveB=0;p.fm=p.mix=0;p.attack=engine==3?.001f:.008f+u(rng)*.15f;p.sustain=engine==3?1:.7f;p.noise=engine==4?u(rng)*.15f:0;p.resonance=.2f+u(rng)*.75f;}
  nova::renderCandidate(e,p,t,t.left,t.right);auto x=nova::neuralFeatures(extractor.extract(t.left,t.right,t.rate),t.note,seconds);auto g=nova::toGenes(p);float label=static_cast<float>(cls);
  out.write(reinterpret_cast<const char*>(x.data()),sizeof(x));out.write(reinterpret_cast<const char*>(g.data()),sizeof(g));out.write(reinterpret_cast<const char*>(&label),sizeof(label));if(row%1024==0)std::cout<<row<<std::endl;
 }
}
