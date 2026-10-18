import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_object_configure.dll'))
helpers={}
for a,name in [(0x8c643624,'sa_engine_frame_object_configure'),(0x8c65a568,'sa_engine_object_state_get'),(0x8c65a5cc,'sa_engine_object_state_set'),(0x8c65a5ac,'sa_engine_object_address_set'),(0x8c65a5d4,'sa_engine_object_length_set'),(0x8c65a5b4,'sa_engine_object_callback_set'),(0x8c65acac,'sa_engine_error_record')]:
 helper=getattr(lib,name);helper.argtypes=abi;helpers[a]=helper
addresses=[int(line.split('\t')[0],16) for line in (out/'analysis/ghidra-all/function_index.tsv').read_text().splitlines()[1:] if line.startswith('8c')]

def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for selector,rejection,transfer_answer,bank,sr0 in itertools.product([-1,0,1,2,255,256],['success','context','object','state'],[0,456],[0,0x200000],[0,0x70000063]):
 configure_answer={'success':0,'context':0x0d000001,'object':0x07000103,'state':0x7fffffff}[rejection]
 entry=0x8c6684c4;handle=0x8ca08000
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_block_transfer.dll')).sa_sound_driver_block_transfer
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0xa08028,0x8ca09000);struct.pack_into('<I',ram,0xa08040,0x55)
 clock_target=0x8c60260e;configure_target=0x8c643624;transfer_target=0x8c65a5dc;error_target=0x8c667b7a
 timestamps=lit(0x8c6686d8);struct.pack_into('<3I',ram,timestamps,0xa5a5a5a5,0xa5a5a5a5,0xa5a5a5a5)
 for channel in (-1,0,1,2):struct.pack_into('<I',ram,0xa08018+4*channel,0x8ca10000+64*(channel+1))
 mode=0x10203040;address=0x8ca20000;length=0x1234;struct.pack_into('<I',ram,0xf00000,length)
 target_handle=0x8ca10000+64*(ctypes.c_int8(selector).value+1)
 object_base=target_handle&0xffffff;initial_object=(1,2 if rejection=='state' else 1,0x55,0x66,0x77,0x88,0x99)
 struct.pack_into('<7I',ram,object_base,*initial_object)
 struct.pack_into('<I',ram,0x67c1d0,0 if rejection=='context' else 0x8ca09000)
 if rejection=='object':struct.pack_into('<I',ram,0xa08018+4*ctypes.c_int8(selector).value,0);target_handle=0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[];ticks=[0,0];configure_stack=[];configure_calls=[];reference_stack=[]
 def service(a,r,which):
  if a==clock_target:ticks[which]+=1;return 0x1000*ticks[which]
  if a==configure_target:return configure_answer
  if a==transfer_target:return transfer_answer
  assert a==error_target;return 777
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==configure_target:configure_stack.append(sum(int(m[(r[15]&0xffffff)+j])<<(8*j) for j in range(4)))
  if a==configure_target:configure_calls.append(tuple(r[i] for i in range(4,8)))
  if a in helpers:return helpers[a](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  calls.append((a,r[4],r[5],r[6],r[7]));r[0]=service(a,r,0);return 0
 def hook(a):
  def run(m,r,sr):
   if a==configure_target:reference_stack.append(struct.unpack_from('<I',m,r[15]&0xffffff)[0])
   events.append((a,r[4],r[5],r[6],r[7]));r[0]=service(a,r,1);return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=selector&0xffffffff;s.r[6]=mode;s.r[7]=address;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,*helpers)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert calls==events and configure_stack==[0]
 assert configure_calls==[(target_handle,address,length,0)]
 expected_sequence=[clock_target]
 if configure_answer:expected_sequence.append(error_target)
 else:
  expected_sequence.extend([clock_target,transfer_target])
  if transfer_answer:expected_sequence.append(error_target)
  expected_sequence.append(clock_target)
 assert [x[0] for x in calls]==expected_sequence
 if not configure_answer:assert [x for x in calls if x[0]==transfer_target][0][:3]==(transfer_target,target_handle,mode)
 for x in calls:
  if x[0]==error_target:assert x[1]==0x8c000000+lit(0x8c6686e4 if configure_answer else 0x8c6686ec)
 assert struct.unpack_from('<3I',native,timestamps)==((0x1000,0xa5a5a5a5,0xa5a5a5a5) if configure_answer else (0x1000,0x2000,0x3000))
 assert r[0]==(configure_answer or transfer_answer)
 if configure_answer:
  assert struct.unpack_from('<7I',native,object_base)==initial_object
  assert struct.unpack_from('<2I',native,0x67c1c8)==(configure_target,configure_answer)
 else:assert struct.unpack_from('<5I',native,object_base+4)==(0,address,length,0,0)
 assert macl.value==special['macl']
 cases+=1
(out/'analysis/sound_driver_block_transfer_native_configure_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native69-instruction driver memory-block transfer6684c4. Signed byte selector/truncation,banks/SR,configuration/transfer success/error. Full retail RAM/CPU/FPU/SR/PR/MACL and service traces matched. Independent object selection,configure address/length/zero stack argument,transfer mode,clock/error sequence,timestamp writes/untouched failure timestamps and final result checked. Native object configure/getter/setters/error recorder connected; context/null object/busy rejection and independent successful five-field descriptor verified. Clock/transfer/outer diagnostics modeled; actual transfer unverified.'),indent=2)+'\n')
print(cases,'driver block transfer native configure comparisons passed')
