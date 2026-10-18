"""Native alternate strips against original code and independent triangle sets."""
import ctypes,gc,hashlib,inspect,itertools,json,math,sys
from pathlib import Path
import verify_main_dispatch_uv_constant_near_clip as previous
import verify_main_dispatch_uv_constant_primitives as face_parent
import verify_main_dispatch_lit_alternate_strips as lit_alternate
import verify_main_dispatch_alternate_strips as flat_alternate
PROJECT=Path(__file__).resolve().parent
vertex=previous.vertex;services=previous.services;mesh=previous.mesh;particle=previous.particle;current=previous.current;base=previous.base
load,store=previous.load,previous.store
Bus,Memory,QueueReference,WORKERS=previous.Bus,previous.Memory,previous.QueueReference,previous.WORKERS
PLAIN,CLIPPED,NEAR=previous.PLAIN,previous.CLIPPED,previous.NEAR
ALTERNATE,ALT_CLIP,STUB=previous.ALTERNATE,previous.ALT_CLIP,0x8C611708
LIT,LIT_CLIP,LIT_NEAR=previous.LIT,previous.LIT_CLIP,previous.LIT_NEAR
SECONDARY,EXTENDED,EXT_SECONDARY=previous.SECONDARY,previous.EXTENDED,previous.EXT_SECONDARY
LIT_ALT,LIT_ALT_CLIP=previous.LIT_ALT,previous.LIT_ALT_CLIP
UV_LIT,UV_LIT_CLIP,UV_NEAR=previous.UV_LIT,previous.UV_LIT_CLIP,previous.UV_NEAR
UV_ALT,UV_ALT_CLIP=previous.UV_ALT,previous.UV_ALT_CLIP
ENV,ENV_CLIP,ENV_NEAR=previous.ENV,previous.ENV_CLIP,previous.ENV_NEAR
ENV_ALT,ENV_ALT_CLIP=previous.ENV_ALT,previous.ENV_ALT_CLIP
FACE,FACE_CLIP,FACE_NEAR=previous.FACE,previous.FACE_CLIP,previous.FACE_NEAR
FACE_ALT,FACE_ALT_CLIP,NORMALIZE=previous.FACE_ALT,previous.FACE_ALT_CLIP,previous.NORMALIZE
UV_FACE,UV_FACE_CLIP,UV_FACE_NEAR=previous.UV_FACE,previous.UV_FACE_CLIP,previous.UV_FACE_NEAR
UV_FACE_ALT,UV_FACE_ALT_CLIP=previous.UV_FACE_ALT,previous.UV_FACE_ALT_CLIP
UV_CONSTANT,UV_CONSTANT_CLIP,UV_CONSTANT_NEAR=previous.UV_CONSTANT,previous.UV_CONSTANT_CLIP,previous.UV_CONSTANT_NEAR
UV_CONSTANT_ALT,UV_CONSTANT_ALT_CLIP=0x8C61430E,0x8C616068
UVS=0x8CDF0000
SEED=previous.SEED
KINDS=("uv_constant_alternate","uv_constant_alternate_clipped")
SERVICES=previous.SERVICES-{UV_CONSTANT_ALT,UV_CONSTANT_ALT_CLIP}
SEGMENT,INDICES,CACHE,PACKETS=flat_alternate.SEGMENT,flat_alternate.INDICES,previous.CACHE,previous.PACKETS
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_uv_constant_alternate_strips.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(previous.SPECS,uv_constant_alternate=(UV_CONSTANT_ALT,'sa_uv_constant_alternate_strip_connected'),uv_constant_alternate_clipped=(UV_CONSTANT_ALT_CLIP,'sa_uv_constant_alternate_strip_clipped_connected'))
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
 marker='"uv_constant_near")';assert source.count(marker)==1
 source=source.replace(marker,'"uv_constant_near","uv_constant_alternate","uv_constant_alternate_clipped")')
 ns=dict(previous.__dict__);exec(compile(source,'uv_alternate_setup','exec'),ns)
 *_,allowed,execute=ns['local_setup']()
 manifest_path=PROJECT/'analysis/main_dispatch_uv_constant_alternate_strips_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_uv_constant_near_clip_verification.json'
 for item in (manifest,json.loads(baseline.read_text())):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 allowed|={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']};base.TRACKED|={UV_CONSTANT_ALT,UV_CONSTANT_ALT_CLIP}
 execute.__globals__.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,SERVICES=SERVICES)
 return manifest_path,manifest,baseline,allowed,execute


