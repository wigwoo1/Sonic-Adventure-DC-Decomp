"""Native near-plane clipping versus original code and independent polygons."""
import ctypes,gc,hashlib,inspect,itertools,json,math,sys
from pathlib import Path
from types import SimpleNamespace
import verify_main_dispatch_primitives as previous
PROJECT=Path(__file__).resolve().parent
vertex=previous.vertex;services=previous.services;mesh=previous.mesh;particle=previous.particle;current=previous.current;base=previous.base
load,store=previous.load,previous.store
Bus,Memory,QueueReference,WORKERS=previous.Bus,previous.Memory,previous.QueueReference,previous.WORKERS
PLAIN,CLIPPED,NEAR=previous.PLAIN,previous.CLIPPED,0x8C613312
SERVICES=previous.SERVICES-{NEAR};LOWER=previous.LOWER-{NEAR}
CACHE,PACKETS=previous.CACHE,previous.PACKETS
RECORDS=0x8CD41000
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_near_clip.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(previous.SPECS,near=(NEAR,'sa_triangle_near_clip_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE,ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(previous.LIB,name).argtypes;fn.restype=getattr(previous.LIB,name).restype
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def setup():
 source=inspect.getsource(previous.setup).replace('def setup():','def local_setup():',1)
 source=source.replace('("mesh","primitive","clipped")','("mesh","primitive","clipped","near")')
 guard=" changes['  assert pc in allowed,hex(pc);path.append(pc)']+=chr(10)+'  if pc==0x8c61345a and ((r[2]+12)&0x1f000000)!=0x0c000000:stop(pc,r,sr,pr,-3)'\n"
 marker=" addition=' changes['+repr(abi_old)+']='+repr(abi_new)+'\\n'"
 assert source.count(marker)==1;source=source.replace(marker,marker+'\n addition+='+repr(guard))
 ns=dict(previous.__dict__);exec(compile(source,'near_clip_setup','exec'),ns)
 *_,allowed,execute=ns['local_setup']()
 manifest_path=PROJECT/'analysis/main_dispatch_near_clip_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_primitives_verification.json'
 for item in (manifest,json.loads(baseline.read_text())):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 allowed|={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']};base.TRACKED.add(NEAR)
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,SERVICES=SERVICES)
 return manifest_path,manifest,baseline,allowed,execute

def prepare(c,n):
 m,s,f=previous.prepare(c,n)
 if c.get('connected_triangle'):
  # Valid indexed mesh passes real vertices through filter -> queue -> clipper.
  store(m,mesh.DATA+8,3);store(m,mesh.SEGMENTS+2,1,2)
  store(m,mesh.SEGMENTS+4,previous.INDICES)
  for i in range(3):store(m,previous.INDICES+i*2,i,2)
  for i,(x,y) in enumerate(((0.,0.),(4.,0.),(0.,4.),(0.,0.))):
   if c.get('reverse'):x=-x
   z=0.5 if c.get('near_mask',1)&(1<<i) else 4.
   for j,v in enumerate((x,y,z)):store(m,0x8CD50000+i*12+j*4,current.bits(v))
  store(m,0x8C88F65C,0xFFFFFFFE)
 if c['entry']!='near':return m,s,f
 near=c.get('plane',1.);scale=c.get('scale',(1.,1.));offset=c.get('offset',(0.,0.));mask=c.get('near_mask',1)
 points=c.get('camera_points',((0.,0.),(4.,0.),(0.,4.)))
 for a,v in ((0x8C88F610,near),(0x8C890260,scale[0]),(0x8C890264,scale[1]),(0x8C88F5F0,offset[0]),(0x8C88F5F4,offset[1])):store(m,a,current.bits(v))
 for i,(x,y) in enumerate(points):
  if c.get('reverse'):x=-x
  behind=bool(mask&(1<<i));z=near*(0.5 if behind else 4.)
  coords=(x,y,z) if behind else (x*scale[0]/z+offset[0],y*scale[1]/z+offset[1],1/z)
  for j,v in enumerate((0x12340000+i,0x56780000+i,0x9ABC0000+i,int(behind),*[current.bits(v) for v in coords],0xDEF00000+i)):store(m,RECORDS+i*32+j*4,v)
 for i in range(4*8):store(m,0x8C890278+i*4,0x3F000000+i*0x100)
 for i,v in enumerate((0.125,0.25,0.5,1.)):store(m,mesh.STATE+16+i*4,current.bits(v))
 store(m,0x8C8902FC,c.get('cull',1));store(m,0x8C88F65C,0xFFFFFFFE)
 s.r[4:7]=[RECORDS+i*32 for i in range(3)];s.r[8]=(c.get('cursor',PACKETS)&0x03FFFFFF)|0xE0000000
 return m,s,f

def independent(c,m,before,s,native):
 if c.get('connected_triangle'):
  # The first three transfers project vertices; acquisition transfers a header.
  # Compare emitted geometry at the next cursor against the same polygon oracle.
  original=bytearray(before)
  for i in range(4):store(original,mesh.STATE+16+i*4,load(m,mesh.STATE+16+i*4))
  result=independent(dict(c,entry='near',connected_triangle=False,cursor=PACKETS+32),m,original,s,SimpleNamespace(flushes=native.flushes-4))
  assert load(m,services.HEADER+16)==(s.r[8]&0x03FFFFFF)|0x8C000000
  return result
 if c['entry']!='near' or c.get('mode',0)&0x180000 or c.get('near_mask',1)==7:return False
 near=c.get('plane',1.);scale=c.get('scale',(1.,1.));offset=c.get('offset',(0.,0.));mask=c.get('near_mask',1)
 points=[((-x if c.get('reverse') else x),y,near*(0.5 if mask&(1<<i) else 4.)) for i,(x,y) in enumerate(c.get('camera_points',((0.,0.),(4.,0.),(0.,4.))))]
 a,b,d=points
 determinant=a[0]*(b[1]*d[2]-b[2]*d[1])-a[1]*(b[0]*d[2]-b[2]*d[0])+a[2]*(b[0]*d[1]-b[1]*d[0])
 rejected=bool(c.get('cull',1)&int(determinant>0));polygon=[]
 if not rejected:
  last=points[-1]
  for point in points:
   if (last[2]<near)!=(point[2]<near):
    t=(near-last[2])/(point[2]-last[2]);polygon.append(tuple(last[j]+t*(point[j]-last[j]) for j in range(3)))
   if point[2]>=near:polygon.append(point)
   last=point
 expected=len(polygon)
 assert expected in (0,3,4)
 assert native.flushes==expected
 assert s.r[8]==((c.get('cursor',PACKETS)&0x03FFFFFF)|0xE0000000)+32*expected
 assert load(m,0x8C88F65C)==(0xFFFFFFFE+expected)&0xFFFFFFFF
 assert load(m,0x8C8902FC)==(c.get('cull',1) if rejected else 0)
 # The four-vertex result is a strip: perimeter order 0,1,2,3 becomes 0,1,3,2.
 if expected==4:polygon=[polygon[i] for i in (0,1,3,2)]
 for i,(x,y,z) in enumerate(polygon):
  packet=c.get('cursor',PACKETS)+i*32
  assert load(m,packet)==(0xF0000000 if i==expected-1 else 0xE0000000)
  got=[current.value(load(m,packet+4+j*4)) for j in range(3)]
  want=[x*scale[0]/z+offset[0],y*scale[1]/z+offset[1],1/z]
  assert all(math.isclose(v,w,rel_tol=0.00001,abs_tol=0.0001) for v,w in zip(got,want)),(c,'independent clip geometry',i,got,want)
  assert [load(m,packet+16+j*4) for j in range(4)]==[load(before,mesh.STATE+16+j*4) for j in range(4)]
 return True

def cases():
 for mask,reverse,cull,bank in itertools.product(range(8),(False,True),(0,1),range(2)):
  yield dict(entry='near',near_mask=mask,reverse=reverse,cull=cull,mode=bank<<21)
 for mask,plane,projection,reverse,bank in itertools.product((1,2,3,4,5,6),(0.5,1.,2.),range(2),(False,True),range(2)):
  yield dict(entry='near',near_mask=mask,plane=plane,scale=((2.,4.) if projection else (1.,1.)),offset=(0.125,0.25),reverse=reverse,mode=bank<<21,cursor=PACKETS+32)
 for mask,mode,rm in itertools.product((1,3,6),range(8),range(4)):
  yield dict(entry='near',near_mask=mask,mode=mode<<19,rm=rm,reverse=True)
 for enable,bank,rm in itertools.product((0,1,2,4,8,16,32),range(2),range(2)):
  yield dict(entry='near',near_mask=1,enable=enable,mode=bank<<21,rm=rm,reverse=True)
 for mask,cull,bank in itertools.product((1,6),(0,1),range(2)):
  yield dict(entry='near',near_mask=mask,cull=cull,mode=bank<<21,camera_points=((0.,0.),(1.,0.),(2.,0.)))
 for topology,flags,reverse,bank in itertools.product((0,0x4000,0x8000),((0,1,0,0,0,0,0,0),(1,1,0,0,0,0,0,0),(0,1,1,1,0,0,0,0)),(False,True),range(2)):
  yield dict(entry='clipped',topology=topology,vertex_flags=flags,reverse=reverse,mode=bank<<21)
 for entry,variant,bank in itertools.product(('draw','primary'),range(2),range(2)):
  yield dict(entry=entry,graph='retail',variant=variant,mode=bank<<21)
 for mask,reverse,bank in itertools.product((1,2,3,4,5,6),(False,True),range(2)):
  yield dict(entry='mesh',connected_triangle=True,near_mask=mask,reverse=reverse,mode=bank<<21,layout=0x02000000)

def main():
 manifest_path,manifest,baseline,allowed,execute=setup();want={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 rows=set();edges=set();records=[];counts={};totals=dict(boundary_comparisons=0,numerical_operations=0,partial_cases=0,independent_cases=0,queue_flushes=0,queue_events=0,external_service_boundaries=0,near_invocations=0)
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry='near',near_mask=mask,reverse=rev) for mask,rev in itertools.product((1,3,6),(False,True))]+[dict(entry='near',near_mask=1,mode=0x100000,reverse=True),dict(entry='near',mode=0x80000),dict(entry='clipped',vertex_flags=(0,1,0,0,0,0,0,0)),dict(entry='draw',graph='retail')]
 for n,c in enumerate(selected):
  m,s,f=prepare(c,n);before=bytes(m);ref=QueueReference();native=Memory();bus=Bus()
  LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus))
  if c['entry'] in ('primitive','clipped','near'):native.qacr[:]=ref.qacr[:]=[12,12]
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  path,status,b,ops,_,external=execute(c,m,s,f,c['entry'],0xFEED1234,allowed)
  if c.get('connected_triangle'):assert status==0 and external==0 and path.count(NEAR)==path.count(CLIPPED)==1
  previous.acquisition.compare_queue(native,ref);rows.update(set(path)&want)
  for i,a in enumerate(path):
   if a not in want:continue
   w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
   if w>>8 in (0x89,0x8B,0x8D,0x8F):
    j=i+(2 if w>>8 in (0x8D,0x8F) else 1)
    if j<len(path):edges.add((a,path[j]))
  totals['boundary_comparisons']+=b;totals['numerical_operations']+=ops;totals['partial_cases']+=status!=0
  totals['queue_flushes']+=native.flushes;totals['queue_events']+=native.trace_count+native.trace_dropped
  totals['external_service_boundaries']+=external;totals['near_invocations']+=path.count(NEAR)
  if not status:totals['independent_cases']+=independent(c,m,before,s,native)
  counts[c['entry']]=counts.get(c['entry'],0)+1
  records.append(dict(input=c,steps=len(path),status=status,full_state_match=True,queue_match=True))
  if (n+1)%100==0:print(n+1,'native near-plane clipping cases passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Near-plane clipping smoke checks passed:',len(records));return
 assert rows==want,('missing rows',[hex(a) for a in sorted(want-rows)])
 assert edges=={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 files=[Path(__file__),Path(previous.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_near_clip.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_near_clip_connected.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=0,**totals,service_targets=sorted(hex(t) for t in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Complete native near-plane triangle helper and primitive/particle callers against original full state/RAM/queue events, with independent clipped polygon order/projection/material/control/cursor/counter checks.',limitations=['Bounded valid coherent-RAM/MMU-off streams and inherited numeric/oracle guards; no GPU FIFO, visible pixels, MMU-on, physical cache timing/fault or full-game proof.','Two alternate helpers and 19 other acquisition/primitive services remain fixtures. All-near standalone input is outside the normal caller contract and has only instruction-comparison evidence.'],cases=records)
 (PROJECT/'analysis/main_dispatch_near_clip_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
