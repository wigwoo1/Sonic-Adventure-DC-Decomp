"""Preserve prior handler and synchronous-transfer results in the new pipeline."""
import gc,hashlib,json,types
from pathlib import Path
import write_watch_interrupt_context as fast
import verify_movie_transfer_interrupt as irq
import verify_movie_cpu_transfer as cpu
import verify_movie_transfer_dispatch as dispatch
v=fast.old;P=v.P;ROOT=v.ROOT

def main():
 old=P/'source/sa_movie_transfer_dispatch_connected.c';new=P/'source/sa_movie_interrupt_context_connected.c'
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,old,new];checks=0;records=[];ram=v.seed_ram()
 for family,module,expected in (('movie_transfer_dispatch',dispatch,137),('movie_transfer_interrupt',irq,110),('movie_cpu_transfer',cpu,240)):
  path=P/f'analysis/{family}_verification.json';prior=json.loads(path.read_text());files.append(path);inputs=list(module.cases());assert len(inputs)==len(prior['cases'])==expected
  semantics=types.FunctionType(module.semantics.__code__,dict(module.semantics.__globals__,fast=fast),'semantics')
  for c,saved in zip(inputs,prior['cases']):
   m,s,f=module.v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);r,path,a,s,f=fast.compare(m,s,f,c)
   r.update(independent_checks=semantics(m,a,before,s,r,path,c),guest_r0=hex(s.r[0]),guest_sr=hex(s.sr))
   for key,value in saved.items():assert json.loads(json.dumps(r[key]))==value,(family,key,c);checks+=1
   records.append(dict(family=family,entry=hex(c['entry']),status=r['status'],stop_pc=r['stop_pc']));del r,path,a;gc.collect()
  print('Replayed',family,expected,flush=True)
 waits=[]
 for mode in (0,4):
  c=v.Input(entry=cpu.v.REQUEST,mode=mode,sdk_mode=1);m,s,f=cpu.v.prepare(ram,c);r,path,a,s,f=fast.compare(m,s,f,c)
  assert r['status']==-4 and r['stop_pc']=='0x8c65a6ec';checks+=2
  assert v.load(a,v.RECORD+4)==2 and v.load(a,v.RECORD+24)==0;checks+=2
  assert not any(r['sound_state']) and v.HANDLER not in path and v.ASIC not in path;checks+=3
  waits.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc']))
 report=dict(previous_dispatch_comparisons=137,previous_handler_comparisons=110,previous_cpu_comparisons=240,retained_wait_comparisons=2,total_comparisons=len(records)+len(waits),independent_and_record_checks=checks,cases=records,wait_cases=waits,previous_routes_preserved=True,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='487 previously published handler/copy/counter cases replay unchanged against saved records and full original/native machine/device state. Two default REQUEST controls retain genuine mode-one wait65a6ec with state2/done0, untouched SoundRAM and no automatic dispatcher invocation. Old full-copy/video-clear/earliest-caller proofs preserved historically, not rerun. All prior sources preserved; newly declared exception ABI carries separate state. Default waits receive no completion.')
 (P/'analysis/movie_interrupt_context_regression_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Dispatch regressions',report['total_comparisons'],'checks',checks)
if __name__=='__main__':main()
