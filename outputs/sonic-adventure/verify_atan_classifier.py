import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/atan_classifier.dll')).sa_atan_classifier;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];actual=[];expected=[];errors=[];targets=set();count=0
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
 values=[0,0x80000000,0x3e800000,0xbe800000,0x3f400000,0xbf400000,0x3f800000,0xbf800000,0x41800000,0xc1800000,0x4f000000,0x4f7fffff,0x4f800000]
 for value,bucket,bank,rm in itertools.product(values,[0,0x3e000000,0xbe000000,0x3f800000,0xbf800000],range(2),range(2)):
  for mem in (ram,native):mem[0xefff80:0xf00000]=bytes(128);struct.pack_into('<I',mem,0xa00400,0x12345678)
  f=Fpu();f.fpscr=0x4007c|(bank<<21)|rm;f.fpul=0x12345678
  for b in range(2):f.banks[b][:]=[0x3f800000+i*37 for i in range(16)]
  f.banks[bank][4]=value;f.banks[bank][5]=bucket;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[4]=0x8ca00400;s.r[15]=0x8cf00000;s.pc=0x8c10fbb8;s.pr=0x8c000100;s.sr=0x700083f3
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();actual.clear();expected.clear()
  reference(ram,s,{},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert actual==expected and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native,(value,bucket,bank,rm,[(i,hex(r[i]),hex(s.r[i])) for i in range(16) if r[i]!=s.r[i]],hex(sr.value),hex(s.sr),[(b,i,hex(nf.banks[b][i]),hex(f.banks[b][i])) for b in range(2) for i in range(16) if nf.banks[b][i]!=f.banks[b][i]],hex(nf.fpscr),hex(f.fpscr))
  count+=1
 assert not errors,errors
 names=['sa_atan_classifier.c','sh4_divide.c','sh4_add_sub.c','sh4_float_i32.c','sh4_truncate_i32.c','sh4_multiply.c','sh4_fmac.c']
 report=dict(cases_passed=count,instruction_count=114,services_visited=[hex(t) for t in sorted(targets)],binary_sha256=hashlib.sha256(binary).hexdigest(),dependency_source_sha256={n:hashlib.sha256((out/'source'/n).read_bytes()).hexdigest() for n in names},dll_sha256=hashlib.sha256((ROOT/'work/sonic-adventure/build/atan_classifier.dll').read_bytes()).hexdigest(),scope='Whole quotient/remainder reduction helper used to select atan approximation branches. Native division, FLOAT/FTRC, multiply and add/subtract; no injected services/arithmetic. Finite signed numerator, positive/negative/zero divisor, integer rounding/conversion boundaries, both banks/RM0/1. All registers/SR/PR/FPU/FPUL/stack/RAM compared against unhooked retail body. Nonfinite operands and subnormal paths incomplete.')
 (out/'analysis/atan_classifier_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'atan reduction comparisons passed')
if __name__=='__main__':main()
