# NOVA SYNTH 0.22 BETA

Recrear > Detalles y opciones > Exhaustiva / 3600: mas busqueda en este equipo.
Usar servidor es opcional y requiere un servicio Nova desplegado; no hay servidor
publico incluido. Configura direccion/clave, activa y confirma cada envio.
Consulta docs/ACTUALIZACION_0.22.md para alcance y limites.

# NOVA SYNTH 0.21 BETA

Recientes / favoritos carga un sonido del banco directamente; Favorito marca el actual.
Recientes guarda hasta 12 sonidos con tu proyecto. Explorar muestra los filtros
activos y permite limpiarlos sin cambiar el sonido. Ver aviso abre el mensaje
completo al guardar, cargar o terminar una tarea. Las funciones anteriores se conservan.

# Nova Synth 0.20 beta — Menos a la vista, todo a mano

Eleander Sanchez (Bodby Olwen).

Cierra FL Studio, ejecuta NovaSynth_0.20_Instalar_Windows_x64.exe y escanea plugins.
Comprueba 0.20 en la cabecera. El instalador incluye fuentes completas y licencias.

- Tocar: elige preset, mueve las cuatro macros y toca. Expresividad responde al toque.
- Explorar: Banco o Mis sonidos. Busca, preescucha y carga. Filtros y ajustes abre lo extra.
- Recrear: carga/selecciona, revisa, recrea y compara. Las opciones detalladas se despliegan.
- Editar sonido: Osciladores, Parciales, Efectos, Matriz, Arpegiador, Diagnostico y Variaciones.
  Volver a tocar cierra los detalles sin cambiar el sonido. Se recuerda la ultima pestana.
- Guardar abre Mis sonidos con nombre, categoria y etiquetas. Importar / exportar abre archivos.
- Deshacer/Rehacer permanecen arriba. Ctrl+Z / Ctrl+Mayus+Z / Ctrl+Y siguen funcionando.
- Opciones: colores, vidrio, neon, texto, animacion, guia, abrir preset y creditos.

Conserva los 812 programas, parametros, efectos, modelos y funciones de 0.19.
No cambia el motor ni la calibracion. Los desplegables se recuerdan con el proyecto/preset;
la ventana abre en Tocar. Los proyectos antiguos siguen cargando sus valores.
Espacio escucha la referencia en Recrear; teclado PC/MIDI, Z/X y Esc siguen disponibles.

Detalle: docs/ACTUALIZACION_0.20.md. Prueba en FL Studio: docs/PRUEBA_FL_STUDIO.md.
Las pruebas nativas y la compilacion Windows no sustituyen validar dentro de FL Studio.

---

# Nova Synth 0.19 beta — Sonido y confianza

Desarrollador: **Eleander Sanchez (Bodby Olwen)**.

Cierra FL Studio y ejecuta NovaSynth_0.19_Instalar_Windows_x64.exe. Escanea plugins y
comprueba v0.19. Las fuentes completas y licencias acompanian el instalador.

- Banco: seleccion principal revisada con medidas en tres registros y nueva compensacion
  ponderada de nivel. Desmarca Solo principales para ver los 812 programas originales.
  Algunos sonidos mantienen menor nivel para respetar sus transitorios y el margen de acordes.
- Simple/Tocar: Expresividad controla la respuesta a velocidad, ataque, brillo y salida.
  Cero conserva articulacion fija; no agrega vibrato ni desafinacion aleatoria.
- Recrear: el informe muestra ataque/cuerpo/caida y cuantas notas pudo afinar de forma fiable.
  Si hay mezcla, abre Revisar, escucha las regiones y elige las del instrumento que te interesa.
- Mejorar segun mi oido: nueva opcion Demasiados efectos; los resultados aceptados conservan
  versiones y se pueden deshacer. Comparar A/B ayuda a decidir por el oido.
- Audio en el pie indica el tiempo de proceso frente a duracion del bloque; GR muestra la
  reduccion del protector. No es el porcentaje de CPU total del ordenador.

Conserva IDs y programas. Los valores de proyectos/presets anteriores se restauran;
la nueva calibracion se aplica al cargar programas de fabrica, no sobre sus valores guardados.
No hay nueva red entrenada con instrumentos reales, separacion de mezclas ni copia perfecta.
La compilacion automatica no sustituye probar el plugin dentro de FL Studio.

Detalle tecnico y limites: docs/ACTUALIZACION_0.19.md. Prueba manual: docs/PRUEBA_FL_STUDIO.md.

---

# Nova Synth 0.18 beta — Explorar, guardar y recrear

Desarrollador: **Eleander Sanchez (Bodby Olwen)**.

## Instalar 0.18

Cierra FL Studio, ejecuta NovaSynth_0.18_Instalar_Windows_x64.exe y confirma la instalacion.
En FL Studio abre Options > Manage plugins > Find installed plugins y carga Nova Synth.
La interfaz debe mostrar v0.18. No necesitas instalar Python, Git ni modelos aparte.
Conserva 812 programas y los IDs de parametros anteriores. Guarda copia de tus proyectos.
Fuentes y licencias se instalan junto con la guia en Program Files/Nova Synth.

## Explorar sin perder tu sonido

- Simple ofrece cuatro macros. El selector Simple/Avanzada/Tocar abre todos los controles
  o una vista grande para interpretar. El teclado PC y MIDI siguen disponibles.
- Banco selecciona sin cargar: elige una fila, preescucha una nota, acorde o secuencia
  y pulsa Cargar elegido. Doble clic tambien carga. Anterior/Siguiente cargan dentro
  del filtro. La escucha se sintetiza por separado y no cambia el programa actual.
- Combinar filtros exige todas las etiquetas elegidas. Buscar similares ordena por
  cercania de controles de timbre y envolvente; no es un buscador neuronal de audio.
- Fijar FX conserva los efectos al cargar sonidos y durante la preescucha de fabrica.
  Fijar volumen conserva Output, ganancia y compensacion al cargar. La preescucha
  tiene igualacion RMS independiente, limitada por pico; no es LUFS.
- Deshacer/Rehacer conservan hasta 64 cambios de instrumento, modelos, bancos y ARP.
  Las ediciones continuas se agrupan tras 450 ms sin cambios; comenzar otro arrastre
  crea un limite. Ctrl+Z, Ctrl+Mayus+Z y Ctrl+Y no interceptan edicion de texto.
  Restaurar estado desde el host limpia este historial; Abrir preset es deshacible.
- Variar ofrece Sutil/Media/Amplia. Bloquear fija timbre, envolvente, FX o movimiento.
  Siempre conserva el motor, la afinacion, el mapeo tonal y las ganancias.
- Doble clic restablece una perilla. Sus ayudas explican el efecto en lenguaje sencillo.
  Guia ofrece tres pasos opcionales. Esc o PANIC detienen la escucha y el instrumento.

## Mis sonidos

Guardar abre la biblioteca personal. Escribe nombre, categoria y etiquetas y pulsa
Guardar en biblioteca: crea una copia con identificador nuevo; no pisa otro sonido.
Puedes buscar, marcar favoritos, preescuchar, cargar, importar y exportar .novapreset.
Los sonidos personales estan en %APPDATA%/NovaSynth/Presets y los favoritos de fabrica
se guardan en %APPDATA%/NovaSynth/Favourites.xml. Son propios de la cuenta de Windows.
No se suben automaticamente a GitHub. Copia esa carpeta para respaldar tus creaciones.
Desinstalar Nova Synth no borra esta biblioteca. El indicador Modificado/Guardado
corresponde al sonido; las preferencias visuales se guardan con el proyecto.

## Recreacion sin conocer el instrumento

1. Seleccionar: arrastra audio, marca el recorte, usa lupa/zoom y escucha con Espacio.
   Contexto admite hasta 60 s; el analisis usa regiones representativas, no convierte
   la frase completa en un sampler. Nota aislada conserva el limite de 3 s.
2. Revisar: escucha las regiones, elige hasta ocho notas/intensidades y corrige la nota
   MIDI si hace falta. Confianza, desviacion, variacion y clipping son medidas estimadas.
3. Recrear: Automatico es la opcion inicial. Crear automaticamente revisa y compara
   cinco motores y un candidato por zonas. Recrear notas elegidas respeta la revision.
   El tipo de instrumento es opcional: solo favorece candidatos cercanos, sin excluir
   otros motores. No identifica instrumentos reales con certeza.
