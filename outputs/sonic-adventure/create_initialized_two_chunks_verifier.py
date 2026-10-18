from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_initialized_decode.py').read_text()
s=s.replace('0xa60038,256,256','0xa60038,256,512')
s=s.replace('0x8ca40008,256,32768','0x8ca40008,512,32768')
s=s.replace('0,600,0,0,base,1024','0,1112,0,0,base,2048')
s=s.replace('0xa40300]=bytes([0x5a])*768','0xa40500]=bytes([0x5a])*1280')
s=s.replace('0xa50300]=bytes([0x5a])*768','0xa50500]=bytes([0x5a])*1280')
extra='''  previous_history=[struct.unpack_from('<2h',native,0xa30028+channel*4) for channel in range(2)]
  s.r[4]=0x8ca60000;r[4]=s.r[4];s.pc=0x8c65bb88
  reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
  assert lower[0x8c65bb88](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
  assert struct.unpack_from('<I',native,0xa60034)[0]==512,(struct.unpack_from('<8I',native,0xa6002c),struct.unpack_from('<12I',native,0xac0000),struct.unpack_from('<8I',native,state+0x64))
  assert struct.unpack_from('<I',native,state+4)[0]==0 and r[15]==0x8cf00000
  payload=original_prefix[header_length+288:header_length+576]
  for channel,destination in [(0,0xa40208),(1,0xa50208)]:
   newest,older=[raw(float(value*65536)) for value in previous_history[channel]];samples=[]
   for pair in range(8):
    block=payload[(pair*2+channel)*18:(pair*2+channel+1)*18]
    assert len(block)==18 and not (block[0]&128)
    scale=(int.from_bytes(block[:2],'big')&8191)+1
    for packed in block[2:]:
     for nibble in (packed>>4,packed&15):
      signed=nibble-16 if nibble&8 else nibble
      prediction=multiply_add(coefficients[0],newest,multiply(coefficients[1],older)[0])[0]
      sample=multiply_add(raw(float(scale)),raw(float(signed*65536)),prediction)[0]
      samples.append((truncate_i32(sample)[0]>>16)&65535);older,newest=newest,sample
   assert native[destination:destination+512]==struct.pack('<256H',*samples)
   assert native[destination+512:destination+760]==bytes([0x5a])*248
   assert native[0xa30028+channel*4:0xa3002c+channel*4]==struct.pack('<2H',(truncate_i32(newest)[0]>>16)&65535,(truncate_i32(older)[0]>>16)&65535)
   assert struct.unpack_from('<I',native,0xac0000+channel*256+0x10)[0]==88
   assert struct.unpack_from('<I',native,0xac0000+channel*256+0x14)[0]==1024
'''
s=s.replace('  feed_updates+=1',extra+'  feed_updates+=1')
s=s.replace('feed_updates_compared=feed_updates,','feed_updates_compared=feed_updates,decode_updates_compared=feed_updates*2,')
s=s.replace('initialized_decode_verification','initialized_two_chunks_verification').replace('persistent initialized decoding sequences passed','persistent initialized two-chunk sequences passed')
s=s.replace('Producer/refill/full-file streaming/physical playback/fullgame unverified.', 'Second public update decodes256 further samples per channel with preserved signed16 predictor histories. Independent binary32 sample/history/guard checks; sample reservations start1112 bytes, finish88 bytes available/read1024; work limit512, configured output capacity512 and ring2048 controlled. Independent consumed512 and phase0 assertions. Producer/refill/full-file streaming/physical playback/fullgame unverified.')
(p/'verify_initialized_two_chunks.py').write_text(s)
