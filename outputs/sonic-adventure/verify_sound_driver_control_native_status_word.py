import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
helpers={}
for address,dllname,name in [(0x8c66910c,'sound_status_get','sa_sound_status_word_get'),(0x8c6671c4,'sound_status_get','sa_sound_status_byte_get'),(0x8c666ad0,'sound_status_set','sa_sound_command_complete'),(0x8c669144,'sound_status_set','sa_sound_status_byte_set'),(0x8c666a68,'sound_request_process','sa_sound_work_dispatch'),(0x8c643624,'engine_frame_object_configure','sa_engine_frame_object_configure'),(0x8c65a568,'engine_frame_object_configure','sa_engine_object_state_get'),(0x8c65a5cc,'engine_frame_object_configure','sa_engine_object_state_set'),(0x8c65a5ac,'engine_frame_object_configure','sa_engine_object_address_set'),(0x8c65a5d4,'engine_frame_object_configure','sa_engine_object_length_set'),(0x8c65a5b4,'engine_frame_object_configure','sa_engine_object_callback_set'),(0x8c65a378,'sound_request_process','sa_sound_request_process'),(0x8c666ba8,'engine_frame_request_enqueue','sa_engine_frame_request_enqueue'),(0x8c10d25c,'engine_frame_request_enqueue','sa_memory_move'),(0x8c66b28c,'sound_control_setup_request','sa_sound_control_setup_request'),(0x8c66b304,'sound_control_level_request','sa_sound_control_level_request'),(0x8c65a464,'sound_request_commit','sa_sound_request_commit'),(0x8c10cd82,'sound_request_commit','sa_sound_request_copy20'),(0x8c10cd44,'engine_frame_initialize_native','sa_engine_record_copy_loaded_dispatch'),(0x8c669470,'sound_driver_setup_wrapper','sa_sound_driver_setup_wrapper'),(0x8c669488,'sound_driver_level_wrapper','sa_sound_driver_level_wrapper')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dllname}.dll')),name);helper.argtypes=abi;helpers[address]=helper
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for channels,level,cached,bank,answer in itertools.product([0,1,3],[-43,-42,-1,0,1],[0,7],[0,0x200000],[0,123]):
 sr0=0x70000063;entry=0x8c6682be;handle=0x8ca08000
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_control_6682be.dll')).sa_sound_driver_control_6682be
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0xa08008,channels);struct.pack_into('<I',ram,0xa08028,0x8ca0b000);struct.pack_into('<I',ram,0xa08040,cached)
 setup_target=struct.unpack_from('<I',binary,0x668404-0x10000)[0];level_target=struct.unpack_from('<I',binary,0x66840c-0x10000)[0];error_target=struct.unpack_from('<I',binary,0x668400-0x10000)[0]
 setup_lower=struct.unpack_from('<I',binary,0x669484-0x10000)[0];level_lower=struct.unpack_from('<I',binary,0x669494-0x10000)[0]
 for literal in (0x8c66b2e0,0x8c66b354):struct.pack_into('<I',ram,lit(literal),0x8ca09000)
 ram[0xa0b001]=0x42
 setup_packet=lit(0x8c66b2ec);level_packet=lit(0x8c66b360)
 setup_template=bytes(ram[setup_packet:setup_packet+20]);level_template=bytes(ram[level_packet:level_packet+20])
 ring=0xa09000+struct.unpack_from('<h',binary,0x65a4fa-0x10000)[0]
 struct.pack_into('<3I',ram,ring,0x8ca20000,0,0);struct.pack_into('<5I',ram,ring+12,0x8ca10000,256,256 if answer else 0,0,0)
 struct.pack_into('<I',ram,0xa09178,0x8ca30000);struct.pack_into('<7I',ram,0xa30000,1,0,0,0,0,0,0)
 status_word_target=struct.unpack_from('<I',binary,0x669140-0x10000)[0];ready_word=0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[];request_calls=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a in (setup_lower,level_lower):request_calls.append((a,r[4],r[5],r[6]))
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==status_word_target:
   assert r[6]==4 and r[7]==1
   for i in range(4):m[(r[5]&0xffffff)+i]=(ready_word>>(8*i))&255
  calls.append((a,r[4],r[5],r[6]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):
   if a==status_word_target:
    for i in range(4):m[(r[5]&0xffffff)+i]=(ready_word>>(8*i))&255
   events.append((a,r[4],r[5],r[6]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=7;s.r[6]=level&0xffffffff;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,*helpers)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 converted=-127 if level<-42 else 127 if level>=0 else 2*level-(abs(level)//2)
 wanted=[]
 if cached!=7:wanted.extend((setup_lower,0x8ca0b000,7,127) for i in range(channels))
 wanted.extend((level_lower,0x8ca0b000,ctypes.c_int8(converted).value&0xffffffff,converted&0xffffffff) for i in range(channels))
 assert calls==events and request_calls==wanted
 assert struct.unpack_from('<I',native,0xa08040)[0]==7
 queued=0 if answer else len(wanted)
 assert struct.unpack_from('<3I',native,ring+20)==(256 if answer else queued,queued,0)
 expected_records=[]
 if cached!=7:
  expected=bytearray(setup_template);struct.pack_into('<I',expected,0,0x29)
  off=lit(0x8c66b2f0)-setup_packet;expected[off:off+2]=bytes([0x42,7]);expected[lit(0x8c66b2f4)-setup_packet]=(127+struct.unpack_from('<h',binary,0x66b2de-0x10000)[0])&255
  expected_records.extend([expected]*channels)
 expected=bytearray(level_template);struct.pack_into('<I',expected,0,0x2a);off=lit(0x8c66b364)-level_packet;expected[off:off+2]=bytes([0x42,(converted+struct.unpack_from('<h',binary,0x66b352-0x10000)[0])&255]);expected_records.extend([expected]*channels)
 for i,expected in enumerate(expected_records if not answer else []):
  base=0xa10000+28*i
  assert native[base:base+20]==expected and struct.unpack_from('<I',native,base+20)[0]==1
 if answer:assert native[0xa10000:0xa100a8]==bytes(168)
 assert macl.value==special['macl']
 if not answer:
  ready_word=0x01010101
  queued_before=bytes(native[0xa10000:0xa10000+28*queued])
  s.pc=0x8c65a378;s.pr=0x8c000100;pr.value=s.pr
  reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,*helpers)})
  assert helpers[0x8c65a378](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and calls==events
  assert struct.unpack_from('<3I',native,ring+20)==(queued,queued,0)
  assert struct.unpack_from('<2I',native,ring+4)==(0,0)
  assert native[0xa10000:0xa10000+28*queued]==queued_before
  ready_word=0
 if not answer:
  s.pc=0x8c65a378;s.pr=0x8c000100;pr.value=s.pr
  reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,*helpers)})
  assert helpers[0x8c65a378](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and calls==events
  assert struct.unpack_from('<3I',native,ring+20)==(0,queued,queued)
  padded=(queued+1)&~1
  assert struct.unpack_from('<2I',native,ring+4)==(padded,int(bool(queued)))
  if queued:
   assert struct.unpack_from('<6I',native,0xa30000)==(1,0,0x8ca20000,padded*16,0x8c666ad0,0x8ca09158)
   if padded>queued:assert native[0xa20000+16*queued:0xa20000+16*padded]==bytes(16)
  else:assert struct.unpack_from('<7I',native,0xa30000)==(1,0,0,0,0,0,0)
  for i,expected in enumerate(expected_records):
   assert native[0xa20000+16*i:0xa20010+16*i]==bytes([expected[0],0])+expected[4:18]
   assert struct.unpack_from('<I',native,0xa10014+28*i)[0]==0
 if not answer and queued:
  before=bytes(native[0xa20000:0xa20000+padded*16])
  s.pc=0x8c666ad0;s.pr=0x8c000100;pr.value=s.pr
  reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,*helpers)})
  assert helpers[0x8c666ad0](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and calls==events
  assert struct.unpack_from('<2I',native,ring+4)==(0,0)
  expected_packed=bytearray(before)
  for i in range(padded):expected_packed[16*i]=0
  assert native[0xa20000:0xa20000+padded*16]==expected_packed
  assert struct.unpack_from('<3I',native,ring+20)==(0,queued,queued)
 cases+=1
(out/'analysis/sound_driver_control_native_status_word_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native81-instruction driver control6682be. Channels0/1/3,level threshold-43/-42/-1/0/1,cached control equal/different,banks,lower returns. Full retail state and independent setup-on-change/cache update,integer level conversion,all-channel iteration and error call counts; native setup/level byte wrappers connected and independent lower argument conversion checked; canonical command29/2a builders and commit/copies native; independent command order/records/tags/cursors and full-queue preservation checked; native consumer/packed builder/memory move connected for accepted requests; independent ordered16-byte records,cleared tags,drained cursors/count. Native work dispatch/object configure/accessors connected; independent even padding,busy flag,object buffer/length/callback/context checked. Native completion/status-byte setter connected for nonempty accepted batches; independent opcode cleanup,remaining bytes preservation,busy/count reset and stable drained cursors. Callback invoked explicitly;Native readiness byte getter connected; independent busy pass preserves queued records and cursors before ready pass drains them. Native status-word getter connected; independently checked four-byte/one-item read shape. Lower strided read/physical transfer/error services modeled, precise game meaning/audio unverified.'),indent=2)+'\n')
print(cases,'driver control native status-word comparisons passed')
