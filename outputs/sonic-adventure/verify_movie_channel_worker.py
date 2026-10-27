"""Actual original admission, idle resumption and all-command missing boundaries."""
import hashlib,itertools,json,sys
from pathlib import Path
import write_watch_channel_worker as fast
v=fast.old;P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def put(m,a,value):m[a&0xffffff:(a&0xffffff)+4]=(value&0xffffffff).to_bytes(4,'little')
COMMAND_TARGETS={1:0x8c6482a0,3:0x8c648620,4:0x8c64919e,5:0x8c6495ea,6:0x8c649306,7:0x8c649d20,8:0x8c649f1e,9:0x8c64a828,10:0x8c64a244,11:0x8c64b67a,12:0x8c64b71c,13:0x8c64b91c,14:0x8c64bbb2,15:0x8c64b584,16:0x8c64b60e,17:0x8c649e00,18:0x8c64839a,20:0x8c64bcd6}
PAYLOAD=0x8cd60000
def admit(ram,mask,sr,mode):
 c=v.Input(dict(entry=v.REGISTER_CHANNELS,mode=mode,sr=sr,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,register_arguments=[mask,0,0,0]))
 m,s,f=v.prepare(ram,c);f.fpscr=0x40001|(mode<<19)
 for bank in range(2):
  for j in range(16):f.banks[bank][j]=(0x7fa12345^((bank+1)*0x11111111)^((j+1)*0x01020304))&0xffffffff
 r,path,m,s,f=fast.compare(m,s,f,c);assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
 return c,m,s,f,r,path
def run(m,s,f,c,entry):
 c['entry']=entry;s.pc=entry;s.pr=0x8cfed000
 if entry==v.AI_SCHEDULE:s.r[4]=v.load(m,v.load(m,0x8c660880))
 return fast.compare(m,s,f,c)
