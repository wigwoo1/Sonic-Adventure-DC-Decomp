"""Preserve published code behavior when the optional persistent service is absent."""
import gc,hashlib,json,types
from pathlib import Path
import write_watch_transfer_runtime as fast
import verify_movie_transfer_dispatch as dispatch
import verify_movie_transfer_interrupt as irq
import verify_movie_cpu_transfer as cpu
import verify_movie_interrupt_context as context
v=fast.old;P=v.P;ROOT=v.ROOT
def main():
 records=[];files=[Path(__file__),Path(v.__file__),Path(fast.__file__),P/'analysis/movie_transfer_runtime_connection.json'];checks=0;ram=v.seed_ram();coverage=set();edges=set();request_rows=set(v.PER_BODY[cpu.v.REQUEST])
 for family,module,expected in (('movie_transfer_dispatch',dispatch,137),('movie_transfer_interrupt',irq,110),('movie_cpu_transfer',cpu,240),('movie_interrupt_context',context,107)):
  path=P/f'analysis/{family}_verification.json';saved=json.loads(path.read_text());files.append(path);inputs=list(module.cases());assert len(inputs)==len(saved['cases'])==expected
  semantics=types.FunctionType(module.semantics.__code__,dict(module.semantics.__globals__,fast=fast),'semantics')
  if family=='movie_interrupt_context':semantics=types.FunctionType(module.semantics.__code__,dict(module.semantics.__globals__,fast=fast,v=v),'semantics')
  for c,prior in zip(inputs,saved['cases']):
   m,s,f=module.v.prepare(ram,c);v.effects.CONNECTED.update(module.v.effects.CONNECTED);before=v.effects.State.from_buffer_copy(s);fp=bytes(f)
   r,path,a,s,f=fast.compare(m,s,f,c)
   count=semantics(m,a,before,fp,s,f,r,path,c) if family=='movie_interrupt_context' else semantics(m,a,before,s,r,path,c)
   r.update(independent_checks=count,guest_r0=hex(s.r[0]),guest_sr=hex(s.sr))
   for key,value in prior.items():assert json.loads(json.dumps(r[key]))==value,(family,key,c);checks+=1
   assert r['runtime_state'] is None
   coverage.update(set(path)&request_rows)
   for i,pc in enumerate(path):
    text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
    if pc in request_rows and text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
   records.append(dict(family=family,entry=hex(c['entry']),status=r['status'],stop_pc=r['stop_pc']));del r,path,a;gc.collect()
  print('Replayed',family,expected,flush=True)
 waits=[]
 for mode in (0,4):
  c=v.Input(entry=cpu.v.REQUEST,mode=mode,sdk_mode=1);m,s,f=cpu.v.prepare(ram,c);r,path,a,s,f=fast.compare(m,s,f,c)
  assert r['status']==-4 and r['stop_pc']=='0x8c65a6ec' and v.load(a,v.RECORD+24)==0 and v.HANDLER not in path;checks+=4
  assert r['runtime_state'] is None and not any(r['sound_state']);checks+=2
  waits.append(dict(mode=mode,status=r['status'],stop_pc=r['stop_pc']))
 report=dict(previous_dispatch_comparisons=137,previous_handler_comparisons=110,previous_cpu_comparisons=240,previous_context_comparisons=107,retained_wait_comparisons=2,total_comparisons=len(records)+len(waits),independent_and_record_checks=checks,cases=records,wait_cases=waits,request_executed_rows=[hex(a) for a in sorted(coverage)],request_executed_branch_outcomes=[[hex(a),hex(b)] for a,b in sorted(edges)],dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='594previously published own-body cases and2default wait controls replay saved results and full original/native machine/device state unchanged with no persistent service attached. All earlier original sources/proofs/builds preserved. Partial bootstrap retains actualac000010 frontier. No new earliest-caller/full-copy/video-clear proof claimed.')
 (P/'analysis/movie_transfer_runtime_regression_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Runtime regressions',report['total_comparisons'],'checks',checks)
if __name__=='__main__':main()
