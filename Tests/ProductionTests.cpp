#include "PresetTraits.h"
#include "CatalogData.h"
#include "AutoFit.h"
#include <set>
#include <iostream>
#include <stdexcept>
void check(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
int main(){try{
 std::set<int> principals;for(int i=0;i<nova::factoryPresetCount;++i){auto c=nova::catalog[i];check(c.representative>=0&&c.representative<812,"representative range");check(nova::catalog[c.representative].representative==c.representative,"one-level grouping");check(nova::presetRole(i)==nova::presetRole(c.representative),"variant role preserved");check(std::isfinite(c.trim)&&c.trim>=-12&&c.trim<=36,"calibration bounds");principals.insert(c.representative);check(!nova::presetTags(i).empty(),"searchable traits");}check(principals.size()==192,"192 principal sounds and 620 variants");
 nova::Params p;auto none=nova::modulation(p,1,-1,.8,.9,72,.4);check(none.cutoff==0&&none.amplitude==1&&none.morph==0&&none.pan==0,"neutral matrix");
 for(int source=1;source<=6;++source)for(int dest=1;dest<=4;++dest){p.routes[0]={source,dest,.5f};auto a=nova::modulation(p,.7f,.6f,.8f,.9f,72,.4f);check(a.cutoff!=0||a.morph!=0||a.amplitude!=1||a.pan!=0,"all 24 source-target combinations");}
 p.routes[0]={4,1,1};auto soft=nova::modulation(p,0,0,0,.2,60,0),hard=nova::modulation(p,0,0,0,.9,60,0);check(soft.cutoff<0&&hard.cutoff>0,"velocity bipolar response");p.routes[0].amount=-1;check(nova::modulation(p,0,0,0,.9,60,0).cutoff<0,"negative amount inverts source");
 nova::Engine e;e.prepare(48000);auto patch=nova::signaturePreset(4);patch.cutoff=1500;patch.movement=0;patch.touch=0;patch.character=patch.brightness=0;auto render=[&](nova::Params params){nova::FitTarget t;t.rate=48000;t.note=60;t.gate=.7f;t.left.resize(48000);t.right=t.left;std::vector<float> l(48000),r(48000);nova::renderCandidate(e,params,t,l,r);return l;};auto dry=render(patch);for(int dest=1;dest<=4;++dest){auto edited=patch;edited.routes[0]={2,dest,.7f};auto wet=render(edited);double difference=0;for(size_t i=0;i<wet.size();++i)difference+=std::abs(wet[i]-dry[i]);check(difference>.1,"each route target affects actual audio");}
 auto trim=patch;trim.presetTrim=6;auto louder=render(trim);double energy=0,dryEnergy=0;for(size_t i=0;i<dry.size();++i){energy+=louder[i]*louder[i];dryEnergy+=dry[i]*dry[i];}check(std::abs(10*std::log10(energy/dryEnergy)-6)<.01,"calibration gain is correct dB and repeatable");
 for(double sr:{22050.,48000.,96000.}){nova::OutputGuard guard;guard.prepare(sr);float l=2,r=-1;guard.tick(l,r,true);check(l<=.950001f&&std::abs(r+l*.5f)<.000001f,"stereo-linked peak protection");check(guard.reduction()>6,"reduction meter");float previous=guard.reduction();l=r=.1;guard.tick(l,r,true);check(guard.reduction()<previous&&guard.reduction()>0,"smooth recovery");l=2;r=1;guard.tick(l,r,false);check(l==2&&r==1&&guard.reduction()==0,"guard bypass");}
 // Dense chords with maximum macros/trim stay finite through guard.
 e.prepare(48000);patch.presetTrim=36;patch.character=patch.brightness=patch.movement=patch.space=1;e.configure(patch);for(int n=48;n<64;++n)e.noteOn(1,n,1);std::array<float,1024> l{},r{};nova::OutputGuard guard;guard.prepare(48000);for(int b=0;b<50;++b){e.render(l.data(),r.data(),1024);for(int i=0;i<1024;++i){guard.tick(l[i],r[i],true);check(std::isfinite(l[i])&&std::abs(l[i])<=.950001f&&std::abs(r[i])<=.950001f,"dense-chord protected output");}}
 std::cout<<"PASS: catalog coverage/192 representatives, calibration, 24 modulation routes, bipolar velocity, real audio, stereo guard/bypass/recovery and 16-note chord\n";
}catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}}