4. Comparar: prepara la misma secuencia monofonica detectada para A/B, con volumen
   comparable. Cambiar sonido o referencias invalida la comparacion: prepara de nuevo.
   Versiones conserva los puntos de partida y resultados aceptados.

El ajuste compara ataque, cuerpo y caida en varias notas. Ofrece hasta tres alternativas:
Mas parecido, Mas limpio y Mas expresivo; omite variantes iguales o sin verificacion.
El tono se comprueba cuando es medible; en modo Centrada se rechazan candidatos con
mas de 12 cents de error medido en los ejemplos. Una fundamental ambigua no puede
certificarse. Conservar desvio aplica el desvio mediano y conserva movimiento donde
el modelo lo admite; no recupera exactamente la interpretacion.

Mejorar segun mi oido permite priorizar ataque, cuerpo, menos metal, mas brillo,
menos brillo o cola. Es busqueda local con objetivos medidos, no una red nueva.
Las referencias con poca cobertura tonal, clipping o tono incierto necesitan revision.
No hay separacion automatica de mezclas ni transcripcion polifonica universal.
La IA existente sigue entrenada con sintesis propia. No se promete copia perfecta.
La comprobacion en FL Studio/Windows y con grabaciones reales del usuario sigue pendiente.

---

# Nova Synth 0.17 beta — Banco organizado, nivel y modulacion

**Desarrollador: Eleander Sanchez (Bodby Olwen)**

Instrumento VST3 para Windows x64. Nueva navegacion Instrumento / Recrear / Efectos, onda de referencia ampliada, comparacion musical de la secuencia monofonica detectada y hasta doce versiones del instrumento guardadas en el preset/proyecto. Conserva el vidrio oscuro y el neon de siete colores. Las pruebas no sustituyen comprobarlo en FL Studio en Windows; no se promete una copia perfecta ni un nuevo modelo entrenado con instrumentos reales.

## Instalar o actualizar

1. Cierra FL Studio antes de instalar.
2. Ejecuta `NovaSynth_Instalar_Windows_x64.exe`, acepta el permiso de instalacion y confirma.
3. En FL Studio: **Options > Manage plugins > Find installed plugins**.
4. Carga **Nova Synth** como instrumento en el Channel Rack. La interfaz debe mostrar **NOVA / SYNTH v0.17**.

El instalador incluye el plugin y el modelo de IA. No necesitas Python, internet, cuentas, Git, Visual Studio ni descargar un modelo. Sustituye la version anterior en Common Files/VST3. Guia, fuentes, licencias y desinstalador se instalan normalmente en `C:\Program Files\Nova Synth`. Puedes desinstalar desde Aplicaciones de Windows.

## Novedades 0.17

### Banco principal y variantes

El banco contiene los mismos 812 programas, sin renumerarlos ni reescribir sus definiciones. **Solo principales** muestra 192 representantes y agrupa los otros 620 como variantes por proximidad de espectro, envolvente, ataque e imagen estereo dentro de la misma funcion musical. La seleccion es una ayuda computacional para reducir repeticion; no sustituye una revision perceptual profesional. Los 48 Signature se conservan entre los principales.

- **Banco** inicia en todos los bancos con Solo principales activado. El selector superior y las flechas tambien recorren los 192 principales. Si un proyecto carga una variante, su nombre sigue visible en el encabezado.
- El menu de la ficha permite cargar el principal o cualquiera de sus variantes. Una variante puede proceder de otra coleccion. Desactiva Solo principales para ver los 812; los proyectos anteriores mantienen sus indices.
- Hay filtro por **oscuro, brillante, suave, marcado, corto, largo, evolutivo, agresivo o amplio**. La busqueda tambien encuentra etiquetas y funcion: Bajo, Lead, Pluck/Campana, Pad/Textura, Teclas o Efecto. Son etiquetas derivadas del diseno del preset, no una clasificacion de un instrumento real.
- **Comparar original** alterna el preset de fabrica y tu edicion actual. **Volver a ajustes** recupera todos los parametros editados. **Restablecer** carga el preset original y permite Deshacer ajuste en LAB. La comparacion se ofrece para programas de fabrica; una recreacion usa LAB/Comparar A/B. Cambiar programa, cargar estado/restaurar version o cambiar Nivel parejo termina esa comparacion temporal. Cambiar Nivel parejo mientras escuchas el original recupera antes tus ajustes para no perderlos.

### Nivel por preset y proteccion

**Banco > Nivel parejo** aplica una ganancia de calibracion independiente de Output y de la ganancia manual. Se conserva al cambiar de preset y se guarda en el proyecto. Las instancias nuevas la traen activada. **Los proyectos y presets antiguos conservan su nivel original al abrir: activa Nivel parejo si quieres aplicar la nueva calibracion.** Cambiar esta opcion modifica solo la compensacion, no las perillas del sonido. Al cargar otro programa de fabrica se cortan las voces y colas del anterior para evitar que se amplifiquen con la calibracion del nuevo; las notas MIDI nuevas se procesan normalmente.

Los 812 programas se midieron a 22.05 kHz con una nota a intensidad .8 y un acorde mayor a .65, usando registro grave para bajos y C4 para otros roles. El gate dura como minimo un segundo y llega hasta ataque+.6 s para pads lentos; se agrega .8 s de cola. Un tercer ensayo con acorde una octava arriba e intensidad 1 comprueba los picos. Se calibra el RMS estereo conjunto de nota/acorde hacia .07, limitado por pico .70 en esos ensayos y ajuste de -12 a +36 dB. La mediana del aumento calculado es aproximadamente +17.4 dB. El rango RMS de la nota de referencia pasa de unos 33.3 dB de diferencia a 13.5 dB. Los transitorios y las diferencias entre registros limitan la igualacion: no todos los sonidos deben tener la misma energia media.

Es calibracion estatica RMS, no LUFS, AGC continuo ni garantia del mismo volumen en cualquier interpretacion. Cambiar macros, notas, acordes o efectos puede cambiar el nivel. No se vuelve a calibrar un instrumento reconstruido ni se aplica automaticamente al PCM de referencia.

**Aspecto > Proteger salida** usa reduccion de ganancia vinculada entre L/R cuando el pico de muestra supera .95, con ataque inmediato y recuperacion aproximada de 40 ms. No usa lookahead ni verifica true peak; una entrada excesiva puede alterar los transitorios. Se conserva el recorte de seguridad final existente. La proteccion es opcional y se guarda con el proyecto. Nunca aumenta el nivel de un sonido bajo.

### Matriz y respuesta al tocar

**Instrumento > Matriz** abre cuatro conexiones automatizables. Cada una tiene origen, destino y cantidad bipolar; cero o Sin origen/Sin destino la desactivan. Origenes: LFO1, LFO2, envolvente ADSR, velocidad MIDI, registro y azar por nota. Destinos: filtro, timbre/morph, amplitud y panorama. LFO1 usa la velocidad del panel Controles; LFO2 tiene su propia frecuencia libre. No hay sincronizacion al tempo del host en esta matriz.

Timbre actua sobre FM o sobre la mezcla de tablas; no cambia los resonadores de cuerda/soplo ni reconstruye un modelo de parciales. Azar es determinista por evento y permanece fijo durante esa nota. Envolvente es la del volumen de la voz; no es una segunda envolvente independiente. Las cantidades negativas invierten la respuesta. No se incluyo destino de afinacion.

**Toque expresivo** hace que una pulsacion suave cierre el filtro y prolongue el ataque, mientras una fuerte abre el filtro. Cero conserva el comportamiento previo. Puedes combinarlo con las rutas de velocidad. La matriz, frecuencia LFO2, toque y ganancia de calibracion se guardan en presets/versiones. Al abrir estados antiguos faltantes se inicializan neutros, con LFO2 a 1 Hz.

Las etiquetas avanzadas afectadas por los cuatro macros se iluminan con el color del tema; su tooltip indica que la lectura es el valor base, al cual se suma la accion del macro. No es una animacion de las perillas moduladas ni indica cada conexion de la matriz.

Esta version no agrega muestras acusticas, capas de motores ni reentrenamiento neuronal. La recreacion y la prueba con una referencia real del usuario siguen su flujo anterior. No promete equivalencia con Nexus ni copia perfecta.

## Historial: novedades 0.16

### Interfaz de interpretacion y vidrio

