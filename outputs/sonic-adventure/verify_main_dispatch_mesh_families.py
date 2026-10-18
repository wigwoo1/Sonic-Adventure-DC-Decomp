"""Combined native material families and original live-register strip contracts."""
import ctypes,gc,hashlib,itertools,json,math,sys
from pathlib import Path
import verify_main_dispatch_environment_constant_alternate_strips as previous
PROJECT=Path(__file__).resolve().parent
base,current,mesh,services=previous.base,previous.current,previous.mesh,previous.services
load,store=previous.load,previous.store
Bus,Memory,QueueReference=previous.Bus,previous.Memory,previous.QueueReference
CACHE,PACKETS,SEED=previous.CACHE,previous.PACKETS,previous.SEED
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_mesh_families.dll'
LIB=ctypes.CDLL(str(DLL));SPECS=previous.SPECS
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for name,fn in FUNCTIONS.items():fn.argtypes=previous.FUNCTIONS[name].argtypes;fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(previous.LIB,name).argtypes;fn.restype=getattr(previous.LIB,name).restype
FAMILIES={
 'flat':(0x2000000,previous.PLAIN,previous.CLIPPED,previous.NEAR,previous.ALTERNATE,previous.ALT_CLIP),
 'lit':(0,previous.LIT,previous.LIT_CLIP,previous.LIT_NEAR,previous.LIT_ALT,previous.LIT_ALT_CLIP),
 'uv_lit':(0x200000,previous.UV_LIT,previous.UV_LIT_CLIP,previous.UV_NEAR,previous.UV_ALT,previous.UV_ALT_CLIP),
 'environment':(0x600000,previous.ENV,previous.ENV_CLIP,previous.ENV_NEAR,previous.ENV_ALT,previous.ENV_ALT_CLIP),
 'face':(0x1000000,previous.FACE,previous.FACE_CLIP,previous.FACE_NEAR,previous.FACE_ALT,previous.FACE_ALT_CLIP),
 'uv_face':(0x1200000,previous.UV_FACE,previous.UV_FACE_CLIP,previous.UV_FACE_NEAR,previous.UV_FACE_ALT,previous.UV_FACE_ALT_CLIP),
 'uv_constant':(0x2200000,previous.UV_CONSTANT,previous.UV_CONSTANT_CLIP,previous.UV_CONSTANT_NEAR,previous.UV_CONSTANT_ALT,previous.UV_CONSTANT_ALT_CLIP),
 'environment_face':(0x1600000,previous.ENV_FACE,previous.ENV_FACE_CLIP,previous.ENV_FACE_NEAR,previous.ENV_FACE_ALT,previous.ENV_FACE_ALT_CLIP),
 'environment_constant':(0x2600000,previous.ENV_CONSTANT,previous.ENV_CONSTANT_CLIP,previous.ENV_CONSTANT_NEAR,previous.ENV_CONSTANT_ALT,previous.ENV_CONSTANT_ALT_CLIP),
}
READS={0x8c61bca2:'face',0x8c61cdc6:'uv_face',0x8c61ef72:'environment_face'}
TARGETS={v for family in FAMILIES.values() for v in family[1:]}|{previous.NORMALIZE}
UVS=0x8cdf0000

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def setup():
 *_,allowed,execute=previous.setup()
 baseline=PROJECT/'analysis/main_dispatch_environment_constant_alternate_strips_verification.json'
 report=json.loads(baseline.read_text())
 for p,h in report['sha256'].items():assert digest(Path(p))==h,p
 assert not previous.SERVICES
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS)
 return baseline,allowed,execute

