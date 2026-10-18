import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_context_initialize.dll')).sa_file_context_initialize;fn.argtypes=abi
handle=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_handle_open_initialize.dll')).sa_file_handle_open_initialize;handle.argtypes=abi
cases=0
for capacity,initialized,unaligned,bank in itertools.product([0,1,3],[0,1],[0,1],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<I',ram,0x673414,0x8ca01000);struct.pack_into('<H',ram,0xa01038,initialized);struct.pack_into('<I',ram,0xb03000,0x8cb10000);struct.pack_into('<I',ram,0xf00000,0x8cb03000)
 events=[];native_events=[]
 results={0x8c645328:0x8cb03000,0x8cb10000:0,0x8c600dd0:0x8cb05000,0x8c5ffd28:0xfffffff5}
 def dispatch(target,m,regs,log):log.append((target,tuple(regs[i] for i in range(4,8))));regs[0]=results[target]
 hooks={}
 for target in results:
  def hook(m,regs,status,target=target):dispatch(target,m,regs,events);return status
  hooks[target]=hook
 @Call
 def call(ctx,target,m,f,status,regs,pr):
  if target==0x8c600464:return handle(m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  dispatch(target,m,regs,native_events);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=capacity;s.r[5]=0x8ca01000+unaligned;s.r[6]=7;s.r[7]=9;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c600038
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks=hooks)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and events==native_events
 assert r[0]==(0 if initialized else 0xfffffff2 if unaligned else 0xfffffff5)
 if initialized or unaligned:assert not events
 else:
  assert [e[0] for e in events]==[0x8c645328,0x8cb10000,0x8c600dd0,0x8c5ffd28]
  assert struct.unpack_from('<I',native,0xa01000)[0]==capacity
  assert struct.unpack_from('<I',native,0xa01018)[0]==0x8ca020d4
  assert struct.unpack_from('<I',native,0xa0103c)[0]==0x8cb03000
  for i in range(capacity+1):assert struct.unpack_from('<I',native,0xa02080+i*84)[0]==0x8ca01000
  assert struct.unpack_from('<H',native,0xa020cc)[0]==1
 cases+=1
report={'cases_passed':cases,'scope':'Native filesystem context and handle-pool setup against retail RAM/CPU/FPU/SR/PR; capacities0/1/3, already initialized, misaligned context and banks. Native reserved-handle initializer connected. Interface acquisition/initialize, directory storage helper and mount tailcall are explicit hooks; no real filesystem mounting.'}
(root/'analysis/file_context_initialize_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'filesystem context initializer comparisons passed')
