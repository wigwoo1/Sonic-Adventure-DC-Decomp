import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_completion_process.dll')).sa_file_completion_process;fn.argtypes=abi
cases=0
for gate,result,bank in itertools.product([0,1,2,0xffffffff],[0,0xffffffef],[0,0x200000]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0xa00020,gate)
 events=[];actual=[]
 for o,v in [(0x673414,0x8ca00000),(0xa0000c,0x8ca02000),(0xa02010,0x8cb10000)]:struct.pack_into('<I',ram,o,v)
 def service(regs,log):log.append(tuple(regs[i] for i in range(4,8)));regs[0]=result
 def hook(m,regs,status):service(regs,events);return status
 @Call
 def call(ctx,target,m,f,status,regs,pr):assert target==0x8cb10000;service(regs,actual);return 0
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca00000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c5ffa12
 args=tuple(s.r[i] for i in range(4,8));r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,hooks={0x8cb10000:hook})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and actual==events
 assert len(actual)==1 and actual[0][0]==0 and actual[0][1]==0x8cefffd8 and r[0]==result
 assert struct.unpack_from('<I',native,0xa00020)[0]==0
 cases+=1
report={'cases_passed':cases,'scope':'Native completion processor no-active-handle branch with full retail RAM/CPU/FPU/SR/PR equality. Context+20 values0/1/2/ffffffff, backend return0/-17 and banks. Independent zero request token, output pointer, processing flag reset and returned device status. Device status callback modeled; active-handle transitions pending.'}
(root/'analysis/file_completion_process_no_handle_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'completion processor no-handle comparisons passed')
