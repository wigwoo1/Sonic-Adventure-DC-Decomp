import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
stop=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_dispatch.dll')).sa_sound_voice_stop_dispatch
stop.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
note=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_low_index.dll')).sa_sound_low_index;note.argtypes=stop.argtypes
pack=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_low_index.dll')).sa_sound_midi_pack;pack.argtypes=stop.argtypes
helpers={}
for address,dllname,name in [(0x8c643b9c,'sound_low_index','sa_sound_midi_enqueue'),(0x8c643e08,'sound_command_type2','sa_sound_command_type2'),(0x8c65a464,'sound_request_commit','sa_sound_request_commit'),(0x8c10cd82,'sound_request_commit','sa_sound_request_copy20'),(0x8c10cd44,'engine_frame_initialize_native','sa_engine_record_copy_loaded_dispatch')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dllname}.dll')),name);helper.argtypes=stop.argtypes;helpers[address]=helper
for address,dllname,name in [(0x8c669144,'sound_status_set','sa_sound_status_byte_set'),(0x8c666ad0,'sound_status_set','sa_sound_command_complete'),(0x8c65a378,'sound_request_process','sa_sound_request_process'),(0x8c666ba8,'engine_frame_request_enqueue','sa_engine_frame_request_enqueue'),(0x8c10d25c,'engine_frame_request_enqueue','sa_memory_move'),(0x8c643b9c,'sound_low_index','sa_sound_midi_enqueue'),(0x8c65a464,'sound_request_commit','sa_sound_request_commit'),(0x8c10cd82,'sound_request_commit','sa_sound_request_copy20'),(0x8c10cd44,'engine_frame_initialize_native','sa_engine_record_copy_loaded_dispatch')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dllname}.dll')),name);helper.argtypes=stop.argtypes;helpers[address]=helper
for address,dllname,name in [(0x8c666a68,'sound_request_process','sa_sound_work_dispatch'),(0x8c643624,'engine_frame_object_configure','sa_engine_frame_object_configure'),(0x8c65a568,'engine_frame_object_configure','sa_engine_object_state_get'),(0x8c65a5cc,'engine_frame_object_configure','sa_engine_object_state_set'),(0x8c65a5ac,'engine_frame_object_configure','sa_engine_object_address_set'),(0x8c65a5d4,'engine_frame_object_configure','sa_engine_object_length_set'),(0x8c65a5b4,'engine_frame_object_configure','sa_engine_object_callback_set')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dllname}.dll')),name);helper.argtypes=stop.argtypes;helpers[address]=helper
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for bank,answer,sr0 in itertools.product([0,0x200000],[0,123],[0,0x70000063]):
 entry=0x8c094762;name='sa_sound_voice_stop_all'
 fn=getattr(dll,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 table=lit(0x8c09486c);expected_records=[]
 for slot in range(38):
  record=[0x10000000+slot*100+i for i in range(13)];record[1]=slot+1;record[3]=0x1001 if slot%2 else 0x1010
  struct.pack_into('<13I',ram,table+52*slot,*record);record[1]=0;record[3]|=0x10;expected_records.append(struct.pack('<13I',*record))
 handles=lit(0x8c093bb4);reserved_target=struct.unpack_from('<I',binary,0x093bc0-0x10000)[0]
 for slot in range(32,38):struct.pack_into('<I',ram,handles+4*(slot-30),0x8ca08000+slot*8)
 devices=lit(0x8c093448);packet=lit(0x8c093444);notes=lit(0x8c093440)
 struct.pack_into('<2I',ram,devices,0x8ca07000,0x8ca07008);ram[0xa07001]=0;ram[0xa07009]=1
 for slot in range(32):struct.pack_into('<I',ram,notes+4*slot,40+slot)
 struct.pack_into('<I',ram,lit(0x8c643d18),0x8ca09000)
 ram[packet:packet+8]=bytes([0x55])*8
 for slot in range(32,38):ram[0xa08001+slot*8]=slot+40
 struct.pack_into('<I',ram,lit(0x8c643e4c),0x8ca09000)
 ring=0xa09000+struct.unpack_from('<h',binary,0x65a4fa-0x10000)[0]
 struct.pack_into('<3I',ram,ring,0x8ca20000,0,0);struct.pack_into('<5I',ram,ring+12,0x8ca10000,256,0,0,0)
 struct.pack_into('<I',ram,0xa09178,0x8ca30000);struct.pack_into('<7I',ram,0xa30000,1,0,0,0,0,0,0)
 ordinary_template=bytes(ram[lit(0x8c643c00):lit(0x8c643c00)+20]);reserved_template=bytes(ram[lit(0x8c643e58):lit(0x8c643e58)+20])
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[];stop_inputs=[];packets=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c093b70:
   stop_inputs.append(r[4]);return stop(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c0933ba:return note(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c643c18:
   code=pack(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call);packets.append(bytes(m[packet+i] for i in range(4)));return code
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c6671c4:m[r[5]&0xffffff]=0
  calls.append((a,r[4],r[5]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):
   if a==0x8c6671c4:m[r[5]&0xffffff]=0
   events.append((a,r[4],r[5]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,0x8c093b70,0x8c0933ba,0x8c643c18,*helpers)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert calls==events
 assert packets==[bytes([0,40+slot,slot,0]) for slot in range(32)]
 assert stop_inputs==list(range(38))
 assert r[0]==0 and macl.value==special['macl']==37*28
 for slot,record in enumerate(expected_records):assert native[table+52*slot:table+52*(slot+1)]==record
 assert struct.unpack_from('<3I',native,ring+20)==(38,38,0)
 for slot in range(38):
  expected=bytearray(ordinary_template if slot<32 else reserved_template)
  struct.pack_into('<I',expected,0,0x86 if slot<32 else 2)
  if slot<32:expected[6:10]=bytes([0,40+slot,slot,0])
  else:expected[4]=slot+40
  base=0xa10000+28*slot
  assert native[base:base+20]==expected and struct.unpack_from('<I',native,base+20)[0]==1
 for step,next_entry in enumerate((0x8c65a378,0x8c666ad0,0x8c65a378)):
  s.pc=next_entry;s.pr=0x8c000100;pr.value=s.pr
  reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,0x8c093b70,0x8c0933ba,0x8c643c18,*helpers)})
  assert helpers[next_entry](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and calls==events
  if next_entry==0x8c65a378:
   count=struct.unpack_from('<I',native,ring+4)[0]
   first=0 if count==32 else 32
   assert count==(32 if step==0 else 6)
   assert struct.unpack_from('<6I',native,0xa30000)==(1,0,0x8ca20000,count*16,0x8c666ad0,0x8ca09158)
   for i in range(count):
    slot=first+i;expected=bytearray(ordinary_template if slot<32 else reserved_template);struct.pack_into('<I',expected,0,0x86 if slot<32 else 2)
    if slot<32:expected[6:10]=bytes([0,40+slot,slot,0])
    else:expected[4]=slot+40
    assert native[0xa20000+16*i:0xa20010+16*i]==bytes([0x86 if slot<32 else 2,0])+expected[4:18]
  else:assert struct.unpack_from('<2I',native,ring+4)==(0,0)
 assert struct.unpack_from('<3I',native,ring+20)==(0,38,38)
 for slot in range(38):assert struct.unpack_from('<I',native,0xa10014+28*slot)[0]==0
 cases+=1
(out/'analysis/sound_voice_stop_all_native_batches_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='44-instruction native stop-all entry, both FPU banks/SR variants/service returns. Full retail RAM/CPU/FPU/SR/PR/MACL comparison and independently checked38 record lifetimes cleared, stop flag set, other record bytes preserved, slot-order stop calls and forced zero return. Stop dispatcher native; ordinary note-off and MIDI pack native; independently checked32 remembered-note packets; MIDI enqueue, reserved type2 builder and commit/copies native; independently verified all38 queued requests, tags and pending/write/read counters. Consumer/packed builder/memory move/work dispatcher/object configuration/completion callback/status setter native. Independently checks32+6 packed records,512/96-byte objects, cleared tags and drained cursors; completion explicitly invoked. Readiness, physical transfer consumption and diagnostics modeled; actual stopping audio unverified.'),indent=2)+'\n')
print(cases,'voice stop-all native batch comparisons passed')
