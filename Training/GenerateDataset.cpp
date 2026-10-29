// Original synthetic training corpus. No third-party recordings or personal audio.
#include "AutoFit.h"
#include <fstream>
#include <iostream>
int main(int argc,char** argv){
 if(argc!=2)return 1;std::ofstream out(argv[1],std::ios::binary);std::mt19937 rng(0x4e534149);std::normal_distribution<float> normal(0,1);
 nova::Engine engine;nova::FeatureExtractor extractor;
 for(int row=0;row<6144;++row){
  nova::FitTarget t;t.rate=22050;t.note=36+static_cast<int>(rng()%49);double seconds=.6+(rng()%141)/100.;t.left.resize(static_cast<size_t>(seconds*t.rate));t.right=t.left;t.gate=seconds*.7;
  auto params=nova::preset(rng()%64);auto genes=nova::toGenes(params);
  for(int j=0;j<30;++j)genes[j]=std::clamp(genes[j]+normal(rng)*.085f,0.f,1.f);
  params=nova::fromGenes(genes,t.note);params.fineTune=0;genes=nova::toGenes(params);engine.configure(params);engine.prepare(t.rate);nova::renderCandidate(engine,params,t,t.left,t.right);
  auto f=extractor.extract(t.left,t.right,t.rate);std::array<float,155> x{};std::copy(f.spectrum.begin(),f.spectrum.end(),x.begin());std::copy(f.envelope.begin(),f.envelope.end(),x.begin()+128);x[152]=f.width;x[153]=(t.note-60)/24.f;x[154]=static_cast<float>(seconds/3.);
  out.write(reinterpret_cast<const char*>(x.data()),sizeof(x));out.write(reinterpret_cast<const char*>(genes.data()),sizeof(genes));if(row%512==0)std::cout<<row<<std::endl;
 }
}
