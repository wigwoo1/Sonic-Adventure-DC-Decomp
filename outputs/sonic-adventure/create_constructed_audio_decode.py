import re
from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_initialized_decode.py').read_text()
begin=s.index(' struct.pack_into(\'<I\',ram,state+4,0)');end=s.index(' native=bytearray(ram)',begin)
fixture=''' ram[0xa40000:0xa40300]=bytes([0x5a])*768;ram[0xa50000:0xa50300]=bytes([0x5a])*768
 struct.pack_into('<I',ram,0xa90024,0x8c65e462)
 struct.pack_into('<12I',ram,0xa80000,0x8ca90000,1,0,4096,0,0,0,0x8ca10000,8192,0,0,0)
 for off,address in [(0x18,0x8c65e4b8),(0x1c,0x8c65e644),(0x20,0x8c65e58a)]:struct.pack_into('<I',ram,0xa90000+off,address)
 for address,base in [(0xac0000,0x8ca40008),(0xac0100,0x8ca50008)]:struct.pack_into('<12I',ram,address,0x8ca90000,1,0,0,600,0,0,base,1024,64512,0,0)
 struct.pack_into('<2I',ram,0xab0000,0x8cac0000,0x8cac0100)
 object_before=bytes(ram[0xa80000:0xa80030])
'''
s=s[:begin]+fixture+s[end:]
loads='''for address,stem in [(0x8c65b5e8,'sound_work_pool_initialize'),(0x8c667370,'sound_stream_pool_initialize'),(0x8c644b64,'file_decode_pool_initialize'),(0x8c65b624,'sound_work_create'),(0x8c65b606,'sound_work_reset'),(0x8c65e798,'sound_source_buffer_get'),(0x8c65e79c,'sound_source_ring_size_get'),(0x8c65e7a0,'sound_source_mirror_size_get'),(0x8c6673b4,'sound_stream_create'),(0x8c644b6e,'file_decode_handle_allocate'),(0x8c65b778,'sound_work_start'),(0x8c65b774,'sound_work_sample_limit_set'),(0x8c65bc1c,'sound_work_remaining_limit_set'),(0x8c66768c,'sound_stream_input_callback_set')]:
 lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),'sa_'+stem);lower[address].argtypes=abi
'''
s=s.replace("assets=json.loads(",loads+"assets=json.loads(",1)
needle=" special={'macl':0};reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)"
sequence=''' special={'macl':0}
 for entry,args in [(0x8c65b5e8,[]),(0x8c65b624,[0x8ca80000,2,0x8cab0000]),(0x8c65b778,[0x8ca60000]),(0x8c65b774,[0x8ca60000,256]),(0x8c65bc1c,[0x8ca60000,256])]:
  for index,value in enumerate(args,4):s.r[index]=value;r[index]=value
  s.pc=entry
  reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
  assert lower[entry](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
  if entry==0x8c65b5e8:
   assert native[0x8a3910:0x8a3d90]==bytes(1152) and native[0x8d61ac:0x8d670c]==bytes(1376) and native[0x890680:0x890820]==bytes(416)
  if entry==0x8c65b624:
   assert r[0]==0x8ca60000 and struct.unpack_from('<3I',native,0xa60004)==(0x8ca20000,0x8ca80000,0x8cac0000)
   assert struct.unpack_from('<I',native,0xa60010)[0]==0x8cac0100
   assert struct.unpack_from('<4I',native,state+0x38)==(2,0x8ca40008,512,32768)
 work_before=bytes(native[0xa60000:0xa60100]);stream_before=bytes(native[state:state+256]);file_before=bytes(native[0xa30000:0xa30040])
 s.r[4:7]=[0x8ca60000,0x8ca10000,size]
 for index in range(4,7):r[index]=s.r[index]
 s.pc=0x8c65bb88
 reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)'''
assert needle in s;s=s.replace(needle,sequence)
s=s.replace('target=bytearray([0x55])*256','target=bytearray(stream_before)')
s=s.replace('file_target=bytearray([0x55])*64','file_target=bytearray(file_before)')
s=s.replace('0x8ca40008,256,32768','0x8ca40008,512,32768')
def relocate(match):
 value=int(match.group(),16)
 for source,target in [(0xa20000,0x8d61ac),(0x8ca20000,0x8c8d61ac),(0xa30000,0x890680),(0x8ca30000,0x8c890680),(0xa60000,0x8a3910),(0x8ca60000,0x8c8a3910)]:
  if source<=value<=source+256:return hex(target+value-source)
 return match.group()
s=re.sub(r'0x[0-9a-f]+',relocate,s)
s=s.replace('initialized_decode_verification','constructed_audio_decode_verification')
s=s.replace('Producer/refill/full-file streaming/physical playback/fullgame unverified.', 'Objects originate from native pool initializer65b5e8 and work constructor65b624 with native stream/handle allocation and source accessors. Native work start and limit setters configure256 samples; constructor derives512-sample output capacity and32768-sample channel stride from1024-byte ring and64512-byte mirror. Initialization,construction,configuration,header processing,feeding and decoding preserve one RAM/CPU/FPU state without replacing constructed objects. Independent initial zero pools,returned work pointer,channel/source/stream bindings and derived capacity/stride. Producer/refill/full-file streaming/physical playback/fullgame unverified.')
(p/'verify_constructed_audio_decode.py').write_text(s)
reg=(p/'register_initialized_channel_rebinding.py').read_text().replace('initialized_channel_rebinding','constructed_audio_decode')
reg=reg.replace("names.add('sa_sound_work_channel_source_set')", "names.update({'sa_sound_work_pool_initialize','sa_sound_stream_pool_initialize','sa_file_decode_pool_initialize','sa_sound_work_create','sa_sound_work_reset','sa_sound_source_ring_size_get','sa_sound_source_mirror_size_get','sa_sound_stream_create','sa_file_decode_handle_allocate','sa_sound_work_start','sa_sound_work_sample_limit_set','sa_sound_work_remaining_limit_set'})")
reg=reg.replace('Persistent initialized decoding with channel source replacement:', 'Native audio construction through original-header decoding:')
(p/'register_constructed_audio_decode.py').write_text(reg)
