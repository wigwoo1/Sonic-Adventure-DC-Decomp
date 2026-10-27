"""Original pool/enqueue helpers create contexts used by repeated real yields."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_task_yield as fast
v=fast.old;P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 records=[];comparisons=0;checks=0;ram=v.seed_ram()
 for callback,sr,size in itertools.product((v.YIELD_ONE,v.FINISH),(0x400000a1,0x600000a1),(0x100,0x400)):
  c=v.Input(dict(entry=v.REGISTER_POOL,mode=0,sr=sr,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234));m,s,f=v.prepare(ram,c);f.fpscr=0x40001;f.fpul=0x12345678
  for bank in range(2):
   for reg in range(16):f.banks[bank][reg]=(0x7fa12345^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
  phases=[];sequence_checks=0
  def run(entry,args):
   nonlocal m,s,f,comparisons,sequence_checks
   c['entry']=entry;s.pc=entry;s.pr=0x8cfed000
   for j,value in enumerate(args):s.r[4+j]=value
   before=v.effects.State.from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fast.compare(m,s,f,c);comparisons+=1
   assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'];sequence_checks+=3
   assert s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;sequence_checks+=3
   m=a;phases.append(dict(entry=hex(entry),steps=len(path),boundaries=[b['target'] for b in r['boundaries']],status=r['status']))
  run(v.REGISTER_POOL,[]);owner=v.load(m,v.load(m,0x8c660880));assert owner==v.load(m,0x8c668c44);sequence_checks+=1
  assert [v.load(m,owner+off) for off in (0,0x204,0x208,0x20c)]==[1,0,8,0];sequence_checks+=4
  assert '0x8c668b38' in phases[-1]['boundaries'] and '0x8c668b58' in phases[-1]['boundaries'];sequence_checks+=2
  payload=0x8ce20000;run(v.REGISTER_ENQUEUE,[callback,payload,size]);rec=owner+0x210;task_sp=payload+size-4
  assert [v.load(m,rec+off) for off in (0,4,0xc,0x14,0x18,0x20,0x118)]==[payload,size//4,1,0,task_sp,callback,0];sequence_checks+=7
  assert s.r[0]==0 and phases[-1]['boundaries'].count('0x8c110954')==1 and phases[-1]['boundaries'].count('0x8c10d25c')==2;sequence_checks+=3
  cycles=[]
  for j in range(4):
   prior_current=v.load(m,v.load(m,0x8c668ea4));run(v.AI_SCHEDULE,[owner]);targets=phases[-1]['boundaries']
   assert v.load(m,owner+0x204)==j+1 and v.load(m,v.load(m,0x8c668ea4))==prior_current;sequence_checks+=2
   assert v.load(m,owner+0x2690)==rec+7*0x124;sequence_checks+=1
   expected_state=1 if callback==v.YIELD_ONE else 0;expected_deadline=j+1 if callback==v.YIELD_ONE else 0
   assert v.load(m,rec+0xc)==expected_state and v.load(m,rec+0x14)==expected_deadline;sequence_checks+=2
   assert v.load(m,rec+0x18)==task_sp-8 and v.load(m,rec+0x20)==v.YIELD_RESUME;sequence_checks+=2
   assert v.load(m,task_sp-8)==callback and v.load(m,task_sp-4)==rec;sequence_checks+=2
   assert v.load(m,owner+0x20c)==(0 if callback==v.YIELD_ONE else 0xffffffff);sequence_checks+=1
   if callback==v.YIELD_ONE or j==0:
    assert targets.count('0x8c110954')==2 and targets.count('0x8c1108e0')==2 and targets.count(hex(callback))==1 and targets.count(hex(v.YIELD))==1;sequence_checks+=4
   else:assert targets.count('0x8c110954')==1 and '0x8c1108e0' not in targets;sequence_checks+=2
   assert s.sr&1;sequence_checks+=1
   cycles.append(dict(counter=j+1,state=expected_state,deadline=expected_deadline,saved_task_pr=hex(v.load(m,rec+0x20)),saved_task_sp=hex(v.load(m,rec+0x18))))
  records.append(dict(callback=hex(callback),initial_sr=hex(sr),task_stack_bytes=size,owner=hex(owner),record=hex(rec),phases=phases,cycles=cycles,original_native_comparisons=6,independent_checks=sequence_checks,live_count_underflow_observed=callback==v.FINISH));checks+=sequence_checks
  print('TASK LIFECYCLE',hex(callback),hex(sr),size,'4cycles',flush=True)
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_task_yield_connection.json',v.effects.DLL,P/'analysis/movie_active_input_qualification.json',P/'analysis/movie_record_register_qualification.json']
 # Older registration qualification filename is verified through the registry,
 # whose exact source/evidence rows are separately pinned at qualification.
 files=[p for p in files if p.exists()]
 report=dict(lifecycles_passed=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Eight actual66082cpool/660842enqueue helper sequences create original taskcontexts/stacks;four668e60scheduler calls then perform yield/resume/yield or taskfinish/cleanup. No taskcontext/record/callback slot handcrafted after helper calls. Declared realROM taskentries668e5c or668d56,externaltaskstack anddefaultFPSCR40001;not natural gameboot/acquired taskgraph. Complete original/native machine/device/ordered comparisons plus independent metadata,deadline,stackframe,contextPR/SP,callcounts andrestoredcaller/FPUchecks. Low-level finish fixture starts ownercount0 and actualcleanup wraps it toffffffff;recordedunchanged,notfixed. Upstreamcaller ownership/livecount accounting remains unqualified. Unknowncoroutines/interior continuations andphysicaltiming outside scope.')
 (P/'analysis/movie_task_yield_lifecycle_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('TASK LIFECYCLE PASS',len(records),comparisons,checks)
if __name__=='__main__':main()
