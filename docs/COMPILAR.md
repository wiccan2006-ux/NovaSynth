# Compilar y comprobar

## Herramientas y dependencia

- C++17; CMake >=3.22 para la ruta CMake.
- JUCE 7.0.12 esta incluido completo en ThirdParty/JUCE.
- Para Windows nativo: Visual Studio con C++ y Windows SDK.
- Para la ruta cruzada usada en la entrega: Linux, Python 3 y LLVM-MinGW
  `llvm-mingw-20250114-ucrt-ubuntu-20.04-x86_64` descargado de
  https://github.com/mstorsjo/llvm-mingw/releases/tag/20250114 .
- El compilador, CMake, Python, Windows SDK y el DAW no se incluyen en este ZIP.

## Pruebas de nucleo sin dependencia GUI

```sh
cmake -S . -B build-core -DBUILD_PLUGIN=OFF
cmake --build build-core --parallel 2
ctest --test-dir build-core --output-on-failure
```

Es posible compilar una prueba aislada con g++:

```sh
g++ -std=c++17 -O2 -I Source Tests/ProductionTests.cpp -o production_tests
./production_tests
```

Los tests resynth/instrument necesitan sus holdouts incluidos en Training;
CMake les pasa la ruta. Algunas pruebas son intensivas y pueden tardar.

## Plugin en Windows con Visual Studio/CMake

Desde Developer PowerShell, en la raiz de este repositorio:

```powershell
cmake -S . -B build -A x64 -DJUCE_DIR="$PWD/ThirdParty/JUCE"
cmake --build build --config Release --target NovaSynth_VST3 --parallel 2
```

Busca el bundle en build/NovaSynth_artefacts/Release/VST3. Esta es la ruta prevista
por CMake/JUCE; la entrega previa se construyo por la ruta cruzada siguiente,
no se ha validado este comando nativo en una maquina Windows en esta sesion.

## Ruta cruzada utilizada para el instalador

Ejemplo Linux; reemplaza /ruta/llvm-mingw por tu herramienta descomprimida:

```sh
python3 Packaging/build_windows_cross.py --toolchain /ruta/llvm-mingw --juce ThirdParty/JUCE --jobs 2
python3 Packaging/package_installer.py --toolchain /ruta/llvm-mingw --juce ThirdParty/JUCE
```

Resultado VST3:
`build-windows/Nova Synth.vst3/Contents/x86_64-win/Nova Synth.vst3`.
Por defecto el instalador se escribe en la carpeta padre del repositorio.
Usa --output en package_installer.py si quieres otro destino.

Haz una compilacion completa al cambiar version/configuracion. --plugin-only
solo reutiliza objetos JUCE de una compilacion completa con la misma configuracion.
El empaquetado incorpora fuentes completas, JUCE y licencias.

## Interfaz nativa Linux

Packaging/build_native_tests.py necesita un prefijo de bibliotecas de desarrollo
X11/freetype/fontconfig y una pantalla/Xvfb. Lee su --help. Una prueba nativa de
componentes no equivale a ejecutar VST3 o el instalador dentro de FL Studio.

## Entrenamiento y catalogo

Los pesos de inferencia estan incluidos en Source. Los datasets completos se
regeneran con Training/Generate*.cpp; los scripts Train*.py conservan el proceso.
El catalogo usa AuditCatalog.cpp y BuildCatalog.py (Python + numpy). No regeneres
pesos o presets como efecto secundario de arreglar la interfaz.
