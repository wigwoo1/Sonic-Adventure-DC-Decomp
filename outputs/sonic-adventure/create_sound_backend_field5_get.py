from pathlib import Path
p=Path(__file__).resolve().parent
stem='sound_backend_field5_get'
s=(p/'verify_sound_backend_field4_get.py').read_text().replace('sound_backend_field4_get',stem).replace('0x8c66babc','0x8c66b9b0').replace('66babc','66b9b0').replace('32+16','32+17').replace('address plus16','address plus17').replace('56-instruction','50-instruction')
s=s.replace("  if a==0x8c65acac:return error(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n",'')
s=s.replace("  assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c66b9b0,code)","  assert struct.unpack_from('<2I',native,0x67c1c8)==(0,0)")
s=s.replace('Native error recorder;', 'No diagnostic call on errors;').replace('exact error globals','diagnostic globals preserved')
(p/f'verify_{stem}.py').write_text(s)
(p/f'build_{stem}.ps1').write_text((p/'build_sound_backend_field4_get.ps1').read_text().replace('sound_backend_field4_get',stem))
s=(p/'register_sound_backend_field4_get.py').read_text().replace('sound_backend_field4_get',stem).replace(str(0x8c66babc),str(0x8c66b9b0)).replace(str(0x8c66bb2c),str(0x8c66ba14))
(p/f'register_{stem}.py').write_text(s)
