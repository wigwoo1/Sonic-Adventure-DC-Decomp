"""Real input-channel admission and teardown; preserve the original counter."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_task_owner as fast
v=fast.old;P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 records=[];comparisons=0;checks=0;ram=v.seed_ram()
 for mask,sr,prefix in [(m,s,False) for m,s in itertools.product((0,1,0x80,0x55,0xff,0x100,0xffffffff),(0x400000a1,0x600000a1))]+[(1,0x400000a1,True),(0x80,0x600000a1,True)]:
  c=v.Input(dict(entry=v.REGISTER_CHANNELS,mode=0,sr=sr,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,register_arguments=[mask,0,0,0]));m,s,f=v.prepare(ram,c);f.fpscr=0x40001
  r,path,m,s,f=fast.compare(m,s,f,c);comparisons+=1;assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'];n=3
  owner=v.load(m,v.load(m,0x8c660880));selected=[j for j in range(8) if mask&(1<<j)]
  assert owner==v.load(m,0x8c668c44) and [v.load(m,owner+o) for o in (0,0x204,0x208,0x20c)]==[1,0,8,0];n+=5
  assert sum(v.load(m,owner+0x210+j*0x124+12)!=0 for j in range(8))==len(selected);n+=1
  length=int.from_bytes(v.sample(m,0x8c647846,2),'little',signed=True)
  for record_index,channel in enumerate(selected):
   rec=owner+0x210+record_index*0x124;payload=v.load(m,0x8c647934+channel*8);target=v.load(m,0x8c647938+channel*8)
   assert [v.load(m,rec+off) for off in (0,4,0xc,0x18,0x20,0x118)]==[payload,length//4,1,payload+length-4,target,0];n+=6
  assert sum(b['target']=='0x8c660842' for b in r['boundaries'])==len(selected);n+=1
  admission_counter=v.load(m,owner+0x20c);c['entry']=v.AI_SCHEDULE if prefix else v.OWNER_ALL;s.pc=c['entry'];s.pr=0x8cfed000;s.r[4]=owner
  result,path,m,s,f=fast.compare(m,s,f,c);comparisons+=1;assert result['full_state_match'] and result['ordered_boundaries_match'];n+=2
  if prefix:
   target=v.load(m,0x8c647938+selected[0]*8);assert result['status']==-2 and result['stop_pc']==hex(target) and v.load(m,owner+0x20c)==0;n+=3
  else:
   assert result['status']==0 and v.sample(m,owner,0x2694)==bytes(0x2694);n+=2
   assert sum(b['target']=='0x8c668cca' for b in result['boundaries'])==len(selected);n+=1
  checks+=n;records.append(dict(mask=hex(mask),sr=hex(sr),prefix=prefix,task_count=len(selected),owner=hex(owner),owner_counter_after_admission=admission_counter,final_status=result['status'],stop_pc=result['stop_pc'],original_native_comparisons=2,independent_checks=n))
  print('OWNER CALLER',hex(mask),'tasks',len(selected),'counter',admission_counter,'status',result['status'],flush=True)
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_task_owner_connection.json',v.effects.DLL,P/'analysis/movie_task_owner_contract_audit.json']
 report=dict(lifecycles_passed=len(records),original_native_comparisons=comparisons,independent_checks=checks,cases=records,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actual6477cc input-channel admission creates0..8 originaltasks via660842/668bee fromretailpayload/tasktables;no fabricatedrecords orcounterupdates. Owner+20c stays0 for testedmasks,includingall8channels. Actualpoolteardown cleans allcreatedrecords andclearsowner. Two freshoriginalscheduler prefixes thenstop at actualmissing realtaskentries64790e/647980;nothelpercompletion substitutions. Wholeoriginal/native machine/device/ordered state andindependentmetadata/records/SP/PR/cleanupcounts checked. Taskworkerexports notnativecredit,notphysicalcontroller acquisition/gameboot. Fieldsemantics/globalwritercontractremainunqualified.')
 (P/'analysis/movie_task_owner_callers_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('TASK OWNER CALLERS PASS',len(records),comparisons,checks)
if __name__=='__main__':main()
