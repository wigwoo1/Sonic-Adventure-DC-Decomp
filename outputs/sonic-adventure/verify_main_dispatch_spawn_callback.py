"""Native spawn callback/transform/emission versus retail execution and invariants."""
import ctypes,gc,hashlib,itertools,json,math,struct,sys
from pathlib import Path
import verify_main_dispatch_context_callback as previous
import verify_sh4_vector_numeric as vectors
import sh4_binary32
from verify_startup import State
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

PROJECT=Path(__file__).resolve().parent;core=previous.previous;base=core.base
CALLBACK,TRANSFORM,EMIT=0x8C044680,0x8C0446A2,0x8C04474A
PARENT,WORK,DESC,STACK=0x8CDB0000,0x8CDA0000,0x8CD00000,0x8CE10000
NODES=tuple(0x8CE50000+i*0x100 for i in range(8))
TABLE=base.literal(0x8C044844);RNG=0x8C10D334;SEED=0x8C5FBB08
PARTICLE=base.literal(0x8C044884)
load,store=base.load,base.store
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_spawn_callback.dll';LIB=ctypes.CDLL(str(DLL))
SPECS={'callback':(CALLBACK,'sa_main_dispatch_spawn_callback_connected'),
       'transform':(TRANSFORM,'sa_spawn_work_transform_connected'),
       'emit':(EMIT,'sa_spawn_particle_emit_connected'),
       'spawn':(base.previous.SPAWN,'sa_main_dispatch_spawn_connected'),
       'schedule':(previous.SCHEDULE,'sa_task_level_update_connected')}
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE];fn.restype=ctypes.c_int
for name,args in (('sa_sh4_add_sub',[P,P,P,P,ctypes.c_int]),('sa_sh4_multiply_extended',[P,P,P,P]),
                  ('sa_sh4_float_i32',[ctypes.c_uint32,P,P]),('sa_sh4_truncate_i32',[ctypes.c_uint32,P,P])):
 fn=getattr(LIB,name);fn.argtypes=args;fn.restype=ctypes.c_int
OPCODES={word:(kind,n,m) for kind,n,m,word in vectors.SPECS}
LEGACY={0x8C639C70,0x8C639B98,0x8C63A1CC,0x8C639EC8,0x8C639F5C,0x8C638F28}
original_float=sh4_binary32.execute_float;original_add=sh4_binary32.execute_add_sub
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def value(x):return struct.unpack('<f',struct.pack('<I',x))[0]
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def independent_enabled(fn):
 def execute(f,*args):
  enabled=f.fpscr&0xF80;f.fpscr&=~0xF80
  fn(f,*args);f.fpscr|=enabled
 return execute


