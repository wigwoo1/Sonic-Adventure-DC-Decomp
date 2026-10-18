import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_restore_all.dll'))
volume_lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_dispatch.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
volume=volume_lib.sa_sound_volume_dispatch;volume.argtypes=abi
division=volume_lib.sa_signed_division;division.argtypes=abi
control=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_volume_control.dll')).sa_sound_volume_control;control.argtypes=abi
pack=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_low_index.dll')).sa_sound_midi_pack;pack.argtypes=abi
helpers={}
for address,dllname,name in [(0x8c669144,'sound_status_set','sa_sound_status_byte_set'),(0x8c666ad0,'sound_status_set','sa_sound_command_complete'),(0x8c643d88,'sound_command_type5','sa_sound_command_type5'),(0x8c65a378,'sound_request_process','sa_sound_request_process'),(0x8c666ba8,'engine_frame_request_enqueue','sa_engine_frame_request_enqueue'),(0x8c10d25c,'engine_frame_request_enqueue','sa_memory_move'),(0x8c643b9c,'sound_low_index','sa_sound_midi_enqueue'),(0x8c65a464,'sound_request_commit','sa_sound_request_commit'),(0x8c10cd82,'sound_request_commit','sa_sound_request_copy20'),(0x8c10cd44,'engine_frame_initialize_native','sa_engine_record_copy_loaded_dispatch')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dllname}.dll')),name);helper.argtypes=abi;helpers[address]=helper
for address,dllname,name in [(0x8c666a68,'sound_request_process','sa_sound_work_dispatch'),(0x8c643624,'engine_frame_object_configure','sa_engine_frame_object_configure'),(0x8c65a568,'engine_frame_object_configure','sa_engine_object_state_get'),(0x8c65a5cc,'engine_frame_object_configure','sa_engine_object_state_set'),(0x8c65a5ac,'engine_frame_object_configure','sa_engine_object_address_set'),(0x8c65a5d4,'engine_frame_object_configure','sa_engine_object_length_set'),(0x8c65a5b4,'engine_frame_object_configure','sa_engine_object_callback_set')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dllname}.dll')),name);helper.argtypes=abi;helpers[address]=helper
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for bank,answer,sr0 in itertools.product([0,0x200000],[0,123],[0,0x70000063]):
 entry=0x8c094720;name='sa_sound_voice_restore_all'
 fn=getattr(dll,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 mute_global=lit(0x8c094860);struct.pack_into('<I',ram,mute_global,12345)
 final_target=struct.unpack_from('<I',binary,0x094868-0x10000)[0]
 levels=[(-127,0,127,200,-200,17)[i%6] for i in range(38)]
 table=lit(0x8c094864)
 for i,v in enumerate(levels):struct.pack_into("<i",ram,table+4*i,v)
 handles=lit(0x8c093cc0)
 for slot in range(32,38):struct.pack_into('<I',ram,handles+4*(slot-30),0x8ca08000+8*slot)
 for slot in range(32,38):ram[0xa08001+8*slot]=slot+40
 reserved_template_base=lit(0x8c643df0);reserved_template=bytes(ram[reserved_template_base:reserved_template_base+20])
 reserved_target=struct.unpack_from('<I',binary,0x093cc4-0x10000)[0]
 struct.pack_into('<I',ram,lit(0x8c643d18),0x8ca09000)
 devices=lit(0x8c093348);packet=lit(0x8c093344)
 struct.pack_into('<2I',ram,devices,0x8ca07000,0x8ca07008)
 ram[0xa07001]=0;ram[0xa07009]=1
 ram[packet:packet+8]=bytes([0x55])*8
 ring=0xa09000+struct.unpack_from('<h',binary,0x65a4fa-0x10000)[0]
 struct.pack_into('<3I',ram,ring,0x8ca20000,0,0);struct.pack_into('<5I',ram,ring+12,0x8ca10000,256,0,0,0)
 struct.pack_into('<I',ram,0xa09178,0x8ca30000);struct.pack_into('<7I',ram,0xa30000,1,0,0,0,0,0,0)
 request_base=lit(0x8c643c00);template=bytes(ram[request_base:request_base+20])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[];dispatch_inputs=[];packets=[];reference_packets=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c093be0:
   dispatch_inputs.append((a,r[4],r[5]));return volume(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c10ca5c:return division(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c093240:return control(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c643c18:return pack(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c643b9c:packets.append(bytes(m[(r[5]&0xffffff)+i] for i in range(4)))
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c6671c4:m[r[5]&0xffffff]=0
  calls.append((a,r[4],r[5]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):
   if a==0x8c643b9c:reference_packets.append(bytes(m[(r[5]&0xffffff):(r[5]&0xffffff)+4]))
   if a==0x8c6671c4:m[r[5]&0xffffff]=0
   events.append((a,r[4],r[5]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,0x8c093be0,0x8c093240,0x8c10ca5c,0x8c643c18,*helpers)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert len(packets)==32
 for slot,data in enumerate(packets):
  level=levels[slot];expected=min(127,max(0,(level//6 if level>0 else level)+106))
  assert data==bytes([expected,7,slot,3]),(slot,data,expected)
 assert calls==events,(calls,events)
 assert macl.value==special["macl"]==37*28
 assert struct.unpack_from('<3I',native,ring+20)==(38,38,0)
 for slot,data in enumerate(packets):
  expected_request=bytearray(template);struct.pack_into('<I',expected_request,0,0x86);expected_request[6:10]=data
  base=0xa10000+28*slot
  assert native[base:base+20]==expected_request and struct.unpack_from('<I',native,base+20)[0]==1
 assert dispatch_inputs==[(0x8c093be0,levels[i]&0xffffffff,i) for i in range(38)] and calls[-1][0]==final_target
 assert struct.unpack_from('<I',native,mute_global)[0]==0 and r[0]==0
 for slot in range(32,38):
  expected_request=bytearray(reserved_template);struct.pack_into('<I',expected_request,0,5);expected_request[4:8]=bytes([slot+40,(levels[slot]+128)&255,0,0])
  base=0xa10000+28*slot
  assert native[base:base+20]==expected_request and struct.unpack_from('<I',native,base+20)[0]==1
 s.pc=0x8c65a378;s.pr=0x8c000100;pr.value=s.pr
 reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,0x8c093be0,0x8c093240,0x8c10ca5c,0x8c643c18,*helpers)})
 assert helpers[0x8c65a378](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and calls==events
 assert struct.unpack_from('<3I',native,ring+20)==(6,38,32)
 assert struct.unpack_from('<I',native,ring+4)[0]==32
 for slot,data in enumerate(packets):
  expected_request=bytearray(template);struct.pack_into('<I',expected_request,0,0x86);expected_request[6:10]=data
  assert native[0xa20000+16*slot:0xa20010+16*slot]==bytes([0x86,0])+expected_request[4:18]
  assert struct.unpack_from('<I',native,0xa10014+28*slot)[0]==0
 for slot in range(32,38):assert struct.unpack_from('<I',native,0xa10014+28*slot)[0]==1
 assert struct.unpack_from('<I',native,ring+8)[0]==1
 assert struct.unpack_from('<6I',native,0xa30000)==(1,0,0x8ca20000,512,0x8c666ad0,0x8ca09158)
 for next_entry in (0x8c666ad0,0x8c65a378):
  s.pc=next_entry;s.pr=0x8c000100;pr.value=s.pr
  reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,0x8c093be0,0x8c093240,0x8c10ca5c,0x8c643c18,*helpers)})
  assert helpers[next_entry](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and calls==events
  if next_entry==0x8c666ad0:assert struct.unpack_from('<2I',native,ring+4)==(0,0)
 assert struct.unpack_from('<3I',native,ring+20)==(0,38,38)
 assert struct.unpack_from('<2I',native,ring+4)==(6,1)
 assert struct.unpack_from('<6I',native,0xa30000)==(1,0,0x8ca20000,96,0x8c666ad0,0x8ca09158)
 for slot in range(32,38):
  expected_request=bytearray(reserved_template);struct.pack_into('<I',expected_request,0,5);expected_request[4:8]=bytes([slot+40,(levels[slot]+128)&255,0,0])
  i=slot-32
  assert native[0xa20000+16*i:0xa20010+16*i]==bytes([5,0])+expected_request[4:18]
  assert struct.unpack_from('<I',native,0xa10014+28*slot)[0]==0
 cases+=1
(out/'analysis/sound_voice_restore_all_native_second_batch_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='33-instruction native restore-all entry; full retail RAM/CPU/FPU/SR/PR across both banks,SR0/high and service returns0/123. Independent38 volume calls in slot order0..37 with varied signed saved levels,final service target,mute global zero and forced zero return. Volume dispatcher and signed division native; ordinary volume control and MIDI packing native; enqueue, commit and record copies native; reserved type5 builder native; six independently checked reserved queued requests, total38; first32 drained and packed, native completion callback and status setter clear busy/count, second consumer pass packs remaining six reserved requests into96-byte transfer; all38 tags cleared and pending0/write38/read38 verified. Callback invoked explicitly without physical transfer proof; diagnostic logging and final service modeled. Native consumer and packed builder/memory move connected; independent32 packed records in order, cleared tags and drained counters checked; work dispatch, object configure and accessors native; independent busy flag and object512-byte length/buffer/callback/context verified. Readiness and transfer queue consumption modeled. Independent 32 MIDI packets checked; actual restored audio not proven.'),indent=2)+'\n')
print(cases,'voice restore-all native second batch comparisons passed')

