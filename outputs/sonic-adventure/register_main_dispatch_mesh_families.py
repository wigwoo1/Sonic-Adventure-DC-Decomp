"""Register combined renderer integration evidence without new routine credit."""
import collections,hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_mesh_families_verification.json'
LIFECYCLE='analysis/main_dispatch_mesh_families_lifecycle_verification.json'

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 report=json.loads((PROJECT/REPORT).read_text());life=json.loads((PROJECT/LIFECYCLE).read_text())
 expected=dict(cases_passed=1040,combined_cases=896,caller_contract_cases=144,material_families=9,native_targets_covered=46,boundary_comparisons=55128,numerical_operations=751528,external_service_boundaries=0,queue_flushes=33512,queue_events=324880,newly_recovered_entries=0)
 for k,v in expected.items():assert report[k]==v,k
 assert report['independent_checks']==dict(triangle_geometry=192,whole_mesh_reject=128,selection_counter=576,live_register_contract=144)
 assert len(report['cases'])==1040 and all(c['status']==0 and c['full_state_match'] and c['queue_match'] and c['external_service_boundaries']==0 for c in report['cases'])
 assert collections.Counter(c['input']['kind'] for c in report['cases'])==dict(combined=896,first_crossing=48,ordinary_then_crossing=48,crossing_then_crossing=48)
 for k,v in dict(lifecycles_passed=120,stage_comparisons=984,boundary_comparisons=23264,numerical_operations=104016,external_service_boundaries=0,created_parents=64,created_particles=80,paused_scheduler_passes=54,translation_cases_passed=46,translation_rows_covered=56,original_particle_mesh_iterations=880,vertex_worker_invocations=880,primitive_invocations=176,primitive_prefetches=744,near_invocations=160,near_prefetches=0,queue_flushes=10600,queue_events=117808).items():assert life[k]==v,k
 assert life['service_invocations']=={} and len(life['cases'])==120
 assert all(c['independent_pool_heap_lifetime_checks'] and all(s['full_state_match'] and s['geometry_calls']==0 for s in c['stages']) for c in life['cases'])
 for item in (report,life):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_mesh_families.dll')==report['dll_sha256']==life['dll_sha256']
 assert digest(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN')==report['binary_sha256']=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 integration=dict(verification=REPORT,verification_sha256=digest(PROJECT/REPORT),lifecycle_verification=LIFECYCLE,lifecycle_verification_sha256=digest(PROJECT/LIFECYCLE),body_comparisons=1040,lifecycles=120,lifecycle_stage_comparisons=984,newly_recovered_entries=0,new_instruction_rows=0,combined_material_mesh_cases=896,real_caller_strip_cases=144,native_targets_covered=46,explicit_primitive_fixture_targets=[],findings=report['findings'],scope=report['scope']+' '+life['scope'],limitations=report['limitations']+life['limitations'],next_task='Inventory topology and material combinations in original model data, including whether the observed non-alternate face-strip crossing combinations occur, before broader frame/GPU integration.')
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));before=len(registry['routines'])
 addresses={int(a,16) for a in report['targets']}|{0x8c6209a8,0x8c62159a,0x8c611444,0x8c6069a4,0x8c605e68,0x8c605f3c,0x8c6064b0,0x8c6065b0,0x8c611060,0x8c611122,0x8c6111d6,0x8c6112e2}
 for address in sorted(addresses):
  routine=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
  routine.setdefault('integration_evidence',{})['native_mesh_family_integration']=integration
 assert len(registry['routines'])==before
 registry_path.write_text(json.dumps(registry,indent=2)+'\n')
 status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_mesh_family_integration']=integration
 status['verified_progress']['recovered_buildable_routines']=before
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered combined evidence on {len(addresses)} existing routines; 0 new routines, {before} total. No duplicate credit.')

if __name__=='__main__':main()
