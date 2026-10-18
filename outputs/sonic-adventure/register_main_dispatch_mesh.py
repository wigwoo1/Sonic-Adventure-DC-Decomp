"""Register the original mesh/material loop and filter wrapper with scoped proof."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_mesh_verification.json'
LIFECYCLE='analysis/main_dispatch_mesh_lifecycle_verification.json'
MANIFEST='analysis/main_dispatch_mesh_translation.json'

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 report=json.loads((PROJECT/REPORT).read_text());life=json.loads((PROJECT/LIFECYCLE).read_text());manifest=json.loads((PROJECT/MANIFEST).read_text())
 for k,v in dict(cases_passed=663,rows_covered=558,conditional_edges_covered=92,call_sites_covered=33,boundary_comparisons=4404,
                 numerical_operations=8040,partial_guard_error_cases=137,independent_invariant_cases=526,external_service_boundaries=3531).items():assert report[k]==v,k
 assert report['counts']==dict(callback=0,transform=0,emit=0,spawn=0,schedule=0,primary=4,secondary=0,work=0,draw=4,save=0,restore=0,translate=0,geometry=4,model_wrapper=0,traversal=6,mesh_wrapper=159,mesh=453,filter=33)
 for k,v in dict(lifecycles_passed=120,stage_comparisons=984,boundary_comparisons=25744,numerical_operations=57120,
                 external_service_boundaries=5280,created_parents=64,created_particles=80,paused_scheduler_passes=54,
                 translation_cases_passed=46,translation_rows_covered=56).items():assert life[k]==v,k
 assert len(report['service_targets'])==27
 assert life['original_particle_mesh_iterations']==880
 assert sum(life['service_invocations'].values())==5280
 assert report['routines']==manifest['routines'] and len(life['cases'])==120
 assert sum(len(c['stages']) for c in life['cases'])==984
 assert all(c['independent_pool_heap_lifetime_checks'] and all(s['full_state_match'] for s in c['stages']) for c in life['cases'])
 for item in (report,life,manifest):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_mesh.dll')==report['dll_sha256']==life['dll_sha256']
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
 integration=dict(verification=REPORT,verification_sha256=digest(PROJECT/REPORT),lifecycle_verification=LIFECYCLE,
                  lifecycle_verification_sha256=digest(PROJECT/LIFECYCLE),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),
                  body_comparisons=663,lifecycle_stage_comparisons=984,lifecycles=120,newly_recovered_entries=2,new_instruction_rows=558,
                  remaining_service_targets=report['service_targets']+['dynamic setter table values'],scope=report['scope']+' '+life['scope'],
                  limitations=report['limitations']+life['limitations'])
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));added=0
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()==routine['retail_span_sha256']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],
             indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,
             native_build_script='build_main_dispatch_mesh.ps1',verifier_script='verify_main_dispatch_mesh.py',
             instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,
             status='All 558 rows, 92 conditional outcomes and 33 call sites covered. Four internal indirect jumps preserve the original material loop. Lower services remain fixtures.',
             comparison_level=integration['scope'],limitations=integration['limitations'])
  for key in ('internal_indirect_jumps','internal_jump_literals'):
   if key in routine:entry[key]=routine[key]
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_mesh_material_loop']=integration
 for address in (0x8C62113C,0x8C6215BA,0x8C620F2C,0x8C6215FA,0x8C04481A,0x8C0448B4,0x8C6215D6,0x8C605DAC,0x8C605E0A,0x8C63A804,0x8C044680,0x8C0446A2,0x8C04474A,0x8C07B320,0x8C09878C,0x8C09865C):
  parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
  parent.setdefault('integration_evidence',{})['native_mesh_material_loop']=integration
 registry_path.write_text(json.dumps(registry,indent=2)+'\n')
 status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_mesh_material_loop']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} mesh/material loop and filter wrapper entries; {len(registry["routines"])} total. 27 lower services remain unfinished.')

if __name__=='__main__':main()
