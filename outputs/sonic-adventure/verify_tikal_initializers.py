import ctypes,itertools,json,hashlib,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';module=(ROOT/'work/sonic-adventure/decoded/SONICADV/TIKAL_PROG.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0xb00000:0xb00000+len(module)]=module;native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/tikal_initializers.dll'));funcs=[dll.tikal_child_initialize,dll.tikal_effect_create,dll.tikal_effect_apply]
 for f in funcs:f.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 events=[];actual=[];errors=[]
 def effect(mem,r,sr,fp,target,pr):
  trace=(target,tuple(r[4:8]),r[15],pr)
  if mutation:
   struct.pack_into('<3I',mem,0xa00500,0x8ca00800,0x8ca00700,0x8ca00600);fp.fpscr^=0x200000
  result=0
  if target==0x8c098b0c:result=0x8ca00400 if outcome else 0
  elif target==0x8c09852e:result=0x8ca00200 if outcome else 0
  elif target==0x8c098a80:result=0x8ca00400 if outcome==2 else 0
  r[0]=result;r[1]=0x11223344;r[2]=0x22334455;r[3]=0x33445566
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
 counts=[0,0,0]
 fixtures=[itertools.product([0,127,128,255],range(2),range(2),[0,0x180003,0x3f07ff]),itertools.product(range(256),range(3),range(2),[0,0x180003,0x3f07ff]),itertools.product(range(32),range(3),[0,0x180003,0x3f07ff])]
 for kind,cases in enumerate(fixtures):
  for case in cases:
   if kind<2:owner,outcome,mutation,fpscr=case;alias=0;seed=owner
   else:seed,alias,fpscr=case;owner=seed;outcome=2;mutation=0
   for mem in (ram,native):
    mem[0xa00000:0xa01000]=bytes(4096);mem[0xf00000-64:0xf00000]=bytes(64)
    struct.pack_into('<I',mem,0xa00124,0x8ca00400);struct.pack_into('<I',mem,0xa00220,0x8ca00300)
    nodes=[0x8ca00600,0x8ca00700,0x8ca00800]
    if alias==1:nodes=[0x8ca00600]*3
    elif alias==2:nodes=[0x8ca00464]*3
    struct.pack_into('<3I',mem,0xa00500,*nodes);struct.pack_into('<3I',mem,0xa00458,*nodes)
    for i in range(9):struct.pack_into('<I',mem,0xa00464+i*4,(0x12340000+seed*37+i*19)&0xffffffff)
   f=Fpu();f.fpscr=fpscr;f.fpul=0x12345678
   for b in range(2):
    for i in range(16):f.banks[b][i]=0x3f800000+i*37
   nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x12340000+i for i in range(16)];s.r[4]=owner|0x12340000 if kind==1 else 0x8ca00100;s.r[5]=0x8ca00500;s.r[15]=0x8cf00000;s.pc=[0xcb00ed4,0xcb00f60,0xcb00fbe][kind];s.pr=0x8c000100;s.sr=0x700083f3 if owner&1 else 0
   r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32();events.clear();actual.clear()
   reference(ram,s,{t:hook(t) for t in [0x8c098b0c,0x8c09852e,0x8c098a80,0x8c098786]},f)
   assert funcs[kind](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
   assert events==actual and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and ram==native,(kind,case)
   counts[kind]+=1
 assert not errors,errors
 report=dict(child_cases_passed=counts[0],create_cases_passed=counts[1],apply_cases_passed=counts[2],module_sha256=hashlib.sha256(module).hexdigest(),scope='Whole child initializer, effect create and previously unindexed effect-apply leaf recovered. Allocation null/success and rollback task destruction, all256 owner bytes, descriptor table mutation, PR/SZ/reserved FPSCR bits with no floating instructions, alias/order-sensitive nine-coordinate model writes. All integer registers/SR/PR/FPU/FPUL/stack/RAM/callback order compared. Allocation/create/destroy bodies injected; controller connection pending.')
 report['dependency_source_sha256']={name:hashlib.sha256((out/'source'/name).read_bytes()).hexdigest() for name in ['tikal_child_initialize.c', 'tikal_effect_create.c', 'tikal_effect_apply.c']}
 report['dll_sha256']=hashlib.sha256((ROOT/'work/sonic-adventure/build/tikal_initializers.dll').read_bytes()).hexdigest()
 (out/'analysis/tikal_initializers_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(counts,'Tikal initializer/apply comparisons passed')
if __name__=='__main__':main()
