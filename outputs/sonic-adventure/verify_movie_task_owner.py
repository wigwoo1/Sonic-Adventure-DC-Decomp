"""Original owner/pool teardown: complete memory and ordered helper expectations."""
import hashlib,itertools,json,sys
from pathlib import Path
import write_watch_task_owner as fast
import movie_task_owner_fixtures as fixtures
v=fast.old;P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def cases():
 for entry,mode,sr in itertools.product((v.OWNER_ONE,v.OWNER_ALL),(0,4),(0x400000a1,0x600000a1,0x400080a1,0x21)):
  for count,states in ((-1,[]),(0,[]),(1,[0]),(1,[1]),(3,[1,0,3])):
   yield dict(entry=entry,mode=mode,sr=sr,task_count=count,states=states,mask=0x81)
 for mask,callback in itertools.product((0,1,0x80,0x55,0xaa,0xff),(0,v.CD_NOP)):yield dict(entry=v.OWNER_ALL,mode=0,sr=0x400000a1,states=[0,2,0xffffffff],mask=mask,callback=callback)
 for entry in (v.OWNER_ONE,v.OWNER_ALL):yield dict(entry=entry,mode=0,sr=0x400000a1,states=[0,1,3],mask=0x84,callback=0x8cfef000,counter=0xffffffff)
def main():
 selected=list(cases());records=[];checks=0;seen=set();edges=set();ram=v.seed_ram()
 if '--smoke' in sys.argv:selected=[selected[j] for j in (0,4,40,44,80,84,91,92,93)]
 for i,data in enumerate(selected,1):
  c=v.Input(dict(mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234)|data);m,s,f=fixtures.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f);wanted,regs,sr,status,cleanup,owners=fixtures.expected(m,before,c)
  r,path,a,s,f=fast.compare(m,s,f,c);seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');delta=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+delta<len(path):edges.add((hex(pc),hex(path[j+delta])))
  assert r['status']==status and r['full_state_match'] and r['ordered_boundaries_match'],(i,data,r['status'],r['stop_pc']);n=3
  if a!=wanted:raise AssertionError(('footprint',data,[hex(0x8c000000+j) for j,(x,y) in enumerate(zip(a,wanted)) if x!=y][:32]))
  assert a==wanted and bytes(f)==fp;n+=2
  targets=[b['target'] for b in r['boundaries']];assert targets.count('0x8c668cca')==len(cleanup);n+=1
  assert v.load(a,v.load(m,0x8c668c54))==0x12345678;n+=1
  if not status:
   assert list(s.r)==regs and s.sr==sr and s.pc==s.pr==before.pr;n+=3
   assert targets.count('0x8c10d31c')==len(owners);n+=1
  else:assert r['stop_pc']=='0x8cfef000' and '0x8c10d31c' not in targets;n+=2
  checks+=n;records.append(dict(input=data,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),cleanup_records=[hex(x) for x in cleanup],cleared_owners=[hex(x) for x in owners] if not status else [],independent_checks=n))
  if '--smoke' in sys.argv:print('OWNER SMOKE',i,hex(c['entry']),status,flush=True)
 if '--smoke' in sys.argv:return
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges;assert not missing and not missing_edges,(body['address'],missing,missing_edges)
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows'],executed_outcomes=len(body['conditional_edges']),missing_rows=[],missing_outcomes=[]))
 files=[Path(__file__),Path(fixtures.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_task_owner_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Owner/poolteardown complete original/native comparisons. Independent entireRAM including allstackwords/retainedinactiveowners,fullGPR/SR success expectations,realcleanup/clearcallcounts,callbacknull/realNOP/unknownstops,signedcapacities,multipleowner masks,FD/user/SRbank configurations. Counterdecrements preserve originalwrap untilownerzeroing;externalpoolcounter unchanged. Explicit valid bounded ownerrecords/disjointstacks;not arbitrarycallback/resource graphs orprovedbookkeepingmeaning. No counter/result substitutes.')
 (P/'analysis/movie_task_owner_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('TASK OWNER DIRECT PASS',len(records),checks)
if __name__=='__main__':main()
