from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_backend_readback_native_transfer.py').read_text();i=s.index('cases=0');s=s[:i]+"volume_get=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_backend_volume_get.dll')).sa_sound_backend_volume_get;volume_get.argtypes=abi\n"+s[i:]
s=s.replace('calls=[];reads=[]','calls=[];reads=[];volume_reads=[]')
s=s.replace('  assert a in dict(specs);', "  if a==0x8c667188:\n   volume_reads.append(r[4]);m[r[5]&0xffffff]=(value+2)&255;r[0]=answer;return 0\n  assert a in dict(specs);")
s=s.replace('  service(a,m,r);return 0',"  if a==0x8c66bdd4:return volume_get(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  service(a,m,r);return 0")
s=s.replace(' def hook(a):'," def volume_hook(m,r,sr):m[r[5]&0xffffff]=(value+2)&255;r[0]=answer;return sr\n def hook(a):")
s=s.replace('if a!=0x8c6691d0}', 'if a not in [0x8c6691d0,0x8c66bdd4]}').replace('0x8c10ee20]}','0x8c10ee20]},0x8c667188:volume_hook}')
s=s.replace('volume_hook}},fpu','volume_hook},fpu')
s=s.replace(' assert not errors',' assert volume_reads==[(0x8ca30400+0x55*64+signed_selector*32+1)&0xffffffff]\n assert not errors')
s=s.replace('Polling/control services and nine other field readers modeled', 'Native backend volume getter66bdd4 connected;independent volume read address plus1 and packet byte+2. Lower667188,polling/control services and eight other field readers modeled')
s=s.replace('backend_readback_native_transfer_verification.json','backend_readback_native_volume_verification.json')
(p/'verify_backend_readback_native_volume.py').write_text(s)
reg=(p/'register_backend_readback_native_transfer.py').read_text().replace('native_transfer','native_volume').replace("'sa_device_word_read'}","'sa_device_word_read','sa_sound_backend_volume_get'}")
(p/'register_backend_readback_native_volume.py').write_text(reg)
