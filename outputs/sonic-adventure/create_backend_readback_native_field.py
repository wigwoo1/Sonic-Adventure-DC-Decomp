from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_backend_readback_native_pan.py').read_text();i=s.index('cases=0');s=s[:i]+"field_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_field_get.dll')).sa_sound_backend_field_get;field_get.argtypes=abi\n"+s[i:]
s=s.replace('calls=[];reads=[];volume_reads=[]','calls=[];reads=[];volume_reads=[];field_reads=[]')
s=s.replace('  assert a in dict(specs);',"  if a==0x8c667150:\n   field_reads.append(r[4])\n   for i in range(4):m[(r[5]&0xffffff)+i]=(value+4+i)&255\n   r[0]=answer;return 0\n  assert a in dict(specs);")
s=s.replace('  service(a,m,r);return 0','  if a==0x8c66bc24:return field_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  service(a,m,r);return 0')
s=s.replace(' def hook(a):'," def field_hook(m,r,sr):\n  for i in range(4):m[(r[5]&0xffffff)+i]=(value+4+i)&255\n  r[0]=answer;return sr\n def hook(a):")
s=s.replace('0x8c66bdd4,0x8c66bb4c]','0x8c66bdd4,0x8c66bb4c,0x8c66bc24]').replace('0x8c10ee20]}}','0x8c10ee20]},0x8c667150:field_hook}')
s=s.replace('  else:output[4+o]=(value+i)&255',"  elif a==0x8c66bc24:output[4+o:6+o]=bytes([(value+4)&255,(value+5)&255])\n  else:output[4+o]=(value+i)&255")
s=s.replace(' assert not errors',' assert field_reads==[(0x8ca30400+0x55*64+signed_selector*32+8)&0xffffffff]\n assert not errors')
s=s.replace('Polling/control services and seven other field readers modeled','Native16-bit field reader66bc24 connected;independent address plus8,two packet bytes+6/+7 and neighbor preservation. Lower667150,polling/control services and six other field readers modeled')
s=s.replace('backend_readback_native_pan_verification.json','backend_readback_native_field_verification.json')
(p/'verify_backend_readback_native_field.py').write_text(s)
reg=(p/'register_backend_readback_native_pan.py').read_text().replace('native_pan','native_field').replace("'sa_sound_backend_pan_get'}","'sa_sound_backend_pan_get','sa_sound_backend_field_get'}")
(p/'register_backend_readback_native_field.py').write_text(reg)
