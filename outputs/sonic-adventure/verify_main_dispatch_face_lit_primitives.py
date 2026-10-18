"""Normal-derived environment UV family versus original code and packet math."""
import ctypes,gc,hashlib,inspect,itertools,json,math,sys
from pathlib import Path
import verify_main_dispatch_environment_alternate_strips as previous
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
FACE,FACE_CLIP,NORMALIZE=0x8C61AE3C,0x8C61B488,0x8C63A94C
FACE_NEAR,FACE_ALT,FACE_ALT_CLIP=0x8C6178C6,0x8C613DFA,0x8C61572C
KINDS=("face_lit","face_lit_clipped")
LOWER={FACE_NEAR,FACE_ALT,FACE_ALT_CLIP}
SERVICES=(previous.SERVICES-{FACE,FACE_CLIP})|LOWER
SEGMENT,INDICES,CACHE,PACKETS=lit_parent.SEGMENT,lit_parent.INDICES,previous.CACHE,previous.PACKETS
SEED=previous.SEED;VECTOR=0x8CDD8000
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_face_lit_primitives.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(previous.SPECS,face_lit=(FACE,'sa_face_lit_primitive_emit_connected'),face_lit_clipped=(FACE_CLIP,'sa_face_lit_primitive_clipped_connected'),normalize=(NORMALIZE,'sa_face_vector_normalize_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE,ctypes.POINTER(Bus)];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac','sa_sh4_divide','sa_vertex_bus_init'):
 fn=getattr(LIB,name);fn.argtypes=getattr(previous.LIB,name).argtypes;fn.restype=getattr(previous.LIB,name).restype

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def effect(c,target,mem,r,sr,fp):
 if target not in LOWER:return mesh.effect(c,target,mem,r,sr,fp)
 if c.get('capture') is not None:
  colour=tuple(load(mem,r[7]+j*4) for j in range(4)) if target==FACE_NEAR else ()
  c['capture'].append((target,r[4],r[5],r[6],r[7],colour))
 for i in range(8):r[i]=0xBAD00000+i*0x101
 bank=(fp.fpscr>>21)&1
 for i in range(6):fp.banks[bank][i]=current.bits(0.125*(i+1))
 r[8]=(r[8]+c.get('fixture_advance',0))&0xFFFFFFFF
 return sr^1

def setup():
 parent=previous.previous
 source=inspect.getsource(parent.setup).replace('def setup():','def local_setup():',1)
 source=source.replace('"environment_near")','"environment_near","environment_alternate","environment_alternate_clipped","face_lit","face_lit_clipped")')
 guard=" changes['  assert pc in allowed,hex(pc);path.append(pc)']+=chr(10)+'  if pc==0x8c61bca2 and (r[14]&0x1f000000)!=0x0c000000:stop(pc,r,sr,pr,-3)'\n"
 guard+=" changes[' def trace(pc,w,r,sr,pr,_f):']=changes[' def trace(pc,w,r,sr,pr,_f):'].replace('filter_returns=[]','filter_returns=[];face_returns=[]')\n"
 guard+=" changes['  assert pc in allowed,hex(pc);path.append(pc)']+=chr(10)+'  if pc in (0x8c61ae3c,0x8c61b488):face_returns.append(pr)'+chr(10)+'  if pc in (0x8c61b440,0x8c61bdfe):'+chr(10)+'   if pr!=face_returns[-1]:stop(pc,r,sr,pr,-3)'+chr(10)+'   face_returns.pop()'\n"
 # The environment setup passes guard text into its cloned lighting setup.
 marker=next(line for line in source.splitlines() if line.startswith(' marker='))
 source=source.replace(marker,' guard+='+repr(guard)+'\n'+marker,1)
 ns=dict(parent.__dict__);exec(compile(source,'face_lit_setup','exec'),ns)
 *_,allowed,execute=ns['local_setup']()
 manifest_path=PROJECT/'analysis/main_dispatch_face_lit_primitives_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_environment_alternate_strips_verification.json'
 inherited=json.loads((PROJECT/'analysis/main_dispatch_environment_alternate_strips_translation.json').read_text())
 for item in (manifest,inherited,json.loads(baseline.read_text())):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
  if 'routines' in item:allowed|={int(a,16) for r in item['routines'] for a in r['reachable_addresses']}
 base.TRACKED|={ENV_ALT,ENV_ALT_CLIP,FACE,FACE_CLIP,NORMALIZE}
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,SERVICES=SERVICES,effect=effect)
 return manifest_path,manifest,baseline,allowed,execute

