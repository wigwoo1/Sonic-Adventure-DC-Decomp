import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
helpers={}
for address,dllname,name in [(0x8c666ad0,'sound_status_set','sa_sound_command_complete'),(0x8c669144,'sound_status_set','sa_sound_status_byte_set'),(0x8c666a68,'sound_request_process','sa_sound_work_dispatch'),(0x8c643624,'engine_frame_object_configure','sa_engine_frame_object_configure'),(0x8c65a568,'engine_frame_object_configure','sa_engine_object_state_get'),(0x8c65a5cc,'engine_frame_object_configure','sa_engine_object_state_set'),(0x8c65a5ac,'engine_frame_object_configure','sa_engine_object_address_set'),(0x8c65a5d4,'engine_frame_object_configure','sa_engine_object_length_set'),(0x8c65a5b4,'engine_frame_object_configure','sa_engine_object_callback_set'),(0x8c65a378,'sound_request_process','sa_sound_request_process'),(0x8c666ba8,'engine_frame_request_enqueue','sa_engine_frame_request_enqueue'),(0x8c10d25c,'engine_frame_request_enqueue','sa_memory_move'),(0x8c10ca5c,'sound_volume_dispatch','sa_signed_division'),(0x8c669498,'sound_driver_pan_wrapper','sa_sound_driver_pan_wrapper'),(0x8c66b374,'sound_pan_request','sa_sound_pan_request'),(0x8c65a464,'sound_request_commit','sa_sound_request_commit'),(0x8c10cd82,'sound_request_commit','sa_sound_request_copy20'),(0x8c10cd44,'engine_frame_initialize_native','sa_engine_record_copy_loaded_dispatch')]:
 helper=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dllname}.dll')),name);helper.argtypes=abi;helpers[address]=helper
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]

def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for channels,selector,level,bank,capacity in itertools.product([1,2],[0,1,2,3],[-16,-15,-1,0,1,15,16],[0,0x200000],[0,123]):
 if channels==1 and selector==1:continue
 answer=0
 sr0=0x70000063;entry=0x8c66837c
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_pan.dll')).sa_sound_driver_pan
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,lit(0x8c66b46c),0x8ca09000);ram[0xa0b001]=0x42
 struct.pack_into('<I',ram,0xa08028,0x8ca0b000);struct.pack_into('<I',ram,0xa08040,0x55)
 voice=0xa09000+struct.unpack_from('<h',binary,0x66b466-0x10000)[0]+0x42*44
 ram[voice+5]=channels
 target=0x8c65a464;error_target=0x8c65acac
 packet=lit(0x8c66b478);expected=bytearray(ram[packet:packet+20]);struct.pack_into('<I',expected,0,0x26)
 expected[4]=ram[lit(0x8c66b480)+(selector&3)]|0x42
 converted=(abs(127*level)//15)*(-1 if level<0 else 1);signed_level=ctypes.c_int8(converted).value;bias=struct.unpack_from('<h',binary,0x66b468-0x10000)[0]
 auxiliary=0;levels=[signed_level]*channels
 if channels==2:
  if selector==0:levels[0]+=bias
  if selector==1:levels[1]=min(levels[1]-bias,-2)
  levels=[abs(v)//2*(-1 if v<0 else 1) for v in levels]
 levels=[v+bias for v in levels]
 if selector in (0,1):
  for i in range(6):expected[5+i+(i//3)]=0
  off=5+4*selector
  expected[off:off+3]=bytes([levels[selector]&255,auxiliary&255,(auxiliary>>8)&255])
 ring=0xa09000+struct.unpack_from('<h',binary,0x65a4fa-0x10000)[0]
 struct.pack_into('<3I',ram,ring,0x8ca20000,0,0);struct.pack_into('<5I',ram,ring+12,0x8ca10000,256,256 if capacity else 0,0,0)
 struct.pack_into('<I',ram,0xa09178,0x8ca30000);struct.pack_into('<7I',ram,0xa30000,1,0,0,0,0,0,0)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[];canonical_calls=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c66b374:canonical_calls.append(tuple(r[i] for i in range(4,8)))
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c6671c4:m[r[5]&0xffffff]=0
  calls.append((a,r[4],r[5]));r[0]=answer;return 0
 def hook(a):
  def run(m,r,sr):
   if a==0x8c6671c4:m[r[5]&0xffffff]=0
   events.append((a,r[4],r[5]));r[0]=answer;return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca08000;s.r[5]=selector&0xffffffff;s.r[6]=level&0xffffffff;s.r[7]=auxiliary;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,*helpers)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert calls==events and macl.value==special['macl']
 assert canonical_calls==[(0x8ca0b000,selector,converted&0xffffffff,0)]
 assert struct.unpack_from('<I',native,0xa08040)[0]==0x55
 assert native[packet:packet+20]==expected
 assert struct.unpack_from('<i',native,voice+28+4*selector)[0]==signed_level
 assert struct.unpack_from('<3I',native,ring+20)==((256,0,0) if capacity else (1,1,0))
 assert struct.unpack_from('<I',native,0xa10014)[0]==(0 if capacity else 1)
 if not capacity:assert native[0xa10000:0xa10014]==expected
 else:assert native[0xa10000:0xa1001c]==bytes(28)
 if not capacity:
  s.pc=0x8c65a378;s.pr=0x8c000100;pr.value=s.pr
  reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,*helpers)})
  assert helpers[0x8c65a378](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and calls==events
  assert struct.unpack_from('<3I',native,ring+20)==(0,1,1)
  assert struct.unpack_from('<I',native,0xa10014)[0]==0
  assert struct.unpack_from('<2I',native,ring+4)==(2,1)
  assert struct.unpack_from('<6I',native,0xa30000)==(1,0,0x8ca20000,32,0x8c666ad0,0x8ca09158)
  assert native[0xa20010:0xa20020]==bytes(16)
  assert native[0xa20000:0xa20010]==bytes([0x26,0])+expected[4:18]
  before=bytes(native[0xa20000:0xa20020])
  s.pc=0x8c666ad0;s.pr=0x8c000100;pr.value=s.pr
  reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,*helpers)})
  assert helpers[0x8c666ad0](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl'] and calls==events
  assert struct.unpack_from('<2I',native,ring+4)==(0,0)
  assert native[0xa20000:0xa20020]==bytes([0])+before[1:]
  assert struct.unpack_from('<3I',native,ring+20)==(0,1,1)
 cases+=1
(out/'analysis/sound_driver_pan_native_completion_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native33-instruction driver caller, division and three-instruction wrapper connected; independently verified integer conversion and canonical input arguments. Native165-instruction pan command26 builder. One/two-channel state,selectors0/1/2/3,signed byte boundaries,banks,commit returns; single-channel selector1 excluded because it reads an uninitialized second stack word. Full RAM/CPU/FPU/SR/PR/MACL and traces match retail execution. Independent voice state update and entire20-byte packet including stereo conversion,lookup identifier,auxiliary bytes and untouched bytes checked. Native commit/loaded copy/copy20 connected; empty/full256-entry queue, independent stored record/tag/cursors and rejected pool preservation checked. Native consumer/packed builder/memory move connected for accepted requests; independent packed16-byte command, cleared tag, drained count/read/write checked. Native work dispatch/object configure/accessors and completion/status-byte setter connected. Independent even padding/object buffer,length,callback/context,busy/count reset and opcode cleanup with remaining bytes preserved checked. Callback explicitly invoked; readiness/physical transfer/error services modeled; real audio unverified.'),indent=2)+'\n')
print(cases,'driver pan native completion comparisons passed')
