# Nova Synth

Instrumento VST3 para Windows x64 orientado a FL Studio.
Desarrollador: **Eleander Sanchez (Bodby Olwen)**. Version publicada: **0.17 beta**.
Codigo bajo GPL-3.0-or-later; conserva `LICENSE.txt` y los avisos incluidos.

Este repositorio contiene las fuentes propias, pruebas, datos/modelos sinteticos,
 scripts y **JUCE 7.0.12 completo en `ThirdParty/JUCE`**. No usa submodulos ni Git LFS.
El codigo y los datos de la version publicada se preservan sin cambios en esta entrega.
Los archivos de organizacion/documentacion se anadieron para facilitar continuidad.

## Empieza aqui

- [Continuar con otra IA o programador](docs/CONTINUAR.md)
- [Estado real y pendientes](docs/ESTADO.md)
- [Compilar y verificar](docs/COMPILAR.md)
- [Publicar en GitHub](docs/GITHUB.md)
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

Los binarios publicados se entregan fuera del repositorio, en la carpeta hermana
`Distribuciones/0.17`, para publicarlos como archivos de una Release de GitHub.
El respaldo de historial esta en la carpeta hermana `Respaldo_Git`.

## Verificacion rapida sin JUCE ni interfaz

Con CMake >=3.22 y compilador C++17:

```sh
cmake -S . -B build-core -DBUILD_PLUGIN=OFF
cmake --build build-core --target production_tests sequence_tests preview_tests --parallel 2
ctest --test-dir build-core -R '^(production_tests|sequence_tests|preview_tests)$' --output-on-failure
```

La configuracion de GitHub Actions queda preparada, pero **no se ha ejecutado en
GitHub** porque aun no hay repositorio remoto conectado. Las pruebas previas
registradas en VALIDACION.txt no sustituyen abrir el plugin en FL Studio/Windows.

## Historial

El commit inicial importa la instantanea de 0.17 y agrega la documentacion de
continuidad. No se inventaron commits de las versiones anteriores. La rama
principal es `main`; el tag `v0.17-snapshot` identifica este punto de partida.
No hay remote configurado ni credenciales dentro del paquete.
