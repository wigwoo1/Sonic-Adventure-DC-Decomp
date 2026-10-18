"""Native alternate strips against original code and independent triangle sets."""
import ctypes,gc,hashlib,inspect,itertools,json,math,sys
from pathlib import Path
import verify_main_dispatch_near_clip as previous
PROJECT=Path(__file__).resolve().parent
vertex=previous.vertex;services=previous.services;mesh=previous.mesh;particle=previous.particle;current=previous.current;base=previous.base
load,store=previous.load,previous.store
Bus,Memory,QueueReference,WORKERS=previous.Bus,previous.Memory,previous.QueueReference,previous.WORKERS
PLAIN,CLIPPED,NEAR=previous.PLAIN,previous.CLIPPED,previous.NEAR
ALTERNATE,ALT_CLIP,STUB=0x8C6149B4,0x8C616BC4,0x8C611708
SERVICES=previous.SERVICES-{ALTERNATE,ALT_CLIP}
SEGMENT,INDICES,CACHE,PACKETS=previous.previous.SEGMENT,previous.previous.INDICES,previous.CACHE,previous.PACKETS
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_alternate_strips.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(previous.SPECS,alternate=(ALTERNATE,'sa_alternate_strip_connected'),alternate_clipped=(ALT_CLIP,'sa_alternate_strip_clipped_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE,ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(previous.LIB,name).argtypes;fn.restype=getattr(previous.LIB,name).restype
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def setup():
 source=inspect.getsource(previous.setup).replace('def setup():','def local_setup():',1)
 source=source.replace('("mesh","primitive","clipped","near")','("mesh","primitive","clipped","near","alternate","alternate_clipped")')
 ns=dict(previous.__dict__);exec(compile(source,'alternate_strip_setup','exec'),ns)
 *_,allowed,execute=ns['local_setup']()
 manifest_path=PROJECT/'analysis/main_dispatch_alternate_strips_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_near_clip_verification.json'
 for item in (manifest,json.loads(baseline.read_text())):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 allowed|={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}|{STUB,STUB+2}
 assert base.binary[STUB-0x8C010000:STUB+4-0x8C010000]==bytes.fromhex('0b000900')
 base.TRACKED|={ALTERNATE,ALT_CLIP,STUB}
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,SERVICES=SERVICES)
 return manifest_path,manifest,baseline,allowed,execute

def prepare(c,n):
 m,s,f=previous.prepare(c,n)
 if c.get('connected_alternate'):
  store(m,mesh.SEGMENTS,0xC000,2);store(m,mesh.SEGMENTS+2,1,2);store(m,mesh.SEGMENTS+4,INDICES)
  for i,v in enumerate((3|(0x8000 if c.get('strip_flip') else 0),0,1,2,0)):store(m,INDICES+i*2,v,2)
  store(m,0x8C88F658,0xFFFFFFFF)
  return m,s,f
 if c['entry'] not in ('alternate','alternate_clipped') and not c.get('alternate_parent'):return m,s,f
 lengths=c.get('lengths',(4,));flip=c.get('strip_flip',False);indices=[]
 store(m,SEGMENT,0xC000,2);store(m,SEGMENT+2,len(lengths),2);store(m,SEGMENT+4,INDICES)
 for length in lengths:indices.extend((length|(0x8000 if flip else 0),*range(length)))
 for i,v in enumerate(indices+[0,1,2,3,4,5,6,7]):store(m,INDICES+i*2,v,2)
 points=c.get('points',((0.,0.),(1.,0.),(0.,1.),(1.,1.),(0.,2.),(1.,2.),(0.,3.),(1.,3.)))
 flags=c.get('vertex_flags',(0,)*8)
 store(m,0x8C88F64C,CACHE);store(m,0x8C88F658,0xFFFFFFFF);store(m,0x8C88F65C,0xFFFFFFFE)
 for a,v in ((0x8C88F610,1.),(0x8C890260,1.),(0x8C890264,1.),(0x8C88F5F0,0.),(0x8C88F5F4,0.)):store(m,a,current.bits(v))
 for i,(x,y) in enumerate(points):
  if c.get('reverse'):x=-x
  flag=flags[i%len(flags)];z=0.5 if flag&1 else 4.;coords=(x,y,z) if flag&1 else (x/z,y/z,1/z)
  for j,v in enumerate((0x12340000+i,0x56780000+i,0x9ABC0000+i,flag,*[current.bits(v) for v in coords],0xDEF00000+i)):store(m,CACHE+i*32+j*4,v)
 for i,v in enumerate((0.125,0.25,0.5,1.)):
  store(m,mesh.STATE+16+i*4,current.bits(v))
  if c['entry'] in ('alternate','alternate_clipped'):f.banks[((f.fpscr>>21)&1)^1][i]=current.bits(v)
 s.r[4]=SEGMENT;s.r[8]=(c.get('cursor',PACKETS)&0x03FFFFFF)|0xE0000000
 return m,s,f

def independent(c,m,before,s,native):
 if c.get('connected_alternate'):
  # Independent camera-space clipping for a three-vertex input strip after
  # actual filter projection and material/primitive dispatch.
  mask=c['near_mask'];accepted=c['reverse']!=c['strip_flip'];polygon=[]
  points=[((-x if c['reverse'] else x),y,0.5 if mask&(1<<i) else 4.) for i,(x,y) in enumerate(((0.,0.),(4.,0.),(0.,4.)))]
  if accepted:
   last=points[-1]
   for point in points:
    if (last[2]<1)!=(point[2]<1):
     t=(1-last[2])/(point[2]-last[2]);polygon.append(tuple(last[j]+t*(point[j]-last[j]) for j in range(3)))
    if point[2]>=1:polygon.append(point)
    last=point
  emitted=len(polygon)
  assert native.flushes==4+emitted
  assert load(m,0x8C88F658)==2 and load(m,0x8C88F65C)==(0xFFFFFFFE+3+emitted)&0xFFFFFFFF
  if emitted==4:polygon=[polygon[i] for i in (0,1,3,2)]
  for i,(x,y,z) in enumerate(polygon):
   address=PACKETS+(i+1)*32
   assert load(m,address)==(0xF0000000 if i==emitted-1 else 0xE0000000)
   got=[current.value(load(m,address+4+j*4)) for j in range(3)];expected=(x/z,y/z,1/z)
   assert all(math.isclose(a,b,rel_tol=0.00001,abs_tol=0.0001) for a,b in zip(got,expected)),(c,'connected clipped strip',got,expected)
   assert [load(m,address+16+j*4) for j in range(4)]==[load(m,mesh.STATE+16+j*4) for j in range(4)]
  cursor=PACKETS+(emitted+1)*32
  assert s.r[8]==(cursor&0x03FFFFFF)|0xE0000000 and load(m,services.HEADER+16)==cursor
  return True
 if c['entry'] not in ('alternate','alternate_clipped') or c.get('mode',0)&0x180000 or any(v&1 for v in c.get('vertex_flags',())):return False
 lengths=c.get('lengths',(4,));flip=c.get('strip_flip',False)
 point=lambda i:tuple(load(before,CACHE+i*32+16+j*4) for j in range(3))
 xy=lambda i:tuple(current.value(v) for v in point(i)[:2])
 expected=[]
 for length in lengths:
  for i in range(length-2):
   a,b,d=(xy(j) for j in (i,i+1,i+2));area=(b[0]-a[0])*(d[1]-a[1])-(b[1]-a[1])*(d[0]-a[0])
   flags=[load(before,CACHE+j*32+12) for j in (i,i+1,i+2)]
   outside=c['entry']=='alternate_clipped' and bool(flags[0]&flags[1]&flags[2]&63)
   if not outside and area*(-1 if flip^(i%2!=0) else 1)<0:expected.append(tuple(sorted(point(j) for j in (i,i+1,i+2))))
 actual=[];group=[];groups=0
 for i in range(native.flushes):
  a=c.get('cursor',PACKETS)+i*32;control=load(m,a)
  assert control in (0xE0000000,0xF0000000)
  group.append(tuple(load(m,a+4+j*4) for j in range(3)))
  assert [load(m,a+16+j*4) for j in range(4)]==[load(before,mesh.STATE+16+j*4) for j in range(4)]
  if control==0xF0000000:
   groups+=1
   assert len(group)>=3
   actual.extend(tuple(sorted(group[j:j+3])) for j in range(len(group)-2));group=[]
 assert not group and actual==expected,(c,'independent visible strip triangles',actual,expected)
 assert s.r[8]==((c.get('cursor',PACKETS)&0x03FFFFFF)|0xE0000000)+32*native.flushes
 assert load(m,0x8C88F658)==(0xFFFFFFFF+sum(lengths))&0xFFFFFFFF
 # The original counters differ between variants: the plain path adds one
 # extra unit per closed output strip; the clipped path adds input lengths.
 delta=native.flushes+groups if c['entry']=='alternate' else sum(lengths)
 assert load(m,0x8C88F65C)==(0xFFFFFFFE+delta)&0xFFFFFFFF
 return True

def cases():
 for entry,lengths,reverse,flip,bank in itertools.product(('alternate','alternate_clipped'),((3,),(4,),(5,),(6,),(3,4),(4,3)),(False,True),(False,True),range(2)):
  yield dict(entry=entry,lengths=lengths,reverse=reverse,strip_flip=flip,mode=bank<<21)
 for entry,pattern,reverse,flip,bank in itertools.product(('alternate','alternate_clipped'),range(2),(False,True),(False,True),range(2)):
  points=((0.,0.),(1.,0.),(0.,1.),(-1.,0.),(0.,-1.),(1.,0.),(2.,0.),(2.,1.)) if pattern else ((0.,0.),)*8
  yield dict(entry=entry,lengths=(6,),points=points,reverse=reverse,strip_flip=flip,mode=bank<<21,cursor=PACKETS+32)
 for flags,reverse,flip,bank in itertools.product(((1,)*8,(2,)*8,(4,)*8,(0,1,0,0,0,0,0,0),(1,0,1,0,0,0,0,0),(0,1,1,1,0,0,0,0)),(False,True),(False,True),range(2)):
  yield dict(entry='alternate_clipped',lengths=(6,),vertex_flags=flags,reverse=reverse,strip_flip=flip,mode=bank<<21)
 for entry,mode,rm in itertools.product(('alternate','alternate_clipped'),range(8),range(4)):
  yield dict(entry=entry,lengths=(4,),mode=mode<<19,rm=rm,reverse=True)
 for entry,flags,mode in itertools.product(('alternate','alternate_clipped'),((0,1,0,0,0,0,0,0),),range(8)):
  yield dict(entry=entry,lengths=(3,),mode=mode<<19,vertex_flags=flags,reverse=True)
 for entry,bank,reverse,flip in itertools.product(('primitive','clipped'),range(2),(False,True),(False,True)):
  yield dict(entry=entry,alternate_parent=True,topology=0xC000,mode=bank<<21,reverse=reverse,strip_flip=flip,lengths=(4,))
 for mask,reverse,flip,bank in itertools.product((1,2,3,4,5,6),(False,True),(False,True),range(2)):
  yield dict(entry='mesh',connected_triangle=True,connected_alternate=True,near_mask=mask,reverse=reverse,strip_flip=flip,mode=bank<<21,layout=0x02000000)

def main():
 manifest_path,manifest,baseline,allowed,execute=setup();want={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 rows=set();edges=set();calls=set();records=[];counts={};totals=dict(boundary_comparisons=0,numerical_operations=0,partial_cases=0,independent_cases=0,queue_flushes=0,queue_events=0,external_service_boundaries=0,stub_invocations=0,near_invocations=0)
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,lengths=(4,),reverse=rev,strip_flip=flip) for e,rev,flip in itertools.product(('alternate','alternate_clipped'),(False,True),(False,True))]+[dict(entry='alternate_clipped',lengths=(4,),vertex_flags=(0,1,0,0,0,0,0,0),reverse=True),dict(entry='clipped',alternate_parent=True,topology=0xC000,reverse=True)]
 for n,c in enumerate(selected):
  m,s,f=prepare(c,n);before=bytes(m);ref=QueueReference();native=Memory();bus=Bus()
  LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus));native.qacr[:]=ref.qacr[:]=[12,12]
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  path,status,b,ops,_,external=execute(c,m,s,f,c['entry'],0xFEED1234,allowed)
  if c.get('connected_alternate'):
   assert status==0 and external==0 and path.count(ALT_CLIP)==path.count(CLIPPED)==path.count(STUB)==1
   assert path.count(NEAR)==int(c['reverse']!=c['strip_flip'])
  previous.previous.acquisition.compare_queue(native,ref);rows.update(set(path)&want)
  for i,a in enumerate(path):
   if a not in want:continue
   w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
   if w>>8 in (0x89,0x8B,0x8D,0x8F):
    j=i+(2 if w>>8 in (0x8D,0x8F) else 1)
    if j<len(path):edges.add((a,path[j]))
   if w&0xF0FF==0x400B:calls.add(a)
  totals['boundary_comparisons']+=b;totals['numerical_operations']+=ops;totals['partial_cases']+=status!=0
  totals['queue_flushes']+=native.flushes;totals['queue_events']+=native.trace_count+native.trace_dropped
  totals['external_service_boundaries']+=external;totals['stub_invocations']+=path.count(STUB);totals['near_invocations']+=path.count(NEAR)
  if not status:totals['independent_cases']+=independent(c,m,before,s,native)
  counts[c['entry']]=counts.get(c['entry'],0)+1
  records.append(dict(input=c,steps=len(path),status=status,full_state_match=True,queue_match=True))
  if (n+1)%100==0:print(n+1,'native alternate-strip cases passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Alternate-strip smoke checks passed:',len(records));return
 assert rows==want,('missing rows',[hex(a) for a in sorted(want-rows)])
 assert edges=={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']},('edges',len(edges))
 assert calls=={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']}
 assert totals['external_service_boundaries']==0
 files=[Path(__file__),Path(previous.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_alternate_strips.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_alternate_strips_connected.c',PROJECT/'source/retail_empty_stubs.c',PROJECT/'analysis/retail_empty_stubs_verification.json']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),**totals,service_targets=sorted(hex(t) for t in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Complete alternate strip emitters with real empty-stub/near-clip dependencies versus original full state/RAM/events, independent triangle sets/material/control/counter/cursor checks and parent primitive integration.',limitations=['Constructed bounded metadata uses positive strip counts and lengths >=3, plus original lookahead indices. Invalid count loops/pointers and physical hardware exception dispatch remain unmodeled.','Inherited numerical and saved-pointer guards, coherent-RAM/MMU-off queue scope; no GPU FIFO/pixels/MMU-on/cache-timing/full-game proof. Nineteen other acquisition/primitive paths remain unfinished.'],cases=records)
 (PROJECT/'analysis/main_dispatch_alternate_strips_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
