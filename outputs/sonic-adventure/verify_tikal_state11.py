import ctypes,itertools,json,hashlib,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';module=(ROOT/'work/sonic-adventure/decoded/SONICADV/TIKAL_PROG.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0xb00000:0xb00000+len(module)]=module;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/tikal_state11.dll')).tikal_state11
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 events=[];actual=[];errors=[];visited=set()
 def effect(mem,r,sr,fp,target,pr):
  bank=(fp.fpscr>>21)&1;trace=(target,tuple(r[4:8]),tuple(fp.banks[bank][i] for i in range(4,7)),r[15],pr);visited.add(target)
  if target==0x8c04f8fa and mutate:mem[0xa00108]=owner^1;fp.fpscr^=0x200000
  if target==0x8c63a9c4:fp.banks[bank][0]=0x40000000
  elif target==0x8c10d0f8:fp.banks[bank][0]=0x3e800000
  elif target==0x8c63a970:fp.banks[bank][0]=0x3f000000
  elif target==0x8c63a9b8:fp.banks[bank][0]=0xbf000000
  r[0]=0x76543210;r[1]=0x11223344;r[2]=0x22334455;r[3]=0x33445566
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
 axes=[(0,0),(1,0),(0,1),(12,0),(13,0),(-12,0),(-13,0),(0,12),(0,13),(0,-12),(0,-13),(13,13),(32767,-32768)]
 count=0
 for owner,bank,rm,flags,event,mutate,(x,y) in itertools.product([0,127,128,255],range(2),range(2),[0,0x400],range(2),range(2),axes):
  for mem in (ram,native):
   mem[0xa00000:0xa00400]=bytes(1024);mem[0xf00000-64:0xf00000]=bytes(64);mem[0xa00108]=owner
   struct.pack_into('<3I',mem,0xa00120,0x3f800000,0xbf800000,0x80000000)
   struct.pack_into('<I',mem,0x18b7ec,0x8ca00300);struct.pack_into('<I',mem,0xa00318,0xffffffff)
   for index in [owner,owner^1]:
    struct.pack_into('<I',mem,0x754c18+index*52,2 if event else 0);struct.pack_into('<2h',mem,0x754c08+index*52+28,x,y)
  f=Fpu();f.fpscr=0x7f07c|(bank<<21)|rm;f.fpul=0x12345678
  for b in range(2):
   for i in range(16):f.banks[b][i]=0x3f800000+i*37
  nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[4]=0x8ca00100;s.r[5]=0x8ca00280;s.r[6]=0x8ca00200;s.r[15]=0x8cf00000;s.pc=0xcb00a40;s.pr=0x8c000100;s.sr=0x700083f3 if owner&1 else 0
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();events.clear();actual.clear()
  # Flag0x400 belongs to the same owner word whose low bit triggers event service.
  for mem in (ram,native):
   for index in [owner,owner^1]:struct.pack_into('<I',mem,0x754c18+index*52,flags|(2 if event else 0))
  reference(ram,s,{t:hook(t) for t in [0x8c04f8fa,0x8c63a9c4,0x8c10d0f8,0x8c63a970,0x8c63a9b8]},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  nf.fpul=fpul.value
  assert events==actual,(owner,bank,rm,flags,event,mutate,x,y,'calls',events,actual)
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native,(owner,bank,rm,flags,event,mutate,x,y)
  assert macl.value==((owner^1) if event and mutate else owner)*52;count+=1
 assert not errors,errors
 report=dict(cases_passed=count,module_sha256=hashlib.sha256(module).hexdigest(),source_sha256=hashlib.sha256((out/'source/tikal_state11.c').read_bytes()).hexdigest(),services_visited=[hex(x) for x in sorted(visited)],scope='Whole state11 routine, all integer regs/SR/PR/FPU/FPUL/MACL/stack/RAM/callback order. Deadzone +/-12 boundary, signed16 extremes, both owner branches and sqrt zero bypass, banks/RM0/1, optional event service owner/bank mutation. Native integer binary32 conversion/multiply/divide/subtract/FMAC/truncation. Event, sqrt, angle and trig services injected; finite scalar scope PR=SZ=0; controller integration pending.')
 report['dependency_source_sha256']={name:hashlib.sha256((out/'source'/name).read_bytes()).hexdigest() for name in ['tikal_state11.c', 'sh4_fmac.c', 'sh4_multiply.c', 'sh4_divide.c', 'sh4_add_sub.c', 'sh4_float_i32.c', 'sh4_truncate_i32.c']}
 report['dll_sha256']=hashlib.sha256((ROOT/'work/sonic-adventure/build/tikal_state11.dll').read_bytes()).hexdigest()
 (out/'analysis/tikal_state11_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'Tikal state11 comparisons passed')
if __name__=='__main__':main()