def prepare(c,n):
 if c.get('connected_alternate'):
  m,s,f=previous.prepare(c,n)
  for i,normal in enumerate(((0.25,0.5,0.75),(0.5,-0.25,0.125),(-0.5,0.25,0.5),(0.25,0.5,0.75))):
   for j,v in enumerate(normal):store(m,0x8CD60000+i*12+j*4,current.bits(v))
  segment=mesh.SEGMENTS
 else:
  old_entry={'uv_constant_alternate':'alternate','uv_constant_alternate_clipped':'alternate_clipped','uv_constant':'primitive','uv_constant_clipped':'clipped'}.get(c['entry'],c['entry'])
  m,s,f=flat_alternate.prepare(dict(c,entry=old_entry),n);segment=SEGMENT
  normals=c.get('normal_values',((0.25,0.5,0.75),(0.5,-0.25,0.125),(-0.5,0.25,0.5)))
  for i in range(8):
   for j,v in enumerate(normals[i%len(normals)]):store(m,CACHE+i*32+j*4,current.bits(v))
 for i,v in enumerate(c.get('light',(0.25,0.5,0.125))):store(m,0x8C890248+i*4,current.bits(v))
 store(m,0x8C890258,current.bits(c.get('ambient',1.)));store(m,segment+20,UVS)
 pairs=c.get('uv_pairs',((0,0),(256,0),(0,256),(-256,512),(32767,-32768),(-1,1),(128,-128),(1024,2048)))
 for i in range(32):
  for j,v in enumerate(pairs[i%len(pairs)]):store(m,UVS+i*4+j*2,v&0xFFFF,2)
 if c['entry'] in KINDS:
  bank=((f.fpscr>>21)&1)^1
  f.banks[bank][0]=current.bits(1/256.)
  f.banks[bank][1]=c.get('unused_word',0x7FC12345)
 return m,s,f


def independent(c,m,before,s,native):
 if c.get('connected_alternate'):
  points=[((-x if c['reverse'] else x),y,0.5 if c['near_mask']&(1<<i) else 4.) for i,(x,y) in enumerate(((0.,0.),(4.,0.),(0.,4.)))]
  pairs=tuple(tuple(face_parent.signed16(load(before,UVS+i*4+j*2,2)) for j in range(2)) for i in range(3))
  accepted=c['reverse']!=c['strip_flip']
  polygon=previous.uv_clip.polygon(dict(c,cull=0),points,[0.,0.,0.],pairs)[0] if accepted else []
  emitted=len(polygon);start=(0x8CDE0000 if c['material_extra']&0x100000 else PACKETS)+c.get('cursor_offset',0)
  assert native.flushes==4+emitted and load(m,0x8C88F658)==2
  delta=3+emitted if c['near_mask'] else emitted+int(emitted!=0)
  assert load(m,0x8C88F65C)==(0xFFFFFFFE+delta)&0xFFFFFFFF
  if c['near_mask']:
   for i,point in enumerate(polygon):assert [load(m,previous.OUTPUT_UV+i*4+j*2,2) for j in range(2)]==[v&0xFFFF for v in point[4:]]
  if emitted==4:polygon=[polygon[i] for i in (0,1,3,2)]
  padding=[SEED[7],SEED[15]]
  padding[(start>>5)&1]=load(m,mesh.STATE+28)
  for i,(x,y,z,_,u,v) in enumerate(polygon):
   a=start+(i+1)*32;q=(a>>5)&1
   assert load(m,a)==(0xF0000000 if i==emitted-1 else 0xE0000000)
   got=[current.value(load(m,a+4+j*4)) for j in range(3)];want=(x/z,y/z,1/z)
   assert all(math.isclose(a,b,rel_tol=1e-5,abs_tol=1e-4) for a,b in zip(got,want)),(c,'strip geometry',i,got,want)
   assert [load(m,a+16+j*4) for j in range(2)]==[current.bits(value/256.) for value in (u,v)],(c,'strip quantized UV',i)
   assert load(m,a+24)==current.bits(1.),(c,'constant factor',i)
   assert load(m,a+28)==padding[q]
  assert not any(e.kind==6 for e in native.trace[:native.trace_count])
  cursor=start+32*(1+emitted)
  assert s.r[8]==(cursor&0x03FFFFFF)|0xE0000000 and load(m,services.HEADER+(20 if c['material_extra']&0x100000 else 16))==cursor
  return True
 if c['entry'] not in KINDS or c.get('mode',0)&0x180000 or any(v&1 for v in c.get('vertex_flags',())):return False
 lengths=c.get('lengths',(4,));flip=c.get('strip_flip',False)
 def point(index,uv_index):
  uv=[previous.previous.signed16(load(before,UVS+uv_index*4+j*2,2)) for j in range(2)]
  return tuple(load(before,CACHE+index*32+16+j*4) for j in range(3))+tuple(current.bits(v/256.) for v in uv)+(current.bits(1.),)
 expected=[];offset=0
 for length in lengths:
  for i in range(length-2):
   a,b,d=(tuple(current.value(load(before,CACHE+j*32+16+k*4)) for k in range(2)) for j in (i,i+1,i+2))
   area=(b[0]-a[0])*(d[1]-a[1])-(b[1]-a[1])*(d[0]-a[0])
   flags=[load(before,CACHE+j*32+12) for j in (i,i+1,i+2)]
   outside=c['entry']=='uv_constant_alternate_clipped' and bool(flags[0]&flags[1]&flags[2]&63)
   if not outside and area*(-1 if flip^(i%2!=0) else 1)<0:expected.append(tuple(sorted(point(j,offset+j) for j in (i,i+1,i+2))))
  offset+=length
 actual=[];group=[];groups=0
 for i in range(native.flushes):
  a=c.get('cursor',PACKETS)+i*32;control=load(m,a)
  assert control in (0xE0000000,0xF0000000)
  assert load(m,a+28)==SEED[((a>>5)&1)*8+7]
  group.append(tuple(load(m,a+4+j*4) for j in range(6)))
  if control==0xF0000000:
   groups+=1;assert len(group)>=3
   actual.extend(tuple(sorted(group[j:j+3])) for j in range(len(group)-2));group=[]
 assert not group and actual==expected,(c,'independent UV/lighting triangles',actual,expected)
 assert s.r[8]==((c.get('cursor',PACKETS)&0x03FFFFFF)|0xE0000000)+32*native.flushes
 assert load(m,0x8C88F658)==(0xFFFFFFFF+sum(lengths))&0xFFFFFFFF
 delta=native.flushes+groups if c['entry']=='uv_constant_alternate' else sum(lengths)
 assert load(m,0x8C88F65C)==(0xFFFFFFFE+delta)&0xFFFFFFFF
 assert m[CACHE&0xFFFFFF:(CACHE&0xFFFFFF)+256]==before[CACHE&0xFFFFFF:(CACHE&0xFFFFFF)+256],(c,'unchanged input cache')
 assert m[UVS&0xFFFFFF:(UVS&0xFFFFFF)+128]==before[UVS&0xFFFFFF:(UVS&0xFFFFFF)+128],(c,'unchanged source UVs')
 assert not any(e.kind==6 for e in native.trace[:native.trace_count])
 return True


