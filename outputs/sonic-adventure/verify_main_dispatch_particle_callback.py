"""Particle state/update and native draw/state wrappers versus original code."""
import ctypes,gc,hashlib,inspect,itertools,json,math,sys
from pathlib import Path
import verify_main_dispatch_spawn_callback as current
from verify_tikal_controller import Call

PROJECT=Path(__file__).resolve().parent;base=current.base
PRIMARY,SECONDARY,WORK_ENTRY=0x8C04481A,0x8C0448B4,0x8C0448BA
DRAW,SAVE,RESTORE,TRANSLATE,PREDICATE,GEOMETRY=0x8C6215D6,0x8C605DAC,0x8C605E0A,0x8C63A804,0x8C04FE40,0x8C620F2C
literal=base.literal;load,store=current.load,current.store
GAME_MODE=literal(0x8C04FE8C)
STATE_A_SLOT,STATE_B_SLOT,STATE_COPY,STATE_FLAGS=(literal(a) for a in (0x8C605EB8,0x8C605EC0,0x8C605EBC,0x8C605EC4))
STATE_A,STATE_B,CAMERA,MATRIX=0x8CC30000,0x8CC31000,0x8CC20000,0x8CC40000
MODEL_A,MODEL_B=literal(0x8C0449FC),literal(0x8C044A00)
SPECS=dict(current.SPECS,primary=(PRIMARY,'sa_particle_primary_connected'),secondary=(SECONDARY,'sa_particle_secondary_connected'),
           work=(WORK_ENTRY,'sa_particle_work_connected'),draw=(DRAW,'sa_particle_draw_connected'),
           save=(SAVE,'sa_particle_state_save_connected'),restore=(RESTORE,'sa_particle_state_restore_connected'),
           translate=(TRANSLATE,'sa_particle_translation_connected'))
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_particle_callback.dll';LIB=ctypes.CDLL(str(DLL))
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE];fn.restype=ctypes.c_int
for name in ('sa_sh4_add_sub','sa_sh4_multiply_extended','sa_sh4_float_i32','sa_sh4_truncate_i32'):
 fn=getattr(LIB,name);fn.argtypes=getattr(current.LIB,name).argtypes;fn.restype=ctypes.c_int
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def geometry_effect(c,mem,r,sr,fp):
 assert r[4] in (MODEL_A,MODEL_B),hex(r[4])
 if c.get('expected_model') is not None:assert r[4]==c['expected_model']
 for i in range(4):store(mem,STATE_A+0x90+i*4,0xD00D0000+i)
 for i in range(5):store(mem,STATE_B+i*4,0xFACE0000+i)
 if c.get('flip_swap'):store(mem,STATE_FLAGS,load(mem,STATE_FLAGS)^0x4000)
 if 'after_mode' in c:store(mem,GAME_MODE,c['after_mode'],2)
 if c.get('flip_bank'):fp.fpscr^=0x200000
 for i in range(8):r[i]=0xBAD00000+i*0x101
 r[8]=0x8BADF00D # The original wrapper explicitly saves/restores R8.
 return sr^1


