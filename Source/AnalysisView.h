#pragma once
#include "Processor.h"
#include "GlassStyle.h"
class AnalysisView final:public juce::Component,private juce::Timer {
    NovaProcessor& p;
    juce::TextButton original{"Original A"},tuned{"A afinado"},recreated{"Recreado B"},stop{"Detener"},diagnose{"Diagnosticar"},close{"Cerrar"},cancel{"Cancelar busqueda"};
    juce::TextButton add{"Agregar seleccion"},remove{"Quitar elegida"},build{"Recrear banco"},clear{"Vaciar banco"},listen{"Escuchar nota"},centre{"Centrar tono"},undo{"Deshacer ajuste"},apply{"Usar alternativa"};
    juce::ToggleButton level{"Igualar volumen A/B"},useZones{"Usar banco por nota/intensidad"},autoNote{"Auto nota"};
    juce::ComboBox velocity,referencePick,contextPick,alternatives,root;
    std::unique_ptr<NovaProcessor::State::ComboBoxAttachment> rootBinding;
    juce::Label feedback;juce::TextEditor report,list;juce::String lastReport,lastList;juce::StringArray oldReferences,oldContexts,oldAlternatives;juce::Colour accent;
    void focus(){if(auto* e=findParentComponentOfClass<juce::AudioProcessorEditor>())e->grabKeyboardFocus();}
    static void sync(juce::ComboBox& box,juce::StringArray& before,const juce::StringArray& now){if(before==now)return;int selected=std::max(1,box.getSelectedId());box.clear(juce::dontSendNotification);box.addItemList(now,1);if(!now.isEmpty())box.setSelectedId(std::min(selected,now.size()),juce::dontSendNotification);before=now;}
public:
    std::function<void()> onClose;
    AnalysisView(NovaProcessor& owner,juce::Colour colour):p(owner),accent(colour){
        for(auto* c:std::initializer_list<juce::Component*>{&original,&tuned,&recreated,&stop,&diagnose,&close,&cancel,&add,&remove,&build,&clear,&level,&useZones,&velocity,&report,&list,&feedback,&listen,&centre,&undo,&apply,&autoNote,&referencePick,&contextPick,&alternatives,&root}){addAndMakeVisible(c);c->setWantsKeyboardFocus(false);}
        for(auto* text:{&report,&list}){text->setMultiLine(true,true);text->setReadOnly(true);text->setScrollbarsShown(true);text->setColour(juce::TextEditor::backgroundColourId,juce::Colour(0x7a080f1a));text->setColour(juce::TextEditor::textColourId,juce::Colour(0xffd9e2ef));text->setFont(14.f);}
        for(int note=24;note<=108;++note)root.addItem(juce::MidiMessage::getMidiNoteName(note,true,true,4)+" / MIDI "+juce::String(note),note-23);
        rootBinding=std::make_unique<NovaProcessor::State::ComboBoxAttachment>(p.state,"root",root);
        velocity.addItem("Suave / 32",32);velocity.addItem("Media / 64",64);velocity.addItem("Fuerte / 96",96);velocity.addItem("Muy fuerte / 120",120);velocity.setSelectedId(96);
        original.onClick=[this]{focus();p.toggleReference(0,true);};tuned.onClick=[this]{focus();p.toggleReference(3,true);};recreated.onClick=[this]{focus();p.toggleReference(1,true);};stop.onClick=[this]{p.stopReference();focus();};diagnose.onClick=[this]{p.toggleReference(2,true);};
        cancel.onClick=[this]{p.cancelFit();};close.onClick=[this]{if(onClose)onClose();};add.onClick=[this]{p.captureReference(velocity.getSelectedId()/127.f);};remove.onClick=[this]{if(referencePick.getSelectedId()>0)p.removeReference(referencePick.getSelectedItemIndex());};clear.onClick=[this]{p.clearReferences();};build.onClick=[this]{p.startBankFit();};level.onClick=[this]{p.matchLevel.store(level.getToggleState());};useZones.onClick=[this]{p.multiEnabled.store(useZones.getToggleState());if(useZones.getToggleState())p.set("synthengine",1);};
        listen.onClick=[this]{if(referencePick.getSelectedId()>0)p.toggleReference(0,false,referencePick.getSelectedItemIndex());};referencePick.onChange=[this]{p.stopReference();};contextPick.onChange=[this]{p.stopReference();p.comparisonZone.store(std::max(0,contextPick.getSelectedItemIndex()));};
        autoNote.onClick=[this]{p.stopReference();p.autoPitch.store(autoNote.getToggleState());};centre.onClick=[this]{p.centrePitch();};undo.onClick=[this]{p.undoFit();};apply.onClick=[this]{p.applyAlternative(alternatives.getSelectedItemIndex());};
        referencePick.setTextWhenNothingSelected("Referencias manuales");contextPick.setTextWhenNothingSelected("A/B: seleccion actual / sin contexto aprendido");alternatives.setTextWhenNothingSelected("Recrea una nota aislada para obtener alternativas");
        root.setTooltip("Desactiva Auto nota para fijar la nota de referencia. El numero MIDI evita diferencias de nombre de octava entre programas. Repite la recreacion despues de corregirla.");
        tuned.setTooltip("Escucha una copia de comparacion con la desviacion global en cents corregida. El archivo original permanece intacto; no elimina vibrato ni otros efectos.");
        centre.setTooltip("Pone a cero Tune, movimiento tonal, detune, chorus y LFO de tono; desactiva Unison. Deshacer recupera el ajuste anterior.");
        alternatives.setTooltip("Hasta tres motores distintos realmente evaluados. Orden por distancia interna, no porcentaje de parecido. Disponible para notas aisladas; son alternativas iniciales, no tres redes entrenadas nuevas.");
        contextPick.setTooltip("Elige que zona aprendida escuchar y diagnosticar. Variacion: rango tonal entre percentiles 10 y 90 en cents; no mide parecido.");
        level.setTooltip("A y B usan un RMS objetivo comun con limite de pico. No es LUFS.");add.setTooltip("Agrega notas aisladas de hasta 3 s del mismo instrumento. Escucha y quita las que tengan mezcla, ruido u otra nota antes de recrear el banco.");
        feedback.setColour(juce::Label::textColourId,juce::Colour(0xffbac6d8));startTimerHz(8);
    }
    void setAccent(juce::Colour c){accent=c;repaint();}
    void resized()override{
        close.setBounds(getWidth()-108,10,92,28);original.setBounds(18,52,115,30);tuned.setBounds(143,52,115,30);recreated.setBounds(268,52,130,30);stop.setBounds(408,52,90,30);diagnose.setBounds(508,52,125,30);level.setBounds(643,52,215,30);cancel.setBounds(870,52,182,30);
        root.setBounds(18,95,165,29);autoNote.setBounds(193,95,128,29);centre.setBounds(330,95,130,29);undo.setBounds(470,95,128,29);contextPick.setBounds(18,134,580,29);report.setBounds(18,174,580,196);alternatives.setBounds(18,383,430,30);apply.setBounds(458,383,140,30);
        velocity.setBounds(870,95,182,28);referencePick.setBounds(622,132,280,28);listen.setBounds(912,132,140,28);list.setBounds(622,174,430,128);add.setBounds(622,314,211,30);remove.setBounds(843,314,209,30);build.setBounds(622,354,211,32);clear.setBounds(843,354,209,32);useZones.setBounds(622,397,430,28);
        feedback.setBounds(18,456,1034,18);feedback.setFont(12.f);
    }
    void paint(juce::Graphics& g)override{
        g.setColour(juce::Colour(0xff080e18));g.fillRoundedRectangle(getLocalBounds().toFloat(),22);nova::glass::panel(g,getLocalBounds().toFloat(),accent,22,this);g.setColour(juce::Colours::white);g.setFont(19);g.drawText("LAB / COMPARAR Y APRENDER",18,12,420,29,juce::Justification::centredLeft);
        g.setFont(13.f);g.setColour(accent);if(p.fitting.load())g.drawText(juce::String(std::array<const char*,4>{"Notas / afinacion", "Timbre / motores", "Envolvente", "Efectos"}[std::clamp(p.analysisStage.load(),0,3)])+" / "+juce::String(static_cast<int>(p.fitProgress.load()*100))+" %",450,12,490,29,juce::Justification::centredRight);
        g.setColour(juce::Colour(0xffbac6d8));g.drawText("REFERENCIAS / intensidad relativa",622,95,240,28,juce::Justification::centredLeft);g.drawText("Alternativas por motor / menor distancia es mejor",18,420,580,23,juce::Justification::centredLeft);g.drawText("Zonas aprendidas: "+juce::String(p.learnedZones()),622,431,430,20,juce::Justification::centredLeft);
    }
private:
    void timerCallback()override{
        if(!isShowing())return;feedback.setText(p.status(),juce::dontSendNotification);feedback.setTooltip(p.status());auto a=p.analysisReport(),b=p.referenceList();if(a!=lastReport){report.setText(a,false);lastReport=a;}if(b!=lastList){list.setText(b,false);lastList=b;}
        sync(referencePick,oldReferences,p.referenceNames());sync(contextPick,oldContexts,p.contextNames());sync(alternatives,oldAlternatives,p.alternativeNames());if(!oldContexts.isEmpty())contextPick.setSelectedId(std::clamp(p.comparisonZone.load()+1,1,oldContexts.size()),juce::dontSendNotification);
        bool busy=p.fitting.load()||p.loading.load();cancel.setEnabled(busy);for(auto* c:std::initializer_list<juce::Component*>{&original,&tuned,&recreated,&diagnose,&add,&remove,&build,&clear,&velocity,&useZones,&referencePick,&contextPick,&alternatives,&apply,&autoNote,&centre})c->setEnabled(!busy);
        root.setEnabled(!busy&&!p.autoPitch.load());autoNote.setToggleState(p.autoPitch.load(),juce::dontSendNotification);undo.setEnabled(!busy&&p.canUndoFit());apply.setEnabled(!busy&&!oldAlternatives.isEmpty());listen.setEnabled(!busy&&!oldReferences.isEmpty());remove.setEnabled(!busy&&!oldReferences.isEmpty());contextPick.setEnabled(!busy&&!oldContexts.isEmpty());
        level.setToggleState(p.matchLevel.load(),juce::dontSendNotification);useZones.setToggleState(p.multiEnabled.load(),juce::dontSendNotification);useZones.setEnabled(!busy&&p.learnedZones()>0);original.setToggleState(p.referencePlaying()&&p.previewMode.load()==0,juce::dontSendNotification);tuned.setToggleState(p.referencePlaying()&&p.previewMode.load()==3,juce::dontSendNotification);recreated.setToggleState(p.referencePlaying()&&p.previewMode.load()==1,juce::dontSendNotification);repaint();
    }
};
