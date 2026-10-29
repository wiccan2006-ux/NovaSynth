# Acceder al proyecto en GitHub

Repositorio privado: https://github.com/wiccan2006-ux/NovaSynth
Cuenta propietaria: wiccan2006-ux. Rama principal: main.

## Descargar o continuar

- Para programar con historial: inicia sesion en GitHub Desktop, File > Clone repository,
  selecciona wiccan2006-ux/NovaSynth y elige una carpeta en tu PC.
- Para otra IA con acceso a GitHub: conecta esta cuenta y selecciona NovaSynth.
- Para descargar las carpetas actuales: Code > Download ZIP. Este ZIP no incluye .git;
  usa un clon para conservar el historial.
- Para instalador y respaldo completo: Releases > v0.17-snapshot.
- Para entregar a un programador: dale acceso desde Settings > Collaborators cuando
  tu lo decidas. Un repositorio privado no se abre solo por compartir su enlace.

## Clonar desde terminal

```sh
git clone https://github.com/wiccan2006-ux/NovaSynth.git
cd NovaSynth
git switch -c feature/nombre-del-cambio
```

GitHub pedira autenticacion para este repositorio privado. No guardes claves ni tokens
en los archivos del proyecto. Lee README.md, AGENTS.md y docs/CONTINUAR.md.

## Historial conservado

El commit b2b621030c19440b34276454164b1b28a7677d4a importa las fuentes originales
0.17 y documentos de continuidad. La etiqueta anotada v0.17-snapshot conserva ese
punto. La fusion de importacion en main conserva ese commit como antecesor y tambien
los commits de preparacion remota. No se invento historial de versiones anteriores.

NovaSynth.bundle es el respaldo independiente de esa instantanea. Para restaurarlo:

```sh
git clone NovaSynth.bundle NovaSynth_recuperado
```

Ese clon tiene el historial inicial, no los commits posteriores de GitHub.
El paquete completo tambien contiene Distribuciones/0.17; los compiladores y SDK
se instalan aparte.

## Siguientes versiones

1. Clona o actualiza main y crea una rama para el cambio.
2. Guarda commits con descripcion y ejecuta las pruebas apropiadas.
3. Revisa el Pull Request y sus resultados de Actions antes de integrar.
4. Conserva los IDs de parametros/presets y actualiza todos los textos de version.
5. Crea una nueva etiqueta y Release con instalador y fuentes de esa compilacion.
6. Conserva otra copia local; no sustituyas distribuciones antiguas por otros binarios.

GitHub Actions comprueba tres pruebas de nucleo en Ubuntu; no valida por si solo
el instalador ni el VST3 dentro de Windows/FL Studio.
