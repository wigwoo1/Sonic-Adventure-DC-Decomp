"""Recursive native model traversal versus original code and tree invariants."""
import ctypes,gc,hashlib,inspect,itertools,json,sys
from pathlib import Path
import verify_main_dispatch_particle_geometry as geom
from verify_tikal_controller import Call,P
PROJECT=Path(__file__).resolve().parent
particle=geom.particle;current=geom.current;base=geom.base;load,store=geom.load,geom.store
MODEL,LEAF_WRAPPER,LEAF=0x8C62113C,0x8C6215BA,0x8C6209A8
REUSED={0x8C63A878,0x8C63A69C,0x8C639CF4,0x8C639FF8}
NODES=tuple(0x8CC60000+i*0x100 for i in range(8));MESHES=tuple(0x8CC70000+i*0x100 for i in range(8))
MATRIX_POINTER=base.literal(0x8C62124C)
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_model_traversal.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(geom.SPECS,traversal=(MODEL,'sa_model_traversal_connected'),mesh_wrapper=(LEAF_WRAPPER,'sa_model_mesh_wrapper_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac'):
 fn=getattr(LIB,name);fn.argtypes=getattr(geom.LIB,name).argtypes;fn.restype=ctypes.c_int

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def leaf_effect(c,mem,r,sr,fp):
 if c.get('capture') is not None:c['capture'].append((r[4],load(mem,0x8C88F69C),load(mem,0x8C88F5F8),load(mem,MATRIX_POINTER)))
 for i in range(8):r[i]=0xBAD00000+i*0x101
 r[8]=0x8BADF00D
 if c.get('flip_bank'):fp.fpscr^=0x200000
 return sr^1

def setup():
 # Extend the previous proven executor locally; do not edit frozen evidence.
 source=inspect.getsource(geom.setup).replace('def setup():','def local_setup():',1)
 source=source.replace('particle.setup()','geom.setup()')
 source=source.replace('main_dispatch_particle_geometry_translation.json','main_dispatch_model_traversal_translation.json')
 source=source.replace('main_dispatch_particle_callback_verification.json','main_dispatch_particle_geometry_verification.json')
 source=source.replace('MODEL','LEAF')
 source=source.replace('base.TRACKED|={GEOMETRY,WRAPPER}', '''base.TRACKED|={MODEL,LEAF_WRAPPER}|REUSED
 for address in REUSED:
  listing=PROJECT/f'analysis/main-dispatch-model-traversal/{address:08x}_instructions.tsv'
  for line in listing.read_text().splitlines()[1:]:allowed.add(int(line.split('\\t')[0],16))''')
 source=source.replace('LEGACY=current.LEGACY|{particle.TRANSLATE}','LEGACY=current.LEGACY|{particle.TRANSLATE}|REUSED')
 marker=' for old,new in changes.items():'
 assert source.count(marker)==1
 source=source.replace(marker,""" changes['  if w&0xF00F==0xF005 and ef.fpscr&0x80000:stop(pc,r,sr,pr,-1)']='  if w==0xFBFD and ef.fpscr&0x80000:stop(pc,r,sr,pr,-1)\\n  if w&0xF00F==0xF005 and ef.fpscr&0x80000:stop(pc,r,sr,pr,-1)'
"""+marker)
 ns=dict(geom.__dict__)
 ns.update(PROJECT=PROJECT,LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,MODEL=MODEL,LEAF=LEAF,LEAF_WRAPPER=LEAF_WRAPPER,
           REUSED=REUSED,geom=geom,model_effect=leaf_effect)
 exec(compile(source,'model_traversal_setup','exec'),ns)
 return ns['local_setup']()

def graph(c):
 kind=c.get('graph','single');t=c.get('type',0);flags=c.get('flags',0)
 if kind=='retail':return None
 if kind=='single':
  # The flag can suppress a valid child; it never points to null when enabled.
  return [(t|flags,1 if not flags&16 else None,None),(7|8|16,None,None)]
 return [(t|flags,1,3),(2|16,None,2),(5|8|16,None,None),(7,4,None),(4|16,None,None)]

def prepare(c,n):
 m,s,f=geom.prepare(dict(c,expected_model=None),n)
 nodes=graph(c)
 if nodes is not None:
  for i,(flags,child,sibling) in enumerate(nodes):
   a=NODES[i]
   for off in range(0,0x34,4):store(m,a+off,0)
   store(m,a,flags);store(m,a+4,MESHES[i])
   for j,v in enumerate((0.5,-0.25,0.75)):store(m,a+8+j*4,current.bits(v))
   angles=c.get('angles',(0x1000,0x2000,0x3000))
   for j,v in enumerate(angles):store(m,a+0x14+j*4,v)
   for j,v in enumerate(c.get('scale',(1.0,0.5,2.0))):store(m,a+0x20+j*4,current.bits(v))
   store(m,a+0x2C,NODES[child] if child is not None else 0);store(m,a+0x30,NODES[sibling] if sibling is not None else 0)
 if c['entry'] in ('traversal','model_wrapper'):s.r[4]=NODES[0] if nodes is not None else particle.MODEL_A if c.get('variant') else particle.MODEL_B
 elif c['entry']=='mesh_wrapper':s.r[4]=MESHES[0]
 store(m,0x8C88F698,c.get('limit',16));store(m,MATRIX_POINTER,0xA5A55A5A)
 return m,s,f

def expected_leaf_order(c,m):
 if c['entry']=='mesh_wrapper':return [MESHES[0]]
 root=NODES[0] if graph(c) is not None else particle.MODEL_A if c.get('variant') else particle.MODEL_B
 out=[];visited=[]
 def visit(a):
  while a:
   assert a not in visited,'Constructed model graph must be acyclic';visited.append(a)
   flags=load(m,a)
   if not flags&8:out.append(load(m,a+4))
   if not flags&16:visit(load(m,a+0x2C))
   a=load(m,a+0x30)
 visit(root);return out

def cases():
 for t,mask,mode,rm in itertools.product(range(8),range(8),range(8),range(2)):
  yield dict(entry='traversal',type=t,flags=mask<<3,mode=mode<<19,rm=rm)
 for t,mask,bank in itertools.product(range(8),range(8),range(2)):
  yield dict(entry='model_wrapper',type=t,flags=mask<<3,mode=bank<<21,graph='tree')
 for entry,variant,mode,rm in itertools.product(('geometry','draw','primary','secondary','work'),range(2),range(8),range(2)):
  yield dict(entry=entry,graph='retail',variant=variant,mode=mode<<19,rm=rm)
 for entry,t,bank,failure in itertools.product(('traversal','model_wrapper','mesh_wrapper'),range(8),range(2),('missing','error')):
  yield dict(entry=entry,type=t,flags=16,mode=bank<<21,null_model=failure=='missing',fail_model=failure=='error')
 for t,bank,limit in itertools.product(range(8),range(2),(1,2)):
  yield dict(entry='traversal',type=t,flags=16,mode=bank<<21,limit=limit)
 for t,bank,rm in itertools.product(range(8),range(2),(2,3)):
  yield dict(entry='traversal',type=t,flags=16,mode=bank<<21,rm=rm)
 for bank,mode in itertools.product(range(2),(0,0x100000,0x80000)):
  yield dict(entry='mesh_wrapper',mode=mode|(bank<<21))
 for t,bank in itertools.product(range(8),range(2)):
  yield dict(entry='traversal',type=t,flags=16,mode=bank<<21,flip_bank=True)

def main():
 manifest_path,manifest,baseline,allowed,execute=setup()
 records=[];rows=set();edges=set();calls=set();counts=dict.fromkeys(SPECS,0)
 total=dict(boundary_comparisons=0,numerical_operations=0,partial_guard_error_cases=0,independent_invariant_cases=0,mesh_service_invocations=0)
 new_rows={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 expected_calls={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']}
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry='traversal',type=t,flags=16) for t in range(8)]+[dict(entry='model_wrapper',graph='tree',type=0),dict(entry='draw',graph='retail'),dict(entry='traversal',type=7,flags=16,mode=0x100000),dict(entry='mesh_wrapper',null_model=True)]
 for n,c in enumerate(selected):
  m,s,f=prepare(c,n);before=bytes(m);capture=[];c=dict(c,capture=capture,expected_model=None)
  path,status,b,ops,_,invocations=execute(c,m,s,f,c['entry'],0xFEED1234,allowed)
  rows.update(new_rows&set(path));counts[c['entry']]+=1;calls.update(expected_calls&set(path))
  for i,a in enumerate(path):
   if a not in new_rows:continue
   w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
   if w>>8 in (0x89,0x8B,0x8D,0x8F):
    j=i+(2 if w>>8 in (0x8D,0x8F) else 1)
    if j<len(path):edges.add((a,path[j]))
  total['boundary_comparisons']+=b;total['numerical_operations']+=ops;total['partial_guard_error_cases']+=status!=0
  total['mesh_service_invocations']+=invocations
  if status==0:
   want=expected_leaf_order(c,before)
   # The capture records reference and native calls in separate equal halves.
   assert [v[0] for v in capture]==want+want,(c,capture,want)
   if c['entry'] in ('traversal','model_wrapper') and not c.get('flip_bank') and c.get('limit',16)>=8:
    assert load(m,0x8C88F69C)==load(before,0x8C88F69C)
    assert load(m,0x8C88F5F8)==load(before,0x8C88F5F8)
   total['independent_invariant_cases']+=1
  c.pop('capture');records.append(dict(input=c,steps=len(path),status=status,full_state_match=True,boundary_comparisons=b,independent_invariants=status==0))
  if (n+1)%100==0:print(n+1,'recursive model traversal comparisons passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Model traversal smoke checks passed:',len(records));return
 unreachable_rows={int(a,16) for r in manifest['routines'] for a in r.get('structurally_unreachable_rows',[])}
 unreachable_edges={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r.get('structurally_unreachable_edges',[])}
 assert rows==new_rows-unreachable_rows,('rows',[hex(a) for a in (new_rows-unreachable_rows)-rows])
 assert edges=={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}-unreachable_edges,('edges',edges)
 assert calls==expected_calls,('calls',expected_calls-calls)
 files=[Path(__file__),Path(geom.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_model_traversal.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_model_traversal_connected.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 files.extend(Path(p) for p in manifest['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),static_instruction_rows=len(new_rows),structurally_unreachable_rows=sorted(hex(a) for a in unreachable_rows),conditional_edges_covered=len(edges),structurally_unreachable_edges=sorted([hex(a) for a in e] for e in unreachable_edges),call_sites_covered=len(calls),**total,sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Recursive original model traversal and mesh wrapper with existing native transforms, full-state and boundary comparisons, independent depth-first mesh order and matrix-stack restoration, including unmodified original particle model nodes.',limitations=['Mesh body 8c6209a8 is an explicit state-mutating fixture; no visible rendering, GPU/MMIO or full frame ordering claimed.','Register translation dependency uses its existing implicit branch only; traversal always passes R4=0. Other existing transform source reused without new routine credit.','Constructed acyclic graphs and bounded recursion; arbitrary invalid pointers/cycles, hardware exceptions and complete game behavior remain unverified.','Inherits shared vector numerical oracle, native guard-probe and finite scalar limits.'],cases=records)
 (PROJECT/'analysis/main_dispatch_model_traversal_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','static_instruction_rows','conditional_edges_covered','call_sites_covered',*total)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_fmac=geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
