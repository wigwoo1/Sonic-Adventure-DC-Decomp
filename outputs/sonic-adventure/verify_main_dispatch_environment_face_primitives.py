"""Environment UVs with normalized face factors: original state and independent packets."""
import ctypes,gc,hashlib,inspect,itertools,json,math,sys
from pathlib import Path
import verify_main_dispatch_uv_constant_alternate_strips as previous
import verify_main_dispatch_uv_constant_primitives as const_parent
import verify_main_dispatch_environment_primitives as env_parent
import verify_main_dispatch_face_lit_primitives as face_parent
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
ENV,ENV_CLIP,ENV_NEAR=previous.ENV,previous.ENV_CLIP,previous.ENV_NEAR
ENV_ALT,ENV_ALT_CLIP=previous.ENV_ALT,previous.ENV_ALT_CLIP
FACE,FACE_CLIP,NORMALIZE=previous.FACE,previous.FACE_CLIP,previous.NORMALIZE
FACE_NEAR,FACE_ALT,FACE_ALT_CLIP=previous.FACE_NEAR,previous.FACE_ALT,previous.FACE_ALT_CLIP
UV_FACE,UV_FACE_CLIP,UV_FACE_NEAR=previous.UV_FACE,previous.UV_FACE_CLIP,previous.UV_FACE_NEAR
UV_FACE_ALT,UV_FACE_ALT_CLIP=previous.UV_FACE_ALT,previous.UV_FACE_ALT_CLIP
UV_CONSTANT,UV_CONSTANT_CLIP,UV_CONSTANT_NEAR=previous.UV_CONSTANT,previous.UV_CONSTANT_CLIP,previous.UV_CONSTANT_NEAR
UV_CONSTANT_ALT,UV_CONSTANT_ALT_CLIP=previous.UV_CONSTANT_ALT,previous.UV_CONSTANT_ALT_CLIP
ENV_FACE,ENV_FACE_CLIP=0x8C61DB98,0x8C61E4F4
ENV_FACE_NEAR,ENV_FACE_ALT,ENV_FACE_ALT_CLIP=0x8C6173BA,0x8C614580,0x8C616474
KINDS=("environment_face","environment_face_clipped")
LOWER={ENV_FACE_NEAR,ENV_FACE_ALT,ENV_FACE_ALT_CLIP}
SERVICES=(previous.SERVICES-{ENV_FACE,ENV_FACE_CLIP})|LOWER
SEGMENT,INDICES,CACHE,PACKETS=lit_parent.SEGMENT,lit_parent.INDICES,previous.CACHE,previous.PACKETS
SEED=previous.SEED;MATRIX=env_parent.MATRIX
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_environment_face_primitives.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(previous.SPECS,environment_face=(ENV_FACE,'sa_environment_face_primitive_emit_connected'),environment_face_clipped=(ENV_FACE_CLIP,'sa_environment_face_primitive_clipped_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE,ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(previous.LIB,name).argtypes;fn.restype=getattr(previous.LIB,name).restype

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def effect(c,target,mem,r,sr,fp):
 if target not in LOWER:return mesh.effect(c,target,mem,r,sr,fp)
 if c.get('capture') is not None:
  uv=()
  c['capture'].append((target,r[4],r[5],r[6],r[7],uv,fp.banks[(fp.fpscr>>21)&1][4]))
 for i in range(8):r[i]=0xBAD00000+i*0x101
 bank=(fp.fpscr>>21)&1
 for i in range(6):fp.banks[bank][i]=current.bits(0.125*(i+1))
 r[8]=(r[8]+c.get('fixture_advance',0))&0xFFFFFFFF
 return sr^1

def setup():
 source=inspect.getsource(const_parent.setup).replace('def setup():','def local_setup():',1)
 marker='"uv_constant_clipped")';assert source.count(marker)==1
 source=source.replace(marker,'"uv_constant_clipped","uv_constant_near","uv_constant_alternate","uv_constant_alternate_clipped","environment_face","environment_face_clipped")')
 guard=" changes['  assert pc in allowed,hex(pc);path.append(pc)']+=chr(10)+'  if pc==0x8c612614 and ((r[2]+12)&0x1f000000)!=0x0c000000:stop(pc,r,sr,pr,-3)'\n"
 guard+=" changes['  assert pc in allowed,hex(pc);path.append(pc)']+=chr(10)+'  if pc==0x8c61ef72 and (r[14]&0x1f000000)!=0x0c000000:stop(pc,r,sr,pr,-3)'\n"
 guard+=" changes[' def trace(pc,w,r,sr,pr,_f):']=changes[' def trace(pc,w,r,sr,pr,_f):'].replace('filter_returns=[]','filter_returns=[];env_face_returns=[]')\n"
 guard+=" changes['  assert pc in allowed,hex(pc);path.append(pc)']+=chr(10)+'  if pc in (0x8c61db98,0x8c61e4f4):env_face_returns.append(pr)'+chr(10)+'  if pc in (0x8c61e4b8,0x8c61f0ce):'+chr(10)+'   if pr!=env_face_returns[-1]:stop(pc,r,sr,pr,-3)'+chr(10)+'   env_face_returns.pop()'\n"
 lines=source.splitlines(True);position=next(i for i,line in enumerate(lines) if line.lstrip().startswith('marker=') and '# The environment setup' in line)
 lines.insert(position,' guard+='+repr(guard)+'\n');source=''.join(lines)
 ns=dict(const_parent.__dict__);exec(compile(source,'environment_face_setup','exec'),ns)
 *_,allowed,execute=ns['local_setup']()
 manifest_path=PROJECT/'analysis/main_dispatch_environment_face_primitives_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_uv_constant_alternate_strips_verification.json'
 inherited=[json.loads((PROJECT/f'analysis/main_dispatch_{name}_translation.json').read_text()) for name in ('uv_constant_near_clip','uv_constant_alternate_strips')]
 for item in [manifest,*inherited,json.loads(baseline.read_text())]:
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
  if 'routines' in item:allowed|={int(a,16) for r in item['routines'] for a in r['reachable_addresses']}
 base.TRACKED|={UV_CONSTANT_NEAR,UV_CONSTANT_ALT,UV_CONSTANT_ALT_CLIP,ENV_FACE,ENV_FACE_CLIP}
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,SERVICES=SERVICES,effect=effect)
 return manifest_path,manifest,baseline,allowed,execute


def prepare(c,n):
 if c['entry'] in KINDS:
  m,s,f=face_parent.prepare(dict(c,entry='face_lit' if c['entry']=='environment_face' else 'face_lit_clipped'),n)
 else:
  m,s,f=face_parent.prepare(dict(c,face_mesh=c.get('environment_face_mesh',False)),n)
  if c.get('environment_face_mesh') and c['screen_outside']:store(m,0x8CD50000,current.bits(100. if c['reverse'] else -100.))
 store(m,0x8C890238,MATRIX)
 values=c.get('environment_matrix',(1.,0.25,0.,0.,-0.5,1.,0.,0.,0.25,-0.5,1.,0.,0.,0.,0.,1.))
 for i,v in enumerate(values):
  store(m,MATRIX+i*4,current.bits(v))
  if c.get('environment_face_mesh'):store(m,0x8CE10000+i*4,current.bits(v))
 return m,s,f

def cases():
 for c in face_parent.cases():
  if c['entry']=='normalize' or c.get('face_mesh'):continue
  yield dict(c,entry={'face_lit':'environment_face','face_lit_clipped':'environment_face_clipped'}.get(c['entry'],c['entry']))
 for entry,matrix,reverse,bank in itertools.product(KINDS,((1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.),(-2.,0.5,0.,0.,1.,-2.,0.,0.,-0.5,1.,1.,0.,0.,0.,0.,1.)),(False,True),range(2)):
  yield dict(entry=entry,environment_matrix=matrix,reverse=reverse,mode=bank<<21)
 for (mask,outside),reverse,bank,secondary,offset in itertools.product(((0,False),(0,True),(1,False)),(False,True),range(2),range(2),(0,32)):
  yield dict(entry='mesh',environment_face_mesh=True,connected_triangle=True,near_mask=mask,screen_outside=outside,reverse=reverse,mode=bank<<21,normals=True,clip=False,acquisition_mesh=True,material_extra=0x1600000|(secondary<<20),cursor_offset=offset)


def face_factor(m,ids):
 rounded=lambda x:current.value(current.bits(x))
 normal=[rounded(rounded(current.value(load(m,CACHE+ids[0]*32+j*4))+current.value(load(m,CACHE+ids[1]*32+j*4)))+current.value(load(m,CACHE+ids[2]*32+j*4))) for j in range(3)]
 length=math.sqrt(sum(v*v for v in normal))
 if not length:return None
 return current.value(load(m,0x8C890258))-sum(normal[j]/length*current.value(load(m,0x8C890248+j*4)) for j in range(3))

def signed16(v):return v-65536 if v&0x8000 else v

def independent(c,m,before,s,native,captured):
 connected=c.get('environment_face_mesh',False)
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
  clipped=c['entry']=='environment_face_clipped' or connected and (c['near_mask'] or c['screen_outside'])
  rejected=clipped and bool(flags[0]&flags[1]&flags[2]&63)
  near=clipped and not rejected and bool((flags[0]|flags[1]|flags[2])&1)
  value=face_factor(cache,ids)
  if value is None:return False
  uv=()
  if near:near_calls.append((tuple(CACHE+i*32 for i in ids),uv,value))
  elif not rejected and area<=0:
   for local,index in enumerate(ids):records.append((index,local,env_parent.environment_uv(cache,index),value))
 assert len(captured)==2*len(near_calls),(c,captured,near_calls)
 for event,(ids,uv,value) in zip(captured,near_calls*2):
  assert event[0]==ENV_FACE_NEAR and event[1:4]==ids and event[-2]==uv
  assert math.isclose(current.value(event[-1]),value,rel_tol=2e-5,abs_tol=2e-5),(c,'staged face factor')
 emitted=len(records)
 if not connected:
  assert load(m,0x8C88F658)==(0xFFFFFFFF+3*count)&0xFFFFFFFF
  assert load(m,0x8C88F65C)==(0xFFFFFFFE+emitted)&0xFFFFFFFF
  assert m[CACHE&0xFFFFFF:(CACHE&0xFFFFFF)+256]==before[CACHE&0xFFFFFF:(CACHE&0xFFFFFF)+256],(c,'unchanged input cache')
 assert native.flushes==emitted+(4 if connected else 0)
 assert s.r[8]==((start&0x03FFFFFF)|0xE0000000)+32*emitted
 retained=[SEED[6],SEED[14]];padding=[SEED[7],SEED[15]]
 if connected:
  retained=[load(m,CACHE+2*32+24),load(m,CACHE+32+24)]
  retained[((start-32)>>5)&1]=load(m,mesh.STATE+24);padding[((start-32)>>5)&1]=load(m,mesh.STATE+28)
 for i,(index,local,uv,value) in enumerate(records):
  destination=start+i*32;q=(destination>>5)&1
  expected=[0xF0000000 if local==2 else 0xE0000000]+[load(cache,CACHE+index*32+16+j*4) for j in range(3)]+list(uv)
  assert [load(m,destination+j*4) for j in range(6)]==expected,(c,'Environment-face packet',i)
  actual=load(m,destination+24)
  if local==2:
   assert math.isclose(current.value(actual),value,rel_tol=2e-5,abs_tol=2e-5),(c,'provoking factor');retained[q]=actual
  else:assert actual==retained[q],(c,'retained earlier factor',i)
  assert load(m,destination+28)==padding[q]
 if connected:assert load(m,services.HEADER+(20 if c['material_extra']&0x100000 else 16))==start+32*emitted
 return True

def main():
 manifest_path,manifest,baseline,allowed,execute=setup()
 wanted={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']};rows=set();edges=set();calls=set();records=[];counts={};first={}
 totals=dict(normalizer_invocations=0,boundary_comparisons=0,numerical_operations=0,partial_cases=0,independent_cases=0,queue_flushes=0,queue_events=0,external_service_boundaries=0)
 def check(c,n):
  m,s,f=prepare(c,n);before=bytes(m);ref=QueueReference();native=Memory();bus=Bus()
  LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus))
  for i in range(16):native.queue[i]=ref.queue[i]=SEED[i]
  if c['entry'] in KINDS or c['entry']=='normalize':native.qacr[:]=ref.qacr[:]=[12,12]
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  captured=[];case=dict(c,capture=captured)
  path,status,b,ops,_,external=execute(case,m,s,f,c['entry'],0xFEED1234,allowed)
  if c.get('environment_face_mesh'):
   assert status==0 and path.count(ENV_FACE)+path.count(ENV_FACE_CLIP)==1
   assert path.count(ENV_FACE_CLIP)==int(bool(c['near_mask'] or c['screen_outside']))
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
  totals['normalizer_invocations']+=path.count(NORMALIZE)
  totals['boundary_comparisons']+=b;totals['numerical_operations']+=ops;totals['partial_cases']+=status!=0
  totals['queue_flushes']+=native.flushes;totals['queue_events']+=native.trace_count+native.trace_dropped;totals['external_service_boundaries']+=external
  if not status:
   totals['independent_cases']+=independent(c,m,before,s,native,captured)
   for target,*_ in captured:first.setdefault(target,dict(c))
  counts[c['entry']]=counts.get(c['entry'],0)+1
  records.append(dict(input=c,steps=len(path),status=status,full_state_match=True,queue_match=True))
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,topology=t,reverse=rev) for e,t,rev in itertools.product(KINDS,(0,0x4000,0x8000,0xC000),(False,True))]+[dict(entry='environment_face_clipped',vertex_flags=(0,1,0,0,0,0,0,0))]+[c for c in selected if c.get('environment_face_mesh') and c['mode']==0 and c['material_extra']==0x1600000 and c['cursor_offset']==0]

 for n,c in enumerate(selected):
  check(c,n)
  if (n+1)%100==0:print(n+1,'Environment-face primitive comparisons passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Environment-face primitive smoke checks passed:',len(records));return
 for target,c in sorted(first.items()):
  check(dict(c,missing_target=target),len(records));check(dict(c,fail_target=target),len(records))
 assert rows==wanted,('missing rows',[hex(a) for a in sorted(wanted-rows)])
 want_edges={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert edges==want_edges,('missing edges',[[hex(a),hex(b)] for a,b in sorted(want_edges-edges)])
 assert calls=={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']}
 assert LOWER<=set(first),('lower services',LOWER-set(first))
 files=[Path(__file__),Path(previous.__file__),Path(lit_parent.__file__),Path(face_parent.__file__),Path(const_parent.__file__),Path(env_parent.__file__),Path(vertex.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_environment_face_primitives.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_environment_face_primitives_connected.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),**totals,service_targets=sorted(hex(t) for t in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Complete environment-UV/face-factor primitive pair with native normalization versus original full state/RAM/queues; independent normal-derived environment UVs, normalized face factors on provoking vertices, retained earlier factors/padding, preserved input caches, counters/cursors and near pointer/FR4 staging.',limitations=['Three lower environment-face clipping/alternate helpers and two other primitive emitters remain fixtures. Native normalization uses the pinned vector backend; independent factors have tolerance, not arbitrary hardware precision proof.','Bounded constructed streams, controlled strip-register preconditions and pointer/return guards; coherent-RAM/MMU-off queues; no hardware timing/GPU FIFO/pixels/MMU-on/fault or full-game proof.'],cases=records)
 (PROJECT/'analysis/main_dispatch_environment_face_primitives_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
