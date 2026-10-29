#!/usr/bin/env python3
"""Native Linux component tests, using an unpacked JUCE/development prefix.
Does not test the Windows installer or any DAW. No system packages installed.
"""
from pathlib import Path
import subprocess,concurrent.futures,argparse
parser=argparse.ArgumentParser();parser.add_argument('--juce',type=Path,required=True);parser.add_argument('--prefix',type=Path,required=True);parser.add_argument('--out',type=Path,required=True);parser.add_argument('--tests-only',action='store_true');a=parser.parse_args()
root=Path(__file__).resolve().parents[1];out=a.out;out.mkdir(exist_ok=True);juce=a.juce;prefix=a.prefix
mods=['juce_core','juce_events','juce_data_structures','juce_graphics','juce_gui_basics','juce_gui_extra','juce_audio_basics','juce_audio_formats','juce_audio_processors']
flags=['-std=c++17','-O1','-DNDEBUG','-DJUCE_MODAL_LOOPS_PERMITTED=1','-DJUCE_ALSA=0','-DJUCE_JACK=0','-include',str(root/'Packaging/BuildConfig.h'),'-I'+str(juce/'modules'),'-I'+str(root/'Source'),'-I'+str(prefix/'include'),'-I'+str(prefix/'include/freetype2'),'-I'+str(prefix/'include/x86_64-linux-gnu'),'-pthread']
libs=['-pthread','-ldl','-lrt','-l:libfreetype.so.6','-l:libfontconfig.so.1','-l:libX11.so.6','-l:libXext.so.6']
def build(src):
 src=Path(src);obj=out/(src.stem+'_native.o');print('Compiling',src.stem,flush=True)
 with (out/(src.stem+'_native.log')).open('w') as log:result=subprocess.run(['g++',*flags,'-c',str(src),'-o',str(obj)],stdout=log,stderr=subprocess.STDOUT)
 if result.returncode:raise RuntimeError((out/(src.stem+'_native.log')).read_text()[-15000:])
 return obj
sources=[juce/'modules'/m/(m+'.cpp') for m in mods]+[root/'Tests/EditorIntegrationTests.cpp']
if a.tests_only:objs=[out/(m+'_native.o') for m in mods]+[build(sources[-1])]
else:
 with concurrent.futures.ThreadPoolExecutor(max_workers=2) as pool:objs=list(pool.map(build,sources))
subprocess.run(['g++',*map(str,objs),*libs,'-o',str(out/'editor_tests')],check=True)
