"""Original task/API/parser yields, finite retries, waits and bad-return guards."""
import hashlib,itertools,json,sys
from pathlib import Path
import movie_channel_io_fixtures as fixtures
v=fixtures.v;P=v.P;fast=fixtures.fast;put=fixtures.put;call=fixtures.call
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();comparisons=0;checks=0;smoke='--smoke' in sys.argv
 def note(phase,r,path,n,**data):
  nonlocal comparisons,checks
  comparisons+=1;checks+=n;seen.update(path)
  for j,pc in enumerate(path[:-1]):
   if v.ROWS.get(pc,'').startswith(('bt ','bf ')):edges.add((hex(pc),hex(path[j+1])))
  records.append(dict(phase=phase,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n,**data))
 bases=list(itertools.product((0,7),(0,4),(1,18)))
 if smoke:bases=bases[:1]
 for channel,mode,command in bases:
  c=v.Input(dict(entry=v.REGISTER_CHANNELS,mode=mode,sr=0x400000a1,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,register_arguments=[1<<channel,0,0,0]));m,s,f=v.prepare(ram,c);f.fpscr=0x40001|(mode<<19)
  for bank in range(2):
   for reg in range(16):f.banks[bank][reg]=(0x7fa12345^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
  fixtures.configure(m,channel,0,0,2);base=v.load(m,0x8c648368);m[base&0xffffff:(base&0xffffff)+8*0x148]=bytes(8*0x148)
  descriptor=base+channel*0x148;put(m,descriptor+2,1,1);put(m,descriptor+0x1c,0x8cc70000)
  put(m,v.load(m,0x8c648370),fixtures.GLOBAL_OBJECT);put(m,fixtures.GLOBAL_OBJECT+8,0);put(m,v.load(m,0x8c6475fc),0)
  r,path,m,s,f=call(m,s,f,c,v.REGISTER_CHANNELS,[1<<channel,0,0,0]);assert r['status']==0;note('admission',r,path,1,channel=channel,mode=mode,command=command)
  r,path,m,s,f=call(m,s,f,c,v.AI_SCHEDULE);assert r['status']==0;note('initial_idle',r,path,1,channel=channel,mode=mode,command=command)
  owner=v.load(m,v.load(m,0x8c660880));rec=owner+0x210;idle_sp=v.load(m,rec+0x18);channel_record=v.load(m,0x8c6479bc)+channel*0x80;status_word=v.load(m,0x8c6479c4)+channel*4
  base_m=bytearray(m);base_s=type(s).from_buffer_copy(s);base_f=type(f).from_buffer_copy(f);base_c=dict(c)
  configs=[(budget,callback,False) for budget,callback in itertools.product((0,2),(0,0x8c05180c,0x8cfef000))]+[(-1,0,False),(0,0,True)]
  if smoke:configs=configs[:1]
  for budget,callback,fault in configs:
   m=bytearray(base_m);s=type(base_s).from_buffer_copy(base_s);f=type(base_f).from_buffer_copy(base_f);c=v.Input(dict(base_c))
   # Retry count is a declared caller configuration; original parser state and
   # every task context are left to original code throughout normal execution.
   put(m,v.load(m,0x8c6475f4),budget)
   task_before=v.sample(m,owner+0x210,8*0x124)
   for entry,args,stack in ((0x8c647c50,[callback],()),(0x8c647c6c,[0x8c05180c],()),(0x8c647cc4,[channel,channel,0,0],(0,0,0)),(0x8c647cb4,[channel,command],())):
    r,path,m,s,f=call(m,s,f,c,entry,args,stack);assert r['status']==0;note('original_submission',r,path,1,channel=channel,mode=mode,command=command,budget=budget,callback=hex(callback),fault=fault,entry=hex(entry))
   assert v.sample(m,owner+0x210,8*0x124)==task_before;checks+=1
   attempts=3 if budget==-1 else budget+1;first_sp=None
   for attempt in range(1 if fault else attempts):
    r,path,m,s,f=call(m,s,f,c,v.AI_SCHEDULE);n=3;assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
    sp=v.load(m,rec+0x18);expected_delta=112 if command==1 else 108
    assert sp==idle_sp-expected_delta and (first_sp is None or sp==first_sp);first_sp=sp;n+=2
    assert v.load(m,rec+0x20)==0x8c668e48 and v.load(m,sp)==0x8c660a68 and v.load(m,sp+24)==0x8c64756e;n+=3
    assert v.load(m,sp+64)==(0x8c64831c if command==1 else 0x8c648400) and v.load(m,sp+100)==(0x8c647a08 if command==1 else 0x8c6479fa);n+=2
    assert v.load(m,channel_record+8)==command and v.load(m,status_word)==0 and v.load(m,owner+0x20c)==0;n+=3
    assert v.load(m,rec+0x14)==attempt+2 and v.load(m,owner+0x204)==attempt+2;n+=2
    assert 0x8c6608a4 in path and 0x8c660a64 in path and ((0x8c660a68 in path)==(attempt>0));n+=1
    assert not any(b['pr']=='0x8c647bb4' for b in r['boundaries']);n+=1
    note('nested_io_yield',r,path,n,channel=channel,mode=mode,command=command,budget=budget,callback=hex(callback),attempt=attempt,stack=hex(sp),fault=fault)
   if fault:
    # A separate negative fixture corrupts one saved guest return address. It
    # never supplies a successful result or repairs a suspended task.
    sp=v.load(m,rec+0x18);saved_r14=v.load(m,sp+4);put(m,sp+24,0x8cfef000)
    r,path,m,s,f=call(m,s,f,c,v.AI_SCHEDULE);n=3;assert r['status']==-3 and r['full_state_match'] and r['ordered_boundaries_match']
    assert r['stop_pc']=='0x8c660a7a' and s.r[15]==sp+52 and s.r[14]==saved_r14 and s.pr==0x8cfef000;n+=4
    assert v.load(m,channel_record+8)==command and v.load(m,status_word)==0;n+=2
    note('bad_saved_return',r,path,n,channel=channel,mode=mode,command=command);continue
   if budget==-1:
    assert v.load(m,channel_record+8)==command and v.load(m,status_word)==0;checks+=2
    r,path,m,s,f=call(m,s,f,c,v.OWNER_ALL);assert r['status']==0 and v.sample(m,owner,0x2694)==bytes(0x2694);note('cancel_waiting_owner',r,path,2,channel=channel,mode=mode,command=command);continue
   r,path,m,s,f=call(m,s,f,c,v.AI_SCHEDULE);status=-2 if callback==0x8cfef000 else 0;n=3;assert r['status']==status and r['full_state_match'] and r['ordered_boundaries_match']
   assert all(pc in path for pc in (0x8c660a68,0x8c64756e,0x8c64831c if command==1 else 0x8c648400,0x8c647a08 if command==1 else 0x8c6479fa));n+=1
   assert not any(b['target'] in ('0x8c660a68','0x8c64756e','0x8c64831c','0x8c648400','0x8c647a08','0x8c6479fa') for b in r['boundaries']);n+=1
   assert v.sample(m,channel_record,0x2c)==bytes(0x2c) and v.load(m,status_word)==0xffffff01;n+=2
   assert v.sample(m,descriptor+2,1)==b'\x01' and v.load(m,descriptor+4)==0 and v.sample(m,descriptor+8,5)==bytes(5);n+=3
   if callback:
    event=[b for b in r['boundaries'] if b['target']==hex(callback) and b['pr']=='0x8c647bb4'];assert len(event)==1 and event[0]['arguments']==[channel,command,0xffffff01,0];n+=2
   if status:assert r['stop_pc']==hex(callback) and s.pr==0x8c647bb4;n+=2
   else:assert v.load(m,rec+0x18)==idle_sp and v.load(m,v.load(m,rec+0x18))==0x8c647b74;n+=2
   note('nested_completion',r,path,n,channel=channel,mode=mode,command=command,budget=budget,callback=hex(callback))
   if not status:
    r,path,m,s,f=call(m,s,f,c,v.AI_SCHEDULE);assert r['status']==0 and 0x8c6608a4 not in path and v.load(m,status_word)==0xffffff01;note('subsequent_idle',r,path,3,channel=channel,mode=mode,command=command)
    r,path,m,s,f=call(m,s,f,c,v.OWNER_ALL);assert r['status']==0 and v.sample(m,owner,0x2694)==bytes(0x2694);note('teardown',r,path,2,channel=channel,mode=mode,command=command)
  print('NESTED IO',channel,mode,command,'finite retry/wait/fault controls',flush=True)
 if smoke:return
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
 files=[Path(__file__),Path(fixtures.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_channel_io_connection.json',P/'analysis/movie_channel_io_returns.json',v.effects.DLL]
 report=dict(original_native_comparisons=comparisons,independent_checks=checks,admission_bases=len(bases),finite_lifecycles=len(bases)*6,pending_prefix_lifecycles=len(bases),negative_return_lifecycles=len(bases),cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actualoriginalconstructor/API/task/scheduler paths commands1/18,channels0/7,FR0/FR1. Originalinactiveheader failure yieldsinside6608a4;contextrestorer resumes660a68->64756e->64831c/648400->647a08/6479fa beforefalsefunction observation. Budgets0/2 naturallyperform1/3IOyields beforeoriginal-255completion,cleanup/callback/idle/teardown. Budget-1 remains pending for3observedschedulerABIreturns thenoriginalownercancellation;no fabricatedcompletion orsourcewaitlimit. IndependentnestedgueststackPR/SP/deadline/taskrecord/status/callback/stack-recovery oracles pluswholemachine/device/boundary matches. Eightseparatebad-saved-PR fixtures mutateonegueststackwordonlyasfaultinput;guardstops-3beforeRTSdelayslot,notrelaxedintounknownjump. Initialcoherentinactiveinput/header/descriptor/globalobjectfixtures,originalretailindexmap,callerretryconfiguration;no normal taskcontext/counter/stack repairs or parser/callback resultsubstitutes. Activeheader/request/successfulparse/arbitraryresumegraphs remainunqualified.')
 (P/'analysis/movie_channel_io_nested_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CHANNEL IO NESTED PASS',comparisons,checks)
if __name__=='__main__':main()