def prepare(c,n):
 if c['entry'] in KINDS:
  m,s,f=lit_parent.prepare(dict(c,entry='lit' if c['entry']=='face_lit' else 'lit_clipped'),n)
  for a in (0x8C890260,0x8C890264):store(m,a,current.bits(1.))
 else:
  m,s,f=previous.previous.prepare(c,n)
  if c.get('face_mesh'):
   for i,normal in enumerate(c.get('normal_values',((0.25,0.5,0.75),(0.5,-0.25,0.125),(-0.5,0.25,0.5),(0.25,0.5,0.75)))):
    for j,v in enumerate(normal):store(m,0x8CD60000+i*12+j*4,current.bits(v))
   offset=c.get('cursor_offset',0)
   store(m,services.HEADER+16,PACKETS+offset);store(m,services.HEADER+20,0x8CDE0000+offset)
  if c['entry']=='normalize':
   for i,v in enumerate(c.get('vector',(3.,4.,0.))):store(m,VECTOR+i*4,current.bits(v))
   s.r[4]=VECTOR
 if c.get('seed_strip_registers'):
  s.r[12]=CACHE;s.r[14]=CACHE+32;s.r[13]=CACHE+64
 return m,s,f

def cases():
 for c in lit_parent.cases():
  if c.get('lit_mesh'):continue
  yield dict(c,entry={'lit':'face_lit','lit_clipped':'face_lit_clipped'}.get(c['entry'],c['entry']))
 for entry,normal,reverse,bank in itertools.product(KINDS,(((1.,0.,0.),)*3,((0.,1.,0.),)*3,((0.,0.,1.),)*3,((1.,0.,0.),(-1.,0.,0.),(0.,1.,0.))),(False,True),range(2)):
  yield dict(entry=entry,normal_values=normal,reverse=reverse,mode=bank<<21)
 for entry,reverse,bank in itertools.product(KINDS,(False,True),range(2)):
  yield dict(entry=entry,count=2,indices=(0,1,2,0,2,1),reverse=reverse,mode=bank<<21,cursor=PACKETS+32)
 for mask,reverse,bank,secondary,offset in itertools.product((0,1),(False,True),range(2),range(2),(0,32)):
  yield dict(entry='mesh',face_mesh=True,connected_triangle=True,near_mask=mask,reverse=reverse,mode=bank<<21,normals=True,clip=False,acquisition_mesh=True,material_extra=0x1000000|(secondary<<20),cursor_offset=offset)
 # The original crossing-strip branch uses live R12/R14/R13 before its
 # ordinary-path initialization. Supply explicit valid pointers to cover it;
 # inherited arbitrary-register cases stop at the unsupported-pointer boundary.
 for mask,reverse,bank in itertools.product((1,2,3,4,5,6),(False,True),range(2)):
  yield dict(entry='face_lit_clipped',topology=0x8000,strip_count=3,vertex_flags=tuple(int(mask&(1<<i)!=0) for i in range(3))+(0,)*5,reverse=reverse,mode=bank<<21,seed_strip_registers=True)
 for length,flags,reverse,bank in itertools.product((2,4,5),((0,1,1,1,0,0,0,0),(1,0,0,0,0,0,0,0)),(False,True),range(2)):
  yield dict(entry='face_lit_clipped',topology=0x8000,strip_count=length,vertex_flags=flags,reverse=reverse,mode=bank<<21,seed_strip_registers=True)
 for vector,mode,rm in itertools.product(((3.,4.,0.),(-3.,4.,0.),(1.,0.,0.),(0.,-1.,0.),(0.,0.,0.),(1/1024.,2/1024.,-2/1024.)),range(8),range(4)):
  yield dict(entry='normalize',vector=vector,mode=mode<<19,rm=rm)

