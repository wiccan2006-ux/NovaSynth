#include "Processor.h"
#include "Editor.h"
juce::AudioProcessorEditor* NovaProcessor::createEditor(){return new NovaEditor(*this);}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){return new NovaProcessor();}
