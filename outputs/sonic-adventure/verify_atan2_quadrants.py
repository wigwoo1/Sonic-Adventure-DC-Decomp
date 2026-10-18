import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/atan2_quadrants.dll')).sa_atan2_quadrants;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 events=[];actual=[];errors=[]
 def effect(mem,r,sr,fp,target,pr):
  bank=(fp.fpscr>>21)&1;trace=(target,tuple(r[4:8]),fp.banks[bank][4],fp.banks[bank][5],r[15],pr)
  if target not in [0x8c10ef84]:raise AssertionError(hex(target))
  if mutation:fp.fpscr^=0x200000
  bank=(fp.fpscr>>21)&1;fp.banks[bank][0]=answer
  for i,v in enumerate([0x12345678,0x23456789,0x34567890,0x45678901]):r[i]=v
  return trace,sr^0x31
 @Call
 def call(ctx,target,m,fp,sr,r,pr):
  try:trace,sr[0]=effect(native,r,sr[0],fp.contents,target,pr[0]);events.append(trace)
  except Exception as e:errors.append(str(e));return -9
  return 0
 def hook(t):
  def body(mem,r,sr):trace,sr=effect(mem,r,sr,f,t,s.pr);actual.append(trace);return sr
  return body
 count=0
 vectors=[(0,0,0),(0x3f800000,0,0),(0xbf800000,0x3f000000,0x3f800000),(0xbf800000,0x80000000,0xbf000000),(0x3f800000,0,0xbf800000)]
 for xyz,second,alias,bank,rm,mutation,answer in itertools.product(vectors,range(2),range(3),range(2),range(2),range(2),[0,0x3e800000,0xbf000000]):
  if alias and answer==0xbf000000:continue # Aliased negative integer angles become NaN float inputs, outside scalar model.
  source=0x8ca00400;other=0x8ca00500 if second else 0;dest=[0x8ca00600,source,other or source][alias]
  for mem in (ram,native):
   struct.pack_into('<I',mem,0x7ac108,0x12345678);mem[0xa00400:0xa00700]=bytes(768);mem[0xefff80:0xf00000]=bytes(128)
   struct.pack_into('<3I',mem,source&0xffffff,*xyz);struct.pack_into('<3I',mem,0xa00500,0x3f000000,0xbf800000,0x3f800000)
  f=Fpu();f.fpscr=0x4007c|(bank<<21)|rm;f.fpul=0xdeadbeef
  for b in range(2):f.banks[b][:]=[0x3f800000+i*37 for i in range(16)]
  f.banks[bank][4]=xyz[0];f.banks[bank][5]=xyz[2] if xyz[2]&0x7fffffff else 0x3f800000
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[4:7]=[source,other,dest];s.r[15]=0x8cf00000;s.pc=0x8c10e6a0;s.pr=0x8c000100;s.sr=0x700083f3
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();events.clear();actual.clear()
  reference(ram,s,{t:hook(t) for t in [0x8c10ef84]},f)
  status=fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert status==0,(status,xyz,second,alias,bank,rm,mutation,answer,errors);nf.fpul=fpul.value
  assert events==actual and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native,(xyz,second,alias,bank,rm,mutation,answer)
  count+=1
 assert not errors,errors
 names=['sa_atan2_quadrants.c','sh4_divide.c','sh4_add_sub.c']
 report=dict(cases_passed=count,instruction_count=35,binary_sha256=hashlib.sha256(binary).hexdigest(),dependency_source_sha256={n:hashlib.sha256((out/'source'/n).read_bytes()).hexdigest() for n in names},dll_sha256=hashlib.sha256((ROOT/'work/sonic-adventure/build/atan2_quadrants.dll').read_bytes()).hexdigest(),scope='Whole atan2 quotient/quadrant adjustment body with native FDIV/FADD; atan implementation8c10ef84 injected. Nonzero finite denominators, positive/negative/zero numerator, quadrant correction, both banks/RM0/1 and callback bank changes. Full registers/SR/PR/FPU/FPUL/stack/RAM/call arguments compared. Zero denominator/nonfinite division and numerical atan core incomplete.')
 (out/'analysis/atan2_quadrants_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'atan2 quadrant comparisons passed')
if __name__=='__main__':main()
