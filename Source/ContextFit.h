#pragma once
#include "ContextAnalysis.h"
#include "AutoFit.h"
namespace nova {
struct ContextFitReport {ContextAnalysis analysis;std::shared_ptr<ZoneBank> bank;FitReport fit;};
inline ContextFitReport fitContextNotes(const Sound& source,double offset,ContextAnalysis analysis,int totalBudget,std::atomic<bool>& cancel,bool clean=true,const std::function<void(int,int,double,float)>& progress={},const std::function<void(int)>& stage={}){
 ContextFitReport out;out.analysis=std::move(analysis);
 if(cancel.load()||out.analysis.cancelled){out.fit.cancelled=true;return out;}
 if(out.analysis.picked.size()>maxZones){out.fit.error="Selecciona como maximo ocho notas.";return out;}
 if(out.analysis.picked.empty()){out.fit.error="No hay notas aisladas suficientemente estables. Usa el mismo instrumento sin mezcla ni acordes.";return out;}
 auto bank=std::make_shared<ZoneBank>();std::vector<ContextNote> accepted;int budget=std::max(40,totalBudget/static_cast<int>(out.analysis.picked.size())),completed=0;
 for(auto candidate:out.analysis.picked){
  if(cancel.load())break;auto note=extractContextNote(source,candidate);if(!note.tonal||std::abs(note.root-candidate.note)>1)continue;
  auto target=makeTarget(note,note.root,totalBudget>=1200?44100:22050,false);if(stage)stage(1);
  auto fit=fitParameters(target,Params{},budget,cancel,[&](int trial,int,double loss){if(progress)progress(completed*budget+trial,static_cast<int>(out.analysis.picked.size())*budget,loss,.15f+.85f*static_cast<float>(completed*budget+trial)/(out.analysis.picked.size()*budget));},2,{},false,clean);
  if(cancel.load())break;if(!fit.valid||!fit.model)continue;bool duplicate=false;
  for(int i=0;i<bank->count;++i)if(bank->zones[i].model.root==fit.model->root&&std::abs(bank->zones[i].velocity-candidate.velocity)<.09f)duplicate=true;
  if(!duplicate){candidate.note=fit.model->root;candidate.range.start+=offset;candidate.range.end+=offset;bank->zones[bank->count++]={*fit.model,candidate.velocity};accepted.push_back(candidate);if(!out.fit.valid)out.fit=fit;}++completed;
 }
 if(cancel.load()){out.fit.cancelled=true;return out;}
 if(!bank->valid()){out.fit={};out.fit.error="No se pudo construir un instrumento tonal fiable. Prueba otro contexto.";return out;}
 out.analysis.picked=std::move(accepted);out.bank=bank;out.fit.parameters.engine=1;out.fit.parameters.useZones=true;out.fit.trials=completed*budget;out.fit.error="Contexto aprendido: toca tus notas en el piano roll; no se reproduce la frase original.";if(progress)progress(out.fit.trials,out.fit.trials,out.fit.bestError,1);return out;
}
inline ContextFitReport learnContextInstrument(const Sound& source,double offset,int totalBudget,std::atomic<bool>& cancel,bool clean=true,const std::function<void(int,int,double,float)>& progress={},const std::function<void(int)>& stage={}){
 if(stage)stage(0);auto analysis=analyseContext(source,cancel,[&](float value){if(progress)progress(0,totalBudget,0,value*.15f);});
 return fitContextNotes(source,offset,std::move(analysis),totalBudget,cancel,clean,progress,stage);
}

}
