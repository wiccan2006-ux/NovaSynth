# Estado publicado 0.22: prototipo validado localmente

Busqueda exhaustiva y servicio CPU experimental; servidor publico/GPU no desplegado.
No hay entrenamiento neuronal nuevo. Lee ACTUALIZACION_0.22.md. Compilacion cruzada Windows y pruebas locales: 10/10 DSP, AudioProcessor, GUI y HTTP real PASS. Actions sigue en cola. EXE/ZIP, recursos exactos VST3/fuentes, version/manifest y SHA256 verificados. Publicado v0.22.0-beta desde fde96c6fae030b773673bbf3fc602fc56cabba1d. FL Studio interactivo pendiente.

# Estado publicado 0.21

Acceso rapido a recientes/favoritos, filtros visibles y avisos globales.
Lee ACTUALIZACION_0.21.md. Actions 41089733181 / commit 43650dd79a113fc1b1e1e9ce09727a1fbf5b6a60: Windows/MSVC y empaquetado SUCCESS; 9 pruebas DSP, contrato AudioProcessor y pruebas de interfaz/migracion/recientes/favoritos/filtros SUCCESS. Capturas revisadas. EXE/ZIP y recursos verificados. FL Studio interactivo sigue pendiente.

# Estado publicado 0.20

Interfaz reorganizada por tareas. Leer ACTUALIZACION_0.20.md. Conserva las funciones,
812 programas y DSP de 0.19. Verificado: Actions 40984769831 / commit 2e3c83d3adb74fdff369a62944c910928ac9f4dd.
Windows/MSVC y empaquetado SUCCESS; 9 pruebas DSP, contrato AudioProcessor y recorrido
real de componentes SUCCESS. Capturas de Tocar/Explorar/Recrear y edicion revisadas.
FL Studio/Windows interactivo sigue pendiente.

## Registro anterior

# Estado publicado 0.19

Implementacion y condiciones: ACTUALIZACION_0.19.md. Se preservan IDs y 812 programas.
La version 0.18 se compilo y publico desde e3ad5a26e782c7a410ea68fc06e902c121e28038;
Actions 40978482745 aprobo Windows/MSVC y pruebas nativas. Los textos del instalador
se corrigieron entonces. No se ha validado dentro de FL Studio.

0.19 agrega medicion multirregistro del banco, compensacion ponderada, respuesta al toque,
expresividad accesible, informe tonal parcial/completo, menos efectos y prueba AudioProcessor.
Validado: Actions 40981799622 / commit 1722d335cafe0500ff4b1057b325a62e8e9f194c.
Windows/MSVC y empaquetado: aprobados. 9 pruebas DSP + contrato AudioProcessor + recorrido
real de componentes: aprobados. Exportacion de los casos probados: delta maximo 0 entre
bloques de 64/512. Instalador: PE x64, version/manifest, recurso VST3/fuentes y SHA256: aprobados.
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
