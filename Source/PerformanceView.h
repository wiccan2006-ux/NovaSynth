#pragma once
#include "Processor.h"
#include "GlassStyle.h"
class PerformanceView final:public juce::Component {
    NovaProcessor& p;std::array<juce::Slider,4> macros;std::array<juce::Label,4> labels;std::array<std::unique_ptr<NovaProcessor::State::SliderAttachment>,4> bindings;
public:
    juce::Colour accent{0xff40a9ff};explicit PerformanceView(NovaProcessor& owner):p(owner){
        const char* ids[]={"character","brightness","movement","space"};const char* names[]={"Caracter","Brillo","Movimiento","Espacio"};const char* help[]={"Cambia la densidad armonica y la saturacion. No cambia la afinacion.","Abre el filtro hasta tres octavas, sin cambiar la nota.","Modula el filtro y, en wavetable, el espectro y la anchura. Sin vibrato de tono.","Aumenta delay y reverb. Respeta los interruptores de los efectos."};
        for(int i=0;i<4;++i){auto& m=macros[i];m.setName(ids[i]);addAndMakeVisible(m);addAndMakeVisible(labels[i]);m.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);m.setTextBoxStyle(juce::Slider::TextBoxBelow,false,90,24);m.setNumDecimalPlacesToDisplay(2);m.setDoubleClickReturnValue(true,0);m.setTooltip(help[i]);m.setWantsKeyboardFocus(false);labels[i].setText(names[i],juce::dontSendNotification);labels[i].setJustificationType(juce::Justification::centred);bindings[i]=std::make_unique<NovaProcessor::State::SliderAttachment>(p.state,ids[i],m);}
    }
    void resized()override{for(int i=0;i<4;++i){int x=i*getWidth()/4;labels[i].setBounds(x+10,102,getWidth()/4-20,28);macros[i].setBounds(x+26,132,getWidth()/4-52,getHeight()-166);}}
    void refresh(){for(auto& l:labels)l.setFont(16*p.textScale());repaint();}
    void paint(juce::Graphics& g)override{
        auto area=getLocalBounds().toFloat();nova::glass::panel(g,area,accent,26,this);int program=p.program.load();g.setFont(23.f);g.setColour(juce::Colours::white);g.drawFittedText(program<0?"Tu instrumento":juce::String(nova::names()[program]).fromFirstOccurrenceOf(" / ",false,false),22,16,450,36,juce::Justification::centredLeft,1);
        g.setFont(13.f*p.textScale());g.setColour(juce::Colour(0xffc4d1e2));g.drawFittedText(program>=nova::signaturePresetStart?juce::String(nova::signaturePatches()[program-nova::signaturePresetStart].description):"Explora el sonido con cuatro gestos. Abre Controles para editar cada parametro.",22,62,getWidth()-44,34,juce::Justification::centredLeft,2);
        const char* captions[]={"ARMONICOS / DENSIDAD","APERTURA DEL FILTRO","EVOLUCION / ESTEREO","DELAY / REVERB"};for(int i=0;i<4;++i){auto box=juce::Rectangle<float>(i*getWidth()/4.f+8,100,getWidth()/4.f-16,getHeight()-108.f);g.setColour(juce::Colours::white.withAlpha(.025f));g.fillRoundedRectangle(box,20);g.setColour(accent.withAlpha(.8f));g.setFont(10.5f);g.drawText(captions[i],static_cast<int>(box.getX()),getHeight()-20,static_cast<int>(box.getWidth()),16,juce::Justification::centred);}
    }
};
class AppearanceView final:public juce::Component {
    NovaProcessor& p;juce::Slider tint,glow;juce::ComboBox text;juce::TextButton close{"Listo"};juce::ToggleButton motion{"Animaciones suaves"},protect{"Proteger salida"};
public:
    std::function<void()> onClose;juce::Colour accent{0xff40a9ff};explicit AppearanceView(NovaProcessor& owner):p(owner){
        for(auto* c:std::initializer_list<juce::Component*>{&tint,&glow,&text,&close,&motion,&protect})addAndMakeVisible(c);
        for(auto* s:{&tint,&glow}){s->setRange(0,1,.01);s->setSliderStyle(juce::Slider::LinearHorizontal);s->setTextBoxStyle(juce::Slider::NoTextBox,false,0,0);s->setWantsKeyboardFocus(false);}
        tint.setName("glassTint");glow.setName("neonGlow");tint.setTooltip("Cristal claro a cristal oscuro: cambia la opacidad de los paneles.");glow.setTooltip("Intensidad del halo neon en los controles y ondas.");tint.onValueChange=[this]{p.glassTint.store(static_cast<float>(tint.getValue()));};glow.onValueChange=[this]{p.neonGlow.store(static_cast<float>(glow.getValue()));};
        text.addItemList({"Texto normal","Texto mediano","Texto grande"},1);text.onChange=[this]{p.textSize.store(text.getSelectedId()-1);};close.onClick=[this]{if(onClose)onClose();};motion.onClick=[this]{p.animated.store(motion.getToggleState());};protect.onClick=[this]{p.protectOutput.store(protect.getToggleState());};protect.setTooltip("Reduce ganancia al superar 0.95 de pico de muestra. Ataque inmediato y recuperacion 40 ms; sin lookahead ni garantia true-peak.");
    }
    void refresh(){tint.setValue(p.glassTint.load(),juce::dontSendNotification);glow.setValue(p.neonGlow.load(),juce::dontSendNotification);text.setSelectedId(p.textSize.load()+1,juce::dontSendNotification);motion.setToggleState(p.animated.load(),juce::dontSendNotification);protect.setToggleState(p.protectOutput.load(),juce::dontSendNotification);}
    void resized()override{close.setBounds(getWidth()-100,16,82,30);tint.setBounds(155,64,getWidth()-180,30);glow.setBounds(155,110,getWidth()-180,30);text.setBounds(20,164,190,32);motion.setBounds(234,164,getWidth()-254,24);protect.setBounds(234,190,getWidth()-254,24);}
    void paint(juce::Graphics& g)override{g.fillAll(juce::Colour(0xff0c1320));nova::glass::panel(g,getLocalBounds().toFloat(),accent,26,this);g.setFont(21.f);g.setColour(juce::Colours::white);g.drawText("Tu espacio",20,16,300,30,juce::Justification::centredLeft);g.setFont(14.f);g.drawText("Tinte del vidrio",20,64,135,30,juce::Justification::centredLeft);g.drawText("Brillo neon",20,110,135,30,juce::Justification::centredLeft);}
};
