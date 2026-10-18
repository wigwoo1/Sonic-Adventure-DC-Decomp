from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_backend_readback.py').read_text();i=s.index('cases=0');s=s[:i]+"query=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_descriptor_byte_query.dll')).sa_sound_descriptor_byte_query;query.argtypes=abi\n"+s[i:]
s=s.replace(' before=bytes(ram);'," struct.pack_into('<I',ram,0xa091b8,0x8ca30000)\n before=bytes(ram);")
s=s.replace('native);calls=[]','native);calls=[];reads=[]')
a=s.index('  assert a in dict(specs);');z=s.index(' def hook',a)
s=s[:a]+"  if a==0x8c6671c4:\n   reads.append(r[4]);m[r[5]&0xffffff]=value&255;r[0]=answer;return 0\n  assert a in dict(specs);calls.append((a,r[4],r[5],r[6]))\n  if a==0x8c6691d0:return query(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  service(a,m,r);return 0\n def read_hook(m,r,sr):m[r[5]&0xffffff]=value&255;r[0]=answer;return sr\n"+s[z:]
s=s.replace('{a:hook(a) for a,o in specs}', '{**{a:hook(a) for a,o in specs if a!=0x8c6691d0},0x8c6671c4:read_hook}')
s=s.replace('for i,(a,o) in enumerate(specs):output[4+o]=(value+i)&255',"for i,(a,o) in enumerate(specs):\n  if a==0x8c6691d0:struct.pack_into('<I',output,4+o,value&255)\n  else:output[4+o]=(value+i)&255\n signed_selector=ctypes.c_int8(value&255).value\n assert reads==[(0x8ca30400+0x55*64+signed_selector*32)&0xffffffff]")
s=s.replace('Ten field readers modeled','Native descriptor query6691d0 connected with independent signed selector address and unsigned32-bit output. Lower6671c4 and nine other field readers modeled').replace('sound_backend_readback_verification.json','backend_readback_native_descriptor_verification.json')
(p/'verify_backend_readback_native_descriptor.py').write_text(s)
reg=(p/'register_sound_apply_native_zero_queries.py').read_text().replace("['sound_apply']","['backend_readback']").replace('sound_apply_native_zero_queries_verification.json','backend_readback_native_descriptor_verification.json').replace('==168','==28').replace("{'sa_sound_subordinate_apply','sa_sound_driver_zero_query_a','sa_sound_driver_zero_query_b'}","{'sa_sound_backend_readback','sa_sound_descriptor_byte_query'}").replace('native_zero_queries','native_descriptor')
(p/'register_backend_readback_native_descriptor.py').write_text(reg)