def face_colour(m,ids):
 rounded=lambda x:current.value(current.bits(x))
 normal=[rounded(rounded(current.value(load(m,CACHE+ids[0]*32+j*4))+current.value(load(m,CACHE+ids[1]*32+j*4)))+current.value(load(m,CACHE+ids[2]*32+j*4))) for j in range(3)]
 length=math.sqrt(sum(v*v for v in normal))
 if not length:return None
 unit=[v/length for v in normal];factor=current.value(load(m,0x8C890258))-sum(unit[j]*current.value(load(m,0x8C890248+j*4)) for j in range(3))
 return [load(m,mesh.STATE+16)]+[current.value(load(m,mesh.STATE+16+j*4))*factor for j in range(1,4)]

def colour_matches(actual,want,c):
 assert actual[0]==want[0],(c,'alpha')
 assert all(math.isclose(current.value(a),b,rel_tol=2e-5,abs_tol=2e-5) for a,b in zip(actual[1:],want[1:])),(c,'face RGB',list(map(current.value,actual[1:])),want[1:])

def independent(c,m,before,s,native,captured):
 if c['entry']=='normalize':
  if c.get('mode',0)&0x180000 or c.get('rm',0)!=0:return False
  vector=c['vector'];length=math.sqrt(sum(v*v for v in vector))
  if not length:return False
  got=[current.value(load(m,VECTOR+j*4)) for j in range(3)]
  assert all(math.isclose(a,b/length,rel_tol=2e-5,abs_tol=2e-5) for a,b in zip(got,vector)),(c,'unit vector',got)
  assert math.isclose(math.sqrt(sum(v*v for v in got)),1.,rel_tol=2e-5) and s.r[4]==VECTOR and native.flushes==0
  return True
 connected=c.get('face_mesh',False)
 if c['entry'] not in KINDS and not connected:return False
 if c.get('mode',0)&0x180000 or not connected and c.get('topology',0)!=0:return False
 count=1 if connected else c.get('count',1);cache=m if connected else before
 start=((0x8CDE0000 if c['material_extra']&0x100000 else PACKETS)+c.get('cursor_offset',0)+32) if connected else c.get('cursor',PACKETS)
 records=[];near_calls=[]
 for triangle in range(count):
  ids=(0,1,2) if connected else tuple(load(before,INDICES+(triangle*3+i)*2,2) for i in range(3))
  xy=[tuple(current.value(load(cache,CACHE+i*32+16+j*4)) for j in range(2)) for i in ids]
  area=(xy[1][0]-xy[0][0])*(xy[2][1]-xy[0][1])-(xy[1][1]-xy[0][1])*(xy[2][0]-xy[0][0])
  flags=[load(cache,CACHE+i*32+12) for i in ids]
  clipped=c['entry']=='face_lit_clipped' or connected and c['near_mask']
  rejected=clipped and bool(flags[0]&flags[1]&flags[2]&63)
  near=clipped and not rejected and bool((flags[0]|flags[1]|flags[2])&1)
  colour=face_colour(cache,ids)
  if colour is None:return False
  if near:near_calls.append((tuple(CACHE+i*32 for i in ids),colour))
  elif not rejected and area<=0:
   for local,index in enumerate(ids):records.append((index,local,colour))
 assert len(captured)==2*len(near_calls),(c,captured,near_calls)
 if near_calls:
  for event,(ids,colour) in zip(captured,near_calls*2):
   assert event[0]==FACE_NEAR and event[1:4]==ids
   colour_matches(event[-1],colour,c)
 emitted=len(records)
 if not connected:
  assert load(m,0x8C88F658)==(0xFFFFFFFF+3*count)&0xFFFFFFFF
  assert load(m,0x8C88F65C)==(0xFFFFFFFE+emitted)&0xFFFFFFFF
  assert all(load(m,CACHE+i*32+28)==load(before,CACHE+i*32+28) for i in range(8))
 assert native.flushes==emitted+(4 if connected else 0),(c,'transfers',native.flushes,emitted)
 assert s.r[8]==((start&0x03FFFFFF)|0xE0000000)+32*emitted
 retained=[SEED[4:8],SEED[12:16]]
 for i,(index,local,colour) in enumerate(records):
  destination=start+i*32
  assert [load(m,destination+j*4) for j in range(4)]==[0xF0000000 if local==2 else 0xE0000000]+[load(cache,CACHE+index*32+16+j*4) for j in range(3)]
  actual=[load(m,destination+16+j*4) for j in range(4)];q=(destination>>5)&1
  if local==2:
   colour_matches(actual,colour,c);retained[q]=actual
  elif not connected:assert actual==retained[q],(c,'retained non-provoking colour',i)
 if connected:assert load(m,services.HEADER+(20 if c['material_extra']&0x100000 else 16))==start+32*emitted
 return True