def setup():
 *_,allowed=current.setup()
 manifest_path=PROJECT/'analysis/main_dispatch_particle_callback_translation.json'
 manifest=json.loads(manifest_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_spawn_callback_verification.json'
 for report in (manifest,json.loads(baseline.read_text())):
  for p,h in report['sha256'].items():assert digest(Path(p))==h,p
 for routine in manifest['routines']:allowed.update(int(a,16) for a in routine['reachable_addresses'])
 allowed.update(range(TRANSLATE,0x8C63A874,2));allowed.update(range(PREDICATE,0x8C04FE4A,2))
 base.TRACKED|={PRIMARY,SECONDARY,DRAW,SAVE,RESTORE,TRANSLATE,PREDICATE}
 # Reuse the proven executor locally, adding the one explicit renderer service.
 # The saved executor and earlier verification reports are not edited.
 source=inspect.getsource(current.execute)
 changes={
  'load(mem,base.LEVELS+20)':'load(mem,base.LEVELS+20),tuple(load(mem,STATE_COPY+i*4) for i in range(9))',
  '(0xF08D,0xF04D,0xF05D)':'(0xF08D,0xF09D,0xF04D,0xF05D)',
  ' try:base.previous.reference(em,expected,fpu=ef,special=special,trace=trace,vector_instruction=vector,instruction_budget=40000)':
  ''' def geometry_hook(mem,r,sr):
  wanted.append(event(GEOMETRY,mem,r,sr,expected.pr,ef,ef.fpul,special['macl']))
  if c.get('null_geometry'):stop(GEOMETRY,r,sr,expected.pr,-2)
  sr=geometry_effect(c,mem,r,sr,ef)
  if c.get('fail_geometry'):stop(GEOMETRY,r,sr,expected.pr,-17)
  return sr
 try:base.previous.reference(em,expected,fpu=ef,special=special,trace=trace,hooks={GEOMETRY:geometry_hook},vector_instruction=vector,instruction_budget=40000)''',
  ' @base.OBSERVE\n def observe':
  ''' @Call
 def geometry_call(_ctx,target,mem,fp,ss,r,p):
  try:
   assert target==GEOMETRY,hex(target)
   ss[0]=geometry_effect(c,mem,r,ss[0],fp.contents)
   return -17 if c.get('fail_geometry') else 0
  except Exception as exc:errors.append(repr(exc));return -99
 @base.OBSERVE
 def observe''',
  'None,Call(),observe)':'None,Call() if c.get("null_geometry") else geometry_call,observe)',
  'return path,status,len(observed),ops,ml.value':'return path,status,len(observed),ops,ml.value,(0 if c.get("null_geometry") else sum(e[0]==GEOMETRY for e in observed))',
 }
 for old,new in changes.items():assert source.count(old)==1,(old,source.count(old));source=source.replace(old,new)
 namespace=dict(current.__dict__)
 namespace.update(LIB=LIB,FUNCTIONS=FUNCTIONS,SPECS=SPECS,LEGACY=current.LEGACY|{TRANSLATE},
                  STATE_COPY=STATE_COPY,GEOMETRY=GEOMETRY,geometry_effect=geometry_effect)
 exec(compile(source,'particle_observed_execute','exec'),namespace)
 return manifest_path,manifest,baseline,allowed,namespace['execute']


def prepare(c,n):
 m,s,f=current.prepare(dict(c,entry='callback'),n)
 store(m,current.WORK,c.get('state',1),1);store(m,current.WORK+3,c.get('variant',0),1)
 store(m,current.WORK+6,c.get('timer',0),2)
 store(m,current.WORK+0x14,c.get('x',0));store(m,current.WORK+0x18,c.get('y',0));store(m,current.WORK+0x1C,c.get('spin',0))
 store(m,current.PARENT+16,PRIMARY);store(m,current.PARENT+20,0xBAD03CB)
 store(m,GAME_MODE,c.get('game_mode',0),2)
 store(m,literal(0x8C0449F0),CAMERA);store(m,CAMERA+0x18,c.get('camera_y',0))
 store(m,STATE_A_SLOT,STATE_A);store(m,STATE_B_SLOT,STATE_B)
 store(m,STATE_FLAGS,0xA0000000|(0x4000 if c.get('swap') else 0))
 for i in range(4):store(m,STATE_A+0x90+i*4,0x12340000+i*0x101)
 for i in range(5):store(m,STATE_B+i*4,0x56780000+i*0x102)
 for i in range(9):store(m,STATE_COPY+i*4,0xCAFE0000+i*0x103)
 entry=c['entry']
 s.r[4]=current.PARENT if entry in ('primary','secondary') else MODEL_A if c.get('variant',0)&1 else MODEL_B if entry=='draw' else current.WORK
 if entry=='draw':s.r[4]=MODEL_A if c.get('variant',0)&1 else MODEL_B
 elif entry=='work':s.r[4]=current.WORK
 elif entry=='translate':
  for i in range(16):store(m,MATRIX+i*4,current.bits(1) if i%5==0 else 0)
  s.r[4]=MATRIX if c.get('explicit') else 0;s.r[5]=current.WORK+0x20
 return m,s,f


def independent(c,m,before,entry):
 w=current.WORK;initial_state=c.get('state',1)&255;timer=c.get('timer',0)&0xFFFF
 gm=c.get('game_mode',0);updated=entry in ('secondary','work') or entry=='primary' and initial_state==1
 expired=updated and gm!=16 and timer>=10
 drawn=entry=='draw' or updated and not expired
 if entry=='primary' and initial_state==0:
  assert load(m,w,1)==1 and load(m,current.PARENT+20)==SECONDARY
 elif entry=='primary' and initial_state==2:assert load(m,current.PARENT+16)==current.previous.DELETE
 if updated:
  assert load(m,w+6,2)==((timer+1)&0xFFFF if gm!=16 else timer)
  assert load(m,w,1)==(2 if expired else initial_state)
  original_position=c.get('position',(current.bits(1.25),current.bits(-2),current.bits(0.75)))
  after=c.get('after_mode',gm) if drawn else gm
  if not expired and after!=16:
   x,y=(c.get(k,0)*(2*math.pi/65536) for k in ('x','y'))
   delta=(2*math.cos(y),2*math.sin(x)*math.sin(y),-2*math.cos(x)*math.sin(y))
   assert all(abs(current.value(load(m,w+off))-(current.value(raw)+d))<0.001 for off,raw,d in zip((0x20,0x24,0x28),original_position,delta)),(c,'analytic movement')
   assert load(m,w+0x1C)==(c.get('spin',0)+0x1000)&0xFFFFFFFF
  else:
   assert [load(m,w+off) for off in (0x20,0x24,0x28)]==list(original_position)
   assert load(m,w+0x1C)==c.get('spin',0)&0xFFFFFFFF
 if drawn:
  assert [load(m,STATE_A+0x90+i*4) for i in range(4)]==[0x12340000+i*0x101 for i in range(4)]
  expected=[0x56780000+i*0x102 for i in range(5)]
  if c.get('flip_swap'):expected[2],expected[4]=expected[4],expected[2]
  assert [load(m,STATE_B+i*4) for i in range(5)]==expected,(c,'render state restoration')
 if entry=='save':
  expected=[0x12340000+i*0x101 for i in range(4)]+[0x56780000,0,0,0x56780102,0x56780306]
  expected[5:7]=[0x56780408,0x56780204] if c.get('swap') else [0x56780204,0x56780408]
  assert [load(m,STATE_COPY+i*4) for i in range(9)]==expected
 if entry=='restore':
  expected=[0xCAFE0000+i*0x103 for i in range(9)]
  assert [load(m,STATE_A+0x90+i*4) for i in range(4)]==expected[:4]
  result=[expected[4],expected[7],expected[6] if c.get('swap') else expected[5],expected[8],expected[5] if c.get('swap') else expected[6]]
  assert [load(m,STATE_B+i*4) for i in range(5)]==result
 return drawn


def cases():
 for state,mode,gm in itertools.product((0,1,2,3,127,128,255),range(8),(0,16)):
  yield dict(entry='primary',state=state,mode=mode<<19,game_mode=gm)
 for entry,timer,gm,bank in itertools.product(('primary','secondary','work'),(0,9,10,11,32767,32768,65535),(0,16),range(2)):
  yield dict(entry=entry,timer=timer,game_mode=gm,mode=bank<<21)
 for entry,mask,bank,rm in itertools.product(('primary','secondary','work'),range(8),range(2),range(2)):
  yield dict(entry=entry,x=0x4000 if mask&1 else 0,y=0x4000 if mask&2 else 0,spin=0x4000 if mask&4 else 0,
             camera_y=0x4000,variant=mask,swap=bool(mask&1),mode=bank<<21,rm=rm)
 for swap,mode,flip in itertools.product((False,True),range(8),(False,True)):
  yield dict(entry='draw',swap=swap,mode=mode<<19,flip_swap=flip,variant=mode&1)
 for entry,swap,mode in itertools.product(('save','restore'),(False,True),range(8)):
  yield dict(entry=entry,swap=swap,mode=mode<<19,null_geometry=True)
 for explicit,mode,rm in itertools.product((False,True),range(8),range(2)):
  yield dict(entry='translate',explicit=explicit,mode=mode<<19,rm=rm)
 for entry,gm,after,bank in itertools.product(('primary','secondary','work'),(0,16),(0,16),range(2)):
  yield dict(entry=entry,game_mode=gm,after_mode=after,mode=bank<<21,flip_swap=True)
 for entry,mode,failure in itertools.product(('primary','secondary','draw'),(0,0x200000),('missing','error')):
  yield dict(entry=entry,mode=mode,null_geometry=failure=='missing',fail_geometry=failure=='error')
 for state in (0,2,255):yield dict(entry='primary',state=state,null_geometry=True)
 for entry,bank,enable in itertools.product(('primary','work','translate'),range(2),(1,2,4,8,16,32)):
  yield dict(entry=entry,mode=bank<<21,enable=enable)
 for entry,rm in itertools.product(('work','translate'),(2,3)):
  yield dict(entry=entry,rm=rm)
 for spin,bank in itertools.product((0x7FFFF000,0xFFFFF000),range(2)):
  yield dict(entry='work',spin=spin,mode=bank<<21,flip_bank=True)


def main():
 manifest_path,manifest,baseline,allowed,execute=setup()
 records=[];rows=set();edges=set();calls=set();counts=dict.fromkeys(SPECS,0)
 new_rows={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']}
 totals=dict(boundary_comparisons=0,numerical_operations=0,partial_guard_error_cases=0,independent_invariant_cases=0,geometry_service_invocations=0)
 def check(c,n):
  c=dict(c,expected_model=MODEL_A if c.get('variant',0)&1 else MODEL_B)
  m,s,f=prepare(c,n);before=bytes(m)
  path,status,b,ops,_,geometry_calls=execute(c,m,s,f,c['entry'],0xFEED1234,allowed)
  rows.update(set(path)&new_rows);counts[c['entry']]+=1
  for i,a in enumerate(path):
   if a not in new_rows:continue
   w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
   if w>>8 in (0x89,0x8B,0x8D,0x8F):
    nxt=i+(2 if w>>8 in (0x8D,0x8F) else 1)
    if nxt<len(path):edges.add((a,path[nxt]))
   if w>>12 in (0xA,0xB) or w&0xF0FF in (0x400B,0x402B):
    if a in {int(x,16) for r in manifest['routines'] for x in r['external_call_sites']}:calls.add(a)
  totals['boundary_comparisons']+=b;totals['numerical_operations']+=ops;totals['partial_guard_error_cases']+=status!=0
  totals['geometry_service_invocations']+=geometry_calls
  if status==0:independent(c,m,before,c['entry']);totals['independent_invariant_cases']+=1
  records.append(dict(input=c,steps=len(path),status=status,boundary_comparisons=b,full_state_match=True,independent_invariants=status==0))
 if '--smoke' in sys.argv:
  smoke=[dict(entry='primary',state=s) for s in (0,1,2,255)]+[dict(entry='secondary'),dict(entry='work',timer=10),
         dict(entry='work',game_mode=16,timer=65535),dict(entry='draw',flip_swap=True),dict(entry='save',swap=True),
         dict(entry='restore'),dict(entry='translate',explicit=True),dict(entry='primary',null_geometry=True)]
  for n,c in enumerate(smoke):check(c,n)
  print('Particle callback smoke checks passed:',len(records));return
 for n,c in enumerate(cases()):
  check(c,n)
  if (n+1)%100==0:print(n+1,'particle/update/draw/state comparisons passed',flush=True)
  if (n+1)%25==0:gc.collect()
 assert rows==new_rows,('uncovered rows',[hex(a) for a in new_rows-rows])
 assert edges=={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert calls=={int(a,16) for r in manifest['routines'] for a in r['external_call_sites']},calls
 files=[Path(__file__),Path(current.__file__),manifest_path,baseline,PROJECT/'build_main_dispatch_particle_callback.ps1',
        PROJECT/manifest['source'],PROJECT/'source/sa_main_dispatch_particle_callback_connected.c',
        PROJECT/'source/model_matrix_translate.c',PROJECT/'source/sa_global_mode_predicates_8c04fe40.c']
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],
             cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),
             **totals,sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),
             scope='Five particle/draw/state entries with one shared tail compiled once; native matrix translation and other existing dependencies, original full-state/ordered boundary comparisons, independent lifetime/motion/model-choice/render-state assertions.',
             limitations=['Only low-level geometry submission 8c620f2c remains an explicit fixture with register/state mutations and missing/error cases; no real geometry rendering, pixels, hardware or MMIO claimed.',
                          'Shared vector numerical oracle and scalar guard-probe limits inherited from the spawn-callback verification. Matrix translation retains entry PR/SZ guards.',
                          'Constructed valid global/work/render-state data; full frame ordering, concurrency, live gameplay and playable ports remain unfinished.'],cases=records)
 (PROJECT/'analysis/main_dispatch_particle_callback_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered',*totals)},indent=2))


if __name__=='__main__':
 try:main()
 finally:
  current.sh4_binary32.execute_float=current.original_float;current.sh4_binary32.execute_add_sub=current.original_add
  current.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
