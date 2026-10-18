from pathlib import Path
p=Path(__file__).resolve().parent
for kind,suffix,getter,entry in [('pan','native_diagnostics','sound_subordinate_pan_get',0x8c65de56),('effect','native_queue','sound_subordinate_effect_get',0x8c65de7a)]:
 stem='sound_subordinate_'+kind
 s=(p/f'verify_{stem}_{suffix}.py').read_text()
 s=s.replace('cases=0',f"getfn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/{getter}.dll')).sa_{getter};getfn.argtypes=abi\ncases=0",1)
 if kind=='pan':
  extra=f''' for null in [0,1]:
  s.r[4:6]=[0 if null else 0x8ca06000,selector]
  r[4]=s.r[4];r[5]=s.r[5];s.pc={entry:#x}
  reference(ram,s,fpu=f,special=special,hooks={{}})
  assert getfn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  assert r[0]==(0xffffffff if null else clamped&0xffffffff)
  assert native[0xa06000:0xa06080]==wanted_upper and r[15]==0x8cf00000
'''
  detail=' Persistent native pan query follows native setter/queue on the same RAM/CPU/FPU;valid query returns clamped cached pan even when queue rejected the command. Null query returnsffffffff through native error reporting. Full retail state and independent128-byte object preservation after both queries;no state resets.'
 else:
  extra=f''' for mode in ['distinct','alias','null']:
  first=0x8caa3004;second=first if mode=='alias' else 0x8caa3008
  for mem in [ram,native]:mem[0xaa3000:0xaa3010]=bytes([0x66])*16
  s.r[4:7]=[0 if mode=='null' else 0x8ca06000,first,second]
  for index in range(4,7):r[index]=s.r[index]
  s.pc={entry:#x};reference(ram,s,fpu=f,special=special,hooks={{}})
  assert getfn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  wanted_output=bytearray([0x66]*16)
  for ptr,result in [(first,0xffffffff if mode=='null' else effect),(second,0xffffffff if mode=='null' else clamped&0xffffffff)]:struct.pack_into('<I',wanted_output,(ptr&0xffffff)-0xaa3000,result)
  assert native[0xaa3000:0xaa3010]==wanted_output and native[0xa06000:0xa06080]==wanted_upper and r[15]==0x8cf00000
'''
  detail=' Persistent native effect-pair queries follow native setter/queue on the same RAM/CPU/FPU. Distinct outputs return cachedeffect/clampedlevel even on queue rejection;aliased outputs retain the second stored level;null query writesffffffff to both outputs via native error reporting. Independent entire16-byte output window/guards and128-byte object preservation after every query. Caller initializes output guards before each query;no object/CPU/FPU resets.'
 s=s.replace(' cases+=1',extra+' cases+=1')
 s=s.replace(f'{stem}_{suffix}_verification.json',f'{stem}_connected_readback_verification.json')
 s=s.replace('Physical audio/game caller integration/fullgame',detail+' Physical audio/game caller integration/fullgame')
 (p/f'verify_{stem}_connected_readback.py').write_text(s)
 reg=(p/f'register_{stem}_{suffix}.py').read_text().replace('_'+suffix,'_connected_readback').replace(' native diagnostics:',' connected readback:').replace(' native queue:',' connected readback:')
 reg=reg.replace("names={'sa_'+stem,",f"names={{'sa_{getter}','sa_'+stem,")
 (p/f'register_{stem}_connected_readback.py').write_text(reg)
