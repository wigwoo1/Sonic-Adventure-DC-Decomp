"""Lit near clipping versus original instructions and independent polygon colours."""
import ctypes,gc,hashlib,inspect,itertools,json,math,sys
from pathlib import Path
import verify_main_dispatch_lit_primitives as previous
import verify_main_dispatch_near_clip as flat_clip
PROJECT=Path(__file__).resolve().parent
vertex=previous.vertex;services=previous.services;mesh=previous.mesh;particle=previous.particle;current=previous.current;base=previous.base
load,store=previous.load,previous.store
Bus,Memory,QueueReference,WORKERS=previous.Bus,previous.Memory,previous.QueueReference,previous.WORKERS
PLAIN,CLIPPED,NEAR,ALTERNATE,ALT_CLIP=previous.PLAIN,previous.CLIPPED,previous.NEAR,previous.ALTERNATE,previous.ALT_CLIP
SECONDARY,EXTENDED,EXT_SECONDARY=previous.SECONDARY,previous.EXTENDED,previous.EXT_SECONDARY
LIT,LIT_CLIP,LIT_NEAR=previous.LIT,previous.LIT_CLIP,0x8C612E64
SERVICES=previous.SERVICES-{LIT_NEAR};LOWER=previous.LOWER-{LIT_NEAR}
CACHE,PACKETS,RECORDS=previous.CACHE,previous.PACKETS,flat_clip.RECORDS
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_lit_near_clip.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(previous.SPECS,lit_near=(LIT_NEAR,'sa_lit_triangle_near_clip_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE,ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(previous.LIB,name).argtypes;fn.restype=getattr(previous.LIB,name).restype

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def setup():
 # Preserve the old saved-pointer guard and extend this process with the new
 # analogous guard. No archived verifier or reference interpreter is edited.
 source=inspect.getsource(flat_clip.setup).replace('def setup():','def local_setup():',1)
 source=source.replace('("mesh","primitive","clipped","near")','("mesh","primitive","clipped","near","alternate","alternate_clipped","lit","lit_clipped","lit_near")')
 new_guard=" changes['  assert pc in allowed,hex(pc);path.append(pc)']+=chr(10)+'  if pc==0x8c612ff2 and ((r[2]+12)&0x1f000000)!=0x0c000000:stop(pc,r,sr,pr,-3)'\n"
 marker=' marker=';assert source.count(marker)==1
 source=source.replace(marker,' guard+='+repr(new_guard)+'\n'+marker,1)
 ns=dict(flat_clip.__dict__);exec(compile(source,'lit_near_clip_setup','exec'),ns)
 *_,allowed,execute=ns['local_setup']()
 manifest_path=PROJECT/'analysis/main_dispatch_lit_near_clip_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_lit_primitives_verification.json'
 manifests=[manifest]+[json.loads((PROJECT/f'analysis/main_dispatch_{name}_translation.json').read_text()) for name in ('alternate_strips','acquisition_variants','lit_primitives')]
 for item in manifests+[json.loads(baseline.read_text())]:
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
  if 'routines' in item:allowed|={int(a,16) for r in item['routines'] for a in r['reachable_addresses']}
 stub=0x8C611708;assert base.binary[stub-0x8C010000:stub+4-0x8C010000]==bytes.fromhex('0b000900')
 allowed|={stub,stub+2};base.TRACKED|={LIT,LIT_CLIP,LIT_NEAR,ALTERNATE,ALT_CLIP,stub}|previous.previous.ACQUISITIONS
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,SERVICES=SERVICES,effect=previous.effect)
 return manifest_path,manifest,baseline,allowed,execute

def prepare(c,n):
 if c['entry']=='lit_near':
  m,s,f=flat_clip.prepare(dict(c,entry='near'),n)
  normals=c.get('normal_values',((0.25,0.5,0.75),(0.5,-0.25,0.125),(-0.5,0.25,0.5)))
  for i,normal in enumerate(normals):
   for j,v in enumerate(normal):store(m,RECORDS+i*32+j*4,current.bits(v))
 else:m,s,f=previous.prepare(c,n)
 for i,v in enumerate(c.get('light',(0.25,0.5,0.125))):store(m,0x8C890248+i*4,current.bits(v))
 store(m,0x8C890258,current.bits(c.get('ambient',1.)))
 if c['entry'] in ('lit','lit_clipped'):
  # Direct parent fixtures bypass the real filter, which normally publishes
  # these divisors. Zero was harmless with the old service substitute only.
  for a in (0x8C890260,0x8C890264):store(m,a,current.bits(1.))
 return m,s,f

def factor(m,address):
 rounded=lambda x:current.value(current.bits(x))
 result=current.value(load(m,0x8C890258))
 for j in range(3):result=rounded(result-rounded(current.value(load(m,address+j*4))*current.value(load(m,0x8C890248+j*4))))
 return result

def polygon(c,points,factors):
 a,b,d=points
 determinant=a[0]*(b[1]*d[2]-b[2]*d[1])-a[1]*(b[0]*d[2]-b[2]*d[0])+a[2]*(b[0]*d[1]-b[1]*d[0])
 if c.get('cull',1)&int(determinant>0):return [],True
 near=c.get('plane',1.);result=[];vertices=[(*p,f) for p,f in zip(points,factors)];last=vertices[-1]
 for point in vertices:
  if (last[2]<near)!=(point[2]<near):
   t=(near-last[2])/(point[2]-last[2]);result.append(tuple(last[j]+t*(point[j]-last[j]) for j in range(4)))
  if point[2]>=near:result.append(point)
  last=point
 return result,False

def independent(c,m,before,s,native):
 connected=c.get('lit_clipped_mesh',False)
 if c['entry']!='lit_near' and not connected:return False
 if c.get('mode',0)&0x180000:return False
 near=c.get('plane',1.);mask=c.get('near_mask',1)
 points=[((-x if c.get('reverse') else x),y,near*(0.5 if mask&(1<<i) else 4.)) for i,(x,y) in enumerate(c.get('camera_points',((0.,0.),(4.,0.),(0.,4.))))]
 working=m if connected else before;records=CACHE if connected else RECORDS
 factors=[factor(working,records+i*32) for i in range(3)]
 # Input factors are computed before orientation rejection, even if nothing
 # is emitted. This also checks the valid all-near comparison-only fixtures.
 for i,value in enumerate(factors):assert load(m,records+i*32+28)==current.bits(value),(c,'input lighting cache',i)
 if mask==7:return False # Outside caller contract; original-code comparison only.
 clipped,rejected=polygon(c,points,factors);emitted=len(clipped)
 assert emitted in (0,3,4)
 if emitted==4:clipped=[clipped[i] for i in (0,1,3,2)]
 extra=4 if connected else 0
 assert native.flushes==emitted+extra,(c,'transfers',native.flushes,emitted)
 assert load(m,0x8C88F65C)==(0xFFFFFFFE+emitted)&0xFFFFFFFF
 assert load(m,0x8C8902FC)==(c.get('cull',1) if rejected else 0)
 cursor=c.get('cursor',PACKETS)
 if connected:cursor=(0x8CDE0000 if c['material_extra']&0x100000 else PACKETS)+32
 scale=c.get('scale',(1.,1.));offset=c.get('offset',(0.,0.))
 for i,(x,y,z,factor_) in enumerate(clipped):
  a=cursor+i*32
  assert load(m,a)==(0xF0000000 if i==emitted-1 else 0xE0000000)
  got=[current.value(load(m,a+4+j*4)) for j in range(3)];want=(x*scale[0]/z+offset[0],y*scale[1]/z+offset[1],1/z)
  assert all(math.isclose(v,w,rel_tol=1e-5,abs_tol=1e-4) for v,w in zip(got,want)),(c,'projected clipping',i,got,want)
  assert load(m,a+16)==load(working,mesh.STATE+16)
  for j in range(1,4):
   actual=current.value(load(m,a+16+j*4));expected=current.value(load(working,mesh.STATE+16+j*4))*factor_
   assert math.isclose(actual,expected,rel_tol=1e-5,abs_tol=1e-5),(c,'interpolated lighting',i,j,actual,expected)
 assert s.r[8]==((cursor&0x03FFFFFF)|0xE0000000)+32*emitted
 if connected:
  assert load(m,services.HEADER+(20 if c['material_extra']&0x100000 else 16))==cursor+32*emitted
  assert load(m,0x8C88F658)==3
 return True

def cases():
 for c in flat_clip.cases():
  if c['entry']=='near':yield dict(c,entry='lit_near')
  elif c['entry']=='clipped':yield dict(c,entry='lit_clipped')
  elif c.get('graph')=='retail':yield c
 for entry in ('lit','lit_clipped'):
  for bank in range(2):yield dict(entry=entry,topology=0xC000,mode=bank<<21)
 for mask,reverse,bank,extra in itertools.product((1,2,3,4,5,6),(False,True),range(2),(0,0x100000)):
  # The inherited field name 'clip' sets ENV bit 2, which actually bypasses
  # classification. Keep it clear and assert the real classified/clip path.
  yield dict(entry='mesh',connected_triangle=True,lit_clipped_mesh=True,near_mask=mask,reverse=reverse,mode=bank<<21,layout=0,normals=True,clip=False,acquisition_mesh=True,material_extra=extra)
 for mask,ambient,bank in itertools.product((1,3,6),(-1.,0.,2.),range(2)):
  yield dict(entry='lit_near',near_mask=mask,ambient=ambient,reverse=True,mode=bank<<21)
 # Reach all four original near-helper call sites in the parent family.
 for c in previous.cases():
  if c['entry']=='lit_clipped' and 'vertex_flags' in c:yield c

def main():
 manifest_path,manifest,baseline,allowed,execute=setup();want={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 rows=set();edges=set();records=[];counts={};parent_calls=set()
 totals=dict(boundary_comparisons=0,numerical_operations=0,partial_cases=0,independent_cases=0,independent_mesh_cases=0,queue_flushes=0,queue_events=0,external_service_boundaries=0,lit_near_invocations=0)
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry='lit_near',near_mask=mask,reverse=rev) for mask,rev in itertools.product((1,3,6),(False,True))]+[dict(entry='lit_near',near_mask=1,mode=0x100000,reverse=True),dict(entry='lit_near',mode=0x80000)]+[c for c in selected if c.get('lit_clipped_mesh') and c['mode']==0 and c['material_extra']==0]
 for n,c in enumerate(selected):
  m,s,f=prepare(c,n);before=bytes(m);ref=QueueReference();native=Memory();bus=Bus()
  LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus))
  if c['entry'] in ('lit','lit_clipped','lit_near'):native.qacr[:]=ref.qacr[:]=[12,12]
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  path,status,b,ops,_,external=execute(c,m,s,f,c['entry'],0xFEED1234,allowed)
  if c['entry'] in ('lit','lit_clipped'):assert status==0,(c,status)
  if c.get('lit_clipped_mesh'):assert status==0 and external==0 and path.count(LIT_NEAR)==path.count(LIT_CLIP)==1,(c,status,external,path.count(LIT_NEAR),path.count(LIT_CLIP))
  previous.acquisition.compare_queue(native,ref);rows.update(set(path)&want)
  parent_calls.update(set(path)&{0x8C618724,0x8C6189D4,0x8C6189EE,0x8C618C3C})
  for i,a in enumerate(path):
   if a not in want:continue
   w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
   if w>>8 in (0x89,0x8B,0x8D,0x8F):
    j=i+(2 if w>>8 in (0x8D,0x8F) else 1)
    if j<len(path):edges.add((a,path[j]))
  totals['boundary_comparisons']+=b;totals['numerical_operations']+=ops;totals['partial_cases']+=status!=0
  totals['queue_flushes']+=native.flushes;totals['queue_events']+=native.trace_count+native.trace_dropped
  totals['external_service_boundaries']+=external;totals['lit_near_invocations']+=path.count(LIT_NEAR)
  if not status:
   checked=independent(c,m,before,s,native);totals['independent_cases']+=checked;totals['independent_mesh_cases']+=checked and c.get('lit_clipped_mesh',False)
  counts[c['entry']]=counts.get(c['entry'],0)+1
  records.append(dict(input=c,steps=len(path),status=status,full_state_match=True,queue_match=True))
  if (n+1)%100==0:print(n+1,'lit near-clipping cases passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Lit near-clipping smoke checks passed:',len(records));return
 assert rows==want,('missing rows',[hex(a) for a in sorted(want-rows)])
 assert edges=={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert len(parent_calls)==4
 files=[Path(__file__),Path(previous.__file__),Path(flat_clip.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_lit_near_clip.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_lit_near_clip_connected.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=0,parent_near_call_sites_covered=sorted(hex(a) for a in parent_calls),**totals,service_targets=sorted(hex(t) for t in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Complete native lit near-plane helper versus original CPU/FPU/RAM/ordered boundaries/queues; independent camera clipping, interpolated lighting factors and output RGB/unchanged alpha, connected normal projection/material/mesh dispatch and all four native parent call sites.',limitations=['Valid bounded coherent-RAM/MMU-off queues and inherited numerical guards/oracle limits; saved-pointer guard stops unsupported transfers, not physical hardware exceptions. No GPU FIFO/pixels/MMU-on/cache-timing/full-game proof.','All-near standalone inputs have original-instruction comparison evidence only, outside normal caller contract. Two alternate lighting helpers and fourteen other primitive emitters remain fixtures.'],cases=records)
 (PROJECT/'analysis/main_dispatch_lit_near_clip_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','parent_near_call_sites_covered',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
