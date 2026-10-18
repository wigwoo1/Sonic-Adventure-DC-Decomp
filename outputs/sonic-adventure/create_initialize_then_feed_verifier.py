from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_public_initialize_headers.py').read_text()
s=s.replace("[(0x8c65bb88,'sound_stream_work_update'","[(0x8c65b930,'sound_stream_work_feed','sa_sound_stream_work_feed'),(0x8c65e462,'sound_source_capacity','sa_sound_source_capacity'),(0x8c65bc6c,'sound_work_unit_count_get','sa_sound_work_unit_count_get'),(0x8c65bc84,'sound_work_total_samples_get','sa_sound_work_total_samples_get'),(0x8c6676ee,'sound_stream_unit_count_get','sa_sound_stream_unit_count_get'),(0x8c6676f6,'sound_stream_total_samples_get','sa_sound_stream_total_samples_get'),(0x8c66771c,'sound_stream_source_set','sa_sound_stream_source_set'),(0x8c66776e,'sound_stream_start','sa_sound_stream_start'),(0x8c65bb88,'sound_stream_work_update'")
s=s.replace('records=[];cases=0','records=[];cases=0;feed_updates=0')
s=s.replace(" with path.open('rb') as file:data=file.read(64)"," with path.open('rb') as file:original_prefix=file.read(4096)\n data=original_prefix[:64];assert len(original_prefix)==4096")
s=s.replace('ram[0xa10000:0xa10000+len(data)]=data','ram[0xa10000:0xa11000]=original_prefix')
s=s.replace(" struct.pack_into('<I',ram,state+8", " struct.pack_into('<I',ram,state+4,0)\n struct.pack_into('<I',ram,state+8")
s=s.replace(" struct.pack_into('<2I',ram,0xa60004,0x8ca20000,0x8ca80000)"," struct.pack_into('<2I',ram,0xa60004,0x8ca20000,0x8ca80000)\n ram[0xa60003]=0;struct.pack_into('<I',ram,0xa60034,0)\n struct.pack_into('<I',ram,0xa6000c,0x8cac0000)\n struct.pack_into('<I',ram,0xac0000,0x8ca90000);struct.pack_into('<I',ram,0xac0010,512)\n struct.pack_into('<I',ram,0xa90024,0x8c65e462)")
s=s.replace("target=bytearray([0x55])*256;", "target=bytearray([0x55])*256;struct.pack_into('<I',target,4,0);")
needle=' records.append(dict('
at=s.index(needle)
addition=''' if success:
  feed_work_before=bytes(native[0xa60000:0xa60100]);feed_stream_before=bytes(native[state:state+256]);feed_handle_before=bytes(native[0xa30000:0xa30040])
  s.r[4]=0x8ca60000;r[4]=s.r[4];s.pc=0x8c65b930
  reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
  assert lower[0x8c65b930](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
  reserved=4096-header_length;pointer=0x8ca10000+header_length
  wanted=bytearray(feed_work_before);struct.pack_into('<2I',wanted,0x14,pointer,reserved)
  assert native[0xa60000:0xa60100]==wanted
  wanted=bytearray(feed_stream_before)
  for off,value in [(4,1),(0x50,pointer),(0x54,reserved//18),(0x84,0),(0xa0,0),(0xa4,0)]:struct.pack_into('<I',wanted,off,value)
  assert native[state:state+256]==wanted
  assert struct.unpack_from('<I',native,0xa8000c)[0]==0 and struct.unpack_from('<I',native,0xa80010)[0]==header_length and struct.unpack_from('<I',native,0xa80018)[0]==4096
  assert native[0xa30000:0xa30040]==feed_handle_before and r[15]==0x8cf00000
  feed_updates+=1
 assert native[0xa10000:0xa11000]==original_prefix
'''
s=s[:at]+addition+s[at:]
s=s.replace('cases_passed=cases,original_files=','cases_passed=cases,feed_updates_compared=feed_updates,original_files=')
s=s.replace('public_initialize_headers_verification','initialize_then_feed_verification').replace('public original header initialization comparisons passed','persistent initialization/feed sequences passed')
s=s.replace('Full payload/decoder/physical audio/fullgame unverified.','Successful initialization continues native feeding on the same RAM/CPU/FPU/objects without resets. Independent work descriptor starts after header, source remaining reservation exhausted/write position4096, stream source/unit count/reset fields/phase1 and decoder handle unchanged. Actual4096-byte original prefix preserved. Native decoding/physical playback/fullgame unverified.')
(p/'verify_initialize_then_feed.py').write_text(s)
r=(p/'register_public_initialize_headers.py').read_text().replace('public_initialize_headers','initialize_then_feed').replace("'sa_sound_stream_work_update'}","'sa_sound_stream_work_update','sa_sound_stream_work_feed','sa_sound_stream_source_set','sa_sound_stream_start'}").replace('Original headers through public work initialization:','Persistent original initialization and feeding:')
(p/'register_initialize_then_feed.py').write_text(r)