def cases():
 for c in flat_alternate.cases():
  entry={'alternate':'uv_constant_alternate','alternate_clipped':'uv_constant_alternate_clipped','primitive':'uv_constant','clipped':'uv_constant_clipped'}.get(c['entry'],c['entry'])
  if c.get('connected_alternate'):
   for extra,offset in itertools.product((0x2200000,0x2300000,0x3200000,0x3300000),(0,32)):
    yield dict(c,entry=entry,layout=0,normals=True,clip=False,uv_constant_mesh=True,acquisition_mesh=True,material_extra=extra,cursor_offset=offset)
  else:yield dict(c,entry=entry)
 for entry,ambient,reverse,bank in itertools.product(KINDS,(-1.,0.,2.),(False,True),range(2)):
  yield dict(entry=entry,lengths=(4,),ambient=ambient,reverse=reverse,mode=bank<<21)
 for entry,pairs,reverse,bank in itertools.product(KINDS,(((0,0),)*3,((32767,-32768),(-32768,32767),(-1,1)),((-256,512),(128,-64),(257,-513))),(False,True),range(2)):
  yield dict(entry=entry,lengths=(3,4),uv_pairs=pairs,reverse=reverse,mode=bank<<21,cursor=PACKETS+32)

 for reverse,flip,bank,extra,offset in itertools.product((False,True),(False,True),range(2),(0x2200000,0x2300000,0x3200000,0x3300000),(0,32)):
  yield dict(entry='mesh',connected_triangle=True,connected_alternate=True,near_mask=0,reverse=reverse,strip_flip=flip,mode=bank<<21,layout=0,normals=True,clip=False,uv_constant_mesh=True,acquisition_mesh=True,material_extra=extra,cursor_offset=offset)


