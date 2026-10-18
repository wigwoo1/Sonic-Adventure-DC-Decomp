from pathlib import Path
p=Path(__file__).resolve().parent
for stem in ['sound_subordinate_field8_set','sound_subordinate_fieldc_set']:
 s=(p/f'verify_{stem}.py').read_text()
 s=s.replace('cases=0',"lower={}\nfor a,dep in [(0x8c667b7a,'sound_error_report'),(0x8c65e16e,'sound_linear_source_identity_get')]:\n lower[a]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dep}.dll')),'sa_'+dep);lower[a].argtypes=abi\ncases=0",1)
 s=s.replace(' before=bytes('," struct.pack_into('<2I',ram,0x67c37c,0x8c65e16e,0x8ca08000)\n before=bytes(",1)
 a=s.index('  assert a==0x8c667b7a');z=s.index(' s=State()',a)
 s=s[:a]+'''  if a==0x8c667b7a:assert r[4]==0x8c67217c
  else:assert a==0x8c65e16e and (r[4],r[5])==(0x8ca08000,0x8c67217c)
  return lower[a](m,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
'''+s[z:]
 s=s.replace('reference(ram,s,{0x8c667b7a:error_hook}', 'reference(ram,s,{}')
 s=s.replace('else:assert calls==events==[0x8c667b7a]','else:assert calls==[0x8c667b7a,0x8c65e16e]')
 s=s.replace("f'analysis/{stem}_verification.json'", "f'analysis/{stem}_native_error_verification.json'")
 s=s.replace('Null path forwards error string8c67217c to modeled reporter667b7a;native reporter/game callers/audio hardware unverified.','Null path forwards error string8c67217c through native reporter667b7a to controlled native identity-getter callback(userdata8ca08000,message8c67217c);no modeled calls. Game callers/audio hardware and application callback semantics unverified.')
 (p/f'verify_{stem}_native_error.py').write_text(s)
