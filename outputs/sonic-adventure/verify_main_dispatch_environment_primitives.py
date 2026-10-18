"""Normal-derived environment UV family versus original code and packet math."""
import ctypes,gc,hashlib,inspect,itertools,json,sys
from pathlib import Path
import verify_main_dispatch_uv_alternate_strips as previous
import verify_main_dispatch_lit_primitives as lit_parent
import verify_main_dispatch_uv_lit_primitives as uv_parent
import verify_main_dispatch_acquisition as acquisition
PROJECT=Path(__file__).resolve().parent
vertex=previous.vertex;services=previous.services;mesh=previous.mesh;particle=previous.particle;current=previous.current;base=previous.base
load,store=previous.load,previous.store
Bus,Memory,QueueReference,WORKERS=previous.Bus,previous.Memory,previous.QueueReference,previous.WORKERS
PLAIN,CLIPPED,NEAR,ALTERNATE,ALT_CLIP=previous.PLAIN,previous.CLIPPED,previous.NEAR,previous.ALTERNATE,previous.ALT_CLIP
SECONDARY,EXTENDED,EXT_SECONDARY=previous.SECONDARY,previous.EXTENDED,previous.EXT_SECONDARY
LIT,LIT_CLIP,LIT_NEAR=previous.LIT,previous.LIT_CLIP,previous.LIT_NEAR
LIT_ALT,LIT_ALT_CLIP=previous.LIT_ALT,previous.LIT_ALT_CLIP
UV_LIT,UV_LIT_CLIP,UV_NEAR=previous.UV_LIT,previous.UV_LIT_CLIP,previous.UV_NEAR
UV_ALT,UV_ALT_CLIP=previous.UV_ALT,previous.UV_ALT_CLIP
ENV,ENV_CLIP=0x8C619C34,0x8C61A488
ENV_NEAR,ENV_ALT,ENV_ALT_CLIP=0x8C611F24,0x8C613BE2,0x8C6153BC
KINDS=("environment","environment_clipped")
LOWER={ENV_NEAR,ENV_ALT,ENV_ALT_CLIP}
SERVICES=(previous.SERVICES-{ENV,ENV_CLIP})|LOWER
SEGMENT,INDICES,CACHE,PACKETS=lit_parent.SEGMENT,lit_parent.INDICES,previous.CACHE,previous.PACKETS
SEED=previous.SEED;MATRIX=0x8CDF1000
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_environment_primitives.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(previous.SPECS,environment=(ENV,'sa_environment_primitive_emit_connected'),environment_clipped=(ENV_CLIP,'sa_environment_primitive_clipped_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE,ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(previous.LIB,name).argtypes;fn.restype=getattr(previous.LIB,name).restype

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def effect(c,target,mem,r,sr,fp):
 if target not in LOWER:return mesh.effect(c,target,mem,r,sr,fp)
 if c.get('capture') is not None:c['capture'].append((target,r[4],r[5],r[6],load(mem,mesh.FLAGS)))
 for i in range(8):r[i]=0xBAD00000+i*0x101
 bank=(fp.fpscr>>21)&1
 for i in range(6):fp.banks[bank][i]=current.bits(0.125*(i+1))
 r[8]=(r[8]+c.get('fixture_advance',0))&0xFFFFFFFF
 return sr^1

def setup():
 source=inspect.getsource(previous.setup).replace('def setup():','def local_setup():',1)
 needle='new=old[:-1]+'+repr(',"uv_alternate","uv_alternate_clipped")')
 replacement='new=old[:-1]+'+repr(',"uv_alternate","uv_alternate_clipped","environment","environment_clipped")')
 assert source.count(needle)==1;source=source.replace(needle,replacement)
 ns=dict(previous.__dict__);exec(compile(source,'environment_setup','exec'),ns)
 *_,allowed,execute=ns['local_setup']()
 manifest_path=PROJECT/'analysis/main_dispatch_environment_primitives_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_uv_alternate_strips_verification.json'
 for item in (manifest,json.loads(baseline.read_text())):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 allowed|={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']};base.TRACKED|={ENV,ENV_CLIP}
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,SERVICES=SERVICES,effect=effect)
 return manifest_path,manifest,baseline,allowed,execute

def prepare(c,n):
 if c['entry'] in KINDS:
  m,s,f=lit_parent.prepare(dict(c,entry='lit' if c['entry']=='environment' else 'lit_clipped'),n)
  for a in (0x8C890260,0x8C890264):store(m,a,current.bits(1.))
 else:
  m,s,f=previous.previous.prepare(c,n)
  if c.get('environment_mesh'):
   if c['screen_outside']:store(m,0x8CD50000,current.bits(100. if c['reverse'] else -100.))
   for i,normal in enumerate(((0.25,0.5,0.75),(0.5,-0.25,0.125),(-0.5,0.25,0.5),(0.25,0.5,0.75))):
    for j,v in enumerate(normal):store(m,0x8CD60000+i*12+j*4,current.bits(v))
   offset=c.get('cursor_offset',0)
   store(m,services.HEADER+16,PACKETS+offset);store(m,services.HEADER+20,0x8CDE0000+offset)
 store(m,0x8C890238,MATRIX)
 values=c.get('environment_matrix',(1.,0.25,0.,0.,-0.5,1.,0.,0.,0.25,-0.5,1.,0.,0.,0.,0.,1.))
 for i,v in enumerate(values):
  store(m,MATRIX+i*4,current.bits(v))
  # The real filter publishes the existing matrix-stack buffer at 8ce10000.
  if c.get('environment_mesh'):store(m,0x8CE10000+i*4,current.bits(v))
 return m,s,f

def cases():
 for c in lit_parent.cases():
  if c.get('lit_mesh'):continue
  yield dict(c,entry={'lit':'environment','lit_clipped':'environment_clipped'}.get(c['entry'],c['entry']))
 for entry,matrix,reverse,bank in itertools.product(KINDS,((1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.),(-2.,0.5,0.,0.,1.,-2.,0.,0.,-0.5,1.,1.,0.,0.,0.,0.,1.)),(False,True),range(2)):
  yield dict(entry=entry,environment_matrix=matrix,reverse=reverse,mode=bank<<21)
 for entry,reverse,bank in itertools.product(KINDS,(False,True),range(2)):
  yield dict(entry=entry,count=2,indices=(0,1,2,0,2,1),reverse=reverse,mode=bank<<21,cursor=PACKETS+32)
 for (mask,outside),reverse,bank,secondary,offset in itertools.product(((0,False),(0,True),(1,False)),(False,True),range(2),range(2),(0,32)):
  yield dict(entry='mesh',environment_mesh=True,connected_triangle=True,near_mask=mask,screen_outside=outside,reverse=reverse,mode=bank<<21,normals=True,clip=False,acquisition_mesh=True,material_extra=0x600000|(secondary<<20),cursor_offset=offset)

def environment_uv(m,index):
 rounded=lambda v:current.value(current.bits(v))
 normal=[current.value(load(m,CACHE+index*32+j*4)) for j in range(3)]
 matrix=[current.value(load(m,load(m,0x8C890238)+j*4)) for j in range(16)];result=[]
 for column in (0,1):
  value=rounded(normal[1]*matrix[4+column])
  value=rounded(value+normal[0]*matrix[column])
  value=rounded(value+normal[2]*matrix[8+column])
  result.append(current.bits(rounded(0.5+0.5*value)))
 return result

def independent(c,m,before,s,native,captured):
 connected=c.get('environment_mesh',False)
 if c['entry'] not in KINDS and not connected:return False
 if c.get('mode',0)&0x180000 or not connected and c.get('topology',0)!=0:return False
 count=1 if connected else c.get('count',1);cache=m if connected else before
 if connected:
  assert load(m,0x8C890238)==0x8CE10000
  assert all(load(m,0x8CE10000+j*4)==load(before,MATRIX+j*4) for j in range(16))
 start=((0x8CDE0000 if c['material_extra']&0x100000 else PACKETS)+c.get('cursor_offset',0)+32) if connected else c.get('cursor',PACKETS)
 records=[];near_calls=[]
 for triangle in range(count):
  ids=(0,1,2) if connected else tuple(load(before,INDICES+(triangle*3+i)*2,2) for i in range(3))
  xy=[tuple(current.value(load(cache,CACHE+i*32+16+j*4)) for j in range(2)) for i in ids]
  area=(xy[1][0]-xy[0][0])*(xy[2][1]-xy[0][1])-(xy[1][1]-xy[0][1])*(xy[2][0]-xy[0][0])
  flags=[load(cache,CACHE+i*32+12) for i in ids]
  clipped=c['entry']=='environment_clipped' or connected and (c['near_mask'] or c['screen_outside'])
  rejected=clipped and bool(flags[0]&flags[1]&flags[2]&63)
  near=clipped and not rejected and bool((flags[0]|flags[1]|flags[2])&1)
  if near:near_calls.append(tuple(CACHE+i*32 for i in ids))
  elif not rejected and area<=0:
   for local,index in enumerate(ids):records.append((index,local))
 assert len(captured)==2*len(near_calls),(c,captured,near_calls)
 if near_calls:assert [event[1:4] for event in captured]==near_calls*2 and all(event[0]==ENV_NEAR for event in captured)
 emitted=len(records)
 if not connected:
  assert load(m,0x8C88F658)==(0xFFFFFFFF+3*count)&0xFFFFFFFF
  assert load(m,0x8C88F65C)==(0xFFFFFFFE+emitted)&0xFFFFFFFF
  assert all(load(m,CACHE+i*32+28)==load(before,CACHE+i*32+28) for i in range(8))
 assert native.flushes==emitted+(4 if connected else 0),(c,'transfers',native.flushes,emitted)
 assert s.r[8]==((start&0x03FFFFFF)|0xE0000000)+32*emitted
 retained=[SEED[7],SEED[15]]
 if connected:retained[((start-32)>>5)&1]=load(m,mesh.STATE+28)
 for i,(index,local) in enumerate(records):
  destination=start+i*32;factor=lit_parent.lighting_factor(cache,index)
  packet=[0xF0000000 if local==2 else 0xE0000000]+[load(cache,CACHE+index*32+16+j*4) for j in range(3)]+environment_uv(cache,index)+[current.bits(factor),retained[(destination>>5)&1]]
  assert [load(m,destination+j*4) for j in range(8)]==packet,(c,'independent normal-derived UV packet',i)
 if connected:assert load(m,services.HEADER+(20 if c['material_extra']&0x100000 else 16))==start+32*emitted
 return True

def main():
 manifest_path,manifest,baseline,allowed,execute=setup()
 wanted={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']};rows=set();edges=set();calls=set();records=[];counts={};first={}
 totals=dict(boundary_comparisons=0,numerical_operations=0,partial_cases=0,independent_cases=0,queue_flushes=0,queue_events=0,external_service_boundaries=0)
 def check(c,n):
  m,s,f=prepare(c,n);before=bytes(m);ref=QueueReference();native=Memory();bus=Bus()
  LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus))
  for i in range(16):native.queue[i]=ref.queue[i]=SEED[i]
  for i in range(16):native.queue[i]=ref.queue[i]=SEED[i]
  if c['entry'] in KINDS:native.qacr[:]=ref.qacr[:]=[12,12]
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  captured=[];case=dict(c,capture=captured)
  path,status,b,ops,_,external=execute(case,m,s,f,c['entry'],0xFEED1234,allowed)
  if c.get('environment_mesh'):
   assert status==0 and path.count(ENV)+path.count(ENV_CLIP)==1
   assert path.count(ENV_CLIP)==int(bool(c['near_mask'] or c['screen_outside']))
   assert all(t in LOWER for t,*_ in captured)
  acquisition.compare_queue(native,ref)
  rows.update(set(path)&wanted)
  for i,a in enumerate(path):
   if a not in wanted:continue
   w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
   if w>>8 in (0x89,0x8B,0x8D,0x8F):
    j=i+(2 if w>>8 in (0x8D,0x8F) else 1)
    if j<len(path):edges.add((a,path[j]))
   if w&0xF0FF==0x400B:calls.add(a)
  totals['boundary_comparisons']+=b;totals['numerical_operations']+=ops;totals['partial_cases']+=status!=0
  totals['queue_flushes']+=native.flushes;totals['queue_events']+=native.trace_count+native.trace_dropped;totals['external_service_boundaries']+=external
  if not status:
   totals['independent_cases']+=independent(c,m,before,s,native,captured)
   for target,*_ in captured:first.setdefault(target,dict(c))
  counts[c['entry']]=counts.get(c['entry'],0)+1
  records.append(dict(input=c,steps=len(path),status=status,full_state_match=True,queue_match=True))
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,topology=t,reverse=rev) for e,t,rev in itertools.product(KINDS,(0,0x4000,0x8000,0xC000),(False,True))]+[dict(entry='environment_clipped',vertex_flags=(0,1,0,0,0,0,0,0))]+[c for c in selected if c.get('environment_mesh') and c['mode']==0 and c['material_extra']==0x600000 and c['cursor_offset']==0]

 for n,c in enumerate(selected):
  check(c,n)
  if (n+1)%100==0:print(n+1,'Environment primitive comparisons passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Environment primitive smoke checks passed:',len(records));return
 for target,c in sorted(first.items()):
  check(dict(c,missing_target=target),len(records));check(dict(c,fail_target=target),len(records))
 assert rows==wanted,('missing rows',[hex(a) for a in sorted(wanted-rows)])
 want_edges={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert edges==want_edges,('missing edges',[[hex(a),hex(b)] for a,b in sorted(want_edges-edges)])
 assert calls=={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']}
 assert LOWER<=set(first),('lower services',LOWER-set(first))
 files=[Path(__file__),Path(previous.__file__),Path(lit_parent.__file__),Path(uv_parent.__file__),Path(vertex.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_environment_primitives.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_environment_primitives_connected.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),**totals,service_targets=sorted(hex(t) for t in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Complete signed-Environment primitive family versus original CPU/FPU/RAM/call boundaries and queues; independent signed coordinate scaling, lighting, retained queue words, stream consumption and clipped-helper argument staging.',limitations=['Three lower environment near/alternate helpers and ten other primitive emitters remain explicit fixtures; clipped output is not verified until those helpers are recovered.','Valid bounded RAM/queue streams only, inherited numerical guards, no hardware timing/GPU FIFO/pixels/MMU-on/fault or full-game proof.'],cases=records)
 (PROJECT/'analysis/main_dispatch_environment_primitives_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
