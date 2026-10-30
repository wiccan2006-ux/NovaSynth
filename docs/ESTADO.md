# Estado de trabajo 0.19

Implementacion y condiciones: ACTUALIZACION_0.19.md. Se preservan IDs y 812 programas.
La version 0.18 se compilo y publico desde e3ad5a26e782c7a410ea68fc06e902c121e28038;
Actions 40978482745 aprobo Windows/MSVC y pruebas nativas. Los textos del instalador
se corrigieron entonces. No se ha validado dentro de FL Studio.

0.19 agrega medicion multirregistro del banco, compensacion ponderada, respuesta al toque,
expresividad accesible, informe tonal parcial/completo, menos efectos y prueba AudioProcessor.
Consultar Actions del commit 0.19 antes de marcar compilacion y checks como aprobados.
Escucha humana del banco y prueba FL Studio pendientes. PRUEBA_FL_STUDIO.md contiene casos.

## Registro historico 0.17 (no describe los avances de 0.18/0.19)

# Estado real del proyecto al entregar 0.17

## Implementado

- Cinco motores: FM/substractivo, 48 parciales mas ruido, wavetable procedural,
  cuerda pulsada y resonador de soplo. Efectos internos.
- Ajuste de parametros por referencia y semillas neuronales entrenadas con
  sintesis propia. No modelo general entrenado con instrumentos reales.
- Contexto hasta 60 s, hasta ocho zonas por nota/intensidad; revision y refinado.
- Comparacion musical monofonica temporal, LAB, versiones y ARP de 16 pasos/MIDI.
- 812 programas compatibles: 192 principales y 620 variantes agrupadas por
  mediciones. Los 48 Signature conservan sus posiciones.
- Calibracion RMS por programa y proteccion opcional de pico de muestra.
- Cuatro rutas de modulacion, toque expresivo y preferencias visuales persistentes.
- Interfaz JUCE con vidrio interno, siete temas, texto escalable, teclado PC/MIDI.

## Evidencia disponible

VALIDACION.txt y Training/*Report.json registran ensayos sinteticos, pruebas
nativas de componentes y comprobaciones de archivos/compilacion. No se conservan
las carpetas temporales de cada ejecucion. Las pruebas se pueden ejecutar otra vez.
Las fuentes entregadas coinciden con las publicadas dentro del instalador 0.17.

## Pendientes concretos

1. Ejecutar instalacion y VST3 en Windows/FL Studio: foco, DPI, teclado, parametros,
   guardado, rendimiento y audio en una maquina real del usuario.
2. Analizar una muestra real del usuario que se recree mal, con evaluacion auditiva.
3. Curacion perceptual del banco: el agrupamiento por metricas no demuestra que
   los 192 principales sean auditivamente unicos o buenos en cualquier mezcla.
4. Armonizar los textos del instalador: Setup.c aun tiene un mensaje de bienvenida
   que dice 0.13/764 presets y una instruccion de recorte de 3 s. Esos textos no
   describen todas las funciones de 0.17. Corregirlos en la siguiente compilacion.
5. No hay separador de fuentes, transcripcion polifonica universal, sistema de
   capas multiples ni libreria de grabaciones acusticas.
6. LFO de la matriz libre, sin sincronizacion al host. Algunos parametros nuevos
   como palette/velocitytone se editan desde el host, sin editor dedicado.
7. No hay firma digital ni CI de Windows/FL validado.

Esta entrega organiza el proyecto; no cambia el plugin ni reempaqueta su version
publicada. Los ejecutables conservan sus hashes originales.
