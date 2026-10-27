"""Shared yield/termination frames and deadline/context oracles."""
import hashlib,itertools,json,sys
from pathlib import Path
import write_watch_task_yield as fast
import movie_task_yield_fixtures as fixtures
v=fast.old;P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def cases():
 for entry,mode,counter in itertools.product((v.YIELD,v.FINISH,v.YIELD_ONE),(0,4),(0,0xffffffff,0x7fffffff)):
  for delay in ((0,1,7,0x7fffffff,0x80000000,0xffffffff) if entry==v.YIELD else (0,)):yield dict(entry=entry,mode=mode,counter=counter,delay=delay)
 for mode,value in itertools.product((0,4),(0,1,0xffffffff)):yield dict(entry=v.YIELD_RESUME,mode=mode,value=value)
 for mode in (0,4):yield dict(entry=v.YIELD_RESUME,mode=mode,value=1,unknown=True)
 for entry in (v.YIELD,v.FINISH,v.YIELD_ONE):yield dict(entry=entry,mode=0,unknown_restore=True)
 for entry in (v.YIELD,v.FINISH,v.YIELD_ONE):yield dict(entry=entry,mode=0,sr=0x400080a1,fault=True)
def main():
 records=[];checks=0;seen=set();edges=set();ram=v.seed_ram();selected=list(cases())
 if '--smoke' in sys.argv:selected=[selected[j] for j in (0,4,36,42,48,51,55,58,60)]
 for i,data in enumerate(selected,1):
  c=v.Input(dict(sr=0x400000a1,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234)|data);m,s,f=fixtures.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=type(f).from_buffer_copy(f)
  r,path,a,s,f=fast.compare(m,s,f,c);seen.update(path)
  for j,pc in enumerate(path):
   if v.ROWS.get(pc,'').startswith(('bt ','bf ')) and j+1<len(path):edges.add((hex(pc),hex(path[j+1])))
  status=-4 if c.get('fault') else -2 if c.get('unknown') or c.get('unknown_restore') else 0
  assert r['status']==status and r['full_state_match'] and r['ordered_boundaries_match'],(i,data,r['status'],r['stop_pc']);n=3
  if c.get('fault'):
   assert r['stop_pc']=='0x8c11098a' and v.load(a,fixtures.RECORD+0x18+8)==0x8c668e48 and bytes(f)==bytes(fp);n+=3
  else:
   wanted,regs,sr,pr,newfp,special=fixtures.expected(m,before,fp,c)
   if a!=wanted:raise AssertionError(('footprint',data,[hex(0x8c000000+j) for j,(x,y) in enumerate(zip(a,wanted)) if x!=y][:24]))
   assert a==wanted and list(s.r)==regs and s.sr==sr and s.pr==pr and s.pc==pr and bytes(f)==bytes(newfp);n+=6
   assert [r[k] for k in ('final_mach','final_macl','final_gbr','final_fpul')]==special;n+=1
  checks+=n;records.append(dict(input=data,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n))
  if '--smoke' in sys.argv:print('YIELD SMOKE',i,hex(c['entry']),status,flush=True)
 if '--smoke' in sys.argv:return
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};miss_edges=set(map(tuple,body['conditional_edges']))-edges;assert not missing and not miss_edges,(body['address'],missing,miss_edges)
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows'],executed_outcomes=len(body['conditional_edges']),missing_rows=[],missing_outcomes=[]))
 files=[Path(__file__),Path(fixtures.__file__),Path(fixtures.old.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_task_yield_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Direct original/native deadline,termination,prefix/shared-tail and resumed-yield comparisons. Independent entireRAM/stack/task-contextsave/root-contextrestore/GPR/FPU/specialstate expectations;unsignedwrap/signeddelay branches,realunknownrestore/return targets and reachedFDfaults. Explicit coherent standaloneframes,initialSZ0 FR0/FR1;allwidth retained controls and originalconstructor lifecycles separate. Shared31rows creditedonce;668e48 continuation no extra routine credit. Not physicalframe timing or arbitrary task-code/interior continuations.')
 (P/'analysis/movie_task_yield_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('TASK YIELD DIRECT PASS',len(records),checks)
if __name__=='__main__':main()
