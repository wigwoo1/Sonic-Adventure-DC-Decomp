"""Actual native traversal-to-matrix-to-mesh calls with constructed trees."""
import collections,ctypes,hashlib,itertools,json,sys
from pathlib import Path
import verify_draw_matrix_renderer as renderer
PROJECT=Path(__file__).resolve().parent
v=renderer.previous.previous;traversal=v.mesh.traversal
SCALE,TRANSLATE=0x8c63a69c,0x8c63a878
LEAF=0x8c6209a8
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 baseline,allowed,execute=renderer.setup();selected=list(itertools.product(range(8),('single','tree'),range(2),(0,32)))
 if '--smoke' in sys.argv:selected=[(0,'single',0,0),(0,'tree',1,32),(7,'tree',0,0)]
 records=[];totals=collections.Counter();targets=collections.Counter()
 for n,(t,graph,bank,rotation) in enumerate(selected):
  c=dict(entry='traversal',type=t,flags=16|rotation if graph=='single' else rotation,graph=graph,mode=bank<<21,expected_model=None,normals=True,clip=False,acquisition_mesh=True,profiles=(0x2000000,),connected_triangle=True)
  m,s,f=v.prepare(c,n);before=bytes(m);order=traversal.expected_leaf_order(c,before)
  depth=v.load(m,0x8c88f69c);pointer=v.load(m,0x8c88f5f8)
  ref=v.QueueReference();native=v.Memory();bus=v.Bus();renderer.LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus))
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  observed=[];errors=[];real=renderer.FUNCTIONS['traversal']
  def wrapper(*args):
   outer=args[-2]
   @v.base.OBSERVE
   def observe(ctx,target,mem,fp,sr,r,pr):
    try:
     if target in (SCALE,TRANSLATE,LEAF):observed.append((target,r[4]))
     outer(ctx,target,mem,fp,sr,r,pr)
    except Exception as exc:errors.append(repr(exc))
   return real(*args[:-2],observe,args[-1])
  execute.__globals__['FUNCTIONS']=dict(renderer.FUNCTIONS,traversal=wrapper)
  try:path,status,b,ops,_,external=execute(c,m,s,f,'traversal',0xfeed1234,allowed)
  finally:execute.__globals__['FUNCTIONS']=renderer.FUNCTIONS
  assert not errors and status==0 and external==0,(c,status,external,errors)
  v.flat_alternate.previous.previous.acquisition.compare_queue(native,ref)
  assert [a for target,a in observed if target==LEAF]==order,(c,'independent preorder mesh addresses')
  assert all(a==0 for target,a in observed if target in (SCALE,TRANSLATE)),(c,'original implicit matrix ABI')
  assert v.load(m,0x8c88f69c)==depth and v.load(m,0x8c88f5f8)==pointer,(c,'independent stack restoration')
  for target in (SCALE,TRANSLATE):
   count=path.count(target);assert sum(a==target for a,_ in observed)==count;targets[hex(target)]+=count
  totals.update(dict(boundary_comparisons=b,numerical_operations=ops,mesh_calls=path.count(LEAF),queue_flushes=native.flushes,queue_events=native.trace_count+native.trace_dropped))
  records.append(dict(input=c,status=status,full_state_match=True,queue_match=True,independent_mesh_order_and_stack=True,external_service_boundaries=external,observed_calls=[[hex(t),hex(a)] for t,a in observed],steps=len(path)))
  if (n+1)%16==0:print(n+1,'transformed native model callers passed',flush=True)
 if '--smoke' in sys.argv:print('Transformed native model smoke passed:',len(records));return
 assert all(targets[hex(a)] for a in (SCALE,TRANSLATE))
 files=[Path(__file__),PROJECT/'verify_draw_matrix_renderer.py',baseline,Path(v.__file__),Path(traversal.__file__),renderer.DLL]
 for name in ('draw_matrix_renderer_connection.json','draw_matrix_registers_verification.json'):
  proof=PROJECT/'analysis'/name;files.append(proof);files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 report=dict(cases_passed=len(records),matrix_invocations=dict(targets),**totals,external_game_service_fixtures=0,independent_cases=len(records),newly_recovered_entries=0,reused_routine_credit=0,sha256={str(p):digest(p) for p in files},dll_sha256=digest(renderer.DLL),scope='All eight original traversal transform selectors, single/five-node trees, both FPU banks and both Euler-order flags through complete native matrix helpers, mesh projection/material/flat primitive and queues. Original full-state/ordered-boundary/queue comparisons, independent mesh-address preorder and matrix-stack restoration.',limitations=['Constructed nodes and valid four-vertex flat mesh inputs, not proof of natural scene execution. Caller always supplies R4=0; explicit matrix branches have separate focused checks.','This renderer build exposes the legacy aggregate status ABI; exact helper stop-PC proof is from the direct guarded API. Numerical/backend/coherent-RAM and hardware/pixel limitations remain.'],cases=records)
 (PROJECT/'analysis/draw_matrix_model_callers_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','matrix_invocations','boundary_comparisons','numerical_operations','mesh_calls','queue_flushes','queue_events')},indent=2))
if __name__=='__main__':main()
