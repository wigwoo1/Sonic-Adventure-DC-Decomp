"""Native primitive bodies versus original instructions and packet invariants."""
import ctypes,gc,hashlib,inspect,itertools,json,sys
from pathlib import Path
import verify_main_dispatch_acquisition as acquisition
PROJECT=Path(__file__).resolve().parent
vertex=acquisition.previous;services=acquisition.services;mesh=acquisition.mesh;particle=acquisition.particle;current=acquisition.current;base=acquisition.base
load,store=acquisition.load,acquisition.store
Bus,Memory,QueueReference,WORKERS=vertex.Bus,vertex.Memory,vertex.QueueReference,vertex.WORKERS
PLAIN,CLIPPED=0x8C61FFE4,0x8C620428
LOWER={0x8C613312,0x8C6149B4,0x8C616BC4}
SERVICES=(acquisition.SERVICES-{PLAIN,CLIPPED})|LOWER
SEGMENT,INDICES,CACHE,PACKETS=0x8CDD0000,0x8CDD1000,0x8CD40000,acquisition.PACKETS
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_primitives.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(acquisition.SPECS,primitive=(PLAIN,'sa_primitive_emit_connected'),clipped=(CLIPPED,'sa_primitive_clipped_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE,ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(acquisition.LIB,name).argtypes;fn.restype=getattr(acquisition.LIB,name).restype
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def effect(c,target,mem,r,sr,fp):
 if target not in LOWER:return mesh.effect(c,target,mem,r,sr,fp)
 if c.get('capture') is not None:c['capture'].append((target,r[4],r[5],load(mem,mesh.FLAGS)))
 for i in range(8):r[i]=0xBAD00000+i*0x101
 bank=(fp.fpscr>>21)&1
 for i in range(6):fp.banks[bank][i]=current.bits(0.125*(i+1))
 r[8]=(r[8]+c.get('fixture_advance',0))&0xFFFFFFFF
 return sr^1

def setup():
 # Extend the established process-local executor's ABI assertion: the primitive
 # bodies intentionally return an advanced R8 queue cursor, like the mesh body.
 source=inspect.getsource(vertex.setup).replace('def setup():','def local_setup():',1)
 old=' source=source.replace(marker,extra+marker)'
 abi_old='  assert list(s.r)[8:15]==list(initial.r)[8:15]'
 abi_new='  assert list(s.r)[9 if entry in ("mesh","primitive","clipped") else 8:15]==list(initial.r)[9 if entry in ("mesh","primitive","clipped") else 8:15]'
 addition=' changes['+repr(abi_old)+']='+repr(abi_new)+'\n'
 replacement=' extra=extra.rsplit(" for old,new in changes.items():",1);extra=extra[0]+'+repr(addition)+'+" for old,new in changes.items():"+extra[1]\n'+old
 assert source.count(old)==1;source=source.replace(old,replacement)
 ns=dict(vertex.__dict__);exec(compile(source,'primitive_setup','exec'),ns)
 *_,allowed,execute=ns['local_setup']()
 manifest_path=PROJECT/'analysis/main_dispatch_primitives_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_acquisition_verification.json'
 for item in (manifest,json.loads(baseline.read_text()),json.loads((PROJECT/'analysis/main_dispatch_acquisition_translation.json').read_text())):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
  if 'routines' in item:allowed|={int(a,16) for r in item['routines'] for a in r['reachable_addresses']}
 base.TRACKED|={PLAIN,CLIPPED,acquisition.ACQUIRE}
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,SERVICES=SERVICES,effect=effect)
 return manifest_path,manifest,baseline,allowed,execute

def prepare(c,n):
 m,s,f=acquisition.prepare(c,n)
 if c['entry'] not in ('primitive','clipped'):return m,s,f
 topology=c.get('topology',0);count=c.get('count',1);strip_count=c.get('strip_count',4)
 store(m,SEGMENT,topology|c.get('material_index',0),2);store(m,SEGMENT+2,count,2);store(m,SEGMENT+4,INDICES)
 indices=[]
 for _ in range(count):
  if topology==0:indices.extend(c.get('indices',(0,1,2)))
  elif topology==0x4000:indices.extend(c.get('indices',(0,1,2,3)))
  elif topology==0x8000:indices.extend((strip_count,*c.get('indices',tuple(range(strip_count)))))
 for i,v in enumerate(indices+[0,1,2,3,4,5,6,7]):store(m,INDICES+i*2,v,2)
 store(m,0x8C88F64C,CACHE);store(m,0x8C88F658,0xFFFFFFFF);store(m,0x8C88F65C,0xFFFFFFFE)
 points=c.get('points',((0.,0.),(1.,0.),(0.,1.),(1.,1.),(2.,0.),(2.,1.),(3.,0.),(3.,1.)))
 if c.get('reverse'):points=tuple((-x,y) for x,y in points)
 flags=c.get('vertex_flags',(0,)*8)
 for i,(x,y) in enumerate(points):
  for j,v in enumerate((0x12340000+i,0x56780000+i,0x9ABC0000+i,flags[i%len(flags)],current.bits(x),current.bits(y),current.bits(0.25+i*0.125),0xDEF00000+i)):
   store(m,CACHE+i*32+j*4,v)
 for i,v in enumerate((0.125,0.25,0.5,1.0)):store(m,mesh.STATE+16+i*4,current.bits(v))
 s.r[4]=SEGMENT;s.r[8]=(c.get('cursor',PACKETS)&0x03FFFFFF)|0xE0000000
 return m,s,f

def cases():
 for entry,topology,count,reverse,bank in itertools.product(('primitive','clipped'),(0,0x4000,0x8000,0xC000),(0,1,2),(False,True),range(2)):
  yield dict(entry=entry,topology=topology,count=count,reverse=reverse,mode=bank<<21)
 for entry,strip_count,reverse,bank in itertools.product(('primitive','clipped'),(0,1,2,3,4,5,6),(False,True),range(2)):
  yield dict(entry=entry,topology=0x8000,strip_count=strip_count,reverse=reverse,mode=bank<<21)
 for topology,flags,reverse,bank in itertools.product((0,0x4000,0x8000),((1,)*8,(2,)*8,(4,)*8,(0,1,0,0,0,0,0,0),(0,0,1,0,0,0,0,0),(1,0,0,0,0,0,0,0),(1,1,0,0,0,0,0,0),(1,1,1,0,0,0,0,0),(0,0,0,1,0,0,0,0)),(False,True),range(2)):
  yield dict(entry='clipped',topology=topology,vertex_flags=flags,reverse=reverse,mode=bank<<21)
 for entry,topology,mode,rm in itertools.product(('primitive','clipped'),(0,0x4000,0x8000,0xC000),range(8),range(4)):
  yield dict(entry=entry,topology=topology,mode=mode<<19,rm=rm)
 for entry,topology,bank in itertools.product(('primitive','clipped'),(0,0x4000,0x8000),range(2)):
  yield dict(entry=entry,topology=topology,points=((0.,0.),)*8,mode=bank<<21,cursor=PACKETS+32)
 for bank in range(2):
  # Only the second half of a quad is entirely behind the near plane.
  yield dict(entry='clipped',topology=0x4000,vertex_flags=(0,1,1,1,0,0,0,0),mode=bank<<21)
  # A mixed two-vertex strip reaches clipping dispatch but has no triangle.
  yield dict(entry='clipped',topology=0x8000,strip_count=2,vertex_flags=(0,1,0,0,0,0,0,0),mode=bank<<21)
 for entry,variant,bank in itertools.product(('draw','primary'),range(2),range(2)):
  yield dict(entry=entry,graph='retail',variant=variant,mode=bank<<21)

def independent(c,m,before,s,native,captured):
 if c['entry'] not in ('primitive','clipped') or c.get('mode',0)&0x180000 or c.get('topology',0)!=0:return False
 count=c.get('count',1)
 xy=[tuple(current.value(load(before,CACHE+i*32+16+j*4)) for j in range(2)) for i in range(3)]
 area=(xy[1][0]-xy[0][0])*(xy[2][1]-xy[0][1])-(xy[1][1]-xy[0][1])*(xy[2][0]-xy[0][0])
 flags=[load(before,CACHE+i*32+12) for i in range(3)]
 rejected=c['entry']=='clipped' and bool(flags[0]&flags[1]&flags[2]&63)
 near=c['entry']=='clipped' and not rejected and bool((flags[0]|flags[1]|flags[2])&1)
 emitted=3*count if not rejected and not near and area<=0 else 0
 assert len(captured)==2*count*int(near)
 if near:assert all(target==0x8C613312 for target,*_ in captured)
 assert load(m,0x8C88F658)==(0xFFFFFFFF+3*count)&0xFFFFFFFF
 assert load(m,0x8C88F65C)==(0xFFFFFFFE+emitted)&0xFFFFFFFF
 assert native.flushes==emitted
 start=(c.get('cursor',PACKETS)&0x03FFFFFF)|0xE0000000
 assert s.r[8]==start+32*emitted
 for i in range(emitted):
  index=i%3;destination=c.get('cursor',PACKETS)+i*32
  expected=[0xF0000000 if index==2 else 0xE0000000]+[load(before,CACHE+index*32+16+j*4) for j in range(3)]+[load(before,mesh.STATE+16+j*4) for j in range(4)]
  assert [load(m,destination+j*4) for j in range(8)]==expected,(c,'independent emitted packet',i)
 return True

def main():
 manifest_path,manifest,baseline,allowed,execute=setup()
 wanted={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']};rows=set();edges=set();calls=set();records=[];counts={};first={}
 totals=dict(boundary_comparisons=0,numerical_operations=0,partial_cases=0,independent_cases=0,queue_flushes=0,queue_events=0,external_service_boundaries=0)
 def check(c,n):
  m,s,f=prepare(c,n);before=bytes(m);ref=QueueReference();native=Memory();bus=Bus()
  LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus))
  if c['entry'] in ('primitive','clipped'):native.qacr[:]=ref.qacr[:]=[12,12]
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  captured=[];case=dict(c,capture=captured)
  path,status,b,ops,_,external=execute(case,m,s,f,c['entry'],0xFEED1234,allowed)
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
 if '--smoke' in sys.argv:selected=[dict(entry=e,topology=t,reverse=rev) for e,t,rev in itertools.product(('primitive','clipped'),(0,0x4000,0x8000,0xC000),(False,True))]+[dict(entry='clipped',vertex_flags=(0,1,0,0,0,0,0,0)),dict(entry='draw',graph='retail')]
 for n,c in enumerate(selected):
  check(c,n)
  if (n+1)%100==0:print(n+1,'primitive comparisons passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Primitive smoke checks passed:',len(records));return
 for target,c in sorted(first.items()):
  check(dict(c,missing_target=target),len(records));check(dict(c,fail_target=target),len(records))
 assert rows==wanted,('missing rows',[hex(a) for a in sorted(wanted-rows)])
 want_edges={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert edges==want_edges,('missing edges',[[hex(a),hex(b)] for a,b in sorted(want_edges-edges)])
 assert calls=={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']}
 assert LOWER<=set(first),('lower services',LOWER-set(first))
 files=[Path(__file__),Path(acquisition.__file__),Path(vertex.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_primitives.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_primitives_connected.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),**totals,service_targets=sorted(hex(t) for t in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Complete native particle primitive bodies versus original state/RAM/boundaries/queue events, every topology/fallback, clipping masks and independent triangle packets/counters/cursor.',limitations=['Three lower clipping/alternate helpers and 19 other acquisition/primitive services remain fixtures; these are not native rendering proof.','Valid bounded RAM/queue streams only, inherited numerical guards, no hardware timing/GPU FIFO/pixels/MMU-on/fault or full-game proof.'],cases=records)
 (PROJECT/'analysis/main_dispatch_primitives_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
