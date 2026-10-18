"""Execute created context callbacks through native scheduling and later deletion."""
import ctypes,gc,hashlib,itertools,json,sys
from pathlib import Path
import verify_main_dispatch_cancellation as previous
from verify_startup import State
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call

PROJECT=Path(__file__).resolve().parent;base=previous.base
CALLBACK,REMOVE,SCHEDULE,DELETE=0x8C01B358,0x8C098786,0x8C09878C,0x8C09865C
ACTIVE=base.literal(0x8C0987EC)
SPARE=0x8CE50300
load,store=base.load,base.store
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_context_callback.dll';LIB=ctypes.CDLL(str(DLL))
SPECS={'cancel':(previous.CANCEL,'sa_main_dispatch_cancellation_connected'),
       'restore':(previous.RESTORE,'sa_main_dispatch_context_restore_connected'),
       'gate':(base.previous.START,'sa_main_dispatch_notified_prefix_gate_connected'),
       'callback':(CALLBACK,'sa_main_dispatch_context_callback_connected'),
       'schedule':(SCHEDULE,'sa_task_level_update_connected')}
FUNCTIONS={k:getattr(LIB,name) for k,(_,name) in SPECS.items()}
for fn in FUNCTIONS.values():fn.argtypes=base.previous.ARGS+[base.OBSERVE];fn.restype=ctypes.c_int
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def setup():
 *_,allowed=previous.setup()
 inventory_path=PROJECT/'analysis/main_dispatch_context_callback_translation.json'
 inventory=json.loads(inventory_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_cancellation_verification.json'
 for report in (inventory,json.loads(baseline.read_text())):
  for p,h in report['sha256'].items():assert digest(Path(p))==h,p
 for routine in inventory['routines']+inventory['existing_dependencies']:
  allowed.update(int(a,16) for a in routine['reachable_addresses'])
 base.TRACKED|={CALLBACK,REMOVE,SCHEDULE}
 return inventory_path,inventory,baseline,allowed


def execute(entry,m,state,fpu,macl,allowed):
 state.pc=SPECS[entry][0];initial=State.from_buffer_copy(state)
 em=bytearray(m);expected=State.from_buffer_copy(initial);ef=Fpu.from_buffer_copy(fpu)
 special={'macl':macl};path=[];wanted=[];observed=[];errors=[]
 def event(t,mem,r,sr,pr,f,fpul,ml):
  task=tuple(load(mem,r[4]+i*4) for i in range(12)) if t in (CALLBACK,REMOVE,DELETE) else ()
  return t,tuple(r),sr,pr,bytes(f)[:132],fpul,ml,load(mem,ACTIVE),task
 def trace(pc,w,r,sr,pr,_f):
  assert pc in allowed,hex(pc);path.append(pc)
  if pc in base.TRACKED and len(path)>1:wanted.append(event(pc,em,r,sr,pr,ef,ef.fpul,special['macl']))
 base.previous.reference(em,expected,fpu=ef,special=special,trace=trace,instruction_budget=20000)
 sr=ctypes.c_uint32(initial.sr);pr=ctypes.c_uint32(initial.pr)
 fpul=ctypes.c_uint32(fpu.fpul);ml=ctypes.c_uint32(macl)
 @Call
 def external(_ctx,t,mem,f,s,r,p):errors.append(hex(t));return -99
 @base.OBSERVE
 def observe(_ctx,t,mem,f,s,r,p):
  try:observed.append(event(t,mem,r[:16],s[0],p[0],f.contents,fpul.value,ml.value))
  except Exception as exc:errors.append(repr(exc))
 rc=FUNCTIONS[entry]((ctypes.c_uint8*base.previous.RAM_SIZE).from_buffer(m),ctypes.byref(fpu),ctypes.byref(sr),state.r,
                     ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(ml),None,Call(),observe)
 assert rc==0 and not errors,(entry,rc,errors)
 assert m==em and list(state.r)==list(expected.r),(entry,'RAM/GPR differs')
 assert sr.value==expected.sr and pr.value==expected.pr,(entry,'SR/PR differs')
 assert bytes(fpu)[:132]==bytes(ef)[:132] and fpul.value==ef.fpul and ml.value==special['macl'],(entry,'FPU/FPUL/MACL differs')
 assert observed==wanted,(entry,'ordered CPU/RAM boundaries differ')
 assert state.r[15]==initial.r[15] and pr.value==initial.pr
 assert list(state.r)[8:15]==list(initial.r)[8:15]
 state.sr=sr.value;state.pr=pr.value;fpu.fpul=fpul.value
 previous.previous.LAST['registers']=list(state.r)
 return path,len(observed),ml.value


def chain(m,head,slot=0):
 result=[]
 while head:
  assert head not in result and len(result)<8,('invalid list',result,hex(head))
  result.append(head);head=load(m,head+slot)
 return result


def cycles():
 for entry,heap,pool,resume,mode,old in itertools.product(('cancel','restore'),(0,16,32,64,96),range(4),(False,True),range(8),(False,True)):
  yield dict(entry=entry,heap=heap,pool_count=pool,restored_mode=0 if resume else 1,
             resume_id=1,mode=mode<<19,old_head=old,null_external=True)
 for state,mode in itertools.product((0,44),(0,4)):
  yield dict(entry='gate',state=state,available=0,heap=256,pool_count=3,restored_mode=0,resume_id=1,mode=mode<<19)


def schedulers():
 patterns=((CALLBACK,CALLBACK,CALLBACK),(0,CALLBACK,0),(DELETE,CALLBACK,DELETE),(CALLBACK,DELETE,CALLBACK))
 for count,pattern,level,mode in itertools.product(range(4),patterns,(0,6,7,8),range(8)):
  yield dict(count=count,pattern=list(pattern),level=level,mode=mode<<19,free_head=bool(mode&1))


def main():
 inventory_path,inventory,baseline,allowed=setup()
 records=[];rows=set();edges=set();calls=set();boundaries=0;counts=dict.fromkeys(SPECS,0)
 new_rows={int(a,16) for r in inventory['routines'] for a in r['reachable_addresses']}
 reused_rows={int(a,16) for r in inventory['existing_dependencies'] for a in r['reachable_addresses']}
 all_rows=new_rows|reused_rows
 def run(entry,m,s,f,macl,case,stage):
  nonlocal boundaries
  path,b,macl=execute(entry,m,s,f,macl,allowed);rows.update(set(path)&all_rows);boundaries+=b;counts[entry]+=1
  for i,a in enumerate(path):
   if a not in all_rows:continue
   w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
   if w>>8 in (0x89,0x8B,0x8D,0x8F):
    nxt=i+(2 if w>>8 in (0x8D,0x8F) else 1)
    if nxt<len(path):edges.add((a,path[nxt]))
   if w>>12==0xB or w&0xF0FF in (0x400B,0x402B):calls.add(a)
  records.append(dict(case=case,stage=stage,entry=entry,steps=len(path),boundary_comparisons=b,full_state_match=True))
  return path,macl
 smoke='--smoke' in sys.argv
 # Direct thunk: full RAM may change only at the requested callback word.
 direct_cases=list(itertools.product((0,1,2,3),range(8)))
 if smoke:direct_cases=direct_cases[:2]
 for n,(variant,mode) in enumerate(direct_cases):
  m,s,f=previous.prepare(dict(entry='cancel',mode=mode<<19),n);s.r[4]=previous.NODES[variant%3]
  store(m,s.r[4]+16,(0,CALLBACK,DELETE,0xFFFFFFFF)[variant]);before=bytearray(m)
  store(before,s.r[4]+16,DELETE)
  _,_=run('callback',m,s,f,0xFEED1234,dict(kind='direct',variant=variant,mode=mode<<19),'request')
  assert m==before and s.r[3]==DELETE
 # Actual native creator followed by two scheduler passes over the same state.
 lifecycle_cases=list(cycles())
 if smoke:lifecycle_cases=[lifecycle_cases[i] for i in (0,255,511,767,1023,1279)]+lifecycle_cases[-1:]
 lifecycle_total=0;created_total=0
 for n,c in enumerate(lifecycle_cases):
  m,s,f=previous.prepare(c,n);macl=0xFEED1234
  path,macl=run(c['entry'],m,s,f,macl,c,'create')
  previous.independent(c,path,0)
  head=load(m,base.LEVELS+24);live=chain(m,head)
  created=[t for t in live if t!=previous.previous.OLDHEAD]
  assert all(load(m,t+16)==CALLBACK for t in created)
  created_total+=len(created);free_before=load(m,base.FREE_TASK)
  heap_before=bytes(m[base.ARENA&0xFFFFFF:(base.ARENA&0xFFFFFF)+c.get('heap',128)+16])
  store(m,ACTIVE,0xA5A55A5A);s.r[4]=6
  _,macl=run('schedule',m,s,f,macl,c,'mark_for_deletion')
  assert chain(m,load(m,base.LEVELS+24))==live
  assert all(load(m,t+16)==DELETE for t in created)
  assert load(m,base.FREE_TASK)==free_before
  assert bytes(m[base.ARENA&0xFFFFFF:(base.ARENA&0xFFFFFF)+c.get('heap',128)+16])==heap_before
  assert load(m,ACTIVE)==(0 if live else 0xA5A55A5A)
  s.r[4]=6;_,macl=run('schedule',m,s,f,macl,c,'delete_and_release')
  assert chain(m,load(m,base.LEVELS+24))==([previous.previous.OLDHEAD] if c.get('old_head') else [])
  if c['entry']!='gate':
   assert chain(m,load(m,base.FREE_TASK))==list(previous.NODES[:c.get('pool_count',2)])
   size=c.get('heap',128)
   assert load(m,base.FREE_HEAP)==(base.ARENA if size else 0)
   if size:
    assert [load(m,base.ARENA+i*4) for i in range(4)]==[base.ARENA+size,0,base.ARENA+size,0]
  lifecycle_total+=1
  if (n+1)%100==0:print(n+1,'created callback lifecycles passed',flush=True)
  if (n+1)%25==0:gc.collect()
 # Scheduler branches, null callbacks, level-8 remap, and saved-next deletion.
 scheduler_cases=list(schedulers())
 if smoke:scheduler_cases=[scheduler_cases[i] for i in (0,128,256,384,511)]
 for n,c in enumerate(scheduler_cases):
  m,s,f=previous.prepare(dict(entry='cancel',mode=c['mode']),n)
  for level in range(8):store(m,base.LEVELS+level*4,0)
  nodes=list(previous.NODES[:c['count']]);effective=7 if c['level']==8 else c['level']
  for i,t in enumerate(nodes):
   for off in range(0,48,4):store(m,t+off,0)
   store(m,t,nodes[i+1] if i+1<len(nodes) else 0);store(m,t+4,nodes[i-1] if i else 0)
   store(m,t+16,c['pattern'][i])
  store(m,base.LEVELS+effective*4,nodes[0] if nodes else 0)
  store(m,SPARE,0);store(m,SPARE+4,0);store(m,base.FREE_TASK,SPARE if c['free_head'] else 0)
  store(m,ACTIVE,0xA5A55A5A);macl=0xFEED1234
  alive=nodes[:];free=[SPARE] if c['free_head'] else []
  callbacks=dict(zip(nodes,c['pattern']));diagnostic=0xA5A55A5A
  for pass_ in range(2):
   had_tasks=bool(alive);s.r[4]=c['level']
   _,macl=run('schedule',m,s,f,macl,c,f'constructed_pass_{pass_+1}')
   for t in alive[:]:
    if callbacks[t]==DELETE:alive.remove(t);free.insert(0,t)
    elif callbacks[t]==CALLBACK:callbacks[t]=DELETE
   assert chain(m,load(m,base.LEVELS+effective*4))==alive
   assert chain(m,load(m,base.FREE_TASK))==free
   assert all(load(m,t+16)==callbacks[t] for t in alive)
   if had_tasks:diagnostic=0
   assert load(m,ACTIVE)==diagnostic
  if (n+1)%128==0:print(n+1,'constructed native scheduler lifecycles passed',flush=True)
 if smoke:print('Context callback smoke verification passed:',len(records),'stage comparisons');return
 assert rows==all_rows,('uncovered rows',[hex(a) for a in all_rows-rows])
 assert edges=={tuple(int(a,16) for a in e) for r in inventory['routines']+inventory['existing_dependencies'] for e in r['conditional_edges']}
 assert calls=={int(a,16) for r in inventory['routines']+inventory['existing_dependencies'] for a in r['external_call_sites']}
 files=[Path(__file__),Path(previous.__file__),inventory_path,baseline,PROJECT/'build_main_dispatch_context_callback.ps1',
        PROJECT/'source/sa_main_dispatch_context_callback_connected.c',PROJECT/inventory['source']]
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 files.extend(Path(p) for p in inventory['sha256'])
 report=dict(source=inventory['source'],source_sha256=digest(PROJECT/inventory['source']),routines=inventory['routines'],
             existing_dependencies=inventory['existing_dependencies'],stage_comparisons=len(records),counts=counts,
             direct_callback_cases=len(direct_cases),created_lifecycles=lifecycle_total,created_tasks_executed=created_total,
             constructed_scheduler_lifecycles=len(scheduler_cases),new_rows_covered=len(rows&new_rows),
             reused_rows_covered=len(rows&reused_rows),conditional_edges_covered=len(edges),call_sites_covered=len(calls),
             boundary_comparisons=boundaries,external_game_service_calls=0,
             sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),
             scope='Original creation -> scheduler callback -> deferred deletion on the same RAM/CPU/FPU state, plus mixed/null/deleting lists; complete RAM/register/special state and ordered CPU/task-RAM/diagnostic boundaries, with independent callback/list/free-pool/heap restoration invariants.',
             limitations=['Constructed context/table/pool fixtures and host orchestration between separate native scheduler passes; no frame timer, full task-update driver, live gameplay or hardware proof.',
                          'New callback thunk plus existing removal-request and scheduler bodies; no extra original-function credit for reused bodies.',
                          'Earlier spawn callback 8c044680 and dynamic setter table targets remain unfinished. Full decompilation and ports are incomplete.'],stages=records)
 (PROJECT/'analysis/main_dispatch_context_callback_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('stage_comparisons','counts','direct_callback_cases','created_lifecycles','created_tasks_executed',
                   'constructed_scheduler_lifecycles','new_rows_covered','reused_rows_covered','conditional_edges_covered','call_sites_covered','boundary_comparisons','external_game_service_calls')},indent=2))


if __name__=='__main__':
 try:main()
 finally:previous.sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
