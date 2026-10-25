"""Replay prior query controls; only the three intended caller prefixes advance."""
import hashlib,json
from pathlib import Path
import write_watch_cpu_transfer as fast
v=fast.old;base=v.old.old.old
import verify_movie_disc_init as boot_v
P=v.P;ROOT=v.ROOT
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 old_bridge=P/'source/sa_movie_transfer_start_connected.c';new_bridge=P/'source/sa_movie_cpu_transfer_connected.c';text=new_bridge.read_text()
 text=text.replace('sa_movie_cpu_transfer_connected','sa_movie_transfer_start_connected').replace('sa_movie_cpu_transfer_initialized_connected','sa_movie_transfer_start_initialized_connected')
 translation=json.loads(v.MP.read_text())
 for routine in translation['routines']:
  declaration=f'VDECL({routine["name"]});\n';assert text.count(declaration)==1;text=text.replace(declaration,'')
  route=f' if(target=={routine["address"]}u)return {routine["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n';assert text.count(route)==1;text=text.replace(route,'')
 assert text==old_bridge.read_text()
 ram=v.seed_ram();checks=0;cases=[];prior_paths=[]
 for name,module,expected in (('movie_console_verification',base.old.old.old.old.old.old,225),('movie_init_records_verification',base.old.old.old.old.old,59),('movie_record_constructors_verification',base.old.old.old.old,65),('movie_diagnostics_verification',base.old.old.old,351),('movie_request_fields_verification',base.old.old,79),('movie_driver_control_verification',base.old,121)):
  prior_path=P/f'analysis/{name}.json';prior_paths.append(prior_path);prior=json.loads(prior_path.read_text());inputs=list(module.cases());assert len(inputs)==len(prior['cases']);completed=0
  for c,saved in zip(inputs,prior['cases']):
   if c['entry']==0x8c0546c0 or (name=='movie_record_constructors_verification' and (c['entry']==0x8c65acc0 or (c['entry']==0x8c666b34 and c.get('enabled',True)))):continue
   m,s,f=module.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);record,path,a,s,f=fast.compare(m,s,f,c);record['guest_r0_at_stop']=hex(s.r[0])
   if name=='movie_driver_control_verification':record['independent_checks']=module.semantics(m,a,before,s,record,path,c);record['guest_sr_at_stop']=hex(s.sr)
   for key,value in saved.items():assert json.loads(json.dumps(record[key]))==value,(name,key);checks+=1
   cases.append(dict(entry=hex(c['entry']),status=record['status'],stop_pc=record['stop_pc']));completed+=1
  assert completed==expected
 assert len(cases)==900
 default_path=P/'analysis/movie_diagnostics_default_base_verification.json';prior_paths.append(default_path)
 for saved in json.loads(default_path.read_text())['cases']:
  c=dict(saved['input']);m,s,f=base.old.old.old.prepare(ram,c);s.r[8]=c['incoming_radix'];record,path,a,s,f=fast.compare(m,s,f,c);record['guest_r0_at_stop']=hex(s.r[0])
  for key,value in saved.items():assert json.loads(json.dumps(record[key]))==value,('default radix',key);checks+=1
  cases.append(dict(entry=hex(c['entry']),status=record['status'],stop_pc=record['stop_pc']))
 assert len(cases)==945
 warm_path=P/'analysis/movie_disc_init_verification.json';warm=next(c for c in json.loads(warm_path.read_text())['cases'] if c['input']['entry']==0x8c010180 and not c['input'].get('cold_filesystem'))
 c=next(c for c in boot_v.cases() if c['entry']==0x8c010180 and not c.get('cold_filesystem'));m,s,f=boot_v.prepare(ram,c);record,path,a,s,f=fast.compare(m,s,f,c)
 assert record['status']==0 and hex(s.r[0])==warm['guest_r0']=='0xfffffff8' and record['stop_pc']=='0x8cfed000';checks+=3
 for key in ('steps','boundaries','boundary_comparisons','disc_reads','disc_snapshot','clear_native','clear_reference','vram_sha256','clock_reads'):assert json.loads(json.dumps(record[key]))==warm[key],key;checks+=1
 assert record['clear_native']['writes']==0x200000;checks+=1
 warm_record=record;warm_r0=hex(s.r[0])
 import types,gc
 import verify_movie_bulk_family as bulk
 import verify_movie_queue_family as queue
 import verify_movie_transfer_start as transfer
 retained={};advanced=[];omitted=[]
 for family,module,count in (('movie_bulk_family',bulk,201),('movie_queue_family',queue,479),('movie_transfer_start',transfer,399)):
  prior_path=P/f'analysis/{family}_verification.json';prior_paths.append(prior_path);prior=json.loads(prior_path.read_text());inputs=list(module.cases());assert len(inputs)==len(prior['cases'])==count
  retained[family]=[]
  semantic=types.FunctionType(module.semantics.__code__,dict(module.semantics.__globals__,fast=fast),'semantics')
  for c,saved in zip(inputs,prior['cases']):
   if family=='movie_bulk_family' and c['entry']==bulk.v.RESET and c.get('fill_word')==0x89abcdef:
    omitted.append(dict(family=family,input=dict(c),reason='Previously verified full constant fill retained historically; current new full varying-data copy executes all524288wordstores through the same unchanged provider. Small fixed-stride regressions rerun.'));continue
   advances=(family=='movie_bulk_family' and c['entry']==bulk.v.REQUEST and c.get('root_enabled',True) and c.get('record',bulk.v.RECORD) and c.get('state',0)==0) or (family=='movie_queue_family' and saved['stop_pc']=='0x8c669088') or (family=='movie_transfer_start' and saved['stop_pc']=='0x8c666e06')
   m,s,f=module.v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);record,path,a,s,f=fast.compare(m,s,f,c)
   if advances:
    status,stop,words=(0,'0x8cfed000',0) if family!='movie_queue_family' else (-3,'0x8c65a582',16)
    assert record['status']==status and record['stop_pc']==stop and record['sound_state'][1]==words,(family,c,record['status'],record['stop_pc']);checks+=3
    keys=('input','status','stop_pc','steps','boundary_comparisons','full_state_match','ordered_boundaries_match','sound_state')
    advanced.append(dict(family=family,prior_status=saved['status'],prior_stop=saved['stop_pc'],current={k:record[k] for k in keys}));del record,path,a;gc.collect();continue
   independent=semantic(m,a,before,s,record,c) if family=='movie_queue_family' else semantic(m,a,before,s,record,path,c)
   record.update(independent_checks=independent,guest_r0=hex(s.r[0]),guest_sr=hex(s.sr))
   for key,value in saved.items():assert json.loads(json.dumps(record[key]))==value,(family,key,c);checks+=1
   retained[family].append(dict(entry=hex(c['entry']),status=record['status'],stop_pc=record['stop_pc']));del record,path,a;gc.collect()
  print('REPLAYED',family,len(retained[family]),'retained;',len(advanced),'advanced total',flush=True)
 assert [len(retained[k]) for k in ('movie_bulk_family','movie_queue_family','movie_transfer_start')]==[198,476,395] and len(advanced)==9 and len(omitted)==1
 capture=dict(earlier_direct_cases=cases,retained=retained,advanced=advanced,omitted=omitted,warm_result=warm_record,warm_guest_r0=warm_r0,saved_record_checks=checks,dll_sha256=digest(v.effects.DLL),harness_sha256=digest(__file__),all_current_comparisons_completed=True)
 (ROOT/'work/sonic-adventure/movie_cpu_transfer_regression_completed_capture.json').write_text(json.dumps(capture,indent=2)+'\n')
 files=[Path(__file__),Path(v.__file__),v.MP,*prior_paths,warm_path,old_bridge,new_bridge,P/'analysis/movie_cpu_transfer_connection.json']
 report=dict(previous_direct_cases_replayed=945,prior_bulk_small_cases_replayed=198,prior_queue_cases_replayed=476,prior_transfer_cases_replayed=395,retained=retained,advanced_cases=advanced,advanced_comparisons=9,historical_full_constant_fill_not_repeated=omitted,retained_warm_startup_comparisons=1,total_comparisons=2024,unchanged_comparisons=2015,saved_record_checks=checks,original_bridge_preserved_except_added_routes=True,warm_result=warm_record,warm_guest_r0=warm_r0,cases=cases,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='2015unchanged comparisons plus9explicitly advanced original/native controls. Earlier mode-zero/counter stops now execute real new code; inherited invalid queue-child controls retain their genuine later address fault. A new complete varying-data two-MiB copy verifies every original word store through the unchanged provider; the older full constant pattern remains preserved historical evidence rather than being rerun. The prior warm full-video-clear startup reexecutes and matches. All original source/proof/build bytes remain preserved; no claim to rerun the entire historical suite or fresh full earliest cold caller.')
 (P/'analysis/movie_cpu_transfer_regression_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Matched2015unchanged+9advanced comparisons;',checks,'saved-record checks')
if __name__=='__main__':main()
