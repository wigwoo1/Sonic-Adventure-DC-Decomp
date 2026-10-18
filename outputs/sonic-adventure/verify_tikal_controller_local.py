import ctypes,itertools,json,hashlib,struct,re
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
P=ctypes.POINTER(ctypes.c_uint32)
Call=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P)
def main():
 out=ROOT/'outputs/sonic-adventure';module=(ROOT/'work/sonic-adventure/decoded/SONICADV/TIKAL_PROG.BIN').read_bytes()
 ram=bytearray(16*1024*1024);ram[0xb00000:0xb00000+len(module)]=module;native=bytearray(ram)
 view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/tikal_controller_local.dll')).sa_tikal_controller_local
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 targets={int(x,16) for x in re.findall(r'COMPUTED_CALL:([0-9a-f]+)',(out/'analysis/instructions/0cb00000.tsv').read_text())}
 targets|={0x8c06c6f8,0xcb003a0,0xcb00f60,0xcb00a40,0xcb007a0,0xcb008a0};targets.discard(0xcb00994)
 targets.discard(0xcb003a0)
 targets|={int(x,16) for x in re.findall(r'(?:COMPUTED_CALL|UNCONDITIONAL_CALL):([0-9a-f]+)',(out/'analysis/instructions/tikal_preupdate.tsv').read_text())};targets.discard(0xcb00994);targets.discard(0xcb00e74);targets.discard(0xcb00e90);targets.add(0x8c06cd22);targets.discard(0xcb00b64);targets.discard(0xcb00c20);targets|={0x8c06cebe,0x8c07cb54,0x8c0940da,0x8c07256a,0x8c07c298};targets.discard(0xcb00cd2);targets.discard(0xcb00c64)
 events=[];actual=[];errors=[]
 def effect(mem,regs,status,fp,target,pr):
  trace=(target,tuple(regs[4:8]),regs[15],pr)
  if target==0x8c07c298 and variant==2:fp.fpscr^=0x200000
  if target==0x8c09852e:regs[0]=0x8ca00900 if variant&1 else 0
  else:regs[0]=variant&1
  regs[1]=0x12340001;regs[2]=0x12340002;regs[3]=0x12340003
  return trace,status^0x31
 @Call
 def call(ctx,target,m,fp,sr,regs,pr):
  try:
   trace,sr[0]=effect(native,regs,sr[0],fp.contents,target,pr[0]);events.append(trace)
  except Exception as ex:errors.append(str(ex));return -9
  return 0
 def hook(t):
  def body(mem,regs,sr):
   trace,sr=effect(mem,regs,sr,f,t,state.pr);actual.append(trace);return sr
  return body
 hooks={t:hook(t) for t in targets};cases=0
 for mode,variant in itertools.product(range(256),range(4)):
  owner=(mode*19+variant*7)&255;bank=variant&1;rm=(variant>>1)&1;sr0=0x700083f3 if mode&1 else 0
  for mem in (ram,native):
   mem[0xa00000:0xa01000]=bytes(4096);mem[0xf00000-64:0xf00000]=bytes(64)
   struct.pack_into('<2I',mem,0xa00120,0x8ca00200,0x8ca00300);struct.pack_into('<I',mem,0xa00300,0x8ca00400)
   mem[0xa00200]=mode;mem[0xa00201]=(variant*7)&255;mem[0xa00b24]=5 if variant&2 else 0;mem[0xa00208]=owner;struct.pack_into('<H',mem,0xa00204,0x3023 if variant&2 else 0x3000)
   struct.pack_into('<I',mem,0xa00308,0xbf800000 if variant&1 else 0x3f800000)
   struct.pack_into('<I',mem,0xa0023c,0x8ca00b00);struct.pack_into('<I',mem,0xa00b4c,variant&1);struct.pack_into('<I',mem,0xa00238,0x8ca00b00);struct.pack_into('<2I',mem,0xa00b0c,0x8ca00c00,0x8ca00d00)
   struct.pack_into('<I',mem,0xa00d20,0x8ca00e00);struct.pack_into('<I',mem,0xa00e14,0x11223344);struct.pack_into('<I',mem,0xa00e1c,0x55667788)
   struct.pack_into('<I',mem,0xa00920,0x8ca00a00)
   for off,val in [(0x38,0x3f800000),(0x3c,0xbf800000),(0xb8,0 if variant&1 else 0xc0000000),(0xc0,0x3e800000),(0x10c,0x3f000000)]:struct.pack_into('<I',mem,0xa00400+off,val)
   struct.pack_into('<3H',mem,0xa00524,8 if variant&1 else 1,0,8 if variant&2 else 1)
   struct.pack_into('<I',mem,0x19e38c,4);struct.pack_into('<I',mem,0x754c18+owner*52,(0x400 if variant&1 else 0)|(4 if variant&2 else 0))
  f=Fpu();f.fpscr=0x7f07c|(bank<<21)|rm;f.fpul=0x98765432
  for b in range(2):
   for i in range(16):f.banks[b][i]=0x3f800000+i*17
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[0x12340000+i for i in range(16)]
  state.r[4]=0x8ca00100;state.r[15]=0x8cf00000;state.pc=0xcb00000;state.pr=0x8c000100;state.sr=sr0
  r=(ctypes.c_uint32*16)(*state.r);sr=ctypes.c_uint32(sr0);pr=ctypes.c_uint32(state.pr);macl=ctypes.c_uint32();fpul=ctypes.c_uint32(f.fpul)
  events.clear();actual.clear();reference(ram,state,hooks,f)
  result=fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call);nf.fpul=fpul.value
  assert result==0,(mode,variant,result)
  assert events==actual,(mode,variant,'calls',events,actual)
  assert list(r)==list(state.r),(mode,variant,'regs',list(r),list(state.r))
  assert sr.value==state.sr and pr.value==state.pr and bytes(nf)==bytes(f),(mode,variant,'sr/fpu')
  assert ram==native,(mode,variant,'ram')
  assert macl.value==owner*52,(mode,variant,'macl');cases+=1
 assert not errors,errors
 report=dict(dependency_source_sha256={name:hashlib.sha256((out/'source'/name).read_bytes()).hexdigest() for name in ['tikal_controller.c','tikal_state_predicates.c','tikal_preupdate_predicates.c','tikal_preupdate.c','tikal_transition.c','sh4_fmac.c','sh4_add_sub.c']},dll_sha256=hashlib.sha256((ROOT/'work/sonic-adventure/build/tikal_controller_local.dll').read_bytes()).hexdigest(),retail_span_sha256=hashlib.sha256(module[:0x352]).hexdigest(),cases_passed=cases,retail_instruction_count=358,module_sha256=hashlib.sha256(module).hexdigest(),source_sha256=hashlib.sha256((out/'source/tikal_controller_local.c').read_bytes()).hexdigest(),scope='Controller connected to native pre-update, flags/idle predicates, b64/c20 transitions, c64/cd2 event dispatch and scalar transition including prologue, all 256 state bytes, state switch after actual native pre-update decisions, mode 0 allocation null/success, state2 native transition and additive step, branch service return, callback FPSCR bank change, owner flag transition, tail call and saved stack. All registers/SR/PR/FPU/FPUL/RAM and callback sequence compared. Native pre-update, flags/idle predicates, b64/c20 transitions, c64/cd2 event dispatch and scalar transition; remaining gameplay/rendering and subordinate functions injected. Finite normal/zero PR=SZ=0; exact external bodies still pending.')
 report['dependency_source_sha256']={name:hashlib.sha256((out/'source'/name).read_bytes()).hexdigest() for name in ['tikal_controller_local.c', 'tikal_controller.c', 'tikal_preupdate_predicates.c', 'tikal_state_predicates.c', 'tikal_preupdate.c', 'tikal_transition.c', 'sh4_fmac.c', 'sh4_add_sub.c']}
 report['dll_sha256']=hashlib.sha256((ROOT/'work/sonic-adventure/build/tikal_controller_local.dll').read_bytes()).hexdigest()
 report['dependency_source_sha256']={name:hashlib.sha256((out/'source'/name).read_bytes()).hexdigest() for name in ['tikal_controller_local.c', 'tikal_controller.c', 'tikal_preupdate_subordinates.c', 'tikal_b64_transition.c', 'tikal_c20_transition.c', 'tikal_state_predicates.c', 'tikal_preupdate.c', 'tikal_transition.c', 'sh4_fmac.c', 'sh4_add_sub.c']}
 report['dll_sha256']=hashlib.sha256((ROOT/'work/sonic-adventure/build/tikal_controller_local.dll').read_bytes()).hexdigest()
 report['dependency_source_sha256']={name:hashlib.sha256((out/'source'/name).read_bytes()).hexdigest() for name in ['tikal_controller_local.c', 'tikal_controller.c', 'tikal_preupdate_local.c', 'tikal_events_native.c', 'tikal_c64_event.c', 'tikal_cd2_event.c', 'tikal_b64_transition.c', 'tikal_c20_transition.c', 'tikal_state_predicates.c', 'tikal_preupdate.c', 'tikal_transition.c', 'sh4_fmac.c', 'sh4_add_sub.c']}
 report['dll_sha256']=hashlib.sha256((ROOT/'work/sonic-adventure/build/tikal_controller_local.dll').read_bytes()).hexdigest()
 (out/'analysis/tikal_controller_local_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(cases,'Tikal connected-local-helper controller comparisons passed')
if __name__=='__main__':main()
