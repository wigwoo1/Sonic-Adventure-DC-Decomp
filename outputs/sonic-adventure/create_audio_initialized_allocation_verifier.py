from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_work_native_allocation.py').read_text()
s=s.replace('pool=0x8a3910;cases=0', "for address,stem in [(0x8c65b5e8,'sound_work_pool_initialize'),(0x8c667370,'sound_stream_pool_initialize'),(0x8c644b64,'file_decode_pool_initialize')]:helpers[address]=load(stem+'.dll','sa_'+stem)\npool=0x8a3910;cases=0")
s=s.replace('for slot,channels,created,bank in itertools.product(range(9),[0,1,2],[0x8c8d61ac],[0,1]):', 'for pattern,channels,bank in itertools.product([0,1,127,128,255,0x55],[0,1,2],[0,1]):\n slot=0;created=0x8c8d61ac')
s=s.replace('bytes([0x77])*1152','bytes([pattern])*1152').replace('bytes([0x55])*(172*8)','bytes([pattern^0x55])*(172*8)').replace('bytes([0x77])*(52*8)','bytes([pattern^0xaa])*(52*8)')
at=s.index(' reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)')
extra=''' s.pc=0x8c65b5e8
 reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)
 assert helpers[0x8c65b5e8](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 nf.fpul=fpul.value
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
 before=bytes(1152);stream_before=bytes(1376);handles_before=bytes(416)
 assert native[pool:pool+1152]==before and native[stream_pool:stream_pool+1376]==stream_before and native[hs:hs+416]==handles_before
 s.r[4:7]=[0x8ca80000,channels,0x8ca10000]
 for index in range(4,7):r[index]=s.r[index]
 s.pc=0x8c65b624
'''
s=s[:at]+extra+s[at:]
s=s.replace('work_native_allocation_verification','audio_initialized_allocation_verification')
start=s.index("scope='");end=s.index("'),indent=2)",start)
s=s[:start]+"scope='36 persistent native pool-initialization-to-allocation sequences,each with full retail RAM/CPU/FPU/SR/PR comparison:six prior pool byte patterns,channels0/1/2,both banks. Work initializer65b5e8 connects stream initializer667370 and decode-handle initializer644b64; independent all work1152/stream1376/handle416 bytes zero before constructor. Constructor65b624 then allocates first work/stream/handle slots on same RAM/CPU/FPU without resets,all subordinate allocation/coefficient/accessor/callback/reset helpers native,no modeled calls. Independent complete three pools,active flags,bindings,callbacks,user data,source object preservation and returned work pointer;coefficients compared with retail and covered independently in dedicated coefficient verifier. Subsequent allocations,pool exhaustion,full playback/fullgame unverified."+s[end:]
(p/'verify_audio_initialized_allocation.py').write_text(s)
s=(p/'register_work_native_allocation.py').read_text().replace('work_native_allocation','audio_initialized_allocation').replace('==54','==36')
s=s.replace("'sa_file_decode_handle_release'}", "'sa_file_decode_handle_release','sa_sound_work_pool_initialize','sa_sound_stream_pool_initialize','sa_file_decode_pool_initialize'}")
s=s.replace('Fully native work and stream construction:', 'Native audio pool initialization followed by allocation:')
(p/'register_audio_initialized_allocation.py').write_text(s)