def prepare(c,n):
 order=c['families'];profiles=tuple(FAMILIES[name][0] for name in order)
 fixture=dict(c,entry='mesh',connected_triangle=True,environment_constant_mesh=True,screen_outside=False,normals=True,clip=False,acquisition_mesh=True,materials=tuple(range(len(order))),profiles=profiles,material_extra=c.get('secondary',0)<<20,layout=0)
 m,s,f=previous.previous.prepare(fixture,n)
 # Original fields and layouts are repeated into separate material and segment
 # records, while one actual filter pass supplies a shared four-vertex cache.
 template=bytes(m[(mesh.MATERIALS&0xffffff):(mesh.MATERIALS&0xffffff)+20])
 store(m,mesh.DATA+8,4)
 for i,name in enumerate(order):
  a=mesh.MATERIALS+i*20;m[(a&0xffffff):(a&0xffffff)+20]=template
  store(m,a+16,FAMILIES[name][0]|(c.get('secondary',0)<<20))
  segment=mesh.SEGMENTS+i*24
  store(m,segment,c['topology']|i,2);store(m,segment+2,len(c.get('groups',((0,1,2,3),))),2)
  stream=previous.INDICES+i*0x100;store(m,segment+4,stream);store(m,segment+20,UVS+i*0x100)
  indices=[]
  for group in c.get('groups',((0,1,2,3),)):
   if c['topology']==0:indices.extend(group[:3])
   elif c['topology']==0x4000:indices.extend(group[:4])
   else:indices.extend((len(group),*group))
  for j,v in enumerate(indices+[0,1,2,3,0,1,2,3]):store(m,stream+j*2,v,2)
  for j in range(24):
   pair=((0,0),(256,0),(0,256),(-256,512))[j%4]
   for k,v in enumerate(pair):store(m,UVS+i*0x100+j*4+k*2,v&0xffff,2)
 points=((0.,0.),(4.,0.),(0.,4.),(4.,4.))
 normals=((0.25,0.5,0.75),(0.5,-0.25,0.125),(-0.5,0.25,0.5),(1.,-0.5,0.25))
 for i,normal in enumerate(normals):
  for j,v in enumerate(normal):store(m,0x8cd60000+i*12+j*4,current.bits(v))
 for i,(x,y) in enumerate(points):
  if c.get('reverse'):x=-x
  for j,v in enumerate((x,y,0.5 if c['near_mask']&(1<<i) else 4.)):store(m,0x8cd50000+i*12+j*4,current.bits(v))
 store(m,0x8c88f658,0xffffffff);store(m,0x8c88f65c,0xfffffffe)
 return fixture,m,s,f

def snapshot(target,mem,r):
 extra=tuple(load(mem,r[4]+i*4) for i in range(3)) if target==previous.NORMALIZE else ()
 return (target,tuple(r[4:7]),tuple(r[12:15]),extra,r[8])

def check(c,n,allowed,execute):
 fixture,m,s,f=prepare(c,n);ref=QueueReference();native=Memory();bus=Bus()
 LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus))
 for i in range(16):native.queue[i]=ref.queue[i]=SEED[i]
 execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
 original_reference=base.previous.reference;reference_events=[];native_events=[];reads=[]
 def reference(mem,state,*args,trace=None,**kwargs):
  last=[]
  def observed(pc,w,r,sr,pr,fp):
   last[:]=[hex(pc),hex(r[8]),hex(r[12]),hex(r[13]),hex(r[14])]
   if pc in TARGETS:reference_events.append(snapshot(pc,mem,r))
   if pc in READS:
    pointers=tuple(r[i] for i in (12,14,13))
    reads.append(dict(normalizer_event_index=len(reference_events),pc=hex(pc),family=READS[pc],pointers=[hex(a) for a in pointers],normal_words=[[load(mem,a+j*4) for j in range(3)] for a in pointers]))
   trace(pc,w,r,sr,pr,fp)
  try:return original_reference(mem,state,*args,trace=observed,**kwargs)
  except Exception as exc:raise AssertionError(c,last,reference_events,ref.qacr) from exc
 real_function=FUNCTIONS['mesh']
 errors=[]
 def wrapper(*args):
  outer=args[-2]
  @base.OBSERVE
  def observe(ctx,target,mem,fp,sr,r,pr):
   try:
    if target in TARGETS:native_events.append(snapshot(target,mem,r[:16]))
    outer(ctx,target,mem,fp,sr,r,pr)
   except Exception as exc:errors.append(repr(exc))
  return real_function(*args[:-2],observe,args[-1])
 base.previous.reference=reference;execute.__globals__['FUNCTIONS']=dict(FUNCTIONS,mesh=wrapper)
 try:path,status,b,ops,_,external=execute(fixture,m,s,f,'mesh',0xfeed1234,allowed)
 finally:base.previous.reference=original_reference;execute.__globals__['FUNCTIONS']=FUNCTIONS
 assert not errors and native_events==reference_events,(c,'native/reference snapshots',errors)
 previous.flat_alternate.previous.previous.acquisition.compare_queue(native,ref)
 return dict(input=c,status=status,full_state_match=True,queue_match=True,steps=len(path),boundary_comparisons=b,numerical_operations=ops,external_service_boundaries=external,queue_flushes=native.flushes,queue_events=native.trace_count+native.trace_dropped,targets=[hex(a) for a,*_ in reference_events],snapshots=reference_events,strip_reads=reads),path,m,s

