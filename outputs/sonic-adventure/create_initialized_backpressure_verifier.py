from pathlib import Path

p=Path(__file__).resolve().parent
s=(p/'verify_initialized_decode.py').read_text()
s=s.replace('0xa60038,256,256','0xa60038,256,512')
at=s.index('  feed_updates+=1')
extra='''  wait_regions=[(0xa30028,12),(0xa80000,48),(0xac0000,48),(0xac0100,48),(0xa40000,768),(0xa50000,768)]
  wait_before=[bytes(native[address:address+length]) for address,length in wait_regions]
  previous_history=[struct.unpack_from('<2h',native,0xa30028+channel*4) for channel in range(2)]
  s.r[4]=0x8ca60000;r[4]=s.r[4];s.pc=0x8c65bb88
  reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
  assert lower[0x8c65bb88](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
  for (address,length),before in zip(wait_regions,wait_before):assert native[address:address+length]==before
  assert struct.unpack_from('<I',native,0xa60034)[0]==256
  assert struct.unpack_from('<I',native,state+4)[0]==0 and r[15]==0x8cf00000
  assert struct.unpack_from('<2I',native,state+0xa0)==(0,0)
'''
s=s[:at]+extra+s[at:]
s=s.replace('initialized_decode_verification','initialized_backpressure_verification')
s=s.replace('persistent initialized decoding sequences passed','persistent initialized backpressure sequences passed')
s=s.replace('Producer/refill/full-file streaming/physical playback/fullgame unverified.', 'Second public update on the same objects decodes32 further samples per channel from remaining88-byte output reservations, preserving predictor histories and advancing each sample object read position to576/remaining24. Work limit512 is controlled; independent second-chunk sample/history/guard checks and consumed288 assertions. Producer/refill/full-file streaming/physical playback/fullgame unverified.')
s=s.replace('Second public update on the same objects decodes32 further samples per channel from remaining88-byte output reservations, preserving predictor histories and advancing each sample object read position to576/remaining24. Work limit512 is controlled; independent second-chunk sample/history/guard checks and consumed288 assertions.', 'Second public update reports zero decoded samples/bytes with88 bytes available per output channel. Independent complete encoded and sample reservation objects, sample buffers, predictor histories and coefficients remain unchanged; consumed256 and phase0 preserved. Request pointers/capacity fields change and are covered by full retail RAM/CPU/FPU comparisons. Work limit512 is controlled.')
(p/'verify_initialized_backpressure.py').write_text(s)
