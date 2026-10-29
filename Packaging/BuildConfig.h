#pragma once
// Configuration of the distributable Windows x64 VST3, built with JUCE 7.0.12.
#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED 1
#define JUCE_MODULE_AVAILABLE_juce_core 1
#define JUCE_MODULE_AVAILABLE_juce_events 1
#define JUCE_MODULE_AVAILABLE_juce_data_structures 1
#define JUCE_MODULE_AVAILABLE_juce_graphics 1
#define JUCE_MODULE_AVAILABLE_juce_gui_basics 1
#define JUCE_MODULE_AVAILABLE_juce_gui_extra 1
#define JUCE_MODULE_AVAILABLE_juce_audio_basics 1
#define JUCE_MODULE_AVAILABLE_juce_audio_processors 1
#define JUCE_WEB_BROWSER 0
#define JUCE_USE_CURL 0
#define JUCE_PLUGINHOST_VST 0
#define JUCE_PLUGINHOST_VST3 0
#define JUCE_PLUGINHOST_AU 0
#define JUCE_PLUGINHOST_LADSPA 0
#define JUCE_VST3_CAN_REPLACE_VST2 0
#define JUCE_STANDALONE_APPLICATION 0
#define JUCE_DISPLAY_SPLASH_SCREEN 0
#define JUCE_REPORT_APP_USAGE 0
#define JucePlugin_Build_VST 0
#define JucePlugin_Build_VST3 1
#define JucePlugin_Build_AU 0
#define JucePlugin_Build_AUv3 0
#define JucePlugin_Build_AAX 0
#define JucePlugin_Build_Standalone 0
#define JucePlugin_Build_LV2 0
#define JucePlugin_Build_Unity 0
#define JucePlugin_Enable_ARA 0
#define JucePlugin_Name "Nova Synth"
#define JucePlugin_Desc "Neural assisted procedural resynthesis"
#define JucePlugin_Manufacturer "Eleander Sanchez (Bodby Olwen)"
#define JucePlugin_ManufacturerWebsite ""
#define JucePlugin_ManufacturerEmail ""
#define JucePlugin_ManufacturerCode 0x567a436c
#define JucePlugin_PluginCode 0x4e733031
#define JucePlugin_IsSynth 1
#define JucePlugin_IsMidiEffect 0
#define JucePlugin_WantsMidiInput 1
#define JucePlugin_ProducesMidiOutput 0
#define JucePlugin_EditorRequiresKeyboardFocus 1
#define JucePlugin_Version 0x001100
#define JucePlugin_VersionCode 0x001100
#define JucePlugin_VersionString "0.17.0"
#define JucePlugin_VSTUniqueID JucePlugin_PluginCode
#define JucePlugin_VstCategory 2
#define JucePlugin_Vst3Category "Instrument|Synth"
#define JucePlugin_VSTNumMidiInputs 0
#define JucePlugin_VSTNumMidiOutputs 0

#define JUCE_MODULE_AVAILABLE_juce_audio_formats 1
#define JUCE_USE_WINDOWS_MEDIA_FORMAT 0
