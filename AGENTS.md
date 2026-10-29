# Instrucciones de continuidad

Lee README.md, docs/CONTINUAR.md y docs/ESTADO.md antes de modificar el proyecto.
La tarea y las preferencias actuales del usuario prevalecen sobre esta guia.

- Conserva el nombre del desarrollador: Eleander Sanchez (Bodby Olwen).
- Conserva fabricante/plugin IDs y los IDs/orden anteriores de parametros.
- Conserva los 812 indices de programas y la compatibilidad de estados antiguos.
- El PCM de referencia solo se usa para analisis/audicion, no como oscilador sampler.
- No prometas copias perfectas ni una IA nueva sin implementacion y pruebas reales.
- No describas una prueba nativa de interfaz como una prueba en FL Studio.
- No cambies JUCE 7.0.12 ni archivos de ThirdParty en una tarea rutinaria.
- Haz cambios en una rama, con pruebas adecuadas al riesgo. No hace falta probar
  DSP por un cambio exclusivamente documental.
- Para un cambio de version actualiza CMakeLists.txt, Packaging/BuildConfig.h,
  Packaging/package_installer.py, Packaging/Setup.c, setup.manifest, interfaz,
  guia y validacion. Lee todos los textos de Setup.c: hay cadenas antiguas.
- Guardar fuentes completas y licencias junto con cualquier nuevo instalador.
- Los binarios de una release se conservan como artefactos, no dentro de Source.