- Vista principal despejada con **Caracter, Brillo, Movimiento y Espacio**, cuatro macros grandes automatizables. **Controles** abre/cierra la vista de perillas anterior. Parciales, Banco, ARP, LAB, Recrear y Efectos se conservan.
- **Aspecto** abre una hoja flotante: **Tinte del vidrio**, **Brillo neon**, tamano de texto y animaciones. El tinte va de claro a oscuro sobre una escena propia desenfocada; el brillo regula los halos de los controles y trazas principales. Los indicadores funcionales conservan su contraste al poner brillo en cero. Se guardan con el proyecto/preset.
- Vidrio con bordes de luz, reflejos, paneles redondeados y navegacion en capsula, inspirado en Liquid Glass. No usa materiales nativos de Apple ni captura el escritorio. Se conservan los siete colores.
- El visor arranca en **En vivo**, con la salida real. Puedes volver a onda, espectro o envolvente desde su selector. No modifica el sonido.
- **Banco** ahora ocupa una vista amplia con buscador, filtro, ficha, descripcion, envolvente y escucha de tres notas (canal interno 15). La escucha usa el sintetizador actual, se puede detener y se libera al ocultar Banco, cargar otro sonido o cerrar el editor. No es una muestra grabada. Favoritos se guardan con este proyecto/preset, no como biblioteca global entre instancias.

### Banco Signature: 48 nuevos sonidos

**812 presets en total.** Los primeros 764 conservan sus posiciones y definiciones. El filtro inicial de Banco muestra los 48 nuevos. Hay seis colecciones de ocho: Vocal/Formantes, Cristal/Percusion, Organico/Hibridos, Bajo/Materia, Teclas/Refraccion y Textura/Movimiento. Ejemplos: Voces de mercurio, Prisma liquido, Motor de tinta, Memoria refractada y Maquina de luciernagas. Una instancia nueva inicia con Prisma liquido; los proyectos guardados recuperan su instrumento.

Los 48 tienen ajustes explicitos de motor, envolvente, filtro, intensidad, modulacion y efectos. El motor wavetable incorpora seis pares nuevos de espectros armonicos calculados por sintesis y limitados en banda. Movimiento cambia el cruce entre espectros y la anchura, sin cambiar la frecuencia de los osciladores. Los hibridos tambien usan cuerda pulsada, soplo y FM. El volumen tiene calibracion estatica moderada a partir de una nota de referencia; no es normalizacion LUFS ni garantiza el mismo volumen en todo registro/acorde.

- **Caracter:** densidad espectral, FM y saturacion, segun el motor.
- **Brillo:** abre el filtro hasta tres octavas; si ya esta abierto o limitado por Nyquist puede no producir un cambio apreciable.
- **Movimiento:** modulacion del filtro y, en wavetable, del espectro/imagen estereo. No es vibrato de afinacion.
- **Espacio:** aumenta delay y reverb respetando sus interruptores de bypass.

Los macros se guardan como parametros propios. Las perillas avanzadas muestran los valores base; el macro se aplica encima de ellos. Cero es neutro. Los presets/versiones antiguos sin estos parametros se cargan con macros neutros. La paleta de espectro (palette 0..6) y la sensibilidad tonal a velocidad (velocitytone 0..1) tambien se exponen al host y se guardan; sus editores dedicados aun no estan en la interfaz.

Este banco tiene diseños nuevos dentro de Nova; no se afirma que ningun otro plugin pueda hacer timbres parecidos. Esta actualizacion se centra en interfaz y sintesis/presets: conserva la IA y el analisis anteriores, sin prometer separacion automatica de instrumentos, transcripcion polifonica o copia perfecta. La prueba con una muestra real del usuario queda pendiente.

## Historial: novedades 0.15

### Interfaz y navegacion

- **Instrumento** reune osciladores, motor, perillas, Parciales, Banco, ARP y LAB. El visor y los controles conservan sus funciones. LAB y ARP siguen disponibles como editores avanzados.
- **Recrear** contiene cuatro pasos, sin superponer los espacios principales: **Muestra y recorte**, **Aprender**, **Comparar A/B** y **Versiones**.
- En **Muestra y recorte** la onda usa casi toda la altura del area de trabajo. Conserva lupa, zoom con rueda, arrastre, campos Inicio/Fin, desplazamiento, marcadores y audicion con Espacio. El boton superior de esta onda mantiene el flujo AutoFit de nota corta o contexto automatico. Para revisar y elegir las notas antes de aprender, usa Aprender.
- **Aprender** conserva el asistente de 0.14 y sus ocho referencias elegidas. El boton Muestra vuelve al recorte. Cancelar tarea esta disponible en la barra de Recrear, incluso al cambiar de paso durante una tarea.
- **Efectos** reune los interruptores y controles internos de Drive, Chorus, EQ, Delay y Reverb.
- **Texto normal / mediano / grande** aumenta los controles y lecturas principales aproximadamente 0 / 10 / 20%, independientemente de redimensionar la ventana. Los titulos y algunas leyendas compactas mantienen su tamano para conservar la jerarquia. Las etiquetas largas pueden abreviarse al caber; las perillas conservan sus tooltips. La preferencia se guarda en el proyecto/preset y no la cambia restaurar una version de sonido.
- Dos barras de salida muestran los picos reales de los canales izquierdo y derecho. Se conservan las animaciones y la opcion de desactivarlas, el teclado PC/MIDI y la ganancia.

### Comparacion musical

1. Revisa el fragmento en Recrear > Aprender. La secuencia para comparar usa los eventos monofonicos estables de todo el fragmento revisado, no solo las ocho notas elegidas para construir el banco de osciladores.
2. Abre **Comparar A/B** y pulsa **Preparar misma secuencia**. Se renderiza B en segundo plano con el instrumento actual, usando notas, inicios, duraciones e intensidades relativas detectadas. Los sostenidos largos se representan como un solo evento, aunque el aprendizaje haya extraido ventanas cortas de esa nota.
3. **Original A** escucha una copia del fragmento original. **Recreado B** escucha la sintesis de esa secuencia. Ambas tienen la misma longitud; cada escucha empieza al inicio del fragmento. Espacio inicia/detiene el ultimo modo de comparacion. No es un cambio A/B continuo conservando la posicion.
4. Se muestran las dos ondas con la misma escala, los eventos de la secuencia, distancias de ataque/cuerpo/envolvente y el mayor error tonal medido frente a la nota MIDI detectada. Las distancias y la afinacion se calculan en hasta ocho eventos, no en cada nota de cualquier mezcla. Menor distancia es mejor; no equivale a porcentaje de parecido.
5. **Igualar volumen A/B** aplica el mismo RMS objetivo con limite de pico, no LUFS. Si el limite de pico actua, los RMS pueden no ser identicos. No modifica el archivo original.

El instrumento toca notas y osciladores, no el PCM original. El PCM de A solo entra al transporte de audicion y el de B es un render temporal de la sintesis. La comparacion ignora el arpegiador para poder seguir directamente los eventos detectados; no reproduce el pitch bend externo del host ni infiere la velocidad MIDI original.

Cambiar los controles de sonido, el modelo, el banco, el fragmento, la revision o el ajuste de volumen igualado invalida la comparacion. Preparala otra vez para no escuchar datos obsoletos. Cambiar color o texto no modifica el sonido. Cancelar conserva el instrumento; si la comparacion previa corresponde al mismo estado puede seguir siendo util.

La secuencia detectada puede omitir notas, equivocarse o aproximar sus bordes. No transcribe acordes de forma universal ni separa instrumentos: usa una referencia aislada. A conserva las imperfecciones y la afinacion de la interpretacion; B usa las notas MIDI estimadas y los ajustes actuales. Para una nota aislada sigue disponible A afinado y el diagnostico de LAB.

### Versiones dentro del proyecto

- Cada recreacion o refinamiento aceptado guarda automaticamente una version. Antes del primer resultado se conserva un punto de partida.
- En **Versiones**, escribe un nombre y pulsa **Guardar version** para conservar ajustes manuales. Elige una entrada y **Restaurar elegida** para recuperar sus controles, modelos sintetizados, banco, ganancia, programa y patron/BPM/activacion del arpegiador. **Deshacer ajuste** permite volver al estado anterior a restaurar.
- El historial admite **doce versiones**. Al llenarse, la siguiente reemplaza la mas antigua. Los numeros son consecutivos y pueden superar doce; el limite es la cantidad de entradas conservadas.
- El historial se guarda al guardar el preset o el proyecto del host. Cada preset/proyecto tiene su propio historial; abrir otro carga el suyo. Los presets anteriores sin historial siguen cargando normalmente.
- No se incluyen archivos originales, selecciones, revisiones ni audio A/B en el historial. Para comparar o refinar un banco al reabrir debes cargar/revisar y aprender de nuevo la referencia. Restaurar una version conserva el aspecto de la ventana. Las comparaciones son temporales y se vuelven a preparar cuando sea necesario.

