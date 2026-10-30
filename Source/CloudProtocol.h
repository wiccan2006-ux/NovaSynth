#pragma once
#include "PatchSnapshot.h"
namespace nova {
constexpr int cloudProtocol=1;
constexpr int cloudMaxReply=2*1024*1024;
inline bool cloudEndpointAllowed(const juce::String& text){juce::URL url(text);auto host=url.getDomain().toLowerCase();return host.isNotEmpty()&&text.length()<=512&&!text.fromFirstOccurrenceOf("://",false,false).upToFirstOccurrenceOf("/",false,false).contains("@")&&!text.contains("#")&&url.getQueryString().isEmpty()&&(url.getScheme()=="https"||(url.getScheme()=="http"&&(host=="127.0.0.1"||host=="localhost")));}
inline std::vector<PatchSnapshot> readCloudSnapshots(const juce::var& reply){
 if(static_cast<int>(reply["protocol"])!=cloudProtocol||reply["status"].toString()!="done")throw std::runtime_error("Respuesta de servidor incompatible.");
 auto* array=reply["candidates"].getArray();if(!array||array->isEmpty()||array->size()>3)throw std::runtime_error("Numero de propuestas invalido.");std::vector<PatchSnapshot> out;
 for(auto& value:*array){auto text=value.toString();if(text.length()>600000)throw std::runtime_error("Propuesta demasiado grande.");auto xml=juce::XmlDocument::parse(text);PatchSnapshot s;if(!xml||!decodeSnapshot(juce::ValueTree::fromXml(*xml),s))throw std::runtime_error("Propuesta de sintesis invalida.");out.push_back(std::move(s));}return out;
}
}
