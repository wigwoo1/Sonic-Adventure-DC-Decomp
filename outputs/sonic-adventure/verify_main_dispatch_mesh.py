"""Native mesh/material loop versus original bytes with explicit lower services."""
import ctypes,gc,hashlib,inspect,itertools,json,sys
from pathlib import Path
import verify_main_dispatch_model_traversal as traversal
from verify_tikal_controller import Call,P
PROJECT=Path(__file__).resolve().parent
geom=traversal.geom;particle=traversal.particle;current=traversal.current;base=traversal.base
load,store=base.load,base.store
MESH,FILTER=0x8C6209A8,0x8C62159A
CULL=0x8C611444
LIMIT=base.literal(0x8C620A20);STATE=base.literal(0x8C620A1C)
FLAGS=base.literal(0x8C620B68);MODE=base.literal(0x8C620E74);ENV=base.literal(0x8C620B6C)
OVERRIDE=base.literal(0x8C620B88)
ACQUIRE={base.literal(a) for a in (0x8C620CD8,0x8C620CE0,0x8C620CF0,0x8C620CF8)}
DRAWERS={base.literal(a) for a in range(0x8C620EA4,0x8C620EEC,4)}
SERVICES=ACQUIRE|DRAWERS|{CULL,0x8C606A88,0x8C606C56,0x8C606A08,0x8C6069A4}
LAYOUTS=tuple(((a<<24)|(b<<21)) for a,b in itertools.product(range(4),range(4)))
DATA=0x8CC80000;SEGMENTS=0x8CC90000;MATERIALS=0x8CCA0000;HANDLE=0x8CCB0000
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_mesh.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(traversal.SPECS,mesh=(MESH,'sa_mesh_material_loop_connected'),filter=(FILTER,'sa_mesh_filter_wrapper_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac'):
 fn=getattr(LIB,name);fn.argtypes=getattr(geom.LIB,name).argtypes;fn.restype=ctypes.c_int

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def effect(c,target,mem,r,sr,fp):
 if c.get('capture') is not None:c['capture'].append((target,r[4],r[5],load(mem,FLAGS)))
 for i in range(8):r[i]=0xBAD00000+i*0x101
 if target==CULL:
  r[0]=c.get('reject',0)&0xFFFFFFFF;r[8]=0x8BADF00D
  if c.get('flip_bank'):fp.fpscr^=0x200000
  if 'after_env' in c:store(mem,ENV,c['after_env'])
 elif target in ACQUIRE:r[0]=HANDLE
 return sr^1

def setup():
 *_,allowed,_=traversal.setup()
 manifest_path=PROJECT/'analysis/main_dispatch_mesh_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_model_traversal_verification.json'
 for report in (manifest,json.loads(baseline.read_text())):
  for p,h in report['sha256'].items():assert digest(Path(p))==h,p
 allowed|={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 base.TRACKED|={MESH,FILTER}
 source=inspect.getsource(current.execute)
 changes={
  'load(mem,base.LEVELS+20)':'load(mem,base.LEVELS+20),tuple(load(mem,STATE+i*4) for i in range(16)),load(mem,FLAGS),load(mem,MODE)',
  '(0xF08D,0xF04D,0xF05D)':'(0xF08D,0xF09D,0xF04D,0xF05D)',
  '  elif w&15==2:rc=LIB.sa_sh4_multiply_extended':'  elif w&15==14:rc=LIB.sa_sh4_fmac(ptr(0),ptr(src),ptr(n),ctypes.byref(fps),ptr(n))\n  elif w&15==2:rc=LIB.sa_sh4_multiply_extended',
  ' def trace(pc,w,r,sr,pr,_f):':' def trace(pc,w,r,sr,pr,_f):\n  nonlocal ops',
  'w&15 in (0,1,2)':'w&15 in (0,1,2,14)',
  '   rc,probe=scalar_probe(w)':'   ops+=1\n   rc,probe=scalar_probe(w)',
  '  if w&0xF00F==0xF005 and ef.fpscr&0x80000:stop(pc,r,sr,pr,-1)':
  '  if w==0xFBFD and ef.fpscr&0x80000:stop(pc,r,sr,pr,-1)\n  if w&0xF00F==0xF005 and ef.fpscr&0x80000:stop(pc,r,sr,pr,-1)',
  ' try:base.previous.reference(em,expected,fpu=ef,special=special,trace=trace,vector_instruction=vector,instruction_budget=40000)':
  ''' def make_hook(target):
  def service_hook(mem,r,sr):
   wanted.append(event(target,mem,r,sr,expected.pr,ef,ef.fpul,special['macl']))
   if c.get('null_service') or c.get('missing_target')==target:stop(target,r,sr,expected.pr,-2)
   sr=effect(c,target,mem,r,sr,ef)
   if c.get('fail_target')==target:stop(target,r,sr,expected.pr,-17)
   return sr
  return service_hook
 try:base.previous.reference(em,expected,fpu=ef,special=special,trace=trace,hooks={t:make_hook(t) for t in SERVICES},vector_instruction=vector,instruction_budget=40000)''',
  ' @base.OBSERVE\n def observe':
  ''' @Call
 def service_call(_ctx,target,mem,fp,ss,r,p):
  try:
   assert target in SERVICES,hex(target)
   if c.get('missing_target')==target:return -2
   ss[0]=effect(c,target,mem,r,ss[0],fp.contents)
   return -17 if c.get('fail_target')==target else 0
  except Exception as exc:errors.append(repr(exc));return -99
 @base.OBSERVE
 def observe''',
  'None,Call(),observe)':'None,Call() if c.get("null_service") else service_call,observe)',
  '  assert list(s.r)[8:15]==list(initial.r)[8:15]':'  assert list(s.r)[9 if entry=="mesh" else 8:15]==list(initial.r)[9 if entry=="mesh" else 8:15]',
  'return path,status,len(observed),ops,ml.value':'return path,status,len(observed),ops,ml.value,sum(e[0] in SERVICES for e in observed)',
 }
 for old,new in changes.items():assert source.count(old)==1,(old,source.count(old));source=source.replace(old,new)
 ns=dict(current.__dict__)
 ns.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,LEGACY=current.LEGACY|{particle.TRANSLATE}|traversal.REUSED,
           STATE=STATE,FLAGS=FLAGS,MODE=MODE,SERVICES=SERVICES,effect=effect)
 exec(compile(source,'mesh_observed_execute','exec'),ns)
 return manifest_path,manifest,baseline,allowed,ns['execute']

def prepare(c,n):
 m,s,f=traversal.prepare(c,n)
 entry=c['entry']
 meshes=[DATA] if entry in ('mesh','filter') else list(traversal.MESHES[:5])
 if c.get('graph')=='retail':meshes=[]
 materials=c.get('materials',(0,));profiles=c.get('profiles',(c.get('layout',0),))
 for a in meshes:
  store(m,a+8,c.get('vertices',4));store(m,a+12,SEGMENTS);store(m,a+16,MATERIALS);store(m,a+20,len(materials),2)
 for i,idx in enumerate(materials):
  store(m,SEGMENTS+i*24,idx|c.get('segment_bits',0),2)
  for off in range(4,24,4):store(m,SEGMENTS+i*24+off,0xA0000000+i*0x100+off)
 for i in range(4):
  a=MATERIALS+i*20
  for j,v in enumerate((17,65,129,255,33,97,201,0)):store(m,a+j,v,1)
  store(m,a+8,current.bits(12.5+i));store(m,a+12,0xC0000000|i)
  store(m,a+16,profiles[i%len(profiles)]|c.get('material_extra',0))
 store(m,LIMIT,c.get('vertex_limit',4096 if c.get('graph')=='retail' else 4));store(m,ENV,c.get('env',0));store(m,MODE,c.get('render_mode',0))
 store(m,base.literal(0x8C620B70),c.get('and_mask',0xFFFFFFFF));store(m,base.literal(0x8C620B74),c.get('or_mask',0))
 for i,v in enumerate((0.125,0.25,0.375,0.5)):store(m,OVERRIDE+i*4,current.bits(v))
 for i in range(16):store(m,STATE+i*4,current.bits(i*0.0625))
 store(m,FLAGS,0x12345678)
 if entry in ('mesh','filter'):s.r[4]=DATA
 return m,s,f

def cases():
 for entry,layout,mode,bank,extra in itertools.product(('mesh','mesh_wrapper'),LAYOUTS,(0,1),range(2),(0,0x100000)):
  yield dict(entry=entry,layout=layout,render_mode=mode,mode=bank<<21,material_extra=extra)
 for env,extra,rm,bank in itertools.product((0,4,16,32,0x200,0x210,0x224),(0,0x100000),range(2),range(2)):
  yield dict(entry='mesh',layout=0x200000,env=env,material_extra=extra,rm=rm,mode=bank<<21)
 for indices,mode,bank in itertools.product(((),(0,),(0,0),(0,1),(0,1,0),(0,1,2,3)),range(8),range(2)):
  yield dict(entry='mesh',materials=indices,mode=(mode<<19)|(bank<<21),profiles=(0,0x200000,0x600000,0x1600000))
 for verts,limit,reject,bank in itertools.product((0,4,5,0x7FFFFFFF,0x80000000,0xFFFFFFFF),(4,0xFFFFFFFF),(0,1,0xFFFFFFFF),range(2)):
  yield dict(entry='mesh',vertices=verts,vertex_limit=limit,reject=reject,mode=bank<<21)
 for mode,bank,reject in itertools.product(range(8),range(2),(0,1)):
  yield dict(entry='filter',mode=(mode<<19)|(bank<<21),reject=reject)
 for layout,bank,env in itertools.product((0,0x200000,0x600000,0x1000000),range(2),(0,4)):
  yield dict(entry='mesh',layout=layout,env=env,and_mask=~0x200000&0xFFFFFFFF,or_mask=0x1000000,materials=(0,0,1),segment_bits=0xC000,after_env=0x20)
 for entry,bank in itertools.product(('mesh','mesh_wrapper','traversal'),range(2)):
  yield dict(entry=entry,type=7,flags=16,mode=bank<<21,flip_bank=True)
 for entry,rm,enable in itertools.product(('mesh','mesh_wrapper'),range(4),(0,1,2,4,8,16,32)):
  yield dict(entry=entry,layout=0x200000,rm=rm,enable=enable)
 for entry,variant,bank in itertools.product(('geometry','draw','primary','traversal'),range(2),range(2)):
  yield dict(entry=entry,graph='retail',variant=variant,mode=bank<<21)

def main():
 manifest_path,manifest,baseline,allowed,execute=setup()
 records=[];rows=set();edges=set();calls=set();counts=dict.fromkeys(SPECS,0);first={}
 total=dict(boundary_comparisons=0,numerical_operations=0,partial_guard_error_cases=0,independent_invariant_cases=0,external_service_boundaries=0)
 new_rows={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 wanted_calls={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']}
 def check(c,n):
  m,s,f=prepare(c,n);before=bytes(m);capture=[];c=dict(c,capture=capture)
  path,status,b,ops,_,invocations=execute(c,m,s,f,c['entry'],0xFEED1234,allowed)
  rows.update(new_rows&set(path));counts[c['entry']]+=1;calls.update(wanted_calls&set(path))
  for i,a in enumerate(path):
   if a not in new_rows:continue
   w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
   if w>>8 in (0x89,0x8B,0x8D,0x8F):
    j=i+(2 if w>>8 in (0x8D,0x8F) else 1)
    if j<len(path):edges.add((a,path[j]))
  total['boundary_comparisons']+=b;total['numerical_operations']+=ops;total['partial_guard_error_cases']+=status!=0;total['external_service_boundaries']+=invocations
  if status==0:
   assert len(capture)%2==0 and capture[:len(capture)//2]==capture[len(capture)//2:]
   if c['entry']=='filter':assert s.r[0]==c.get('reject',0)&0xFFFFFFFF
   signed=lambda v:v-0x100000000 if v&0x80000000 else v
   if c['entry']=='mesh' and signed(c.get('vertices',4))<=signed(c.get('vertex_limit',4)) and not c.get('reject',0):
    expected=len(c.get('materials',(0,)))
    assert sum(t==0x8C6069A4 for t,*_ in capture)==2*expected,(n,c,capture,expected)
   total['independent_invariant_cases']+=1
  c.pop('capture')
  if status==0:
   for target,*_ in capture:first.setdefault(target,dict(c))
  records.append(dict(input=c,steps=len(path),status=status,full_state_match=True,boundary_comparisons=b,independent_invariants=status==0))
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry='mesh',materials=()),dict(entry='mesh'),dict(entry='filter'),dict(entry='mesh',vertices=5),dict(entry='mesh',layout=0x200000),dict(entry='mesh',materials=(0,0,1),profiles=(0,0x600000)),dict(entry='draw',graph='retail'),dict(entry='mesh',null_service=True)]
 for n,c in enumerate(selected):
  check(c,n)
  if (n+1)%100==0:print(n+1,'mesh/material-loop comparisons passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Mesh smoke checks passed:',len(records));return
 assert set(first)==SERVICES,('services not reached',[hex(a) for a in SERVICES-set(first)])
 for target,c in sorted(first.items()):
  check(dict(c,missing_target=target),len(records));check(dict(c,fail_target=target),len(records))
 for entry in ('mesh','filter','mesh_wrapper'):check(dict(entry=entry,null_service=True),len(records))
 assert rows==new_rows,('rows',[hex(a) for a in new_rows-rows])
 assert edges=={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']},('edges missing',[list(map(hex,e)) for r in manifest['routines'] for e in [tuple(int(a,16) for a in x) for x in r['conditional_edges']] if e not in edges])
 assert calls==wanted_calls,('calls',wanted_calls-calls)
 files=[Path(__file__),Path(traversal.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_mesh.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_mesh_connected.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),**total,service_targets=sorted(hex(a) for a in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Full original material-loop control flow including four internal indirect jumps, filter wrapper, full RAM/CPU/FPU/special-state and ordered boundary comparisons; material-count/return assertions and connected original particle mesh inputs.',limitations=['27 lower filter/material/renderer/release services remain explicit fixtures; no actual primitive rendering, pixels, GPU/MMIO or full frame-driver proof.','Constructed valid materials/segments and bounded loops; old numerical guards and shared vector-oracle limitations retained.','R8 is intentionally an output/scratch register in the mesh body; its wrappers preserve it. Original literal jump targets are retained; arbitrary patched code/literals are outside scope.'],cases=records)
 (PROJECT/'analysis/main_dispatch_mesh_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered',*total)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_fmac=geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
