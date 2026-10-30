# Alcance de 0.18 beta

Rama: feature/intuitive-recreation-018. Desarrollador: Eleander Sanchez (Bodby Olwen).

Implementacion: WorkflowTools/Methods/View para historial, variacion y navegacion;
PersonalLibrary para presets en disco; RecreationSearch/View para busqueda automatica
multimotor/multinota y alternativas. AssistantView permite corregir notas y refinar.
No se renumeran programas ni parametros, no se cambian JUCE ni los pesos neuronales.
El PCM de referencia no se entrega al motor como oscilador ni se guarda en los presets.

Las variantes de recreacion tienen funciones diferentes, no tres promesas de calidad:
limpia reduce ruido y movimiento; expresiva modifica respuesta a velocidad MIDI.
El parecido se ordena con distancias internas, no con un porcentaje validado.
Los candidatos se evalúan en las notas/intensidades elegidas, no en todo el teclado.
Las zonas usan asignacion por cercania; no hay interpolacion continua entre zonas.
La busqueda de presets similares compara parametros, no escucha una biblioteca externa.

Pruebas: RecreationWorkflowTests valida cinco motores, varias notas, variacion,
bloqueos, cancelacion y control tonal. EditorIntegrationTests comprueba componentes
reales, la pagina correcta de cada pestana, biblioteca, deshacer/rehacer, estados y
recreacion asincrona. Sus capturas se conservan como artefacto de Actions.
Una prueba nativa Linux no equivale a una prueba del host FL Studio/Windows.

Compilacion: release-build.yml compila en Windows 2022/MSVC, genera instalador,
fuentes completas, ZIP alternativo, VST3 portable y SHA256SUMS. No publica automaticamente
una Release: primero deben pasar las comprobaciones y revisarse los archivos.
Packaging/package_windows_native.py requiere el entorno de compilador/SDK de Visual Studio.
El metodo LLVM-MinGW anterior permanece disponible; usa empaquetado con raices explicitas
para evitar incluir builds, respaldos Git y ZIP de versiones anteriores recursivamente.

Pendiente para evaluar calidad perceptual: referencias reales autorizadas, escucha en
varios registros/dinamicas y validacion con proyectos reales de FL Studio.
La fase de entrenamiento neuronal con grabaciones reales no forma parte de 0.18.
