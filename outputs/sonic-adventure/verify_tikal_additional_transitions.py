import ctypes,itertools,json,hashlib,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';module=(ROOT/'work/sonic-adventure/decoded/SONICADV/TIKAL_PROG.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0xb00000:0xb00000+len(module)]=module;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/tikal_additional_transitions.dll'))
 funcs=[dll.tikal_b64_transition,dll.tikal_c20_transition]
 for f in funcs:f.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 events=[];actual=[];errors=[]
 def effect(mem,r,sr,fp,target,pr):
  bank=(fp.fpscr>>21)&1;trace=(target,tuple(r[4:8]),tuple(fp.banks[bank][i] for i in range(4,7)),r[15],pr)
  if mutation:
   fp.fpscr^=0x200000;struct.pack_into('<I',mem,0xa0026c,0x8ca00300)
  r[0]=ret;r[1]=0x11223344;r[2]=0x22334455;r[3]=0x33445566
  return trace,sr^0x31
 @Call
 def call(ctx,target,m,fp,sr,r,pr):
  try:trace,sr[0]=effect(native,r,sr[0],fp.contents,target,pr[0]);events.append(trace)
  except Exception as ex:errors.append(str(ex));return -9
  return 0
 def hook(t):
  def body(mem,r,sr):
   trace,sr=effect(mem,r,sr,f,t,s.pr);actual.append(trace);return sr
  return body
 counts=[0,0]
 fixtures=[itertools.product([0,127,128,255],range(2),range(3),[0,0x80000000,0x7fc01234,0x3f800000],range(2),range(2),range(2)),itertools.product(range(256),[0,4,0xffffffff],[0,4,0x100,0xffffffff])]
 for kind,cases in enumerate(fixtures):
  for case in cases:
   if kind==0:owner,srbit,link,raw,ret,bank,mutation=case;mask=4;event=4
   else:owner,event,mask=case;srbit=owner&1;bank=(owner>>1)&1;mutation=(owner>>2)&1;ret=owner&1;link=2;raw=0x7fc01234
   for mem in (ram,native):
    mem[0xa00000:0xa00500]=bytes(1280);mem[0xf00000-64:0xf00000]=bytes(64)
    mem[0xa00100]=11;mem[0xa00108]=owner;struct.pack_into('<H',mem,0xa00104,0xffff)
    struct.pack_into('<I',mem,0xa002b4,raw);struct.pack_into('<I',mem,0xa0026c,0x8ca00300 if link else 0)
    struct.pack_into('<I',mem,0xa00328,0x8ca00400 if link==2 else 0);struct.pack_into('<3I',mem,0xa00410,0x3f000000,0xbf000000,0x80000000)
    struct.pack_into('<I',mem,0x19e38c,mask);struct.pack_into('<I',mem,0x754c18+owner*52,event)
   f=Fpu();f.fpscr=0x7f07c|(bank<<21);f.fpul=0x12345678
   for b in range(2):
    for i in range(16):f.banks[b][i]=0x3f800000+i*37
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[4]=0x8ca00100;s.r[5]=0x8ca00280;s.r[6]=0x8ca00200;s.r[15]=0x8cf00000;s.pc=[0xcb00b64,0xcb00c20][kind];s.pr=0x8c000100;s.sr=0x700083f3 if srbit else 0
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();events.clear();actual.clear()
   targets=[0x8c06cebe,0x8c07cb54,0x8c0940da] if kind==0 else [0x8c07256a]
   reference(ram,s,{t:hook(t) for t in targets},f)
   assert funcs[kind](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   assert events==actual and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native,(kind,case)
   if kind==1:assert macl.value==owner*52
   counts[kind]+=1
 assert not errors,errors
 sources=['tikal_b64_transition.c','tikal_c20_transition.c'];report=dict(b64_cases_passed=counts[0],c20_cases_passed=counts[1],module_sha256=hashlib.sha256(module).hexdigest(),source_sha256={x:hashlib.sha256((out/'source'/x).read_bytes()).hexdigest() for x in sources},scope='Complete retail b64 and c20 transition bodies, all registers/SR/PR/FPU/FPUL/RAM and calls compared. b64 external eligibility zero/nonzero, null/child chains, raw scalar copies including signed zero/NaN, signed owner bytes, callback RAM/FPSCR mutation. c20 all256 owners and event/global mask intersection branches with delayed saved entity pointer, native MACL. External gameplay/sound/position services injected; PR=SZ=0 scope. Parent integration pending.')
 report['dependency_source_sha256']={name:hashlib.sha256((out/'source'/name).read_bytes()).hexdigest() for name in ['tikal_b64_transition.c', 'tikal_c20_transition.c']}
 report['dll_sha256']=hashlib.sha256((ROOT/'work/sonic-adventure/build/tikal_additional_transitions.dll').read_bytes()).hexdigest()
 (out/'analysis/tikal_additional_transitions_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'additional Tikal transition comparisons passed')
if __name__=='__main__':main()
