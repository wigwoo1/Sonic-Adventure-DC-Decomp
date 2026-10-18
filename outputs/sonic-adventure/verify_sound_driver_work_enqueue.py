import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_work_enqueue.dll')).sa_input_work_create;fn.argtypes=abi
fill=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_work_enqueue.dll')).sa_memory_fill;fill.argtypes=abi
save=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_context_save.dll')).sa_input_context_save
save.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P]+[ctypes.c_uint32]*5
move=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_work_enqueue.dll')).sa_memory_move;move.argtypes=abi
cases=0
for free,reject,size,bank,sr0 in itertools.product([0,1,7,31],[False,True],[0,4,17],[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 start=0xa08000;stride=0x124
 ram[start:start+0x2694]=bytes([0x55])*0x2694
 struct.pack_into('<3I',ram,start+0x204,0xabcdef01,1,1 if reject else 0)
 for i in range(32):struct.pack_into('<I',ram,start+0x210+i*stride+12,0 if i==free else 1)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c10d31c:return fill(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if a==0x8c10d25c:return move(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a==0x8c110954,a
  return save(m,f,r,pr[0],fpul.value,0,macl.value,0)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca08000;s.r[5]=0x12345678;s.r[6]=0x8ca50000;s.r[7]=size;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c668bee
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,special={'mach':0,'macl':0,'gbr':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert r[0]==(0xffffffff if reject else free)
 if not reject:
  job=start+0x210+stride*free
  assert struct.unpack_from('<6I',native,job)==(0x8ca50000,size//4,0,1,0,0xabcdef01)
  assert struct.unpack_from('<I',native,job+0x18)[0]==(0x8ca50000+((size//4-1)*4))&0xffffffff
  assert struct.unpack_from('<I',native,job+0x20)[0]==0x12345678
  assert struct.unpack_from('<2I',native,job+0x118)==(0,0)
 cases+=1
(out/'analysis/sound_driver_work_enqueue_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native87-instruction work entry setup668bee with native fill/move/context save. Full retail RAM/CPU/FPU/SR/PR comparisons. Free entries0/1/7/31, equality rejection, sizes0/4/17,both FPU banks/SR masks. Independent entry header, word-count truncation, saved stack pointer wrap, callback and tail zeros. Special registers fixture zero; full game unverified.'),indent=2)+'\n')
print(cases,'work entry comparisons passed')
