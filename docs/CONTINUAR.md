# Entrega a otra IA o a un programador

## Mensaje que puedes copiar

> Este es Nova Synth 0.22 beta, VST3 C++/JUCE para Windows x64 y FL Studio.
> Lee README.md, AGENTS.md, docs/ESTADO.md y docs/ARQUITECTURA.md.
> Trabaja a partir de estos archivos y del historial Git; no dependas de mi chat
> anterior. Conserva los estados/presets, IDs del plugin y los creditos de
> Eleander Sanchez (Bodby Olwen). Explica que cambiaste, como lo verificaste
> y que falta probar dentro de FL Studio en Windows.

## Lo que quiere el usuario

- Recrear el timbre de una referencia con sintesis y controles editables.
- Evitar que la funcion se convierta simplemente en un sampler con cambio de pitch.
- Mejor afinacion, referencia seleccionable/zoom/audicion con Espacio, analisis
  de contexto largo, referencias multiples y arpegio editable/exportable.
- Sonidos variados, utiles y con volumen equilibrado; navegador agil.
- Interfaz negra, Liquid Glass inspirado en iOS, neon regulable y siete colores.
- Ventana redimensionable, teclado PC/MIDI y instalador Windows sencillo.
- El audio real problematico del usuario aun no se entrego; se dejo para despues.

## Como continuar

1. Lee el estado real y las limitaciones; escucha ejemplos antes de prometer calidad.
2. Crea una rama `feature/nombre-del-cambio` desde main.
3. Implementa y ejecuta pruebas apropiadas al cambio.
4. Comprueba GUI y FL Studio en Windows si el cambio afecta foco, teclado, DPI,
   instalacion, automatizacion, rendimiento o integracion con el host.
5. Actualiza guia, validacion y version cuando corresponda.
6. Guarda un commit, crea una version etiquetada y conserva instalador y fuentes.

## Lo que hay y lo que no hay en el respaldo

Hay codigo completo de 0.22, JUCE, pesos de las redes, scripts, holdouts y reportes.
No se conserva el conjunto masivo de entrenamiento generado de cada experimento;
se regenera con los generadores incluidos. No hay grabaciones del usuario ni
biblioteca de muestras acusticas externa. No hay cuentas, claves o credenciales.
Los compiladores y SDK de desarrollo se instalan aparte; son herramientas externas.

El archivo LEEME.md conserva historial de funciones anteriores. Eso no representa
una cadena Git retroactiva: solo hay un commit inicial de esta instantanea.
