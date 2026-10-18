from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_native_audio_refill.py').read_text()
s=s.replace('(0x8c65bc1c,[0x8c8a3910,512])','(0x8c65bc1c,[0x8c8a3910,4096])')
a=s.index('  first_samples=');z=s.index('  feed_updates+=1',a)
extra='''  original_data=original_prefix+refill_bytes
  assert len(original_data)>=header_length+16*288
  for update_index in range(1,16):
   for channel,obj in enumerate([0x8a65e8,0x8a6618]):
    original_source=bytes(native[obj:obj+48]);descriptor=0x8caa1000+channel*16
    for entry,args in [(0x8c65e4b8,[0x8c000000|obj,1,512,descriptor]),(0x8c65e58a,[0x8c000000|obj,0,descriptor])]:
     for index,value in enumerate(args,4):s.r[index]=value;r[index]=value
     s.pc=entry
     reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
     assert lower[entry](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
     nf.fpul=fpul.value
     assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
     if entry==0x8c65e4b8:assert struct.unpack_from('<2I',native,descriptor&0xffffff)==(0x8ca40008+channel*0x10000+((update_index-1)%2)*512,512)
    wanted_source=bytearray(original_source)
    for off,value in [(0xc,0),(0x10,1024),(0x18,(update_index%2)*512)]:struct.pack_into('<I',wanted_source,off,value)
    assert native[obj:obj+48]==wanted_source
   previous_history=[struct.unpack_from('<2h',native,0x8906a8+channel*4) for channel in range(2)]
   previous_buffers=[bytes(native[base:base+2048]) for base in [0xa40008,0xa50008]]
   s.r[4]=0x8c8a3910;r[4]=s.r[4];s.pc=0x8c65bb88
   reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
   assert lower[0x8c65bb88](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
   assert struct.unpack_from('<I',native,0x8a3944)[0]==256*(update_index+1),(asset['path'],update_index,struct.unpack_from('<I',native,0x8a3944)[0])
   assert struct.unpack_from('<I',native,state+4)[0]==0 and r[15]==0x8cf00000
   payload=original_data[header_length+update_index*288:header_length+(update_index+1)*288]
   for channel,base in enumerate([0xa40008,0xa50008]):
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
    packed_samples=struct.pack('<256H',*samples);offset=(update_index%2)*512
    wanted_buffer=bytearray(previous_buffers[channel]);wanted_buffer[offset:offset+512]=packed_samples;wanted_buffer[offset+1024:offset+1536]=packed_samples
    assert native[base:base+2048]==wanted_buffer,(asset['path'],update_index,channel)
    assert native[base-8:base]==bytes([0x5a])*8 and native[base+2048:base+2296]==bytes([0x5a])*248
    assert native[0x8906a8+channel*4:0x8906ac+channel*4]==struct.pack('<2H',(truncate_i32(newest)[0]>>16)&65535,(truncate_i32(older)[0]>>16)&65535)
    obj=[0x8a65e8,0x8a6618][channel]
    assert struct.unpack_from('<4I',native,obj+0xc)==(512,512,((update_index+1)%2)*512,(update_index%2)*512)
'''
s=s[:a]+extra+s[z:]
s=s.replace('native_audio_refill_verification','native_audio_refilled_decode_verification').replace('decode_updates_compared=feed_updates*2','decode_updates_compared=feed_updates*16')
s=s.replace("['assets'];records=[]", "['assets'];\nimport sys\nif len(sys.argv)>1:assets=assets[:int(sys.argv[1])]\nrecords=[]")
a=s.index("scope='218 full");z=s.index("'),indent=2)",a)
scope='Native source/work/stream/handle construction,original-header processing,producer reservation/copy/publication,16 persistent256-sample decode updates and15 output-consumption cycles per asset/bank. Full retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparison after each public operation. Independent binary32 oracle for all4096 samples per channel,history continuity,full2048-byte ring/mirror state each update,guards,source counters and producer/consumer wrap. Original first4096-byte prefix plus up to4096 next original-file bytes published with native query/byte move/commit; final decoded payload crosses into refilled bytes. No modeled calls,constructed objects never replaced. Actual case/file/update counts are reported; optional command-line asset limit supports diagnostic runs. Repeated encoded refill,full-file streaming,physical playback/fullgame unverified.'
s=s[:a]+'scope='+repr(scope)+s[z+1:]
(p/'verify_native_audio_refilled_decode.py').write_text(s)
reg=(p/'register_native_audio_refill.py').read_text().replace('native_audio_refill','native_audio_refilled_decode')
reg=reg.replace('Native audio producer refill with output consumption and decoding:', 'Native decoding across original input refill boundary:')
reg=reg.replace("exec(compile(base", "base = base.replace(\"assert evidence['cases_passed'] == 218\", \"assert evidence['cases_passed'] == 218 and evidence['decode_updates_compared'] == 3488\")\nexec(compile(base")
(p/'register_native_audio_refilled_decode.py').write_text(reg)