def setup():
 *_,allowed=previous.setup()
 manifest_path=PROJECT/'analysis/main_dispatch_spawn_callback_translation.json'
 manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_context_callback_verification.json'
 for report in (manifest,json.loads(baseline.read_text())):
  for p,h in report['sha256'].items():assert digest(Path(p))==h,p
 allowed|={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 registry=json.loads((PROJECT/'analysis/recovered_routines.json').read_text(encoding='utf-8-sig'))
 dependencies=[]
 for address in (*LEGACY,RNG):
  r=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
  allowed.update(range(address,int(r['end_exclusive'],16),2))
  dependencies.append({k:r[k] for k in ('address','end_exclusive','source','verification')})
  report=json.loads((PROJECT/r['verification']).read_text())
  if report.get('source_sha256') and report.get('source',r['source'])==r['source']:
   assert digest(PROJECT/r['source'])==report['source_sha256'],r['source']
 base.TRACKED|={CALLBACK,TRANSFORM,EMIT,RNG}|LEGACY
 sh4_binary32.execute_float=independent_enabled(original_float)
 sh4_binary32.execute_add_sub=independent_enabled(original_add)
 return manifest_path,manifest,baseline,dependencies,allowed


def prepare(c,n):
 m,s,f=core.prepare(dict(entry='cancel',mode=c.get('mode',0),pool_count=0),n)
 f.fpscr=c.get('mode',0)|0x40000|c.get('rm',0)|(c.get('enable',0)<<7)
 for b in range(2):f.banks[b][:]=[bits((i+1)*0.125) for i in range(16)]
 bank=(f.fpscr>>21)&1
 f.banks[bank^1][:]=[bits(1.0) if i%5==0 else 0 for i in range(16)]
 if c.get('wide_pair'):f.banks[bank^1][2]=bits(2.0**50);f.banks[bank^1][3]=0x4F000000
 for node in (*NODES,PARENT):
  for off in range(0,48,4):store(m,node+off,0)
 pool=c.get('pool',6)
 for i,node in enumerate(NODES[:pool]):
  store(m,node,NODES[i+1] if i+1<pool else 0);store(m,node+4,NODES[i-1] if i else 0)
 store(m,base.FREE_TASK,NODES[0] if pool else 0)
 for i in range(8):store(m,base.LEVELS+i*4,0)
 size=c.get('heap',512)
 for off in range(0,1040,4):store(m,base.ARENA+off,0xA5A5A5A5)
 store(m,base.FREE_HEAP,base.ARENA if size else 0)
 if size:
  for i,v in enumerate((base.ARENA+size,0,base.ARENA+size,0)):store(m,base.ARENA+i*4,v)
  for i,v in enumerate((0,base.ARENA,0,base.ARENA)):store(m,base.ARENA+size+i*4,v)
 character=c.get('character',0xFF)&255
 for off in range(0,64,4):store(m,WORK+off,0)
 store(m,WORK+8,character,1);store(m,base.previous.OBJECT+8,character,1)
 for off,raw in zip((0x20,0x24,0x28),c.get('position',(bits(1.25),bits(-2),bits(0.75)))):store(m,WORK+off,raw)
 store(m,PARENT+0x20,WORK);store(m,PARENT+16,CALLBACK)
 store(m,TABLE+character*4,DESC if c.get('descriptor',True) else 0)
 x,y,z=c.get('angles',(0,0,0))
 store(m,DESC+0x14,x);store(m,DESC+0x18,y);store(m,DESC+0x1C,z)
 for off,raw in zip((0x20,0x24,0x28),c.get('base',(bits(1),bits(2),bits(3)))):store(m,DESC+off,raw)
 for i in range(16):store(m,base.literal(0x8C04484C)+4*i,bits(1) if i%5==0 else 0)
 store(m,0x8C88F698,16);store(m,0x8C88F69C,1);store(m,0x8C88F5F8,STACK)
 for off in range(0,256,4):store(m,STACK+off,0xC0DEC0DE)
 store(m,0x8C890070,0);store(m,SEED,c.get('seed',0x12345678))
 store(m,previous.ACTIVE,0xA5A55A5A)
 s.r[4]=PARENT if c['entry']=='callback' else base.previous.OBJECT if c['entry']=='spawn' else WORK
 return m,s,f


class Stopped(Exception):pass


def execute(c,m,s,f,entry,macl,allowed):
 s.pc=SPECS[entry][0];initial=State.from_buffer_copy(s);before_f=Fpu.from_buffer_copy(f)
 em=bytearray(m);expected=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f)
 special={'macl':macl};path=[];wanted=[];observed=[];status=0;errors=[];ops=0
 def event(t,mem,r,sr,pr,fp,fpul,ml):
  return t,tuple(r),sr,pr,bytes(fp)[:132],fpul,ml,load(mem,SEED),load(mem,0x8C88F69C),load(mem,base.LEVELS+20)
 def stop(pc,r,sr,pr,rc,probe=None):
  nonlocal status
  if probe is not None:ctypes.memmove(ctypes.byref(ef),ctypes.byref(probe),ctypes.sizeof(Fpu))
  expected.r[:]=r;expected.sr=sr;expected.pr=pr;expected.pc=pc;status=rc;raise Stopped()
 def scalar_probe(w):
  probe=Fpu.from_buffer_copy(ef);bank=(probe.fpscr>>21)&1;n=(w>>8)&15;src=(w>>4)&15
  fps=ctypes.c_uint32(probe.fpscr);fpul=ctypes.c_uint32(probe.fpul)
  ptr=lambda i:ctypes.cast(ctypes.byref(probe.banks[bank],i*4),P)
  if w&0xF0FF==0xF02D:rc=LIB.sa_sh4_float_i32(fpul.value,ctypes.byref(fps),ptr(n))
  elif w&0xF0FF==0xF03D:rc=LIB.sa_sh4_truncate_i32(probe.banks[bank][n],ctypes.byref(fps),ctypes.byref(fpul))
  elif w&15==2:rc=LIB.sa_sh4_multiply_extended(ptr(n),ptr(src),ctypes.byref(fps),ptr(n))
  else:rc=LIB.sa_sh4_add_sub(ptr(n),ptr(src),ctypes.byref(fps),ptr(n),int(w&15==1))
  probe.fpscr=fps.value;probe.fpul=fpul.value
  return rc,probe
 def trace(pc,w,r,sr,pr,_f):
  assert pc in allowed,hex(pc);path.append(pc)
  if pc in base.TRACKED and len(path)>1:wanted.append(event(pc,em,r,sr,pr,ef,ef.fpul,special['macl']))
  if pc in LEGACY and ef.fpscr&0x180000:stop(pc,r,sr,pr,-1)
  if w&0xF0FF in (0xF08D,0xF04D,0xF05D) and ef.fpscr&0x80000:stop(pc,r,sr,pr,-1)
  if w in OPCODES:
   if ef.fpscr&3>1:stop(pc,r,sr,pr,-1)
   kind,n,src=OPCODES[w];probe=Fpu.from_buffer_copy(ef)
   rc=vectors.primitive(kind,n,src,ctypes.byref(probe),probe.fpul)
   if rc:stop(pc,r,sr,pr,-1 if 0x8C638F28<=pc<0x8C638F8C else rc,probe)
  if w>>12==15 and (w&15 in (0,1,2) or w&0xF0FF in (0xF02D,0xF03D)):
   rc,probe=scalar_probe(w)
   if rc:stop(pc,r,sr,pr,rc,probe)
  if w&0xF00F==0xF005 and ef.fpscr&0x80000:stop(pc,r,sr,pr,-1)
 def vector(w,fp):
  nonlocal ops
  kind,n,src=OPCODES[w];ops+=1
  assert vectors.primitive(kind,n,src,ctypes.byref(fp),fp.fpul)==0
 try:base.previous.reference(em,expected,fpu=ef,special=special,trace=trace,vector_instruction=vector,instruction_budget=40000)
 except Stopped:pass
 sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr);fpul=ctypes.c_uint32(before_f.fpul);ml=ctypes.c_uint32(macl)
 @base.OBSERVE
 def observe(_ctx,t,mem,fp,ss,r,p):
  try:observed.append(event(t,mem,r[:16],ss[0],p[0],fp.contents,fpul.value,ml.value))
  except Exception as exc:errors.append(repr(exc))
 rc=FUNCTIONS[entry]((ctypes.c_uint8*base.previous.RAM_SIZE).from_buffer(m),ctypes.byref(f),ctypes.byref(sr),s.r,
                      ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(ml),None,Call(),observe)
 assert rc==status and not errors,(c,entry,rc,status,errors)
 assert m==em and list(s.r)==list(expected.r),(c,entry,'RAM/GPR differs')
 assert sr.value==expected.sr and pr.value==expected.pr,(c,entry,'SR/PR differs')
 assert bytes(f)[:132]==bytes(ef)[:132] and fpul.value==ef.fpul and ml.value==special['macl'],(c,entry,'FPU/FPUL/MACL differs')
 assert observed==wanted,(c,entry,'ordered boundary differs')
 if not status:
  assert s.r[15]==initial.r[15] and pr.value==initial.pr
  assert list(s.r)[8:15]==list(initial.r)[8:15]
 s.sr=sr.value;s.pr=pr.value;f.fpul=fpul.value
 return path,status,len(observed),ops,ml.value