def main():
 manifest_path,manifest,baseline,allowed,execute=setup();want={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 rows=set();edges=set();calls=set();records=[];counts={};totals=dict(boundary_comparisons=0,numerical_operations=0,partial_cases=0,independent_cases=0,independent_mesh_cases=0,queue_flushes=0,queue_events=0,writeback_events=0,fipr_instructions=0,helper_fipr_instructions=0,external_service_boundaries=0,stub_invocations=0,near_invocations=0,normalizer_invocations=0)
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,lengths=(4,),reverse=rev,strip_flip=flip) for e,rev,flip in itertools.product(KINDS,(False,True),(False,True))]+[dict(entry='uv_constant_alternate_clipped',lengths=(4,),vertex_flags=(0,1,0,0,0,0,0,0),reverse=True),dict(entry='uv_constant_clipped',alternate_parent=True,topology=0xC000,reverse=True)]
 for n,c in enumerate(selected):
  m,s,f=prepare(c,n);before=bytes(m);ref=QueueReference();native=Memory();bus=Bus()
  LIB.sa_vertex_bus_init(ctypes.byref(native),(ctypes.c_uint8*len(m)).from_buffer(m),ctypes.byref(bus));native.qacr[:]=ref.qacr[:]=[12,12]
  for i in range(16):native.queue[i]=ref.queue[i]=SEED[i]
  execute.__globals__.update(queue_reference=ref,bus_pointer=ctypes.byref(bus))
  path,status,b,ops,_,external=execute(c,m,s,f,c['entry'],0xFEED1234,allowed)
  if c.get('connected_alternate'):
   assert status==0 and external==0 and path.count(STUB)==1
   assert path.count(UV_CONSTANT_ALT_CLIP)==path.count(UV_CONSTANT_CLIP)==int(bool(c['near_mask']))
   assert path.count(UV_CONSTANT_ALT)==path.count(UV_CONSTANT)==int(not c['near_mask'])
   assert path.count(UV_CONSTANT_NEAR)==int(bool(c['near_mask']) and c['reverse']!=c['strip_flip'])
   assert path.count(NORMALIZE)==0
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
  totals['external_service_boundaries']+=external;totals['stub_invocations']+=path.count(STUB);totals['near_invocations']+=path.count(UV_CONSTANT_NEAR);totals['normalizer_invocations']+=path.count(NORMALIZE)
  totals['writeback_events']+=sum(e.kind==6 for e in native.trace[:native.trace_count])
  totals['fipr_instructions']+=sum(int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')&0xF0FF==0xF0ED for a in path)
  totals['helper_fipr_instructions']+=sum(a in want and int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')&0xF0FF==0xF0ED for a in path)
  if not status:
   checked=independent(c,m,before,s,native);totals['independent_cases']+=checked;totals['independent_mesh_cases']+=checked and c.get('connected_alternate',False)
  counts[c['entry']]=counts.get(c['entry'],0)+1
  records.append(dict(input=c,steps=len(path),status=status,full_state_match=True,queue_match=True))
  if (n+1)%100==0:print(n+1,'native UV-constant alternate-strip cases passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('UV-constant alternate-strip smoke checks passed:',len(records));return
 assert rows==want,('missing rows',[hex(a) for a in sorted(want-rows)])
 assert edges=={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']},('edges',len(edges))
 assert calls=={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']}
 assert totals['external_service_boundaries']==0
 assert totals['helper_fipr_instructions']==totals['normalizer_invocations']==0
 files=[Path(__file__),Path(previous.__file__),Path(lit_alternate.__file__),Path(flat_alternate.__file__),Path(face_parent.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_uv_constant_alternate_strips.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_uv_constant_alternate_strips_connected.c',PROJECT/'source/retail_empty_stubs.c',PROJECT/'analysis/retail_empty_stubs_verification.json',PROJECT/'source/vertex_bus_lighting.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),**totals,service_targets=sorted(hex(t) for t in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Complete UV-constant alternate strips with native stub and constant-factor UV clipper versus original full state/RAM/events; independent signed UV triangle sets, quantized crossing UVs, exact factor 1.0 on every emitted vertex, padding/input preservation, grouping/counters/cursors and native parent integration.',limitations=['Constructed bounded metadata uses positive strip counts and lengths >=3, plus original lookahead indices. Invalid count loops/pointers and physical hardware exception dispatch remain unmodeled.','Inherited numerical and saved-pointer guards, coherent-RAM/MMU-off queue scope; no GPU FIFO/pixels/MMU-on/cache-timing/full-game proof. Four other primitive emitters remain unfinished.'],cases=records)
 (PROJECT/'analysis/main_dispatch_uv_constant_alternate_strips_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered',*totals)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=services.original_divide;current.sh4_binary32.execute_fmac=services.geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
