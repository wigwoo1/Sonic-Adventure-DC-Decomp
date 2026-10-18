import ctypes,itertools,json,hashlib,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';module=(ROOT/'work/sonic-adventure/decoded/SONICADV/TIKAL_PROG.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0xb00000:0xb00000+len(module)]=module;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/tikal_event_dispatch.dll'));funcs=[dll.tikal_c64_event,dll.sa_tikal_cd2_connected]
 for f in funcs:f.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 events=[];actual=[];errors=[]
 def effect(mem,r,sr,fp,target,pr):
  bank=(fp.fpscr>>21)&1;trace=(target,tuple(r[4:8]),r[15],pr)
  if mutation:fp.fpscr^=0x200000;mem[0xa00100]=11
  r[0]=1;r[1]=0x11223344;r[2]=0x22334455;r[3]=0x33445566
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
 fixtures=[itertools.product(range(256),[0,0x4000,0xffff],[0,127,128,255]),itertools.product([0,1],range(256),[0,0x1000,0xffff],[1,2],[0xbf800000,0x3f800000])]
 for kind,cases in enumerate(fixtures):
  for case in cases:
   if kind==0:collision,flags,owner=case;action=0;mode=1;height=0x3f800000
   else:collision,action,flags,mode,height=case;owner=(action*19)&255
   mutation=action&1;bank=owner&1
   for mem in (ram,native):
    mem[0xa00000:0xa00500]=bytes(1280);mem[0xf00000-64:0xf00000]=bytes(64)
    mem[0xa00100]=mode;mem[0xa00101]=action;mem[0xa00108]=owner;struct.pack_into('<H',mem,0xa00104,flags)
    struct.pack_into('<I',mem,0xa0013c,0x8ca00300);mem[0xa00324]=collision;struct.pack_into('<I',mem,0xa0034c,1 if action&1 else 0)
    struct.pack_into('<I',mem,0xa0032c,0xfffffffe if owner&1 else 2);struct.pack_into('<I',mem,0xa00118,0x7fffffff if owner&1 else 0xffffffff)
    struct.pack_into('<I',mem,0xa00238,height);struct.pack_into('<I',mem,0xa002c4,0x3e800000)
   f=Fpu();f.fpscr=0x7f07c|(bank<<21);f.fpul=0x12345678
   for b in range(2):
    for i in range(16):f.banks[b][i]=0x3f800000+i*37
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[4]=0x8ca00100;s.r[5]=0x8ca00280;s.r[6]=0x8ca00200;s.r[15]=0x8cf00000;s.pc=[0xcb00c64,0xcb00cd2][kind];s.pr=0x8c000100;s.sr=0x700083f3 if owner&1 else 0
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();events.clear();actual.clear()
   reference(ram,s,{t:hook(t) for t in [0x8c07c298,0x8c07256a,0x8c0940da]},f)
   assert funcs[kind](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   assert events==actual and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native,(kind,case)
   counts[kind]+=1
 assert not errors,errors
 sources=['tikal_c64_event.c','tikal_cd2_event.c','tikal_events_native.c'];report=dict(c64_cases_passed=counts[0],cd2_cases_passed=counts[1],module_sha256=hashlib.sha256(module).hexdigest(),source_sha256={x:hashlib.sha256((out/'source'/x).read_bytes()).hexdigest() for x in sources},scope='Full c64 dispatch and full cd2 event routine with native c64 connected; no local function hooks. All256 collision/event bytes, action bytes, flags, modes1/2, positive/negative height threshold, signed owner bytes, angle integer wrap, callback state/bank mutation. All registers/SR/PR/FPU/FPUL/RAM and external calls compared. External gameplay/sound/angle services injected; finite normal/zero PR=SZ=0. Parent pre-update integration pending.')
 report['dependency_source_sha256']={name:hashlib.sha256((out/'source'/name).read_bytes()).hexdigest() for name in ['tikal_events_native.c', 'tikal_c64_event.c', 'tikal_cd2_event.c']}
 report['dll_sha256']=hashlib.sha256((ROOT/'work/sonic-adventure/build/tikal_event_dispatch.dll').read_bytes()).hexdigest()
 (out/'analysis/tikal_event_dispatch_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'Tikal event dispatch comparisons passed')
if __name__=='__main__':main()
