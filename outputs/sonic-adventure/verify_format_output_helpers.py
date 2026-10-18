import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/format_output_helpers.dll'))
cases=0
for (entry,name),mode,flag,bank in itertools.product([(0x8c10e566,'sa_format_finish'),(0x8c10e576,'sa_format_ready')],[0,1,2,0xffffffff],[0,0x40,0xff],[0,0x200000]):
 fn=getattr(dll,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 struct.pack_into('<2I',ram,0xa02000,mode,0x8ca01000);ram[0xa01000:0xa01020]=bytes([0x55])*32;ram[0xa01010]=flag
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
 @Call
 def call(ctx,a,m,f,sr,r,pr):errors.append(hex(a));return -9
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca02000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
 assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 if entry==0x8c10e566:assert native[0xa01000]==(0 if mode==1 else 0x55) and native[0xa01001:0xa01010]==bytes([0x55])*15
 else:assert r[0]==(0 if mode==1 else flag&0x40)
 cases+=1
(out/'analysis/format_output_helpers_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native finish8/readiness11 instructions; full retail RAM/CPU/FPU/SR/PR state comparisons, modes0/1/2/ffffffff, flags0/40/ff, both FPU banks. Independent terminator write/neighbor guards and readiness bit checked. Synthetic descriptors; actual FILE I/O not tested.'),indent=2)+'\n')
print(cases,'format output helper comparisons passed')
