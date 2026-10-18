"""Cancellation/restore and connected prefix versus original SH-4 execution."""
import ctypes,gc,hashlib,itertools,json,sys
from pathlib import Path
import verify_main_dispatch_spawn_setter as previous
import sh4_binary32

PROJECT=Path(__file__).resolve().parent
base=previous.base
DLL=base.ROOT/'work/sonic-adventure/build/main_dispatch_cancellation.dll'
LIB=ctypes.CDLL(str(DLL))
CANCEL,RESTORE=0x8C01AE60,0x8C01AAE0
literal=base.literal;load,store=base.load,base.store
CONTEXT_SLOT,PEER_SLOT=literal(0x8C01B000),literal(0x8C01B008)
CHAR_SLOT,OLD_SLOT,SELECT_SLOT=literal(0x8C01B004),literal(0x8C01AC74),literal(0x8C01AC78)
RESUME_ID,RESUME_PARAM,FLAG=literal(0x8C01B014),literal(0x8C01B018),literal(0x8C01B01C)
TASK_CALLBACK=literal(0x8C01B00C)
CONTEXT,PEER=0x8CEC0000,0x8CEE1000
CHAR_TABLE,OLD_TABLE,SELECT_TABLE=0x8CB00000,0x8C700000,0x8CC00000
THIRD_TASK=0x8CE50200
NODES=(base.TASK,base.NEXTTASK,THIRD_TASK)
LAST={}

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def signed(v,b):return (v&((1<<b)-1))-(1<<b) if v&(1<<(b-1)) else v&((1<<b)-1)
def char_byte(index):return ((index*37)^0xA7)&255
def char_word(index):return (0x91230000^((index&0xFFFF)*0x1021))&0xFFFFFFFF
def select_word(index):return (0x80400000^((index&0xFFFF)*257))&0xFFFFFFFF

FUNCTIONS={}
for key,name in (('notify','sa_notification_connected'),('create','sa_notification_create_connected'),
                 ('ready','sa_notification_ready_connected'),('gate','sa_main_dispatch_notified_prefix_gate_connected'),
                 ('spawn','sa_main_dispatch_spawn_connected'),('set','sa_main_dispatch_callback_set_connected'),
                 ('cancel','sa_main_dispatch_cancellation_connected'),('restore','sa_main_dispatch_context_restore_connected')):
 fn=getattr(LIB,name);fn.argtypes=base.previous.ARGS+[base.OBSERVE];fn.restype=ctypes.c_int
 FUNCTIONS[key]=previous.captured(fn)


def prepare(c,n):
 entry=c['entry'];m,state,fpu=previous.prepare(dict(c,entry='notify' if entry in ('cancel','restore') else entry),n)
 count=0 if c.get('task_empty') else c.get('pool_count',2)
 for i in range(12):store(m,THIRD_TASK+4*i,0xDD000000+i)
 for i,node in enumerate(NODES):
  store(m,node,NODES[i+1] if i+1<count else 0)
  store(m,node+4,NODES[i-1] if i and i<count else 0)
 store(m,base.FREE_TASK,NODES[0] if count else 0)
 peer=CONTEXT if c.get('peer_alias_context') else PEER
 store(m,CONTEXT_SLOT,CONTEXT);store(m,PEER_SLOT,peer)
 for i in range(0x90):store(m,CONTEXT+i,(0x31+i*7)&255,1)
 for i in range(0x30):store(m,PEER+i,(0x73+i*3)&255,1)
 initial_mode=c.get('initial_mode',0 if entry=='restore' else 2)
 store(m,CONTEXT+8,initial_mode,1)
 store(m,CONTEXT+4,c.get('old_word',0x1234),2)
 store(m,CONTEXT+6,0x81,1);store(m,CONTEXT+9,0xFE,1);store(m,CONTEXT+11,0xA4,1)
 store(m,CONTEXT+12,0xDEAD1234);store(m,CONTEXT+16,0xA1B2C3D4)
 store(m,CONTEXT+0x20,c.get('restored_mode',1))
 store(m,CONTEXT+0x34,c.get('saved_word',0xFFFE),2)
 store(m,CONTEXT+0x3E,0xB4,1);store(m,CONTEXT+0x76,c.get('saved_char',0x80),1)
 store(m,CONTEXT+0x4C,0xD00D1234);store(m,CONTEXT+0x7C,c.get('saved_param',0xFF),1)
 store(m,CONTEXT+0x64,0x89ABCDEF)
 store(m,CHAR_SLOT,CHAR_TABLE);store(m,OLD_SLOT,OLD_TABLE);store(m,SELECT_SLOT,SELECT_TABLE)
 resume_id=c.get('resume_id',0xFFFFFFFF);param=c.get('resume_param',0x180)
 store(m,RESUME_ID,resume_id);store(m,RESUME_PARAM,param);store(m,FLAG,0xFACEB00C)
 arg_id=c.get('arg_id',0x12348001);arg_param=c.get('arg_param',0x180)
 kind=c.get('kind',0x80)&255
 for word in (c.get('old_word',0x1234),c.get('saved_word',0xFFFE)):
  store(m,OLD_TABLE+signed(word,16)*64+2,kind,1)
 for ident in (resume_id,arg_id):
  store(m,CHAR_TABLE+signed(ident,16)*16,char_word(signed(ident,16)))
 for ident in (resume_id,arg_id,c.get('saved_char',0x80),0x81):
  ix=signed(ident,8);store(m,CHAR_TABLE+ix*16+5,char_byte(ix),1)
 for ix in (signed(kind,8),21,param&255,signed(param,8),arg_param&255,signed(arg_param,8)):
  store(m,SELECT_TABLE+ix*8,c.get('lookup_flags',0))
  store(m,SELECT_TABLE+ix*8+4,select_word(ix))
 if entry in ('cancel','restore'):
  state.pc=CANCEL if entry=='cancel' else RESTORE
  state.r[4]=arg_id&0xFFFFFFFF;state.r[5]=arg_param&0xFFFFFFFF
 LAST.clear();LAST.update(memory=m,context_before=bytes(m[CONTEXT&0xFFFFFF:(CONTEXT&0xFFFFFF)+0x90]),
                         peer_before=bytes(m[peer&0xFFFFFF:(peer&0xFFFFFF)+0x30]),peer=peer,pool_count=count)
 return m,state,fpu