def children(m):return previous.chain(m,load(m,base.LEVELS+20))


def independent(c,m,parent_work,entry):
 nodes=children(m);seed=c.get('seed',0x12345678);random_values=[]
 for _ in nodes:
  seed=(seed*1103515245+12345)&0xFFFFFFFF;random_values.append((seed>>16)&32767)
 assert load(m,SEED)==seed
 assert len(nodes)==(0 if entry=='transform' else min(6,c.get('pool',6),c.get('heap',512)//80))
 if not c.get('mode',0)&0x180000:
  for t,rnd in zip(reversed(nodes),random_values):
   w=load(m,t+0x20);assert load(m,t+16)==PARTICLE
   assert load(m,w+0x18)==(rnd//1024)<<11
   assert load(m,w+0x14)==65536-(rnd*3//8)
   assert load(m,w+3,1)==(rnd//512)&1
   assert [load(m,w+off) for off in (0x20,0x24,0x28)]==[load(m,parent_work+off) for off in (0x20,0x24,0x28)]
 if entry in ('callback','transform'):
  assert load(m,parent_work,1)==1
  if c.get('descriptor',True):
   assert load(m,0x8C88F69C)==1 and load(m,0x8C88F5F8)==STACK
   ax,ay,az=(a*(2*math.pi/65536) for a in c.get('angles',(0,0,0)))
   # Matrices compose Z then X then -Y; apply them to the local up vector.
   vx,vy,vz=0.0,5.0,0.0
   vx,vz=math.cos(-ay)*vx+math.sin(-ay)*vz,-math.sin(-ay)*vx+math.cos(-ay)*vz
   vy,vz=math.cos(ax)*vy-math.sin(ax)*vz,math.sin(ax)*vy+math.cos(ax)*vz
   vx,vy=math.cos(az)*vx-math.sin(az)*vy,math.sin(az)*vx+math.cos(az)*vy
   expected=[value(raw)+delta for raw,delta in zip(c.get('base',(bits(1),bits(2),bits(3))),(vx,vy,vz))]
   assert all(abs(value(load(m,parent_work+off))-x)<0.001 for off,x in zip((0x20,0x24,0x28),expected)),(c,'analytic rotated vector')
 return len(nodes)


def cases():
 for entry,mode,pool,descriptor in itertools.product(('callback','transform','emit'),range(8),(0,3),(False,True)):
  yield dict(entry=entry,mode=mode<<19,pool=pool,descriptor=descriptor)
 for entry,mask,bank,rm in itertools.product(('callback','transform','emit'),range(8),range(2),range(2)):
  yield dict(entry=entry,angles=tuple((0x4000 if mask&(1<<i) else 0) for i in range(3)),mode=bank<<21,rm=rm)
 for entry,pool,heap,bank in itertools.product(('emit','callback'),range(8),(0,16,64,80,160,240,480,560),range(2)):
  yield dict(entry=entry,pool=pool,heap=heap,mode=bank<<21,descriptor=False)
 for entry,char,seed,bank,rm in itertools.product(('callback','emit'),(0,127,128,255),(0,1,0xFFFFFFFF,0x80000000),range(2),range(2)):
  yield dict(entry=entry,character=char,seed=seed,mode=bank<<21,rm=rm)
 for entry,enable,bank in itertools.product(('transform','emit'),(1,2,4,8,16,32),range(2)):
  yield dict(entry=entry,enable=enable,mode=bank<<21,angles=(1,2,3))
 for entry,rm,bank in itertools.product(('transform','emit'),(2,3),range(2)):
  yield dict(entry=entry,rm=rm,mode=bank<<21)
 for bank,enable in itertools.product(range(2),(0,8)):
  yield dict(entry='emit',mode=0x100000|(bank<<21),wide_pair=True,descriptor=False,enable=enable)


def main():
 manifest_path,manifest,baseline,dependencies,allowed=setup()
 records=[];rows=set();edges=set();calls=set();counts=dict.fromkeys(SPECS,0)
 new_rows={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 totals=dict(boundary_comparisons=0,numerical_operations=0,partial_guard_cases=0,independent_invariant_cases=0,children_checked=0)
 def check(c,n):
  m,s,f=prepare(c,n);path,status,b,ops,_=execute(c,m,s,f,c['entry'],0xFEED1234,allowed)
  rows.update(set(path)&new_rows);counts[c['entry']]+=1
  for i,a in enumerate(path):
   if a not in new_rows:continue
   w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
   if w>>8 in (0x89,0x8B,0x8D,0x8F):
    nxt=i+(2 if w>>8 in (0x8D,0x8F) else 1)
    if nxt<len(path):edges.add((a,path[nxt]))
   if w>>12==0xB or w&0xF0FF in (0x400B,0x402B):calls.add(a)
  totals['boundary_comparisons']+=b;totals['numerical_operations']+=ops;totals['partial_guard_cases']+=status!=0
  checked=not status
  if checked:
   totals['children_checked']+=independent(c,m,WORK,c['entry']);totals['independent_invariant_cases']+=1
   if c['entry']=='callback':assert load(m,PARENT+16)==previous.DELETE
  records.append(dict(input=c,steps=len(path),status=status,boundary_comparisons=b,independent_invariants=checked,full_state_match=True))
 if '--smoke' in sys.argv:
  smoke=[dict(entry='callback',descriptor=False,pool=0),dict(entry='transform'),dict(entry='callback'),
         dict(entry='emit'),dict(entry='callback',angles=(0x4000,0,0)),
         dict(entry='transform',mode=0x80000),dict(entry='emit',mode=0x80000,pool=0),
         dict(entry='emit',mode=0x100000,wide_pair=True,descriptor=False)]
  for n,c in enumerate(smoke):check(c,n)
  print('Spawn callback smoke checks passed:',len(records));return
 for n,c in enumerate(cases()):
  check(c,n)
  if (n+1)%100==0:print(n+1,'spawn callback/transform/emission comparisons passed',flush=True)
  if (n+1)%25==0:gc.collect()
 assert rows==new_rows,('missing rows',[hex(a) for a in new_rows-rows])
 assert edges=={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']},('missing edges',edges)
 assert calls=={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']}
 files=[Path(__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_spawn_callback.ps1',PROJECT/'source/sa_main_dispatch_spawn_callback_connected.c',PROJECT/manifest['source']]
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 for dep in dependencies:files.extend((PROJECT/dep['source'],PROJECT/dep['verification']))
 for name in ('sh4_add_sub.c','sh4_float_i32.c','sh4_truncate_i32.c','sh4_vector_numeric.c','sh4_compare_bits.h'):
  files.append(PROJECT/'source'/name)
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],
             cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),
             **totals,external_game_service_calls=0,sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),
             binary_sha256=digest(base.previous.BINARY),native_dependencies=dependencies,
             scope='Three original bodies with native matrices/rotations/vector arithmetic/RNG/task allocation/removal request, retail full-state and ordered boundary comparisons plus independent LCG/particle field/zero-angle transform checks.',
             limitations=['Vector results use the established pinned upstream primitive oracle: numerical integration with shared math, not new physical-hardware validation. Scalar guard/trap classification uses existing native helper probes; ordinary scalar results use the Python reference.',
                          'Legacy matrix entry PR/SZ guards and reached scalar precision/rounding limits retained. Wide-pair SZ=1 fixtures cover conversion fallback code; not a claim of normal gameplay reachability.',
                          'Generated particle callback 8c04481a is stored but not executed here. Constructed valid RAM/tables/heap; full gameplay, hardware/MMIO and ports remain unfinished.'],cases=records)
 (PROJECT/'analysis/main_dispatch_spawn_callback_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered',*totals,'external_game_service_calls')},indent=2))


if __name__=='__main__':
 try:main()
 finally:
  sh4_binary32.execute_float=original_float;sh4_binary32.execute_add_sub=original_add
  sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
