import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_slot_allocate.dll')).sa_input_work_object_create;fn.argtypes=abi
fill=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_initialize_native.dll')).sa_memory_fill;fill.argtypes=abi
save=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_context_save.dll')).sa_input_context_save
save.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P]+[ctypes.c_uint32]*5
cases=0
for free,bank,sr0 in itertools.product(range(9),[0,0x200000],[0,0x70000063]):
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
 start=0x8d6938;stride=0x2694
 ram[start:start+stride*8]=bytes([0x55])*(stride*8)
 for i in range(8):struct.pack_into('<I',ram,start+i*stride,0 if i==free else 1)
 struct.pack_into('<I',ram,0x8e9de0,0xffffffff)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 @Call
 def call(ctx,a,m,f,sr,r,pr):
  if a==0x8c10d31c:return fill(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert a==0x8c110954,a
  return save(m,f,r,pr[0],fpul.value,0,macl.value,0)
 s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x12345678;s.r[15]=0x8cf00000;s.sr=sr0;s.pr=0x8c000100;s.pc=0x8c668b58
 r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
 reference(ram,s,fpu=f,special={'mach':0,'macl':0,'gbr':0})
 assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf)
 assert (sr.value&0xf0)==(sr0&0xf0)
 assert r[0]==(0x8c000000+start+stride*free if free<8 else 0)
 if free<8:
  slot=start+stride*free
  assert struct.unpack_from('<I',native,slot)[0]==1
  assert struct.unpack_from('<3I',native,slot+0x204)==(0,0x12345678,0)
  assert native[slot+0x210:slot+0x2690]==bytes(0x2480)
  assert struct.unpack_from('<I',native,0x8e9de0)[0]==0
  for i in range(8):
   if i!=free:assert native[start+i*stride:start+(i+1)*stride]==ram[start+i*stride:start+(i+1)*stride]
 else:
  assert struct.unpack_from('<I',native,0x8e9de0)[0]==0xffffffff
 cases+=1
(out/'analysis/sound_driver_slot_allocate_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Native75-instruction eight-slot allocation668b58 with native memory fill and CPU/FPU context save110954, default MACH/MACL/GBR/FPUL zero. Full retail RAM/CPU/FPU/SR/PR comparison. Independent first-free slot0..7 and exhaustion, stride2694, occupancy/owner/zero fields, cleared payload, count wrapFFFFFFFF to0 and interrupt-mask restoration. Both FPU banks/SR masks. Context fixture and full game unverified.'),indent=2)+'\n')
print(cases,'slot allocation comparisons passed')
