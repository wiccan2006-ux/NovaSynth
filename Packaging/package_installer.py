#!/usr/bin/env python3
"""Embed the VST3, complete corresponding sources, GPL and third-party notices.
No network access, no changes to the host system. Requires an existing VST3 build.
"""
import argparse
from pathlib import Path
import shutil
import subprocess
import zipfile

p=argparse.ArgumentParser()
p.add_argument('--toolchain',type=Path,required=True)
p.add_argument('--juce',type=Path,required=True)
p.add_argument('--output',type=Path)
a=p.parse_args()
root=Path(__file__).resolve().parents[1]
build=root/'build-windows'
build.mkdir(exist_ok=True)
plugin=build/'Nova Synth.vst3/Contents/x86_64-win/Nova Synth.vst3'
assert plugin.is_file(),'Build the VST3 before packaging.'
notices=[]
license_files=[a.toolchain/'LICENSE.TXT',a.juce/'LICENSE.md']
license_files+=list((a.toolchain/'x86_64-w64-mingw32/share/mingw32').glob('COPYING*'))
for path in license_files:
    notices.append('\n\n===== '+path.name+' =====\n\n'+path.read_text(errors='replace'))
third_party=build/'LICENCIAS_TERCEROS.txt'
third_party.write_text('Nova Synth 0.19.0\nJUCE 7.0.12 / VST3 SDK (GPL v3 option)\nLLVM-MinGW runtime notices\n'+''.join(notices))
sources=build/'Fuentes.zip'
with zipfile.ZipFile(sources,'w',zipfile.ZIP_DEFLATED,compresslevel=6) as z:
    paths=[]
    for folder in ('Source','Tests','Training','Packaging','docs','.github'):
        paths.extend((root/folder).rglob('*'))
    paths+=list(root.glob('*.md'))+list(root.glob('*.txt'))+[root/'CMakeLists.txt',root/'.gitignore',root/'.gitattributes']
    for path in sorted(set(paths)):
        relative=path.relative_to(root)
        if any(part in {'__pycache__','.git'} for part in relative.parts): continue
        if path.is_file(): z.write(path,Path('NovaSynth')/relative)
    for path in sorted(a.juce.rglob('*')):
        relative=path.relative_to(a.juce)
        if '.git' in relative.parts: continue
        if path.is_file(): z.write(path,Path('NovaSynth/ThirdParty/JUCE')/relative)
    z.write(third_party,'NovaSynth/LICENCIAS_TERCEROS.txt')
guide=build/'LEEME.txt'
guide.write_text(root.joinpath('LEEME.md').read_text(),encoding='utf-8')
rc=build/'setup.rc'
def quote(path): return '"'+str(path.resolve()).replace('\\','/')+'"'
rc.write_text('''#include <windows.h>
1 RT_MANIFEST '''+quote(root/'Packaging/setup.manifest')+'''
101 RCDATA '''+quote(plugin)+'''
102 RCDATA '''+quote(guide)+'''
103 RCDATA '''+quote(root/'LICENSE.txt')+'''
104 RCDATA '''+quote(sources)+'''
105 RCDATA '''+quote(third_party)+'''
1 VERSIONINFO
 FILEVERSION 0,19,0,0
 PRODUCTVERSION 0,19,0,0
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
   VALUE "FileVersion", "0.19.0\\0"
   VALUE "ProductName", "Nova Synth\\0"
   VALUE "ProductVersion", "0.19.0\\0"
  END
 END
 BLOCK "VarFileInfo"
 BEGIN
  VALUE "Translation", 0x0409, 1200
 END
END
''')
tool=a.toolchain/'bin'
obj=build/'setup-resource.o'
subprocess.run([str(tool/'x86_64-w64-mingw32-windres'),str(rc),'-o',str(obj)],check=True)
output=a.output or root.parent/'NovaSynth_Instalar_Windows_x64.exe'
subprocess.run([str(tool/'x86_64-w64-mingw32-clang'),'-O2','-Wall','-Wextra',
    '-municode','-mwindows','-static','-Wl,--no-insert-timestamp','-Wl,--nxcompat','-Wl,--dynamicbase',
    str(root/'Packaging/Setup.c'),str(obj),'-lshell32','-lshlwapi','-lole32','-luuid','-ladvapi32','-o',str(output)],check=True)
print('Installer:',output,'bytes:',output.stat().st_size)
