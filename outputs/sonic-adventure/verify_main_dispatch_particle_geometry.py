"""Native rendering-data preparation versus original instructions and algebra."""
import ctypes,gc,hashlib,inspect,itertools,json,math,sys
from pathlib import Path
import verify_main_dispatch_particle_callback as particle
from verify_tikal_controller import Call,P
PROJECT=Path(__file__).resolve().parent
current=particle.current;base=current.base;load,store=current.load,current.store
GEOMETRY,WRAPPER,MODEL=0x8C620F2C,0x8C6215FA,0x8C62113C
VIEW_SLOT,COEFFICIENTS,OUTPUT,COPY_A,COPY_B=(base.literal(a) for a in (0x8C62110C,0x8C621110,0x8C621114,0x8C621238,0x8C62123C))
VIEW=0x8CC50000
OUTPUT_WORDS=(OUTPUT,OUTPUT+4,OUTPUT+8,COPY_A,COPY_B)
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_particle_geometry.dll';LIB=ctypes.CDLL(str(DLL))
SPECS=dict(particle.SPECS,geometry=(GEOMETRY,'sa_particle_geometry_connected'),model_wrapper=(WRAPPER,'sa_particle_model_wrapper_connected'))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32'):
 fn=getattr(LIB,name);fn.argtypes=getattr(current.LIB,name).argtypes;fn.restype=ctypes.c_int
LIB.sa_sh4_fmac.argtypes=[P,P,P,P,P];LIB.sa_sh4_fmac.restype=ctypes.c_int
original_fmac=current.sh4_binary32.execute_fmac

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def model_effect(c,mem,r,sr,fp):
 if c.get('expected_model') is not None:assert r[4]==c['expected_model'],hex(r[4])
 if not c.get('mode',0)&0x100000 and c.get('entry')!='model_wrapper':
  # Copy-through fields must reach the model call before its effects.
  assert load(mem,COPY_A)==load(mem,VIEW+0x12C)
  assert load(mem,COPY_B)==load(mem,VIEW+0x128)
 if c.get('capture') is not None:
  c['capture'].append([load(mem,a) for a in OUTPUT_WORDS])
 return particle.geometry_effect(c,mem,r,sr,fp)

def setup():
 *_,allowed,_=particle.setup()
 manifest_path=PROJECT/'analysis/main_dispatch_particle_geometry_translation.json';manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_particle_callback_verification.json'
 for report in (manifest,json.loads(baseline.read_text())):
  for p,h in report['sha256'].items():assert digest(Path(p))==h,p
 allowed|={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 base.TRACKED|={GEOMETRY,WRAPPER}
 current.sh4_binary32.execute_fmac=current.independent_enabled(original_fmac)
 source=inspect.getsource(current.execute)
 changes={
  'load(mem,base.LEVELS+20)':'load(mem,base.LEVELS+20),tuple(load(mem,particle.STATE_COPY+i*4) for i in range(9)),tuple(load(mem,a) for a in OUTPUT_WORDS)',
  '(0xF08D,0xF04D,0xF05D)':'(0xF08D,0xF09D,0xF04D,0xF05D)',
  '  elif w&15==2:rc=LIB.sa_sh4_multiply_extended':'  elif w&15==14:rc=LIB.sa_sh4_fmac(ptr(0),ptr(src),ptr(n),ctypes.byref(fps),ptr(n))\n  elif w&15==2:rc=LIB.sa_sh4_multiply_extended',
  ' def trace(pc,w,r,sr,pr,_f):':' def trace(pc,w,r,sr,pr,_f):\n  nonlocal ops',
  'w&15 in (0,1,2)':'w&15 in (0,1,2,14)',
  '   rc,probe=scalar_probe(w)':'   ops+=1\n   rc,probe=scalar_probe(w)',
  ' try:base.previous.reference(em,expected,fpu=ef,special=special,trace=trace,vector_instruction=vector,instruction_budget=40000)':
  ''' def model_hook(mem,r,sr):
  wanted.append(event(MODEL,mem,r,sr,expected.pr,ef,ef.fpul,special['macl']))
  if c.get('null_model'):stop(MODEL,r,sr,expected.pr,-2)
  sr=model_effect(c,mem,r,sr,ef)
  if c.get('fail_model'):stop(MODEL,r,sr,expected.pr,-17)
  return sr
 try:base.previous.reference(em,expected,fpu=ef,special=special,trace=trace,hooks={MODEL:model_hook},vector_instruction=vector,instruction_budget=40000)''',
  ' @base.OBSERVE\n def observe':
  ''' @Call
 def model_call(_ctx,target,mem,fp,ss,r,p):
  try:
   assert target==MODEL,hex(target)
   ss[0]=model_effect(c,mem,r,ss[0],fp.contents)
   return -17 if c.get('fail_model') else 0
  except Exception as exc:errors.append(repr(exc));return -99
 @base.OBSERVE
 def observe''',
  'None,Call(),observe)':'None,Call() if c.get("null_model") else model_call,observe)',
  'return path,status,len(observed),ops,ml.value':'return path,status,len(observed),ops,ml.value,(0 if c.get("null_model") else sum(e[0]==MODEL for e in observed))',
 }
 for old,new in changes.items():assert source.count(old)==1,(old,source.count(old));source=source.replace(old,new)
 namespace=dict(current.__dict__)
 namespace.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,LEGACY=current.LEGACY|{particle.TRANSLATE},
                  particle=particle,OUTPUT_WORDS=OUTPUT_WORDS,MODEL=MODEL,model_effect=model_effect)
 exec(compile(source,'particle_geometry_observed_execute','exec'),namespace)
 return manifest_path,manifest,baseline,allowed,namespace['execute']

