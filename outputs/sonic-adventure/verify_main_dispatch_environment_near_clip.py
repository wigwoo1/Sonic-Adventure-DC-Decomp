"""Environment near clipping against original instructions and independent UV math."""
import ctypes,gc,hashlib,inspect,itertools,json,math,sys
from pathlib import Path
import verify_main_dispatch_environment_primitives as previous
import verify_main_dispatch_lit_near_clip as lit_clip
import verify_main_dispatch_near_clip as flat_clip
import verify_main_dispatch_uv_near_clip as uv_clip
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
OUTPUT_UV=0x8C890268
SEED=previous.SEED;MATRIX=previous.MATRIX
SERVICES=previous.SERVICES-{ENV_NEAR};LOWER=previous.LOWER-{ENV_NEAR}
CACHE,PACKETS,RECORDS=previous.CACHE,previous.PACKETS,flat_clip.RECORDS
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_environment_near_clip.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(previous.SPECS,environment_near=(ENV_NEAR,'sa_environment_triangle_near_clip_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE,ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(previous.LIB,name).argtypes;fn.restype=getattr(previous.LIB,name).restype

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def setup():
 source=inspect.getsource(lit_clip.setup).replace('def setup():','def local_setup():',1)
 source=source.replace('("mesh","primitive","clipped","near","alternate","alternate_clipped","lit","lit_clipped","lit_near")','("mesh","primitive","clipped","near","alternate","alternate_clipped","lit","lit_clipped","lit_near","lit_alternate","lit_alternate_clipped","uv_lit","uv_lit_clipped","uv_near","uv_alternate","uv_alternate_clipped","environment","environment_clipped","environment_near")')
 guard=" changes['  assert pc in allowed,hex(pc);path.append(pc)']+=chr(10)+'  if pc==0x8c611b9a and ((r[2]+12)&0x1f000000)!=0x0c000000:stop(pc,r,sr,pr,-3)'\n"
 guard+=" changes['  assert pc in allowed,hex(pc);path.append(pc)']+=chr(10)+'  if pc==0x8c612136 and ((r[2]+12)&0x1f000000)!=0x0c000000:stop(pc,r,sr,pr,-3)'\n"
 marker=" marker=' marker='";assert source.count(marker)==1
 source=source.replace(marker,' new_guard+='+repr(guard)+'\n'+marker,1)
 source=source.replace('main_dispatch_lit_near_clip_translation.json','main_dispatch_environment_near_clip_translation.json').replace('main_dispatch_lit_primitives_verification.json','main_dispatch_environment_primitives_verification.json')
 source=source.replace("('alternate_strips','acquisition_variants','lit_primitives')","('alternate_strips','acquisition_variants','lit_primitives','lit_near_clip','lit_alternate_strips','uv_lit_primitives','uv_near_clip','uv_alternate_strips','environment_primitives')")
 ns=dict(lit_clip.__dict__);exec(compile(source,'environment_near_clip_setup','exec'),ns)
 manifest_path,manifest,baseline,allowed,execute=ns['local_setup']()
 base.TRACKED|={UV_LIT,UV_LIT_CLIP,UV_NEAR,LIT_ALT,LIT_ALT_CLIP,UV_ALT,UV_ALT_CLIP,ENV,ENV_CLIP,ENV_NEAR}
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,SERVICES=SERVICES,effect=previous.effect)
 original_reference=base.previous.reference
 def reference_with_writeback(*args,**kwargs):
  kwargs['cache_writeback']=execute.__globals__['queue_reference'].writeback
  return original_reference(*args,**kwargs)
 base.previous.reference=reference_with_writeback
 return manifest_path,manifest,baseline,allowed,execute

def prepare(c,n):
 if c['entry']=='environment_near':
  m,s,f=lit_clip.prepare(dict(c,entry='lit_near'),n)
  store(m,0x8C890238,MATRIX)
  values=c.get('environment_matrix',(1.,0.25,0.,0.,-0.5,1.,0.,0.,0.25,-0.5,1.,0.,0.,0.,0.,1.))
  for i,v in enumerate(values):store(m,MATRIX+i*4,current.bits(v))
 else:m,s,f=previous.prepare(c,n)
 for i in range(8):store(m,OUTPUT_UV+i*2,0x7000+i,2)
 return m,s,f

def generated_uvs(m,records):
 rounded=lambda v:current.value(current.bits(v))
 matrix=load(m,0x8C890238);values=[current.value(load(m,matrix+j*4)) for j in range(16)]
 result=[]
 for i in range(3):
  normal=[current.value(load(m,records+i*32+j*4)) for j in range(3)];pair=[]
  for col in (0,1):
   dot=rounded(normal[1]*values[4+col])
   dot=rounded(dot+normal[0]*values[col]);dot=rounded(dot+normal[2]*values[8+col])
   word=int(rounded(128.+128.*dot))&0xFFFF
   pair.append(word-65536 if word&0x8000 else word)
  result.append(tuple(pair))
 return result

def independent(c,m,before,s,native,initial_sp):
 connected=c.get('environment_clipped_mesh',False)
 if c['entry']!='environment_near' and not connected:return False
 if c.get('mode',0)&0x180000 or c.get('rm',0)!=0:return False
 near=c.get('plane',1.);mask=c.get('near_mask',1)
 points=[((-x if c.get('reverse') else x),y,near*(0.5 if mask&(1<<i) else 4.)) for i,(x,y) in enumerate(c.get('camera_points',((0.,0.),(4.,0.),(0.,4.))))]
 working=m if connected else before;records=CACHE if connected else RECORDS
 factors=[lit_clip.factor(working,records+i*32) for i in range(3)]
 for i,value in enumerate(factors):assert load(m,records+i*32+28)==current.bits(value),(c,'cached factor',i)
 pairs=generated_uvs(working,records)
 if not connected:
  # Six saved GPRs and 0x54 local bytes; generated UVs start at local+0x18.
  raw_uv=initial_sp-6*4-0x54+0x18
  assert [load(m,raw_uv+i*2,2) for i in range(6)]==[v&0xFFFF for pair in pairs for v in pair]
 if mask==7:return False # Original-comparison only, outside normal caller contract.
 clipped,rejected=uv_clip.polygon(c,points,factors,pairs);emitted=len(clipped)
 assert emitted in (0,3,4)
 for i,point in enumerate(clipped):
  assert [load(m,OUTPUT_UV+i*4+j*2,2) for j in range(2)]==[v&0xFFFF for v in point[4:]],(c,'quantized UV scratch',i,point)
 if emitted==4:clipped=[clipped[i] for i in (0,1,3,2)]
 assert native.flushes==emitted+(4 if connected else 0)
 assert load(m,0x8C88F65C)==(0xFFFFFFFE+emitted)&0xFFFFFFFF
 assert load(m,0x8C8902FC)==(c.get('cull',1) if rejected else 0)
 cursor=c.get('cursor',PACKETS)
 if connected:cursor=(0x8CDE0000 if c['material_extra']&0x100000 else PACKETS)+c.get('cursor_offset',0)+32
 scale=c.get('scale',(1.,1.));offset=c.get('offset',(0.,0.));retained=[SEED[7],SEED[15]]
 if connected:retained[((cursor-32)>>5)&1]=load(m,mesh.STATE+28)
 for i,(x,y,z,factor,u,v) in enumerate(clipped):
  a=cursor+i*32
  assert load(m,a)==(0xF0000000 if i==emitted-1 else 0xE0000000)
  got=[current.value(load(m,a+4+j*4)) for j in range(3)];want=(x*scale[0]/z+offset[0],y*scale[1]/z+offset[1],1/z)
  assert all(math.isclose(v,w,rel_tol=1e-5,abs_tol=1e-4) for v,w in zip(got,want)),(c,'clipped projection',i,got,want)
  assert [load(m,a+16+j*4) for j in range(2)]==[current.bits(value/256.) for value in (u,v)],(c,'packet UV',i,u,v)
  assert math.isclose(current.value(load(m,a+24)),factor,rel_tol=1e-5,abs_tol=1e-5),(c,'factor interpolation',i)
  assert load(m,a+28)==retained[(a>>5)&1]
 assert s.r[8]==((cursor&0x03FFFFFF)|0xE0000000)+32*emitted
 if connected:
  assert load(m,services.HEADER+(20 if c['material_extra']&0x100000 else 16))==cursor+32*emitted
  assert load(m,0x8C88F658)==3
 return True


def cases():
 for c in lit_clip.cases():
  if c['entry']=='lit_near':yield dict(c,entry='environment_near')
  elif c.get('graph')=='retail':yield c
 for c in previous.cases():
  if c['entry']=='environment_clipped' and 'vertex_flags' in c:yield c
 for entry,bank in itertools.product(('environment','environment_clipped'),range(2)):
  yield dict(entry=entry,topology=0xC000,mode=bank<<21)
 for mask,reverse,bank,extra,offset in itertools.product((1,2,3,4,5,6),(False,True),range(2),(0x600000,0x700000),(0,32)):
  yield dict(entry='mesh',connected_triangle=True,environment_mesh=True,environment_clipped_mesh=True,near_mask=mask,screen_outside=False,reverse=reverse,mode=bank<<21,normals=True,clip=False,acquisition_mesh=True,material_extra=extra,cursor_offset=offset)
 matrices=((1/512.,0.25,0.,0.,-0.5,1/1024.,0.,0.,0.25,-0.5,1.,0.,0.,0.,0.,1.),(512.,-512.,0.,0.,-512.,512.,0.,0.,512.,512.,1.,0.,0.,0.,0.,1.),(0.,)*16)
 for matrix,mask,reverse,bank in itertools.product(matrices,(1,3,6),(False,True),range(2)):
  yield dict(entry='environment_near',near_mask=mask,environment_matrix=matrix,reverse=reverse,mode=bank<<21)


def main():
 manifest_path,manifest,baseline,allowed,execute=setup();want={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 rows=set();edges=set();records=[];counts={};parent_calls=set()
 totals=dict(boundary_comparisons=0,numerical_operations=0,partial_cases=0,independent_cases=0,independent_mesh_cases=0,queue_flushes=0,queue_events=0,external_service_boundaries=0,environment_near_invocations=0)
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry='environment_near',near_mask=mask,reverse=rev) for mask,rev in itertools.product((1,3,6),(False,True))]+[dict(entry='environment_near',near_mask=1,mode=0x100000,reverse=True),dict(entry='environment_near',mode=0x80000)]+[c for c in selected if c.get('environment_clipped_mesh') and c['mode']==0 and c['material_extra']==0x600000 and c['cursor_offset']==0]

 for n,c in enumerate(selected):
  m,s,f=prepare(c,n);initial_sp=s.r[15];before=bytes(m);ref=QueueReference();native=Memory();bus=Bus()
  LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus))
  for i in range(16):native.queue[i]=ref.queue[i]=SEED[i]
  if c['entry'] in ('environment','environment_clipped','environment_near'):native.qacr[:]=ref.qacr[:]=[12,12]
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  path,status,b,ops,_,external=execute(c,m,s,f,c['entry'],0xFEED1234,allowed)
  if c['entry'] in ('environment','environment_clipped'):assert status==0,(c,status)
  if c.get('environment_clipped_mesh'):assert status==0 and external==0 and path.count(ENV_NEAR)==path.count(ENV_CLIP)==1,(c,status,external,path.count(ENV_NEAR),path.count(ENV_CLIP))
  previous.acquisition.compare_queue(native,ref);rows.update(set(path)&want)
  parent_calls.update(set(path)&{0x8C61A73A,0x8C61AA9C,0x8C61AAB6,0x8C61AD4C})
  for i,a in enumerate(path):
   if a not in want:continue
   w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
   if w>>8 in (0x89,0x8B,0x8D,0x8F):
    j=i+(2 if w>>8 in (0x8D,0x8F) else 1)
    if j<len(path):edges.add((a,path[j]))
  totals['boundary_comparisons']+=b;totals['numerical_operations']+=ops;totals['partial_cases']+=status!=0
  totals['queue_flushes']+=native.flushes;totals['queue_events']+=native.trace_count+native.trace_dropped
  totals['external_service_boundaries']+=external;totals['environment_near_invocations']+=path.count(ENV_NEAR)
  if not status:
   checked=independent(c,m,before,s,native,initial_sp);totals['independent_cases']+=checked;totals['independent_mesh_cases']+=checked and c.get('environment_clipped_mesh',False)
  counts[c['entry']]=counts.get(c['entry'],0)+1
  records.append(dict(input=c,steps=len(path),status=status,full_state_match=True,queue_match=True))
  if (n+1)%100==0:print(n+1,'Environment near-clipping cases passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Environment near-clipping smoke checks passed:',len(records));return
 assert rows==want,('missing rows',[hex(a) for a in sorted(want-rows)])
 assert edges=={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert len(parent_calls)==4
 files=[Path(__file__),Path(previous.__file__),Path(lit_clip.__file__),Path(flat_clip.__file__),Path(uv_clip.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_environment_near_clip.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_environment_near_clip_connected.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=0,parent_near_call_sites_covered=sorted(hex(a) for a in parent_calls),**totals,service_targets=sorted(hex(t) for t in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Complete native environment-map near helper versus original full state/RAM/boundaries/queues; independent normal-derived UV generation, first integer quantization/wrap, clipped re-quantization, camera geometry, lighting, retained words, classified meshes and all four parent calls.',limitations=['Valid bounded coherent-RAM/MMU-off queues and inherited numerical guards/oracle limits; saved-pointer guard stops unsupported transfers, not physical hardware exceptions. No GPU FIFO/pixels/MMU-on/cache-timing/full-game proof.','All-near standalone inputs have original-instruction comparison evidence only, outside normal caller contract. Two alternate environment helpers and ten other primitive emitters remain fixtures.'],cases=records)
 (PROJECT/'analysis/main_dispatch_environment_near_clip_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','parent_near_call_sites_covered',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
