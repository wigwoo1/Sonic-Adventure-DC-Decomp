"""Native primitive bodies versus original instructions and packet invariants."""
import ctypes,gc,hashlib,inspect,itertools,json,sys
from pathlib import Path
import verify_main_dispatch_lit_alternate_strips as previous
import verify_main_dispatch_lit_primitives as lit_parent
import verify_main_dispatch_primitives as plain
import verify_main_dispatch_acquisition as acquisition
PROJECT=Path(__file__).resolve().parent
vertex=previous.vertex;services=acquisition.services;mesh=acquisition.mesh;particle=acquisition.particle;current=acquisition.current;base=acquisition.base
load,store=acquisition.load,acquisition.store
Bus,Memory,QueueReference,WORKERS=previous.Bus,previous.Memory,previous.QueueReference,previous.WORKERS
PLAIN,CLIPPED,NEAR,ALTERNATE,ALT_CLIP=previous.PLAIN,previous.CLIPPED,previous.NEAR,previous.ALTERNATE,previous.ALT_CLIP
SECONDARY,EXTENDED,EXT_SECONDARY=previous.SECONDARY,previous.EXTENDED,previous.EXT_SECONDARY
LIT,LIT_CLIP,LIT_NEAR=previous.LIT,previous.LIT_CLIP,previous.LIT_NEAR
LIT_ALT,LIT_ALT_CLIP=previous.LIT_ALT,previous.LIT_ALT_CLIP
UV_LIT,UV_LIT_CLIP=0x8C618D18,0x8C619310
KINDS=("uv_lit","uv_lit_clipped")
UVS=0x8CDF0000
NEAR_UV=0x8C611A14
SEED=[0xA0A00000+i*0x101 for i in range(16)]
LOWER={NEAR_UV,0x8C61394A,0x8C614F70}
SERVICES=(previous.SERVICES-{UV_LIT,UV_LIT_CLIP})|LOWER
SEGMENT,INDICES,CACHE,PACKETS=0x8CDD0000,0x8CDD1000,0x8CD40000,acquisition.PACKETS
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_uv_lit_primitives.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(previous.SPECS,uv_lit=(UV_LIT,'sa_uv_lit_primitive_emit_connected'),uv_lit_clipped=(UV_LIT_CLIP,'sa_uv_lit_primitive_clipped_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE,ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(previous.LIB,name).argtypes;fn.restype=getattr(previous.LIB,name).restype
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def effect(c,target,mem,r,sr,fp):
 if target not in LOWER:return mesh.effect(c,target,mem,r,sr,fp)
 if c.get('capture') is not None:
  uv=tuple(load(mem,r[7]+i*2,2) for i in range(6)) if target==NEAR_UV else ()
  c['capture'].append((target,r[4],r[5],load(mem,mesh.FLAGS),r[7],uv))
 for i in range(8):r[i]=0xBAD00000+i*0x101
 bank=(fp.fpscr>>21)&1
 for i in range(6):fp.banks[bank][i]=current.bits(0.125*(i+1))
 r[8]=(r[8]+c.get('fixture_advance',0))&0xFFFFFFFF
 return sr^1

def setup():
 source=inspect.getsource(previous.setup).replace('def setup():','def local_setup():',1)
 source=source.replace('("mesh","primitive","clipped","near","alternate","alternate_clipped","lit","lit_clipped","lit_near","lit_alternate","lit_alternate_clipped")','("mesh","primitive","clipped","near","alternate","alternate_clipped","lit","lit_clipped","lit_near","lit_alternate","lit_alternate_clipped","uv_lit","uv_lit_clipped")')
 ns=dict(previous.__dict__);exec(compile(source,'uv_lit_setup','exec'),ns)
 *_,allowed,execute=ns['local_setup']()
 manifest_path=PROJECT/'analysis/main_dispatch_uv_lit_primitives_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_lit_alternate_strips_verification.json'
 for item in (manifest,json.loads(baseline.read_text())):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 allowed|={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']};base.TRACKED|={UV_LIT,UV_LIT_CLIP}
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,SERVICES=SERVICES,effect=effect)
 return manifest_path,manifest,baseline,allowed,execute


def prepare(c,n):
 if c['entry'] in KINDS:
  m,s,f=lit_parent.prepare(dict(c,entry='lit' if c['entry']=='uv_lit' else 'lit_clipped'),n)
  segment=SEGMENT
 else:
  m,s,f=previous.previous.prepare(c,n);segment=mesh.SEGMENTS
  if c.get('uv_mesh'):
   if c['screen_outside']:
    store(m,0x8CD50000,current.bits(100. if c['reverse'] else -100.))
   offset=c.get('cursor_offset',0)
   store(m,services.HEADER+16,PACKETS+offset);store(m,services.HEADER+20,0x8CDE0000+offset)
 store(m,segment+20,UVS)
 pairs=c.get('uv_pairs',((0,0),(256,0),(0,256),(-256,512),(32767,-32768),(1,-1),(128,-128),(1024,2048)))
 for i in range(32):
  for j,v in enumerate(pairs[i%len(pairs)]):store(m,UVS+i*4+j*2,v&0xFFFF,2)
 return m,s,f


def cases():
 for c in lit_parent.cases():
  if c.get('lit_mesh'):continue
  entry={'lit':'uv_lit','lit_clipped':'uv_lit_clipped'}.get(c['entry'],c['entry'])
  yield dict(c,entry=entry)
 for entry,pairs,reverse,bank in itertools.product(KINDS,(((0,0),)*3,((32767,-32768),(-1,1),(127,-128)),((-256,512),(128,-64),(257,-513))),(False,True),range(2)):
  yield dict(entry=entry,uv_pairs=pairs,reverse=reverse,mode=bank<<21)
 for entry,reverse,bank in itertools.product(KINDS,(False,True),range(2)):
  yield dict(entry=entry,count=2,indices=(0,1,2,0,2,1),reverse=reverse,mode=bank<<21,cursor=PACKETS+32)
 for (mask,outside),reverse,bank,secondary,offset in itertools.product(((0,False),(0,True),(1,False)),(False,True),range(2),range(2),(0,32)):
  yield dict(entry='mesh',uv_mesh=True,connected_triangle=True,near_mask=mask,screen_outside=outside,reverse=reverse,mode=bank<<21,normals=True,clip=False,acquisition_mesh=True,material_extra=0x200000|(secondary<<20),cursor_offset=offset)


def signed16(v):return v-65536 if v&0x8000 else v

def independent(c,m,before,s,native,captured):
 if c['entry'] not in KINDS and not c.get('uv_mesh'):return False
 if c.get('mode',0)&0x180000:return False
 connected=c.get('uv_mesh',False)
 if not connected and c.get('topology',0)!=0:return False
 count=1 if connected else c.get('count',1)
 cache=m if connected else before
 start=((0x8CDE0000 if c['material_extra']&0x100000 else PACKETS)+c.get('cursor_offset',0)+32) if connected else c.get('cursor',PACKETS)
 records=[];expected_near=[]
 for triangle in range(count):
  ids=(0,1,2) if connected else tuple(load(before,INDICES+(triangle*3+i)*2,2) for i in range(3))
  xy=[tuple(current.value(load(cache,CACHE+i*32+16+j*4)) for j in range(2)) for i in ids]
  area=(xy[1][0]-xy[0][0])*(xy[2][1]-xy[0][1])-(xy[1][1]-xy[0][1])*(xy[2][0]-xy[0][0])
  flags=[load(cache,CACHE+i*32+12) for i in ids]
  clipped=c['entry']=='uv_lit_clipped' or connected and (c['near_mask'] or c['screen_outside'])
  reject=clipped and bool(flags[0]&flags[1]&flags[2]&63)
  near=clipped and not reject and bool((flags[0]|flags[1]|flags[2])&1)
  uv=tuple(load(before,UVS+triangle*12+i*2,2) for i in range(6))
  if near:expected_near.append(uv)
  elif not reject and area<=0:
   for local,index in enumerate(ids):records.append((index,local,uv[local*2:local*2+2]))
 assert len(captured)==2*len(expected_near),(c,'near calls',captured,expected_near)
 if expected_near:
  assert [event[-1] for event in captured]==expected_near*2
  assert all(event[0]==NEAR_UV and event[-2]==0x8C890304 for event in captured)
 emitted=len(records)
 if not connected:
  assert load(m,0x8C88F658)==(0xFFFFFFFF+3*count)&0xFFFFFFFF
  assert load(m,0x8C88F65C)==(0xFFFFFFFE+emitted)&0xFFFFFFFF
  assert all(load(m,CACHE+i*32+28)==load(before,CACHE+i*32+28) for i in range(8))
 assert native.flushes==emitted+(4 if connected else 0),(c,'transfers',native.flushes,emitted)
 assert s.r[8]==((start&0x03FFFFFF)|0xE0000000)+32*emitted
 retained=[SEED[7],SEED[15]]
 if connected:retained[((start-32)>>5)&1]=load(m,mesh.STATE+28)
 for i,(index,local,uv) in enumerate(records):
  destination=start+i*32
  factor=previous.previous.factor(cache,CACHE+index*32)
  packet=[0xF0000000 if local==2 else 0xE0000000]+[load(cache,CACHE+index*32+16+j*4) for j in range(3)]+[current.bits(signed16(v)/256.) for v in uv]+[current.bits(factor),retained[(destination>>5)&1]]
  assert [load(m,destination+j*4) for j in range(8)]==packet,(c,'independent UV/lit packet',i)
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
  if c['entry'] in KINDS:native.qacr[:]=ref.qacr[:]=[12,12]
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  captured=[];case=dict(c,capture=captured)
  path,status,b,ops,_,external=execute(case,m,s,f,c['entry'],0xFEED1234,allowed)
  if c.get('uv_mesh'):
   assert status==0 and path.count(UV_LIT)+path.count(UV_LIT_CLIP)==1
   assert path.count(UV_LIT_CLIP)==int(bool(c['near_mask'] or c['screen_outside']))
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
 if '--smoke' in sys.argv:selected=[dict(entry=e,topology=t,reverse=rev) for e,t,rev in itertools.product(KINDS,(0,0x4000,0x8000,0xC000),(False,True))]+[dict(entry='uv_lit_clipped',vertex_flags=(0,1,0,0,0,0,0,0))]+[c for c in selected if c.get('uv_mesh') and c['mode']==0 and c['material_extra']==0x200000 and c['cursor_offset']==0]

 for n,c in enumerate(selected):
  check(c,n)
  if (n+1)%100==0:print(n+1,'UV/lit primitive comparisons passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('UV/lit primitive smoke checks passed:',len(records));return
 for target,c in sorted(first.items()):
  check(dict(c,missing_target=target),len(records));check(dict(c,fail_target=target),len(records))
 assert rows==wanted,('missing rows',[hex(a) for a in sorted(wanted-rows)])
 want_edges={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert edges==want_edges,('missing edges',[[hex(a),hex(b)] for a,b in sorted(want_edges-edges)])
 assert calls=={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']}
 assert LOWER<=set(first),('lower services',LOWER-set(first))
 files=[Path(__file__),Path(previous.__file__),Path(lit_parent.__file__),Path(plain.__file__),Path(vertex.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_uv_lit_primitives.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_uv_lit_primitives_connected.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),**totals,service_targets=sorted(hex(t) for t in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Complete signed-UV/lit primitive family versus original CPU/FPU/RAM/call boundaries and queues; independent signed coordinate scaling, lighting, retained queue words, stream consumption and clipped-helper argument staging.',limitations=['Three lower UV/clipping/alternate helpers and twelve other primitive emitters remain explicit fixtures; these are not complete native rendering proof.','Valid bounded RAM/queue streams only, inherited numerical guards, no hardware timing/GPU FIFO/pixels/MMU-on/fault or full-game proof.'],cases=records)
 (PROJECT/'analysis/main_dispatch_uv_lit_primitives_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
