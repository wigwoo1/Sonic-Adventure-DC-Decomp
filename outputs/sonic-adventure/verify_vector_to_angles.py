import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/vector_to_angles.dll')).sa_vector_to_angles;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 events=[];actual=[];errors=[]
 def effect(mem,r,sr,fp,target,pr):
  bank=(fp.fpscr>>21)&1;trace=(target,tuple(r[4:8]),fp.banks[bank][4],fp.banks[bank][5],r[15],pr)
  if target not in [0x8c63a9c4,0x8c10d0f8]:raise AssertionError(hex(target))
  if mutation:fp.fpscr^=0x200000
  bank=(fp.fpscr>>21)&1;fp.banks[bank][0]=0x3f800000 if target==0x8c63a9c4 else answer
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
 vectors=[(0,0,0),(0x3f800000,0,0),(0xbf800000,0x3f000000,0x3f800000),(0x00800000,0x80000000,0x80800000)]
 for xyz,second,alias,bank,rm,mutation,answer in itertools.product(vectors,range(2),range(3),range(2),range(2),range(2),[0,0x3e800000,0xbf000000]):
  if alias and answer==0xbf000000:continue # Aliased negative integer angles become NaN float inputs, outside scalar model.
  source=0x8ca00400;other=0x8ca00500 if second else 0;dest=[0x8ca00600,source,other or source][alias]
  for mem in (ram,native):
   mem[0xa00400:0xa00700]=bytes(768);mem[0xefff80:0xf00000]=bytes(128)
   struct.pack_into('<3I',mem,source&0xffffff,*xyz);struct.pack_into('<3I',mem,0xa00500,0x3f000000,0xbf800000,0x3f800000)
  f=Fpu();f.fpscr=0x4007c|(bank<<21)|rm;f.fpul=0xdeadbeef
  for b in range(2):f.banks[b][:]=[0x3f800000+i*37 for i in range(16)]
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[4:7]=[source,other,dest];s.r[15]=0x8cf00000;s.pc=0x8c056860;s.pr=0x8c000100;s.sr=0x700083f3
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();events.clear();actual.clear()
  reference(ram,s,{t:hook(t) for t in [0x8c63a9c4,0x8c10d0f8]},f)
  status=fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
  assert status==0,(status,xyz,second,alias,bank,rm,mutation,answer,errors);nf.fpul=fpul.value
  assert events==actual and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native,(xyz,second,alias,bank,rm,mutation,answer)
  count+=1
 assert not errors,errors
 names=['sa_vector_to_angles.c','sh4_multiply.c','sh4_fmac.c','sh4_truncate_i32.c']
 report=dict(cases_passed=count,instruction_count=99,binary_sha256=hashlib.sha256(binary).hexdigest(),dependency_source_sha256={n:hashlib.sha256((out/'source'/n).read_bytes()).hexdigest() for n in names},dll_sha256=hashlib.sha256((ROOT/'work/sonic-adventure/build/vector_to_angles.dll').read_bytes()).hexdigest(),scope='Whole vector-to-angle body, native multiply/FMAC/FTRC, optional relative vector, aliased output/input buffers restricted to finite subsequent float inputs, both banks/RM0/1, callback bank mutation and signed angle outputs. Full registers/SR/PR/FPU/FPUL/stack/RAM/callback input/order compared against retail body. Sqrt and atan2 helper bodies injected; finite PR=SZ=0 scalar scope.')
 (out/'analysis/vector_to_angles_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'vector-to-angle comparisons passed')
if __name__=='__main__':main()
