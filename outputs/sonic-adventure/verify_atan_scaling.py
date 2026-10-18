import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/atan_scaling.dll')).sa_atan_scaling;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];actual=[];expected=[];errors=[];targets=set();count=0
 def effect(mem,r,sr,fp,target,pr):
  b=(fp.fpscr>>21)&1;trace=(target,tuple(r[4:8]),fp.banks[b][4],fp.banks[b][5],r[15],pr);targets.add(target)
  if target==0x8c10fbb8:struct.pack_into('<I',mem,r[4]&0xffffff,bucket)
  elif target in [0x8c10fb94,0x8c10e7b8]:fp.banks[b][0]=0x3e000000
  else:raise AssertionError(hex(target))
  for i,v in enumerate([0x12345678,0x23456789,0x34567890,0x45678901]):r[i]=v
  return trace,sr^0x31
 @Call
 def call(ctx,target,m,fp,sr,r,pr):
  try:trace,sr[0]=effect(native,r,sr[0],fp.contents,target,pr[0]);actual.append(trace)
  except Exception as e:errors.append(str(e));return -9
  return 0
 def hook(t):
  def body(mem,r,sr):trace,sr=effect(mem,r,sr,f,t,s.pr);expected.append(trace);return sr
  return body
 values=[0,0x80000000,1,0x80000001,0x007fffff,0x807fffff,0x00800000,0x80800000,0x3f800000,0xbf800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc00001,0xff800001]
 for value,bucket,bank,rm in itertools.product(values,[0,1,2,0xffffffff,0xfffffffe,126,127,254,255,256,0xffffff01,0x7fffffff,0x80000000],range(2),range(2)):
  for mem in (ram,native):mem[0xefff80:0xf00000]=bytes(128);struct.pack_into('<I',mem,0xa00400,0x12345678)
  f=Fpu();f.fpscr=0x4007c|(bank<<21)|rm;f.fpul=0x12345678
  for b in range(2):f.banks[b][:]=[0x3f800000+i*37 for i in range(16)]
  f.banks[bank][4]=value;f.banks[bank][5]=bucket;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[4]=bucket;s.r[15]=0x8cf00000;s.pc=0x8c10e7b8;s.pr=0x8c000100;s.sr=0x700083f3
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();actual.clear();expected.clear()
  reference(ram,s,{},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert actual==expected and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native,(value,bucket,bank,rm,[(i,hex(r[i]),hex(s.r[i])) for i in range(16) if r[i]!=s.r[i]],hex(sr.value),hex(s.sr),[(b,i,hex(nf.banks[b][i]),hex(f.banks[b][i])) for b in range(2) for i in range(16) if nf.banks[b][i]!=f.banks[b][i]],hex(nf.fpscr),hex(f.fpscr))
  count+=1
 assert not errors,errors
 names=['sa_atan_scaling.c']
 report=dict(cases_passed=count,instruction_count=79,services_visited=[hex(t) for t in sorted(targets)],binary_sha256=hashlib.sha256(binary).hexdigest(),dependency_source_sha256={n:hashlib.sha256((out/'source'/n).read_bytes()).hexdigest() for n in names},dll_sha256=hashlib.sha256((ROOT/'work/sonic-adventure/build/atan_scaling.dll').read_bytes()).hexdigest(),scope='Whole integer bit-field binary32 exponent scaling leaf, no injected services/arithmetic. Signed zeros/subnormal/normal/infinity/NaN raw input patterns, signed scale extremes and overflow/underflow thresholds, both banks/RM0/1. All integer registers/SR/PR/FPU/FPUL/stack/RAM compared against unhooked retail instructions. PR=SZ=0; exact retail flush/overflow behavior preserved.')
 (out/'analysis/atan_scaling_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'atan scaling comparisons passed')
if __name__=='__main__':main()
