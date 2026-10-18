from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_audio_initialized_pool_capacity.py').read_text()
s=s.replace('pool=0x8a3910;cases=0',"for address,stem in [(0x8c65b72c,'sound_work_destroy'),(0x8c65ad68,'sound_critical_enter'),(0x8c65ad8e,'sound_critical_exit')]:helpers[address]=load(stem+'.dll','sa_'+stem)\npool=0x8a3910;cases=0;reuse_cases=0")
s=s.replace(' ram[pool:pool+1152]='," struct.pack_into('<2I',ram,0x67c1e0,0,9)\n ram[pool:pool+1152]=")
extra=''' for slot in [3,0,7,2,5,1,6,4]:
  selected=0x8c000000|(pool+slot*144)
  original_work=bytes(native[pool:pool+1152]);original_stream=bytes(native[stream_pool:stream_pool+1376]);original_handles=bytes(native[hs:hs+416])
  s.r[4]=selected;r[4]=selected;s.pc=0x8c65b72c
  reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)
  assert helpers[0x8c65b72c](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  released_work=bytearray(original_work);released_work[slot*144:slot*144+4]=bytes(4)
  released_stream=bytearray(original_stream);released_stream[slot*172:(slot+1)*172]=bytes(172)
  released_handles=bytearray(original_handles);released_handles[slot*52:(slot+1)*52]=bytes(52)
  assert native[pool:pool+1152]==released_work and native[stream_pool:stream_pool+1376]==released_stream and native[hs:hs+416]==released_handles
  assert struct.unpack_from('<2I',native,0x67c1e0)==(0,6) and (sr.value>>4)&15==6 and r[15]==0x8cf00000
  s.r[4:7]=[0x8ca80000,channels,0x8ca10000]
  for index in range(4,7):r[index]=s.r[index]
  s.pc=0x8c65b624
  reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  assert native[pool:pool+1152]==original_work and native[stream_pool:stream_pool+1376]==original_stream and native[hs:hs+416]==original_handles
  assert r[0]==selected and r[15]==0x8cf00000 and native[0xa30000:0xa30030]==source_before and actual==expected==[]
  reuse_cases+=1
'''
s=s.replace(' cases+=1',extra+' cases+=1')
s=s.replace('audio_initialized_pool_capacity_verification','audio_full_pool_reuse_verification')
s=s.replace('allocation_calls=cases*9,','allocation_calls=cases*17,reuse_sequences_passed=reuse_cases,')
s=s.replace('Unequal subordinate pool exhaustion, allocation failure callback behavior, full playback and full game remain unverified.','After saturation, native destruction and reallocation of each of eight slots in nonsequential order on persistent RAM/CPU/FPU: 288 release/reuse pairs. Independent complete released pools, unchanged occupied slots, restored constructed pools, same returned pointer, source preservation and interrupt mask. Native critical enter/exit, stream destroy and handle release connected. Unequal subordinate pool exhaustion, allocation failure callback behavior, full playback and full game remain unverified.')
s=s.replace('with 324 constructor calls','with 612 constructor calls')
s=s.replace("cases*9, 'constructor calls'","cases*17, 'constructor calls;', reuse_cases, 'release/reuse pairs'")
(p/'verify_audio_full_pool_reuse.py').write_text(s)
reg=(p/'register_audio_initialized_pool_capacity.py').read_text().replace('audio_initialized_pool_capacity','audio_full_pool_reuse')
reg=reg.replace("assert evidence['cases_passed']==36","assert evidence['cases_passed']==36 and evidence['reuse_sequences_passed']==288")
reg=reg.replace("'sa_file_decode_pool_initialize'}","'sa_file_decode_pool_initialize','sa_sound_work_destroy','sa_sound_critical_enter','sa_sound_critical_exit'}")
reg=reg.replace('Native audio initialization and full pool capacity:','Native full audio pool release and reuse:')
(p/'register_audio_full_pool_reuse.py').write_text(reg)