def cases():
 names=tuple(FAMILIES)
 for order,topology,mask,reverse,bank,secondary,offset in itertools.product((names,names[::-1]),(0,0x4000,0x8000,0xc000),(0,1,3,7,8,14,15),(False,True),range(2),range(2),(0,32)):
  yield dict(entry='mesh',families=order,topology=topology,near_mask=mask,reverse=reverse,mode=bank<<21,secondary=secondary,cursor_offset=offset,kind='combined')
 scenarios=(('first_crossing',1,((0,1,2,3),)),('ordinary_then_crossing',8,((0,1,2),(1,2,3))),('crossing_then_crossing',1,((0,1,2,3),(0,1,2))))
 for family,(label,mask,groups),reverse,bank,secondary,offset in itertools.product(('face','uv_face','environment_face'),scenarios,(False,True),range(2),range(2),(0,32)):
  yield dict(entry='mesh',families=(family,),topology=0x8000,near_mask=mask,reverse=reverse,mode=bank<<21,secondary=secondary,cursor_offset=offset,groups=groups,kind=label)


def independently_check(c,record,m):
 events=record['snapshots'];parents=[e for e in events if e[0] in {a for f in FAMILIES.values() for a in f[1:3]}]
 clipped=bool(c['near_mask'])
 wanted=[] if c['near_mask']==15 else [FAMILIES[name][2 if clipped else 1] for name in c['families']]
 assert [e[0] for e in parents]==wanted,(c,'material family selection')
 if c['near_mask']==15:
  assert record['queue_flushes']==4 and load(m,0x8c88f658)==0xffffffff
  return 'whole_mesh_reject'
 vertex_count={0:3,0x4000:4}.get(c['topology'],sum(len(g) for g in c.get('groups',((0,1,2,3),))))
 assert load(m,0x8c88f658)==(0xffffffff+len(c['families'])*vertex_count)&0xffffffff,(c,'input counter')
 if c['kind']!='combined':
  reads=record['strip_reads'];label=c['kind']
  expected=([hex(mesh.SEGMENTS),hex(mesh.STATE),hex(mesh.MATERIALS)] if label=='first_crossing' else
            [hex(CACHE+i*32) for i in ((0,1,2) if label=='ordinary_then_crossing' else (2,3,3))])
  assert reads and reads[-1]['pointers']==expected,(c,'original inherited pointer state',reads,expected)
  if label=='crossing_then_crossing':assert reads[0]['pointers']==[hex(mesh.SEGMENTS),hex(mesh.STATE),hex(mesh.MATERIALS)] and len(reads)==2
  else:assert len(reads)==1
  for read in reads:
   event=events[read['normalizer_event_index']];assert event[0]==previous.NORMALIZE
   words=read['normal_words'];actual=event[3]
   for j in range(3):
    values=[current.value(row[j]) for row in words]
    # NaN material bytes are deliberately preserved. Their classification,
    # not a platform-dependent host NaN payload, is the independent assertion.
    if any(math.isnan(v) for v in values):assert math.isnan(current.value(actual[j]))
    else:
     rounded=lambda v:current.value(current.bits(v))
     want=current.bits(rounded(rounded(values[0]+values[1])+values[2]))
     assert actual[j]==want,(c,'pre-normalizer sum',read,j,hex(actual[j]),hex(want))
  return 'live_register_contract'
 if c['topology']!=0:return 'selection_counter'
 points=[((-x if c['reverse'] else x),y,0.5 if c['near_mask']&(1<<i) else 4.) for i,(x,y) in enumerate(((0.,0.),(4.,0.),(0.,4.)))]
 polygon=previous.previous.uv_clip.polygon(dict(c,cull=1),points,[0.,0.,0.],[(0,0)]*3)[0] if c['reverse'] else []
 if len(polygon)==4:polygon=[polygon[i] for i in (0,1,3,2)]
 emitted=len(polygon)
 assert record['queue_flushes']==4+len(parents)*(1+emitted),(c,'triangle queue transfers',record['queue_flushes'],emitted)
 for event in parents:
  cursor=event[4]
  for i,(x,y,z,*_) in enumerate(polygon):
   a=cursor+i*32;assert load(m,a)==(0xf0000000 if i==emitted-1 else 0xe0000000)
   got=[current.value(load(m,a+4+j*4)) for j in range(3)];want=(x/z,y/z,1/z)
   assert all(math.isclose(a,b,rel_tol=1e-5,abs_tol=1e-4) for a,b in zip(got,want)),(c,'triangle geometry',got,want)
 return 'triangle_geometry'


