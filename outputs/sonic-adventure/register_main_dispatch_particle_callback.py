"""Register five newly recovered entries and admit the existing translation body."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_particle_callback_verification.json'
LIFECYCLE='analysis/main_dispatch_particle_lifecycle_verification.json'
MANIFEST='analysis/main_dispatch_particle_callback_translation.json'

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 report=json.loads((PROJECT/REPORT).read_text());life=json.loads((PROJECT/LIFECYCLE).read_text())
 manifest=json.loads((PROJECT/MANIFEST).read_text())
 for k,v in dict(cases_passed=471,rows_covered=274,conditional_edges_covered=26,call_sites_covered=18,
                 boundary_comparisons=2801,numerical_operations=1144,partial_guard_error_cases=76,
                 independent_invariant_cases=395,geometry_service_invocations=228).items():assert report[k]==v,k
 assert report['counts']==dict(callback=0,transform=0,emit=0,spawn=0,schedule=0,primary=199,secondary=72,work=86,draw=36,save=16,restore=16,translate=46)
 for k,v in dict(lifecycles_passed=120,stage_comparisons=984,boundary_comparisons=16064,numerical_operations=9200,
                 geometry_service_invocations=880,created_parents=64,created_particles=80,paused_scheduler_passes=54,
                 translation_cases_passed=46,translation_rows_covered=56).items():assert life[k]==v,k
 assert report['routines']==manifest['routines']
 assert len(life['cases'])==120 and sum(len(c['stages']) for c in life['cases'])==984
 assert all(c['independent_pool_heap_lifetime_checks'] and all(s['full_state_match'] for s in c['stages']) for c in life['cases'])
 for item in (report,life,manifest):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_particle_callback.dll')==report['dll_sha256']==life['dll_sha256']
 vendor=PROJECT/'third_party/skmp-sh4-fpu/sh4_fpu_approx.h'
 assert digest(vendor)=='cc95d6a3ede9ab57b6f332fcec5dacc97cda16515699dcdcd4bcf4978c869d2f'
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
 dependency=manifest['existing_translation_dependency']
 assert digest(PROJECT/dependency['source'])==dependency['source_sha256']
 prior=json.loads((PROJECT/dependency['prior_verification']).read_text())
 assert prior['cases_passed']==1024 and prior['source_sha256']==dependency['source_sha256']
 integration=dict(verification=REPORT,verification_sha256=digest(PROJECT/REPORT),lifecycle_verification=LIFECYCLE,
                  lifecycle_verification_sha256=digest(PROJECT/LIFECYCLE),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),
                  body_comparisons=471,lifecycle_stage_comparisons=984,lifecycles=120,created_parents=64,created_particles=80,
                  newly_recovered_entries=5,new_instruction_rows=274,reused_translation_rows=56,
                  remaining_service_targets=['0x8c620f2c','dynamic setter table values'],
                  numerical_vendor=dict(path='third_party/skmp-sh4-fpu/sh4_fpu_approx.h',sha256=digest(vendor)),
                  scope=report['scope']+' '+life['scope'],limitations=report['limitations']+life['limitations'])
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));added=0
 for routine in manifest['routines']+[dependency]:
  reused=routine is dependency
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()==routine['retail_span_sha256']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],
             end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_rows']*2,
             retail_span_sha256=routine['retail_span_sha256'],source=dependency['source'] if reused else report['source'],
             verification=dependency['prior_verification'] if reused else REPORT,
             native_build_script='build_main_dispatch_particle_callback.ps1',
             verifier_script='verify_main_dispatch_particle_lifecycle.py' if reused else 'verify_main_dispatch_particle_callback.py',
             instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,
             recovery_credit='Existing source first admitted to registry; no new source recovery credit.' if reused else 'New original entry; shared internal body emitted once.',
             status='Original rows covered with native numerical integration; geometry submission remains a fixture.',
             comparison_level=integration['scope'],limitations=integration['limitations'])
  for k in ('literal_pool_gap','shared_internal_entry'):
   if k in routine:entry[k]=routine[k]
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_particle_lifetime_and_render_state']=integration
 # The internal 8c0448ba entry is deliberately not registered as another routine.
 addresses={0x8C044680,0x8C0446A2,0x8C04474A,0x8C07B320,0x8C09852E,0x8C0985EC,0x8C09865C,0x8C098786,0x8C09878C,0x8C098A80,0x8C098B0C,0x8C098B42,0x8C04FE40}
 for address in addresses:
  parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
  parent.setdefault('integration_evidence',{})['native_particle_lifetime_and_render_state']=integration
 registry_path.write_text(json.dumps(registry,indent=2)+'\n')
 status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_particle_lifetime_and_render_state']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} entries: five new bodies and one reused translation admission; {len(registry["routines"])} total. Geometry 8c620f2c remains unfinished.')

if __name__=='__main__':main()
