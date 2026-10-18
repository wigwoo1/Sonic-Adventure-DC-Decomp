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
  assert struct.unpack_from('<I',native,0xa60034)[0]==288, (asset['path'],struct.unpack_from('<8I',native,0xa6002c),struct.unpack_from('<4I',native,state+4),struct.unpack_from('<12I',native,0xac0000))
  assert struct.unpack_from('<I',native,state+4)[0]==0 and r[15]==0x8cf00000
  assert struct.unpack_from('<2I',native,state+0xa0)==(0,0)
  payload=original_prefix[header_length+288:header_length+324]
  for channel,destination in [(0,0xa40208),(1,0xa50208)]:
   newest,older=[raw(float(value*65536)) for value in previous_history[channel]];samples=[]
   block=payload[channel*18:(channel+1)*18]
   assert len(block)==18 and not (block[0]&128)
   scale=(int.from_bytes(block[:2],'big')&8191)+1
   for packed in block[2:]:
    for nibble in (packed>>4,packed&15):
     signed=nibble-16 if nibble&8 else nibble
     prediction=multiply_add(coefficients[0],newest,multiply(coefficients[1],older)[0])[0]
     sample=multiply_add(raw(float(scale)),raw(float(signed*65536)),prediction)[0]
     samples.append((truncate_i32(sample)[0]>>16)&65535);older,newest=newest,sample
   assert native[destination:destination+64]==struct.pack('<32H',*samples)
   assert native[destination+64:destination+248]==bytes([0x5a])*184
   assert native[0xa30028+channel*4:0xa3002c+channel*4]==struct.pack('<2H',(truncate_i32(newest)[0]>>16)&65535,(truncate_i32(older)[0]>>16)&65535)
   assert struct.unpack_from('<I',native,0xac0000+channel*256+0x10)[0]==24
   assert struct.unpack_from('<I',native,0xac0000+channel*256+0x14)[0]==576
'''
extra=extra[:extra.index("  assert struct.unpack_from('<I',native,0xa60034)[0]==288")]+'''  for (address,length),before in zip(wait_regions,wait_before):assert native[address:address+length]==before,(hex(address),[(i,before[i],native[address+i]) for i in range(length) if before[i]!=native[address+i]])
  assert struct.unpack_from('<I',native,0xa60034)[0]==256
  assert struct.unpack_from('<I',native,state+4)[0]==0 and r[15]==0x8cf00000
'''
s=s[:at]+extra+s[at:]
s=s.replace('initialized_decode_verification','initialized_backpressure_verification')
s=s.replace('persistent initialized decoding sequences passed','persistent initialized backpressure sequences passed')
s=s.replace('Producer/refill/full-file streaming/physical playback/fullgame unverified.', 'Second public update on the same objects decodes32 further samples per channel from remaining88-byte output reservations, preserving predictor histories and advancing each sample object read position to576/remaining24. Work limit512 is controlled; independent second-chunk sample/history/guard checks and consumed288 assertions. Producer/refill/full-file streaming/physical playback/fullgame unverified.')
s=s.replace('Second public update on the same objects decodes32 further samples per channel from remaining88-byte output reservations, preserving predictor histories and advancing each sample object read position to576/remaining24. Work limit512 is controlled; independent second-chunk sample/history/guard checks and consumed288 assertions.', 'Second public update waits because sample availability88 bytes is below initialized minimum32 blocks. Independent complete stream, handle, work, encoded and sample reservation objects, sample buffers and histories remain unchanged; consumed256 and phase0 preserved. Work limit512 is controlled.')
(p/'verify_initialized_backpressure.py').write_text(s)
