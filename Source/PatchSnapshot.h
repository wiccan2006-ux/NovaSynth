#pragma once
#include <juce_data_structures/juce_data_structures.h>
#include "ModelCodec.h"
#include "Arpeggio.h"
#include <set>
#include <memory>
namespace nova {
constexpr int maxPatchVersions=12;
struct PatchSnapshot {
    juce::String name;juce::ValueTree parameters{"Parameters"};std::shared_ptr<SpectralModel> model;std::shared_ptr<ZoneBank> bank;
    int program=-1;bool useZones=false,arp=false;float bpm=120;std::array<int,arpSteps> pattern{};uint64_t reviewToken=~uint64_t{0};
};
inline juce::ValueTree encodeSnapshot(const PatchSnapshot& s){
    juce::ValueTree t("PatchVersion");t.setProperty("name",s.name,nullptr);t.setProperty("program",s.program,nullptr);t.setProperty("zones",s.useZones,nullptr);t.setProperty("arp",s.arp,nullptr);t.setProperty("bpm",s.bpm,nullptr);juce::StringArray notes;for(int n:s.pattern)notes.add(juce::String(n));t.setProperty("pattern",notes.joinIntoString(","),nullptr);t.addChild(s.parameters.createCopy(),-1,nullptr);
    if(s.model){auto data=encodeModel(*s.model);t.setProperty("model",juce::MemoryBlock(data.data(),data.size()).toBase64Encoding(),nullptr);}
    if(s.bank){auto data=encodeZones(*s.bank);t.setProperty("bank",juce::MemoryBlock(data.data(),data.size()).toBase64Encoding(),nullptr);}return t;
}
inline bool decodeSnapshot(const juce::ValueTree& t,PatchSnapshot& result){
    if(!t.hasType("PatchVersion"))return false;PatchSnapshot s;s.name=t.getProperty("name").toString();if(s.name.isEmpty()||s.name.length()>100)return false;s.program=static_cast<int>(t.getProperty("program",-1));s.useZones=static_cast<bool>(t.getProperty("zones",false));s.arp=static_cast<bool>(t.getProperty("arp",false));s.bpm=static_cast<float>(t.getProperty("bpm",120));if(!std::isfinite(s.bpm)||s.bpm<30||s.bpm>300)return false;
    s.parameters=t.getChildWithName("Parameters").createCopy();if(!s.parameters.isValid()||s.parameters.getNumChildren()<1||s.parameters.getNumChildren()>256)return false;std::set<juce::String> ids;
    for(auto child:s.parameters){auto id=child.getProperty("id").toString();double value=static_cast<double>(child.getProperty("value"));if(!child.hasType("P")||!child.hasProperty("value")||id.isEmpty()||id.length()>64||!ids.insert(id).second||!std::isfinite(value)||std::abs(value)>1e9)return false;}
    auto pattern=juce::StringArray::fromTokens(t.getProperty("pattern").toString(),",","");if(pattern.size()!=arpSteps)return false;for(int i=0;i<arpSteps;++i){int n=pattern[i].getIntValue();if(n!=arpRest&&(n< -24||n>24))return false;s.pattern[i]=n;}
    juce::MemoryBlock data;auto model=t.getProperty("model").toString();if(model.isNotEmpty()){s.model=std::make_shared<SpectralModel>();if(model.length()>60000||!data.fromBase64Encoding(model)||!decodeModel(data.getData(),data.getSize(),*s.model))return false;}
    auto bank=t.getProperty("bank").toString();if(bank.isNotEmpty()){s.bank=std::make_shared<ZoneBank>();if(bank.length()>450000||!data.fromBase64Encoding(bank)||!decodeZones(data.getData(),data.getSize(),*s.bank))return false;}if(s.useZones&&!s.bank)return false;result=std::move(s);return true;
}
}
