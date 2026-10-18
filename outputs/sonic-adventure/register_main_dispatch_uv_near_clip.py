"""Register the complete near clipper and the native particle lifetime chain."""
import collections,hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_uv_near_clip_verification.json'
LIFECYCLE='analysis/main_dispatch_uv_near_clip_lifecycle_verification.json'
MANIFEST='analysis/main_dispatch_uv_near_clip_translation.json'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 report=json.loads((PROJECT/REPORT).read_text());life=json.loads((PROJECT/LIFECYCLE).read_text());manifest=json.loads((PROJECT/MANIFEST).read_text())
 for k,v in dict(cases_passed=614,rows_covered=616,conditional_edges_covered=14,call_sites_covered=0,boundary_comparisons=1246,numerical_operations=46538,partial_cases=96,independent_cases=372,independent_mesh_cases=96,queue_flushes=1642,queue_events=16074,external_service_boundaries=4,uv_near_invocations=596).items():assert report[k]==v,k
 assert report['counts']==dict(uv_near=394,uv_lit_clipped=114,draw=4,primary=4,uv_lit=2,mesh=96)
 assert collections.Counter(c['status'] for c in report['cases'])=={0:518,-1:76,-3:8,1:12}
 assert sum(bool(c['input'].get('connected_triangle')) for c in report['cases'])==96
 assert all(c['full_state_match'] and c['queue_match'] for c in report['cases'])
 assert report['parent_near_call_sites_covered']==['0x8c619588','0x8c619898','0x8c6198b4','0x8c619b6c']
 assert all(c['status']==0 and c['input']['clip']==False for c in report['cases'] if c['input'].get('uv_clipped_mesh'))
 for k,v in dict(lifecycles_passed=120,stage_comparisons=984,boundary_comparisons=23264,numerical_operations=104016,external_service_boundaries=0,created_parents=64,created_particles=80,paused_scheduler_passes=54,translation_cases_passed=46,translation_rows_covered=56,original_particle_mesh_iterations=880,vertex_worker_invocations=880,primitive_invocations=176,primitive_prefetches=744,near_invocations=160,near_prefetches=0,queue_flushes=10600,queue_events=117808).items():assert life[k]==v,k
 assert life['service_invocations']=={}
 assert report['routines']==manifest['routines'] and len(manifest['routines'])==1
 assert len(life['cases'])==120 and sum(len(c['stages']) for c in life['cases'])==984
 assert sum(s['near_invocations'] for c in life['cases'] for s in c['stages'])==160
 assert all(c['independent_pool_heap_lifetime_checks'] and all(s['full_state_match'] and s['geometry_calls']==0 for s in c['stages']) for c in life['cases'])
 for item in (report,life,manifest):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_uv_near_clip.dll')==report['dll_sha256']==life['dll_sha256']
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 prior=json.loads((PROJECT/'project_status.json').read_text())['verified_progress']['native_uv_lit_primitive_emitters']
 remaining=sorted(set(prior['remaining_service_targets'])-{'0x8c611a14','dynamic setter table values'})
 assert len(remaining)==14 and report['service_targets']==remaining
 integration=dict(verification=REPORT,verification_sha256=digest(PROJECT/REPORT),lifecycle_verification=LIFECYCLE,lifecycle_verification_sha256=digest(PROJECT/LIFECYCLE),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),body_comparisons=614,lifecycle_stage_comparisons=984,lifecycles=120,newly_recovered_entries=1,new_instruction_rows=616,connected_projection_clipping_cases=96,lifetime_external_game_services=0,remaining_service_targets=remaining+['dynamic setter table values'],scope=report['scope']+' '+life['scope'],limitations=report['limitations']+life['limitations'])
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
 routine=manifest['routines'][0];start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
 assert start==0x8C611A14 and end==0x8C611F24
 assert hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()==routine['retail_span_sha256']
 entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,native_build_script='build_main_dispatch_uv_near_clip.ps1',verifier_script='verify_main_dispatch_uv_near_clip.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='All 616 rows / 14 branch outcomes covered; independent clipped polygons, signed UV quantization/interpolation, lighting and 96 native mesh paths verified. Original-model lifetimes are regression only and do not reach this helper. Unsupported saved-pointer transfer stops with host -3; no physical fault or visible-rendering proof.',comparison_level=integration['scope'],limitations=integration['limitations'],native_saved_pointer_guard=routine['native_saved_pointer_guard'])
 old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None);added=old is None
 if added:registry['routines'].append(entry);old=entry
 else:old.update(entry)
 old.setdefault('integration_evidence',{})['native_uv_near_plane_clipping']=integration
 for address in (0x8C618D18,0x8C619310,0x8C6064B0,0x8C6065B0,0x8C617E46,0x8C6184F0,0x8C61FFE4,0x8C620428,0x8C605E68,0x8C6069A4,0x8C6209A8,0x8C62159A,0x8C62113C,0x8C6215BA,0x8C04481A,0x8C0448B4,0x8C6215D6,0x8C605DAC,0x8C605E0A,0x8C07B320,0x8C09878C,0x8C09865C):
  parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
  parent.setdefault('integration_evidence',{})['native_uv_near_plane_clipping']=integration
 registry_path.write_text(json.dumps(registry,indent=2)+'\n')
 status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_uv_near_plane_clipping']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {int(added)} UV near-plane clipper; {len(registry["routines"])} total. Tested particle lifetimes use zero external game services; other rendering paths remain unfinished.')

if __name__=='__main__':main()