Esta entrega se centra en la pantalla Recrear, la comparacion musical y las versiones. El refinamiento sigue siendo el de 0.14: no es una nueva red entrenada con grabaciones reales, un separador de instrumento/efectos ni una garantia de exactitud perceptual. La busqueda y categorias del Banco se conservan; no se agrega un nuevo sistema de favoritos ni entrenamiento real en esta version.

## Funciones heredadas de 0.14: Asistente de recreacion

### Recorrido recomendado

1. Carga tu archivo y marca el fragmento en **MUESTRA**. Con Contexto activo puedes revisar hasta 60 segundos de cualquier parte del archivo; el limite del archivo sigue siendo 60 minutos.
2. Pulsa **ASISTENTE**, junto al selector de grafico, y **1. Revisar audio**. Revisa sin cambiar el instrumento actual. Se muestran hasta 512 regiones candidatas, incluyendo regiones con advertencias de tono o picos recortados. No equivale a transcribir todas las notas de una mezcla.
3. La onda muestra donde estan las regiones elegidas y la fila enfocada. Haz clic en una fila o en la onda para enfocarla; doble clic o **Escuchar A** reproduce solo esa region del archivo original. **Recreado B** sintetiza el instrumento actual a la nota e intensidad relativa de la region. **Espacio** en el asistente alterna reproducir/detener A o B segun el ultimo modo; Detener y Esc/PANIC tambien paran. El arpegiador se omite en esta comparacion por nota.
4. Marca la casilla de cada fila o **Usar esta nota**. Elige hasta ocho notas del mismo instrumento. **Recomendadas** prioriza estabilidad, registros e intensidades diferentes; evita picos recortados y duplicados del mismo registro/intensidad. **Ninguna** limpia la eleccion. Las recomendaciones son heuristicas: escucha y excluye mezclas, acordes u otro instrumento por tu cuenta.
5. Pulsa **2. Aprender elegidas**. Aprende solo tus referencias elegidas, sin rellenar las que excluiste. Se confirma otra vez la fundamental de cada region; se omiten duplicados o notas que no se confirman, y el informe indica elegidas frente a aceptadas. Crea hasta ocho zonas de osciladores parciales y ruido, no un sampler, y desactiva ARP para tocar libremente desde MIDI/piano roll. La referencia original solo se escucha con los controles de audicion.
6. Toca distintas notas y escucha A/B. Elige un objetivo y pulsa **3. Refinar instrumento**. Busca alrededor del patch actual, conservando el motor y la asignacion tonal. Puedes repetirlo o **Deshacer ajuste** para intercambiar con el instrumento anterior. Tambien puedes revisar una referencia y refinar directamente un patch FM/Wavetable/etc existente sin crear un banco nuevo.

### Revision visual y advertencias

Cada region muestra su nota MIDI, tiempos absolutos del archivo, variacion tonal P90-P10 y estado. El panel de detalle muestra confianza estimada, desviacion mediana en cents e intensidad relativa. La cobertura tonal es la fraccion de frames con periodicidad aprovechable; no es parecido ni una certeza de identidad del instrumento. Una cobertura baja puede deberse a silencios, ruido, acordes o efectos y no distingue esas causas con seguridad.

Las recomendaciones exigen confianza tonal de al menos 0.87, variacion de hasta 75 cents y como maximo 0.1% de muestras con pico >=0.999. Puedes incluir manualmente una region advertida, pero esto no repara sus defectos y la fundamental se confirma otra vez antes de aprender. No es un detector universal de acordes o instrumentos ni un eliminador de ruido/reverb.

La curva inicial de la fila es una envolvente RMS normalizada de 24 segmentos, con inicio/cuerpo/final indicados por tercios visuales; no son puntos de segmentacion fisica exactos. Tras refinar se muestran barras grises de antes y de color de despues para ataque, cuerpo y cola. Cada par tiene su propia escala relativa: menor altura significa menor distancia, no un porcentaje. Las cifras exactas y el objetivo aparecen al principio del panel de detalle; otros datos siguen disponibles desplazando el texto.

### Objetivos de refinamiento

- **Equilibrar parecido:** reduce la distancia global de espectro, envolvente, ataque y anchura.
- **Priorizar ataque:** da mas peso a los primeros 32 ms sin dejar de comparar el resto del sonido.
- **Conservar cuerpo:** prioriza el espectro y rechaza ajustes que empeoren su distancia frente al punto inicial. No congela todas las perillas ni garantiza conservar la percepcion exacta.
- **Menos metalico:** agrega una penalizacion heuristica a la energia inarmonica del modelo de parciales; en los otros motores usa FM/resonancia como aproximacion. Puede quitar caracter propio de campanas u otros instrumentos inarmonicos y empeorar otra dimension. Compara antes de conservarlo.

En un banco aprendido por el asistente, cada candidato se compara con todas las notas elegidas a sus alturas e intensidades relativas. En un patch simple se usa la fila enfocada si esta incluida, o la primera elegida. La intensidad es una estimacion por nivel de grabacion; no recupera la velocidad MIDI original. Las notas del banco se vuelven a verificar al final cuando el detector ofrece una fundamental fiable. El refinamiento conserva la nota raiz, Tune, transposicion, Unison, detune y modulaciones de tono del patch; para centrar esos controles usa la opcion de LAB.

La busqueda explora envolventes, filtro, FM, ruido, tiempo y balance de parciales, ganancias de parciales y ciertos ajustes de EQ/delay/reverb. Solo acepta candidatos con mejor objetivo que el punto inicial; esto no equivale a una mejora perceptual garantizada. No cambia frecuencias ni audio original ni aprende pesos neuronales nuevos. El nivel del ganador se ajusta dentro de los limites de Output; conserva el control Gain. El objetivo elegido puede mejorar a costa de otra dimension y se muestran ambas medidas.

El presupuesto deriva de la calidad existente: entre 40 y 360 candidatos, reducido al comparar varias referencias, a 22.05 kHz. Se ejecuta en segundo plano y puede cancelarse; cancelar conserva el instrumento y la revision anteriores. Modificar la seleccion de audio invalida la revision. Cambiar las notas elegidas de un banco obliga a aprenderlo otra vez antes de refinarlo, para evitar mezclar revisiones distintas.

La revision, elecciones, informes y referencias son temporales. Guardar el preset/proyecto conserva el instrumento sintetizado y el banco, sin PCM original; al reabrir hay que cargar/revisar la referencia otra vez para comparar o refinar. Sigue disponible el flujo anterior de Recrear seleccion, contexto automatico, LAB, presets, MIDI y efectos.

El entrenamiento con grabaciones reales, la separacion automatica de instrumento y efectos y la transcripcion polifonica completa siguen pendientes. No se promete una copia indistinguible ni 99.99% de parecido.

## Funciones heredadas de 0.13

