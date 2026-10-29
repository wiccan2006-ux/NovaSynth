#pragma once
#include "NeuralWeights.h"
namespace nova {
// CPU-only inference; called in the analysis worker, never in processBlock.
inline std::array<float,155> neuralFeatures(const Features& f,int note,double duration){
    std::array<float,155> x{};std::copy(f.spectrum.begin(),f.spectrum.end(),x.begin());std::copy(f.envelope.begin(),f.envelope.end(),x.begin()+128);x[152]=f.width;x[153]=(note-60)/24.f;x[154]=static_cast<float>(duration/3);return x;
}
inline Genes inferNeuralSeed(std::array<float,155> x){
    using namespace neural_weights;for(size_t i=0;i<x.size();++i)x[i]=std::clamp((x[i]-mean[i])/std::max(scale[i],1e-8f),-12.f,12.f);
    std::array<float,96> h{};std::array<float,64> z{};Genes result{};
    for(int i=0;i<96;++i){float sum=b0[i];for(int j=0;j<155;++j)sum+=w0[i*155+j]*x[j];h[i]=std::max(0.f,sum);}
    for(int i=0;i<64;++i){float sum=b1[i];for(int j=0;j<96;++j)sum+=w1[i*96+j]*h[j];z[i]=std::max(0.f,sum);}
    for(int i=0;i<30;++i){float sum=b2[i];for(int j=0;j<64;++j)sum+=w2[i*64+j]*z[j];result[i]=std::clamp(sum,0.f,1.f);}return result;
}
inline Genes neuralSeed(const FitTarget& target,FeatureExtractor& extractor){
    if(target.rate==22050)return inferNeuralSeed(neuralFeatures(extractor.extract(target.left,target.right,target.rate),target.note,target.left.size()/target.rate));
    Sound s;s.sampleRate=target.rate;s.left=target.left;s.right=target.right;auto resampled=makeTarget(s,target.note,22050);
    return inferNeuralSeed(neuralFeatures(extractor.extract(resampled.left,resampled.right,resampled.rate),target.note,resampled.left.size()/resampled.rate));
}
}
