# Estado de la migración a GitHub
Repositorio privado: wiccan2006-ux/NovaSynth.

## Respaldo completo
- NovaSynth.bundle conserva las fuentes completas, JUCE 7.0.12, documentación de continuidad, rama main e historial inicial.
- Commit original: b2b621030c19440b34276454164b1b28a7677d4a.
- Tag original: v0.17-snapshot.
- SHA256 bundle: bf8504060f5fa7c62709b1bd1b3751c3c24204da36d3e34241f4d1f8a56e90dc.
- NovaSynth_0.17_Codigo_Fuente_Completo.zip conserva las fuentes publicadas del instalador (sin los documentos posteriores de continuidad).
- SHA256 ZIP: 0163636d1ac52b8518ec4cffa0055c0377927f3b1db6c430fef68f026fd40f25.

## Continuar ahora con Git
Descarga NovaSynth.bundle y ejecuta:

```sh
git clone NovaSynth.bundle NovaSynth
cd NovaSynth
git remote set-url origin https://github.com/wiccan2006-ux/NovaSynth.git
```

Lee README.md, AGENTS.md y docs/CONTINUAR.md dentro del proyecto restaurado.
No hagas push forzado: la rama remota tiene los commits de preparación de la migración.

## Importación pendiente
Las carpetas Source, Tests, Training, Packaging, ThirdParty y los documentos completos todavía están dentro del respaldo; falta desplegarlas en la rama remota y conectar su historial.
La propuesta legible está en docs/importacion-propuesta.yml.txt; es texto inerte y NO se ejecuta.
Propone una ejecución manual con permiso contents: write para importar el respaldo verificado, conservar su commit como padre de una fusión y publicar su etiqueta.
No agrega colaboradores ni cambia la privacidad.
La revisión automática bloqueó habilitar ese permiso sin autorización específica del usuario.
Las pruebas configuradas requieren completar la importación antes de poder validar las fuentes en GitHub.