def prepare(c,n):
 m,s,f=particle.prepare(c,n)
 store(m,VIEW_SLOT,VIEW)
 kind=c.get('data','identity')
 for i in range(96):store(m,VIEW+i*4,current.bits(((i%7)-3)*0.125))
 for i in range(16):
  a=(1.0 if i%5==0 else 0.0) if kind=='identity' else (((i*7)%13)-6)*0.0625
  b=(1.0 if i%5==0 else 0.0) if kind=='identity' else (((i*3)%11)-5)*0.125
  if kind=='rounded':a=(i-7)/7;b=(i-5)/11
  if kind=='negative':a=-a;b=-b
  store(m,VIEW+i*4,current.bits(a));store(m,COEFFICIENTS+i*4,current.bits(b))
 for i,v in enumerate((0.25,0.5,-0.75)):store(m,VIEW+0x4C+i*4,current.bits(v))
 store(m,VIEW+0x128,0xC001D00D);store(m,VIEW+0x12C,0xD00DC0DE)
 for a in OUTPUT_WORDS:store(m,a,0xCAFE1234)
 if not c.get('dn',True):f.fpscr&=~0x40000
 for region,offset,value in c.get('patches',[]):store(m,(VIEW if region=='view' else COEFFICIENTS)+offset,value)
 if c['entry'] in ('geometry','model_wrapper'):s.r[4]=particle.MODEL_A if c.get('variant',0)&1 else particle.MODEL_B
 return m,s,f

def independent(c,m,before,captured):
 if c['entry']=='model_wrapper':
  assert captured and all(v==[load(before,a) for a in OUTPUT_WORDS] for v in captured)
 elif captured and not c.get('mode',0)&0x180000 and not c.get('patches'):
  a=[current.value(load(before,VIEW+i*4)) for i in range(16)]
  b=[current.value(load(before,COEFFICIENTS+i*4)) for i in range(16)]
  d=[current.value(load(before,VIEW+0x4C+i*4)) for i in range(3)]
  # Independent matrix/vector algebra, without the translator or SH-4 executor.
  t=[[sum(a[4*i+k]*b[4*k+j] for k in range(3)) for j in range(3)] for i in range(3)]
  expected=[sum(t[i][j]*d[i] for i in range(3)) for j in range(3)]
  assert all(all(abs(current.value(raw)-value)<0.0001 for raw,value in zip(v[:3],expected)) for v in captured),(c,captured,expected)
 if c['entry'] in ('primary','secondary','work','draw'):
  particle.independent(c,m,before,c['entry'])

