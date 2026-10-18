"""Register both alternate strips; reuse stub/clipper without duplicate credit."""
import collections,hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_uv_alternate_strips_verification.json'
LIFECYCLE='analysis/main_dispatch_uv_alternate_strips_lifecycle_verification.json'
MANIFEST='analysis/main_dispatch_uv_alternate_strips_translation.json'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 report=json.loads((PROJECT/REPORT).read_text());life=json.loads((PROJECT/LIFECYCLE).read_text());manifest=json.loads((PROJECT/MANIFEST).read_text())
 for k,v in dict(cases_passed=512,rows_covered=839,conditional_edges_covered=40,call_sites_covered=3,boundary_comparisons=2560,numerical_operations=37092,partial_cases=58,independent_cases=392,independent_mesh_cases=192,queue_flushes=1822,queue_events=19654,writeback_events=0,fipr_instructions=1362,external_service_boundaries=0,stub_invocations=492,near_invocations=132).items():assert report[k]==v,k
 assert report['counts']==dict(uv_alternate=128,uv_alternate_clipped=176,uv_lit=8,uv_lit_clipped=8,mesh=192)
 assert collections.Counter(c['status'] for c in report['cases'])=={0:454,-1:56,-3:2}
 assert sum(bool(c['input'].get('connected_alternate')) for c in report['cases'])==192
 assert all(c['full_state_match'] and c['queue_match'] for c in report['cases'])
 assert all(c['status']==0 and c['input']['clip']==False for c in report['cases'] if c['input'].get('connected_alternate'))
 for k,v in dict(lifecycles_passed=120,stage_comparisons=984,boundary_comparisons=23264,numerical_operations=104016,external_service_boundaries=0,created_parents=64,created_particles=80,paused_scheduler_passes=54,translation_cases_passed=46,translation_rows_covered=56,original_particle_mesh_iterations=880,vertex_worker_invocations=880,primitive_invocations=176,primitive_prefetches=744,near_invocations=160,near_prefetches=0,queue_flushes=10600,queue_events=117808).items():assert life[k]==v,k
 assert life['service_invocations']=={} and report['routines']==manifest['routines'] and len(manifest['routines'])==2
 assert len(life['cases'])==120 and sum(len(c['stages']) for c in life['cases'])==984
 assert all(c['independent_pool_heap_lifetime_checks'] and all(s['full_state_match'] and s['geometry_calls']==0 for s in c['stages']) for c in life['cases'])
 for item in (report,life,manifest):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_uv_alternate_strips.dll')==report['dll_sha256']==life['dll_sha256']
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 assert binary[0x8C611708-0x8C010000:0x8C61170C-0x8C010000]==bytes.fromhex('0b000900')
 assert manifest['reused_stub']['new_recovery_credit']==0
 prior=json.loads((PROJECT/'project_status.json').read_text())['verified_progress']['native_uv_near_plane_clipping']
 remaining=sorted(set(prior['remaining_service_targets'])-{'0x8c61394a','0x8c614f70','dynamic setter table values'})
 assert len(remaining)==12 and report['service_targets']==remaining
 integration=dict(verification=REPORT,verification_sha256=digest(PROJECT/REPORT),lifecycle_verification=LIFECYCLE,lifecycle_verification_sha256=digest(PROJECT/LIFECYCLE),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),body_comparisons=512,lifecycle_stage_comparisons=984,lifecycles=120,newly_recovered_entries=2,new_instruction_rows=839,connected_projection_alternate_clipping_cases=192,reused_stub_new_credit=0,remaining_service_targets=remaining+['dynamic setter table values'],scope=report['scope']+' '+life['scope'],limitations=report['limitations']+life['limitations'])
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));added=0
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()==routine['retail_span_sha256']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,native_build_script='build_main_dispatch_uv_alternate_strips.ps1',verifier_script='verify_main_dispatch_uv_alternate_strips.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='All 839 rows / 40 outcomes / three call sites across both complete alternate strips covered, using native existing stub and UV near clipper. Positive bounded strip metadata only; hardware/visible rendering remains unverified.',comparison_level=integration['scope'],limitations=integration['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_uv_alternate_strip_emitters']=integration
 for address in (0x8C611A14,0x8C618D18,0x8C619310,0x8C6064B0,0x8C6065B0,0x8C611708,0x8C612E64,0x8C617E46,0x8C6184F0,0x8C613312,0x8C61FFE4,0x8C620428,0x8C605E68,0x8C6069A4,0x8C6209A8,0x8C62159A,0x8C62113C,0x8C6215BA,0x8C04481A,0x8C0448B4,0x8C6215D6,0x8C605DAC,0x8C605E0A,0x8C07B320,0x8C09878C,0x8C09865C):
  parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
  parent.setdefault('integration_evidence',{})['native_uv_alternate_strip_emitters']=integration
 registry_path.write_text(json.dumps(registry,indent=2)+'\n')
 status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_uv_alternate_strip_emitters']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} alternate strip entries; {len(registry["routines"])} total. Existing stub/clipper reused without new credit; 12 other primitive emitters remain unfinished.')

if __name__=='__main__':main()
