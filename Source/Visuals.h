#pragma once
#include "Processor.h"
#include "GlassStyle.h"
#include "MotionState.h"
class NovaLook final:public juce::LookAndFeel_V4{
public:
    float textScale=1.f,glow=.7f;
    juce::Colour accent{0xff40a9ff};
    juce::Font getTextButtonFont(juce::TextButton&,int height)override{return juce::Font(std::min(16.5f*textScale,height*.64f));}
    void advance(juce::Component& root,double dt,bool enabled){
        for(int i=0;i<root.getNumChildComponents();++i){auto* c=root.getChildComponent(i);
            if(auto* b=dynamic_cast<juce::Button*>(c)){float target=b->isMouseOverOrDragging()?1.f:0.f;float h=b->getProperties().getWithDefault("novaHover",0.f);b->getProperties().set("novaHover",enabled?nova::easeMotion(h,target,dt,.085):target);
                float targetOn=b->getToggleState()?1.f:0.f;float on=b->getProperties().getWithDefault("novaOn",targetOn);b->getProperties().set("novaOn",enabled?nova::easeMotion(on,targetOn,dt,.065):targetOn);}
            advance(*c,dt,enabled);
        }
    }
    void drawButtonBackground(juce::Graphics& g,juce::Button& button,const juce::Colour&,bool highlighted,bool down)override{
        auto area=button.getLocalBounds().toFloat().reduced(1.5f);float radius=area.getHeight()*.5f;bool active=button.getToggleState()||down;float hover=button.getProperties().getWithDefault("novaHover",highlighted?1.f:0.f);
        if(active||hover>.005f)nova::glass::halo(g,area.expanded(6.f),accent,(active?.24f:.16f*hover)*glow);
        nova::glass::panel(g,area,accent,radius,&button);
        if(active||hover>.005f){g.setColour(accent.withAlpha(active?.27f:.10f*hover));g.fillRoundedRectangle(area.reduced(1),radius);g.setColour(accent.withAlpha(active?.65f:.28f*hover));g.drawRoundedRectangle(area.reduced(1),radius,1);}
    }
    void drawToggleButton(juce::Graphics& g,juce::ToggleButton& button,bool highlighted,bool down)override{
        float y=button.getHeight()*.5f;bool on=button.getToggleState();float alpha=button.isEnabled()?1.f:.38f;juce::Rectangle<float> track(3,y-9,32,18);
        nova::glass::panel(g,track,accent,9,&button);if(on){g.setColour(accent.withAlpha(.68f*alpha));g.fillRoundedRectangle(track.reduced(1),8);nova::glass::halo(g,track.expanded(5),accent,.16f*alpha);}
        if(highlighted||down){g.setColour(juce::Colours::white.withAlpha(.08f));g.fillRoundedRectangle(track,9);}
        float amount=button.getProperties().getWithDefault("novaOn",on?1.f:0.f);float x=12+14*amount;g.setColour(juce::Colours::black.withAlpha(.15f));g.fillEllipse(x-7,y-6,14,14);g.setColour(juce::Colours::white.withAlpha(.94f*alpha));g.fillEllipse(x-7,y-7,14,14);
        g.setColour(juce::Colours::white.withAlpha(.90f*alpha));g.setFont(12.5f*textScale);g.drawFittedText(button.getButtonText(),41,0,button.getWidth()-43,button.getHeight(),juce::Justification::centredLeft,1,.85f);
    }
    void drawRotarySlider(juce::Graphics& g,int x,int y,int w,int h,float value,float start,float end,juce::Slider& slider)override{
        const float side=static_cast<float>(std::min(w,h));if(side<8)return;const float cx=x+w*.5f,cy=y+h*.5f,radius=side*.34f;float alpha=slider.isEnabled()?1.f:.35f;
        nova::glass::halo(g,{cx-side*.48f,cy-side*.48f,side*.96f,side*.96f},accent,.25f*alpha*glow);
        juce::Path ring;ring.addCentredArc(cx,cy,radius,radius,0,start,end,true);g.setColour(juce::Colours::white.withAlpha(.15f*alpha));g.strokePath(ring,juce::PathStrokeType(5.f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
        juce::Path lit;lit.addCentredArc(cx,cy,radius,radius,0,start,start+(end-start)*value,true);nova::glass::strokeGlow(g,lit,accent.withMultipliedAlpha(alpha),2.5f,&slider);
        auto disc=juce::Rectangle<float>(cx-radius*.79f,cy-radius*.79f,radius*1.58f,radius*1.58f);
        nova::glass::panel(g,disc,accent,disc.getWidth()*.5f,&slider);
        auto shine=disc.reduced(disc.getWidth()*.17f);shine.setHeight(shine.getHeight()*.26f);juce::ColourGradient highlight(juce::Colours::white.withAlpha(.21f*alpha),cx,shine.getY(),juce::Colours::transparentWhite,cx,shine.getBottom(),false);g.setGradientFill(highlight);g.fillEllipse(shine);
        float angle=start+(end-start)*value;juce::Path needle;needle.startNewSubPath(cx,cy);needle.lineTo(cx+std::sin(angle)*radius*.58f,cy-std::cos(angle)*radius*.58f);nova::glass::strokeGlow(g,needle,accent.withMultipliedAlpha(alpha),2.f,&slider);
    }
    void drawLinearSlider(juce::Graphics& g,int x,int y,int w,int h,float pos,float minimum,float maximum,juce::Slider::SliderStyle style,juce::Slider& slider)override{
        if(style!=juce::Slider::LinearHorizontal){juce::LookAndFeel_V4::drawLinearSlider(g,x,y,w,h,pos,minimum,maximum,style,slider);return;}
        float start=x+6.f,end=x+w-6.f,cy=y+h*.5f,tip=std::clamp(pos,start,end);juce::Path track;track.startNewSubPath(start,cy);track.lineTo(end,cy);g.setColour(juce::Colour(0xff353a45));g.strokePath(track,juce::PathStrokeType(5.f,juce::PathStrokeType::curved,juce::PathStrokeType::rounded));
        juce::Path lit;lit.startNewSubPath(start,cy);lit.lineTo(tip,cy);nova::glass::strokeGlow(g,lit,accent.withMultipliedAlpha(slider.isEnabled()?1.f:.35f),2.5f,&slider);nova::glass::halo(g,{tip-13.f,cy-13.f,26.f,26.f},accent,.35f);g.setColour(juce::Colours::white.withAlpha(.94f));g.fillEllipse(tip-3.f,cy-3.f,6.f,6.f);
    }
    void drawTickBox(juce::Graphics& g,juce::Component& component,float x,float y,float w,float h,bool ticked,bool enabled,bool highlighted,bool down)override{
        if(ticked)nova::glass::halo(g,{x-5,y-5,w+10,h+10},accent,enabled?.34f:.12f);juce::LookAndFeel_V4::drawTickBox(g,component,x,y,w,h,ticked,enabled,highlighted,down);
    }
    void drawComboBox(juce::Graphics& g,int w,int h,bool,int,int,int,int,juce::ComboBox& box)override{
        auto area=juce::Rectangle<float>(0,0,static_cast<float>(w),static_cast<float>(h)).reduced(1.f);nova::glass::panel(g,area,accent,area.getHeight()*.48f,&box);
        juce::Path arrow;float x=static_cast<float>(w-16),y=h*.5f;arrow.startNewSubPath(x-4,y-2);arrow.lineTo(x,y+2);arrow.lineTo(x+4,y-2);g.setColour(accent.withAlpha(box.isEnabled()?.9f:.35f));g.strokePath(arrow,juce::PathStrokeType(1.7f));
    }
    void positionComboBoxText(juce::ComboBox& box,juce::Label& label)override{label.setBounds(12,1,std::max(1,box.getWidth()-36),box.getHeight()-2);label.setFont(juce::Font(13.f*textScale));}
    void drawPopupMenuBackground(juce::Graphics& g,int width,int height)override{nova::glass::panel(g,{0,0,static_cast<float>(width),static_cast<float>(height)},accent,14);g.setColour(juce::Colour(0xff080d17).withAlpha(.45f));g.fillRoundedRectangle(juce::Rectangle<float>(0,0,static_cast<float>(width),static_cast<float>(height)),14);}
    void theme(int index){const juce::Colour colours[]={juce::Colour(0xff40a9ff),juce::Colour(0xffffcf44),juce::Colour(0xff4be291),juce::Colour(0xffb789ff),juce::Colour(0xffff4eb6),juce::Colour(0xfff5fbff),juce::Colour(0xffff8b24)};accent=colours[std::clamp(index,0,6)];
        setColour(juce::ResizableWindow::backgroundColourId,juce::Colour(0xff050505));setColour(juce::Label::textColourId,juce::Colour(0xffd7d7d7));setColour(juce::Slider::rotarySliderFillColourId,accent);setColour(juce::Slider::rotarySliderOutlineColourId,juce::Colour(0xff292929));setColour(juce::Slider::thumbColourId,accent);setColour(juce::Slider::textBoxBackgroundColourId,juce::Colour(0x35141d2b));setColour(juce::Slider::textBoxTextColourId,juce::Colour(0xffededed));setColour(juce::Slider::textBoxOutlineColourId,juce::Colour(0x32ffffff));setColour(juce::TextButton::buttonColourId,juce::Colour(0xff141414));setColour(juce::TextButton::buttonOnColourId,accent.darker(.6f));setColour(juce::TextButton::textColourOffId,juce::Colour(0xffdddddd));setColour(juce::TextButton::textColourOnId,juce::Colours::white);setColour(juce::ToggleButton::textColourId,juce::Colour(0xffdddddd));setColour(juce::ToggleButton::tickColourId,accent);
        setColour(juce::ComboBox::backgroundColourId,juce::Colour(0xff101010));setColour(juce::ComboBox::textColourId,juce::Colours::white);setColour(juce::ComboBox::outlineColourId,juce::Colour(0xff303030));setColour(juce::PopupMenu::backgroundColourId,juce::Colour(0xff111111));setColour(juce::PopupMenu::textColourId,juce::Colours::white);setColour(juce::PopupMenu::highlightedBackgroundColourId,accent.darker(.6f));setColour(juce::PopupMenu::highlightedTextColourId,juce::Colours::white);
    }
};
class SignalView final:public juce::Component{
    NovaProcessor& p;
public:
    nova::MotionState motion;
    void advance(double dt,bool enabled){motion.advance(dt,p.peak.load(),p.fitting.load()||p.loading.load(),enabled);}
    int mode=1;juce::Colour accent{0xff40a9ff};
    void paintOverChildren(juce::Graphics& g)override{
        if(p.animated.load()){
            if(motion.activity>.01f){float x=12+static_cast<float>(motion.phase)*(getWidth()-94);nova::glass::halo(g,{x,-3,94,17},accent,.4f*motion.activity);}
            g.setColour(accent.withAlpha(.75f));g.fillRoundedRectangle(12.f,getHeight()-6.f,(getWidth()-24)*motion.level,2.f,1.f);
            if(mode!=3&&motion.level>.006f){auto box=juce::Rectangle<float>(getWidth()-202.f,26.f,186.f,74.f);nova::glass::panel(g,box,accent,12,this);auto live=p.liveWave();juce::Path trace;for(int i=0;i<128;++i){float x=box.getX()+8+i*(box.getWidth()-16)/127,y=box.getCentreY()+8-live[i*2]*22;if(i==0)trace.startNewSubPath(x,y);else trace.lineTo(x,y);}nova::glass::strokeGlow(g,trace,accent,1.2f,this);g.setFont(11.f);g.setColour(juce::Colours::white);g.drawText("SALIDA EN VIVO",box.reduced(8).removeFromTop(16),juce::Justification::centredLeft);}
        }
    }
    explicit SignalView(NovaProcessor& processor):p(processor){}
    void paint(juce::Graphics& g)override{
        auto area=getLocalBounds().toFloat();nova::glass::panel(g,area,accent,18,this);
        area=area.reduced(12,20);g.setColour(juce::Colour(0xff272727));for(int i=1;i<4;++i)g.drawHorizontalLine(static_cast<int>(area.getY()+area.getHeight()*i/4),area.getX(),area.getRight());
        if(mode>=4){
            auto model=p.modelView();g.setFont(12.f);g.setColour(juce::Colour(0xffb0b0b0));
            if(!model.valid()){g.drawText("Sin modelo de resintesis: carga una referencia y espera AutoFit",12,3,900,18,juce::Justification::centredLeft);return;}
            if(mode==4){
                int selected=0;float strongest=0;for(int j=0;j<model.count;++j){float a=*std::max_element(model.partials[j].amplitude.begin(),model.partials[j].amplitude.end());if(a>strongest){strongest=a;selected=j;}}
                const auto& partial=model.partials[selected];juce::Path path;float limit=20;std::array<float,nova::modelFrames> cents{};
                for(int i=0;i<nova::modelFrames;++i){cents[i]=model.tracked?1200*std::log2(partial.frequencyRatio[i]):0;limit=std::max(limit,std::abs(cents[i]));}
                for(int i=0;i<nova::modelFrames;++i){float x=area.getX()+area.getWidth()*model.times[i]/model.duration,y=area.getCentreY()-cents[i]/limit*area.getHeight()*.45f;if(i==0)path.startNewSubPath(x,y);else path.lineTo(x,y);}g.setColour(accent);g.strokePath(path,juce::PathStrokeType(2));g.drawText("MOVIMIENTO ANALIZADO / P"+juce::String(selected+1)+" / "+juce::String(partial.frequency,1)+" Hz / escala +/- "+juce::String(limit,1)+" cents",12,2,getWidth()-24,17,juce::Justification::centredLeft);
            }else{
                std::array<float,nova::noiseBands> levels{};float maximum=.00001f;for(int b=0;b<nova::noiseBands;++b){for(float a:model.noise[b])levels[b]+=a*a;levels[b]=std::sqrt(levels[b]/nova::modelFrames);maximum=std::max(maximum,levels[b]);}
                for(int b=0;b<nova::noiseBands;++b){float width=area.getWidth()/nova::noiseBands,h=area.getHeight()*levels[b]/maximum;g.setColour(accent);g.fillRect(area.getX()+b*width,area.getBottom()-h,width-4,h);}g.drawText("RUIDO ANALIZADO / 12 BANDAS / 40 Hz - "+juce::String(model.analysisRate*.44/1000,1)+" kHz / niveles relativos",12,2,getWidth()-24,17,juce::Justification::centredLeft);
            }return;
        }
        auto c=p.plots();auto draw=[&](const auto& values,juce::Colour colour,bool signedSignal,int start,int count,float maximum){juce::Path path;for(int i=0;i<count;++i){float value=values[static_cast<size_t>(i+start)]/std::max(maximum,.00001f);float x=area.getX()+area.getWidth()*i/std::max(1,count-1),y=signedSignal?area.getCentreY()-value*area.getHeight()*.46f:area.getBottom()-value*area.getHeight()*.92f;if(i==0)path.startNewSubPath(x,y);else path.lineTo(x,y);}if(colour==accent)nova::glass::strokeGlow(g,path,accent,2.f,this);else{g.setColour(colour);g.strokePath(path,juce::PathStrokeType(1.2f));}};
        if(mode==3){auto live=p.liveWave();draw(live,accent,true,0,256,1);g.setColour(juce::Colour(0xffaaaaaa));g.setFont(11.f);g.drawText("SALIDA REAL / EN VIVO",12,3,500,16,juce::Justification::centredLeft);return;}
        if(!c.valid){auto wave=p.waveform();float maximum=.01f;for(float x:wave)maximum=std::max(maximum,x);draw(wave,juce::Colour(0xff999999),false,0,256,maximum);g.setColour(juce::Colour(0xffaaaaaa));g.setFont(12.f);g.drawText(p.hasReference()?"REFERENCIA CARGADA / marca el fragmento en MUESTRA y pulsa Recrear seleccion":"ARRASTRA UNA NOTA AISLADA PARA ANALIZAR",12,3,900,16,juce::Justification::centredLeft);return;}
        if(mode==0){draw(c.referenceWave,juce::Colour(0xff999999),true,0,256,1);draw(c.resultWave,accent,true,0,256,1);}
        else if(mode==1){float maximum=.1f;for(int i=96;i<128;++i)maximum=std::max(maximum,std::max(c.referenceSpectrum[i],c.resultSpectrum[i]));draw(c.referenceSpectrum,juce::Colour(0xff999999),false,96,32,maximum);draw(c.resultSpectrum,accent,false,96,32,maximum);}
        else{float maximum=.1f;for(int i=0;i<24;++i)maximum=std::max(maximum,std::max(c.referenceEnvelope[i],c.resultEnvelope[i]));draw(c.referenceEnvelope,juce::Colour(0xff999999),false,0,24,maximum);draw(c.resultEnvelope,accent,false,0,24,maximum);}
        g.setFont(11.f);g.setColour(juce::Colour(0xffb0b0b0));g.drawText(mode==0?"ONDA / 30 ms / cada traza normalizada":mode==1?"ESPECTRO NORMALIZADO / 35 Hz - "+juce::String(c.sampleRate*.45f/1000,1)+" kHz":"ENVOLVENTE RMS / duracion de analisis",12,2,650,16,juce::Justification::centredLeft);g.drawText("REFERENCIA",getWidth()-190,2,90,16,juce::Justification::centredRight);g.setColour(accent);g.drawText("RESULTADO",getWidth()-95,2,83,16,juce::Justification::centredRight);
    }
};
class PartialView final:public juce::Component,public juce::SettableTooltipClient{
    NovaProcessor& p;int selected=0,editing=-1;
    void update(juce::Point<float> point){int i=std::clamp(static_cast<int>((point.x-8)/(getWidth()-16)*nova::partialCount),0,nova::partialCount-1);selected=i;if(editing>=0)i=editing;float gain=std::clamp((166-point.y)/142*2,0.f,2.f);p.set(NovaProcessor::partialId(i).toRawUTF8(),gain);repaint();}
public:
    juce::Colour accent{0xff40a9ff};
explicit PartialView(NovaProcessor& processor):p(processor){}
    void paint(juce::Graphics& g)override{
        nova::glass::panel(g,getLocalBounds().toFloat(),accent,18,this);auto model=p.modelView();g.setFont(13.f);g.setColour(juce::Colour(0xffcfcfcf));
        if(!model.valid()){g.drawText("La busqueda puede elegir FM o un banco de hasta 48 osciladores. Aun no hay banco para editar.",getLocalBounds(),juce::Justification::centred);return;}
        g.drawText("PARCIALES / arrastra: ganancia 0-2 / doble clic: restablecer / curva inferior: envolvente del parcial",8,0,getWidth()-16,22,juce::Justification::centredLeft);
        float maximum=.00001f;for(int i=0;i<model.count;++i)for(float x:model.partials[i].amplitude)maximum=std::max(maximum,x);
        float width=(getWidth()-16.f)/nova::partialCount;
        for(int i=0;i<nova::partialCount;++i){float x=8+i*width;float gain=p.state.getRawParameterValue(NovaProcessor::partialId(i))->load();g.setColour(juce::Colour(0xff252525));g.fillRect(x,25.f,width-2,141.f);if(i<model.count){float a=*std::max_element(model.partials[i].amplitude.begin(),model.partials[i].amplitude.end())/maximum;float h=std::min(1.f,a*gain)*140;g.setColour(i==selected?accent.brighter(.25f):accent.darker(.25f));g.fillRect(x,166-h,width-2,h);if(i%4==0){g.setFont(10.f);g.setColour(juce::Colour(0xffbbbbbb));g.drawText(juce::String(i+1),static_cast<int>(x),168,static_cast<int>(width+8),14,juce::Justification::centredLeft);}}}
        selected=std::clamp(selected,0,model.count-1);const auto& part=model.partials[selected];juce::Path curve;float peak=.00001f;for(float a:part.amplitude)peak=std::max(peak,a);for(int i=0;i<nova::modelFrames;++i){float x=8+(getWidth()-16)*model.times[i]/model.duration,y=232-part.amplitude[i]/peak*43;if(i==0)curve.startNewSubPath(x,y);else curve.lineTo(x,y);}g.setColour(accent);g.strokePath(curve,juce::PathStrokeType(1.6f));g.setFont(12.f);g.drawText("P"+juce::String(selected+1)+" / "+juce::String(part.frequency,1)+" Hz / "+juce::String(model.count)+" osciladores independientes",8,234,getWidth()-16,17,juce::Justification::centredLeft);
    }
    void mouseDown(const juce::MouseEvent& e)override{auto model=p.modelView();if(!model.valid()||e.position.y>168)return;editing=std::clamp(static_cast<int>((e.position.x-8)/(getWidth()-16)*nova::partialCount),0,model.count-1);selected=editing;p.state.getParameter(NovaProcessor::partialId(editing))->beginChangeGesture();update(e.position);}
    void mouseDrag(const juce::MouseEvent& e)override{if(editing>=0)update(e.position);}
    void mouseUp(const juce::MouseEvent&)override{if(editing>=0)p.state.getParameter(NovaProcessor::partialId(editing))->endChangeGesture();editing=-1;}
    void mouseDoubleClick(const juce::MouseEvent& e)override{auto model=p.modelView();if(!model.valid())return;int i=std::clamp(static_cast<int>((e.position.x-8)/(getWidth()-16)*nova::partialCount),0,model.count-1);p.set(NovaProcessor::partialId(i).toRawUTF8(),1);repaint();}
};
