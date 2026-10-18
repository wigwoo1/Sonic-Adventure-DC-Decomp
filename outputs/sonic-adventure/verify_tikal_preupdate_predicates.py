import ctypes,itertools,json,hashlib,struct,re
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';module=(ROOT/'work/sonic-adventure/decoded/SONICADV/TIKAL_PROG.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0xb00000:0xb00000+len(module)]=module;native=bytearray(ram)
 view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/tikal_preupdate_predicates.dll')).sa_tikal_preupdate_predicates
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 targets={int(x,16) for x in re.findall(r'(?:COMPUTED_CALL|UNCONDITIONAL_CALL):([0-9a-f]+)',(out/'analysis/instructions/tikal_preupdate.tsv').read_text())};targets.discard(0xcb00994);targets.discard(0xcb00e74);targets.discard(0xcb00e90);targets.add(0x8c06cd22)
 events=[];actual=[];errors=[];visited=set()
 def effect(mem,regs,status,fp,target,pr):
  trace=(target,tuple(regs[4:8]),regs[15],pr);visited.add(target)
  bit={0xcb00cd2:0,0xcb00e74:1,0xcb00b64:2,0xcb00e90:3,0xcb00c20:3,0x8c06ce40:4,0x8c06cd22:4,0x8c06cefe:4}.get(target,0)
  regs[0]=(variant>>bit)&1;regs[1]=0x12340001;regs[2]=0x12340002;regs[3]=0x12340003
  if target==0xcb00cd2 and variant==16:fp.fpscr^=0x200000
  return trace,status^0x31
 @Call
 def call(ctx,target,m,fp,sr,regs,pr):
  try:trace,sr[0]=effect(native,regs,sr[0],fp.contents,target,pr[0]);events.append(trace)
  except Exception as ex:errors.append(str(ex));return -9
  return 0
 def hook(t):
  def body(mem,regs,sr):
   trace,sr=effect(mem,regs,sr,f,t,state.pr);actual.append(trace);return sr
  return body
 hooks={t:hook(t) for t in targets};cases=0
 for mode,variant in itertools.product(range(256),range(32)):
  owner=(variant>>1)&1;bank=variant&1;rm=(variant>>1)&1;sr0=0x700083f3 if mode&1 else 0
  for mem in (ram,native):
   mem[0xa00000:0xa01000]=bytes(4096);mem[0xf00000-64:0xf00000]=bytes(64)
   mem[0xa00200]=mode;mem[0xa00208]=owner;struct.pack_into('<H',mem,0xa00204,0xffff if variant&8 else 0xfffc)
   struct.pack_into('<I',mem,0xa0023c,0x8ca00b00);struct.pack_into('<I',mem,0xa00b4c,variant&1)
   struct.pack_into('<I',mem,0xa00308,0xbf800000 if variant&8 else 0x3f800000)
   struct.pack_into('<3I',mem,0xa0031c,0x11223344,0x22334455,0x33445566)
   struct.pack_into('<I',mem,0xa00338,0x3e800000)
   for off,val in [(0x38,0 if variant&4 else (0x3f800000 if variant&16 else 0xbf800000)),(0x3c,0xbf800000),(0xc0,0x3e800000),(0xd4,0xbe800000)]:struct.pack_into('<I',mem,0xa00400+off,val)
   struct.pack_into('<H',mem,0xa00524,7 if variant&4 else 1)
   struct.pack_into('<I',mem,0x754c18+owner*52,4 if variant&4 else 0)
  f=Fpu();f.fpscr=0x7f07c|(bank<<21)|rm;f.fpul=0x98765432
  for b in range(2):
   for i in range(16):f.banks[b][i]=0x3f800000+i*17
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[0x12340000+i for i in range(16)]
  state.r[4]=0x8ca00200;state.r[5]=0x8ca00300;state.r[6]=0x8ca00400;state.r[15]=0x8cf00000;state.pc=0xcb003a0;state.pr=0x8c000100;state.sr=sr0
  r=(ctypes.c_uint32*16)(*state.r);sr=ctypes.c_uint32(sr0);pr=ctypes.c_uint32(state.pr);macl=ctypes.c_uint32();fpul=ctypes.c_uint32(f.fpul)
  events.clear();actual.clear();reference(ram,state,hooks,f)
  result=fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call);nf.fpul=fpul.value
  assert result==0,(mode,variant,result)
  assert events==actual,(mode,variant,'calls',events,actual)
  assert list(r)==list(state.r),(mode,variant,'regs',list(r),list(state.r))
  assert sr.value==state.sr and pr.value==state.pr and bytes(nf)==bytes(f),(mode,variant,'sr/fpu')
  assert ram==native,(mode,variant,'ram');cases+=1
 assert not errors,errors
 report=dict(dll_sha256=hashlib.sha256((ROOT/'work/sonic-adventure/build/tikal_preupdate_predicates.dll').read_bytes()).hexdigest(),retail_span_sha256=hashlib.sha256(module[0x3a0:0x780]).hexdigest(),cases_passed=cases,retail_instruction_count=431,module_sha256=hashlib.sha256(module).hexdigest(),source_sha256=hashlib.sha256((out/'source/tikal_preupdate_predicates.c').read_bytes()).hexdigest(),services_visited=[hex(x) for x in sorted(visited)],scope='Complete pre-update routine, all state bytes, early-return service combinations, flags/owner event table, positive/negative phase/height, both banks/RM0/1 and callback bank changes. Native flags/idle predicates and transition tail calls; other subordinate/external functions injected. All regs/SR/PR/FPU/FPUL/RAM/calls compared. Finite normal/zero PR=SZ=0; controller tested separately.')
 report['dependency_source_sha256']={name:hashlib.sha256((out/'source'/name).read_bytes()).hexdigest() for name in ['tikal_preupdate_predicates.c', 'tikal_state_predicates.c', 'tikal_preupdate.c', 'tikal_transition.c', 'sh4_fmac.c', 'sh4_add_sub.c']}
 report['dll_sha256']=hashlib.sha256((ROOT/'work/sonic-adventure/build/tikal_preupdate_predicates.dll').read_bytes()).hexdigest()
 (out/'analysis/tikal_preupdate_predicates_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(cases,'Tikal pre-update/predicate comparisons passed')
if __name__=='__main__':main()