- Letras: etiquetas de perillas, menus, controles y leyendas suben aproximadamente 1 unidad; informes LAB pasan de 13 a 14. Los botones suben como maximo medio punto. El redimensionado sigue disponible.
- Animaciones: transiciones de resaltado e interruptores, brillo de teclas con una breve caida, indicador de nivel y mini osciloscopio de salida real. Un reflejo recorre el visor durante la busqueda. No se inventa una onda cuando no hay audio. **Animaciones** desactiva estos movimientos y se guarda en el preset/proyecto. El visor En vivo y el cursor de audicion siguen mostrando datos funcionales aunque se reduzca el movimiento.
- **LAB > A afinado**: escucha una copia de comparacion con la desviacion global en cents centrada en la nota detectada. Original A conserva su afinacion; no se modifica el archivo. A afinado no elimina vibrato, ruido ni efectos y necesita una nota corta o un contexto ya aprendido.
- **LAB > Auto nota / nota MIDI**: desactiva Auto nota y elige la nota de referencia si el detector confunde una octava; despues repite la recreacion. La comparacion B respeta esta nota manual. En un contexto automatico cada zona se sigue analizando por su propia nota.
- **LAB > Centrar tono**: pone a cero Tune, movimiento tonal, detune, chorus y LFO de tono y desactiva Unison. No cambia el pitch bend enviado desde el host/controlador.
- El diagnostico muestra nota original, confianza tonal, nota MIDI objetivo y desviacion frente al objetivo, ademas de distancias de ataque, cuerpo y envolvente. No son porcentajes de parecido y una fundamental incierta no se certifica como correcta.
- La busqueda de nota corta conserva hasta **tres alternativas de motores distintos**, ordenadas por su distancia interna. Elige una y pulsa **Usar alternativa**; compara A/B. Son los mejores candidatos realmente evaluados por cada motor, con su nivel ajustado; no tres entrenamientos nuevos ni tres variantes completas de todo el contexto. Una verificacion final descarta alternativas con nota/octava claramente incorrecta cuando hay una medicion confiable. Si el candidato ganador falla esta verificacion se conserva el instrumento anterior y se informa el problema.
- **Deshacer ajuste** intercambia el ultimo instrumento anterior y el actual: controles, modelo sintetizado, banco y patron/BPM/activacion ARP. Funciona despues de recrear, elegir alternativa o centrar tono. Es un nivel de deshacer temporal; abrir otro preset lo limpia. No deshace la lista de referencias ni restaura archivos originales.
- En contexto largo se estima la dispersion tonal de cada region y se descartan regiones con variacion P90-P10 mayor de 75 cents. El informe muestra la cobertura tonal y descartes; ese dato no mide fidelidad ni separa mezclas. En LAB puedes elegir que nota aprendida comparar.
- Banco manual: elige una referencia concreta para escucharla o quitarla. Permite descartar notas contaminadas por otros instrumentos antes de pulsar Recrear banco. La seleccion manual sigue limitada a 3 segundos por nota y ocho notas.
- **ARP > Exportar MIDI** guarda un compas de 16 pasos en .mid con tempo, nota base y silencios. Conserva el gate de 80% del arpegiador y exporta intensidad fija 100. Arrastra ese archivo al piano roll de FL Studio para editar notas, duraciones e intensidades. El campo MIDI base solo afecta al archivo exportado.

Las alternativas, referencias originales y comparaciones son temporales. Guarda el preset para conservar el instrumento elegido; no contiene PCM de la fuente. El entrenamiento con grabaciones reales, la separacion automatica de instrumento y efectos y la transcripcion polifonica completa siguen pendientes. La arquitectura actual no garantiza copiar cualquier instrumento de forma indistinguible.

## Aspecto heredado de 0.12

Paneles, perillas, botones, menus, banco de presets, teclado y editor de arpegios usan el nuevo material. Los paneles y controles muestran distintas zonas del fondo del plugin, previamente desenfocado; se combinan con capas translucidas, contornos claros y reflejos suaves. Los botones son capsulas y los interruptores tienen un deslizador blanco. El neón se mantiene en curvas, valores y selecciones.

Los siete colores y sus indices se conservan: azul, amarillo, verde, morado, rosado, blanco y naranja. El fondo permanece oscuro. El acabado se dibuja dentro de la ventana del instrumento y no usa el material nativo de iOS ni captura el escritorio. El fondo se calcula y difumina una vez por color y se reutiliza; no se recalcula el desenfoque en cada repintado. El rendimiento y la apariencia completos deben comprobarse dentro del host.

El acabado visual procede de 0.12; los pesos neuronales y los cinco motores conservan su base anterior. Las mejoras nuevas de busqueda, comparacion y revision se describen arriba.

## Novedad 0.11: contexto largo para aprender el instrumento

1. Carga o arrastra un archivo y abre **MUESTRA**.
2. **Contexto / instrumento** viene activado. Puedes seleccionar hasta **60 segundos**, en cualquier parte de un archivo de hasta 60 minutos. Al cargar se marcan inicialmente hasta 15 segundos; amplia o mueve los bordes, o escribe los tiempos de inicio/fin. La lupa sigue disponible.
3. Elige una frase del mismo instrumento, preferiblemente tocando notas aisladas, con distintos registros y ejecuciones suaves/fuertes. Evita mezclas con bateria, voces u otros instrumentos y acordes si quieres un resultado mas fiable.
4. Para selecciones mayores de 3 segundos, el boton cambia a **Aprender contexto**. Pulsalo: primero examina todo el contexto y despues escoge hasta ocho notas/regiones representativas, favoreciendo registros e intensidades distintos y descartando silencios o fragmentos sin tono estable.
5. Los fragmentos realmente usados duran hasta 2 segundos y se marcan en la onda con su nota MIDI. Sus posiciones, notas e intensidades relativas estimadas aparecen en LAB. El contexto largo no se entrega completo a un generador que lo copie: aporta ejemplos para un banco de modelos de parciales y envolventes.
6. Cuando termine, escribe o toca tus notas en el **piano roll de FL Studio**. El banco selecciona el modelo sintetizado mas cercano a cada nota e intensidad. El arpegiador se desactiva al aplicar el contexto, para que tus notas controlen directamente el instrumento. Puedes reactivarlo despues.

**Espacio / Escuchar** en MUESTRA reproduce la seleccion original completa, hasta 60 segundos, solo para revisar la referencia. En **LAB**, A/B y Diagnosticar usan una nota representativa del ultimo contexto aprendido, indicada en el informe; asi comparan original y recreado del mismo fragmento, en lugar de mantener una sola nota durante toda la frase. Si cambias la seleccion larga, aprende primero el nuevo contexto antes de comparar Recreado B.

Para el comportamiento de nota corta, desactiva Contexto: vuelve el limite de 3 segundos y la busqueda entre los cinco motores. Tambien con Contexto activo, selecciones de hasta 3 segundos conservan la busqueda corta. Las referencias manuales de LAB siguen siendo notas de hasta 3 segundos; para una frase larga usa Aprender contexto directamente.

### Que aporta y que no

Un contexto largo puede aportar distintas alturas, ataques e intensidades. No sustituye ejemplos claros: si contiene una sola nota repetida, el informe lo indica, y si no hay notas suficientemente estables conserva el sonido anterior. La intensidad se estima por nivel relativo dentro de la grabacion; compresion, automatizaciones de volumen y cambios de microfono pueden alterar esa estimacion.

La deteccion es heuristica y monofonica; no es una nueva red entrenada con tus grabaciones, un separador de instrumentos ni una transcripcion polifonica a MIDI. No garantiza reconocer cada nota real ni quitar reverb, ruido o todos los defectos de la fuente. Mas duracion no garantiza mas precision. Se sigue recomendando audio del instrumento aislado.

El banco automatico usa el motor de parciales y ruido. La lista de posiciones aprendidas y la referencia son temporales; el banco de modelos y su activacion si se guardan en el preset/proyecto, sin PCM original. Para escuchar A/B al reabrir debes cargar y analizar la referencia otra vez. El modo Contexto se guarda como preferencia; no cambia presets anteriores por si solo.

El analisis se hace en segundo plano y puede cancelarse. El presupuesto se reparte entre las notas elegidas, con un minimo de 40 pruebas por nota; el tiempo puede aumentar segun duracion, cantidad de notas y calidad. En calidad Profunda las notas elegidas se analizan a 44.1 kHz; en Rapida/Normal, a 22.05 kHz.

## Novedades 0.10: recorrido rapido

1. Carga una referencia y elige una nota aislada en **MUESTRA**.
2. Deja **Auto nota**, **Tono limpio** e **IA local** activados. Pulsa **Recrear seleccion**.
3. La busqueda centra la afinacion, compara motores y mejora timbre, envolvente y efectos en fases. El indicador muestra la fase y se puede cancelar conservando el patch previo.
4. Abre **LAB / A-B**. Pulsa **Original A** y **Recreado B** para alternar; cada boton empieza desde el inicio del recorte; tras aprender un contexto largo usa la primera nota representativa del informe. No es una conmutacion a mitad de la frase ni en tiempo real. **Detener** corta la audicion.
5. **Igualar volumen A/B** normaliza ambos audios hacia el mismo RMS objetivo, limitado por el pico. Ayuda a comparar sin que el mas fuerte parezca mejor; no es una medicion LUFS. Desactivalo para conservar sus niveles.
6. **Diagnosticar** mide la seleccion frente al patch actual: frecuencia estimada, desviacion en cents y distancias de ataque, cuerpo y envolvente. La medicion es en segundo plano. Estos valores no son porcentajes de parecido ni certifican afinacion perfecta.

