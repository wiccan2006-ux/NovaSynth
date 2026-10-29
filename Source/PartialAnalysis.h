#pragma once
// Included after FitTarget/FeatureExtractor. Outputs oscillator parameters, never PCM.
namespace nova {
struct SoundProfile {int family=0;std::string description="Sin identificar";float attack=0,tonality=0;};
inline SoundProfile classifySound(const FitTarget& target,const Features& features){
    SoundProfile p;p.tonality=target.tonal?1.f:0.f;
    size_t peak=0;for(size_t i=1;i<features.envelope.size();++i)if(features.envelope[i]>features.envelope[peak])peak=i;
    p.attack=static_cast<float>(peak*target.left.size()/features.envelope.size()/target.rate);
    float early=0,late=0;for(int i=0;i<6;++i){early+=features.envelope[i];late+=features.envelope[i+18];}
    if(!target.tonal){p.family=7;p.description="Ruidoso / percusivo o tono incierto";}
    else if(target.note<48){p.family=0;p.description="Bajo tonal";}
    else if(peak>5){p.family=2;p.description="Ataque lento / sostenido tipo pad";}
    else if(late<early*.55f){p.family=3;p.description="Pulsado: posible tecla o cuerda";}
    else {p.family=1;p.description="Tono sostenido / lead";}
    return p;
}
inline SpectralModel estimatePartials(const FitTarget& target,std::atomic<bool>& cancel,bool track=true){
    SpectralModel model;model.root=target.note;model.duration=static_cast<float>(target.left.size()/target.rate);
    if(target.left.size()<128)return model;
    constexpr int N=8192;std::array<std::complex<double>,N> bins{};
    struct Peak{double frequency,power;};std::vector<Peak> peaks;
    for(int pass=0;pass<3&&!cancel.load();++pass){
        size_t start=static_cast<size_t>(std::max(0.,static_cast<double>(target.left.size())-N)*(pass*.4));double maxPower=0;
        for(int i=0;i<N;++i){size_t k=start+static_cast<size_t>(i);double x=k<target.left.size()?target.left[k]:0;bins[i]={x*(.5-.5*std::cos(2*pi*i/(N-1))),0};}
        for(int i=1,j=0;i<N;++i){int bit=N>>1;for(;j&bit;bit>>=1)j^=bit;j^=bit;if(i<j)std::swap(bins[i],bins[j]);}
        for(int len=2;len<=N;len<<=1){auto step=std::polar(1.,-2*pi/len);for(int base=0;base<N;base+=len){std::complex<double> w=1;for(int j=0;j<len/2;++j){auto a=bins[base+j],b=w*bins[base+j+len/2];bins[base+j]=a+b;bins[base+j+len/2]=a-b;w*=step;}}}
        for(int i=std::max(2,static_cast<int>(20*N/target.rate));i<N/2-1;++i)maxPower=std::max(maxPower,std::norm(bins[i]));
        for(int i=std::max(2,static_cast<int>(20*N/target.rate));i<N/2-1;++i){double a=std::norm(bins[i-1]),b=std::norm(bins[i]),c=std::norm(bins[i+1]);if(b<=a||b<c||b<maxPower*1e-6)continue;
            double la=std::log(std::max(a,1e-30)),lb=std::log(std::max(b,1e-30)),lc=std::log(std::max(c,1e-30));double denominator=la-2*lb+lc;double d=std::abs(denominator)>1e-12?.5*(la-lc)/denominator:0;
            peaks.push_back({(i+std::clamp(d,-.5,.5))*target.rate/N,b});}
    }
    std::sort(peaks.begin(),peaks.end(),[](const Peak& a,const Peak& b){return a.power>b.power;});
    for(const auto& peak:peaks){if(model.count==partialCount)break;bool distinct=true;for(int j=0;j<model.count;++j)if(std::abs(model.partials[j].frequency-peak.frequency)<std::max(20.,std::min(120.,440*std::pow(2.,(target.note-69)/12.)*.25)))distinct=false;if(distinct&&peak.frequency<target.rate*.44)model.partials[model.count++].frequency=static_cast<float>(peak.frequency);}
    std::sort(model.partials.begin(),model.partials.begin()+model.count,[](const Partial& a,const Partial& b){return a.frequency<b.frequency;});
    if(!track){
        for(int f=0;f<modelFrames;++f)model.times[f]=model.duration*static_cast<float>(std::pow(f/static_cast<double>(modelFrames-1),1.7));
        double baseFrequency=440*std::pow(2.,(target.note-69+target.fine/100.)/12.);int window=std::clamp(static_cast<int>(target.rate*std::max(.012,2/baseFrequency)),256,4096);
        for(int j=0;j<model.count&&!cancel.load();++j){auto& part=model.partials[j];double strongest=0;
            for(int frame=0;frame<modelFrames;++frame){int centre=static_cast<int>(model.times[frame]*target.rate);double c=0,ss=0,weightSum=0;
                for(int n=0;n<window;++n){int k=centre+n-window/2;double w=.5-.5*std::cos(2*pi*n/(window-1));weightSum+=w;if(k<0||static_cast<size_t>(k)>=target.left.size())continue;double angle=2*pi*part.frequency*k/target.rate,x=target.left[static_cast<size_t>(k)];c+=x*w*std::cos(angle);ss+=x*w*std::sin(angle);}
                double amp=2*std::hypot(c,ss)/std::max(1.,weightSum);part.amplitude[frame]=std::clamp(static_cast<float>(amp/.3),0.f,8.f);part.frequencyRatio[frame]=1;if(amp>strongest){strongest=amp;part.phase=static_cast<float>(std::atan2(c,ss));}
            }
        }model.analysisRate=static_cast<float>(target.rate);return model;
    }
    model.tracked=true;model.analysisRate=static_cast<float>(target.rate);
    for(int f=0;f<modelFrames;++f)model.times[f]=model.duration*static_cast<float>(std::pow(f/static_cast<double>(modelFrames-1),1.35));
    double baseFrequency=440*std::pow(2.,(target.note-69+target.fine/100.)/12.);
    int window=baseFrequency<140?4096:2048;
    std::array<std::complex<double>,4096> local{};
    auto transform=[&]{
        for(int i=1,j=0;i<window;++i){int bit=window>>1;for(;j&bit;bit>>=1)j^=bit;j^=bit;if(i<j)std::swap(local[i],local[j]);}
        for(int len=2;len<=window;len<<=1){auto step=std::polar(1.,-2*pi/len);for(int base=0;base<window;base+=len){std::complex<double> w=1;for(int j=0;j<len/2;++j){auto a=local[base+j],b=w*local[base+j+len/2];local[base+j]=a+b;local[base+j+len/2]=a-b;w*=step;}}}
    };
    std::array<float,partialCount> strongest{};
    for(int frame=0;frame<modelFrames&&!cancel.load();++frame){
        int centre=static_cast<int>(model.times[frame]*target.rate);double weightSum=0,weightSquared=0;
        for(int n=0;n<window;++n){int k=centre+n-window/2;double w=.5-.5*std::cos(2*pi*n/(window-1));weightSum+=w;weightSquared+=w*w;double x=k>=0&&static_cast<size_t>(k)<target.left.size()?target.left[static_cast<size_t>(k)]:0;local[n]={x*w,0};}
        transform();std::array<bool,2049> tonalBins{};
        for(int j=0;j<model.count;++j){auto& part=model.partials[j];
            double range=std::min(baseFrequency*.30,std::max(12.,part.frequency*.045));int a=std::clamp(static_cast<int>((part.frequency-range)*window/target.rate),2,window/2-2),z=std::clamp(static_cast<int>((part.frequency+range)*window/target.rate+.5),a,window/2-2),best=a;
            for(int k=a+1;k<=z;++k)if(std::norm(local[k])>std::norm(local[best]))best=k;
            double la=std::log(std::max(1e-30,std::norm(local[best-1]))),lb=std::log(std::max(1e-30,std::norm(local[best]))),lc=std::log(std::max(1e-30,std::norm(local[best+1]))),den=la-2*lb+lc;
            double offset=std::abs(den)>1e-12?.5*(la-lc)/den:0,frequency=(best+std::clamp(offset,-.5,.5))*target.rate/window;
            // Restrict each trajectory to its own frequency neighbourhood; silence holds the seed.
            frequency=std::clamp(frequency,part.frequency-range,part.frequency+range);part.frequencyRatio[frame]=static_cast<float>(frequency/part.frequency);
            double c=0,ss=0;
            for(int n=0;n<window;++n){int k=centre+n-window/2;if(k<0||static_cast<size_t>(k)>=target.left.size())continue;double w=.5-.5*std::cos(2*pi*n/(window-1)),angle=2*pi*frequency*k/target.rate,x=target.left[static_cast<size_t>(k)];c+=x*w*std::cos(angle);ss+=x*w*std::sin(angle);}
            float amplitude=static_cast<float>(2*std::hypot(c,ss)/std::max(1.,weightSum));part.amplitude[frame]=std::clamp(amplitude/.3f,0.f,8.f);
            if(amplitude>strongest[j]){strongest[j]=amplitude;part.phase=static_cast<float>(std::atan2(c,ss));}
            for(int k=std::max(1,best-2);k<=std::min(window/2,best+2);++k)tonalBins[k]=true;
        }
        // Residual energy becomes filter-bank envelopes, never stored audio samples.
        for(int band=0;band<noiseBands;++band){double lo=40*std::pow(target.rate*.44/40,band/static_cast<double>(noiseBands)),hi=40*std::pow(target.rate*.44/40,(band+1.)/noiseBands),energy=0;int a=std::clamp(static_cast<int>(std::ceil(lo*window/target.rate)),1,window/2-1),z=std::clamp(static_cast<int>(hi*window/target.rate),a,window/2-1),kept=0;
            for(int k=a;k<=z;++k)if(!tonalBins[k]){energy+=std::norm(local[k]);++kept;}
            double correction=kept>0?std::min(3.,(z-a+1.)/kept):0;model.noise[band][frame]=std::clamp(static_cast<float>(std::sqrt(2*energy*correction/std::max(1.,window*weightSquared))/.3),0.f,8.f);
        }
    }
    return model;
}
struct Comparison {
    std::array<float,256> referenceWave{},resultWave{};
    std::array<float,128> referenceSpectrum{},resultSpectrum{};
    std::array<float,24> referenceEnvelope{},resultEnvelope{};
    bool valid=false;float sampleRate=22050;
};
inline Comparison compareSignals(const FitTarget& target,const std::vector<float>& l,const std::vector<float>& r){
    Comparison c;c.sampleRate=static_cast<float>(target.rate);FeatureExtractor e;auto a=e.extract(target.left,target.right,target.rate),b=e.extract(l,r,target.rate);c.referenceSpectrum=a.spectrum;c.resultSpectrum=b.spectrum;c.referenceEnvelope=a.envelope;c.resultEnvelope=b.envelope;
    // Short waveform view: the same first 30 ms on each independently peak-normalised trace.
    float peakA=1e-9f,peakB=1e-9f;for(int i=0;i<256;++i){size_t k=std::min(target.left.size()-1,static_cast<size_t>(i*target.rate*.03/256));c.referenceWave[i]=target.left[k];c.resultWave[i]=l[k];peakA=std::max(peakA,std::abs(c.referenceWave[i]));peakB=std::max(peakB,std::abs(c.resultWave[i]));}
    for(int i=0;i<256;++i){c.referenceWave[i]/=peakA;c.resultWave[i]/=peakB;}c.valid=true;return c;
}
}
