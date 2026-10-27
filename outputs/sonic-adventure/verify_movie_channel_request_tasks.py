"""Original admission/submission and active request/pending coroutine lifecycles."""
import hashlib,itertools,json,sys
from pathlib import Path
import movie_channel_request_fixtures as backend
import movie_channel_request_task_fixtures as fixtures
v=fixtures.v;P=v.P;put=fixtures.put;call=fixtures.call

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 ram=v.seed_ram();records=[];checks=0;comparisons=0;seen=set();smoke='--smoke' in sys.argv
 def note(phase,r,path,n,**data):
  nonlocal checks,comparisons
  checks+=n;comparisons+=1;seen.update(path)
  records.append(dict(phase=phase,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n,**data))
 bases=list(itertools.product((0,7),(0,4),(1,18),('packet_wait','device_busy','lock_busy')))
 if smoke:bases=bases[:1]
 for channel,mode,command,kind in bases:
  c=v.Input(dict(entry=v.REGISTER_CHANNELS,mode=mode,sr=0x400000a1,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,register_arguments=[1<<channel,0,0,0]));m,s,f=v.prepare(ram,c);f.fpscr=0x40001|(mode<<19)
  mapped=fixtures.layout(m,channel)['mapped']
  backend.configure(m,mapped,lock=1 if kind=='lock_busy' else 0,busy=0xff if kind=='device_busy' else 0,context=fixtures.INPUT_OBJECT)
  d=fixtures.configure(m,channel,1,0,2)
  # Device-busy byte and completion word are separate original fields. Never
  # install a completion serial merely to get the parser past its real wait.
  put(m,fixtures.INPUT_OBJECT+0x2e+mapped*44,0xff if kind=='device_busy' else 0,1)
  base=v.load(m,0x8c648368);m[base&0xffffff:(base&0xffffff)+8*0x148]=bytes(8*0x148)
  descriptor=base+channel*0x148;put(m,descriptor+2,1,1);put(m,descriptor+0x1c,0x8cc70000)
  put(m,v.load(m,0x8c648370),fixtures.GLOBAL_OBJECT);put(m,fixtures.GLOBAL_OBJECT+8,0);put(m,v.load(m,0x8c6475fc),0)
  r,path,m,s,f=call(m,s,f,c,v.REGISTER_CHANNELS,[1<<channel,0,0,0]);assert r['status']==0;note('admission',r,path,1,channel=channel,mode=mode,command=command,kind=kind)
  r,path,m,s,f=call(m,s,f,c,v.AI_SCHEDULE);assert r['status']==0;note('initial_idle',r,path,1)
  owner=v.load(m,v.load(m,0x8c660880));rec=owner+0x210;idle_sp=v.load(m,rec+0x18);channel_record=v.load(m,0x8c6479bc)+channel*0x80;status_word=v.load(m,0x8c6479c4)+channel*4
  task_before=v.sample(m,owner+0x210,8*0x124)
  for entry,args,stack in ((0x8c647c50,[0],()),(0x8c647c6c,[0x8c05180c],()),(0x8c647cc4,[channel,channel,0,0],(0,0,0)),(0x8c647cb4,[channel,command],())):
   r,path,m,s,f=call(m,s,f,c,entry,args,stack);assert r['status']==0;note('submission',r,path,1,entry=hex(entry))
  assert v.sample(m,owner+0x210,8*0x124)==task_before;checks+=1
  before=type(s).from_buffer_copy(s);before.r[4]=mapped;before.r[5]=2;before.r[6]=0
  expected,result,opcode,words=backend.expected_packet(m,before,0x8c660494,0,context=fixtures.INPUT_OBJECT)
  saved_sp=None
  for attempt in range(3):
   prior_frame=v.sample(m,backend.FRAME,512);r,path,m,s,f=call(m,s,f,c,v.AI_SCHEDULE);n=3
   assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
   sp=v.load(m,rec+0x18);wanted_pc=0x8c66091a if kind=='packet_wait' else 0x8c660906
   assert sp==idle_sp-(112 if command==1 else 108) and (saved_sp is None or sp==saved_sp);saved_sp=sp;n+=2
   assert v.load(m,sp)==wanted_pc and v.load(m,rec+0x20)==0x8c668e48;n+=2
   assert v.load(m,sp+24)==0x8c64756e and v.load(m,sp+64)==(0x8c64831c if command==1 else 0x8c648400) and v.load(m,sp+100)==(0x8c647a08 if command==1 else 0x8c6479fa);n+=3
   assert v.load(m,channel_record+8)==command and v.load(m,status_word)==0 and v.load(m,owner+0x20c)==0;n+=3
   assert v.load(m,rec+0x14)==attempt+2 and v.load(m,owner+0x204)==attempt+2;n+=2
   assert v.load(m,d['status']+0x20)==0 and v.sample(m,d['status']+0xd,1)==b'\x01';n+=2
   if kind=='packet_wait':
    assert v.sample(m,backend.FRAME,512)==v.sample(expected,backend.FRAME,512);n+=1
    assert v.load(m,fixtures.INPUT_OBJECT+4064)==2 and v.load(m,d['status']+0x18)==1;n+=2
    assert sum(b['target']=='0x8c65f810' for b in r['boundaries'])==int(attempt==0) and r['cache_purge_sequence']==([backend.LOCK] if attempt==0 else []);n+=2
   else:
    assert v.sample(m,backend.FRAME,512)==prior_frame and v.load(m,fixtures.INPUT_OBJECT+4064)==1;n+=2
    assert r['cache_purge_sequence']==[backend.LOCK];n+=1
   assert m[backend.LOCK&0xffffff]==(0x81 if kind=='lock_busy' else 0);n+=1
   assert not any(b['pr']=='0x8c647bb4' for b in r['boundaries']);n+=1
   note('active_pending',r,path,n,channel=channel,mode=mode,command=command,kind=kind,attempt=attempt,saved_resume=hex(wanted_pc))
  r,path,m,s,f=call(m,s,f,c,v.OWNER_ALL);assert r['status']==0 and v.sample(m,owner,0x2694)==bytes(0x2694);note('original_cancel',r,path,2)
  print('REQUEST TASK',channel,mode,command,kind,flush=True)
 if smoke:return
 files=[Path(__file__),Path(backend.__file__),Path(fixtures.__file__),Path(v.__file__),Path(fixtures.fast.__file__),v.MP,P/'analysis/movie_channel_request_connection.json',v.effects.DLL]
 report=dict(original_native_comparisons=comparisons,independent_checks=checks,admission_bases=len(bases),observed_pending_returns=3*len(bases),cases=records,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original taskconstruction/admission/submission,commands1/18,channels0/7,FR0/FR1. Activeheader postsactual packet thenoriginal completionmatch remainsfalse andyields;66091a resumescorrectlythreeobservedtimeswithoutduplicatepacket orstackleak. Busydevice/mutex pathsyield660906 and retryactualbackends,notfabricatedsuccess. Wholemachine/devices/boundaries plusindependentrequestbytes/serial/lock/nestedstack/ownerdeadline/pendingstateoracles. Originalownercancellation endsobservedprefixes;completion status/response data areNEVER installed. Physicalresponse/parser/codeccompletionunqualified;coherentcontextisexplicitfixture.')
 (P/'analysis/movie_channel_request_tasks_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CHANNEL REQUEST TASKS PASS',comparisons,checks)
if __name__=='__main__':main()
