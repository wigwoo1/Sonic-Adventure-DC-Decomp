"""Replay prior query controls; only the three intended caller prefixes advance."""
import hashlib,json
from pathlib import Path
import write_watch_queue_family as fast
v=fast.old;base=v.old
import verify_movie_disc_init as boot_v
P=v.P;ROOT=v.ROOT
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 old_bridge=P/'source/sa_movie_bulk_family_connected.c';new_bridge=P/'source/sa_movie_queue_family_connected.c';text=new_bridge.read_text()
 text=text.replace('sa_movie_queue_family_connected','sa_movie_bulk_family_connected').replace('sa_movie_queue_family_initialized_connected','sa_movie_bulk_family_initialized_connected')
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
 bulk_path=P/'analysis/movie_bulk_family_verification.json';prior_paths.append(bulk_path);prior=json.loads(bulk_path.read_text());inputs=list(bulk.cases());assert len(inputs)==len(prior['cases'])==201
 bulk_semantics=types.FunctionType(bulk.semantics.__code__,dict(bulk.semantics.__globals__,fast=fast),'bulk_semantics');bulk_cases=[];advanced=[]
 for c,saved in zip(inputs,prior['cases']):
  if c['entry']==bulk.v.REQUEST and c.get('root_enabled',True) and c.get('record',bulk.v.RECORD) and c.get('state',0)==0:advanced.append(c);continue
  m,s,f=bulk.v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);record,path,a,s,f=fast.compare(m,s,f,c)
  record.update(independent_checks=bulk_semantics(m,a,before,s,record,path,c),guest_r0=hex(s.r[0]),guest_sr=hex(s.sr))
  for key,value in saved.items():assert json.loads(json.dumps(record[key]))==value,('bulk',key,c);checks+=1
  bulk_cases.append(dict(entry=hex(c['entry']),status=record['status'],stop_pc=record['stop_pc']));print('Retained bulk',len(bulk_cases),record['stop_pc'],flush=True);del record,path,a;gc.collect()
 assert len(advanced)==2 and len(bulk_cases)==199
 capture=dict(previous_direct_cases=cases,retained_bulk_cases=bulk_cases,advanced_inputs=advanced,warm_result=warm_record,warm_guest_r0=warm_r0,saved_record_checks=checks,dll_sha256=digest(v.effects.DLL),harness_sha256=digest(__file__),all_original_native_comparisons_completed=True)
 (ROOT/'work/sonic-adventure/movie_queue_family_regression_completed_capture.json').write_text(json.dumps(capture,indent=2)+'\n')
 files=[Path(__file__),Path(v.__file__),v.MP,*prior_paths,warm_path,old_bridge,new_bridge,P/'analysis/movie_queue_family_connection.json']
 report=dict(previous_direct_cases_replayed=945,prior_bulk_cases_replayed=199,advanced_bulk_inputs=advanced,bulk_cases=bulk_cases,intentionally_advanced_caller_cases=3,intentionally_advanced_constructor_cases=2,retained_warm_startup_comparisons=1,total_comparisons=1145,saved_record_checks=checks,original_bridge_preserved_except_added_routes=True,warm_result=warm_record,warm_guest_r0=warm_r0,cases=cases,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='All 945 earlier direct controls and 199 prior bulk-family controls, including the complete nonzero two-MiB fill, replay with saved records unchanged. Two valid request controls intentionally advance into newly connected dependencies and are covered separately. The three earlier caller cases now legitimately advance into newly translated original code and are covered separately by the new caller/driver proof. The retained earlier warm startup/full-clear replay still matches. A separate new cold caller comparison also executes in the new build; the complete accumulated suite is not claimed.')
 (P/'analysis/movie_queue_family_regression_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Matched 1144 prior direct records and earlier full-clear startup;',checks,'saved-record checks')
if __name__=='__main__':main()
