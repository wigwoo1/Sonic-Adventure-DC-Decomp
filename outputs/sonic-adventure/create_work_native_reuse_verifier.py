from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_work_native_slots.py').read_text()
s=s.replace('pool=0x8a3910;cases=0', "for address,stem in [(0x8c65b72c,'sound_work_destroy'),(0x8c65ad68,'sound_critical_enter'),(0x8c65ad8e,'sound_critical_exit')]:helpers[address]=load(stem+'.dll','sa_'+stem)\npool=0x8a3910;cases=0;reuse_cases=0")
s=s.replace(' ram[pool:pool+1152]='," struct.pack_into('<2I',ram,0x67c1e0,0,9)\n ram[pool:pool+1152]=")
extra=''' if slot<8:
  s.r[4]=selected;r[4]=selected;s.pc=0x8c65b72c
  reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)
  assert helpers[0x8c65b72c](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  released_work=bytearray(wanted);released_work[slot*144:slot*144+4]=bytes(4)
  released_stream=bytearray(wanted_stream);released_stream[stream_slot*172:(stream_slot+1)*172]=bytes(172)
  released_handles=bytearray(wanted_handles);released_handles[handle_slot*52:(handle_slot+1)*52]=bytes(52)
  assert native[pool:pool+1152]==released_work
  assert native[stream_pool:stream_pool+1376]==released_stream and native[hs:hs+416]==released_handles
  assert struct.unpack_from('<2I',native,0x67c1e0)==(0,6) and (sr.value>>4)&15==6 and r[15]==0x8cf00000
  s.r[4:7]=[0x8ca80000,channels,0x8ca10000]
  for index in range(4,7):r[index]=s.r[index]
  s.pc=0x8c65b624;reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  assert native[pool:pool+1152]==wanted and native[stream_pool:stream_pool+1376]==wanted_stream and native[hs:hs+416]==wanted_handles
  assert r[0]==selected and r[15]==0x8cf00000 and native[0xa30000:0xa30030]==source_before
  reuse_cases+=1
'''
s=s.replace(' cases+=1',extra+' cases+=1')
s=s.replace('work_native_slots_verification','work_native_reuse_verification').replace('cases_passed=cases,','cases_passed=cases,reuse_sequences_passed=reuse_cases,')
s=s.replace('Playback/fullgame unverified.', '384 successful allocations continue native work destruction65b72c,stream/handle release,nested critical sections and recreation on persistent RAM/CPU/FPU without resets. Independent cleared pools after release and exact original pools restored by recreation,source unchanged,same return pointer,interrupt mask6 preserved. Playback/fullgame unverified.')
(p/'verify_work_native_reuse.py').write_text(s)
s=(p/'register_work_native_slots.py').read_text().replace('work_native_slots','work_native_reuse')
s=s.replace("'sa_file_decode_handle_release'}", "'sa_file_decode_handle_release','sa_sound_work_destroy','sa_sound_critical_enter','sa_sound_critical_exit'}")
s=s.replace('Native work/stream construction across pool slots:', 'Persistent native work/stream/handle release and reuse:')
s=s.replace("assert evidence['cases_passed']==432", "assert evidence['cases_passed']==432 and evidence['reuse_sequences_passed']==384")
(p/'register_work_native_reuse.py').write_text(s)