def cases():
 for entry,mode,rm,data in itertools.product(('geometry','model_wrapper','draw','primary','secondary','work'),range(8),range(2),('identity','mixed','negative')):
  yield dict(entry=entry,mode=mode<<19,rm=rm,data=data,variant=mode&1)
 for entry,bank,failure in itertools.product(('geometry','model_wrapper','draw','primary'),range(2),('missing','error')):
  yield dict(entry=entry,mode=bank<<21,data='mixed',null_model=failure=='missing',fail_model=failure=='error')
 for entry,bank,enable in itertools.product(('geometry','draw','primary'),range(2),(1,2,4,8,16,32)):
  yield dict(entry=entry,mode=bank<<21,data='mixed',enable=enable)
 for entry,bank,rm in itertools.product(('geometry','draw','primary'),range(2),(2,3)):
  yield dict(entry=entry,mode=bank<<21,rm=rm)
 for entry,bank,rm,enable in itertools.product(('geometry','draw','primary'),range(2),range(2),(0,1,2,4,8,16,32)):
  yield dict(entry=entry,mode=bank<<21,rm=rm,enable=enable,data='rounded')
 for raw,region,offset,dn,bank in itertools.product((1,0x80000001,0x7F800000,0x7FBFFFFF),('view','coefficients'),(0,4,8),(False,True),range(2)):
  yield dict(entry='geometry',patches=[(region,offset,raw)],dn=dn,mode=bank<<21,data='mixed')
 for bank,swap,gm,after in itertools.product(range(2),(False,True),(0,16),(0,16)):
  yield dict(entry='work',mode=bank<<21,swap=swap,game_mode=gm,after_mode=after,data='mixed',flip_bank=True,flip_swap=True)

def main():
 manifest_path,manifest,baseline,allowed,execute=setup()
 records=[];rows=set();calls=set();counts=dict.fromkeys(SPECS,0)
 total=dict(boundary_comparisons=0,numerical_operations=0,partial_guard_error_cases=0,independent_invariant_cases=0,model_service_invocations=0)
 new_rows={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry='geometry'),dict(entry='model_wrapper'),dict(entry='draw'),dict(entry='primary'),dict(entry='geometry',data='mixed'),dict(entry='geometry',mode=0x100000),dict(entry='geometry',null_model=True),dict(entry='primary',fail_model=True)]
 for n,c in enumerate(selected):
  c=dict(c,expected_model=particle.MODEL_A if c.get('variant',0)&1 else particle.MODEL_B)
  m,s,f=prepare(c,n);before=bytes(m);captured=[];c['capture']=captured
  path,status,b,ops,_,invocations=execute(c,m,s,f,c['entry'],0xFEED1234,allowed)
  rows.update(new_rows&set(path));counts[c['entry']]+=1
  calls.update(a for a in path if a in (0x8C621126,0x8C621604))
  total['boundary_comparisons']+=b;total['numerical_operations']+=ops;total['partial_guard_error_cases']+=status!=0
  total['model_service_invocations']+=invocations
  if status==0:independent(c,m,before,captured);total['independent_invariant_cases']+=1
  c.pop('capture');records.append(dict(input=c,steps=len(path),status=status,full_state_match=True,boundary_comparisons=b,independent_invariants=status==0))
  if (n+1)%100==0:print(n+1,'geometry/model wrapper comparisons passed',flush=True)
  if (n+1)%25==0:gc.collect()
 if '--smoke' in sys.argv:print('Geometry smoke checks passed:',len(records));return
 assert rows==new_rows,('missing rows',[hex(a) for a in new_rows-rows])
 assert calls=={0x8C621126,0x8C621604}
 files=[Path(__file__),Path(particle.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_particle_geometry.ps1',PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_particle_geometry_connected.c',PROJECT/'source/sh4_fmac.c',PROJECT/'source/sh4_fmac.h',PROJECT/'sh4_binary32.py']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 result=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=0,call_sites_covered=len(calls),**total,sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),scope='Native geometry preparation and R8-preserving model wrapper, called directly and through recovered particle callbacks; original full-state/boundary comparisons and independent matrix-vector algebra.',limitations=['Model traversal 8c62113c is an explicit fixture; no actual rendering, pixels, GPU/MMIO or full frame-order proof.','Finite scalar arithmetic uses existing helpers and independent Python dyadics. Guard/trap classification uses native probes; hardware exception dispatch and arbitrary pointers remain outside scope.','Inherited shared vector numerical oracle limits; wide-FMOV modes use constructed data and do not establish normal gameplay reachability.'],cases=records)
 (PROJECT/'analysis/main_dispatch_particle_geometry_verification.json').write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({k:result[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered',*total)},indent=2))

if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_fmac=original_fmac
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
