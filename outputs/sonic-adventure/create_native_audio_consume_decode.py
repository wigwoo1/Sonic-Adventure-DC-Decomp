import ast
from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_native_sources_audio_decode.py').read_text()
s=s.replace('(0x8c65bc1c,[0x8c8a3910,256])','(0x8c65bc1c,[0x8c8a3910,512])')
s=s.replace('0xa40300]=bytes([0x5a])*768','0xa40900]=bytes([0x5a])*2304').replace('0xa50300]=bytes([0x5a])*768','0xa50900]=bytes([0x5a])*2304')
consumer='''  first_samples=[bytes(native[a:a+512]) for a in [0xa40008,0xa50008]]
  for channel,obj in enumerate([0x8a65e8,0x8a6618]):
   original_source=bytes(native[obj:obj+48]);descriptor=0x8caa1000+channel*16
   for entry,args in [(0x8c65e4b8,[0x8c000000|obj,1,512,descriptor]),(0x8c65e58a,[0x8c000000|obj,0,descriptor])]:
    for index,value in enumerate(args,4):s.r[index]=value;r[index]=value
    s.pc=entry
    reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
    assert lower[entry](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    nf.fpul=fpul.value
    assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
    if entry==0x8c65e4b8:assert struct.unpack_from('<2I',native,descriptor&0xffffff)==(0x8ca40008+channel*0x10000,512)
   wanted_source=bytearray(original_source)
   for off,value in [(0xc,0),(0x10,1024),(0x18,512)]:struct.pack_into('<I',wanted_source,off,value)
   assert native[obj:obj+48]==wanted_source
   assert native[0xa40008+channel*0x10000:0xa40208+channel*0x10000]==first_samples[channel]
'''
tree=ast.parse((p/'create_initialized_two_chunks_verifier.py').read_text())
extra=next(ast.literal_eval(n.value) for n in tree.body if isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='extra' for t in n.targets))
extra=extra.replace('0xa30028','0x8906a8').replace('0xa3002c','0x8906ac').replace('0x8ca60000','0x8c8a3910').replace('0xa60034','0x8a3944').replace('0xa6002c','0x8a393c')
extra=extra.replace('   assert native[destination+512:destination+760]==bytes([0x5a])*248', "   base=destination-512\n   assert native[base+1024:base+1536]==first_samples[channel]\n   assert native[base+1536:base+2048]==struct.pack('<256H',*samples)\n   assert native[base+2048:base+2296]==bytes([0x5a])*248")
extra=extra.replace("   assert struct.unpack_from('<I',native,0xac0000+channel*256+0x10)[0]==88\n   assert struct.unpack_from('<I',native,0xac0000+channel*256+0x14)[0]==1024\n", "   obj=[0x8a65e8,0x8a6618][channel]\n   assert struct.unpack_from('<4I',native,obj+0xc)==(512,512,0,512)\n   assert native[0xa40008+channel*0x10000:0xa40208+channel*0x10000]==first_samples[channel]\n")
s=s.replace('  feed_updates+=1',consumer+extra+'  feed_updates+=1')
s=s.replace('native_sources_audio_decode_verification','native_audio_consume_decode_verification')
s=s.replace('feed_updates_compared=feed_updates,','feed_updates_compared=feed_updates,decode_updates_compared=feed_updates*2,')
s=s.replace('Refill/full-file streaming/physical playback/fullgame unverified.', 'After first256 decoded samples per channel, native consumer query mode1 reserves512 bytes and native commit mode0 releases capacity. Independent entire48-byte source objects,published0/free1024/read512,returned descriptor and samples preserved. Second public update decodes another256 samples with preserved signed16 predictor histories; independent exact binary32 sample/history/guard checks,first samples preserved,consumed512,phase0 and source counters published512/free512/write0/read512,including producer cursor wrap at1024 bytes. Independent exact mirrored copies of both512-byte sample blocks at offsets1024/1536 and untouched248-byte region beyond offset2048. 436 decode updates. Encoded refill/full-file streaming/physical playback/fullgame unverified.')
(p/'verify_native_audio_consume_decode.py').write_text(s)
reg=(p/'register_native_sources_audio_decode.py').read_text().replace('native_sources_audio_decode','native_audio_consume_decode').replace('Native source creation and publication through audio decoding:', 'Native audio source construction, output consumption and repeated decoding:')
(p/'register_native_audio_consume_decode.py').write_text(reg)
