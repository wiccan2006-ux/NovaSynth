# Publicar la carpeta maestra en GitHub

La entrega ya es un repositorio Git local, rama main, con un commit y el tag
v0.17-snapshot. **Todavia no hay repositorio remoto ni URL real de GitHub.**
Nombre propuesto: NovaSynth. Visibilidad inicial propuesta: privado.

## Opcion sencilla: GitHub Desktop

1. Descomprime el paquete completo en tu PC.
2. Instala GitHub Desktop desde https://desktop.github.com/ e inicia sesion.
3. File > Add local repository: elige la subcarpeta **NovaSynth**, donde estan
   README.md y la carpeta .git. No elijas solo Distribuciones ni el ZIP.
4. Pulsa Publish repository. Nombre: NovaSynth.
5. Conserva Keep this code private y publica en tu cuenta personal.
6. Guarda la URL que GitHub te entregue; esa sera la ubicacion permanente del codigo.
7. Desde GitHub comparte acceso al repositorio con tu programador cuando lo decidas.
8. Crea una Release basada en el tag v0.17-snapshot y adjunta los binarios/fuentes
   publicados de Distribuciones/0.17. El codigo ya estara en el repositorio.

Referencia oficial:
https://docs.github.com/en/desktop/adding-and-cloning-repositories/adding-an-existing-project-to-github-using-github-desktop

## Alternativa para un programador

Crea un repositorio remoto vacio en tu cuenta, sin README ni licencia nuevos.
Los comandos siguientes usan TU_USUARIO como marcador; no son una URL ya creada:

```sh
git remote add origin https://github.com/TU_USUARIO/NovaSynth.git
git push -u origin main
git push origin v0.17-snapshot
```

## Recuperar el historial desde el bundle

Si se pierde .git o una herramienta omite carpetas ocultas, desde la carpeta
que contiene NovaSynth.bundle ejecuta:

```sh
git clone NovaSynth.bundle NovaSynth_recuperado
```

Ese clon incluye todos los archivos versionados, incluida ThirdParty/JUCE.
El bundle no contiene los instaladores, que van aparte en Distribuciones.

## Reglas para siguientes versiones

- main conserva el estado estable; cada cambio se trabaja en una rama corta.
- Guarda commits con descripcion concreta del cambio y su validacion.
- Etiqueta releases y adjunta instalador y fuentes de esa compilacion.
- No sobrescribas una distribucion vieja con binarios diferentes usando el mismo nombre.
- Conserva dos copias: GitHub y una copia local del repositorio/artefactos.
- Configura tu nombre/correo Git para commits futuros. La identidad inicial del
  respaldo es una identidad tecnica local y no autentica ninguna cuenta GitHub.

La automatizacion de pruebas en .github/workflows queda preparada. Solo sabremos
si pasa en GitHub cuando el repositorio se publique y Actions la ejecute.