**Recreado B se genera con el patch actual**, no transpone el archivo original. Si el arpegiador esta activado, incluye su patron actual; de lo contrario toca una nota. Si cambias controles, vuelve a pulsar el boton para volver a renderizar. La audicion usa intensidad MIDI aproximada de 102/127. Como los datos del archivo son referencia temporal, A/B y diagnostico requieren volver a cargar el audio al reabrir un proyecto.

## Cinco motores

- **FM / Subtractive:** motor anterior, con sus osciladores y filtro.
- **Parciales + ruido:** hasta 48 osciladores, envolventes y ruido por bandas, utiles para sonidos armonicos e inarmonicos.
- **Wavetable:** tablas periodicas originales limitadas en armonicos. **Table Scan** mezcla las formas OSC A/B y **Scan Motion** mueve esa mezcla con el tiempo. No contiene un ciclo PCM extraido del archivo ni una copia de Serum.
- **Cuerda pulsada:** guia de ondas con excitacion triangular y ruido, realimentacion amortiguada y compensacion de retardo fraccionario. **Pluck Position** cambia la excitacion y **String Decay** la realimentacion. Es un modelo fisico simplificado, no una simulacion completa de guitarra, piano o violin.
- **Soplo resonante:** resonador de guia de ondas excitado de forma tonal y ruidosa. **Bore Feedback** controla la realimentacion y Noise su aire. Es un modelo experimental, no una simulacion fisica completa de todos los instrumentos de viento.

Puedes seleccionar los motores desde MOTOR. Los controles sin efecto en el motor elegido se desactivan. AutoFit prueba los cinco y conserva el mejor segun su distancia interna. Primero construye candidatos de timbre con una envolvente aproximada y sin efectos; despues refina la envolvente y finalmente los efectos. La comparacion de motores y su distancia aparecen en LAB. El suboscilador, el detune automatico, el chorus y los intervalos inferiores de OSC B se excluyen de la busqueda para reducir falsas octavas graves. Siguen disponibles al editar manualmente patches compatibles.

## Aprender de varias notas e intensidades

El banco aprende **modelos de parciales**, no muestras para reproducir:

1. En MUESTRA selecciona una sola nota del instrumento.
2. Abre LAB y elige la **intensidad original** aproximada: suave, media, fuerte o muy fuerte. Este dato lo indica el usuario; el volumen de una grabacion por si solo no determina su velocidad MIDI.
3. Pulsa **Agregar seleccion**. Puedes cerrar LAB, cambiar el recorte o cargar otro archivo y agregar mas notas; maximo ocho referencias.
4. Incluye, por ejemplo, una nota grave, una media y una aguda, con ejecuciones suaves y fuertes del mismo instrumento.
5. Pulsa **Recrear banco**. Las referencias se analizan en segundo plano y cada una genera osciladores y envolventes. Las referencias con tono incierto se rechazan y se conserva el banco anterior. Dos referencias con la misma nota y casi igual intensidad se combinan conservando la ultima.
6. Con **Usar banco por nota/intensidad** activado, al tocar se elige la zona sintetizada mas cercana a la nota y fuerza MIDI. Es seleccion por proximidad, no interpolacion continua entre zonas.

El banco y sus intensidades se guardan en el preset/proyecto sin el audio original. La lista de archivos/recortes es temporal y no se guarda: para reconstruir el banco otra vez debes agregar de nuevo las referencias. **Quitar ultima** modifica la lista; **Vaciar banco** borra la lista y el banco aprendido. Cambiar a un preset de fabrica desactiva el banco. El aprendizaje usa analisis de parciales, no reentrena una red neuronal con tus grabaciones. En calidad Profunda se analiza a 44.1 kHz; en Rapida/Normal, a 22.05 kHz.

## NovaSeed Multi 1: alcance del nuevo entrenamiento

Ademas de NovaSeed 1 para FM, esta version incluye una red de prioridad de familias (155 > 48 > 4) y una red de propuestas condicionada por motor (159 > 80 > 48 > 30). Se entrenaron con **8.192 sonidos originales** generados por FM, wavetable, cuerda y soplo: 7.021 para entrenamiento y 1.171 reservados como prueba. La busqueda espectral se analiza directamente y tambien compite con estas propuestas.

En el corpus sintetico reservado, la clasificacion de familia de motor obtuvo aproximadamente **86.08 %** y el error cuadratico de parametros fue 0.01027. **No es un porcentaje de parecido, ni precision para identificar instrumentos reales.** El regresor alcanzo el limite de 180 iteraciones sin declarar convergencia; el reporte lo documenta. Las propuestas se renderizan y comparan: no se aceptan por la etiqueta del clasificador. Desactivar IA omite ambas redes.

No se dispone en este proyecto de un corpus de instrumentos reales con grabaciones, permisos y objetivos de entrenamiento verificados. Por ello no se afirma haber completado ese entrenamiento ni un modelo DDSP entrenado con instrumentos reales. Los cinco motores, la busqueda por etapas y el banco de referencias si estan implementados. El modelo de cuerda y el de soplo son aproximaciones sencillas; no sustituyen una biblioteca profesional de instrumentos reales.

Los generadores, scripts, pesos, informes y pruebas reservadas se incluyen en Fuentes.zip, carpeta Training. El corpus bruto puede regenerarse con GenerateMultiDataset.cpp y TrainMultiModel.py; entrenar otra vez requiere numpy y scikit-learn, pero usar el plugin no requiere instalar nada adicional.

## Tocar con las teclas del ordenador

Activa **Teclado PC** y haz clic en el fondo de Nova Synth para darle el foco.

- **A W S E D F T G Y H U J K:** trece notas cromaticas, de Do a Do.
- **Z / X:** bajar o subir una octava. Tambien hay botones Oct - y Oct +.
- **Esc / PANIC:** cortar las voces y vaciar las colas de efectos.
- El teclado dibujado tambien se toca con el raton. El piano roll y los dispositivos MIDI del host siguen funcionando.

La base inicial es MIDI 48. La octava se conserva en el preset/proyecto. Las notas del teclado PC usan el canal 16 para evitar cortar notas del piano roll en canal 1. Al perder el foco, escribir en un campo numerico o cerrar la ventana se liberan las notas del teclado PC. No se hace captura global de teclas fuera del plugin.

Si FL Studio consume las teclas antes de enviarlas al plugin, activa **Typing Keyboard Priority** en el wrapper de Nova Synth. Para evitar que FL Studio toque tambien su propio mapa, desactiva su funcion Typing keyboard to piano mientras usas el teclado interno. Referencia: https://www.image-line.com/fl-studio-learning/fl-studio-online-manual/html/plugins/wrapper.htm

Las notas del teclado interno sirven para audicion; no se envian como MIDI de salida ni se graban automaticamente en el piano roll. Para grabar notas usa la entrada MIDI/teclado de FL Studio.

## Ventana y temas

Arrastra la esquina inferior derecha para cambiar el tamano. Se escala todo el panel manteniendo su proporcion, entre 784 x 560 y 1792 x 1280. El tamano inicial es 1008 x 720. El tamano se conserva al guardar el proyecto o preset; abrir un preset con el editor ya visible aplicara su tamano al volver a abrir la ventana.

El fondo oscuro admite siete temas con brillo neon: **Azul, Amarillo, Verde, Morado, Rosado, Blanco y Naranja**. Los halos aparecen en perillas, graficas, seleccion de la onda y notas del arpegiador. Paneles, menus y teclas combinan un fondo interno difuminado con transparencias, reflejos y bordes para un acabado inspirado en el vidrio de iOS. El acabado se dibuja dentro del plugin: no hace transparente la ventana de FL Studio ni necesita GPU especial. Doble clic en un mando restablece su valor inicial. Los campos numericos permiten introducir valores sin disparar notas del teclado PC.

## Afinacion de nuevas reconstrucciones

AutoFit mide el tono en varias ventanas y refina la frecuencia sobre los samples a su frecuencia original. La mediana reduce la influencia de ventanas atipicas. La referencia que evalua el buscador se centra antes en la nota temperada (La 440 Hz), para no favorecer candidatos que reproduzcan su desafinacion original.

En **MUESTRA**, **Tono limpio** viene activado. Cuando la estructura es claramente armonica, acerca cada parcial a un multiplo exacto de la fundamental y combina duplicados cercanos para reducir batimientos. No fuerza este tratamiento en campanas u otros sonidos que parecen inarmonicos. Desactivarlo conserva las desviaciones entre parciales, pero la correccion global de la nota sigue activa. La opcion se guarda en el preset y afecta a la proxima reconstruccion.

