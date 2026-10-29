#pragma once
#include <array>
#include <cmath>
namespace nova {
// Band-limited procedural tables. No PCM from the reference is placed here.
struct WaveTables {
    static constexpr int size=1024,levels=7;
    std::array<std::array<std::array<float,size+1>,levels>,5> data{};
    WaveTables(){
        constexpr double tau=6.2831853071795864769;
        for(int wave=0;wave<5;++wave)for(int level=0;level<levels;++level){int limit=64>>level;
            for(int i=0;i<size;++i){double phase=tau*i/size,value=0;
                for(int h=1;h<=limit;++h){double a=0;
                    if(wave==0)a=h==1?1:0;
                    if(wave==1)a=-2./(3.141592653589793*h);
                    if(wave==2)a=h%2?4./(3.141592653589793*h):0;
                    if(wave==3)a=h%2?8./(3.141592653589793*3.141592653589793*h*h)*(h%4==1?1:-1):0;
                    if(wave==4)a=h==1?.7:(h==2?.3:(h==3?.12:0));
                    value+=a*std::sin(h*phase);
                }data[wave][level][i]=static_cast<float>(value);
            }data[wave][level][size]=data[wave][level][0];
        }
    }
    float read(int wave,double phase,double dt)const{
        int level=0;while(level<levels-1&&(64>>level)*dt>.44)++level;
        double pos=phase*size;int a=std::clamp(static_cast<int>(pos),0,size-1);float t=static_cast<float>(pos-a);const auto& table=data[std::clamp(wave,0,4)][level];return table[a]*(1-t)+table[a+1]*t;
    }
};
inline const WaveTables& waveTables(){static const WaveTables tables;return tables;}
// Six pairs of harmonic spectra. Crossfading changes timbre, never frequency.
struct SignatureTables {
    std::array<std::array<std::array<std::array<float,WaveTables::size+1>,WaveTables::levels>,2>,6> data{};
    SignatureTables(){
        constexpr double tau=6.2831853071795864769;
        for(int family=0;family<6;++family)for(int end=0;end<2;++end)for(int level=0;level<WaveTables::levels;++level){
            int limit=64>>level;std::array<double,65> amplitudes{};double sum=0;
            for(int h=1;h<=limit;++h){double a=0,x=h;
                switch(family){
                case 0:a=(.12+std::exp(-std::pow((x-(end?8:3))/1.8,2))+.55*std::exp(-std::pow((x-(end?15:7))/2.4,2)))/std::pow(x,.65);break;
                case 1:a=(h==1?1.:.62*std::exp(-x/(end?14.:5.))*(h%(end?3:2)? .17:1.));break;
                case 2:a=(h%2?1.:end?.72:.08)*std::exp(-x/(end?28.:8.))/std::pow(x,.8);break;
                case 3:a=(.14+.86*std::pow(std::sin(x*(end?1.13:.63)),2))/std::pow(x,end?.75:1.5);break;
                case 4:a=(h==1?1.:std::exp(-std::pow((x-(end?11:5))/3.,2))*.8)/std::sqrt(x);break;
                case 5:a=(h==1?1.:((h+end)%4==0?.9:.06))/std::pow(x,end?.8:1.15);break;
                }amplitudes[h]=a;sum+=a;
            }
            for(int i=0;i<WaveTables::size;++i){double value=0;for(int h=1;h<=limit;++h)value+=amplitudes[h]*std::sin(tau*h*i/WaveTables::size);data[family][end][level][i]=static_cast<float>(value/std::max(1.,sum)*1.35);}
            data[family][end][level][WaveTables::size]=data[family][end][level][0];
        }
    }
    float read(int family,int end,double phase,double dt)const{
        int level=0;while(level<WaveTables::levels-1&&(64>>level)*dt>.44)++level;
        double pos=phase*WaveTables::size;int i=std::clamp(static_cast<int>(pos),0,WaveTables::size-1);float t=static_cast<float>(pos-i);const auto& table=data[std::clamp(family,0,5)][std::clamp(end,0,1)][level];return table[i]*(1-t)+table[i+1]*t;
    }
};
inline const SignatureTables& signatureTables(){static const SignatureTables tables;return tables;}
}
