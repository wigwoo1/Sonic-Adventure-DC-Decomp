import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);dll=ROOT/'work/sonic-adventure/build/saved_position.dll';lib=ctypes.CDLL(str(dll));expected=[];actual=[];errors=[];visited=set();counts={}
 targets=[0x8c04f7e0,0x8c064e80,0x8c08ab20,0x8c08a2bc,0x8c08a198,0x8c07d5aa,0x8c08ab2e]
 def effect(mem,r,sr,fp,target,pr):
  visited.add(target);trace=(target,tuple(r[4:8]),r[15],pr)
  if target==0x8c08ab2e:
   for index,value in zip([4,5,6],[255,128,7]):mem[r[index]&0xffffff]=value
  if target==0x8c064e80:struct.pack_into('<I',mem,0x78c598,variant)
  for i in range(4):r[i]=0x12340000+i
  if target==0x8c04f7e0:r[0]=variant
  elif target==0x8c08a2bc:r[0]=0x80000000 if variant else 7
  return trace,sr^0x31
 @Call
 def call(ctx,target,m,fp,sr,r,pr):
  try:trace,sr[0]=effect(native,r,sr[0],fp.contents,target,pr[0]);actual.append(trace)
  except Exception as ex:errors.append(str(ex));return -9
  return 0
 def hook(t):
  def body(mem,r,sr):trace,sr=effect(mem,r,sr,f,t,s.pr);expected.append(trace);return sr
  return body
 for name,entry in [('sa_saved_position_available',0x8c064e92),('sa_saved_position_load',0x8c064eb2),('sa_saved_position_store',0x8c064ef6)]:
  fn=getattr(lib,name);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
  for variant,stage,alias,mode in itertools.product([0,1,0xffff,0x10001],[0,1,0xffff],[0,1,2,3],[0,0x200000,0x380003]):
   for mem in [ram,native]:
    mem[0xa00000:0xa00400]=bytes(1024);mem[0xefffc0:0xf00000]=bytes(64)
    struct.pack_into('<H',mem,0x78c590,stage);mem[0x78c592:0x78c595]=bytes([255,128,7]);struct.pack_into('<I',mem,0x78c598,0xdeadbeef)
    for off in [0x78c59c,0x78c5a8,0xa00100,0xa00200]:struct.pack_into('<3I',mem,off,0x12345678,0x80000000,0xffffffff)
   f=Fpu();f.fpscr=mode;f.fpul=0x12345678
   for b in range(2):f.banks[b][:]=[0x12340000+i for i in range(16)]
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=[0x8ca00100,0x8c78c59c,0x8c78c5a0,0x8ca00100][alias];s.r[5]=[0x8ca00200,0x8c78c5a8,0x8c78c5ac,0x8ca00100][alias];s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8c000100;s.sr=0x700083f3
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();expected.clear();actual.clear()
   reference(ram,s,{t:hook(t) for t in targets},f)
   assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   nf.fpul=fpul.value
   assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native and actual==expected,(name,variant,stage,alias,mode)
   count+=1
  counts[name]=count
 assert not errors,errors
 report=dict(cases_passed=counts,instruction_count=87,services_visited=[hex(t) for t in sorted(visited)],source_sha256={n:hashlib.sha256((out/'source'/f'{n}.c').read_bytes()).hexdigest() for n in counts},binary_sha256=hashlib.sha256(binary).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Complete availability/load/store bodies, stage low16 comparison, reset branch, signed byte parameters, return-negation/tail services, saved position/rotation ordered copies, overlapping and shared buffers. Full regs/SR/PR/FPU/FPUL/stack/RAM/calls compared against retail instructions across arbitrary FPU mode bits. Subordinate stage/audio/engine services injected; parent integration incomplete.')
 (out/'analysis/saved_position_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'saved position comparisons passed')
if __name__=='__main__':main()
