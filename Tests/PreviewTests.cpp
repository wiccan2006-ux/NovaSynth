#include "SelectionPreview.h"
#include <iostream>
#include <stdexcept>
void check(bool ok,const char* text){if(!ok)throw std::runtime_error(text);}
std::unique_ptr<nova::SelectionPreview::Clip> clip(uint64_t id,double rate=48000){auto c=std::make_unique<nova::SelectionPreview::Clip>();c->generation=id;c->start=30.125;c->sound.sampleRate=rate;c->sound.left.resize(static_cast<size_t>(rate*.5));c->sound.right=c->sound.left;for(size_t i=0;i<c->sound.left.size();++i){c->sound.left[i]=static_cast<float>(.3*std::sin(2*nova::pi*440*i/rate));c->sound.right[i]=c->sound.left[i]*.5f;}return c;}
int main(){try{
 for(double rate:{8000.,44100.,48000.,96000.}){
  nova::SelectionPreview p;auto id=p.stop();p.publish(clip(id,rate));std::vector<float> l(4800),r(4800);p.render(l.data(),r.data(),4800,48000);check(p.playing.load(),"audition starts");check(std::abs(p.position.load()-30.225)<1e-8,"cursor in original source seconds");double error=0;for(int i=500;i<4800;++i){double expected=.3*std::sin(2*nova::pi*440*i/48000);error+=std::pow(l[i]-expected,2);check(std::abs(l[i]*.5-r[i])<1e-6,"stereo channels preserved");}check(std::sqrt(error/4300)<.003,"preview keeps original pitch at host sample rate");
  p.stop();std::fill(l.begin(),l.end(),0);p.render(l.data(),r.data(),4800,48000);check(!p.playing.load()&&p.position.load()<0,"stop clears state");check(*std::max_element(l.begin(),l.end())==0,"stopped preview is silent");p.collect();
  auto stale=id;p.publish(clip(stale,rate));check(!p.pending(),"outdated read is discarded");id=p.stop();p.publish(clip(id,rate));std::vector<float> full(30000),right(30000);p.render(full.data(),right.data(),30000,48000);check(!p.playing.load()&&p.position.load()<0,"selection ends automatically");for(int i=24001;i<30000;++i)check(full[i]==0,"no audio after selection end");
 }
 // Preview mixes into existing instrument audio instead of resetting it.
 nova::SelectionPreview p;auto id=p.stop();p.publish(clip(id));std::vector<float> l(512,.1f),r(512,.2f);p.render(l.data(),r.data(),512,48000);check(std::abs(l[0]-.1)<1e-6&&std::abs(r[0]-.2)<1e-6,"mix preserves instrument buffer");
 std::cout<<"PASS: selection audition at 8/44.1/48/96 kHz, original pitch, stereo, cursor, stop, end, stale reads and mixing\n";
}catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}}
