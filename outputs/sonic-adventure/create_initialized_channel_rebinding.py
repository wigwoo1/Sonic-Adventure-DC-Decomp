from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_initialized_decode.py').read_text()
s=s.replace("fn=lower[0x8c65b7a8]", "lower[0x8c65b768]=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_work_channel_source_set.dll')),'sa_sound_work_channel_source_set');lower[0x8c65b768].argtypes=abi\nfn=lower[0x8c65b7a8]")
s=s.replace(" work_before=bytes(ram[0xa60000:0xa60100])", " ram[0xad0000:0xad0030]=ram[0xac0000:0xac0030];ram[0xad0100:0xad0130]=ram[0xac0100:0xac0130]\n old_channels=[bytes(ram[a:a+48]) for a in [0xac0000,0xac0100]]\n work_before=bytes(ram[0xa60000:0xa60100])")
needle='  s.r[4]=0x8ca60000;r[4]=s.r[4];s.pc=0x8c65bb88\n'
extra='''  for channel in [0,1]:
   before_binding=bytes(native[0xa60000:0xa60100])
   s.r[4:7]=[0x8ca60000,channel,0x8cad0000+channel*256]
   for index in range(4,7):r[index]=s.r[index]
   s.pc=0x8c65b768
   reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
   assert lower[0x8c65b768](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
   wanted_binding=bytearray(before_binding);struct.pack_into('<I',wanted_binding,12+channel*4,0x8cad0000+channel*256)
   assert native[0xa60000:0xa60100]==wanted_binding
'''
assert s.count(needle)==1
s=s.replace(needle,extra+needle)
needle='  feed_updates+=1'
extra='''  for channel in [0,1]:
   old_address=0xac0000+channel*256;new_address=0xad0000+channel*256
   assert native[old_address:old_address+48]==old_channels[channel]
   wanted_channel=bytearray(old_channels[channel])
   if channel<data[7]:
    for off,value in [(0xc,512),(0x10,88),(0x14,512)]:struct.pack_into('<I',wanted_channel,off,value)
   assert native[new_address:new_address+48]==wanted_channel,(asset['path'],channel,bytes(native[new_address:new_address+48]).hex(),wanted_channel.hex())
'''
s=s.replace(needle,extra+needle)
s=s.replace('initialized_decode_verification','initialized_channel_rebinding_verification')
s=s.replace('Producer/refill/full-file streaming/physical playback/fullgame unverified.', 'Before decoding, native channel source setter65b768 replaces both channel bindings with distinct source objects backed by the same sample buffers. Independent complete work object after each setter; subsequent native input provider and completion consume/publish through replacement objects. All48 bytes of old objects unchanged; independent full replacement objects,512 bytes published,88 available and read cursor512 for active channels,all other bytes preserved. Producer/refill/full-file streaming/physical playback/fullgame unverified.')
(p/'verify_initialized_channel_rebinding.py').write_text(s)
s=(p/'register_initialized_decode.py').read_text()
s=s.replace('base = base.replace("path = root', "names.add('sa_sound_work_channel_source_set')\nbase = base.replace(\"path = root",1)
s=s.replace("base = base.replace('initialize_then_feed', 'initialized_decode')", "base = base.replace('initialize_then_feed', 'initialized_channel_rebinding')")
s=s.replace('Persistent original initialization, feeding and decoding:', 'Persistent initialized decoding with channel source replacement:')
(p/'register_initialized_channel_rebinding.py').write_text(s)
