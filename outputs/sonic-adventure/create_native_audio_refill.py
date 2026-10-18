from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_native_audio_consume_decode.py').read_text()
s=s.replace(" data=original_prefix[:64]", " with path.open('rb') as file:file.seek(4096);refill_bytes=file.read(4096)\n data=original_prefix[:64]")
s=s.replace(' native=bytearray(ram)'," ram[0xaa2000:0xaa2000+len(refill_bytes)]=refill_bytes\n native=bytearray(ram)")
extra='''  if refill_bytes:
   before_refill=bytes(native[0x8a65b8:0x8a65e8]);count=len(refill_bytes)
   for entry,args in [(0x8c65e4b8,[0x8c8a65b8,0,count,0x8caa1100]),(0x8c10d25c,[0x8ca11000,0x8caa2000,count]),(0x8c65e58a,[0x8c8a65b8,1,0x8caa1100])]:
    for index,value in enumerate(args,4):s.r[index]=value;r[index]=value
    s.pc=entry
    reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
    target_fn=lower.get(entry,completion_helpers.get(entry))
    assert target_fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    nf.fpul=fpul.value
    assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
    if entry==0x8c65e4b8:assert struct.unpack_from('<2I',native,0xaa1100)==(0x8ca11000,count)
   wanted_refill=bytearray(before_refill)
   ready,free,write,read=struct.unpack_from('<4I',before_refill,0xc)
   for off,value in [(0xc,ready+count),(0x10,free-count),(0x14,(write+count)%8192)]:struct.pack_into('<I',wanted_refill,off,value)
   assert native[0x8a65b8:0x8a65e8]==wanted_refill
   assert native[0xa11000:0xa11000+count]==refill_bytes and native[0xa10000:0xa11000]==original_prefix
   refill_updates+=1
'''
s=s.replace('  first_samples=',extra+'  first_samples=',1)
s=s.replace('feed_updates=0','feed_updates=0;refill_updates=0')
s=s.replace('native_audio_consume_decode_verification','native_audio_refill_verification')
s=s.replace('decode_updates_compared=feed_updates*2,','decode_updates_compared=feed_updates*2,refill_updates_compared=refill_updates,')
s=s.replace('Encoded refill/full-file streaming/physical playback/fullgame unverified.', 'Between decode updates, producer reserves up to4096 next original-file bytes through native query mode0,copies staging bytes with native byte move10d25c,and publishes through native commit mode1. Independent descriptor pointer/length,entire48-byte encoded-source ready/free/write-cursor accounting and all refilled bytes;original first prefix preserved. A full4096-byte refill wraps producer cursor at8192-byte ring end. Second decode maintains exact previous sample/history/mirror/consumer checks. Refilled bytes are published but this two-update test does not yet decode beyond the original4096-byte prefix. Repeated refill/full-file streaming/physical playback/fullgame unverified.')
(p/'verify_native_audio_refill.py').write_text(s)
reg=(p/'register_native_audio_consume_decode.py').read_text().replace('native_audio_consume_decode','native_audio_refill').replace('Native audio source construction, output consumption and repeated decoding:', 'Native audio producer refill with output consumption and decoding:')
(p/'register_native_audio_refill.py').write_text(reg)
