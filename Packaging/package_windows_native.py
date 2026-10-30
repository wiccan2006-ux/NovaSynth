#!/usr/bin/env python3
"""Package the MSVC-built VST3 and full corresponding GPL sources on Windows."""
from pathlib import Path
import hashlib,subprocess,zipfile,shutil
root=Path(__file__).resolve().parents[1]
build=root/'build-windows';dist=root/'dist';dist.mkdir(exist_ok=True)
plugins=[p for p in build.rglob('Nova Synth.vst3') if p.is_file()]
assert len(plugins)==1,plugins
plugin=plugins[0]
notices=root/'LICENCIAS_TERCEROS.txt'
notices.write_text('Nova Synth 0.21.0\nJUCE 7.0.12 / VST3 SDK: GPL v3 option\n\n'+(root/'ThirdParty/JUCE/LICENSE.md').read_text(encoding='utf-8'),encoding='utf-8')
source=dist/'NovaSynth_0.21_Codigo_Fuente_Completo.zip'
with zipfile.ZipFile(source,'w',zipfile.ZIP_DEFLATED,compresslevel=6) as z:
 paths=[]
 for folder in ['Source','Tests','Training','Packaging','docs','.github','ThirdParty']:
  paths.extend((root/folder).rglob('*'))
 paths+=list(root.glob('*.md'))+list(root.glob('*.txt'))+[root/'CMakeLists.txt',root/'.gitignore',root/'.gitattributes']
 for p in sorted(set(paths)):
  if p.is_file() and not any(part in {'.git','__pycache__'} for part in p.relative_to(root).parts):z.write(p,Path('NovaSynth')/p.relative_to(root))
shutil.copyfile(root/'LEEME.md',build/'LEEME.txt')
def q(p):return '"'+p.resolve().as_posix()+'"'
rc=build/'setup-native.rc'
rc.write_text('#include <windows.h>\n1 RT_MANIFEST '+q(root/'Packaging/setup.manifest')+'\n'+''.join(f'{i} RCDATA {q(p)}\n' for i,p in [(101,plugin),(102,build/'LEEME.txt'),(103,root/'LICENSE.txt'),(104,source),(105,notices)])+'''1 VERSIONINFO
 FILEVERSION 0,21,0,0
 PRODUCTVERSION 0,21,0,0
 FILEFLAGSMASK 0x3fL
 FILEOS VOS_NT_WINDOWS32
 FILETYPE VFT_APP
BEGIN
 BLOCK "StringFileInfo"
 BEGIN
  BLOCK "040904B0"
  BEGIN
   VALUE "CompanyName", "Eleander Sanchez (Bodby Olwen)\\0"
   VALUE "FileDescription", "Nova Synth VST3 installer\\0"
   VALUE "FileVersion", "0.21.0\\0"
   VALUE "ProductName", "Nova Synth\\0"
   VALUE "ProductVersion", "0.21.0\\0"
  END
 END
 BLOCK "VarFileInfo"
 BEGIN
  VALUE "Translation", 0x0409, 1200
 END
END
''',encoding='utf-8')
resource=build/'setup-native.res';subprocess.run(['rc','/nologo','/fo',str(resource),str(rc)],check=True)
exe=dist/'NovaSynth_0.21_Instalar_Windows_x64.exe'
subprocess.run(['cl','/nologo','/O2','/MT','/utf-8','/DUNICODE','/D_UNICODE',str(root/'Packaging/Setup.c'),str(resource),'/Fe'+str(exe),'/Fo'+str(build/'setup-native.obj'),'/link','/SUBSYSTEM:WINDOWS','/DYNAMICBASE','/NXCOMPAT','user32.lib','shell32.lib','shlwapi.lib','ole32.lib','uuid.lib','advapi32.lib'],check=True)
with zipfile.ZipFile(dist/'NovaSynth_0.21_Instalador.zip','w',zipfile.ZIP_DEFLATED) as z:z.write(exe,exe.name)
with zipfile.ZipFile(dist/'NovaSynth_0.21_VST3_Portable.zip','w',zipfile.ZIP_DEFLATED) as z:
 bundle=plugin.parents[2]
 for p in bundle.rglob('*'):
  if p.is_file():z.write(p,Path('Nova Synth.vst3')/p.relative_to(bundle))
files=[p for p in dist.iterdir() if p.is_file() and p.name!='SHA256SUMS.txt']
(dist/'SHA256SUMS.txt').write_text(''.join(hashlib.sha256(p.read_bytes()).hexdigest()+'  '+p.name+'\n' for p in sorted(files)),encoding='utf-8')
print('Packaged',[(p.name,p.stat().st_size) for p in dist.iterdir()])
