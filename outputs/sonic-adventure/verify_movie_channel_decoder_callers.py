"""Original admission/submission APIs to decoder return, callback and resumption."""
import hashlib,itertools,json,sys
from pathlib import Path
import verify_movie_channel_decoder_leaves as leaf
v=leaf.v;P=v.P;fast=leaf.fast;put=leaf.put
POST_SET,PAYLOAD_SET,COMMAND_SET=0x8c647c50,0x8c647cc4,0x8c647cb4
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def carry(c,r):c.update(other=r['integer_other'],mach=r['final_mach'],macl=r['final_macl'],gbr=r['final_gbr'],vbr=r['vbr'])
def call(m,s,f,c,entry,args=None,stack_args=()):
 c['entry']=entry;s.pc=entry;s.pr=0x8cfed000
 if args is not None:
  for j,value in enumerate(args):s.r[4+j]=value
 for j,value in enumerate(stack_args):put(m,s.r[15]+j*4,value)
 if entry==v.AI_SCHEDULE:s.r[4]=v.load(m,v.load(m,0x8c660880))
 r,path,m,s,f=fast.compare(m,s,f,c);carry(c,r)
 return r,path,m,s,f
def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();comparisons=0;checks=0;smoke='--smoke' in sys.argv
 def note(phase,r,path,n,**data):
  nonlocal comparisons,checks
  comparisons+=1;checks+=n;seen.update(path)
  for j,pc in enumerate(path[:-1]):
   if v.ROWS.get(pc,'').startswith(('bt ','bf ')):edges.add((hex(pc),hex(path[j+1])))
  records.append(dict(phase=phase,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n,**data))
 bases=list(itertools.product((0,7),(0,4),(0,1)))
 if smoke:bases=bases[:1]
 for channel,mode,global_mode in bases:
  c=v.Input(dict(entry=v.REGISTER_CHANNELS,mode=mode,sr=0x400000a1,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,register_arguments=[1<<channel,0,0,0]));m,s,f=v.prepare(ram,c);f.fpscr=0x40001|(mode<<19)
  # Declared inactive descriptor/global-object fixture, established before every
  # original task constructor. Task contexts/stacks are never written by tests.
  base=v.load(m,0x8c648368);m[base&0xffffff:(base&0xffffff)+8*0x148]=bytes(8*0x148);put(m,v.load(m,0x8c648370),leaf.OBJECT);put(m,leaf.OBJECT+8,global_mode)
  r,path,m,s,f=call(m,s,f,c,v.REGISTER_CHANNELS,[1<<channel,0,0,0]);assert r['status']==0;note('admission',r,path,1,channel=channel,mode=mode,global_mode=global_mode)
  owner=v.load(m,v.load(m,0x8c660880));rec=owner+0x210;channel_record=v.load(m,0x8c6479bc)+channel*0x80;status_word=v.load(m,0x8c6479c4)+channel*4
  r,path,m,s,f=call(m,s,f,c,v.AI_SCHEDULE);assert r['status']==0 and v.load(m,rec+0x20)==0x8c668e48;note('initial_idle',r,path,2,channel=channel,mode=mode,global_mode=global_mode)
  base_m=bytearray(m);base_s=type(s).from_buffer_copy(s);base_f=type(f).from_buffer_copy(f);base_c=dict(c)
  configs=list(itertools.product((0,7),(0,0x8c05180c,0x8cfef000)))
  if smoke:configs=configs[:1]
  for index,callback in configs:
   m=bytearray(base_m);s=type(base_s).from_buffer_copy(base_s);f=type(base_f).from_buffer_copy(base_f);c=v.Input(dict(base_c));task_before=v.sample(m,owner+0x210,8*0x124)
   r,path,m,s,f=call(m,s,f,c,POST_SET,[callback]);assert r['status']==0 and all(v.load(m,v.load(m,0x8c6479bc)+j*0x80+0x74)==callback for j in range(8));note('original_callback_setter',r,path,2,channel=channel,index=index,callback=hex(callback))
   r,path,m,s,f=call(m,s,f,c,PAYLOAD_SET,[channel,index,0x11223344,0x55667788],(0xaabbccdd,0x13579bdf,0x2468ace0));assert r['status']==0 and [v.load(m,channel_record+off) for off in (0x14,0x18,0x1c,0x20,0x24,0x28)]==[index,0x11223344,0x55667788,0xaabbccdd,0x13579bdf,0x2468ace0];note('original_payload_setter',r,path,7,channel=channel,index=index,callback=hex(callback))
   r,path,m,s,f=call(m,s,f,c,COMMAND_SET,[channel,1]);assert r['status']==0 and v.load(m,channel_record+8)==1 and v.sample(m,owner+0x210,8*0x124)==task_before;note('original_command_setter',r,path,3,channel=channel,index=index,callback=hex(callback))
   r,path,m,s,f=call(m,s,f,c,v.AI_SCHEDULE);status=-2 if callback==0x8cfef000 else 0;n=3;assert r['status']==status and r['full_state_match'] and r['ordered_boundaries_match']
   assert v.sample(m,channel_record,0x2c)==bytes(0x2c) and v.load(m,status_word)==0xffffff01;n+=2
   assert v.load(m,owner+0x20c)==0 and v.sample(m,base+index*0x148,0x148)==bytes(0x148);n+=2
   assert all(pc in path for pc in (0x8c647b74,0x8c6482a0,0x8c64850a,0x8c647b82));n+=1
   assert (0x8c648470 in path)==bool(global_mode) and 0x8c647982 not in path;n+=2
   if callback:
    observed=[b for b in r['boundaries'] if b['target']==hex(callback)];assert len(observed)==1 and observed[0]['arguments']==[channel,1,0xffffff01,0] and observed[0]['pr']=='0x8c647bb4';n+=3
   if status:assert r['stop_pc']==hex(callback) and s.pr==0x8c647bb4;n+=2
   else:
    assert v.load(m,rec+0x20)==0x8c668e48 and v.load(m,v.load(m,rec+0x18))==0x8c647b74 and v.load(m,rec+0x14)==2;n+=3
   note('decoder_return_callback',r,path,n,channel=channel,index=index,callback=hex(callback),mode=mode,global_mode=global_mode)
   if not status:
    r,path,m,s,f=call(m,s,f,c,v.AI_SCHEDULE);assert r['status']==0 and 0x8c6482a0 not in path and v.load(m,status_word)==0xffffff01 and v.load(m,rec+0x14)==3;note('subsequent_idle',r,path,4,channel=channel,index=index,callback=hex(callback))
    r,path,m,s,f=call(m,s,f,c,v.OWNER_ALL);assert r['status']==0 and v.sample(m,owner,0x2694)==bytes(0x2694);note('teardown',r,path,2,channel=channel,index=index,callback=hex(callback))
  print('DECODER CALLER',channel,mode,global_mode,'original APIs/worker/callback',flush=True)
 if smoke:return
 coverage=[]
 worker=v.old.manifest['routines']
 for body in v.manifest['routines']+worker:
  if any(r['address']==body['address'] for r in coverage):continue
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};miss_edges=set(map(tuple,body['conditional_edges']))-edges
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(miss_edges),missing_rows=sorted(missing),missing_outcomes=sorted(miss_edges)))
 files=[Path(__file__),Path(leaf.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_channel_decoder_connection.json',v.effects.DLL]
 report=dict(original_native_comparisons=comparisons,independent_checks=checks,admission_bases=len(bases),command_lifecycles=len(bases)*6,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actual original6477cc/660842/668bee taskadmission andoneidle yield;647c50 callbackregistration,647cc4 payloadsubmission,647cb4 commandposting createcommand1 withresourceindices0/7. Neworiginalinactive6482a0/mode1-648470 returnssentinel;actual64850a cleanup,workerrecord/statuswrites andnull/realNOP/unknownpostcallbacks execute. Normalcasesyieldagain,resumeidle,thenactualteardown. Unknowncallbackprefix retainsoriginalclearedrecord/statuswrites andstops-2 withoutfakecompletion. Initialinactive8descriptor/globalobject fixture establishedbeforetaskconstructors;tasks/contexts/stacks/counters neverpatched bytests,callerstackargsareABIinputs. Independenttasksnapshot/APIwrites/zeroedrecords/sentinelstatus/callbackargs/deadline/continuation/teardown checks pluswholeoriginal/native machine/devices/boundaries. No activecodec/resourceacquisition/physicalcontroller/fullworker claim.')
 (P/'analysis/movie_channel_decoder_callers_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CHANNEL DECODER CALLERS PASS',comparisons,checks)
if __name__=='__main__':main()
