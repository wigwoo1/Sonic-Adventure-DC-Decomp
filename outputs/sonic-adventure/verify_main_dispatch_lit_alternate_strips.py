"""Native alternate strips against original code and independent triangle sets."""
import ctypes,gc,hashlib,inspect,itertools,json,math,sys
from pathlib import Path
import verify_main_dispatch_lit_near_clip as previous
import verify_main_dispatch_alternate_strips as flat_alternate
PROJECT=Path(__file__).resolve().parent
vertex=previous.vertex;services=previous.services;mesh=previous.mesh;particle=previous.particle;current=previous.current;base=previous.base
load,store=previous.load,previous.store
Bus,Memory,QueueReference,WORKERS=previous.Bus,previous.Memory,previous.QueueReference,previous.WORKERS
PLAIN,CLIPPED,NEAR=previous.PLAIN,previous.CLIPPED,previous.NEAR
ALTERNATE,ALT_CLIP,STUB=previous.ALTERNATE,previous.ALT_CLIP,0x8C611708
LIT,LIT_CLIP,LIT_NEAR=previous.LIT,previous.LIT_CLIP,previous.LIT_NEAR
SECONDARY,EXTENDED,EXT_SECONDARY=previous.SECONDARY,previous.EXTENDED,previous.EXT_SECONDARY
LIT_ALT,LIT_ALT_CLIP=0x8C6136C0,0x8C614BB0
KINDS=("lit_alternate","lit_alternate_clipped")
SERVICES=previous.SERVICES-{LIT_ALT,LIT_ALT_CLIP}
SEGMENT,INDICES,CACHE,PACKETS=flat_alternate.SEGMENT,flat_alternate.INDICES,previous.CACHE,previous.PACKETS
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_lit_alternate_strips.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(previous.SPECS,lit_alternate=(LIT_ALT,'sa_lit_alternate_strip_connected'),lit_alternate_clipped=(LIT_ALT_CLIP,'sa_lit_alternate_strip_clipped_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE,ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(previous.LIB,name).argtypes;fn.restype=getattr(previous.LIB,name).restype
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

class QueueReference(previous.QueueReference):
 def writeback(self,address):
  assert address&0x1F000000==0x0C000000
  self.events.append((6,address,0,0))

def setup():
 source=inspect.getsource(previous.setup).replace('def setup():','def local_setup():',1)
 source=source.replace('("mesh","primitive","clipped","near","alternate","alternate_clipped","lit","lit_clipped","lit_near")','("mesh","primitive","clipped","near","alternate","alternate_clipped","lit","lit_clipped","lit_near","lit_alternate","lit_alternate_clipped")')
 ns=dict(previous.__dict__);exec(compile(source,'lit_alternate_setup','exec'),ns)
 *_,allowed,execute=ns['local_setup']()
 manifest_path=PROJECT/'analysis/main_dispatch_lit_alternate_strips_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_lit_near_clip_verification.json'
 for item in (manifest,json.loads(baseline.read_text())):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 allowed|={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 base.TRACKED|={LIT_ALT,LIT_ALT_CLIP}
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,SERVICES=SERVICES)
 original_reference=base.previous.reference
 def reference_with_writeback(*args,**kwargs):
  kwargs['cache_writeback']=execute.__globals__['queue_reference'].writeback
  return original_reference(*args,**kwargs)
 base.previous.reference=reference_with_writeback
 return manifest_path,manifest,baseline,allowed,execute


def prepare(c,n):
 if c.get('connected_alternate'):
  m,s,f=previous.prepare(c,n)
  # Distinct vertex normals make newly clipped colour interpolation observable.
  for i,normal in enumerate(((0.25,0.5,0.75),(0.5,-0.25,0.125),(-0.5,0.25,0.5),(0.25,0.5,0.75))):
   for j,v in enumerate(normal):store(m,0x8CD60000+i*12+j*4,current.bits(v))
 else:
  old_entry={'lit_alternate':'alternate','lit_alternate_clipped':'alternate_clipped'}.get(c['entry'],c['entry'])
  m,s,f=flat_alternate.prepare(dict(c,entry=old_entry),n)
  normals=c.get('normal_values',((0.25,0.5,0.75),(0.5,-0.25,0.125),(-0.5,0.25,0.5)))
  for i in range(8):
   for j,v in enumerate(normals[i%len(normals)]):store(m,CACHE+i*32+j*4,current.bits(v))
 for i,v in enumerate(c.get('light',(0.25,0.5,0.125))):store(m,0x8C890248+i*4,current.bits(v))
 store(m,0x8C890258,current.bits(c.get('ambient',1.)))
 if c['entry'] in KINDS:
  bank=((f.fpscr>>21)&1)^1
  for i in range(3):f.banks[bank][i]=load(m,0x8C890248+i*4)
  for i in range(4):f.banks[bank][i+4]=load(m,mesh.STATE+16+i*4)
  f.banks[bank][8]=load(m,0x8C890258)
 return m,s,f


def independent(c,m,before,s,native):
 if c.get('connected_alternate'):
  # Independent camera-space clipping for a three-vertex input strip after
  # actual filter projection and material/primitive dispatch.
  mask=c['near_mask'];accepted=c['reverse']!=c['strip_flip'];polygon=[]
  points=[((-x if c['reverse'] else x),y,0.5 if mask&(1<<i) else 4.,previous.factor(m,CACHE+i*32)) for i,(x,y) in enumerate(((0.,0.),(4.,0.),(0.,4.)))]
  if accepted:
   last=points[-1]
   for point in points:
    if (last[2]<1)!=(point[2]<1):
     t=(1-last[2])/(point[2]-last[2]);polygon.append(tuple(last[j]+t*(point[j]-last[j]) for j in range(4)))
    if point[2]>=1:polygon.append(point)
    last=point
  emitted=len(polygon)
  assert native.flushes==4+emitted
  assert load(m,0x8C88F658)==2 and load(m,0x8C88F65C)==(0xFFFFFFFE+3+emitted)&0xFFFFFFFF
  if emitted==4:polygon=[polygon[i] for i in (0,1,3,2)]
  start=0x8CDE0000 if c['material_extra']&0x100000 else PACKETS
  for i,(x,y,z,factor) in enumerate(polygon):
   address=start+(i+1)*32
   assert load(m,address)==(0xF0000000 if i==emitted-1 else 0xE0000000)
   got=[current.value(load(m,address+4+j*4)) for j in range(3)];expected=(x/z,y/z,1/z)
   assert all(math.isclose(a,b,rel_tol=0.00001,abs_tol=0.0001) for a,b in zip(got,expected)),(c,'connected clipped strip',got,expected)
   assert load(m,address+16)==load(m,mesh.STATE+16)
   for j in range(1,4):
    got=current.value(load(m,address+16+j*4));expected=current.value(load(m,mesh.STATE+16+j*4))*factor
    assert math.isclose(got,expected,rel_tol=1e-5,abs_tol=1e-5),(c,'connected interpolated RGB',i,j,got,expected)
  cursor=start+(emitted+1)*32
  assert s.r[8]==(cursor&0x03FFFFFF)|0xE0000000 and load(m,services.HEADER+(20 if c['material_extra']&0x100000 else 16))==cursor
  return True
 if c['entry'] not in KINDS or c.get('mode',0)&0x180000 or any(v&1 for v in c.get('vertex_flags',())):return False
 lengths=c.get('lengths',(4,));flip=c.get('strip_flip',False)
 def point(i):
  factor=previous.factor(before,CACHE+i*32)
  colour=(load(before,mesh.STATE+16),*[current.bits(current.value(load(before,mesh.STATE+16+j*4))*factor) for j in range(1,4)])
  return tuple(load(before,CACHE+i*32+16+j*4) for j in range(3))+colour
 xy=lambda i:tuple(current.value(v) for v in point(i)[:2])
 expected=[]
 for length in lengths:
  for i in range(length-2):
   a,b,d=(xy(j) for j in (i,i+1,i+2));area=(b[0]-a[0])*(d[1]-a[1])-(b[1]-a[1])*(d[0]-a[0])
   flags=[load(before,CACHE+j*32+12) for j in (i,i+1,i+2)]
   outside=c['entry']=='lit_alternate_clipped' and bool(flags[0]&flags[1]&flags[2]&63)
   if not outside and area*(-1 if flip^(i%2!=0) else 1)<0:expected.append(tuple(sorted(point(j) for j in (i,i+1,i+2))))
 actual=[];group=[];groups=0
 for i in range(native.flushes):
  a=c.get('cursor',PACKETS)+i*32;control=load(m,a)
  assert control in (0xE0000000,0xF0000000)
  group.append(tuple(load(m,a+4+j*4) for j in range(7)))
  if control==0xF0000000:
   groups+=1
   assert len(group)>=3
   actual.extend(tuple(sorted(group[j:j+3])) for j in range(len(group)-2));group=[]
 assert not group and actual==expected,(c,'independent visible strip triangles',actual,expected)
 assert s.r[8]==((c.get('cursor',PACKETS)&0x03FFFFFF)|0xE0000000)+32*native.flushes
 assert load(m,0x8C88F658)==(0xFFFFFFFF+sum(lengths))&0xFFFFFFFF
 # The original counters differ between variants: the plain path adds one
 # extra unit per closed output strip; the clipped path adds input lengths.
 delta=native.flushes+groups if c['entry']=='lit_alternate' else sum(lengths)
 assert load(m,0x8C88F65C)==(0xFFFFFFFE+delta)&0xFFFFFFFF
 writebacks=[e.address for e in native.trace[:native.trace_count] if e.kind==6]
 assert len(writebacks)==native.flushes
 for address in writebacks:
  assert (address-CACHE)%32==28 and 0<=address-CACHE<8*32
  assert load(m,address)==current.bits(previous.factor(before,address-28)),(c,'cached dot-product lighting',hex(address))
 return True

def cases():
 for c in flat_alternate.cases():
  entry={'alternate':'lit_alternate','alternate_clipped':'lit_alternate_clipped','primitive':'lit','clipped':'lit_clipped'}.get(c['entry'],c['entry'])
  if c.get('connected_alternate'):
   for extra in (0,0x100000):yield dict(c,entry=entry,layout=0,normals=True,clip=False,acquisition_mesh=True,material_extra=extra)
  else:yield dict(c,entry=entry)
 for entry,ambient,reverse,bank in itertools.product(KINDS,(-1.,0.,2.),(False,True),range(2)):
  yield dict(entry=entry,lengths=(4,),ambient=ambient,reverse=reverse,mode=bank<<21)


def main():
 manifest_path,manifest,baseline,allowed,execute=setup();want={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 rows=set();edges=set();calls=set();records=[];counts={};totals=dict(boundary_comparisons=0,numerical_operations=0,partial_cases=0,independent_cases=0,independent_mesh_cases=0,queue_flushes=0,queue_events=0,writeback_events=0,fipr_instructions=0,external_service_boundaries=0,stub_invocations=0,near_invocations=0)
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,lengths=(4,),reverse=rev,strip_flip=flip) for e,rev,flip in itertools.product(KINDS,(False,True),(False,True))]+[dict(entry='lit_alternate_clipped',lengths=(4,),vertex_flags=(0,1,0,0,0,0,0,0),reverse=True),dict(entry='lit_clipped',alternate_parent=True,topology=0xC000,reverse=True)]
 for n,c in enumerate(selected):
  m,s,f=prepare(c,n);before=bytes(m);ref=QueueReference();native=Memory();bus=Bus()
  LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus));native.qacr[:]=ref.qacr[:]=[12,12]
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  path,status,b,ops,_,external=execute(c,m,s,f,c['entry'],0xFEED1234,allowed)
  if c.get('connected_alternate'):
   assert status==0 and external==0 and path.count(LIT_ALT_CLIP)==path.count(LIT_CLIP)==path.count(STUB)==1
   assert path.count(LIT_NEAR)==int(c['reverse']!=c['strip_flip'])
  flat_alternate.previous.previous.acquisition.compare_queue(native,ref);rows.update(set(path)&want)
  for i,a in enumerate(path):
   if a not in want:continue
   w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
   if w>>8 in (0x89,0x8B,0x8D,0x8F):
    j=i+(2 if w>>8 in (0x8D,0x8F) else 1)
    if j<len(path):edges.add((a,path[j]))
   if w&0xF0FF==0x400B:calls.add(a)
  totals['boundary_comparisons']+=b;totals['numerical_operations']+=ops;totals['partial_cases']+=status!=0
  totals['queue_flushes']+=native.flushes;totals['queue_events']+=native.trace_count+native.trace_dropped
  totals['external_service_boundaries']+=external;totals['stub_invocations']+=path.count(STUB);totals['near_invocations']+=path.count(LIT_NEAR)
  totals['writeback_events']+=sum(e.kind==6 for e in native.trace[:native.trace_count])
  totals['fipr_instructions']+=sum(int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')&0xF0FF==0xF0ED for a in path)
  if not status:
   checked=independent(c,m,before,s,native);totals['independent_cases']+=checked;totals['independent_mesh_cases']+=checked and c.get('connected_alternate',False)
  counts[c['entry']]=counts.get(c['entry'],0)+1
  records.append(dict(input=c,steps=len(path),status=status,full_state_match=True,queue_match=True))
  if (n+1)%100==0:print(n+1,'native lit alternate-strip cases passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Lit alternate-strip smoke checks passed:',len(records));return
 assert rows==want,('missing rows',[hex(a) for a in sorted(want-rows)])
 assert edges=={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']},('edges',len(edges))
 assert calls=={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']}
 assert totals['external_service_boundaries']==0
 files=[Path(__file__),Path(previous.__file__),Path(flat_alternate.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_lit_alternate_strips.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_lit_alternate_strips_connected.c',PROJECT/'source/retail_empty_stubs.c',PROJECT/'analysis/retail_empty_stubs_verification.json',PROJECT/'source/vertex_bus_lighting.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),**totals,service_targets=sorted(hex(t) for t in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Complete lighting alternate strip emitters with real empty-stub/lighting near-clip dependencies versus original full state/RAM/events, independent lit triangle sets/material/cache/control/counter/cursor checks and parent primitive integration.',limitations=['Constructed bounded metadata uses positive strip counts and lengths >=3, plus original lookahead indices. Invalid count loops/pointers and physical hardware exception dispatch remain unmodeled.','Inherited numerical and saved-pointer guards, coherent-RAM/MMU-off queue scope; no GPU FIFO/pixels/MMU-on/cache-timing/full-game proof. Fourteen other primitive emitters remain unfinished.'],cases=records)
 (PROJECT/'analysis/main_dispatch_lit_alternate_strips_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