def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();comparisons=0;checks=0;admissions=0
 def record(kind,r,path,n,**extra):
  nonlocal comparisons,checks
  comparisons+=1;checks+=n;seen.update(path)
  for j,pc in enumerate(path[:-1]):
   if v.ROWS.get(pc,'').startswith(('bt ','bf ')):edges.add((hex(pc),hex(path[j+1])))
  records.append(dict(kind=kind,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n,**extra))
 # Every selector uses real constructor-created contexts and stacks, never patched.
 for channel,mode,sr in itertools.product(range(8),(0,4),(0x400000a1,0x600000a1)):
  c,m,s,f,r,path=admit(ram,1<<channel,sr,mode);admissions+=1;record('admission',r,path,3,channel=channel,mode=mode,sr=hex(sr))
  owner=v.load(m,v.load(m,0x8c660880));rec=owner+0x210;task_sp=v.load(m,rec+0x18)
  fields=[v.load(m,0x8c6479bc)+0x80*channel,v.load(m,0x8c6479c4)+4*channel,v.load(m,0x8c6479c0),int.from_bytes(v.sample(m,0x8c6479b8,2),'little',signed=True)&0xffffffff,channel,0]
  for cycle in range(4):
   r,path,m,s,f=run(m,s,f,c,v.AI_SCHEDULE);assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'];n=3
   assert v.load(m,rec+0x20)==0x8c668e48 and v.load(m,v.load(m,rec+0x18))==0x8c647b74,(channel,cycle,hex(rec),hex(v.load(m,rec+0x20)),hex(v.load(m,rec+0x18)),hex(v.load(m,v.load(m,rec+0x18))));n+=2
   assert v.load(m,v.load(m,rec+0x18)+4)==fields[0] and v.load(m,rec+0x18+4)==v.load(m,0x8c668ea4);n+=2
   assert [v.load(m,rec+0x18+off) for off in (0x1c,0x20,0x18,0x14,0xc)]==fields[1:];n+=5
   assert v.load(m,rec+0xc)==1 and v.load(m,rec+0x14)==cycle+1 and v.load(m,owner+0x20c)==0;n+=3
   assert (0x8c647982 in path)==(cycle==0) and (0x8c647b74 in path)==(cycle>0);n+=2
   assert sum(b['target']=='0x8c66086a' for b in r['boundaries'])==1 and not any(b['target'] in ('0x8c647982','0x8c647b74') for b in r['boundaries']);n+=2
   assert v.load(m,rec+0x18)==task_sp-8;n+=1
   record('idle_cycle',r,path,n,channel=channel,cycle=cycle,mode=mode,sr=hex(sr))
  r,path,m,s,f=run(m,s,f,c,v.OWNER_ALL);assert r['status']==0 and v.sample(m,owner,0x2694)==bytes(0x2694);record('teardown',r,path,2,channel=channel,mode=mode,sr=hex(sr))
  print('WORKER IDLE',channel,mode,hex(sr),'4 real yields/resumes',flush=True)
  if '--smoke' in sys.argv:break
 # Direct command inputs are explicit test fixtures, independent of task admission.
 # Every task context/stack is still created by original constructors. No decoder
 # or callback result is supplied. Non-idle commands stop at actual missing code.
 command_cases=[]
 if '--smoke' not in sys.argv:
  for channel,mode in itertools.product((0,7),(0,4)):
   base_c,base_m,base_s,base_f,r,path=admit(ram,1<<channel,0x400000a1,mode);admissions+=1;record('command_admission',r,path,3,channel=channel,mode=mode)
   for command in tuple(range(21))+(21,0x7fffffff,0x80000000,0xffffffff):
    c=v.Input(dict(base_c));m=bytearray(base_m);s=type(base_s).from_buffer_copy(base_s);f=type(base_f).from_buffer_copy(base_f)
    owner=v.load(m,v.load(m,0x8c660880));rec=owner+0x210;sp=v.load(m,rec+0x18);channel_record=v.load(m,0x8c6479bc)+0x80*channel
    for j in range(0x80//4):put(m,channel_record+j*4,0xa5000000+j)
    put(m,channel_record+8,command);put(m,channel_record+0x14,PAYLOAD);put(m,PAYLOAD+0x28,0x12345678)
    r,path,m,s,f=run(m,s,f,c,v.AI_SCHEDULE);n=2;assert r['full_state_match'] and r['ordered_boundaries_match']
    assert (0x8c6479b4 in path)==(command<21);n+=1
    if command in COMMAND_TARGETS:
     target=COMMAND_TARGETS[command];assert r['status']==-2 and r['stop_pc']==hex(target);n+=2
     assert r['boundaries'][-1]['target']==hex(target) and s.r[4]==PAYLOAD;n+=2
     assert s.r[14]==channel_record and s.r[11]==channel and s.r[13]==0;n+=3
     expected_store=0x12345678 if command in (4,5,6) else PAYLOAD
     assert v.load(m,channel_record+0x10)==expected_store and v.load(m,channel_record+8)==command;n+=2
     args={1:[PAYLOAD],3:[PAYLOAD,channel_record+0x4c,v.load(m,channel_record+0x1c),channel_record+0x6c],4:[PAYLOAD,v.load(m,channel_record+0x18)],5:[PAYLOAD,v.load(m,channel_record+0x18),v.load(m,channel_record+0x1c)],6:[PAYLOAD,v.load(m,channel_record+0x18),v.load(m,channel_record+0x1c)],7:[PAYLOAD,channel_record+0x2c],8:[PAYLOAD,channel_record+0x2c,channel_record+0x3c],9:[PAYLOAD,v.load(m,channel_record+0x18)],10:[PAYLOAD,channel_record+0x2c,v.load(m,channel_record+0x1c),v.load(m,channel_record+0x20)],11:[PAYLOAD,channel_record+0x2c,v.load(m,channel_record+0x1c),v.load(m,channel_record+0x20)],12:[PAYLOAD,channel_record+0x2c,v.load(m,channel_record+0x1c),v.load(m,channel_record+0x20)],13:[PAYLOAD,channel_record+0x2c,v.load(m,channel_record+0x1c),v.load(m,channel_record+0x20)],14:[PAYLOAD,channel_record+0x2c,v.load(m,channel_record+0x1c),v.load(m,channel_record+0x20)],15:[PAYLOAD,v.load(m,channel_record+0x18)],16:[PAYLOAD,v.load(m,channel_record+0x18)],17:[PAYLOAD,v.load(m,channel_record+0x18),v.load(m,channel_record+0x1c)],18:[PAYLOAD],20:[PAYLOAD,channel_record+0x2c,v.load(m,channel_record+0x1c),v.load(m,channel_record+0x20)]}[command]
     assert list(s.r[4:4+len(args)])==args;n+=len(args)
     stacked={3:[v.load(m,channel_record+0x24)],12:[channel_record+0x6c,v.load(m,channel_record+0x28)],13:[channel_record+0x6c,v.load(m,channel_record+0x28)],14:[v.load(m,channel_record+0x24)],20:[v.load(m,channel_record+0x24)]}.get(command,[])
     assert s.r[15]==sp-4*len(stacked) and [v.load(m,s.r[15]+j*4) for j in range(len(stacked))]==stacked;n+=1+len(stacked)
    else:
     assert r['status']==0 and v.load(m,channel_record+8)==command and v.load(m,rec+0x20)==0x8c668e48;n+=3
    command_cases.append(dict(channel=channel,mode=mode,command=command,target=r['stop_pc']));record('command',r,path,n,channel=channel,mode=mode,command=hex(command))
   print('WORKER COMMANDS',channel,mode,'all21 plus unsigned out-of-range',flush=True)
 if '--smoke' in sys.argv:return
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};miss_edges=set(map(tuple,body['conditional_edges']))-edges
  if body['address']!='0x8c647980':assert not missing and not miss_edges
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),missing_rows=sorted(missing),missing_outcomes=sorted(miss_edges),complete=not missing and not miss_edges))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_channel_worker_connection.json',v.effects.DLL]
 report=dict(original_native_comparisons=comparisons,independent_checks=checks,original_admissions=admissions,cases=records,commands=command_cases,coverage_by_routine=coverage,newly_registered_entries=0,compiled_source_count=219,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actual original6477cc admission initializes all8 selector contexts/stacks.32 standalonechannel lifecycles with4 real scheduler/yield/resume cycles each thenactualteardown. All21commands plus4unsignedout-of-range values atchannels0/7 andFR0/FR1;commandwords/payloads areexplicitfixtures,never task-context repairs or decoderresults. Allnonidle18commands stop at realunrecovereddecoder boundaries. WholeRAM/devices/CPU/FPU/ordered-boundary matching and independent continuation/deadline/args/stack/store oracles. Completeworkerpost-decoder/callback behavior remainsunqualified;no newregistrycredit,no physicalcontrollerclaim,no guestservice substitute.')
 (P/'analysis/movie_channel_worker_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CHANNEL WORKER PASS',comparisons,checks,flush=True)
if __name__=='__main__':main()
