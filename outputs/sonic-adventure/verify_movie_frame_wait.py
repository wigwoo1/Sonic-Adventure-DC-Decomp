"""Real display/queue/wait comparisons with independent RAM and call oracles."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_frame_wait as fast
import movie_frame_wait_fixtures as fixtures
v=fast.old;P=v.P;put=fixtures.put
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
CONFIGS=((0,0),(1,0x400000f3),(4,0x600003a1),(7,0x40008000))
def case(entry,mode=0,sr=0x400000a1,**extra):return dict(entry=entry,mode=mode,sr=sr,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234)|extra
def cases():
 for (mode,sr),flag,bank,index in itertools.product(CONFIGS,(0,1,2,0xffffffff),(0,1),(0,1,2)):yield case(v.DISPLAY_SYNC,mode,sr,flag=flag,bank=bank,index=index,pattern=0x12345678^index)
 for (mode,sr),counter in itertools.product(CONFIGS,(1,2,0x7fffffff)):yield case(v.FRAME_WAIT,mode,sr,counter=counter)
 for entry,(mode,sr),payload in itertools.product((v.RECORD_FIELDS,v.TRIPLET,v.ERROR_RESET),CONFIGS,(0,0xffffffff,0x89abcdef)):yield case(entry,mode,sr,payload=payload,flags=payload)
 for (mode,sr),count in itertools.product(CONFIGS,(0,0xffffffff,0x80000000)):yield case(v.BATCH,mode,sr,count=count)
 for (mode,sr),release in itertools.product(CONFIGS,(False,True)):yield case(v.BATCH,mode,sr,count=1,release=release)
 for mode,sr in CONFIGS:
  yield case(v.SUBMIT,mode,sr,count=0)
  for flags in (0,0xffffffff):yield case(v.SUBMIT,mode,sr,count=1,flags=flags)
  for flags,index in itertools.product((0,0xffffffff),(0,1,2)):yield case(v.RESOURCE_DISPATCH,mode,sr,flags=flags,index=index)
  for count in (0,1,2):yield case(v.RELEASE,mode,sr,count=count)
  for flags in (1,2):yield case(v.RESOURCE_RELEASE,mode,sr,flags=flags)
def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=fixtures.prepare(ram,c);assert all(c[key]==value for key,value in data.items()),('requested fixture overwritten',data,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f);entry=c['entry'];wanted=None;expected_status=0
  if entry==v.DISPLAY_SYNC:wanted,writes=fixtures.display_expected(m,before,c)
  if entry==v.FRAME_WAIT:expected_status=-4
  if entry in (v.RESOURCE_DISPATCH,) or entry==v.SUBMIT and c['count'] or entry==v.BATCH and c['count']==1 or entry==v.RESOURCE_RELEASE and c['flags']==2:expected_status=-2
  r,path,a,s,f=fast.compare(m,s,f,c);seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');delta=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+delta<len(path):edges.add((pc,path[j+delta]))
  assert r['status']==expected_status and r['full_state_match'] and r['ordered_boundaries_match'],(i,c,r['status'],r['stop_pc']);n=3
  assert bytes(f)==fp and r['final_macl']==c['macl'] and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'];n+=4
  if entry==v.DISPLAY_SYNC:
   assert a==wanted and list(s.r[8:])==list(before.r[8:]) and s.pc==s.pr==before.pr;n+=3
   actual=[tuple(b['arguments'][:2]) for b in r['boundaries'] if b['target']=='0x8c653444'];assert actual==writes;n+=1
   reads=[b['arguments'][0] for b in r['boundaries'] if b['target']=='0x8c653476'];assert reads==([writes[-1][0]] if writes else []);n+=1
   assert r['boundaries'][-1]['target']=='0x8c6564e0';n+=1
  elif entry==v.FRAME_WAIT:
   wanted=bytearray(m);put(wanted,before.r[15]-4,before.pr);regs=list(before.r);regs[15]-=4;regs[4]=v.load(m,0x8c051884);regs[2]=c['counter']
   assert a==wanted and list(s.r)==regs and s.pr==before.pr and s.sr==before.sr|1 and s.pc==0x8c051798;n+=5
   assert path.count(0x8c051798)==2 and len(r['boundaries'])==1;n+=2
  elif entry in (v.RECORD_FIELDS,v.TRIPLET,v.ERROR_RESET):
   wanted=bytearray(m);regs=list(before.r);sr=before.sr
   if entry==v.RECORD_FIELDS:
    regs[6]=(regs[6]<<16)&0xffffffff;regs[3]=v.load(m,regs[15]);put(wanted,regs[4],regs[5]);put(wanted,regs[4]+16,regs[7]);put(wanted,regs[4]+20,regs[3]);put(wanted,regs[4]+4,regs[6])
   elif entry==v.TRIPLET:
    regs[3]=v.load(m,0x8c608930);sr=(sr&~1)|int(regs[7]&regs[3]==0)
    for off,value in ((0,regs[5]),(4,regs[7]),(8,regs[6])):put(wanted,regs[4]+off,value)
   else:
    regs[4]=v.load(m,0x8c608e70);regs[5]=0
    for off,value in enumerate((0xffffffff,0xffffffff,0,0,0)):put(wanted,regs[4]+off*4,value)
   assert a==wanted and list(s.r)==regs and s.sr==sr and s.pc==s.pr==before.pr;n+=4
  elif entry==v.BATCH:
   if c['count']!=1:
    wanted=bytearray(m)
    for j,reg in enumerate(range(14,7,-1),1):put(wanted,before.r[15]-j*4,before.r[reg])
    put(wanted,before.r[15]-32,before.pr)
    if c['count']:put(wanted,v.load(m,0x8c055d08),0)
    assert a==wanted and list(s.r[8:])==list(before.r[8:]) and s.pc==s.pr==before.pr;n+=3
   else:
    assert v.load(a,fixtures.RECORD+0x28)==fixtures.RECORD+0x30 and v.load(a,fixtures.RECORD+0x2c)==1;n+=2
    assert [v.load(a,fixtures.RECORD+0x30+j*4) for j in range(3)]==[before.r[15]-76,0x40800000,0x12345678];n+=1
    stack=before.r[15]-76;assert [v.load(a,stack+off) for off in (0,4,16,20)]==[0x8cd83000,0x09000000,0xfedc,0x89ab];n+=1
    assert r['stop_pc'] in ('0x8c64c374','0x8c64d1a6') and v.load(a,v.load(m,0x8c055d08))==1;n+=2
  elif entry==v.SUBMIT:
   error=v.load(m,0x8c608e70);assert [v.load(a,error+j*4) for j in range(5)]==[0xffffffff,0xffffffff,1 if not c['count'] else 0,0,0];n+=1
   if not c['count']:assert s.r[0]==0xffffffff and s.pc==s.pr==before.pr;n+=2
   else:assert r['stop_pc'] in ('0x8c64c374','0x8c64d1a6');n+=1
  elif entry==v.RESOURCE_DISPATCH:
   wanted=bytearray(m);put(wanted,before.r[15]-4,before.r[14]);assert a==wanted and s.r[14]==before.r[14] and s.r[15]==before.r[15] and s.pr==before.pr;n+=4
   assert r['stop_pc'] in ('0x8c64c374','0x8c64d1a6') and len(r['boundaries'])==2;n+=2
  elif entry==v.RELEASE:assert s.r[0]==(0xffffffff if c['count'] else 1) and s.pc==s.pr==before.pr;n+=2
  elif entry==v.RESOURCE_RELEASE:
   if c['flags']==1:assert s.r[0]==0xffffffff and s.pc==s.pr==before.pr;n+=2
   else:assert r['stop_pc']=='0x8c64f206';n+=1
  checks+=n;records.append(dict(input=data,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n))
  if i%25==0:print('FRAME DIRECT',i,flush=True)
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};missing_edges=set(map(tuple,body['conditional_edges']))-{(hex(a),hex(b)) for a,b in edges}
  if int(body['address'],16)==v.DISPLAY_SYNC:assert not missing and not missing_edges
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),missing_rows=sorted(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_outcomes=sorted(missing_edges)))
 files=[Path(__file__),Path(fixtures.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_frame_wait_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,qualified_new_entries=['0x8c6433e0'],partial_entries=['0x8c051790','0x8c055c4c','0x8c608c82','0x8c64c34c'],dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actual original display/queue/wait/resource callees; no mock results or forced wait progress. Full original/native machine/boundary comparison; independent fullRAM/stack/data/MMIO-call and preserved-register expectations as specified per case. Display66rows/twooutcomes fully execute. Positive counter waits stop incomplete-4 aftertwo tests, unchanged counter. Queue/resource missing callees propagate-2 with actual preceding effects; no completeness credit. Explicit standalone fixtures, not game startup/physical timing/reset proof.')
 (P/'analysis/movie_frame_wait_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('FRAME DIRECT PASS',len(records),checks)
if __name__=='__main__':main()
