# Mapa para el desarrollador

| Area | Archivos principales |
| --- | --- |
| Entrada del plugin | Source/Plugin.cpp |
| APVTS, MIDI, estado, trabajos y transporte | Source/Processor.h |
| Ventana, navegacion y teclado | Source/Editor.h, Piano.h, ComputerKeys.h |
| Motores y parametros | SynthCore.h, Parameters.h, WaveTables.h, SpectralModel.h, ZoneBank.h |
| Efectos y proteccion | Effects.h, Modulation.h |
| Analisis y busqueda | AutoFit.h, PartialAnalysis.h, InstrumentFit.h, ContextAnalysis.h, ContextFit.h |
| Revision/refinado | ReferenceReview.h, Refinement.h, AssistantView.h |
| Referencias y recorte | ReferenceFile.h, Selection.h, ReferenceView.h, SelectionPreview.h |
| Comparaciones | Diagnostics.h, MusicalComparison.h, AnalysisView.h, RecreateWorkspace.h |
| Versiones | PatchSnapshot.h y metodos del procesador |
| Presets y catalogo | Presets.h, DanceBank.h, SignatureBank.h, PresetTraits.h, CatalogData.h |
| Interfaz visual | GlassStyle.h, Visuals.h, PerformanceView.h, PresetBrowser.h, ModulationView.h |
| Redes locales | NeuralSeed.h, NeuralWeights.h, MultiNeural.h, MultiNeuralWeights.h |
| ARP/MIDI | Arpeggio.h, ArpView.h, ArpMidi.h |

El PCM original no se entrega al motor como forma de onda de instrumento.
Los trabajadores leen y analizan referencias; publican parametros y modelos.
Los modelos compartidos tienen rutas de intercambio/retiro para evitar destruir
buffers grandes en el callback. La audicion es un transporte separado.

Los parametros persistentes, matrices y snapshots deben seguir cargando estados
antiguos con valores neutros para campos que no existian. No renumerar programas.
El catalogo medido se genera con Training/AuditCatalog.cpp y BuildCatalog.py;
las etiquetas descriptivas son reglas sobre diseno, no un clasificador acustico.
