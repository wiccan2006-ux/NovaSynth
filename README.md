# Nova Synth

Instrumento VST3 para Windows x64 orientado a FL Studio.
Desarrollador: **Eleander Sanchez (Bodby Olwen)**. Version publicada: **0.22 beta**.
Codigo bajo GPL-3.0-or-later; conserva `LICENSE.txt` y los avisos incluidos.

Este repositorio contiene las fuentes propias, pruebas, datos/modelos sinteticos,
 scripts y **JUCE 7.0.12 completo en `ThirdParty/JUCE`**. No usa submodulos ni Git LFS.
El codigo y los datos de la version publicada se preservan sin cambios en esta entrega.
Los archivos de organizacion/documentacion se anadieron para facilitar continuidad.

## Novedades 0.22

Busqueda exhaustiva local y servicio remoto CPU experimental. No hay servidor
publico ni una nueva red neuronal incluida. Lee [alcance](docs/ACTUALIZACION_0.22.md).

## Novedades 0.21

Favoritos directos, recientes por proyecto, filtros visibles y avisos completos.
Lee [el alcance](docs/ACTUALIZACION_0.21.md).

## Novedades 0.20

Tocar, Explorar y Recrear; edicion completa por pestanas y detalles desplegables.
Sin cambios de motor ni presets. Lee [la reorganizacion](docs/ACTUALIZACION_0.20.md).

## Novedades 0.19

Seleccion y nivel medidos en tres registros, expresividad accesible, informe tonal
completo/parcial, refinado con menos efectos y pruebas del contrato AudioProcessor.
Lee [alcance y limites](docs/ACTUALIZACION_0.19.md).

## Novedades 0.18

Vistas Simple/Avanzada/Tocar, preescucha antes de cargar, biblioteca personal,
favoritos globales, historial, variaciones bloqueables y recreacion automatica
comparando motores y notas. Lee [la guia](LEEME.md) y [el alcance real](docs/ACTUALIZACION_0.18.md).

## Empieza aqui

- [Continuar con otra IA o programador](docs/CONTINUAR.md)
- [Estado real y pendientes](docs/ESTADO.md)
- [Compilar y verificar](docs/COMPILAR.md)
- [Acceder al repositorio y continuar](docs/GITHUB.md)
- [Organizacion del codigo](docs/ARQUITECTURA.md)
- [Uso del plugin e historial funcional](LEEME.md)
- [Registro de validacion](VALIDACION.txt)

## Carpetas

| Ruta | Contenido |
| --- | --- |
| `Source/` | Interfaz JUCE, procesador, sintesis, efectos, analisis, presets y pesos de redes. |
| `Tests/` | Pruebas C++ de audio, modelos, seleccion, MIDI e interfaz nativa. |
| `Training/` | Generadores, scripts, holdouts, mediciones y reportes sinteticos. |
| `Packaging/` | Compilacion cruzada Windows, instalador C, empaquetado y pruebas nativas. |
| `ThirdParty/JUCE/` | Dependencia completa fijada en 7.0.12, con su licencia. |
| `docs/` | Instrucciones de desarrollo, continuidad y publicacion. |
| `.github/workflows/` | Pruebas basicas preparadas para ejecutarse al subir a GitHub. |

Repositorio privado: https://github.com/wiccan2006-ux/NovaSynth

Descarga el instalador, las fuentes publicadas y el paquete completo desde
**Releases > v0.22.0-beta**. El paquete completo conserva tambien la distribucion
y el historial inicial; los binarios no se guardan dentro de Source.
`NovaSynth.bundle` permite recuperar de forma independiente la instantanea inicial.

## Verificacion rapida sin JUCE ni interfaz

Con CMake >=3.22 y compilador C++17:

```sh
cmake -S . -B build-core -DBUILD_PLUGIN=OFF
cmake --build build-core --target production_tests sequence_tests preview_tests --parallel 2
ctest --test-dir build-core -R '^(production_tests|sequence_tests|preview_tests)$' --output-on-failure
```

GitHub Actions ejecuta ProductionTests, SequenceTests y PreviewTests en Ubuntu.
Consulta el resultado vigente en la pestaña Actions. Estas pruebas y VALIDACION.txt
no sustituyen abrir el plugin en FL Studio/Windows.

## Historial

El commit inicial importa la instantanea de 0.17 y agrega la documentacion de
continuidad. No se inventaron commits de las versiones anteriores. La rama
principal es `main`; el tag `v0.17-snapshot` identifica este punto de partida.
La importacion a GitHub conserva ese commit como antecesor de main y mantiene
la etiqueta original. Los commits posteriores documentan la migracion y siguientes
cambios. No hay credenciales dentro del proyecto.
