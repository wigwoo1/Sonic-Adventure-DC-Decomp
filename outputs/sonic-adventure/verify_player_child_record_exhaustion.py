"""Reach the record allocator's null return through genuine heap exhaustion."""
import hashlib,itertools,json
from pathlib import Path
import player_collision_attach_fixtures as q
P=q.P
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_child_record_exhaustion_verification.json';assert not target.exists()
 records=[];checks=0;seen=set();edges=set()
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 def clone(base):
  m,s,f,c=base
  return bytearray(m),type(s).from_buffer_copy(s),type(f).from_buffer_copy(f),q.v.Input(dict(c))
 def phase(base,entry,args,label):
  m,s,f,c=base;sp=s.r[15];c['entry']=s.pc=entry;s.pr=0x8cfed000;saved=list(s.r[8:15])
  for j,value in enumerate(args):s.r[4+j]=value
  r,path,m,s,f=q.fast.compare(m,s,f,c)
  require(r['status']==0,(label,r['status'],r['stop_pc']))
  require(r['full_state_match'] and r['ordered_boundaries_match'],'exact state and ordered boundaries')
  require(s.r[15]==sp and s.pc==s.pr==0x8cfed000 and list(s.r[8:15])==saved,'balanced return and callee saved registers')
  c.update(other=r['integer_other'],mach=r['final_mach'],macl=r['final_macl'],gbr=r['final_gbr'],vbr=r['vbr'])
  seen.update(path)
  for i,a in enumerate(path):
   text=q.v.ROWS.get(a,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
  records.append(dict(label=label,entry=hex(entry),status=0,return_value=s.r[0],steps=len(path),boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
  return m,s,f,c
 initial=q.prepare(q.v.seed_ram(),dict(kind=0));initial=phase(initial,0x8c098a10,[],'original heap initialization')
 for mode,rm in itertools.product((0,4),(0,1)):
  base=clone(initial);m,s,f,c=base;f.fpscr=0x40000|(mode<<19)|rm
  base=phase(base,q.ALLOCATE,[q.CHILD_ACTOR],dict(mode=mode,rm=rm,phase='allocate real first node'))
  m,s,f,c=base;node=s.r[0];require(node==0x8c7ee8f0,'independent first heap payload address')
  base=phase(base,0x8c098a80,[0x8ffa0],dict(mode=mode,rm=rm,phase='consume entire remaining heap'))
  m,s,f,c=base;pressure=s.r[0];require(pressure==node+0x50,'independent rounded record allocation extent')
  base=phase(base,q.ALLOCATE,[q.CHILD_ACTOR],dict(mode=mode,rm=rm,phase='genuine record allocation failure'))
  m,s,f,c=base
  require(s.r[0]==0,'original allocator returns null after real exhaustion')
  require(q.get(m,q.CHILD_ACTOR+0x10)==node and q.get(m,node+0x2c)==q.get(m,node+0x30)==0,'allocation failure preserves actual existing list')
  base=phase(base,q.REMOVE,[q.CHILD_ACTOR,node],dict(mode=mode,rm=rm,phase='free actual node'))
  require(q.get(base[0],q.CHILD_ACTOR+0x10)==0,'list becomes empty')
  base=phase(base,q.ALLOCATE,[q.CHILD_ACTOR],dict(mode=mode,rm=rm,phase='allocate freed space again'))
  require(base[1].r[0]==node,'original allocator reuses genuinely released block')
  base=phase(base,q.CLEANUP,[q.TASK],dict(mode=mode,rm=rm,phase='original cleanup of recycled node'))
  require(q.get(base[0],q.CHILD_ACTOR+0x10)==0,'cleanup releases recycled node')
  base=phase(base,0x8c098b42,[pressure],dict(mode=mode,rm=rm,phase='release pressure allocation'))
  print('RECORD EXHAUSTION',mode,rm,'PASS',flush=True)
 require(('0x8c0b33b0','0x8c0b33c6') in edges,'actual original null-allocation branch executes')
 files=[Path(__file__),P/'player_collision_attach_fixtures.py',P/'player_collision_attach_reference.py',P/'write_watch_player_collision_attach.py',P/'analysis/player_child_update_translation.json',P/'analysis/player_collision_attach_translation.json',q.v.effects.DLL]
 report=dict(status='genuine_record_exhaustion_preserves_list_and_released_space_is_reusable',original_native_comparisons=len(records),checks=checks,bank_rounding_combinations=4,phases=records,executed_rows=sorted(map(hex,seen)),executed_conditional_outcomes=sorted(edges),supplied_allocator_results=0,supplied_game_callee_outputs=0,supplied_private_execution_frames=0,new_original_routine_credit=0,overall_completion_percent=10,sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('RECORD EXHAUSTION COMPLETE',len(records),checks,flush=True)
if __name__=='__main__':main()
