#pragma once
#include "MultiNeuralWeights.h"
namespace nova {
struct MultiProposal {std::array<float,4> priority{};std::array<Genes,4> genes{};};
inline MultiProposal inferMultiProposal(std::array<float,155> x){
 using namespace multi_weights;for(int j=0;j<155;++j)x[j]=std::clamp((x[j]-mean[j])/std::max(scale[j],1e-8f),-12.f,12.f);
 MultiProposal out;std::array<float,48> h{};for(int i=0;i<48;++i){float sum=cb0[i];for(int j=0;j<155;++j)sum+=cw0[i*155+j]*x[j];h[i]=std::max(0.f,sum);}for(int i=0;i<4;++i){float sum=cb1[i];for(int j=0;j<48;++j)sum+=cw1[i*48+j]*h[j];out.priority[i]=sum;}
 for(int cls=0;cls<4;++cls){std::array<float,80> a{};std::array<float,48> b{};for(int i=0;i<80;++i){float sum=rb0[i]+rw0[i*159+155+cls];for(int j=0;j<155;++j)sum+=rw0[i*159+j]*x[j];a[i]=std::max(0.f,sum);}for(int i=0;i<48;++i){float sum=rb1[i];for(int j=0;j<80;++j)sum+=rw1[i*80+j]*a[j];b[i]=std::max(0.f,sum);}for(int i=0;i<30;++i){float sum=rb2[i];for(int j=0;j<48;++j)sum+=rw2[i*48+j]*b[j];out.genes[cls][i]=std::clamp(sum,0.f,1.f);}}
 return out;
}
inline MultiProposal multiProposal(const FitTarget& t){FeatureExtractor ex;if(t.rate==22050)return inferMultiProposal(neuralFeatures(ex.extract(t.left,t.right,t.rate),t.note,t.left.size()/t.rate));Sound s;s.sampleRate=t.rate;s.left=t.left;s.right=t.right;auto target=makeTarget(s,t.note,22050,false);return inferMultiProposal(neuralFeatures(ex.extract(target.left,target.right,target.rate),target.note,target.left.size()/target.rate));}
}
