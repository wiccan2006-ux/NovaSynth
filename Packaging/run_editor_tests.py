#!/usr/bin/env python3
"""Headless Linux GUI regression harness. This is not a Windows/FL Studio test."""
from pathlib import Path
import ctypes,os,re,subprocess,time,sys
root=Path(__file__).resolve().parents[1];binary=Path(sys.argv[1]).resolve();screens=Path(sys.argv[2]).resolve();screens.mkdir(parents=True,exist_ok=True)
env=os.environ.copy();env['DISPLAY']='localhost:89'
with (screens/'xvfb.log').open('w') as log:
 server=subprocess.Popen(['Xvfb',':89','-screen','0','1920x1440x24','-ac','-nolisten','unix','-nolisten','local','-listen','tcp','-noreset'],env=env,stdout=log,stderr=subprocess.STDOUT)
 try:
  lib=ctypes.CDLL('libX11.so.6');lib.XOpenDisplay.argtypes=[ctypes.c_char_p];lib.XOpenDisplay.restype=ctypes.c_void_p;lib.XInternAtom.argtypes=[ctypes.c_void_p,ctypes.c_char_p,ctypes.c_int];lib.XInternAtom.restype=ctypes.c_ulong
  display=None
  for _ in range(30):
   display=lib.XOpenDisplay(b'localhost:89')
   if display:break
   if server.poll() is not None:raise RuntimeError('Xvfb exited')
   time.sleep(.1)
  if not display:raise RuntimeError('Test display unavailable')
  src=(root/'ThirdParty/JUCE/modules/juce_gui_basics/native/juce_XWindowSystem_linux.cpp').read_text()
  for name in set(re.findall(r'getIfExists\s*\([^\"]*\"([^\"]+)\"',src)):lib.XInternAtom(display,name.encode(),False)
  raise SystemExit(subprocess.run([str(binary),str(screens)],env=env).returncode)
 finally:server.terminate();server.wait()
