"""Native primitive bodies versus original instructions and packet invariants."""
import ctypes,gc,hashlib,inspect,itertools,json,sys
from pathlib import Path
import verify_main_dispatch_acquisition_variants as previous
import verify_main_dispatch_primitives as plain
import verify_main_dispatch_acquisition as acquisition
PROJECT=Path(__file__).resolve().parent
vertex=previous.vertex;services=acquisition.services;mesh=acquisition.mesh;particle=acquisition.particle;current=acquisition.current;base=acquisition.base
load,store=acquisition.load,acquisition.store
Bus,Memory,QueueReference,WORKERS=vertex.Bus,vertex.Memory,vertex.QueueReference,vertex.WORKERS
PLAIN,CLIPPED,NEAR,ALTERNATE,ALT_CLIP=previous.PLAIN,previous.CLIPPED,previous.NEAR,previous.ALTERNATE,previous.ALT_CLIP
SECONDARY,EXTENDED,EXT_SECONDARY=previous.SECONDARY,previous.EXTENDED,previous.EXT_SECONDARY
LIT,LIT_CLIP=0x8C617E46,0x8C6184F0
KINDS=("lit","lit_clipped")
LOWER={0x8C612E64,0x8C6136C0,0x8C614BB0}
SERVICES=(previous.SERVICES-{LIT,LIT_CLIP})|LOWER
SEGMENT,INDICES,CACHE,PACKETS=0x8CDD0000,0x8CDD1000,0x8CD40000,acquisition.PACKETS
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_lit_primitives.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(previous.SPECS,lit=(LIT,'sa_lit_primitive_emit_connected'),lit_clipped=(LIT_CLIP,'sa_lit_primitive_clipped_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE,ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(previous.LIB,name).argtypes;fn.restype=getattr(previous.LIB,name).restype
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
 # Extend only this process's established ABI check for the two R8 cursor outputs.
 alternate=previous.previous
 source=inspect.getsource(alternate.setup).replace('def setup():','def local_setup():',1)
 source=source.replace('("mesh","primitive","clipped","near","alternate","alternate_clipped")','("mesh","primitive","clipped","near","alternate","alternate_clipped","lit","lit_clipped")')
 ns=dict(alternate.__dict__);exec(compile(source,'lit_primitive_setup','exec'),ns)
 *_,allowed,execute=ns['local_setup']()
 manifest_path=PROJECT/'analysis/main_dispatch_lit_primitives_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_acquisition_variants_verification.json'
 acquisition_manifest=json.loads((PROJECT/'analysis/main_dispatch_acquisition_variants_translation.json').read_text())
 for item in (manifest,json.loads(baseline.read_text()),acquisition_manifest):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
  if 'routines' in item:allowed|={int(a,16) for r in item['routines'] for a in r['reachable_addresses']}
 base.TRACKED|={LIT,LIT_CLIP}|previous.ACQUISITIONS
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,SERVICES=SERVICES,effect=effect)
 return manifest_path,manifest,baseline,allowed,execute


def prepare(c,n):
 if c['entry'] in KINDS:
  original=dict(c,entry='primitive' if c['entry']=='lit' else 'clipped')
  m,s,f=plain.prepare(original,n)
  normals=c.get('normal_values',((0.25,0.5,0.75),(0.5,-0.25,0.125),(-0.5,0.25,0.5)))
  for i in range(8):
   for j,v in enumerate(normals[i%len(normals)]):store(m,CACHE+i*32+j*4,current.bits(v))
 else:m,s,f=previous.prepare(c,n)
 light=c.get('light',(0.25,0.5,0.125));ambient=c.get('ambient',1.)
 for i,v in enumerate(light):store(m,0x8C890248+i*4,current.bits(v))
 store(m,0x8C890258,current.bits(ambient))
 return m,s,f


def cases():
 for entry,topology,count,reverse,bank in itertools.product(KINDS,(0,0x4000,0x8000,0xC000),(0,1,2),(False,True),range(2)):
  yield dict(entry=entry,topology=topology,count=count,reverse=reverse,mode=bank<<21)
 for entry,strip_count,reverse,bank in itertools.product(KINDS,(0,1,2,3,4,5,6),(False,True),range(2)):
  yield dict(entry=entry,topology=0x8000,strip_count=strip_count,reverse=reverse,mode=bank<<21)
 for topology,flags,reverse,bank in itertools.product((0,0x4000,0x8000),((1,)*8,(2,)*8,(4,)*8,(0,1,0,0,0,0,0,0),(0,0,1,0,0,0,0,0),(1,0,0,0,0,0,0,0),(1,1,0,0,0,0,0,0),(1,1,1,0,0,0,0,0),(0,0,0,1,0,0,0,0)),(False,True),range(2)):
  yield dict(entry='lit_clipped',topology=topology,vertex_flags=flags,reverse=reverse,mode=bank<<21)
 for entry,topology,mode,rm in itertools.product(KINDS,(0,0x4000,0x8000,0xC000),range(8),range(4)):
  yield dict(entry=entry,topology=topology,mode=mode<<19,rm=rm)
 for entry,topology,bank in itertools.product(KINDS,(0,0x4000,0x8000),range(2)):
  yield dict(entry=entry,topology=topology,points=((0.,0.),)*8,mode=bank<<21,cursor=PACKETS+32)
 for bank in range(2):
  # Only the second half of a quad is entirely behind the near plane.
  yield dict(entry='lit_clipped',topology=0x4000,vertex_flags=(0,1,1,1,0,0,0,0),mode=bank<<21)
  # A mixed two-vertex strip reaches clipping dispatch but has no triangle.
  yield dict(entry='lit_clipped',topology=0x8000,strip_count=2,vertex_flags=(0,1,0,0,0,0,0,0),mode=bank<<21)
 for entry,variant,bank in itertools.product(('draw','primary'),range(2),range(2)):
  yield dict(entry=entry,graph='retail',variant=variant,mode=bank<<21)
 for entry,ambient,bank in itertools.product(KINDS,(-1.,0.,2.),range(2)):
  yield dict(entry=entry,ambient=ambient,reverse=True,mode=bank<<21)
 # Bit 200000 also changes the primitive layout; only primary/secondary
 # three-word acquisitions select this lighting family with otherwise zero flags.
 for (clip,mask),reverse,bank,extra,ambient in itertools.product(((False,0),(True,0),(True,1)),(False,True),range(2),(0,0x100000),(1.,-1.)):
  yield dict(entry='mesh',connected_triangle=True,lit_mesh=True,clip=clip,near_mask=mask,reverse=reverse,mode=bank<<21,layout=0,normals=True,acquisition_mesh=True,material_extra=extra,ambient=ambient)

def lighting_factor(m,index):
 # Independent scalar formula, rounded at every original binary32 operation.
 # Constructed inputs are binary-exact, including negative and >1 factors.
 rounded=lambda x:current.value(current.bits(x))
 factor=current.value(load(m,0x8C890258))
 for j in range(3):
  normal=current.value(load(m,CACHE+index*32+j*4));light=current.value(load(m,0x8C890248+j*4))
  factor=rounded(factor-rounded(normal*light))
 return factor

def lighting_words(m,index):
 factor=lighting_factor(m,index)
 return [load(m,mesh.STATE+16)]+[current.bits(current.value(load(m,mesh.STATE+16+j*4))*factor) for j in range(1,4)]

def independent(c,m,before,s,native,captured):
 if c.get('lit_mesh'):
  if c.get('near_mask'):return False # Lower near-clip service remains a fixture.
  emitted=3 if c['reverse'] else 0
  assert not captured and native.flushes==4+emitted,(c,native.flushes,captured)
  cursor=(0x8CDE0000 if c['material_extra']&0x100000 else PACKETS)
  for i in range(3):
   assert [load(m,CACHE+i*32+j*4) for j in range(3)]==[current.bits(v) for v in (0.25,0.5,0.75)]
  for i in range(emitted):
   expected=[0xF0000000 if i==2 else 0xE0000000]+[load(m,CACHE+i*32+16+j*4) for j in range(3)]+lighting_words(m,i)
   assert [load(m,cursor+32+i*32+j*4) for j in range(8)]==expected,(c,'connected lit packet',i)
   assert load(m,CACHE+i*32+28)==current.bits(lighting_factor(m,i))
  assert load(m,services.HEADER+(20 if c['material_extra']&0x100000 else 16))==cursor+32*(1+emitted)
  assert s.r[8]==((cursor&0x03FFFFFF)|0xE0000000)+32*(1+emitted)
  return True
 if c['entry'] not in KINDS or c.get('mode',0)&0x180000 or c.get('topology',0)!=0:return False
 count=c.get('count',1)
 xy=[tuple(current.value(load(before,CACHE+i*32+16+j*4)) for j in range(2)) for i in range(3)]
 area=(xy[1][0]-xy[0][0])*(xy[2][1]-xy[0][1])-(xy[1][1]-xy[0][1])*(xy[2][0]-xy[0][0])
 flags=[load(before,CACHE+i*32+12) for i in range(3)]
 rejected=c['entry']=='lit_clipped' and bool(flags[0]&flags[1]&flags[2]&63)
 near=c['entry']=='lit_clipped' and not rejected and bool((flags[0]|flags[1]|flags[2])&1)
 emitted=3*count if not rejected and not near and area<=0 else 0
 assert len(captured)==2*count*int(near)
 if near:assert all(target==0x8C612E64 for target,*_ in captured)
 assert load(m,0x8C88F658)==(0xFFFFFFFF+3*count)&0xFFFFFFFF
 assert load(m,0x8C88F65C)==(0xFFFFFFFE+emitted)&0xFFFFFFFF
 assert native.flushes==emitted
 start=(c.get('cursor',PACKETS)&0x03FFFFFF)|0xE0000000
 assert s.r[8]==start+32*emitted
 for i in range(emitted):
  index=i%3;destination=c.get('cursor',PACKETS)+i*32
  expected=[0xF0000000 if index==2 else 0xE0000000]+[load(before,CACHE+index*32+16+j*4) for j in range(3)]+lighting_words(before,index)
  assert [load(m,destination+j*4) for j in range(8)]==expected,(c,'independent emitted packet',i)
  assert load(m,CACHE+index*32+28)==current.bits(lighting_factor(before,index))
 if not emitted:
  assert all(load(m,CACHE+i*32+28)==load(before,CACHE+i*32+28) for i in range(3))
 return True

def main():
 manifest_path,manifest,baseline,allowed,execute=setup()
 wanted={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']};rows=set();edges=set();calls=set();records=[];counts={};first={}
 totals=dict(boundary_comparisons=0,numerical_operations=0,partial_cases=0,independent_cases=0,queue_flushes=0,queue_events=0,external_service_boundaries=0)
 def check(c,n):
  m,s,f=prepare(c,n);before=bytes(m);ref=QueueReference();native=Memory();bus=Bus()
  LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus))
  if c['entry'] in KINDS:native.qacr[:]=ref.qacr[:]=[12,12]
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  captured=[];case=dict(c,capture=captured)
  path,status,b,ops,_,external=execute(case,m,s,f,c['entry'],0xFEED1234,allowed)
  if c.get('lit_mesh'):
   assert status==0 and path.count(LIT)+path.count(LIT_CLIP)==1
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
 if '--smoke' in sys.argv:selected=[dict(entry=e,topology=t,reverse=rev) for e,t,rev in itertools.product(KINDS,(0,0x4000,0x8000,0xC000),(False,True))]+[dict(entry='lit_clipped',vertex_flags=(0,1,0,0,0,0,0,0)),dict(entry='draw',graph='retail')]+[c for c in selected if c.get('lit_mesh') and c['mode']==0 and c['material_extra']==0]
 for n,c in enumerate(selected):
  check(c,n)
  if (n+1)%100==0:print(n+1,'normal-lit primitive comparisons passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Normal-lit primitive smoke checks passed:',len(records));return
 for target,c in sorted(first.items()):
  check(dict(c,missing_target=target),len(records));check(dict(c,fail_target=target),len(records))
 assert rows==wanted,('missing rows',[hex(a) for a in sorted(wanted-rows)])
 want_edges={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert edges==want_edges,('missing edges',[[hex(a),hex(b)] for a,b in sorted(want_edges-edges)])
 assert calls=={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']}
 assert LOWER<=set(first),('lower services',LOWER-set(first))
 files=[Path(__file__),Path(previous.__file__),Path(plain.__file__),Path(vertex.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_lit_primitives.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_lit_primitives_connected.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),**totals,service_targets=sorted(hex(t) for t in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Complete normal-lit primitive family versus original CPU/FPU/RAM/call boundaries and queue events; topology/fallback, clipping flags, lighting factors and independent packets/counters/cursors.',limitations=['Three lower lighting/clipping/alternate helpers and fourteen other primitive emitters remain explicit fixtures; these are not complete native rendering proof.','Valid bounded RAM/queue streams only, inherited numerical guards, no hardware timing/GPU FIFO/pixels/MMU-on/fault or full-game proof.'],cases=records)
 (PROJECT/'analysis/main_dispatch_lit_primitives_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
