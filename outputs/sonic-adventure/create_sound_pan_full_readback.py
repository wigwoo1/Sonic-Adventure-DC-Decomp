from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_backend_readback_native_pan.py').read_text()
s=s.replace("build/sound_backend_readback.dll')).sa_sound_backend_readback;fn", "build/sound_driver_pan_get.dll')).sa_sound_driver_pan_get;fn")
i=s.index('cases=0');s=s[:i]+"upper_helpers={}\nfor a,dll,name in [(0x8c669278,'sound_driver_readback_wrapper','sa_sound_driver_readback_wrapper'),(0x8c66adc4,'sound_backend_readback','sa_sound_backend_readback'),(0x8c10ca5c,'signed_division','sa_signed_division')]:\n helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dll}.dll')),name);helper.argtypes=abi;upper_helpers[a]=helper\n"+s[i:]
s=s.replace(' before=bytes(ram);'," struct.pack_into('<I',ram,0xa06028,0x8ca08000)\n before=bytes(ram);")
s=s.replace(' def call(ctx,a,m,f,sr,r,pr):',' def call(ctx,a,m,f,sr,r,pr):\n  if a in upper_helpers:return upper_helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)')
s=s.replace('s.r[4]=0x8ca08000 if present else 0;', 's.r[4]=0x8ca06000;').replace('s.pc=0x8c66adc4','s.pc=0x8c668112')
s=s.replace('0x8ca20004+o) for a,o in specs','0x8cefffe0+o) for a,o in specs')
a=s.index(' output=bytearray');z=s.index(' signed_selector=',a)
s=s[:a]+" output=bytearray(before[0xa20000:0xa20028])\n result=ctypes.c_int8((value+2)&255).value\n if channels==2:\n  if (value&255)==0:result+=64\n  elif (value&255)==1:result=(result if result else -1)-64\n  result*=2\n result=ctypes.c_int8(result&255).value;scaled=(abs(result*15)//127)*(-1 if result<0 else 1)\n struct.pack_into('<I',output,4,scaled&0xffffffff)\n assert native[0xa06000:0xa06080]==before[0xa06000:0xa06080]\n"+s[z:]
a=s.index("scope='");z=s.index('\n(p/',a);s=s[:a]+"scope='112 complete retail RAM/CPU/FPU/SR/PR comparisons of connected upper pan getter668112,wrapper669278,backend66adc4,native descriptor/volume/pan readers,byte/device-word readers,transfer10d460 and signed division. Seven selector/payload fixtures,mono/stereo,both banks,transfer context modes,two modeled service returns. Independent hardware-byte2 selection,stereo bias/wrapping then signed15/127 scaling,caller40-byte output guards,upper128-byte/driver144-byte preservation,three aligned MMIO reads and restored stack. Seven other field readers/polling services modeled;controlled MMIO,real hardware/game integration unverified.'"+s[z:]
s=s.replace('backend_readback_native_pan_verification.json','sound_pan_full_readback_verification.json')
(p/'verify_sound_pan_full_readback.py').write_text(s)
reg=(p/'register_backend_readback_native_pan.py').read_text().replace("['backend_readback']","['sound_pan']").replace('backend_readback_native_pan_verification.json','sound_pan_full_readback_verification.json').replace('native_pan','full_readback').replace("'sa_sound_backend_pan_get'}","'sa_sound_backend_pan_get','sa_sound_driver_pan_get','sa_sound_driver_readback_wrapper','sa_signed_division'}")
(p/'register_sound_pan_full_readback.py').write_text(reg)
