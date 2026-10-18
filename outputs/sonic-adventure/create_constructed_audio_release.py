from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_constructed_audio_decode.py').read_text()
loads='''for address,stem in [(0x8c65b72c,'sound_work_destroy'),(0x8c667440,'sound_stream_destroy'),(0x8c644bfe,'file_decode_handle_release'),(0x8c65ad68,'sound_critical_enter'),(0x8c65ad8e,'sound_critical_exit')]:
 lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);lower[address].argtypes=abi
'''
s=s.replace('assets=json.loads(',loads+'assets=json.loads(',1)
s=s.replace(' native=bytearray(ram)'," struct.pack_into('<2I',ram,0x67c1e0,0,9)\n native=bytearray(ram)")
extra='''  before_work_pool=bytes(native[0x8a3910:0x8a3d90]);before_stream_pool=bytes(native[0x8d61ac:0x8d670c]);before_handle_pool=bytes(native[0x890680:0x890820])
  before_buffers=[bytes(native[a:a+768]) for a in [0xa40000,0xa50000]]
  before_sources=[bytes(native[a:a+48]) for a in [0xa80000,0xac0000,0xac0100]]
  s.r[4]=0x8c8a3910;r[4]=s.r[4];s.pc=0x8c65b72c
  reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
  assert lower[0x8c65b72c](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
  wanted_pool=bytearray(before_work_pool);wanted_pool[:4]=bytes(4)
  assert native[0x8a3910:0x8a3d90]==wanted_pool
  wanted_pool=bytearray(before_stream_pool);wanted_pool[:172]=bytes(172)
  assert native[0x8d61ac:0x8d670c]==wanted_pool
  wanted_pool=bytearray(before_handle_pool);wanted_pool[:52]=bytes(52)
  assert native[0x890680:0x890820]==wanted_pool
  for a,before in zip([0xa40000,0xa50000],before_buffers):assert native[a:a+768]==before
  for a,before in zip([0xa80000,0xac0000,0xac0100],before_sources):assert native[a:a+48]==before
  assert struct.unpack_from('<2I',native,0x67c1e0)==(0,6) and (sr.value>>4)&15==6 and r[15]==0x8cf00000
'''
s=s.replace('  feed_updates+=1',extra+'  feed_updates+=1')
s=s.replace('constructed_audio_decode_verification','constructed_audio_release_verification')
s=s.replace('Producer/refill/full-file streaming/physical playback/fullgame unverified.', 'After verified decoding, native work destruction65b72c connects stream destruction667440,handle release644bfe,byte fill and critical enter/exit. Independent entire work1152/stream1376/handle416 pools: work first4 bytes cleared,stream first172 andhandle first52 cleared,other slots/bytes unchanged. Both768-byte sample/guard regions and all48 bytes of encoded and sample source objects preserved. Critical depth0,saved mask6,SR interrupt mask6 and stack restored. Producer/refill/full-file streaming/physical playback/fullgame unverified.')
(p/'verify_constructed_audio_release.py').write_text(s)
reg=(p/'register_constructed_audio_decode.py').read_text().replace('constructed_audio_decode','constructed_audio_release')
reg=reg.replace("names.update({'sa_sound_work_pool_initialize'", "names.update({'sa_sound_work_destroy','sa_sound_stream_destroy','sa_file_decode_handle_release','sa_sound_critical_enter','sa_sound_critical_exit','sa_sound_work_pool_initialize'")
reg=reg.replace('Native audio construction through original-header decoding:', 'Native audio construction, original-header decoding and release:')
(p/'register_constructed_audio_release.py').write_text(reg)
