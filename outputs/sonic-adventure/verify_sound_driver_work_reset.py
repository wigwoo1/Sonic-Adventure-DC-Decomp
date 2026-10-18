import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_work_reset.dll')).sa_input_work_pool_initialize;fn.argtypes=abi
fill=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_memory_fill;fill.argtypes=abi
cases=0
for pattern,bank,sr0 in itertools.product([0x00,0x55,0xff],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 start=0x8d6938;size=0x134a0
 ram[start-8:start+size+16]=bytes([pattern])*(size+24)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  assert a==0x8c10d31c,a
  return fill(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c668b38
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f)
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert native[start:start+size+12]==bytes(size+12)
 assert native[start-8:start]==bytes([pattern])*8
 assert native[start+size+12:start+size+16]==bytes([pattern])*4
 assert r[0]==0x8c8e9de0 and r[4]==0 and r[1]==0
 cases+=1
(out/'analysis/sound_driver_work_reset_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native16-instruction global work reset668b38 with native memory fill10d31c. Full retail RAM/CPU/FPU/SR/PR comparison; independent zeroed region uses exact retail size0x134a0 at8d6938 plus zero globals8e9dd8/8e9ddc/8e9de0; boundary preservation. Three initial fill bytes,both FPU banks/SR masks. Pointer return8c8e9de0 preserved, not conventional status. Full game unverified.'),indent=2)+'\n')
print(cases,'work reset comparisons passed')
