"""Native filter and material services versus original code and mask invariants."""
import ctypes,gc,hashlib,inspect,itertools,json,sys
from pathlib import Path
import verify_main_dispatch_mesh as mesh
from verify_tikal_controller import Call,P
PROJECT=Path(__file__).resolve().parent
traversal=mesh.traversal;geom=mesh.geom;particle=mesh.particle;current=mesh.current;base=mesh.base
load,store=mesh.load,mesh.store
FILTER,FLAGS_FN,SHADER,TEXTURE,FINISH=0x8C611444,0x8C606C56,0x8C606A08,0x8C606A88,0x8C6069A4
NATIVE={FILTER,FLAGS_FN,SHADER,TEXTURE,FINISH};WORKERS={0x8C611060,0x8C611122,0x8C6111D6,0x8C6112E2}
SERVICES=(mesh.SERVICES-NATIVE)|WORKERS
HEADER=base.literal(0x8C606CA8);HEADER_OUT,CMD_OUT,TEXROOT,TEXTABLE,TEXRECORD=0x8CD10000,0x8CD20000,0x8CD30000,0x8CD31000,0x8CD32000
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_mesh_services.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(mesh.SPECS,filter_core=(FILTER,'sa_mesh_filter_connected'),material_flags=(FLAGS_FN,'sa_material_flags_connected'),shader=(SHADER,'sa_material_shader_connected'),texture=(TEXTURE,'sa_material_texture_connected'),finish=(FINISH,'sa_material_finish_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32','sa_sh4_fmac'):
 fn=getattr(LIB,name);fn.argtypes=getattr(geom.LIB,name).argtypes;fn.restype=ctypes.c_int
LIB.sa_sh4_divide.argtypes=[P,P,P,P];LIB.sa_sh4_divide.restype=ctypes.c_int
original_divide=current.sh4_binary32.execute_divide

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def effect(c,target,mem,r,sr,fp):
 if target not in WORKERS:return mesh.effect(c,target,mem,r,sr,fp)
 if c.get('capture') is not None:c['capture'].append((target,r[4],r[5],load(mem,mesh.FLAGS)))
 if target in (0x8C6111D6,0x8C6112E2):
  store(mem,r[4]+16,c.get('clip_mask',0));store(mem,r[4]+20,c.get('clip_result',0))
 for i in range(8):r[i]=0xBAD00000+i*0x101
 return sr^1

def setup():
 source=inspect.getsource(mesh.setup).replace('def setup():','def local_setup():',1)
 source=source.replace('traversal.setup()','mesh.setup()')
 source=source.replace('main_dispatch_mesh_translation.json','main_dispatch_mesh_services_translation.json')
 source=source.replace('main_dispatch_model_traversal_verification.json','main_dispatch_mesh_verification.json')
 source=source.replace('base.TRACKED|={MESH,FILTER}','base.TRACKED|=NATIVE')
 marker=' for old,new in changes.items():';assert source.count(marker)==1
 source=source.replace(marker,""" changes['  elif w&15==2:rc=LIB.sa_sh4_multiply_extended']=changes['  elif w&15==2:rc=LIB.sa_sh4_multiply_extended'].replace('  elif w&15==14:', '  elif w&15==3:rc=LIB.sa_sh4_divide(ptr(n),ptr(src),ctypes.byref(fps),ptr(n))\\n  elif w&15==14:')
 changes[' def trace(pc,w,r,sr,pr,_f):']=' filter_returns=[]\\n def trace(pc,w,r,sr,pr,_f):\\n  nonlocal ops'
 changes['  assert pc in allowed,hex(pc);path.append(pc)']='  assert pc in allowed,hex(pc);path.append(pc)\\n  if pc==0x8c611444:filter_returns.append(pr)\\n  if pc in (0x8c6114f8,0x8c6115a6,0x8c61162e):\\n   address=(r[2]+(r[0] if pc==0x8c6114f8 else 8))&0xffffffff\\n   if address&0x1f000000!=0x0c000000:stop(pc,r,sr,pr,-3)\\n  if pc==0x8c6116e4:\\n   if pr!=filter_returns[-1]:stop(pc,r,sr,pr,-3)\\n   filter_returns.pop()'
 changes['w&15 in (0,1,2)']='w&15 in (0,1,2,3,14)'
 changes['  if w&0xF00F==0xF005 and ef.fpscr&0x80000:stop(pc,r,sr,pr,-1)']=changes['  if w&0xF00F==0xF005 and ef.fpscr&0x80000:stop(pc,r,sr,pr,-1)'].replace('w==0xFBFD','w in (0xFBFD,0xF3FD)')
"""+marker)
 ns=dict(mesh.__dict__);ns.update(PROJECT=PROJECT,mesh=mesh,LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,NATIVE=NATIVE,SERVICES=SERVICES,effect=effect)
 current.sh4_binary32.execute_divide=current.independent_enabled(original_divide)
 exec(compile(source,'mesh_services_setup','exec'),ns)
 return ns['local_setup']()

def prepare(c,n):
 m,s,f=mesh.prepare(c,n)
 store(m,base.literal(0x8C606B3C),HEADER_OUT);store(m,base.literal(0x8C6069D4),CMD_OUT)
 store(m,base.literal(0x8C6069C0),0xE0000000)
 for i,v in enumerate(c.get('header',(0x12345678,0x87654321,0xA5A55A5A,0x5A5AA5A5))):store(m,HEADER+i*4,v)
 store(m,base.literal(0x8C606B40),TEXROOT);store(m,TEXROOT,TEXTABLE)
 for i in range(4):
  a=TEXRECORD+i*32;store(m,TEXTABLE+i*12+8,a)
  for j,v in enumerate((0x11000000+i,0x22000000+i,0x13579BDF,c.get('texture_address',0x80000))):store(m,a+j*4,v)
 store(m,base.literal(0x8C606B44),0x11000000+c.get('texture_index',0) if c.get('cache') in ('first','both') else 0)
 store(m,base.literal(0x8C606B48),0x22000000+c.get('texture_index',0) if c.get('cache')=='both' else 0)
 # Valid filter camera/projection and unit normal, plus explicit output buffers.
 for a,v in ((0x8C88F5E4,1),(0x8C88F5F0,0),(0x8C88F5F4,0),(0x8C88F618,1),(0x8C88F61C,1),(0x8C88F610,1),(0x8C88F614,100),(0x8C890254,1)):
  store(m,a,current.bits(v))
 for i,v in enumerate((-10,-10,10,10)):store(m,0x8C88F600+i*4,current.bits(v))
 for i,v in enumerate((1,0,0)):store(m,0x8C89023C+i*4,current.bits(v))
 store(m,0x8C88F64C,0x8CD40000);store(m,0x8C88F654,0)
 env=c.get('env',0)|(0x100 if c.get('bounds') else 0)|(2 if c.get('clip') else 0);store(m,mesh.ENV,env)
 targets=[mesh.DATA] if c['entry'] in ('mesh','filter','filter_core') else list(traversal.MESHES[:5])
 if c.get('graph')=='retail':targets=[]
 for a in targets:
  store(m,a,0x8CD50000);store(m,a+4,0x8CD60000 if c.get('normals') else 0)
  for i,v in enumerate(c.get('center',(0,0,4))):store(m,a+0x18+i*4,current.bits(v))
  store(m,a+0x24,current.bits(c.get('radius',1)))
 if c['entry']=='filter_core':s.r[4]=mesh.DATA
 elif c['entry'] in ('material_flags','shader','finish'):s.r[4]=c.get('argument',0x89ABCDEF)
 elif c['entry']=='texture':s.r[4]=c.get('texture_index',0)
 return m,s,f

def independent(c,m,before,s):
 entry=c['entry'];arg=c.get('argument',0x89ABCDEF)
 if entry=='material_flags':assert load(m,HEADER+8)==(load(before,HEADER+8)&0x03F800FF)|(arg&0xFC07FF00)
 if entry=='finish':assert load(m,CMD_OUT)==(arg&0x03FFFFFF)|load(before,0x8C890128)
 if entry=='shader':
  h=[load(before,HEADER+i*4) for i in range(3)];env=c.get('env',0)
  expected=(h[0]&0xFDFFFF00)|(arg&0x020000FF)
  flag=bool(env&0x400) or bool(env&0x1000) and not arg&0x02100000
  expected=expected|0x80 if flag else expected&0xFFFFFF7F
  assert [load(m,HEADER+i*4) for i in range(3)]==[expected,h[1],(h[2]&0xFFE7FFFF)|(arg&0x00180000)]
  assert load(m,HEADER_OUT)==arg
 if entry=='texture':
  skip=c.get('cache')=='both';address=c.get('texture_address',0x80000)
  assert s.r[0]==(1 if skip or address&0x1FFFFF<=0x100000 else 0xFFFFFFFF)
  if skip:assert load(m,HEADER+8)==load(before,HEADER+8)
  else:assert load(m,HEADER+8)==(load(before,HEADER+8)&0xFFFFFFC0)|(0x13579BDF&63)
 if entry=='filter_core' and not c.get('mode',0)&0x180000:
  center=c.get('center',(0,0,4));outside=c.get('bounds') and (center[2] in (0,200) or abs(center[0])==100 or abs(center[1])==100)
  rejected=outside or not c.get('clip') and c.get('clip_mask',0)&63
  assert s.r[0]==(0xFFFFFFFF if rejected else 0),(c,hex(s.r[0]))

def cases():
 for entry,arg,env,mode in itertools.product(('material_flags','shader','finish'),(0,1,0xFFFFFFFF,0x80000000,0x02100000,0x12345678),(0,0x400,0x1000,0x1400),range(8)):
  yield dict(entry=entry,argument=arg,env=env,mode=mode<<19)
 for idx,cache,address,mode in itertools.product(range(3),('none','first','both'),(0x80000,0x100000,0x100001,0xFFFFFFFF),range(8)):
  yield dict(entry='texture',texture_index=idx,cache=cache,texture_address=address,mode=mode<<19)
 for bounds,clip,normals,mode,rm in itertools.product((False,True),(False,True),(False,True),range(8),range(2)):
  yield dict(entry='filter_core',bounds=bounds,clip=clip,normals=normals,mode=mode<<19,rm=rm)
 for center,clip,normals,bank in itertools.product(((0,0,0),(0,0,200),(-100,0,4),(100,0,4),(0,-100,4),(0,100,4),(0,0,4)),(False,True),(False,True),range(2)):
  yield dict(entry='filter_core',center=center,bounds=True,clip=clip,normals=normals,mode=bank<<21)
 for mask,clip,normals,bank in itertools.product((0,1,2,4,8,16,32,64),(False,True),(False,True),range(2)):
  yield dict(entry='filter_core',clip_mask=mask,clip_result=0x12345678,clip=clip,normals=normals,mode=bank<<21)
 for entry,layout,bank,result in itertools.product(('mesh','mesh_wrapper'),mesh.LAYOUTS,range(2),(0,1)):
  yield dict(entry=entry,layout=layout,mode=bank<<21,materials=(0,0,1),profiles=(layout,layout|0x100000),clip_result=result)
 for entry,variant,bank in itertools.product(('geometry','draw','primary'),range(2),range(2)):
  yield dict(entry=entry,graph='retail',variant=variant,mode=bank<<21)
 for bounds,enable,bank,rm in itertools.product((False,True),(0,1,2,4,8,16,32),range(2),(0,1,2,3)):
  yield dict(entry='filter_core',bounds=bounds,mode=bank<<21,enable=enable,rm=rm)

def main():
 manifest_path,manifest,baseline,allowed,execute=setup()
 records=[];rows=set();edges=set();calls=set();counts=dict.fromkeys(SPECS,0);first={}
 total=dict(boundary_comparisons=0,numerical_operations=0,partial_guard_error_cases=0,independent_invariant_cases=0,external_service_boundaries=0)
 new_rows={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 wanted_calls={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']}
 def check(c,n):
  m,s,f=prepare(c,n);before=bytes(m);captured=[];c=dict(c,capture=captured)
  path,status,b,ops,_,invocations=execute(c,m,s,f,c['entry'],0xFEED1234,allowed)
  rows.update(set(path)&new_rows);counts[c['entry']]+=1;calls.update(set(path)&wanted_calls)
  for i,a in enumerate(path):
   if a not in new_rows:continue
   w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
   if w>>8 in (0x89,0x8B,0x8D,0x8F):
    j=i+(2 if w>>8 in (0x8D,0x8F) else 1)
    if j<len(path):edges.add((a,path[j]))
  total['boundary_comparisons']+=b;total['numerical_operations']+=ops;total['partial_guard_error_cases']+=status!=0;total['external_service_boundaries']+=invocations
  if status==0:
   independent(c,m,before,s);total['independent_invariant_cases']+=1
  c.pop('capture')
  if status==0:
   for target,*_ in captured:first.setdefault(target,dict(c))
  records.append(dict(input=c,steps=len(path),status=status,full_state_match=True,boundary_comparisons=b,independent_invariants=status==0))
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e) for e in ('material_flags','shader','texture','finish','filter_core','mesh')]+[dict(entry='filter_core',bounds=True),dict(entry='filter_core',clip=True,normals=True),dict(entry='draw',graph='retail')]
 for n,c in enumerate(selected):
  check(c,n)
  if (n+1)%100==0:print(n+1,'filter/material service comparisons passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Mesh service smoke checks passed:',len(records));return
 assert set(first)==SERVICES,('unreached services',[hex(t) for t in SERVICES-set(first)])
 for target,c in sorted(first.items()):check(dict(c,missing_target=target),len(records));check(dict(c,fail_target=target),len(records))
 assert rows==new_rows,('rows missing',[hex(a) for a in new_rows-rows])
 want_edges={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert edges==want_edges,('edges missing',[list(map(hex,e)) for e in want_edges-edges])
 assert calls==wanted_calls
 files=[Path(__file__),Path(mesh.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_mesh_services.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_mesh_services_connected.c',PROJECT/'source/sh4_divide.c',PROJECT/'source/sh4_divide.h']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),**total,service_targets=sorted(hex(a) for a in SERVICES),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Native mesh filter, material flags, shader header, texture cache/descriptor lookup and command finalization; original full-state and ordered boundary comparisons with independent mask/cache/culling-result assertions.',limitations=['Four lower vertex/filter workers and 22 acquisition/primitive services remain explicit fixtures. No pixels, GPU/MMIO or complete frame-driver proof.','Existing finite scalar and shared vector numerical-oracle limits retained; scalar trap classification uses native probes.','Constructed valid projection, bounds, texture table, headers and command destinations; arbitrary metadata/pointers/hardware exceptions remain outside scope.'],cases=records)
 result['unsupported_transfer_cases']=sum(c['status']==-3 for c in records)
 result['limitations'].append('Reached saved-mesh-pointer and changed-return-address guards return host status -3 before an unsupported transfer; partial state is compared, not physical Dreamcast fault dispatch.')
 (PROJECT/'analysis/main_dispatch_mesh_services_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered',*total)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_divide=original_divide;current.sh4_binary32.execute_fmac=geom.original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
