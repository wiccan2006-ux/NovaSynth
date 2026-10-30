# Nova Synth 0.20 beta: interfaz por tareas

Autor: Eleander Sanchez (Bodby Olwen).

La pantalla inicial Tocar muestra preset, cuatro macros, expresividad, volumen y teclado.
Explorar reune Banco y Mis sonidos. Recrear conserva seleccion/revision/recreacion/comparacion
como pasos independientes. Deshacer/Rehacer y Guardar permanecen arriba en todos los espacios.

Editar sonido conserva las siete secciones: Osciladores, Parciales, Efectos, Matriz,
Arpegiador, Diagnostico y Variaciones. Se recuerda la ultima seccion elegida. Volver a tocar
cierra la edicion sin modificar parametros. Se conserva el teclado y la ventana redimensionable.

Los filtros adicionales de Banco se abren en Filtros y ajustes. La biblioteca muestra
primero busqueda/escucha/carga; Guardar actual abre los campos de guardado, y
Importar / exportar despliega los archivos. Guardar en la cabecera abre directamente
el formulario de Mis sonidos. No se eliminan variantes, favoritos, bloqueos ni preescucha.

En Recrear, Cargar audio esta dentro del primer paso. Revisar fragmento inicia la revision
y abre las notas. Continuar a recrear abre la busqueda. Opciones de seleccion conserva
Auto nota/Tono limpio/Contexto y el acceso directo de recreacion anterior. Detalles de
revision conserva la correccion MIDI, los objetivos de refinado, el informe y deshacer ajuste.
Detalles y opciones en Recrear conserva familia, afinacion, calidad de busqueda, IA local,
notas elegidas e informe. Se recuerda que desplegables has abierto con el proyecto/preset.
Las alternativas, su escucha y los controles de refinado aparecen cuando hay resultados.

Opciones reune Aspecto y colores, Guia rapida, Abrir preset desde archivo y Acerca de.
Se mantienen los siete colores, brillo neon, tinte, texto, animacion y proteccion de salida.
La actividad y la seleccion destacan; se retiran los diagnosticos permanentes de Tocar.
Los detalles y medidores tecnicos siguen en Diagnostico y en los informes desplegables.

Compatibilidad: no cambia el DSP, el banco/calibracion, los pesos neuronales, los IDs
ni el orden de parametros de 0.19. Se agregan solo preferencias de interfaz al estado;
los proyectos antiguos usan desplegables cerrados y la seccion Osciladores. La apertura
de una ventana empieza en Tocar, independientemente del modo antiguo. No guarda PCM nuevo.

Pruebas: EditorIntegrationTests recorre las tres tareas y siete secciones, comprueba que
los controles anteriores siguen accesibles y que navegar no altera ningun parametro;
verifica desplegables, persistencia, biblioteca, teclado, migracion, recreacion y versiones.
HostContractTests comprueba audio/estado/automatizacion. Se revisan capturas a distintos
tamanos. Compilar Windows y probar componentes no equivale a validar dentro de FL Studio.
