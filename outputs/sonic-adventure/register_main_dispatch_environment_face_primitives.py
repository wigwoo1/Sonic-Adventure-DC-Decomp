"""Register the complete environment-UV/face-factor bodies with bounded integration evidence."""
import collections,hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_environment_face_primitives_verification.json'
LIFECYCLE='analysis/main_dispatch_environment_face_primitives_lifecycle_verification.json'
MANIFEST='analysis/main_dispatch_environment_face_primitives_translation.json'

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 report=json.loads((PROJECT/REPORT).read_text());life=json.loads((PROJECT/LIFECYCLE).read_text());manifest=json.loads((PROJECT/MANIFEST).read_text())
 for k,v in dict(cases_passed=710,rows_covered=2541,conditional_edges_covered=72,call_sites_covered=15,normalizer_invocations=260,boundary_comparisons=1048,numerical_operations=15292,partial_cases=232,independent_cases=188,queue_flushes=812,queue_events=7956,external_service_boundaries=180).items():assert report[k]==v,k
 assert report['counts']==dict(environment_face=246,environment_face_clipped=408,draw=4,primary=4,mesh=48)
 assert collections.Counter(c['status'] for c in report['cases'])=={0:478,-3:34,-1:192,-2:3,-17:3}
 assert len(report['cases'])==710 and all(c['full_state_match'] and c['queue_match'] for c in report['cases'])
 assert sum(bool(c['input'].get('environment_face_mesh')) for c in report['cases'])==48
 assert sum(c['input'].get('environment_face_mesh',False) and not c['input']['near_mask'] for c in report['cases'])==32
 assert all(c['status']==0 for c in report['cases'] if c['input'].get('environment_face_mesh'))
 assert {c['input']['missing_target'] for c in report['cases'] if c['status']==-2}=={0x8C6173BA,0x8C614580,0x8C616474}
 assert {c['input']['fail_target'] for c in report['cases'] if c['status']==-17}=={0x8C6173BA,0x8C614580,0x8C616474}
 for k,v in dict(lifecycles_passed=120,stage_comparisons=984,boundary_comparisons=23264,numerical_operations=104016,external_service_boundaries=0,created_parents=64,created_particles=80,paused_scheduler_passes=54,translation_cases_passed=46,translation_rows_covered=56,original_particle_mesh_iterations=880,vertex_worker_invocations=880,primitive_invocations=176,primitive_prefetches=744,near_invocations=160,near_prefetches=0,queue_flushes=10600,queue_events=117808).items():assert life[k]==v,k
 assert life['service_invocations']=={} and report['routines']==manifest['routines'] and len(manifest['routines'])==2
 assert sum(r['instruction_rows'] for r in manifest['routines'])==2545
 assert sum(len(r['reachable_addresses']) for r in manifest['routines'])==2541
 assert sum(len(r['unreachable_proof']['rows']) for r in manifest['routines'])==4
 assert all(r['unreachable_proof']['input_values_exhausted']==65536 for r in manifest['routines'])
 assert len(life['cases'])==120 and sum(len(c['stages']) for c in life['cases'])==984
 assert all(c['independent_pool_heap_lifetime_checks'] and all(s['full_state_match'] and s['geometry_calls']==0 for s in c['stages']) for c in life['cases'])
 for item in (report,life,manifest):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_environment_face_primitives.dll')==report['dll_sha256']==life['dll_sha256']
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 prior=json.loads((PROJECT/'project_status.json').read_text())['verified_progress']['native_uv_constant_alternate_strip_emitters']
 remaining=sorted((set(prior['remaining_service_targets'])-{'0x8c61db98','0x8c61e4f4','dynamic setter table values'})|{'0x8c6173ba','0x8c614580','0x8c616474'})
 assert len(remaining)==5 and report['service_targets']==remaining
 integration=dict(verification=REPORT,verification_sha256=digest(PROJECT/REPORT),lifecycle_verification=LIFECYCLE,lifecycle_verification_sha256=digest(PROJECT/LIFECYCLE),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),body_comparisons=710,lifecycle_stage_comparisons=984,lifecycles=120,newly_recovered_entries=2,new_instruction_rows=2545,new_reachable_rows=2541,proven_unreachable_rows=4,connected_mesh_cases=48,connected_environment_face_packet_or_staging_cases=48,connected_full_native_environment_face_packet_cases=32,reused_normalizer_new_credit=0,remaining_service_targets=remaining+['dynamic setter table values'],scope=report['scope']+' '+life['scope'],limitations=report['limitations']+life['limitations'])
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));added=0
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()==routine['retail_span_sha256']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,native_build_script='build_main_dispatch_environment_face_primitives.ps1',verifier_script='verify_main_dispatch_environment_face_primitives.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='All 2541 reachable rows / 72 outcomes / 15 call sites across complete environment-face bodies covered; four rows/two edges proven impossible. Independent normal-derived environment UVs, provoking face factors and retained packet words, near argument staging and native mesh integration verified; three lower helpers remain explicit fixtures.',comparison_level=integration['scope'],limitations=integration['limitations'],**{k:routine[k] for k in ('unreachable_proof','native_vertex_pointer_guard','native_return_guard') if k in routine})
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_environment_face_primitive_emitters']=integration
 for address in (0x8C63A94C,0x8C6064B0,0x8C6065B0,0x8C605E68,0x8C605F3C,0x8C6069A4,0x8C6209A8,0x8C62159A,0x8C62113C,0x8C6215BA,0x8C04481A,0x8C0448B4,0x8C6215D6,0x8C605DAC,0x8C605E0A,0x8C07B320,0x8C09878C,0x8C09865C):
  parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
  parent.setdefault('integration_evidence',{})['native_environment_face_primitive_emitters']=integration
 registry_path.write_text(json.dumps(registry,indent=2)+'\n')
 status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_environment_face_primitive_emitters']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} Environment-face primitive entries; {len(registry["routines"])} total. Three lower and two other primitive services remain unfinished.')

if __name__=='__main__':main()
