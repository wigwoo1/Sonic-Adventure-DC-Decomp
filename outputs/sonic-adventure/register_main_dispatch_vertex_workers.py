"""Register four original vertex workers and their native queue integration."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_vertex_workers_verification.json'
LIFECYCLE='analysis/main_dispatch_vertex_workers_lifecycle_verification.json'
MANIFEST='analysis/main_dispatch_vertex_workers_translation.json'

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 report=json.loads((PROJECT/REPORT).read_text());life=json.loads((PROJECT/LIFECYCLE).read_text());manifest=json.loads((PROJECT/MANIFEST).read_text())
 for k,v in dict(cases_passed=265,rows_covered=392,conditional_edges_covered=20,call_sites_covered=0,boundary_comparisons=72,numerical_operations=8773,partial_cases=104,independent_cases=161,queue_flushes=940,queue_events=9403).items():assert report[k]==v,k
 assert {k:v for k,v in report['counts'].items() if v}==dict(mesh=8,filter_core=8,project_normals=56,project=57,classify_normals=68,classify=68)
 assert all(c['full_state_match'] and c['queue_match'] for c in report['cases'])
 for k,v in dict(lifecycles_passed=120,stage_comparisons=984,boundary_comparisons=23104,numerical_operations=89616,external_service_boundaries=352,created_parents=64,created_particles=80,paused_scheduler_passes=54,translation_cases_passed=46,translation_rows_covered=56,original_particle_mesh_iterations=880,vertex_worker_invocations=880,queue_flushes=9680,queue_events=109136).items():assert life[k]==v,k
 assert life['service_invocations']=={'0x8c605e68':176,'0x8c61ffe4':160,'0x8c620428':16}
 prior=json.loads((PROJECT/'analysis/main_dispatch_mesh_services_verification.json').read_text())
 remaining=sorted(set(prior['service_targets'])-{'0x8c611060','0x8c611122','0x8c6111d6','0x8c6112e2'});assert len(remaining)==22
 assert report['routines']==manifest['routines'] and len(life['cases'])==120
 assert sum(len(c['stages']) for c in life['cases'])==984
 assert all(c['independent_pool_heap_lifetime_checks'] and all(s['full_state_match'] for s in c['stages']) for c in life['cases'])
 for item in (report,life,manifest):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_vertex_workers.dll')==report['dll_sha256']==life['dll_sha256']
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 assert hashlib.sha256(binary).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 integration=dict(verification=REPORT,verification_sha256=digest(PROJECT/REPORT),lifecycle_verification=LIFECYCLE,
                  lifecycle_verification_sha256=digest(PROJECT/LIFECYCLE),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),
                  body_comparisons=265,lifecycle_stage_comparisons=984,lifecycles=120,newly_recovered_entries=4,new_instruction_rows=392,vertex_queue_flushes=9680,
                  remaining_service_targets=remaining+['dynamic setter table values'],scope=report['scope']+' '+life['scope'],
                  limitations=report['limitations']+life['limitations'])
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));added=0
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()==routine['retail_span_sha256']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],
             indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,
             native_build_script='build_main_dispatch_vertex_workers.ps1',verifier_script='verify_main_dispatch_vertex_workers.py',
             instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,
             status='All 392 rows and 20 outcomes covered with native coherent-RAM/MMU-off store queues; cache/GPU timing and hardware faults unmodeled. Remaining acquisition/primitive services are fixtures.',
             comparison_level=integration['scope'],limitations=integration['limitations'])
  for key in ('internal_indirect_jumps',):
   if key in routine:entry[key]=routine[key]
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_vertex_workers_and_store_queues']=integration
 for address in (0x8C611444,0x8C606C56,0x8C606A08,0x8C606A88,0x8C6069A4,0x8C6209A8,0x8C62159A,0x8C62113C,0x8C6215BA,0x8C620F2C,0x8C6215FA,0x8C04481A,0x8C0448B4,0x8C6215D6,0x8C605DAC,0x8C605E0A,0x8C63A804,0x8C044680,0x8C0446A2,0x8C04474A,0x8C07B320,0x8C09878C,0x8C09865C):
  parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
  parent.setdefault('integration_evidence',{})['native_vertex_workers_and_store_queues']=integration
 registry_path.write_text(json.dumps(registry,indent=2)+'\n')
 status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_vertex_workers_and_store_queues']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} vertex worker entries; {len(registry["routines"])} total. 22 acquisition/primitive services remain unfinished.')

if __name__=='__main__':main()
