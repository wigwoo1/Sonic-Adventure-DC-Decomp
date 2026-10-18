import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_voice_stop_all.dll'))
def lit(a):return struct.unpack_from('<I',binary,a-0x8c010000)[0]&0xffffff
cases=0
for selector,configure_answer,transfer_answer,bank,sr0 in itertools.product([-1,0,1,2,255,256],[0,123],[0,456],[0,0x200000],[0,0x70000063]):
 entry=0x8c6684c4;handle=0x8ca08000
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_block_transfer.dll')).sa_sound_driver_block_transfer
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0xa08028,0x8ca09000);struct.pack_into('<I',ram,0xa08040,0x55)
 clock_target=0x8c60260e;configure_target=0x8c643624;transfer_target=0x8c65a5dc;error_target=0x8c667b7a
 timestamps=lit(0x8c6686d8);struct.pack_into('<3I',ram,timestamps,0xa5a5a5a5,0xa5a5a5a5,0xa5a5a5a5)
 for channel in (-1,0,1,2):struct.pack_into('<I',ram,0xa08018+4*channel,0x8ca10000+64*(channel+1))
 mode=0x10203040;address=0x8ca20000;length=0x1234;struct.pack_into('<I',ram,0xf00000,length)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 calls=[];events=[];ticks=[0,0];configure_stack=[];reference_stack=[]
 def service(a,r,which):
  if a==clock_target:ticks[which]+=1;return 0x1000*ticks[which]
  if a==configure_target:return configure_answer
  if a==transfer_target:return transfer_answer
  assert a==error_target;return 777
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==configure_target:configure_stack.append(sum(int(m[(r[15]&0xffffff)+j])<<(8*j) for j in range(4)))
  calls.append((a,r[4],r[5],r[6],r[7]));r[0]=service(a,r,0);return 0
 def hook(a):
  def run(m,r,sr):
   if a==configure_target:reference_stack.append(struct.unpack_from('<I',m,r[15]&0xffffff)[0])
   events.append((a,r[4],r[5],r[6],r[7]));r[0]=service(a,r,1);return sr
  return run
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[5]=selector&0xffffffff;s.r[6]=mode;s.r[7]=address;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in (clock_target,configure_target,transfer_target,error_target)})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 target_handle=0x8ca10000+64*(ctypes.c_int8(selector).value+1)
 assert calls==events and configure_stack==reference_stack==[0]
 assert [x for x in calls if x[0]==configure_target]==[(configure_target,target_handle,address,length,0)]
 expected_sequence=[clock_target,configure_target]
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
 assert macl.value==special['macl']
 cases+=1
(out/'analysis/sound_driver_block_transfer_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native69-instruction driver memory-block transfer6684c4. Signed byte selector/truncation,banks/SR,configuration/transfer success/error. Full retail RAM/CPU/FPU/SR/PR/MACL and service traces matched. Independent object selection,configure address/length/zero stack argument,transfer mode,clock/error sequence,timestamp writes/untouched failure timestamps and final result checked. Clock/configure/transfer/diagnostics modeled; actual transfer unverified.'),indent=2)+'\n')
print(cases,'driver block transfer comparisons passed')
