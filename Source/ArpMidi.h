#pragma once
#include "Arpeggio.h"
#include <juce_audio_basics/juce_audio_basics.h>
namespace nova {
// One bar at 960 PPQ; gates match the live arpeggiator's 80% duration.
inline juce::MidiFile arpMidi(const std::array<int,arpSteps>& pattern,int root,float bpm){
    juce::MidiMessageSequence track;
    auto tempo=juce::MidiMessage::tempoMetaEvent(static_cast<int>(60000000/std::clamp(bpm,30.f,300.f)));
    track.addEvent(tempo,0);track.addEvent(juce::MidiMessage::timeSignatureMetaEvent(4,4),0);
    for(int step=0;step<arpSteps;++step)if(pattern[step]>=-24&&pattern[step]<=24){
        int note=std::clamp(root+pattern[step],0,127);
        track.addEvent(juce::MidiMessage::noteOn(1,note,static_cast<juce::uint8>(100)),step*240);
        track.addEvent(juce::MidiMessage::noteOff(1,note),step*240+192);
    }
    track.addEvent(juce::MidiMessage::endOfTrack(),3840);track.updateMatchedPairs();
    juce::MidiFile file;file.setTicksPerQuarterNote(960);file.addTrack(track);return file;
}
}
