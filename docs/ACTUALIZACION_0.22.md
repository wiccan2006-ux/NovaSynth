# Nova Synth 0.22 beta: busqueda exhaustiva y servicio experimental

Exhaustiva / 3600 conserva los candidatos de Profunda / 1200 y refina varios
motores con varias notas. Acepta solo candidatos con menor distancia interna y
que superen la comprobacion tonal. No garantiza mejora perceptual universal.

Servicio CPU real en Server/: HTTP con autenticacion, WAV seleccionado hasta 60 s,
una tarea por proceso, tiempo/bytes limitados, heartbeat y borrado temporal.
Devuelve hasta tres snapshots con parametros y modelos; nunca audio como oscilador.
El plugin valida protocolo, rangos, integridad y afinacion local antes de publicar.
Errores/cancelacion conservan el instrumento. Direccion/clave solo en memoria de la
instancia, nunca proyectos/presets. Antes de cada envio muestra destino y duracion.

IMPORTANTE: no hay servidor publico desplegado, GPU contratada ni nuevo modelo
entrenado con instrumentos reales. La prueba usa servicio local real y sonidos
sinteticos. Habilitar internet requiere desplegar Server/Dockerfile en una cuenta
con TLS y definir presupuesto. Esta entrega es el primer prototipo medible, no
la totalidad de la arquitectura propuesta. Separacion de mezclas, entrenamiento
nuevo, GPU y evaluacion auditiva con instrumentos reales siguen pendientes.

Conserva IDs/812 programas y JUCE 7.0.12. FL Studio interactivo pendiente.

Validacion local: 10/10 DSP y recreacion; AudioProcessor a tres frecuencias y cinco
programas, delta 0 en bloques 64/512; GUI (menu Exhaustiva y cancelar configuracion),
HTTP real con worker, afinacion y deshacer aprobados. Compilacion cruzada Windows
aprobada. Actions no aporta resultado en esta sesion; prueba FL Studio pendiente.
Datos y medidas sinteticas en Training/CloudPrototype*.json.
