import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_registered_callback.dll')).sa_file_registered_callback;fn.argtypes=abi
cases=0
for gate,result,bank in itertools.product([0,1,2,0xffffffff],[0,0xffffffef],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0xa00020,gate)
 events=[];actual=[]
 def service(regs,log):log.append(tuple(regs[i] for i in range(4,8)));regs[0]=result
 def hook(m,regs,status):service(regs,events);return status
 @Call
 def call(ctx,target,m,f,status,regs,pr):assert target==0x8c6005c2;service(regs,actual);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c5ffcf0
 args=tuple(s.r[i] for i in range(4,8));r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={0x8c6005c2:hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 assert actual==([] if gate==1 else [args]) and r[0]==(1 if gate==1 else result)
 cases+=1
report={'cases_passed':cases,'scope':'Native registered filesystem callback gate with full retail RAM/CPU/FPU/SR/PR equality. Context+20 values0/1/2/ffffffff, backend return0/-17 and banks. Independent preserved tailcall arguments, skip/dispatch and return. Backend polling/completion explicitly modeled.'}
(root/'analysis/file_registered_callback_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'registered filesystem callback comparisons passed')
