#pragma once
#include "Processor.h"
#include "Piano.h"
#include "Visuals.h"
#include "ComputerKeys.h"
#include "ReferenceView.h"
#include "PresetBrowser.h"
#include "ArpView.h"
#include "AnalysisView.h"
#include "RecreateWorkspace.h"
#include "PerformanceView.h"
#include "ModulationView.h"
#include "WorkflowView.h"

struct BusyCursor{BusyCursor(){juce::MouseCursor::showWaitCursor();}~BusyCursor(){juce::MouseCursor::hideWaitCursor();}};
class NovaEditor final:public juce::AudioProcessorEditor,public juce::FileDragAndDropTarget,private juce::Timer{
    std::unique_ptr<ModulationView> modPanel;juce::TextButton modButton{"Matriz"};bool modShowing=false;
    int mainPage=0,editPage=0;bool editing=false,personal=false;
    std::array<juce::TextButton,7> editTabs;
    juce::TextButton editSound{"Editar sonido"},factoryBank{"Banco"},mySounds{"Mis sonidos"},undoButton{"Deshacer"},redoButton{"Rehacer"},options{"Opciones"};
    juce::TextButton quickFavourite{"Favorito"},quickRecent{"Recientes / favoritos"},messageDetails{"Ver aviso"};juce::Label feedback;
    NovaLook look;NovaProcessor& p;juce::Component canvas;Piano piano;SignalView signal;PartialView partials;ReferenceView referenceView;PresetBrowser bank;
    PerformanceView performance;AppearanceView appearance;WorkflowBar workflow;PersonalLibraryView personalLibrary;QuickGuide guide;bool libraryShowing=false,guideShowing=false,playOnly=false;juce::TextButton aspect{"Aspecto"},advanced{"Controles"};bool advancedShowing=false;
    juce::ComboBox textChoice;int lastTextSize=-1;
    juce::ComboBox presets,waveA,waveB,depth,engineBox,themeBox,graphBox;
    double lastFrame=juce::Time::getMillisecondCounterHiRes();
    juce::ToggleButton animation{"Animaciones"};
    juce::ToggleButton unison{"Unison x3"},neural{"IA local"},computer{"Teclado PC"};
    std::unique_ptr<RecreateWorkspace> assistantPanel;juce::TextButton assistant{"Recrear"};bool assistantShowing=false;
    std::unique_ptr<AnalysisView> labPanel;juce::TextButton lab{"LAB / A-B"};bool labShowing=false;
    std::unique_ptr<ArpView> arpPanel;bool arpShowing=false;bool spaceWasDown=false;
    juce::TextButton previous{"<"},next{">"},arp{"ARP EDITAR"};juce::Slider gain;std::unique_ptr<NovaProcessor::State::SliderAttachment> gainBinding;
    juce::TextButton octaveDown{"Oct -"},octaveUp{"Oct +"},panicButton{"PANIC"};
    nova::ComputerKeys computerKeys;
    juce::TooltipWindow tooltips{this,650};
    void releaseKeys(){computerKeys.release([this](int n){p.keyboard.noteOff(16,n,0);});}
    bool typing()const{auto* c=juce::Component::getCurrentlyFocusedComponent();while(c&&c!=this){if(dynamic_cast<juce::TextEditor*>(c))return true;c=c->getParentComponent();}return false;}
    bool pollKeys(){std::array<bool,13> down{};auto mods=juce::ModifierKeys::getCurrentModifiersRealtime();bool enabled=computer.getToggleState()&&hasKeyboardFocus(true)&&!typing()&&!mods.isCtrlDown()&&!mods.isAltDown()&&!mods.isCommandDown()&&juce::Component::getCurrentlyModalComponent()==nullptr;for(int i=0;i<13;++i)down[i]=juce::KeyPress::isKeyCurrentlyDown(nova::ComputerKeys::mapping[i]);return computerKeys.update(down,enabled,[this](int n){p.keyboard.noteOn(16,n,.8f);},[this](int n){p.keyboard.noteOff(16,n,0);});}
    void octave(int direction){releaseKeys();p.keyboardBase.store(std::clamp(p.keyboardBase.load()+direction*12,24,96));computerKeys.base=p.keyboardBase.load();piano.setBase(computerKeys.base);grabKeyboardFocus();}
    juce::TextButton importSample{"Cargar referencia"},match{"Buscar de nuevo"},save{"Guardar"},load{"Abrir preset"},synthTab{"Instrumento"},effectsTab{"Efectos"},partialTab{"PARCIALES"},sampleTab{"MUESTRA"},bankTab{"BANCO"},about{"i"};
    juce::Label status;std::array<juce::Slider,controlCount> knobs;std::array<juce::Label,controlCount> labels;
    std::array<std::unique_ptr<NovaProcessor::State::SliderAttachment>,controlCount> bindings;
    std::array<juce::ToggleButton,5> effects;
    std::array<std::unique_ptr<NovaProcessor::State::ButtonAttachment>,5> effectBindings;
    std::array<std::unique_ptr<NovaProcessor::State::ComboBoxAttachment>,3> menus;
    std::unique_ptr<NovaProcessor::State::ButtonAttachment> unisonBinding;
    std::unique_ptr<juce::FileChooser> chooser;bool hovering=false;int lastTheme=-1;
    void applyTheme(int id){lastTheme=std::clamp(id,0,6);p.theme.store(lastTheme);look.theme(lastTheme);signal.accent=partials.accent=referenceView.accent=bank.accent=performance.accent=appearance.accent=personalLibrary.accent=guide.accent=look.accent;if(modPanel)modPanel->accent=look.accent;piano.setAccent(look.accent);if(arpPanel)arpPanel->setAccent(look.accent);if(labPanel)labPanel->setAccent(look.accent);if(assistantPanel)assistantPanel->setAccent(look.accent);for(auto& knob:knobs)knob.setColour(juce::Slider::rotarySliderFillColourId,look.accent);themeBox.setSelectedId(lastTheme+1,juce::dontSendNotification);sendLookAndFeelChange();repaint();}
    void showMain(int page){p.stopReference();mainPage=std::clamp(page,0,2);editing=false;guideShowing=false;appearance.setVisible(false);arrange();grabKeyboardFocus();}
    void selectEdit(int page){p.stopReference();mainPage=0;editing=true;editPage=std::clamp(page,0,6);p.uiEditorTab.store(editPage);guideShowing=false;appearance.setVisible(false);arrange();grabKeyboardFocus();}
    void showExplore(bool own){showMain(1);personal=own;if(own)personalLibrary.refresh();arrange();}
    void arrange(){
        assistantShowing=mainPage==2;bool overlay=guideShowing||appearance.isVisible();bool content=!overlay;bool synth=content&&editing&&editPage==0,fx=content&&editing&&editPage==2;
        for(auto* c:std::initializer_list<juce::Component*>{&waveA,&waveB,&engineBox,&unison,&graphBox,&signal,&partials,&bank,&performance,&workflow,&personalLibrary,&referenceView,&status,&partialTab,&effectsTab,&arp,&lab,&modButton,&advanced,&neural,&animation,&match,&depth,&importSample,&load,&about,&aspect,&textChoice,&sampleTab})c->setVisible(false);
        for(size_t i=0,slot=0;i<controlCount;++i){bool show=(synth&&!controls[i].effect)||(fx&&controls[i].effect);knobs[i].setVisible(show);labels[i].setVisible(show);if(show){int x=fx?24+static_cast<int>(slot)*107:24+static_cast<int>(slot%12)*89,y=fx?310:350+static_cast<int>(slot/12)*128;labels[i].setBounds(x,y,fx?104:87,22);knobs[i].setBounds(x,y+23,fx?104:87,94);++slot;}}
        for(int i=0;i<5;++i){effects[i].setVisible(fx);effects[i].setBounds(34+i*212,240,202,33);}
        for(auto* c:std::initializer_list<juce::Component*>{&waveA,&waveB,&engineBox,&unison,&graphBox,&signal})c->setVisible(synth);
        performance.setVisible(content&&mainPage==0&&!editing);partials.setVisible(content&&editing&&editPage==1);workflow.setVisible(content&&editing&&editPage==6);
        bank.setVisible(content&&mainPage==1&&!personal);personalLibrary.setVisible(content&&mainPage==1&&personal);
        factoryBank.setVisible(content&&mainPage==1);mySounds.setVisible(content&&mainPage==1);factoryBank.setToggleState(!personal,juce::dontSendNotification);mySounds.setToggleState(personal,juce::dontSendNotification);
        if(assistantPanel)assistantPanel->setVisible(content&&assistantShowing);if(modPanel)modPanel->setVisible(content&&editing&&editPage==3);if(arpPanel)arpPanel->setVisible(content&&editing&&editPage==4);if(labPanel)labPanel->setVisible(content&&editing&&editPage==5);
        for(int i=0;i<7;++i){editTabs[i].setVisible(content&&editing);editTabs[i].setToggleState(editPage==i,juce::dontSendNotification);}
        synthTab.setToggleState(mainPage==0&&!editing,juce::dontSendNotification);bankTab.setVisible(true);bankTab.setToggleState(mainPage==1,juce::dontSendNotification);assistant.setToggleState(assistantShowing,juce::dontSendNotification);editSound.setToggleState(editing,juce::dontSendNotification);editSound.setButtonText(editing?"Volver a tocar":"Editar sonido");
        guide.setVisible(guideShowing);if(guideShowing)guide.toFront(false);if(appearance.isVisible())appearance.toFront(false);
        labShowing=content&&editing&&editPage==5;arpShowing=content&&editing&&editPage==4;modShowing=content&&editing&&editPage==3;repaint();
    }

public:
    void showAppearance(){guideShowing=false;appearance.setVisible(true);appearance.refresh();arrange();}
    void showGuide(){appearance.setVisible(false);guideShowing=true;arrange();}
    int workspacePage()const{return mainPage;}bool isEditing()const{return editing;}
    explicit NovaEditor(NovaProcessor& owner):AudioProcessorEditor(owner),p(owner),piano(p.keyboard),signal(p),partials(p),referenceView(p),bank(p),performance(p),appearance(p),workflow(p),personalLibrary(p){
        setWantsKeyboardFocus(true);canvas.setName("NovaGlassCanvas");addAndMakeVisible(canvas);canvas.setInterceptsMouseClicks(false,true);setResizable(true,true);setResizeLimits(784,560,1792,1280);getConstrainer()->setFixedAspectRatio(1.4);setSize(p.editorWidth.load(),p.editorHeight.load());look.theme(0);setLookAndFeel(&look);
        for(auto* c:std::initializer_list<juce::Component*>{&piano,&presets,&waveA,&waveB,&depth,&unison,&importSample,&match,&save,&load,&status,&synthTab,&effectsTab,&partialTab,&about,&engineBox,&themeBox,&graphBox,&signal,&partials,&neural,&computer,&octaveDown,&octaveUp,&panicButton,&referenceView,&sampleTab,&bank,&bankTab,&previous,&next,&arp,&gain,&lab,&animation,&assistant,&textChoice,&performance,&appearance,&aspect,&advanced,&modButton,&workflow,&personalLibrary,&guide,&editSound,&factoryBank,&mySounds,&undoButton,&redoButton,&options,&quickFavourite,&quickRecent,&messageDetails,&feedback}){canvas.addAndMakeVisible(c);c->setWantsKeyboardFocus(false);}
        performance.toBack();appearance.setVisible(false);personalLibrary.setVisible(false);guide.setVisible(false);advancedShowing=!p.simpleView.load();workflow.mode=advancedShowing?1:0;
        workflow.onMode=[this](int mode){if(mode==1)selectEdit(0);else showMain(0);};workflow.onLibrary=[this]{showExplore(true);};workflow.onHelp=[this]{showGuide();};personalLibrary.onClose=[this]{showExplore(false);};guide.onClose=[this]{guideShowing=false;arrange();};
textChoice.setVisible(false);appearance.onClose=[this]{appearance.setVisible(false);arrange();};aspect.onClick=[this]{bool show=!appearance.isVisible();appearance.setVisible(show);aspect.setToggleState(show,juce::dontSendNotification);if(show){appearance.refresh();appearance.toFront(false);}};advanced.setClickingTogglesState(true);advanced.onClick=[this]{advancedShowing=advanced.getToggleState();arrange();};
        presets.setName("mainPresets");for(int i=0;i<nova::factoryPresetCount;++i)if(nova::catalog[i].representative==i)presets.addItem(nova::names()[static_cast<size_t>(i)],i+1);
        presets.onChange=[this]{if(presets.getSelectedId()>0)p.setCurrentProgram(presets.getSelectedId()-1);};
        juce::StringArray waves{"Sine","Saw","Pulse","Triangle","Soft Organ"};waveA.addItemList(waves,1);waveB.addItemList(waves,1);
        menus[0]=std::make_unique<NovaProcessor::State::ComboBoxAttachment>(p.state,"wavea",waveA);menus[1]=std::make_unique<NovaProcessor::State::ComboBoxAttachment>(p.state,"waveb",waveB);
        engineBox.setName("synthEngine");engineBox.addItemList({"FM / Subtractive","Parciales + ruido","Wavetable","Cuerda pulsada","Soplo resonante"},1);menus[2]=std::make_unique<NovaProcessor::State::ComboBoxAttachment>(p.state,"synthengine",engineBox);
        engineBox.onChange=[this]{if(engineBox.getSelectedId()==2&&!p.modelView().valid()&&p.learnedZones()>0)p.multiEnabled.store(true);if(engineBox.getSelectedId()==2&&!p.modelView().valid()&&p.learnedZones()==0){p.set("synthengine",0);p.say("Carga una referencia y ejecuta AutoFit para generar un banco de parciales.");}};
        themeBox.addItemList({"Azul neon","Amarillo neon","Verde neon","Morado neon","Rosado neon","Blanco neon","Naranja neon"},1);themeBox.onChange=[this]{applyTheme(themeBox.getSelectedId()-1);};
        animation.setToggleState(p.animated.load(),juce::dontSendNotification);animation.onClick=[this]{p.animated.store(animation.getToggleState());};animation.setTooltip("Transiciones, brillo de teclas y actividad de salida. Desactiva para reducir movimiento.");
        graphBox.setName("graphMode");graphBox.addItemList({"Onda","Espectro","Envolvente","En vivo","Movimiento tonal","Ruido / bandas"},1);graphBox.setSelectedId(4,juce::dontSendNotification);signal.mode=3;graphBox.onChange=[this]{signal.mode=graphBox.getSelectedId()-1;signal.repaint();};
        about.onClick=[] {juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,"Nova Synth 0.22","Desarrollador: Eleander Sanchez (Bodby Olwen)\n\nIA local NovaSeed + NovaSeed Multi: propuestas para cuatro familias de motor, entrenadas con sonidos propios sinteticos.\nCinco motores: FM, parciales, wavetable, cuerda y soplo. Banco Signature: 48 sonidos, seis pares de espectros armonicos, macros, favoritos y escucha. Vidrio y neon regulables. Tocar / Explorar / Recrear. Editar sonido conserva los controles completos. Recrear: muestra ampliada, aprender, comparacion musical A/B y 12 versiones guardadas con el proyecto. LAB y ARP MIDI conservados.\nTipo de instrumento por heuristica; no identifica instrumentos con certeza.\nNo se garantiza 99.99% de parecido. GPL v3 o posterior.");};
        unisonBinding=std::make_unique<NovaProcessor::State::ButtonAttachment>(p.state,"unison",unison);
        depth.addItem("Rapida / 160 pruebas",1);depth.addItem("Normal / 480 pruebas",2);depth.addItem("Profunda / 1200 pruebas",3);depth.setSelectedId(2,juce::dontSendNotification);depth.onChange=[this]{p.quality.store(depth.getSelectedId()==1?160:depth.getSelectedId()==3?1200:480);};
        for(size_t i=0;i<controlCount;++i){auto& knob=knobs[i];knob.setName(controls[i].id);knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);knob.setTextBoxStyle(juce::Slider::TextBoxBelow,false,80,20);knob.setColour(juce::Slider::rotarySliderFillColourId,juce::Colour(0xff77e4d3));knob.setNumDecimalPlacesToDisplay(controls[i].step>=1?0:2);canvas.addAndMakeVisible(knob);knob.setWantsKeyboardFocus(false);knob.onDragStart=[this]{p.beginControlEdit();};labels[i].setText(controls[i].label,juce::dontSendNotification);labels[i].setFont(12.f);labels[i].setJustificationType(juce::Justification::centred);canvas.addAndMakeVisible(labels[i]);bindings[i]=std::make_unique<NovaProcessor::State::SliderAttachment>(p.state,controls[i].id,knob);}
        for(int i=0;i<5;++i){effects[i].setName(effectIds[i]);effects[i].setButtonText(effectNames[i]);canvas.addAndMakeVisible(effects[i]);effects[i].setWantsKeyboardFocus(false);effectBindings[i]=std::make_unique<NovaProcessor::State::ButtonAttachment>(p.state,effectIds[i],effects[i]);}
        partialTab.setClickingTogglesState(true);bankTab.setClickingTogglesState(true);synthTab.setToggleState(true,juce::dontSendNotification);

        importSample.onClick=[this]{choose(0);};match.onClick=[this]{if(p.fitting.load()||p.loading.load())p.cancelFit();else p.startFit();};save.onClick=[this]{showExplore(true);personalLibrary.showSave();};load.onClick=[this]{choose(2);};
        previous.onClick=[this]{p.stepPrincipal(-1);};next.onClick=[this]{p.stepPrincipal(1);};
        arpPanel=std::make_unique<ArpView>(p,look.accent);canvas.addChildComponent(*arpPanel);arpPanel->setBounds(24,160,1072,478);arpPanel->onClose=[this]{arpShowing=false;arpPanel->setVisible(false);};labPanel=std::make_unique<AnalysisView>(p,look.accent);canvas.addChildComponent(*labPanel);labPanel->setBounds(24,160,1072,478);labPanel->onClose=[this]{labShowing=false;labPanel->setVisible(false);grabKeyboardFocus();};lab.onClick=[this]{assistantShowing=false;if(assistantPanel)assistantPanel->setVisible(false);labShowing=!labShowing;labPanel->setVisible(labShowing);if(labShowing){arpShowing=false;arpPanel->setVisible(false);labPanel->toFront(false);}grabKeyboardFocus();};arp.onClick=[this]{assistantShowing=false;if(assistantPanel)assistantPanel->setVisible(false);if(labShowing){labShowing=false;labPanel->setVisible(false);}arpShowing=!arpShowing;arpPanel->setVisible(arpShowing);if(arpShowing)arpPanel->toFront(false);};
        assistantPanel=std::make_unique<RecreateWorkspace>(p,look.accent);canvas.addChildComponent(*assistantPanel);assistantPanel->setBounds(24,126,1072,514);assistantPanel->onClose=[this]{showMain(0);};assistant.onClick=[this]{showMain(1);};synthTab.onClick=[this]{showMain(0);};effectsTab.onClick=[this]{showMain(2);};partialTab.onClick=[this]{bankTab.setToggleState(false,juce::dontSendNotification);arrange();};bankTab.onClick=[this]{partialTab.setToggleState(false,juce::dontSendNotification);arrange();};
        lab.onClick=[this]{showMain(0);labShowing=true;labPanel->setVisible(true);labPanel->toFront(false);};arp.onClick=[this]{showMain(0);arpShowing=true;arpPanel->setVisible(true);arpPanel->toFront(false);};
        textChoice.addItemList({"Texto normal","Texto mediano","Texto grande"},1);textChoice.setSelectedId(p.textSize.load()+1,juce::dontSendNotification);textChoice.onChange=[this]{p.textSize.store(textChoice.getSelectedId()-1);};

        modPanel=std::make_unique<ModulationView>(p);canvas.addChildComponent(*modPanel);modPanel->setBounds(24,165,1072,473);modPanel->onClose=[this]{modShowing=false;modPanel->setVisible(false);};modButton.onClick=[this]{showMain(0);modShowing=true;modPanel->setVisible(true);modPanel->toFront(false);};
        gain.setSliderStyle(juce::Slider::LinearHorizontal);gain.setTextBoxStyle(juce::Slider::TextBoxRight,false,54,24);gain.setDoubleClickReturnValue(true,0);gain.setTooltip("Ganancia de salida: -12 a +12 dB");gainBinding=std::make_unique<NovaProcessor::State::SliderAttachment>(p.state,"boost",gain);
        neural.setToggleState(p.useNeural.load(),juce::dontSendNotification);neural.onClick=[this]{p.useNeural.store(neural.getToggleState());};neural.setTooltip("NovaSeed y NovaSeed Multi proponen controles y prioridad de motores. Entrenadas con sintesis propia, sin grabaciones reales.");
        computer.setToggleState(true,juce::dontSendNotification);computer.onClick=[this]{releaseKeys();if(computer.getToggleState())grabKeyboardFocus();};computer.setTooltip("Haz clic aqui para dar el foco al plugin. A W S E D F T G Y H U J K: notas. Z/X: octava. Esc: cortar sonido.");
        octaveDown.onClick=[this]{octave(-1);};octaveUp.onClick=[this]{octave(1);};panicButton.onClick=[this]{releaseKeys();piano.release();p.panic();grabKeyboardFocus();};
        computerKeys.base=p.keyboardBase.load();piano.setBase(computerKeys.base);
        for(size_t i=0;i<controlCount;++i){knobs[i].setDoubleClickReturnValue(true,controls[i].initial);knobs[i].setTooltip(nova::controlHelp(controls[i].id));}
        synthTab.setButtonText("Tocar");bankTab.setButtonText("Explorar");assistant.setButtonText("Recrear");
        synthTab.setName("navPlay");bankTab.setName("navExplore");assistant.setName("navRecreate");editSound.setName("navEdit");
        synthTab.onClick=[this]{showMain(0);};bankTab.onClick=[this]{showMain(1);};assistant.onClick=[this]{showMain(2);};
        editSound.onClick=[this]{if(editing)showMain(0);else selectEdit(p.uiEditorTab.load());};factoryBank.onClick=[this]{showExplore(false);};mySounds.onClick=[this]{showExplore(true);};
        undoButton.onClick=[this]{p.undo();};redoButton.onClick=[this]{p.redo();};undoButton.setTooltip("Ctrl+Z: recupera controles, presets y recreaciones.");redoButton.setTooltip("Ctrl+Mayus+Z o Ctrl+Y: rehace el cambio.");
        const char* editNames[]={"Osciladores","Parciales","Efectos","Matriz","Arpegiador","Diagnostico","Variaciones"};for(int i=0;i<7;++i){canvas.addAndMakeVisible(editTabs[i]);editTabs[i].setButtonText(editNames[i]);editTabs[i].setName("editTab"+juce::String(i));editTabs[i].setWantsKeyboardFocus(false);editTabs[i].onClick=[this,i]{selectEdit(i);};}
        modPanel->onClose=[this]{selectEdit(0);};arpPanel->onClose=[this]{selectEdit(0);};labPanel->onClose=[this]{selectEdit(0);};
        assistantPanel->onLoad=[this]{choose(0);};assistantPanel->onClose=[this]{showMain(0);};
        appearance.addAndMakeVisible(themeBox);themeBox.setBounds(155,234,400,32);
        options.onClick=[this]{juce::PopupMenu menu;menu.addItem(1,"Aspecto y colores");menu.addItem(2,"Guia rapida");menu.addItem(3,"Abrir preset desde archivo");menu.addSeparator();menu.addItem(4,"Acerca de Nova Synth");auto safe=juce::Component::SafePointer<NovaEditor>(this);menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&options),[safe](int id){if(!safe)return;if(id==1)safe->showAppearance();if(id==2)safe->showGuide();if(id==3)safe->choose(2);if(id==4)safe->about.triggerClick();});};
        quickFavourite.setName("quickFavourite");quickRecent.setName("quickRecent");feedback.setName("globalFeedback");feedback.setColour(juce::Label::textColourId,juce::Colour(0xffbed2e8));feedback.setFont(11.5f);feedback.setInterceptsMouseClicks(false,false);
        quickFavourite.onClick=[this]{int id=p.program.load();if(id>=0)p.setFavourite(id,!p.favouritePresets[id].load());};
        quickFavourite.setTooltip("Guarda el preset del banco como favorito. Los sonidos propios se guardan con Guardar.");
        quickRecent.setTooltip("Ultimos 12 presets del banco cargados; se conservan al guardar el proyecto o preset.");
        quickRecent.onClick=[this]{juce::PopupMenu menu;auto ids=p.recentPrograms();if(ids.empty())menu.addItem(1,"Aun no has cargado presets",false);else for(int id:ids)menu.addItem(id+1,nova::names()[id],true,id==p.program.load());juce::PopupMenu favourites;for(int i=0;i<nova::factoryPresetCount;++i)if(p.favouritePresets[i].load())favourites.addItem(i+1,nova::names()[i],true,i==p.program.load());if(favourites.getNumItems()==0)favourites.addItem(1,"Marca un preset como favorito",false);menu.addSeparator();menu.addSubMenu("Mis favoritos",favourites);auto safe=juce::Component::SafePointer<NovaEditor>(this);menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&quickRecent),[safe](int id){if(safe&&id>0&&!safe->p.loading.load()&&!safe->p.fitting.load())safe->p.setCurrentProgram(id-1);});};
        messageDetails.setTooltip("Lee completo el ultimo aviso del plugin.");messageDetails.onClick=[this]{juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,"Aviso de Nova Synth",status.getText());};
        previous.setTooltip("Preset principal anterior");next.setTooltip("Preset principal siguiente");
        options.setTooltip("Aspecto, colores, guia, archivos y creditos.");gain.setTooltip("Volumen de salida en dB; doble clic vuelve a cero.");
        status.setFont(13.f);status.setColour(juce::Label::textColourId,juce::Colour(0xffcccccc));applyTheme(p.theme.load());arrange();startTimerHz(30);
    }
    ~NovaEditor()override{stopTimer();p.stopReference();releaseKeys();setLookAndFeel(nullptr);}
    bool keyStateChanged(bool)override{bool down=juce::KeyPress::isKeyCurrentlyDown(juce::KeyPress::spaceKey);if(!down)spaceWasDown=false;return pollKeys();}
    bool keyPressed(const juce::KeyPress& key)override{if(!typing()&&(key.getModifiers().isCtrlDown()||key.getModifiers().isCommandDown())){if(key.getKeyCode()=='Z'){if(key.getModifiers().isShiftDown())p.redo();else p.undo();return true;}if(key.getKeyCode()=='Y'){p.redo();return true;}}if(!typing()&&key.getKeyCode()==juce::KeyPress::escapeKey){p.stopReference();p.panic();return true;}

        if(!typing()&&(sampleTab.getToggleState()||labShowing||assistantShowing)&&key.getKeyCode()==juce::KeyPress::spaceKey&&!key.getModifiers().isAnyModifierKeyDown()){if(!spaceWasDown){spaceWasDown=true;if(assistantShowing){assistantPanel->auditionSelected(p.previewMode.load()==1?1:0);return true;}p.toggleReference(labShowing?(p.previewMode.load()==1?1:p.previewMode.load()==3?3:0):0,labShowing);}return true;}
        if(!computer.getToggleState()||typing()||key.getModifiers().isCtrlDown()||key.getModifiers().isAltDown()||key.getModifiers().isCommandDown())return false;
        if(key.getKeyCode()==juce::KeyPress::escapeKey){releaseKeys();piano.release();p.panic();return true;}
        if(key.getKeyCode()=='Z'){octave(-1);return true;}if(key.getKeyCode()=='X'){octave(1);return true;}
        for(char c:nova::ComputerKeys::mapping)if(c&&key.getKeyCode()==c){pollKeys();return true;}return false;
    }
    void mouseDown(const juce::MouseEvent&)override{grabKeyboardFocus();}
    void focusLost(FocusChangeType)override{if(!hasKeyboardFocus(true))releaseKeys();}
    void resized()override{
        float scale=getWidth()/1120.f;canvas.setBounds(0,0,1120,800);canvas.setTransform(juce::AffineTransform::scale(scale));p.editorWidth.store(getWidth());p.editorHeight.store(getHeight());
        previous.setBounds(224,22,34,32);presets.setBounds(268,22,374,32);next.setBounds(652,22,34,32);save.setBounds(706,22,100,32);undoButton.setBounds(818,22,82,32);redoButton.setBounds(910,22,82,32);options.setBounds(1002,22,94,32);
        quickFavourite.setBounds(460,88,142,36);quickRecent.setBounds(612,88,230,36);feedback.setBounds(448,54,550,17);messageDetails.setBounds(1002,54,94,17);
        synthTab.setBounds(24,88,132,36);bankTab.setBounds(166,88,132,36);assistant.setBounds(308,88,132,36);editSound.setBounds(872,88,224,36);
        factoryBank.setBounds(24,130,156,29);mySounds.setBounds(190,130,180,29);for(int i=0;i<7;++i)editTabs[i].setBounds(24+i*154,130,148,29);
        graphBox.setBounds(24,194,160,30);waveA.setBounds(204,194,166,30);waveB.setBounds(390,194,166,30);engineBox.setBounds(576,194,240,30);unison.setBounds(846,194,200,30);signal.setBounds(24,240,1072,90);
        for(auto* c:std::initializer_list<juce::Component*>{&personalLibrary,&guide,&partials,&bank,&performance})c->setBounds(24,165,1072,473);workflow.setBounds(24,330,1072,42);appearance.setBounds(260,220,600,290);themeBox.setBounds(155,234,400,32);
        if(modPanel)modPanel->setBounds(24,165,1072,473);if(assistantPanel)assistantPanel->setBounds(24,126,1072,514);if(labPanel)labPanel->setBounds(24,165,1072,473);if(arpPanel)arpPanel->setBounds(24,165,1072,473);
        gain.setBounds(805,648,195,28);computer.setBounds(24,648,145,28);octaveDown.setBounds(174,648,65,28);octaveUp.setBounds(246,648,65,28);panicButton.setBounds(322,648,85,28);piano.setBounds(24,690,1072,88);arrange();
    }
    void paint(juce::Graphics& g)override{
        g.fillAll(juce::Colour(0xff05080f));g.addTransform(juce::AffineTransform::scale(getWidth()/1120.f));nova::glass::background(g,{0,0,1120,800},look.accent);
        nova::glass::panel(g,{12,8,1096,64},look.accent,24,&canvas);nova::glass::panel(g,{12,644,1096,142},look.accent,24,&canvas);
        g.setColour(juce::Colours::white);g.setFont(22);g.drawText("NOVA SYNTH",24,20,186,34,juce::Justification::centredLeft);g.setFont(11.5f);g.setColour(juce::Colour(0xffb4bfd0));g.drawText("0.22 BETA",26,53,170,17,juce::Justification::centredLeft);g.setColour(p.patchModified()?look.accent:juce::Colour(0xffaebed0));g.drawText(p.patchModified()?"Cambios sin guardar":"Guardado",268,55,176,16,juce::Justification::centredLeft);
        g.setFont(14*p.textScale());g.setColour(juce::Colour(0xffc0cddd));if(mainPage==0&&!editing&&!guideShowing&&!appearance.isVisible())g.drawText("Elige un sonido y toca. Las cuatro macros lo hacen tuyo.",24,132,1040,25,juce::Justification::centredLeft);
        if(editing){nova::glass::panel(g,{12,164,1096,475},look.accent,22,&canvas);if(editPage==0){g.setFont(12);g.drawText("Oscilador A",204,173,166,18,juce::Justification::centredLeft);g.drawText("Oscilador B",390,173,166,18,juce::Justification::centredLeft);g.drawText("Motor",576,173,240,18,juce::Justification::centredLeft);}if(editPage==2){g.setFont(20);g.drawText("Efectos del instrumento",32,184,700,35,juce::Justification::centredLeft);g.setFont(14);g.drawText("Drive  >  Chorus  >  EQ  >  Delay  >  Reverb",32,445,1040,30,juce::Justification::centredLeft);}if(editPage==6){g.setFont(24);g.drawText("Explora otra version de tu sonido",44,210,1000,40,juce::Justification::centredLeft);g.setFont(16);g.drawFittedText("Elige cuanto cambiar y bloquea lo que quieres conservar.\nLa afinacion y la ganancia se mantienen. Deshacer te devuelve al punto anterior.",44,264,1000,60,juce::Justification::topLeft,3);}}
        if(p.loading.load()||p.fitting.load()){g.setColour(look.accent);g.fillRoundedRectangle(24.f,80.f,1072*p.fitProgress.load(),3.f,1.5f);}
        g.setFont(12);g.setColour(juce::Colour(0xffb4c3d5));g.drawText("Z / X: octava   |   Esc: parar",425,651,320,22,juce::Justification::centredLeft);g.drawText("Volumen",747,651,58,22,juce::Justification::centredRight);
        for(int c=0;c<2;++c){float level=c?p.peakRight.load():p.peakLeft.load();g.setColour(juce::Colour(0xff182637));g.fillRoundedRectangle(1008.f,651.f+c*8,84.f,4.f,2);g.setColour(level>.98f?juce::Colour(0xffff9656):look.accent);g.fillRoundedRectangle(1008.f,651.f+c*8,84*std::clamp(level,0.f,1.f),4.f,2);}
        if(editing&&editPage==5){g.setColour(look.accent);g.drawText("Audio "+juce::String(p.audioLoad.load()*100,1)+"% / "+juce::String(p.voiceCount.load())+" voces / GR "+juce::String(p.outputReduction.load(),1)+" dB",700,670,396,18,juce::Justification::centredRight);}
    }
    bool isInterestedInFileDrag(const juce::StringArray& files)override{return !p.fitting.load()&&!p.loading.load()&&files.size()==1&&juce::File(files[0]).hasFileExtension("wav;aif;aiff;flac;ogg");}
    void fileDragEnter(const juce::StringArray&,int,int)override{hovering=true;repaint();}void fileDragExit(const juce::StringArray&)override{hovering=false;repaint();}
    void filesDropped(const juce::StringArray& files,int,int)override{hovering=false;if(files.size()==1){BusyCursor cursor;p.loadSample(juce::File(files[0]));showMain(2);assistantPanel->showPage(0);}repaint();}