Tune, detune, unison, chorus y seguimiento tonal siguen empezando sin desafinacion deliberada. La primera nota aislada de un arpegio se vuelve a medir antes de reconstruir el timbre. Notas ruidosas, acordes, vibrato fuerte, afinaciones ambiguas o fundamentales debiles pueden seguir produciendo errores; esto no garantiza afinacion ni semejanza perfectas en cualquier grabacion.

**Para actualizar un sonido reconstruido anteriormente:** carga otra vez su referencia, deja **Auto nota** y **Tono limpio** activados, selecciona una nota aislada y pulsa **Recrear seleccion**. Instalar esta version no cambia los sonidos guardados en tus proyectos. La limpieza puede cambiar el caracter de una fuente desafinada y no es una restauracion automatica de ruido, reverb o todos sus defectos.

## Escuchar el fragmento seleccionado

En **MUESTRA**, pulsa **Escuchar** o **Espacio** para reproducir desde el inicio hasta el final de la zona marcada. Pulsa otra vez para detener. Un cursor blanco indica la posicion. La lectura se hace en un hilo separado y se conserva el tono original del archivo; la opcion de igualar volumen del LAB tambien se aplica a esta escucha, con conversion de frecuencia de muestreo al host y rampas de 3 ms en los bordes para reducir clics.

Cambiar el recorte, cargar otro archivo, iniciar la recreacion, salir de MUESTRA, cerrar el editor o pulsar PANIC detiene la escucha. Espacio solo se captura con el foco en el plugin, en la pestana MUESTRA y fuera de campos de texto; no es un atajo global. Si FL Studio lo consume antes, activa Typing Keyboard Priority en el wrapper o usa el boton. No controla el transporte del proyecto. Escuchar reproduce el archivo original solo como referencia: las notas del teclado y MIDI siguen usando el motor sintetizado.

## IA local: alcance exacto

Ademas del modelo Multi descrito arriba, el interruptor **IA local** activa NovaSeed 1, una red neuronal 155 > 96 > 64 > 30 que estima treinta parametros normalizados para el motor FM/subtractivo. Corre en CPU dentro del hilo de analisis, sin conexion y sin servicios externos.

Se genero un corpus original de 6.144 sonidos procedurales de Nova Synth, variando presets, controles, notas y duraciones. Se reservaron 1.024 ejemplos para evaluacion independiente del entrenamiento. **No se entreno con grabaciones de instrumentos reales.** El informe de entrenamiento y los scripts reproducibles estan en `Fuentes.zip/Training`.

La red propone cuatro configuraciones iniciales. El optimizador las escucha mediante el propio motor y las compara junto al patch actual, los presets y otras configuraciones. Una propuesta neuronal solo se conserva si mejora la distancia encontrada. Desactivar IA omite esas propuestas; la resintesis y la busqueda convencional siguen disponibles.

Este primer modelo no es un reconocedor neuronal de guitarras, pianos o violines. La etiqueta de tipo de sonido sigue siendo una estimacion heuristica por sus caracteristicas. Tampoco se ha demostrado una mejora perceptual sobre instrumentos reales: hacen falta muestras representativas y pruebas de escucha para evaluarla. La red no controla por si sola todo el motor de resintesis; el analizador espectral estima las trayectorias y el ruido.

## Arrastrar una referencia y reconstruir una nota corta

Carga o arrastra un archivo WAV, AIFF, FLAC u OGG de 8 a 96 kHz y de 0.05 segundos a 60 minutos. La lectura de la forma de onda completa se hace en segundo plano y puede cancelarse. Ya no se limita a los primeros 15 segundos. En modo nota aislada admite **entre 0.05 y 3 segundos por reconstruccion**. Con Contexto activo admite hasta **60 segundos para el analisis**, de los que extrae ejemplos cortos.

1. Abre **MUESTRA**; se abre tambien al cargar o arrastrar un archivo. Desactiva Contexto si quieres limitar la seleccion a una nota de hasta 3 segundos.
2. Marca el fragmento sobre la onda. Arrastrar fuera de la seleccion crea otra; arrastrar dentro la mueve. Los dos bordes permiten ajustar inicio y final. Shift tambien permite moverla.
3. Para precisar, escribe **Inicio s** y **Fin s**, en segundos, y pulsa Enter. Ejemplo: inicio `30.125` y fin `30.625`.
4. Usa **Lupa + / Lupa -** o la rueda del raton para ampliar/reducir, **Ver todo**, **Ver seleccion** y la barra horizontal para recorrer el archivo. Doble clic dentro de la seleccion la amplia.
5. Escucha el recorte con **Escuchar** o **Espacio**; vuelve a pulsar para detener.
6. Pulsa **Recrear seleccion**. La carga del archivo ya no inicia el ajuste automaticamente: primero permite elegir la zona.

Los tiempos se convierten al frame mas cercano del archivo fuente. El ajuste conserva la duracion y el silencio inicial del fragmento, sin buscar otra nota fuera de la zona marcada. Evita seleccionar acordes, varias notas distintas o mezclas si quieres reconstruir un timbre aislado. Los fragmentos muy breves pueden no permitir una deteccion de afinacion estable.

**Auto nota** estima la nota raiz a partir del fragmento seleccionado. Para indicar una nota manual, desactivalo y ajusta **Reference Note** en SYNTH antes de recrear. Si el archivo cambia o se elimina despues de cargarlo, se pedira volver a cargarlo.

La onda es un resumen de minimos/maximos por bloques, con varios niveles de detalle para poder recorrer archivos largos. La vista no reproduce el audio. Solo se decodifica el fragmento marcado para la reconstruccion; la exploracion inicial lee el archivo por bloques para preparar la onda sin mantener todo su PCM en memoria.

AutoFit compara:

1. **FM / Subtractive:** dos osciladores procedurales, modulacion de fase, sub, ruido, unison, ADSR, filtro, LFO y efectos. Las propuestas de IA ayudan a iniciar esta busqueda.
2. **OSC + ruido / tracking:** hasta 48 osciladores sinusoidales independientes, 64 puntos temporales de amplitud y frecuencia por parcial y 12 bandas de ruido sintetizado con envolventes temporales.
3. **Banco fijo:** una alternativa sin movimiento tonal ni ruido residual. Se conserva cuando se ajusta mejor que las opciones nuevas.

La comparacion usa espectro, evolucion de energia, anchura y ahora tambien el ataque en ventanas de 4 ms. Se selecciona la configuracion con menor distancia encontrada, no una supuesta identificacion segura del instrumento original.

Los niveles son Rapida (160 intentos, 22.05 kHz), Normal (480, 22.05 kHz) y Profunda (1200, 44.1 kHz). Profunda puede tardar mas y analiza informacion hasta aproximadamente 20 kHz. Puedes tocar el patch anterior durante la busqueda. **Cancelar conserva el patch anterior.**

El audio de referencia solo se analiza. El motor no reproduce PCM, no transpone el archivo y no usa una tabla de onda importada. Genera audio mediante osciladores y filtros de ruido. El preset funciona sin tener la referencia en disco.

## Controles nuevos del banco

- **Partial Time:** estira las curvas temporales del banco.
- **Partial Tone:** modifica el balance entre parciales graves y agudos.
- **Residual / Air:** nivel del ruido por bandas. Cero lo silencia.
- **Pitch Movement:** intensidad de las trayectorias de frecuencia. Cero congela cada frecuencia base. No afecta a los modelos fijos o antiguos.
- **PARCIALES:** arrastra barras para ajustar las 48 ganancias entre 0 y 2. Doble clic restablece 1. La curva inferior muestra la amplitud del parcial seleccionado.

Los mandos exclusivos del motor FM y Stereo Width se desactivan al usar el banco; los efectos de chorus, delay y reverb permiten anadir espacialidad al banco, cuya fuente es mono.

## Efectos y visualizaciones

Cadena propia: **Drive > Chorus > EQ > Delay > Reverb**, cada efecto con interruptor y mandos. Delay usa segundos y no se sincroniza al tempo.

