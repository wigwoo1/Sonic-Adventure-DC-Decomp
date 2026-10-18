"""Normal-derived environment UV family versus original code and packet math."""
import ctypes,gc,hashlib,inspect,itertools,json,math,sys
from pathlib import Path
import verify_main_dispatch_face_lit_primitives as previous
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
SERVICES=previous.SERVICES-{FACE_NEAR};LOWER=previous.LOWER-{FACE_NEAR}
CACHE,PACKETS=previous.CACHE,previous.PACKETS
import verify_main_dispatch_near_clip as flat_clip
RECORDS,COLOUR=flat_clip.RECORDS,0x8CDF2000
SEED=previous.SEED
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_face_near_clip.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(previous.SPECS,face_near=(FACE_NEAR,'sa_face_triangle_near_clip_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE,ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(previous.LIB,name).argtypes;fn.restype=getattr(previous.LIB,name).restype

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def setup():
 source=inspect.getsource(previous.setup).replace('def setup():','def local_setup():',1)
 marker='"face_lit_clipped")';assert source.count(marker)==1
 source=source.replace(marker,'"face_lit_clipped","face_near")')
 guard=" changes['  assert pc in allowed,hex(pc);path.append(pc)']+=chr(10)+'  if pc==0x8c617a0e and ((r[2]+12)&0x1f000000)!=0x0c000000:stop(pc,r,sr,pr,-3)'\n"
 marker=' # The environment setup passes guard text into its cloned lighting setup.'
 assert source.count(marker)==1;source=source.replace(marker,' guard+='+repr(guard)+'\n'+marker,1)
 ns=dict(previous.__dict__);exec(compile(source,'face_near_setup','exec'),ns)
 *_,allowed,execute=ns['local_setup']()
 manifest_path=PROJECT/'analysis/main_dispatch_face_near_clip_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_face_lit_primitives_verification.json'
 for item in (manifest,json.loads(baseline.read_text())):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 allowed|={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']};base.TRACKED.add(FACE_NEAR)
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,SERVICES=SERVICES)
 return manifest_path,manifest,baseline,allowed,execute

def prepare(c,n):
 if c['entry']=='face_near':
  m,s,f=flat_clip.prepare(dict(c,entry='near'),n)
  words=c.get('colour_words',tuple(current.bits(v) for v in (0.125,0.25,0.5,1.)))
  for i,v in enumerate(words):store(m,COLOUR+i*4,v)
  s.r[7]=COLOUR
 else:m,s,f=previous.prepare(c,n)
 return m,s,f

def polygon(c):
 near=c.get('plane',1.);mask=c.get('near_mask',1)
 points=[((-x if c.get('reverse') else x),y,near*(0.5 if mask&(1<<i) else 4.)) for i,(x,y) in enumerate(c.get('camera_points',((0.,0.),(4.,0.),(0.,4.))))]
 a,b,d=points
 determinant=a[0]*(b[1]*d[2]-b[2]*d[1])-a[1]*(b[0]*d[2]-b[2]*d[0])+a[2]*(b[0]*d[1]-b[1]*d[0])
 rejected=bool(c.get('cull',1)&int(determinant>0));result=[]
 if not rejected:
  last=points[-1]
  for point in points:
   if (last[2]<near)!=(point[2]<near):
    t=(near-last[2])/(point[2]-last[2]);result.append(tuple(last[j]+t*(point[j]-last[j]) for j in range(3)))
   if point[2]>=near:result.append(point)
   last=point
 return result,rejected

def independent(c,m,before,s,native):
 connected=c.get('face_clipped_mesh',False)
 if c['entry']!='face_near' and not connected:return False
 if c.get('mode',0)&0x180000 or c.get('near_mask',1)==7:return False
 points,rejected=polygon(c);emitted=len(points)
 assert emitted in (0,3,4) and native.flushes==emitted+(4 if connected else 0)
 assert load(m,0x8C88F65C)==(0xFFFFFFFE+emitted)&0xFFFFFFFF
 assert load(m,0x8C8902FC)==(c.get('cull',1) if rejected else 0)
 if emitted==4:points=[points[i] for i in (0,1,3,2)]
 cursor=c.get('cursor',PACKETS)
 if connected:cursor=(0x8CDE0000 if c['material_extra']&0x100000 else PACKETS)+c.get('cursor_offset',0)+32
 scale=c.get('scale',(1.,1.));offset=c.get('offset',(0.,0.))
 colour=previous.face_colour(m,(0,1,2)) if connected else [load(before,COLOUR+j*4) for j in range(4)]
 for i,(x,y,z) in enumerate(points):
  a=cursor+i*32
  assert load(m,a)==(0xF0000000 if i==emitted-1 else 0xE0000000)
  got=[current.value(load(m,a+4+j*4)) for j in range(3)];want=(x*scale[0]/z+offset[0],y*scale[1]/z+offset[1],1/z)
  assert all(math.isclose(a,b,rel_tol=1e-5,abs_tol=1e-4) for a,b in zip(got,want)),(c,'clipped face geometry',i,got,want)
  words=[load(m,a+16+j*4) for j in range(4)]
  if i>=2:
   if connected:previous.colour_matches(words,colour,c)
   else:assert words==colour,(c,'copied provoking colour',i,words,colour)
  elif not connected:assert words==SEED[((a>>5)&1)*8+4:((a>>5)&1)*8+8],(c,'retained early colour',i)
 assert s.r[8]==((cursor&0x03FFFFFF)|0xE0000000)+32*emitted
 if connected:
  assert load(m,services.HEADER+(20 if c['material_extra']&0x100000 else 16))==cursor+32*emitted
  assert load(m,0x8C88F658)==3
 else:
  assert [load(m,COLOUR+j*4) for j in range(4)]==colour
  assert m[RECORDS&0xFFFFFF:(RECORDS&0xFFFFFF)+96]==before[RECORDS&0xFFFFFF:(RECORDS&0xFFFFFF)+96]
 return True

def cases():
 for c in flat_clip.cases():
  if c['entry']=='near':yield dict(c,entry='face_near')
  elif c.get('graph')=='retail':yield c
 for c in previous.cases():
  if c['entry']=='face_lit_clipped' and 'vertex_flags' in c:yield c
 for entry,bank in itertools.product(('face_lit','face_lit_clipped'),range(2)):
  yield dict(entry=entry,topology=0xC000,mode=bank<<21)
 for mask,reverse,bank,extra,offset in itertools.product((1,2,3,4,5,6),(False,True),range(2),(0x1000000,0x1100000),(0,32)):
  yield dict(entry='mesh',connected_triangle=True,face_mesh=True,face_clipped_mesh=True,near_mask=mask,reverse=reverse,mode=bank<<21,normals=True,clip=False,acquisition_mesh=True,material_extra=extra,cursor_offset=offset)
 for colour,mask,reverse,bank in itertools.product(((0,0,0,0),(0x80000000,0x7FC12345,0xFFFFFFFF,0x3F800000),(0x3F000000,0xBF800000,0x40000000,0x3F800000)),(1,3,6),(False,True),range(2)):
  yield dict(entry='face_near',colour_words=colour,near_mask=mask,reverse=reverse,mode=bank<<21)


def main():
 manifest_path,manifest,baseline,allowed,execute=setup();want={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 rows=set();edges=set();records=[];counts={};parent_calls=set()
 totals=dict(boundary_comparisons=0,numerical_operations=0,partial_cases=0,independent_cases=0,independent_mesh_cases=0,queue_flushes=0,queue_events=0,external_service_boundaries=0,face_near_invocations=0)
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry='face_near',near_mask=mask,reverse=rev) for mask,rev in itertools.product((1,3,6),(False,True))]+[dict(entry='face_near',near_mask=1,mode=0x100000,reverse=True),dict(entry='face_near',mode=0x80000)]+[c for c in selected if c.get('face_clipped_mesh') and c['mode']==0 and c['material_extra']==0x1000000 and c['cursor_offset']==0]

 for n,c in enumerate(selected):
  m,s,f=prepare(c,n);before=bytes(m);ref=QueueReference();native=Memory();bus=Bus()
  LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus))
  for i in range(16):native.queue[i]=ref.queue[i]=SEED[i]
  if c['entry'] in ('face_lit','face_lit_clipped','face_near'):native.qacr[:]=ref.qacr[:]=[12,12]
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  path,status,b,ops,_,external=execute(c,m,s,f,c['entry'],0xFEED1234,allowed)
  if c.get('face_clipped_mesh'):assert status==0 and external==0 and path.count(FACE_NEAR)==path.count(FACE_CLIP)==1,(c,status,external,path.count(FACE_NEAR),path.count(FACE_CLIP))
  previous.acquisition.compare_queue(native,ref);rows.update(set(path)&want)
  parent_calls.update(set(path)&{0x8C61B702,0x8C61B9CA,0x8C61B9E8,0x8C61BD72})
  for i,a in enumerate(path):
   if a not in want:continue
   w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
   if w>>8 in (0x89,0x8B,0x8D,0x8F):
    j=i+(2 if w>>8 in (0x8D,0x8F) else 1)
    if j<len(path):edges.add((a,path[j]))
  totals['boundary_comparisons']+=b;totals['numerical_operations']+=ops;totals['partial_cases']+=status!=0
  totals['queue_flushes']+=native.flushes;totals['queue_events']+=native.trace_count+native.trace_dropped
  totals['external_service_boundaries']+=external;totals['face_near_invocations']+=path.count(FACE_NEAR)
  if not status:
   checked=independent(c,m,before,s,native);totals['independent_cases']+=checked;totals['independent_mesh_cases']+=checked and c.get('face_clipped_mesh',False)
  counts[c['entry']]=counts.get(c['entry'],0)+1
  records.append(dict(input=c,steps=len(path),status=status,full_state_match=True,queue_match=True))
  if (n+1)%100==0:print(n+1,'Face near-clipping cases passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Face near-clipping smoke checks passed:',len(records));return
 assert rows==want,('missing rows',[hex(a) for a in sorted(want-rows)])
 assert edges=={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert len(parent_calls)==4
 files=[Path(__file__),Path(previous.__file__),Path(flat_clip.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_face_near_clip.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_face_near_clip_connected.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=0,parent_near_call_sites_covered=sorted(hex(a) for a in parent_calls),**totals,service_targets=sorted(hex(t) for t in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Complete native face-colour near clipper versus original full state/RAM/boundaries/queues; independent clipped geometry, exact supplied colour copies, retained early colour words, normalized face lighting in classified meshes and all four parent calls including controlled live-register strip fixtures.',limitations=['Valid bounded coherent-RAM/MMU-off queues and inherited numerical guards/oracle limits; saved-pointer guard stops unsupported transfers, not physical hardware exceptions. No GPU FIFO/pixels/MMU-on/cache-timing/full-game proof.','All-near standalone inputs have original-instruction comparison evidence only, outside normal caller contract. Two alternate face-lit helpers and eight other primitive emitters remain fixtures.'],cases=records)
 (PROJECT/'analysis/main_dispatch_face_near_clip_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','parent_near_call_sites_covered',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