def main():
 manifest_path,manifest,baseline,allowed,execute=setup()
 wanted={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']};rows=set();edges=set();calls=set();records=[];counts={};first={}
 totals=dict(normalizer_invocations=0,boundary_comparisons=0,numerical_operations=0,partial_cases=0,independent_cases=0,queue_flushes=0,queue_events=0,external_service_boundaries=0)
 def check(c,n):
  m,s,f=prepare(c,n);before=bytes(m);ref=QueueReference();native=Memory();bus=Bus()
  LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus))
  for i in range(16):native.queue[i]=ref.queue[i]=SEED[i]
  if c['entry'] in KINDS or c['entry']=='normalize':native.qacr[:]=ref.qacr[:]=[12,12]
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  captured=[];case=dict(c,capture=captured)
  path,status,b,ops,_,external=execute(case,m,s,f,c['entry'],0xFEED1234,allowed)
  if c.get('face_mesh'):
   assert status==0 and path.count(FACE)+path.count(FACE_CLIP)==1
   assert path.count(FACE_CLIP)==int(bool(c['near_mask']))
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
  totals['normalizer_invocations']+=path.count(NORMALIZE)
  totals['boundary_comparisons']+=b;totals['numerical_operations']+=ops;totals['partial_cases']+=status!=0
  totals['queue_flushes']+=native.flushes;totals['queue_events']+=native.trace_count+native.trace_dropped;totals['external_service_boundaries']+=external
  if not status:
   totals['independent_cases']+=independent(c,m,before,s,native,captured)
   for target,*_ in captured:first.setdefault(target,dict(c))
  counts[c['entry']]=counts.get(c['entry'],0)+1
  records.append(dict(input=c,steps=len(path),status=status,full_state_match=True,queue_match=True))
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,topology=t,reverse=rev) for e,t,rev in itertools.product(KINDS,(0,0x4000,0x8000,0xC000),(False,True))]+[dict(entry='face_lit_clipped',vertex_flags=(0,1,0,0,0,0,0,0))]+[c for c in selected if c.get('face_mesh') and c['mode']==0 and c['material_extra']==0x1000000 and c['cursor_offset']==0]

 for n,c in enumerate(selected):
  check(c,n)
  if (n+1)%100==0:print(n+1,'Face-lit primitive comparisons passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Face-lit primitive smoke checks passed:',len(records));return
 for target,c in sorted(first.items()):
  check(dict(c,missing_target=target),len(records));check(dict(c,fail_target=target),len(records))
 assert rows==wanted,('missing rows',[hex(a) for a in sorted(wanted-rows)])
 want_edges={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert edges==want_edges,('missing edges',[[hex(a),hex(b)] for a,b in sorted(want_edges-edges)])
 assert calls=={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']}
 assert LOWER<=set(first),('lower services',LOWER-set(first))
 files=[Path(__file__),Path(previous.__file__),Path(lit_parent.__file__),Path(uv_parent.__file__),Path(vertex.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_face_lit_primitives.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_face_lit_primitives_connected.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),**totals,service_targets=sorted(hex(t) for t in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Complete face-lit primitive pair plus native vector normalization versus original full state/RAM/queues; independent unit vectors, normalized summed-normal lighting, provoking-vertex face RGB and retained earlier colours, unchanged alpha/cache words, counters/cursors and lower clipper argument/colour staging.',limitations=['Three lower face-lit clipping/alternate helpers and eight other primitive emitters remain fixtures. Normalizer FIPR/FSRRA use the pinned numerical backend; independent finite-vector checks have tolerance, not arbitrary hardware numerical proof.','Valid bounded RAM/queue streams only, inherited numerical guards, no hardware timing/GPU FIFO/pixels/MMU-on/fault or full-game proof.'],cases=records)
 (PROJECT/'analysis/main_dispatch_face_lit_primitives_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
