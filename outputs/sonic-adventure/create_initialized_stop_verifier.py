from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_initialized_two_chunks.py').read_text()
at=s.index('assets=json.loads(')
load="for address,stem,name in [(0x8c65b78e,'sound_work_stop','sa_sound_work_stop'),(0x8c66777c,'sound_stream_decode_reset','sa_sound_stream_decode_reset'),(0x8c644c68,'file_decode_handle_reset','sa_file_decode_handle_reset')]:\n lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),name);lower[address].argtypes=abi\n"
s=s[:at]+load+s[at:]
extra='''  stop_work=bytearray(native[0xa60000:0xa60100]);stop_work[1]=0
  stop_stream=bytearray(native[state:state+256]);struct.pack_into('<I',stop_stream,4,0)
  stop_handle=bytearray(native[0xa30000:0xa30040]);struct.pack_into('<I',stop_handle,0xc,0);stop_handle[0x28:0x30]=bytes(8)
  guard_regions=[(0xa40000,1280),(0xa50000,1280),(0xa80000,48),(0xac0000,48),(0xac0100,48)]
  guard_before=[bytes(native[address:address+length]) for address,length in guard_regions]
  s.r[4]=0x8ca60000;r[4]=s.r[4];s.pc=0x8c65b78e
  reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
  assert lower[0x8c65b78e](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
  assert native[0xa60000:0xa60100]==stop_work and native[state:state+256]==stop_stream and native[0xa30000:0xa30040]==stop_handle
  for (address,length),before in zip(guard_regions,guard_before):assert native[address:address+length]==before
  assert r[15]==0x8cf00000
'''
s=s.replace('  feed_updates+=1',extra+'  feed_updates+=1')
s=s.replace('initialized_two_chunks_verification','initialized_stop_verification').replace('persistent initialized two-chunk sequences passed','persistent initialized decode/stop sequences passed')
s=s.replace('work limit512 and ring2048 controlled.', 'work limit512, configured output capacity512 and ring2048 controlled.')
s=s.replace('Producer/refill/full-file streaming/physical playback/fullgame unverified.', 'After both decoded chunks,native work stop65b78e calls stream reset66777c and handle reset644c68 on the same objects. Independent whole work/stream/handle objects:work state1 becomes0,phases0,predictor histories zero;coefficients/counters/samples/all reservation objects preserved. Full retail RAM/CPU/FPU/FPUL/MACL comparison of stop call. Producer/refill/full-file streaming/physical playback/fullgame unverified.')
s=s.replace('work state1 becomes0','work state byte1 becomes0')
(p/'verify_initialized_stop.py').write_text(s)
s=(p/'register_initialized_two_chunks.py').read_text()
s=s.replace("exec(compile(base, str(__file__), 'exec'))", "base=base.replace('initialized_two_chunks','initialized_stop').replace('Persistent original initialization and two decoded chunks:', 'Persistent original initialization, decoding and native stop:')\nbase=base.replace(\"path = root / 'analysis/recovered_routines.json'\",\"names.update({'sa_sound_work_stop','sa_sound_stream_decode_reset','sa_file_decode_handle_reset'})\\npath = root / 'analysis/recovered_routines.json'\")\nexec(compile(base, str(__file__), 'exec'))")
(p/'register_initialized_stop.py').write_text(s)
