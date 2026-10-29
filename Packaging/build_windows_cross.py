#!/usr/bin/env python3
"""Build Windows x64 with portable LLVM-MinGW and an unmodified JUCE 7.0.12 tree.

Usage: python3 Packaging/build_windows_cross.py --toolchain /path/llvm-mingw --juce /path/JUCE
Uses no system package installation and does not need a Windows SDK or CMake.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import subprocess

p=argparse.ArgumentParser()
p.add_argument('--toolchain',type=Path,required=True)
p.add_argument('--juce',type=Path,required=True)
p.add_argument('--jobs',type=int,default=2)
p.add_argument('--plugin-only',action='store_true',help='Reuse JUCE objects after a full build with the same configuration; rebuild only plugin source.')
a=p.parse_args()
root=Path(__file__).resolve().parents[1]
out=root/'build-windows'
out.mkdir(exist_ok=True)
compiler=a.toolchain/'bin/x86_64-w64-mingw32-clang++'
modules=['juce_core','juce_events','juce_data_structures','juce_graphics',
         'juce_gui_basics','juce_gui_extra','juce_audio_basics','juce_audio_formats','juce_audio_processors']
sources=[root/'Source/Plugin.cpp']+[a.juce/'modules'/m/(m+'.cpp') for m in modules]
sources.append(a.juce/'modules/juce_audio_plugin_client/juce_audio_plugin_client_VST3.cpp')
flags=['-std=c++17','-O2','-DNDEBUG','-DWIN32','-D_WINDOWS','-DUNICODE','-D_UNICODE',
       '-D_WIN32_WINNT=0x0A00','-DWINVER=0x0A00','-D_CRT_SECURE_NO_WARNINGS',
       '-include',str(root/'Packaging/BuildConfig.h'),'-I'+str(a.juce/'modules'),'-I'+str(root/'Source'),
       '-I'+str(a.juce/'modules/juce_audio_processors/format_types/VST3_SDK')]
def build(src):
    obj=out/(src.stem+'.o')
    print('Compiling',src.stem,flush=True)
    log=out/(src.stem+'.log')
    with log.open('w') as f:
        result=subprocess.run([str(compiler),*flags,'-c',str(src),'-o',str(obj)],stdout=f,stderr=subprocess.STDOUT)
    if result.returncode:
        print(log.read_text()[-12000:],flush=True)
        raise RuntimeError('Compilation failed: '+src.name)
    return obj
if a.plugin_only:
    objects=[out/(src.stem+'.o') for src in sources]
    assert all(path.is_file() for path in objects[1:]), "Run a full build first."
    objects[0]=build(sources[0])
else:
    with ThreadPoolExecutor(max_workers=a.jobs) as pool:
        objects=list(pool.map(build,sources))
bundle=out/'Nova Synth.vst3/Contents/x86_64-win'
bundle.mkdir(parents=True,exist_ok=True)
libs=['uuid','wsock32','wininet','version','ole32','ws2_32','oleaut32','imm32','comdlg32',
      'shlwapi','rpcrt4','winmm','dxgi','user32','gdi32','comctl32','advapi32','shell32',
      'dwrite','d2d1','dwmapi','opengl32']
command=[str(compiler),'-shared','-static','-static-libgcc','-static-libstdc++',
         '-Wl,--no-insert-timestamp','-Wl,--nxcompat','-Wl,--dynamicbase',
         *map(str,objects),*['-l'+l for l in libs],'-o',str(bundle/'Nova Synth.vst3')]
print('Linking Windows x64 VST3',flush=True)
subprocess.run(command,check=True)
print('Built:',bundle/'Nova Synth.vst3',flush=True)
