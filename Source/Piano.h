#include "GlassStyle.h"
#include "MotionState.h"
class Piano final:public juce::Component {
    juce::MidiKeyboardState& state;int down=-1,base=48;juce::Colour accent{0xff40a9ff};std::array<float,128> light{};
    static bool black(int note){int n=note%12;return n==1||n==3||n==6||n==8||n==10;}
    juce::Rectangle<float> rect(int note) const {
        int whites=0;for(int n=base;n<note;++n)if(!black(n))++whites;
        float w=getWidth()/14.f;
        if(black(note))return {(whites-.32f)*w,0,w*.64f,getHeight()*.61f};
        return {whites*w,0,w,getHeight()*1.f};
    }
    int at(juce::Point<float> p) const {
        for(int n=base;n<base+24;++n)if(black(n)&&rect(n).contains(p))return n;
        for(int n=base;n<base+24;++n)if(!black(n)&&rect(n).contains(p))return n;return -1;
    }
    void play(int next) {if(next==down)return;if(down>=0)state.noteOff(1,down,0);down=next;if(down>=0)state.noteOn(1,down,.8f);repaint();}
public:
    explicit Piano(juce::MidiKeyboardState& s):state(s){}
    void setBase(int n){if(n!=base){play(-1);base=n;repaint();}}
    void release(){play(-1);}
    void advance(double dt,bool animated){for(int n=0;n<128;++n){float target=state.isNoteOnForChannels(0xffff,n)?1.f:0.f;light[n]=animated?nova::easeMotion(light[n],target,dt,target>.5f?.025:.16):target;}}
    void setAccent(juce::Colour c){accent=c;repaint();}
    ~Piano() override {if(down>=0)state.noteOff(1,down,0);}
    void paint(juce::Graphics& g) override {
        for(int pass=0;pass<2;++pass)for(int n=base;n<base+24;++n)if(black(n)==(pass==1)){
            auto r=rect(n).reduced(.7f);if(light[n]>.01f)nova::glass::halo(g,r.expanded(5),accent,.35f*light[n]);
            nova::glass::panel(g,r,accent,pass?5.f:7.f,this);g.setColour((n==down||light[n]>.01f)?accent.withAlpha(.48f*std::max(n==down?1.f:0.f,light[n])):(pass?juce::Colours::black.withAlpha(.48f):juce::Colours::white.withAlpha(.13f)));g.fillRoundedRectangle(r,pass?5.f:7.f);g.setColour(juce::Colours::white.withAlpha(pass?.10f:.24f));g.drawRoundedRectangle(r.reduced(.5f),pass?5.f:7.f,.8f);
            if(n%12==0){g.setColour(juce::Colour(0xffa0a0a0));g.setFont(12.f);g.drawText("C / "+juce::String(n),r.removeFromBottom(19),juce::Justification::centred);}
        }
    }
    void mouseDown(const juce::MouseEvent& e) override{play(at(e.position));}
    void mouseDrag(const juce::MouseEvent& e) override{play(at(e.position));}
    void mouseUp(const juce::MouseEvent&) override{play(-1);}
};

