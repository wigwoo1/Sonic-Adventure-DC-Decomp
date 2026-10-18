"""Admit geometry preparation/model wrapper only after bounded proofs pass."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_particle_geometry_verification.json'
LIFECYCLE='analysis/main_dispatch_particle_geometry_lifecycle_verification.json'
MANIFEST='analysis/main_dispatch_particle_geometry_translation.json'

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 report=json.loads((PROJECT/REPORT).read_text());life=json.loads((PROJECT/LIFECYCLE).read_text());manifest=json.loads((PROJECT/MANIFEST).read_text())
 for k,v in dict(cases_passed=548,rows_covered=271,conditional_edges_covered=0,call_sites_covered=2,boundary_comparisons=1822,
                 numerical_operations=7832,partial_guard_error_cases=316,independent_invariant_cases=232,model_service_invocations=240).items():assert report[k]==v,k
 assert report['counts']==dict(callback=0,transform=0,emit=0,spawn=0,schedule=0,primary=96,secondary=48,work=64,draw=96,save=0,restore=0,translate=0,geometry=192,model_wrapper=52)
 for k,v in dict(lifecycles_passed=120,stage_comparisons=984,boundary_comparisons=17824,numerical_operations=44800,
                 model_service_invocations=880,created_parents=64,created_particles=80,paused_scheduler_passes=54,
                 translation_cases_passed=46,translation_rows_covered=56).items():assert life[k]==v,k
 assert report['routines']==manifest['routines'] and len(life['cases'])==120
 assert sum(len(c['stages']) for c in life['cases'])==984
 assert all(c['independent_pool_heap_lifetime_checks'] and all(s['full_state_match'] for s in c['stages']) for c in life['cases'])
 for item in (report,life,manifest):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_particle_geometry.dll')==report['dll_sha256']==life['dll_sha256']
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
 integration=dict(verification=REPORT,verification_sha256=digest(PROJECT/REPORT),lifecycle_verification=LIFECYCLE,
                  lifecycle_verification_sha256=digest(PROJECT/LIFECYCLE),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),
                  body_comparisons=548,lifecycle_stage_comparisons=984,lifecycles=120,newly_recovered_entries=2,new_instruction_rows=271,
                  remaining_service_targets=['0x8c62113c','dynamic setter table values'],scope=report['scope']+' '+life['scope'],
                  limitations=report['limitations']+life['limitations'])
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));added=0
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()==routine['retail_span_sha256']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],
             indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,
             native_build_script='build_main_dispatch_particle_geometry.ps1',verifier_script='verify_main_dispatch_particle_geometry.py',
             instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,
             status='All original instruction rows and call sites reached with native particle integration; recursive model traversal remains a fixture.',
             comparison_level=integration['scope'],limitations=integration['limitations'])
  if 'literal_pool_gap' in routine:entry['literal_pool_gap']=routine['literal_pool_gap']
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_particle_geometry_preparation']=integration
 for address in (0x8C04481A,0x8C0448B4,0x8C6215D6,0x8C605DAC,0x8C605E0A,0x8C63A804,0x8C044680,0x8C0446A2,0x8C04474A,0x8C07B320,0x8C09878C,0x8C09865C):
  parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
  parent.setdefault('integration_evidence',{})['native_particle_geometry_preparation']=integration
 registry_path.write_text(json.dumps(registry,indent=2)+'\n')
 status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_particle_geometry_preparation']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} geometry preparation/model wrapper entries; {len(registry["routines"])} total. Model traversal 8c62113c remains unfinished.')

if __name__=='__main__':main()
