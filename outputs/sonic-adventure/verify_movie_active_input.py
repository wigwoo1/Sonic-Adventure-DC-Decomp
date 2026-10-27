"""Original scheduler, cleanup and context-restore machine/footprint oracles."""
import hashlib,itertools,json,sys
from pathlib import Path
import write_watch_active_input as fast
import movie_active_input_fixtures as fixtures
v=fast.old;P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def case(entry,mode=0,sr=0x400000a1,**data):return dict(entry=entry,mode=mode,sr=sr,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234)|data
def cases():
 for mode in range(8):
  yield case(v.AI_SCHEDULE,mode,task_count=-1,states=[])
  yield case(v.AI_SCHEDULE,mode,states=[])
  for states in ([0],[2],[3],[4],[0,2,3,4],[3,3],[1],[1,0,2,3,4]):yield case(v.AI_SCHEDULE,mode,states=states)
  for counter,deadline in ((0,0),(7,7),(7,6),(0xffffffff,0xffffffff),(0xffffffff,0),(0x80000000,0x7fffffff),(0,0xffffffff)):
   yield case(v.AI_SCHEDULE,mode,states=[1],counter=counter,deadline=deadline)
  yield case(v.AI_SCHEDULE,mode,states=[1],deadline=7,resume=True)
  yield case(v.AI_SCHEDULE,mode,states=[3],callback=v.CD_NOP)
  yield case(v.AI_SCHEDULE,mode,states=[1],deadline=7,unknown_context=True)
 for mode,live,callback in itertools.product((0,4,7),(0,1,0xffffffff),(0,v.CD_NOP,0x8cfef000)):yield case(v.AI_CLEANUP,mode,live=live,callback=callback)
 for mode,pattern,value,fpscr in itertools.product(range(8),(0,0x80000000,0xffffffff,0x7fa12345),(0,1,0xffffffff),(0x40001,0x3f07f)):
  yield case(v.AI_RESTORE,mode,pattern=pattern,value=value,restored_fpscr=fpscr)
 for mode in (0,4,7):yield case(v.AI_RESTORE,mode,value=0,unknown_context=True)
 for entry in (v.AI_SCHEDULE,v.AI_RESTORE):
  for mode in (0,7):yield case(entry,mode,0x400080a1,states=[],value=7,fault=True)
def main():
 selected=list(cases());records=[];checks=0;seen=set();edges=set();ram=v.seed_ram()
 if '--smoke' in sys.argv:
  selected=[selected[i] for i in (1,6,11,17,18,19,160,161)]+[next(c for c in selected if c['entry']==v.AI_RESTORE and c['mode']==7)]+[next(c for c in selected if c.get('fault'))]
 for i,data in enumerate(selected,1):
  c=v.Input(data);m,s,f=fixtures.prepare(ram,c);assert all(c[k]==x for k,x in data.items());before=v.effects.State.from_buffer_copy(s);fp=type(f).from_buffer_copy(f);entry=c['entry'];expected_status=-4 if c.get('fault') else -2 if c.get('unknown_context') or c.get('callback')==0x8cfef000 else 0
  r,path,a,s,f=fast.compare(m,s,f,c);seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');delta=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+delta<len(path):edges.add((hex(pc),hex(path[j+delta])))
  assert r['status']==expected_status and r['full_state_match'] and r['ordered_boundaries_match'],(i,data,r['status'],r['stop_pc']);n=3
  if c.get('fault'):
   wanted=bytearray(m)
   if entry==v.AI_RESTORE:
    fixtures.put(wanted,v.load(m,0x8c110950),before.r[5]);assert s.r[4]==before.r[4]+0x50 and r['stop_pc']=='0x8c1108e6';n+=2
   else:
    for j,reg in enumerate((14,13,12,11,10,9),1):fixtures.put(wanted,before.r[15]-j*4,before.r[reg])
    fixtures.put(wanted,before.r[15]-28,before.pr);fixtures.literal(wanted,0x8c668eb4,v.load(m,v.load(m,0x8c668ea4)));assert s.r[15]==before.r[15]-32 and r['stop_pc']=='0x8c668e80';n+=2
   assert a==wanted and bytes(f)==bytes(fp);n+=2
  elif entry==v.AI_RESTORE:
   wanted,regs,sr,pr,newfp,special=fixtures.restore_expected(m,before,fp,c)
   assert a==wanted and list(s.r)==regs and s.sr==sr and s.pr==pr and s.pc==pr; n+=5
   assert bytes(f)==bytes(newfp) and [r[k] for k in ('final_mach','final_macl','final_gbr','final_fpul')]==special;n+=2
  elif entry==v.AI_CLEANUP:
   wanted=bytearray(m);fs=fixtures.fields(m);fixtures.put(wanted,fixtures.ROOT+fs['live'],(c['live']-1)&0xffffffff);fixtures.put(wanted,fixtures.RECORD+0xc,0);fixtures.put(wanted,before.r[15]-4,before.r[14]);assert a==wanted and bytes(f)==bytes(fp);n+=2
   if not expected_status:assert list(s.r[4:])==list(before.r[4:]) and s.pc==s.pr==before.pr;n+=2
   else:assert r['stop_pc']=='0x8cfef000';n+=1
  else:
   wanted=fixtures.scheduler_expected(m,before,fp,c)
   if a!=wanted:
    diffs=[hex(0x8c000000+j) for j,(x,y) in enumerate(zip(a,wanted)) if x!=y];raise AssertionError(('scheduler footprint',i,data,diffs[:32]))
   assert a==wanted;n+=1
   if not expected_status:
    assert list(s.r[8:])==list(before.r[8:]) and s.pc==s.pr==before.pr and s.sr==before.sr|1 and bytes(f)==bytes(fp);n+=4
    assert v.load(a,fixtures.ROOT+fixtures.fields(m)['counter'])==(c.get('counter',7)+1)&0xffffffff;n+=1
   else:assert r['stop_pc']=='0x8cfef000';n+=1
  checks+=n;records.append(dict(input=data,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n))
  if i%50==0 or '--smoke' in sys.argv:print('ACTIVE INPUT',i,hex(entry),r['status'],flush=True)
 if '--smoke' in sys.argv:print('ACTIVE INPUT SMOKE PASS',len(records));return
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges;assert not missing and not missing_edges,(body['address'],missing,missing_edges)
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows'],executed_outcomes=len(body['conditional_edges']),missing_rows=[],missing_outcomes=[]))
 files=[Path(__file__),Path(fixtures.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_active_input_connection.json',v.effects.DLL,P/'analysis/active_input_context_primary_evidence.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Three original bodies158rows16outcomes. Full original/native CPU/FPU/RAM/device/ordered comparison;independent entireRAM scheduler/save/cleanup footprints and decoded contextrestore CPU/FPU/banks/specialstate. Signed loopcounts,unsigned deadlines,wrap,all8FPUmodes,realNOP/null/unknown callbacks,actual contexttransfer to668e92 with restored completed-iteration loop state,unknown resume target and reachedFDfault prefixes. Declared coherent disjoint scheduler/context records and valid stack;not natural task acquisition/full scheduler initialization or arbitrary coroutine/gameplay proof. Status3 is internal unwind to actual encoded scheduler continuation,no CPU-state/result substitution. Existing guards/runtime/ABI unchanged outside new contextrestore RTS semantics.')
 (P/'analysis/movie_active_input_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('ACTIVE INPUT DIRECT PASS',len(records),checks)
if __name__=='__main__':main()
