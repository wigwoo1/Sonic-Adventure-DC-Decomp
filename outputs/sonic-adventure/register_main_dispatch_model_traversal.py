"""Register native recursive model traversal and its mesh wrapper."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_model_traversal_verification.json'
LIFECYCLE='analysis/main_dispatch_model_traversal_lifecycle_verification.json'
MANIFEST='analysis/main_dispatch_model_traversal_translation.json'

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 report=json.loads((PROJECT/REPORT).read_text());life=json.loads((PROJECT/LIFECYCLE).read_text());manifest=json.loads((PROJECT/MANIFEST).read_text())
 for k,v in dict(cases_passed=1494,rows_covered=526,static_instruction_rows=528,conditional_edges_covered=57,call_sites_covered=41,boundary_comparisons=6138,
                 numerical_operations=10340,partial_guard_error_cases=964,independent_invariant_cases=530,mesh_service_invocations=690).items():assert report[k]==v,k
 assert report['counts']==dict(callback=0,transform=0,emit=0,spawn=0,schedule=0,primary=32,secondary=32,work=32,draw=32,save=0,restore=0,translate=0,geometry=32,model_wrapper=160,traversal=1136,mesh_wrapper=38)
 for k,v in dict(lifecycles_passed=120,stage_comparisons=984,boundary_comparisons=19584,numerical_operations=44800,
                 mesh_service_invocations=880,created_parents=64,created_particles=80,paused_scheduler_passes=54,
                 translation_cases_passed=46,translation_rows_covered=56).items():assert life[k]==v,k
 assert report['structurally_unreachable_rows']==['0x8c621194','0x8c621196']
 assert report['structurally_unreachable_edges']==[['0x8c62118e','0x8c621194']]
 assert manifest['routines'][0]['unreachable_proof']=={str(i):i&7 for i in range(256)}
 assert report['routines']==manifest['routines'] and len(life['cases'])==120
 assert sum(len(c['stages']) for c in life['cases'])==984
 assert all(c['independent_pool_heap_lifetime_checks'] and all(s['full_state_match'] for s in c['stages']) for c in life['cases'])
 for item in (report,life,manifest):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_model_traversal.dll')==report['dll_sha256']==life['dll_sha256']
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
 integration=dict(verification=REPORT,verification_sha256=digest(PROJECT/REPORT),lifecycle_verification=LIFECYCLE,
                  lifecycle_verification_sha256=digest(PROJECT/LIFECYCLE),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),
                  body_comparisons=1494,lifecycle_stage_comparisons=984,lifecycles=120,newly_recovered_entries=2,static_instruction_rows=528,reachable_rows_covered=526,
                  remaining_service_targets=['0x8c6209a8','dynamic setter table values'],scope=report['scope']+' '+life['scope'],
                  limitations=report['limitations']+life['limitations'])
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));added=0
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()==routine['retail_span_sha256']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],
             indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,
             native_build_script='build_main_dispatch_model_traversal.ps1',verifier_script='verify_main_dispatch_model_traversal.py',
             instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,
             status='All 526 reachable rows, 57 reachable conditional outcomes and 41 call sites covered; two rows and one outcome proven structurally unreachable. Mesh body remains a fixture.',
             comparison_level=integration['scope'],limitations=integration['limitations'])
  for key in ('literal_pool_gaps','structurally_unreachable_rows','structurally_unreachable_edges','unreachable_proof'):
   if key in routine:entry[key]=routine[key]
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_recursive_model_traversal']=integration
 for address in (0x8C620F2C,0x8C6215FA,0x8C04481A,0x8C0448B4,0x8C6215D6,0x8C605DAC,0x8C605E0A,0x8C63A804,0x8C044680,0x8C0446A2,0x8C04474A,0x8C07B320,0x8C09878C,0x8C09865C):
  parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
  parent.setdefault('integration_evidence',{})['native_recursive_model_traversal']=integration
 registry_path.write_text(json.dumps(registry,indent=2)+'\n')
 status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_recursive_model_traversal']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} recursive traversal/mesh wrapper entries; {len(registry["routines"])} total. Mesh body 8c6209a8 remains unfinished.')

if __name__=='__main__':main()