private:
    void timerCallback()override{
        if(lastTextSize!=p.textSize.load()){lastTextSize=p.textSize.load();look.textScale=p.textScale();textChoice.setSelectedId(lastTextSize+1,juce::dontSendNotification);for(auto& label:labels)label.setFont(12*p.textScale());status.setFont(13*p.textScale());feedback.setFont(11.5f*p.textScale());sendLookAndFeelChange();}
        int current=p.program.load();quickFavourite.setEnabled(current>=0&&!p.loading.load()&&!p.fitting.load());quickFavourite.setButtonText(current>=0&&p.favouritePresets[current].load()?"En favoritos":"Favorito");quickFavourite.setToggleState(current>=0&&p.favouritePresets[current].load(),juce::dontSendNotification);quickRecent.setEnabled(!p.loading.load()&&!p.fitting.load());
        workflow.refresh();undoButton.setEnabled(!p.loading.load()&&!p.fitting.load()&&p.canUndo());redoButton.setEnabled(!p.loading.load()&&!p.fitting.load()&&p.canRedo());
        canvas.getProperties().set("novaTint",p.glassTint.load());canvas.getProperties().set("novaGlow",p.neonGlow.load());look.glow=p.neonGlow.load();performance.refresh();
        double now=juce::Time::getMillisecondCounterHiRes(),dt=(now-lastFrame)*.001;lastFrame=now;animation.setToggleState(p.animated.load(),juce::dontSendNotification);look.advance(canvas,dt,p.animated.load());signal.advance(dt,p.animated.load());piano.advance(dt,p.animated.load());
        if(!juce::KeyPress::isKeyCurrentlyDown(juce::KeyPress::spaceKey))spaceWasDown=false;pollKeys();if(computerKeys.base!=p.keyboardBase.load()){releaseKeys();computerKeys.base=p.keyboardBase.load();piano.setBase(computerKeys.base);}piano.repaint();neural.setToggleState(p.useNeural.load(),juce::dontSendNotification);
        if(lastTheme!=p.theme.load())applyTheme(p.theme.load());bool busy=p.fitting.load()||p.loading.load();referenceView.refresh();referenceView.setEnabled(!busy);bank.setEnabled(!busy);bank.sync();if(appearance.isVisible())appearance.toFront(false);status.setText(p.loading.load()?(p.comparisonPreparing()?"Preparando comparacion... ":p.reviewScanning()?"Revisando notas... ":"Leyendo forma de onda... ")+juce::String(static_cast<int>(p.fitProgress.load()*100))+" %":busy?juce::String(std::array<const char*,4>{"Analizando notas... ","Ajustando timbre / motores... ","Ajustando envolvente... ","Ajustando efectos... "}[std::clamp(p.analysisStage.load(),0,3)])+juce::String(static_cast<int>(p.fitProgress.load()*100))+" % | "+juce::String(p.fitTrials.load())+" pruebas | Cancelar conserva el patch anterior.":p.status(),juce::dontSendNotification);
        if(p.remoteWorking.load())status.setText("Servidor trabajando. Tiempo variable; Cancelar conserva el instrumento.",juce::dontSendNotification);
        feedback.setText(status.getText(),juce::dontSendNotification);feedback.setTooltip(status.getText());messageDetails.setEnabled(status.getText().isNotEmpty());
        for(auto* c:std::initializer_list<juce::Component*>{&presets,&waveA,&waveB,&depth,&unison,&engineBox,&importSample,&save,&load,&neural,&previous,&next,&arp,&gain,&lab,&animation,&assistant,&textChoice,&performance,&advanced,&modButton})c->setEnabled(!busy);for(auto& knob:knobs)knob.setEnabled(!busy);for(auto& fx:effects)fx.setEnabled(!busy);partials.setEnabled(!busy);if(!busy&&static_cast<int>(p.v("synthengine"))==1){waveA.setEnabled(false);waveB.setEnabled(false);unison.setEnabled(false);for(int i:{0,1,2,3,5,20})knobs[static_cast<size_t>(i)].setEnabled(false);}
        if(!busy){int e=static_cast<int>(p.v("synthengine"));if(e>=2){unison.setEnabled(false);for(int i:{1,2,3,20})knobs[static_cast<size_t>(i)].setEnabled(false);}if(e>=3){waveA.setEnabled(false);waveB.setEnabled(false);knobs[0].setEnabled(false);if(e==4)knobs[5].setEnabled(false);}}
        match.setButtonText(busy?"Cancelar":p.contextEnabled.load()&&p.selectedRange().length()>3.0001?"Aprender contexto":"Recrear seleccion");match.setEnabled(busy||p.hasReference());
        if(p.program.load()<0){presets.setSelectedId(0,juce::dontSendNotification);presets.setText(p.patchTitle(),juce::dontSendNotification);}else if(nova::catalog[p.program.load()].representative==p.program.load())presets.setSelectedId(p.program.load()+1,juce::dontSendNotification);else{presets.setSelectedId(0,juce::dontSendNotification);presets.setText("Variante / "+juce::String(nova::names()[p.program.load()]),juce::dontSendNotification);}for(size_t i=0;i<controlCount;++i){labels[i].setText(controls[i].label,juce::dontSendNotification);juce::String id=controls[i].id;bool active=(p.v("character")>0&&(id=="fm"||id=="drive"||id=="spectone"))||(p.v("brightness")>0&&id=="cutoff")||(p.v("movement")>0&&id=="lfodepth")||(p.v("space")>0&&(id=="delay"||id=="reverb"));labels[i].setColour(juce::Label::textColourId,active?look.accent:juce::Colour(0xffd7d7d7));knobs[i].setTooltip(juce::String(nova::controlHelp(controls[i].id))+(active?" / Macro activo: lectura base, el macro se aplica encima.":""));}int e=static_cast<int>(p.v("synthengine"));if(e==2){labels[0].setText("Scan Motion",juce::dontSendNotification);labels[5].setText("Table Scan",juce::dontSendNotification);}if(e==3)labels[5].setText("Pluck Position",juce::dontSendNotification);if(e==3||e==4)labels[7].setText(e==3?"String Decay":"Bore Feedback",juce::dontSendNotification);signal.repaint();partials.repaint();repaint();
    }
    void choose(int action){
        chooser=std::make_unique<juce::FileChooser>(action==0?"Referencia para AutoFit":action==1?"Guardar patch sin audio":"Abrir preset",juce::File{},action==0?"*.wav;*.aif;*.aiff;*.flac;*.ogg":"*.novapreset");auto safe=juce::Component::SafePointer<NovaEditor>(this);
        chooser->launchAsync(juce::FileBrowserComponent::canSelectFiles|(action==1?juce::FileBrowserComponent::saveMode|juce::FileBrowserComponent::warnAboutOverwriting:juce::FileBrowserComponent::openMode),[safe,action](const juce::FileChooser& dialog){if(!safe)return;auto file=dialog.getResult();if(file==juce::File{})return;BusyCursor cursor;if(action==0){safe->p.loadSample(file);safe->showMain(2);safe->assistantPanel->showPage(0);}else if(action==1){juce::MemoryBlock data;safe->p.getStateInformation(data);auto dest=file.withFileExtension("novapreset");safe->p.say(dest.replaceWithData(data.getData(),data.getSize())?"Preset guardado: solo controles, sin audio de referencia.":"No se pudo guardar el preset.");}else{juce::MemoryBlock data;if(file.getSize()>24*1024*1024||!file.loadFileAsData(data))safe->p.say("No se pudo leer el preset.");else safe->p.loadPresetFile(file);}});
    }
};
