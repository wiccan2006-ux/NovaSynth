# Nova Synth 0.19 beta: sonido y confianza

Autor: Eleander Sanchez (Bodby Olwen). Rama feature/sound-quality-019.

## Banco y nivel

Los 812 programas conservan indices, parametros y timbres base. La seleccion principal
se vuelve a calcular a partir de espectro, ataque, envolvente y anchura en tres registros.
Las variantes se agrupan por funcion musical; siguen accesibles y los favoritos no se ocultan.
Se mantienen los 48 Signature. Esto es curacion por medidas, no escucha humana certificada.

QualityAudit.cpp genera 6496 renders: solos en tres registros, dos velocidades suaves,
triadas y triadas agudas fuertes. Una condicion fuerte se repite como comprobacion determinista.
QualityMeter usa paso alto 38 Hz, shelf de 4 dB a 1500 Hz y bloques de 100 ms con puerta
relativa. Es una aproximacion de comparacion, NO LUFS, BS.1770 certificado ni true peak.
La compensacion busca .07 RMS ponderado con limite de pico .70 y trim de -12 a +36 dB.
Los transitorios y el margen para acordes prevalecen: no todos los presets alcanzan el objetivo.
QualityReport.json registra condiciones/resultados y permite reproducir la tabla CatalogData.h.
No se aplican ganancias nuevas al restaurar los valores guardados en un proyecto antiguo.

## Interpretacion e interfaz

Expresividad se edita directamente en Simple/Tocar y se automatiza con el parametro touch
ya existente. Afecta ataque y brillo segun velocidad; ahora tambien la salida y la fase
inicial cambian sutilmente por pulsacion. No introduce desafinacion aleatoria. Cero mantiene
la articulacion fija anterior. No es legato monofonico, portamento ni modelado de un interprete real.
El sustain MIDI sigue disponible; se validan liberacion, velocidad cero y canales fuera de rango.
Los paneles de macros tienen respaldo oscuro de mayor contraste y el nombre del primer
macro refleja el motor. La animacion existente respeta Animaciones suaves.

El pie muestra Audio (fraccion del tiempo de bloque usada por este procesador), voces y
GR (reduccion de ganancia del protector). Audio no es el consumo total del sistema ni
una garantia de ausencia de cortes. No se envian estadisticas a ningun servidor.

## Recreacion y refinado

El informe enumera distancias de ataque, cuerpo y caida y el numero exacto de referencias
con medicion tonal fiable. Una medicion parcial ya no se presenta como verificacion completa.
La desviacion informada se mide antes de la opcion Conservar desvio.
La instruccion explica como escoger las notas del sonido que interesa en Revisar: no hay
separacion automatica de instrumentos ni reconocimiento instrumental seguro.

Demasiados efectos agrega un objetivo de refinado que penaliza el ambiente y saturacion,
prueba una alternativa mas seca y evita subir esos cuatro controles por encima del inicial.
Conserva motor, mapeo y afinacion, y los cambios aceptados entran en Versiones/Deshacer.
Las demas opciones ya disponibles permiten ataque, cuerpo, metal, brillo y cola.
Los pesos neuronales no cambian; no se promete copia perfecta ni red nueva.

## Validacion

QualityTests: linealidad de medida, silencio, afinacion expresiva, pedal y liberacion en
22050/44100/48000/96000 Hz. LevelTests comprueba 2436 renders calibrados adicionales.
HostContractTests: 5 programas x 3 frecuencias; restaurar estado, automatizar, MIDI con
posiciones interiores al bloque, pedal, pitch wheel y exportacion a bloques 64/512.
Compara el audio, comprueba finitud y limites; tambien bloque vacio y PANIC.
EditorIntegrationTests mantiene recorrido real y capturas de componentes, incluyendo
acceso a Expresividad y a la octava opcion de refinado.
CI compila VST3/instalador con MSVC en Windows y ejecuta pruebas nativas en Linux.
Una compilacion o prueba AudioProcessor no equivale a abrir FL Studio.

Pendientes externos: escuchar referencias reales autorizadas y comprobar el instalador,
DPI, foco, automatizacion, reapertura de FLP y exportacion dentro de FL Studio en Windows.
Usar docs/PRUEBA_FL_STUDIO.md y registrar resultados; nunca marcarlos aprobados sin ejecutarlos.
