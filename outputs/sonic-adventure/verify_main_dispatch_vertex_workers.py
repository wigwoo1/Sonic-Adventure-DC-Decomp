"""Native vertex records and C store queues versus original code/Python queues."""
import ctypes,gc,hashlib,inspect,itertools,json,math,sys
from pathlib import Path
import verify_main_dispatch_mesh_services as services
from verify_tikal_controller import P
PROJECT=Path(__file__).resolve().parent
mesh=services.mesh;traversal=services.traversal;particle=services.particle;geom=services.geom;current=services.current;base=services.base
load,store=mesh.load,mesh.store
WORKERS={0x8C611060,0x8C611122,0x8C6111D6,0x8C6112E2};SERVICES=services.SERVICES-WORKERS
RECORD,POSITIONS,NORMALS,OUTPUT=0x8CD70000,0x8CD80000,0x8CD90000,0x8CDA0000
Read=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint,P)
Write=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint32,ctypes.c_uint)
Cache=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint)
class Bus(ctypes.Structure):_fields_=[('context',ctypes.c_void_p),('read',Read),('write',Write),('cache',Cache)]
class BusEvent(ctypes.Structure):_fields_=[('kind',ctypes.c_uint32),('address',ctypes.c_uint32),('value',ctypes.c_uint32),('width',ctypes.c_uint32)]
class Memory(ctypes.Structure):_fields_=[('ram',ctypes.POINTER(ctypes.c_uint8)),('qacr',ctypes.c_uint32*2),('queue',ctypes.c_uint32*16),('invalidates',ctypes.c_uint32),('prefetches',ctypes.c_uint32),('flushes',ctypes.c_uint32),('trace_count',ctypes.c_uint32),('trace_dropped',ctypes.c_uint32),('trace',BusEvent*4096)]
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_vertex_workers.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(services.SPECS,project_normals=(0x8C611060,'sa_vertex_project_normals_connected'),project=(0x8C611122,'sa_vertex_project_connected'),classify_normals=(0x8C6111D6,'sa_vertex_classify_normals_connected'),classify=(0x8C6112E2,'sa_vertex_classify_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE,ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide'):
 fn=getattr(LIB,name);fn.argtypes=getattr(services.LIB,name).argtypes;fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=[ctypes.POINTER(Memory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Bus)]

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

class QueueReference:
 def __init__(self):self.qacr=[0,0];self.queue=[0]*16;self.events=[];self.invalidates=self.prefetches=self.flushes=0;self.ram=None
 def __contains__(self,a):return a in (0xFF000038,0xFF00003C) or a>>26==0x38
 def __getitem__(self,a):
  assert a in (0xFF000038,0xFF00003C);v=self.qacr[(a>>2)&1];self.events.append((0,a,v,4));return v
 def __setitem__(self,a,v):
  v&=0xFFFFFFFF
  if a in (0xFF000038,0xFF00003C):self.events.append((1,a,v,4));self.qacr[(a>>2)&1]=v
  else:assert a>>26==0x38;self.events.append((2,a,v,4));self.queue[(a%64)//4]=v
 def invalidate(self,a):self.events.append((4,a,0,0));self.invalidates+=1
 def prefetch(self,a):
  self.events.append((3,a,0,0));self.prefetches+=1
  if a>>26!=0x38:return
  q=(a//32)%2;destination=(a&0x3FFFFE0)+((self.qacr[q]&28)*0x1000000)
  assert destination&0x1F000000==0xC000000
  self.events.append((5,destination,q,32));self.flushes+=1
  for i,value in enumerate(self.queue[q*8:q*8+8]):store(self.ram,destination+i*4,value)

def setup():
 probe=Memory();ctypes.memset(ctypes.byref(probe),0xA5,ctypes.sizeof(probe));probe_bus=Bus();probe_ram=(ctypes.c_uint8*1)()
 LIB.sa_vertex_bus_init(ctypes.byref(probe),probe_ram,ctypes.byref(probe_bus))
 assert list(probe.qacr)==[0,0] and list(probe.queue)==[0]*16
 assert (probe.invalidates,probe.prefetches,probe.flushes,probe.trace_count,probe.trace_dropped)==(0,0,0,0,0)
 source=inspect.getsource(services.setup).replace('def setup():','def local_setup():',1)
 source=source.replace("'traversal.setup()','mesh.setup()'","'traversal.setup()','services.setup()'")
 source=source.replace('main_dispatch_mesh_services_translation.json','main_dispatch_vertex_workers_translation.json').replace('main_dispatch_mesh_verification.json','main_dispatch_mesh_services_verification.json')
 source=source.replace('mesh=mesh,LIB=LIB','mesh=mesh,services=services,LIB=LIB')
 marker=' ns=dict(mesh.__dict__);';assert source.count(marker)==1
 extra=""" source=source.replace(' for old,new in changes.items():', ''' changes[' em=bytearray(m);expected=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f)']=' em=bytearray(m);queue_reference.ram=em;expected=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f)'
 changes['  assert pc in allowed,hex(pc);path.append(pc)']+='\\\\n  if c.get("null_bus") and w>>12==1 and ((r[(w>>8)&15]+(w&15)*4)&0xffffffff) in (0xff000038,0xff00003c):stop(pc,r,sr,pr,-2)'
 for key in list(changes):
  if key.startswith(' try:base.previous.reference'):changes[key]=changes[key].replace('instruction_budget=40000)', 'instruction_budget=40000,mmio=queue_reference,cache_invalidate=queue_reference.invalidate,cache_prefetch=queue_reference.prefetch)')
 changes['None,Call(),observe)']=changes['None,Call(),observe)'].replace('observe)', 'observe,bus_pointer)')
 for old,new in changes.items():''')
 source=source.replace('STATE=STATE,FLAGS=FLAGS,MODE=MODE,SERVICES=SERVICES,effect=effect)', 'STATE=STATE,FLAGS=FLAGS,MODE=MODE,SERVICES=SERVICES,effect=effect,queue_reference=None,bus_pointer=None)')
"""
 source=source.replace(marker,extra+marker)
 ns=dict(services.__dict__);ns.update(PROJECT=PROJECT,services=services,LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,NATIVE=services.NATIVE|WORKERS,SERVICES=SERVICES)
 exec(compile(source,'vertex_workers_setup','exec'),ns)
 result=ns['local_setup']()
 # Preserve the established MACL publication and add PREF only in this process.
 prefix=base.previous._reference_source
 store_line="            struct.pack_into('<I', ram, index(regs[n]), regs[m])"
 assert prefix.count(store_line)==1
 prefix=prefix.replace(store_line,'            write32(regs[n],regs[m])')
 prefix=prefix.replace('cache_purge=None, trace=None):','cache_purge=None, trace=None, cache_prefetch=None):')
 assert prefix.count('        elif word == 9: pass')==1
 prefix=prefix.replace('        elif word == 9: pass','        elif word & 0xF0FF == 0x0083:\n            assert cache_prefetch is not None\n            cache_prefetch(regs[n])\n        elif word == 9: pass')
 ref_ns=dict(base.previous.reference.__globals__);exec(compile(prefix,'vertex_workers_observed_reference','exec'),ref_ns);base.previous.reference=ref_ns['reference']
 return result

def prepare(c,n):
 m,s,f=services.prepare(c,n)
 count=c.get('count',3);out=c.get('output',OUTPUT)
 for i in range(20):store(m,RECORD+i*4,0)
 for i,v in enumerate((count,out,POSITIONS,NORMALS,0xFFFFFFFF,0)):store(m,RECORD+i*4,v)
 for i,v in enumerate((c.get('scale_x',1),c.get('scale_y',1),-10,10,-10,10)):store(m,RECORD+24+i*4,current.bits(v))
 for i in range(count+2):
  point=c.get('point',(-1.0,2.0,4.0))
  for j,v in enumerate(point):store(m,POSITIONS+i*12+j*4,current.bits(v))
  for j,v in enumerate((0.25,0.5,0.75)):store(m,NORMALS+i*12+j*4,current.bits(v))
 # Constructed streams used by the connected filter path, including its lookahead.
 for i in range(20):
  for j,v in enumerate(c.get('point',(-1.0,2.0,4.0))):store(m,0x8CD50000+i*12+j*4,current.bits(v))
  for j,v in enumerate((0.25,0.5,0.75)):store(m,0x8CD60000+i*12+j*4,current.bits(v))
 if c['entry'] in ('project','project_normals','classify','classify_normals'):s.r[4]=RECORD
 return m,s,f

def check_math(c,m,model):
 if c['entry'] not in ('project','project_normals','classify','classify_normals') or c.get('mode',0)&0x180000:return
 count=c.get('count',3);x,y,z=c.get('point',(-1.,2.,4.));out=c.get('output',OUTPUT)
 x=x*c.get('scale_x',1)/abs(z);y=y*c.get('scale_y',1)/abs(z)
 flags=0
 if c['entry'].startswith('classify'):
  flags=(int(y>10)<<5)|(int(y< -10)<<4)|(int(x>10)<<3)|(int(x< -10)<<2)|(int(z>100)<<1)|int(z<1)
  if z<1:flags=int(z>100)*2+int(z<1)
  assert load(m,RECORD+16)==(flags if count else 0xFFFFFFFF) and load(m,RECORD+20)==(flags if count else 0)
 assert model.flushes==count
 for i in range(count):
  a=out+i*32;assert load(m,a+12)==flags
  if z>=1 or not c['entry'].startswith('classify'):
   assert abs(current.value(load(m,a+16))-x)<0.001 and abs(current.value(load(m,a+20))-y)<0.001
   assert abs(current.value(load(m,a+24))-1/abs(z))<0.001
  if 'normals' in c['entry']:assert [load(m,a+j*4) for j in range(3)]==[current.bits(v) for v in (0.25,0.5,0.75)]

def cases():
 for entry,count,bank,rm in itertools.product(('project','project_normals','classify','classify_normals'),(0,1,2,5),range(2),range(2)):
  yield dict(entry=entry,count=count,mode=bank<<21,rm=rm)
 for entry,point,bank in itertools.product(('classify','classify_normals'),((-100,0,4),(100,0,4),(0,-100,4),(0,100,4),(0,0,0.5),(0,0,200),(0,0,4)),range(2)):
  yield dict(entry=entry,point=point,mode=bank<<21,count=2)
 for entry,mode,rm in itertools.product(('project','project_normals','classify','classify_normals'),range(8),range(4)):
  yield dict(entry=entry,mode=mode<<19,rm=rm,count=1)
 for entry,bank in itertools.product(('project','project_normals','classify','classify_normals'),range(2)):
  yield dict(entry=entry,mode=bank<<21,null_bus=True)
 for entry,bank,out in itertools.product(('project','project_normals','classify','classify_normals'),range(2),(OUTPUT+32,0xACDA0020)):
  yield dict(entry=entry,mode=bank<<21,output=out,count=5)
 for entry,bank in itertools.product(('project','project_normals'),range(2)):
  yield dict(entry=entry,point=(1,2,-4),mode=bank<<21)
 yield dict(entry='project',count=600)
 for entry,bank,normals,clip in itertools.product(('filter_core','mesh'),range(2),(False,True),(False,True)):
  yield dict(entry=entry,mode=bank<<21,normals=normals,clip=clip)

def main():
 manifest_path,manifest,baseline,allowed,execute=setup()
 rows=set();edges=set();records=[];counts=dict.fromkeys(SPECS,0);totals=dict(boundary_comparisons=0,numerical_operations=0,partial_cases=0,independent_cases=0,queue_flushes=0,queue_events=0)
 want_rows={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,count=2) for e in ('project','project_normals','classify','classify_normals')]+[dict(entry='project',count=0),dict(entry='filter_core'),dict(entry='mesh'),dict(entry='project',null_bus=True)]
 for n,c in enumerate(selected):
  m,s,f=prepare(c,n);ref=QueueReference();native=Memory();bus=Bus()
  LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus))
  execute.__globals__.update(queue_reference=ref,bus_pointer=None if c.get('null_bus') else ctypes.byref(bus))
  path,status,b,ops,_,_=execute(c,m,s,f,c['entry'],0xFEED1234,allowed)
  actual=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
  assert actual==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096) and list(native.queue)==ref.queue and list(native.qacr)==ref.qacr,(c,'queue state/events')
  assert (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates)
  rows.update(set(path)&want_rows);counts[c['entry']]+=1
  for i,a in enumerate(path):
   if a not in want_rows:continue
   w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
   if w>>8 in (0x89,0x8B,0x8D,0x8F):
    j=i+(2 if w>>8 in (0x8D,0x8F) else 1)
    if j<len(path):edges.add((a,path[j]))
  if status==0:check_math(c,m,native);totals['independent_cases']+=1
  totals['boundary_comparisons']+=b;totals['numerical_operations']+=ops;totals['partial_cases']+=status!=0;totals['queue_flushes']+=native.flushes;totals['queue_events']+=native.trace_count+native.trace_dropped
  records.append(dict(input=c,status=status,steps=len(path),full_state_match=True,queue_match=True,queue_flushes=native.flushes))
  if (n+1)%100==0:print(n+1,'vertex worker comparisons passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Vertex worker smoke checks passed:',len(records));return
 assert rows==want_rows,('missing rows',[hex(a) for a in want_rows-rows])
 want_edges={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']};assert edges==want_edges,('edges',want_edges-edges)
 files=[Path(__file__),Path(services.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_vertex_workers.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_vertex_workers_connected.c',PROJECT/'source/vertex_bus.c',PROJECT/'source/vertex_bus.h']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=0,**totals,sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),scope='Native four vertex workers and C coherent-RAM/MMU-off queue backend versus original instructions and independent Python queues, full state/RAM/events, analytic projected records and clip masks; connected filter/material callers included.',limitations=['Cache invalidation and non-queue prefetch are logged; real cache timing/coherence, MMU-on/TLB, hardware alignment faults and GPU drawing remain unmodeled.','Existing scalar numeric and shared vector numerical-oracle limits retained; constructed valid bounded streams include the original lookahead vertex.','Remaining 22 acquisition/primitive services use fixtures; full frame driver, playable ports and full decompilation unfinished.'],cases=records)
 (PROJECT/'analysis/main_dispatch_vertex_workers_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','rows_covered','conditional_edges_covered',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