El selector grafico ofrece Onda, Espectro, Envolvente, En vivo, Movimiento tonal y Ruido/bandas. Referencia gris y resultado con el color del tema. Onda muestra los primeros 30 ms, con normalizacion independiente. Espectro muestra magnitudes por bandas normalizadas por RMS. En vivo refleja la salida real. Movimiento tonal muestra la trayectoria del parcial de mayor amplitud, en cents; Ruido muestra la energia relativa por bandas.

Las comparaciones y los datos analizados son una fotografia del ultimo ajuste; los cambios manuales se escuchan y se ven en En vivo. Ejecuta otra busqueda para actualizar las comparaciones. Movimiento tonal y Ruido no son analizadores en vivo ni una medida de parecido.

## Banco de sonidos

La pestana **BANCO** permite filtrar por categoria y buscar por nombre o numero. El filtro inicial muestra los 700 posteriores a los 64 originales; **Todos / 764** incluye tambien los originales. Seleccionar una fila carga el sonido; toca el teclado PC, MIDI o piano roll para escucharlo. El desplegable superior incluye los 764 programas y los botones < y > permiten pasar al anterior o al siguiente.

Hay 25 patches originales en cada familia:

- Trance Leads y Festival Leads.
- Dance Plucks y Deep Basses.
- House Keys y Retro Keys.
- Cinematic Pads y Atmospheres.
- Synth Strings y Synth Brass.
- Bells & Mallets y FX & Motion.

Son configuraciones procedurales de osciladores, FM, filtros, envolventes, modulacion y efectos. Comparten las familias habituales de bancos de produccion electronica. No son expansiones de Nexus, no necesitan Nexus y no contienen sus muestras ni datos comerciales. Las categorias Strings, Brass y Keys son interpretaciones sinteticas; no se presenta este motor como una biblioteca acustica multisampleada.

Los 300 producen audio distinto en las pruebas realizadas. Esto no sustituye una evaluacion de gusto o calidad musical. Los 64 programas originales mantienen su numeracion para proyectos anteriores. El banco nuevo no modifica ni reentrena la IA NovaSeed; AutoFit conserva sus semillas originales y puede partir del patch actual que hayas elegido.

### Cuatro bancos adicionales

En BANCO puedes filtrar **Halloween**, **Synthwave 80s**, **Trap** o **Hip Hop 2K**. Cada uno incluye 100 configuraciones procedurales originales. Son variaciones sinteticas de la arquitectura de Nova Synth, no bibliotecas de grabaciones acusticas. Sus salidas son distintas en la prueba automatizada; algunas variaciones pueden sonar cercanas entre si.

Usa los botones **<** y **>** junto al selector para cambiar de preset. El deslizador **GAIN** aumenta o reduce la salida entre -12 y +12 dB y conserva su valor al cambiar de preset. Si el indicador se ilumina cerca del limite, reduce la ganancia para evitar distorsion por recorte.

## Arpegios editables

1. Carga el archivo, marca hasta 3 segundos en MUESTRA y pulsa **Recrear seleccion**.
2. Si se detecta una secuencia monofonica de distintas alturas, se propone un patron de 16 pasos y BPM aproximado. Se usa la primera nota detectada como referencia para ajustar el timbre; el resto forma el patron.
3. Pulsa **ARP EDITAR** para abrir la cuadricula. Un clic crea o mueve la nota de un paso; otro clic en la misma posicion lo convierte en silencio. **Limpiar** borra el patron.
4. Activa **Activar arpegio** y manten una tecla del piano, del teclado PC o MIDI. El patron se transpone a la tecla pulsada. Ajusta BPM y cierra el panel cuando termines. El patron, BPM y estado se guardan en el preset/proyecto.

La rejilla divide uniformemente el fragmento en 16 pasos; las filas representan hasta dos octavas por encima y por debajo de la primera nota. El BPM supone semicorcheas y se reproduce con reloj interno, sin sincronizacion automatica al tempo del host. Es monofonico: interpreta la ultima tecla pulsada. Una seleccion de menos de 0.4 s, una nota sola, sonidos con mucho ruido, notas simultaneas, acordes o una mezcla completa pueden no detectarse o producir notas incorrectas. Si no hay una deteccion segura, conserva el patron anterior y puedes editarlo manualmente.

Esta deteccion utiliza analisis de tono, no una nueva red neuronal entrenada con instrumentos reales. La IA NovaSeed existente sigue proponiendo configuraciones del sintetizador, con los mismos limites indicados arriba. No se garantiza que el arpegio ni el instrumento se reproduzcan de forma exacta.

## Guardado y compatibilidad

Se conservan los 364 presets anteriores en sus posiciones originales y se anaden 400 (764 en total). **Guardar** exporta `.novapreset` con controles, modelo de osciladores/ruido, tema, tamano, octava, arpegio y estado de IA. No incluye el audio de referencia ni los graficos de comparacion. El archivo de referencia y su seleccion permanecen solo en la sesion; para repetir el analisis despues de reabrir el proyecto vuelve a cargar el archivo y marca el fragmento. El sonido reconstruido guardado funciona sin ese archivo.

Se admite el modelo de parciales v0.3: sus 20 puntos se interpolan a 64, con frecuencias fijas y ruido cero. El nuevo formato valida longitudes, valores finitos y limites antes de cargar. Los presets v0.1 de sampler siguen sin reproducir grabaciones. El nuevo motor y la busqueda pueden cambiar timbres y resultados; conserva tus proyectos anteriores si necesitas comparar versiones.

## Limites y validacion

Una nota grabada no describe todas las notas, velocidades, articulaciones ni resonancias de un instrumento. El seguimiento es limitado, el banco tiene 48 parciales, la fuente del banco es mono y el ruido se vuelve a generar aleatoriamente. No hay un motor fisico especializado completo ni aprendizaje con varias muestras por instrumento en esta version. El ruido de ataque se aproxima mediante envolventes y filtros, no mediante un modelo neuronal independiente de transitorios.

La distancia interna no es un porcentaje de exactitud. No se garantiza 99,99 %, igualdad de forma de onda ni copia indistinguible de instrumentos reales. Consulta VALIDACION.txt para resultados y pruebas concretas.

Se compilo para Windows x64 y se probaron en C++ nativo los motores, red neuronal, ajuste, teclado como maquina de estados y serializacion del banco. **La interfaz, la seleccion mediante raton dentro del host, las teclas, el guardado de proyecto y el instalador todavia no se han ejecutado directamente en Windows/FL Studio desde este entorno.** El instalador no tiene certificado de firma digital.

## Fuentes, licencia y reconstruccion

GPL v3 o posterior. Fuentes completas, pesos neuronales originales, scripts de entrenamiento, informe y datos de prueba incluidos en `Fuentes.zip`, junto con JUCE 7.0.12 y el SDK VST3 incorporado. No hay una dependencia de Python ni scikit-learn para ejecutar el plugin.

Reconstruccion con LLVM-MinGW 20250114 UCRT y Python 3:

```bash
python3 Packaging/build_windows_cross.py --toolchain /ruta/llvm-mingw --juce ThirdParty/JUCE
python3 Packaging/package_installer.py --toolchain /ruta/llvm-mingw --juce ThirdParty/JUCE
```

Alternativa con Visual Studio 2022 y CMake:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DJUCE_DIR=ThirdParty/JUCE
cmake --build build --config Release --target NovaSynth_VST3 core_tests spectral_tests resynth_tests selection_tests bank_tests reference_tests arp_tests tuning_tests
ctest --test-dir build -C Release --output-on-failure
```

Pruebas sin JUCE:

```bash
g++ -std=c++17 -O2 Tests/CoreTests.cpp -I Source -o core_tests
./core_tests
g++ -std=c++17 -O2 Tests/SpectralBench.cpp -I Source -o spectral_tests
./spectral_tests
g++ -std=c++17 -O2 Tests/ResynthTests.cpp -I Source -o resynth_tests
./resynth_tests Training/Holdout.bin
g++ -std=c++17 -O2 Tests/SelectionTests.cpp -I Source -o selection_tests
./selection_tests
g++ -std=c++17 -O2 Tests/BankTests.cpp -I Source -o bank_tests
./bank_tests
```

Para regenerar el modelo (solo desarrolladores), usa Python 3, numpy 2.3.5 y scikit-learn 1.8.0:

```bash
g++ -std=c++17 -O3 Training/GenerateDataset.cpp -I Source -o generate_dataset
./generate_dataset dataset.bin
OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1 python3 Training/TrainModel.py dataset.bin
```

Estas herramientas solo son necesarias para modificar el proyecto. El usuario instala unicamente el `.exe`.
