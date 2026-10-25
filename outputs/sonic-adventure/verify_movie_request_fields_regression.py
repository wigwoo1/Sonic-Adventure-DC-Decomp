"""Replay prior query controls; only the three intended caller prefixes advance."""
import hashlib,json
from pathlib import Path
import verify_movie_request_fields as v
import verify_movie_disc_init as boot_v
P=v.P;ROOT=v.ROOT
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 old_bridge=P/'source/sa_movie_diagnostics_connected.c';new_bridge=P/'source/sa_movie_request_fields_connected.c';text=new_bridge.read_text()
 text=text.replace('sa_movie_request_fields_connected','sa_movie_diagnostics_connected').replace('sa_movie_request_fields_initialized_connected','sa_movie_diagnostics_initialized_connected')
 translation=json.loads(v.MP.read_text())
 for routine in translation['routines']:
  declaration=f'VDECL({routine["name"]});\n';assert text.count(declaration)==1;text=text.replace(declaration,'')
  route=f' if(target=={routine["address"]}u)return {routine["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n';assert text.count(route)==1;text=text.replace(route,'')
 assert text==old_bridge.read_text()
 ram=v.seed_ram();checks=0;cases=[];prior_paths=[]
 for name,module,expected in (('movie_console_verification',v.old.old.old.old,225),('movie_init_records_verification',v.old.old.old,59),('movie_record_constructors_verification',v.old.old,65),('movie_diagnostics_verification',v.old,351)):
  prior_path=P/f'analysis/{name}.json';prior_paths.append(prior_path);prior=json.loads(prior_path.read_text());inputs=list(module.cases());assert len(inputs)==len(prior['cases']);completed=0
  for c,saved in zip(inputs,prior['cases']):
   if c['entry']==0x8c0546c0 or (name=='movie_record_constructors_verification' and (c['entry']==0x8c65acc0 or (c['entry']==0x8c666b34 and c.get('enabled',True)))):continue
   m,s,f=module.prepare(ram,c);record,path,a,s,f=v.compare(m,s,f,c);record['guest_r0_at_stop']=hex(s.r[0])
   for key,value in saved.items():assert json.loads(json.dumps(record[key]))==value,(name,key);checks+=1
   cases.append(dict(entry=hex(c['entry']),status=record['status'],stop_pc=record['stop_pc']));completed+=1
  assert completed==expected
 assert len(cases)==700
 default_path=P/'analysis/movie_diagnostics_default_base_verification.json';prior_paths.append(default_path)
 for saved in json.loads(default_path.read_text())['cases']:
  c=dict(saved['input']);m,s,f=v.old.prepare(ram,c);s.r[8]=c['incoming_radix'];record,path,a,s,f=v.compare(m,s,f,c);record['guest_r0_at_stop']=hex(s.r[0])
  for key,value in saved.items():assert json.loads(json.dumps(record[key]))==value,('default radix',key);checks+=1
  cases.append(dict(entry=hex(c['entry']),status=record['status'],stop_pc=record['stop_pc']))
 assert len(cases)==745
 warm_path=P/'analysis/movie_disc_init_verification.json';warm=next(c for c in json.loads(warm_path.read_text())['cases'] if c['input']['entry']==0x8c010180 and not c['input'].get('cold_filesystem'))
 c=next(c for c in boot_v.cases() if c['entry']==0x8c010180 and not c.get('cold_filesystem'));m,s,f=boot_v.prepare(ram,c);record,path,a,s,f=v.compare(m,s,f,c)
 assert record['status']==0 and hex(s.r[0])==warm['guest_r0']=='0xfffffff8' and record['stop_pc']=='0x8cfed000';checks+=3
 for key in ('steps','boundaries','boundary_comparisons','disc_reads','disc_snapshot','clear_native','clear_reference','vram_sha256','clock_reads'):assert json.loads(json.dumps(record[key]))==warm[key],key;checks+=1
 assert record['clear_native']['writes']==0x200000;checks+=1
 files=[Path(__file__),Path(v.__file__),v.MP,*prior_paths,warm_path,old_bridge,new_bridge,P/'analysis/movie_request_fields_connection.json']
 report=dict(previous_direct_cases_replayed=745,intentionally_advanced_caller_cases=3,intentionally_advanced_constructor_cases=2,retained_warm_startup_comparisons=1,total_comparisons=746,saved_record_checks=checks,original_bridge_preserved_except_added_routes=True,warm_result=record,warm_guest_r0=hex(s.r[0]),cases=cases,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='All 745 earlier direct controls replay with saved records unchanged. The three earlier caller cases now legitimately advance into newly translated original code and are covered separately by the new caller/driver proof. The retained earlier warm startup/full-clear replay still matches. A separate new cold caller comparison also executes in the new build; the complete accumulated suite is not claimed.')
 (P/'analysis/movie_request_fields_regression_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Matched 745 prior direct records and earlier full-clear startup;',checks,'saved-record checks')
if __name__=='__main__':main()
