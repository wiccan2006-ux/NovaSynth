# Nova Synth 0.21 beta: acceso mas rapido

- Favorito directo junto a Tocar; el menu Recientes / favoritos carga los ultimos 12
  presets del banco o un favorito sin pasar por todas las categorias.
- Recientes son unicos, mas nuevo primero, y se guardan con el proyecto/preset.
  Las preescuchas no entran al historial; sonidos personales siguen en Mis sonidos.
- Explorar muestra cantidad, filtros activos y Limpiar filtros incluso con los
  ajustes cerrados. Limpiar vuelve a 192 principales sin cambiar el instrumento,
  nivel o bloqueos. Sin coincidencias, explica que hacer y desactiva cargar/escuchar.
- Aviso permanente en cabecera y Ver aviso para leerlo completo en cualquier vista.
  Los errores de archivo y resultados de tareas ya no dependen de abrir Diagnostico.
- Favoritos globales se reflejan en el navegador abierto.

Se conservan 812 IDs, orden de parametros, motores, bancos, pesos neuronales y
compatibilidad con proyectos anteriores. No se modifica el motor de recreacion ni
se promete identificacion universal. La muestra real del usuario sigue pendiente.

Validacion aprobada: Actions 41089733181 compila Windows/MSVC y ejecuta 9 pruebas
DSP, AudioProcessor (3 frecuencias x 5 programas, delta 0 entre bloques 64/512) y
recorrido nativo de interfaz. Incluye migracion, recientes corruptos/duplicados,
favoritos, filtros vacios y limpiar sin cambiar el instrumento. Capturas revisadas.
EXE/ZIP, recursos VST3/fuentes, version y SHA256 verificados. La prueba dentro de
FL Studio sigue pendiente.