def cases():
 yield from previous.cases()
 for state,mode in itertools.product((0,1,2,3,0x7F,0x80,0xFE,0xFF),range(8)):
  yield dict(entry='cancel',initial_mode=state,mode=mode<<19)
 for restored,ident,flags,mode in itertools.product((0,1,2,0x100,0xFF),(0xFFFFFFFF,0,0x7FFF,0xFFFF8000),(0,0x80000000),range(8)):
  yield dict(entry='cancel',restored_mode=restored,resume_id=ident,lookup_flags=flags,mode=mode<<19)
 for char,word,param,mode in itertools.product((0,0x7F,0x80,0xFF),(0xFFFF,0x8000,0x7FFF),(0,0x80,0xFF),(0,7)):
  yield dict(entry='cancel',restored_mode=0,resume_id=0xFFFF8000,saved_char=char,saved_word=word,saved_param=param,
             resume_param=param,lookup_flags=0x80000000,mode=mode<<19)
 for state,mode in itertools.product((0,1,2,0x80,0xFF),range(8)):
  yield dict(entry='restore',initial_mode=state,mode=mode<<19)
 for word,ident,param,flags,mode in itertools.product((0x8000,0xFFFF,0,1,0x7FFF),(-32768,-1,0,1,32767,0x12348001),
                                                    (0,0x7F,0x80,0xFF,0x100),(0,0x80000000),(0,7)):
  yield dict(entry='restore',old_word=word,arg_id=ident,arg_param=param,lookup_flags=flags,mode=mode<<19)
 for entry,heap,pool,mode in itertools.product(('cancel','restore'),(0,16,32,48,64,96),range(4),range(8)):
  yield dict(entry=entry,restored_mode=0,resume_id=1,heap=heap,pool_count=pool,mode=mode<<19,
             old_head=bool(mode&1),peer_alias_context=bool(mode&2),null_external=True)
 for state,initial,restored,pool,available,mode in itertools.product((0,44,45,48,64,128),(0,2),(0,1),(1,2,3),(0,2),(0,7)):
  yield dict(entry='gate',state=state,initial_mode=initial,restored_mode=restored,resume_id=1,pool_count=pool,
             heap=256,available=available,mode=mode<<19,null_external=True)


def setup():
 *_,allowed=previous.setup()
 translation_path=PROJECT/'analysis/main_dispatch_cancellation_translation.json'
 translation=json.loads(translation_path.read_text())
 baseline=PROJECT/'analysis/main_dispatch_spawn_setter_verification.json'
 for record in (translation,json.loads(baseline.read_text())):
  for p,h in record['sha256'].items():assert digest(Path(p))==h,p
 allowed|={int(a,16) for r in translation['routines'] for a in r['reachable_addresses']}
 base.prepare=prepare;base.FUNCTIONS=FUNCTIONS;base.EXTERNAL=set();base.TRACKED|={CANCEL,RESTORE}
 return translation_path,translation,baseline,allowed


