"""Replay prior transfer/copy controls against the new handler-connected DLL."""
import gc,hashlib,json,types
from pathlib import Path
import write_watch_transfer_interrupt as fast
import verify_movie_cpu_transfer as cpu
import verify_movie_transfer_start as transfer
v=fast.old;P=v.P;ROOT=v.ROOT

def main():
 old_bridge=P/'source/sa_movie_cpu_transfer_connected.c';new_bridge=P/'source/sa_movie_transfer_interrupt_connected.c';text=new_bridge.read_text()
 text=text.replace('sa_movie_transfer_interrupt_connected','sa_movie_cpu_transfer_connected').replace('sa_movie_transfer_interrupt_initialized_connected','sa_movie_cpu_transfer_initialized_connected')
 routine=v.manifest['routines'][0];declaration=f'VDECL({routine["name"]});\n';assert text.count(declaration)==1;text=text.replace(declaration,'')
 route=f' if(target=={routine["address"]}u)return {routine["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n';assert text.count(route)==1;text=text.replace(route,'')
 assert text==old_bridge.read_text()
 ram=v.seed_ram();checks=0;records=[];files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,old_bridge,new_bridge]
 baseline_path=P/'analysis/movie_cpu_transfer_regression_verification.json';baseline=json.loads(baseline_path.read_text());files.append(baseline_path)
 endpoints=[row['current'] for row in baseline['advanced_cases'] if row['family']=='movie_transfer_start'];assert len(endpoints)==4
 current_endpoint_checks=0
 for family,module,expected in (('movie_cpu_transfer',cpu,240),('movie_transfer_start',transfer,399)):
  prior_path=P/f'analysis/{family}_verification.json';prior=json.loads(prior_path.read_text());files.append(prior_path)
  inputs=list(module.cases());assert len(inputs)==len(prior['cases'])==expected
  semantics=types.FunctionType(module.semantics.__code__,dict(module.semantics.__globals__,fast=fast),'semantics')
  for c,saved in zip(inputs,prior['cases']):
   m,s,f=module.v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);r,path,a,s,f=fast.compare(m,s,f,c)
   if family=='movie_transfer_start' and saved['stop_pc']=='0x8c666e06':
    # These four cases advanced in the ALREADY published CPU-transfer milestone.
    # Compare that current baseline, retaining the original unmodified inputs.
    current=next(row for row in endpoints if row['input']==dict(c))
    for key,value in current.items():assert json.loads(json.dumps(r[key]))==value,(family,key,c);checks+=1
    current_endpoint_checks+=1
   else:
    r.update(independent_checks=semantics(m,a,before,s,r,path,c),guest_r0=hex(s.r[0]),guest_sr=hex(s.sr))
    for key,value in saved.items():assert json.loads(json.dumps(r[key]))==value,(family,key,c);checks+=1
   records.append(dict(family=family,entry=hex(c['entry']),status=r['status'],stop_pc=r['stop_pc']));del r,path,a;gc.collect()
  print('Replayed',family,expected,flush=True)
 waits=[]
 for mode in (0,4):
  c=v.Input(entry=v.old.REQUEST,mode=mode,sdk_mode=1)
  m,s,f=v.old.prepare(ram,c);r,path,a,s,f=fast.compare(m,s,f,c)
  assert r['status']==-4 and r['stop_pc']=='0x8c65a6ec';checks+=2
  assert v.load(a,v.RECORD+4)==2 and v.load(a,v.RECORD+24)==0;checks+=2
  assert not any(r['sound_state']) and v.HANDLER not in path;checks+=2
  waits.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],boundary_comparisons=r['boundary_comparisons']))
 assert current_endpoint_checks==4
 report=dict(unchanged_prior_comparisons=len(records),original_saved_record_comparisons=len(records)-current_endpoint_checks,published_cpu_endpoint_comparisons=current_endpoint_checks,retained_wait_comparisons=len(waits),total_comparisons=len(records)+len(waits),independent_and_record_checks=checks,cases=records,wait_cases=waits,original_bridge_preserved_except_one_route=True,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='240 prior synchronous/counter and399 transfer/cache/callback comparisons reexecute unchanged against the current published baseline and full original/native states:635 older saved records and4endpoint records already advanced in the previous CPU-transfer milestone. Two fresh default-mode-one REQUEST controls retain genuine wait65a6ec,state2/done0 and untouched SoundRAM; handler is not automatically delivered. Earlier full2MiB copies and earliest-caller/full-video-clear proofs remain immutable historical evidence and are not claimed rerun here. All older native sources/routes remain byte-identical after removing the one new route/declaration and export renames.')
 (P/'analysis/movie_transfer_interrupt_regression_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Transfer regression comparisons',report['total_comparisons'],'checks',checks)
if __name__=='__main__':main()