def main():
 baseline,allowed,execute=setup()
 records=[];totals=dict(boundary_comparisons=0,numerical_operations=0,external_service_boundaries=0,queue_flushes=0,queue_events=0)
 targets=set();independent={};selected=list(cases())
 if '--smoke' in sys.argv:selected=selected[:16]+[c for c in selected if c['kind']!='combined'][:16]
 for n,c in enumerate(selected):
  record,path,m,s=check(c,n,allowed,execute)
  assert record['status']==0 and record['external_service_boundaries']==0,(c,record['status'])
  invariant=independently_check(c,record,m);record['independent_check']=invariant
  independent[invariant]=independent.get(invariant,0)+1
  targets.update(int(a,16) for a in record['targets'])
  for k in totals:totals[k]+=record[k]
  records.append(record)
  if (n+1)%100==0:print(n+1,'combined native mesh-family cases passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Combined mesh-family smoke passed:',len(records));return
 assert targets==TARGETS,('missing native targets',[hex(a) for a in sorted(TARGETS-targets)])
 assert sum(c['input']['kind']=='combined' for c in records)==896 and len(records)==1040
 assert sum(c['input']['kind']!='combined' for c in records)==144
 files=[Path(__file__),Path(previous.__file__),baseline,PROJECT/'build_main_dispatch_mesh_families.ps1',PROJECT/'source/sa_main_dispatch_environment_constant_alternate_strips_connected.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(cases_passed=len(records),combined_cases=896,caller_contract_cases=144,material_families=len(FAMILIES),native_targets_covered=len(targets),targets=sorted(hex(a) for a in targets),independent_checks=independent,**totals,sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),newly_recovered_entries=0,
 scope='All nine native mesh material families in forward/reverse mixed order, four topology codes, selected four-vertex near masks, primary/secondary queues, both FPU banks and cursor halves. Original full RAM/CPU/FPU/boundary/queue comparisons plus independent dispatch/input-counter, triangle geometry and real-caller strip normal-input checks. No fixture or register seeding.',
 findings=['At the first 8000-topology near-crossing group, face/UV-face/environment-face read R12=segment, R14=renderer state and R13=material, rather than vertex-cache pointers. Constructed material bytes include a signaling NaN when interpreted as a normal; original and native numerical backends propagate NaN. This is observed original behavior, not a repaired caller contract.',
 'An ordinary group followed by a crossing group uses the previous group first-three vertex pointers for crossing face lighting. Two crossing groups inherit cache pointers 2,3,3 on the second group. Geometry arguments are subsequently loaded from the current stream. Separate controlled seeding was not required in the real material-loop tests.'],
 limitations=['Constructed metadata and simulated coherent-RAM/MMU-off queues only. No proof that these particular 8000 face-lighting combinations occur in shipped assets; no hardware numerical/NaN-payload, GPU pixel, cache-timing, full-frame or gameplay proof.',
 'Full original-state comparisons use the existing numerical backend and guards. Independent geometry covers triangle topology only; other topology output has original-instruction comparisons and selection/counter assertions. No recovered source is changed or newly credited.'],cases=records)
 (PROJECT/'analysis/main_dispatch_mesh_families_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','combined_cases','caller_contract_cases','material_families','native_targets_covered','independent_checks',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