def independent(c,path,status):
 checks=previous.independent(c,path,status)
 if status or not (CANCEL in path or RESTORE in path):return checks
 m=LAST['memory'];expected=bytearray(LAST['context_before']);peer=bytearray(LAST['peer_before'])
 def get(a,n=4):return int.from_bytes(expected[a:a+n],'little')
 def put(a,v,n=4):expected[a:a+n]=(v&((1<<(n*8))-1)).to_bytes(n,'little')
 def restore(ident,param):
  word=get(4,2);flags=get(11,1);character=get(6,1);pointer=get(12)
  put(0x18,0);put(0x30,word,2);put(0x3C,flags,1);put(0x74,character,1);put(0x44,pointer)
  kind=21 if word==0xFFFF else c.get('kind',0x80)&255
  put(0x7A,kind,1);put(0x5C,select_word(signed(kind,8)))
  put(4,0xFFFF,2);put(8,0,1);put(6,ident,1);put(12,char_word(signed(ident,16)))
  put(9,param,1);put(16,select_word(param&255));put(11,flags|int(bool(c.get('lookup_flags',0))),1)
  put(7,char_byte(signed(ident,8)),1);put(0,1)
  peer[0x14:0x18]=b'\0'*4
  if LAST['peer']==CONTEXT:put(0x14,0)
 mode=get(8,1);requests=0
 if c['entry']=='restore':
  if mode==0:restore(c.get('arg_id',0x12348001),c.get('arg_param',0x180));requests=1
 elif CANCEL in path:
  if mode==2:
   for target,source,n in ((4,0x34,2),(11,0x3E,1),(6,0x76,1),(12,0x4C,4),(9,0x7C,1),(16,0x64,4)):
    put(target,get(source,n),n)
   put(8,get(0x20),1);put(7,char_byte(signed(get(6,1),8)),1);put(0,1);put(0x14,0)
   peer[0x14:0x18]=b'\0'*4;requests=1
   if get(8,1)==0 and c.get('resume_id',0xFFFFFFFF)&0xFFFFFFFF!=0xFFFFFFFF:
    restore(c.get('resume_id',0xFFFFFFFF),c.get('resume_param',0x180));requests+=1
  assert load(m,FLAG)==0
 assert bytes(m[CONTEXT&0xFFFFFF:(CONTEXT&0xFFFFFF)+0x90])==expected,(c,'independent context fields')
 if LAST['peer']!=CONTEXT:
  assert bytes(m[PEER&0xFFFFFF:(PEER&0xFFFFFF)+0x30])==peer,(c,'independent peer fields')
 checks.append('context_snapshot_restore_clear_and_table_semantics')
 if c['entry'] in ('cancel','restore'):
  created=min(requests,LAST['pool_count'],c.get('heap',128)//32)
  head=previous.OLDHEAD if c.get('old_head') else 0
  for i in range(created):
   task=NODES[i];work=base.ARENA+16+i*32
   assert load(m,task)==head and load(m,task+4)==(NODES[i+1] if i+1<created else 0),(c,'independent task links')
   if head:assert load(m,head+4)==task
   assert load(m,task+0x10)==TASK_CALLBACK and load(m,task+0x2C)==work
   assert [load(m,work+j,1) for j in range(16)]==[0]*16
   head=task
  assert load(m,base.LEVELS+24)==head
  assert load(m,base.FREE_TASK)==(NODES[created] if created<LAST['pool_count'] else 0)
  if c['entry']=='restore':
   expected_r0=(NODES[0] if created else 0) if mode==0 else signed(mode,8)&0xFFFFFFFF
  elif mode!=2:expected_r0=signed(mode,8)&0xFFFFFFFF
  elif requests==2:expected_r0=NODES[1] if created==2 else 0
  elif get(8,1):expected_r0=signed(get(8,1),8)&0xFFFFFFFF
  else:expected_r0=0xFFFFFFFF
  assert previous.LAST['registers'][0]==expected_r0,(c,'independent guest R0')
  checks.append('native_task_creation_or_failure_with_zeroed_work_and_guest_result')
 return checks


def main():
 translation_path,translation,baseline,allowed=setup()
 new_rows={int(a,16) for r in translation['routines'] for a in r['reachable_addresses']}
 rows=set();edges=set();calls=set();records=[];counts=dict.fromkeys(FUNCTIONS,0)
 totals=dict(boundary_comparisons=0,partial_guard_error_cases=0,independent_invariants=0,completed_without_external_calls=0)
 def check(c,n):
  path,b,status,inv,external=base.run(c,n,allowed)
  assert not external
  rows.update(set(path)&new_rows)
  for i,a in enumerate(path):
   if a not in new_rows:continue
   w=int.from_bytes(base.binary[a-0x8C010000:a-0x8C010000+2],'little')
   if w>>8 in (0x89,0x8B,0x8D,0x8F):
    nxt=i+(2 if w>>8 in (0x8D,0x8F) else 1)
    if nxt<len(path):edges.add((a,path[nxt]))
   if w>>12==0xB or w&0xF0FF in (0x400B,0x402B):calls.add(a)
  independent_checks=independent(c,path,status)
  counts[c['entry']]+=1;totals['boundary_comparisons']+=b;totals['partial_guard_error_cases']+=status!=0
  totals['independent_invariants']+=bool(independent_checks) or inv
  totals['completed_without_external_calls']+=status==0
  records.append(dict(input=c,steps=len(path),boundary_comparisons=b,status=status,independent_checks=independent_checks,
                      inherited_invariant=inv,external_calls=[],full_state_match=True))
 if '--smoke' in sys.argv:
  smoke=[dict(entry='cancel',initial_mode=0),dict(entry='cancel'),dict(entry='cancel',restored_mode=0),
         dict(entry='cancel',restored_mode=0,resume_id=1),dict(entry='cancel',restored_mode=0,resume_id=0xFFFF8000,
             saved_word=0xFFFF,lookup_flags=0x80000000),dict(entry='restore'),dict(entry='restore',initial_mode=1),
         dict(entry='restore',old_word=0xFFFF,lookup_flags=0x80000000,arg_param=0xFF),
         dict(entry='cancel',restored_mode=0,resume_id=1,heap=32),dict(entry='cancel',task_empty=True),
         dict(entry='cancel',restored_mode=0,resume_id=1,old_head=True),
         dict(entry='restore',peer_alias_context=True),dict(entry='gate',state=44,heap=256,pool_count=3,
             restored_mode=0,resume_id=1),dict(entry='spawn'),dict(entry='set')]
  for n,c in enumerate(smoke):check(c,n)
  print('Cancellation smoke checks passed:',len(records),counts);return
 for n,c in enumerate(cases()):
  check(c,n)
  if (n+1)%200==0:print(n+1,'native cancellation comparisons passed',flush=True)
  if (n+1)%25==0:gc.collect()
 assert rows==new_rows,('missing rows',[hex(a) for a in new_rows-rows])
 assert edges=={tuple(int(a,16) for a in e) for r in translation['routines'] for e in r['conditional_edges']}
 assert calls=={int(a,16) for r in translation['routines'] for a in r['external_call_sites']}
 files=[Path(__file__),Path(previous.__file__),PROJECT/'verify_main_dispatch_notification.py',PROJECT/'verify_main_dispatch_prefix_gate.py',
        PROJECT/'verify_startup.py',translation_path,baseline,PROJECT/'build_main_dispatch_cancellation.ps1',
        PROJECT/'source/sa_main_dispatch_cancellation_connected.c',PROJECT/translation['source']]
 # Preserve the earlier report as evidence; pin all dependency inputs it pins.
 files.extend(Path(p) for p in json.loads(baseline.read_text())['sha256'])
 report=dict(source=translation['source'],source_sha256=digest(PROJECT/translation['source']),routines=translation['routines'],
             cases_passed=len(records),counts=counts,rows_covered=len(rows),conditional_edges_covered=len(edges),
             call_sites_covered=len(calls),**totals,external_game_service_calls=0,
             sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),binary_sha256=digest(base.previous.BINARY),
             scope='Cancellation and saved-context restore through native task/heap/cleanup and prior prefix/notification/spawn/setter paths versus original bytes: full RAM/GPR/SR/PR/FPU/FPUL/MACL, ordered CPU boundaries and independent context/table/list/work/result invariants.',
             limitations=['All tested game-service calls are native. Stored callback 8c01b358 and earlier task/dynamic callbacks are not executed by these routines and remain separate recovery work.',
                          'Synthetic valid global/context/table/heap fixtures, not proof of their normal gameplay lifecycle; names/field meanings inferred. Inherited prefix arithmetic guards remain.',
                          'No physical hardware/MMIO, concurrency, complete game-code discovery, live gameplay or playable port proof.'],cases=records)
 (PROJECT/'analysis/main_dispatch_cancellation_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','counts','rows_covered','conditional_edges_covered','call_sites_covered',*totals,'external_game_service_calls')},indent=2))


if __name__=='__main__':
 try:main()
 finally:sh4_binary32.execute_multiply=base.previous.scalar.finite_execute
