from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_backend_readback_native_descriptor.py').read_text();i=s.index('cases=0');s=s[:i]+"byte_read=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_byte_read.dll')).sa_sound_byte_read;byte_read.argtypes=abi\n"+s[i:]
a=s.index('  if a==0x8c6671c4:');z=s.index('  assert a in dict(specs)',a)
s=s[:a]+"  if a==0x8c6671c4:return byte_read(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  if a==0x8c66910c:\n   reads.append(r[4])\n   for i in range(4):m[(r[5]&0xffffff)+i]=(value+i)&255\n   r[0]=answer;return 0\n"+s[z:]
s=s.replace('def read_hook(m,r,sr):m[r[5]&0xffffff]=value&255;r[0]=answer;return sr',"def read_hook(m,r,sr):\n  for i in range(4):m[(r[5]&0xffffff)+i]=(value+i)&255\n  r[0]=answer;return sr")
s=s.replace('0x8c6671c4:read_hook','0x8c66910c:read_hook').replace('Lower6671c4 and nine other field readers modeled','Native byte reader6671c4 connected;aligned four-byte lower66910c and nine other field readers modeled').replace('backend_readback_native_descriptor_verification.json','backend_readback_native_byte_verification.json')
(p/'verify_backend_readback_native_byte.py').write_text(s)
reg=(p/'register_backend_readback_native_descriptor.py').read_text().replace('native_descriptor','native_byte').replace("'sa_sound_descriptor_byte_query'}","'sa_sound_descriptor_byte_query','sa_sound_byte_read'}")
(p/'register_backend_readback_native_byte.py').write_text(reg)
