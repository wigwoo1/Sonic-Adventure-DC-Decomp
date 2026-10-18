"""First registry entries for complete scale/translation and linked evidence."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_matrix_registers_verification.json';MANIFEST='analysis/draw_matrix_registers_translation.json'
CALLERS='analysis/draw_matrix_model_callers_verification.json';LIFE='analysis/draw_matrix_renderer_lifecycle_verification.json';CONNECTION='analysis/draw_matrix_renderer_connection.json'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 body,manifest,callers,life,connection=[json.loads((PROJECT/p).read_text()) for p in (BODY,MANIFEST,CALLERS,LIFE,CONNECTION)]
 for k,v in dict(cases_passed=856,rows_covered=132,conditional_edges_covered=4,independent_cases=256,external_game_service_fixtures=0,newly_registered_entries=2,reused_prior_implementation_credit=0).items():assert body[k]==v,k
 assert body['status_counts']=={'0':558,'1':42,'-1':192,'-3':64}
 assert body['routines']==manifest['routines'] and len(manifest['routines'])==2
 for k,v in dict(cases_passed=64,boundary_comparisons=2384,numerical_operations=13960,mesh_calls=160,queue_flushes=800,queue_events=8860,external_game_service_fixtures=0,independent_cases=64,newly_recovered_entries=0,reused_routine_credit=0).items():assert callers[k]==v,k
 assert callers['matrix_invocations']=={'0x8c63a69c':64,'0x8c63a878':96}
 assert all(c['status']==0 and c['full_state_match'] and c['queue_match'] and c['independent_mesh_order_and_stack'] and c['external_service_boundaries']==0 for c in callers['cases'])
 for k,v in dict(lifecycles_passed=120,stage_comparisons=984,boundary_comparisons=23264,numerical_operations=104016,external_service_boundaries=0,created_parents=64,created_particles=80,translation_cases_passed=46,translation_rows_covered=56,queue_flushes=10600,queue_events=117808).items():assert life[k]==v,k
 assert life['service_invocations']=={} and life['matrix_invocations']=={'0x8c63a69c':0,'0x8c63a878':0}
 assert all(c['independent_pool_heap_lifetime_checks'] and all(s['full_state_match'] for s in c['stages']) for c in life['cases'])
 for report in (body,manifest,callers,life,connection):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert digest(PROJECT/body['source'])==body['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/draw_matrix_registers.dll')==body['dll_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/draw_matrix_renderer.dll')==callers['dll_sha256']==life['dll_sha256']
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),caller_verification=CALLERS,caller_verification_sha256=digest(PROJECT/CALLERS),lifecycle_verification=LIFE,lifecycle_verification_sha256=digest(PROJECT/LIFE),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=2,complete_instruction_rows=132,reused_prior_implementation_credit=0,focused_cases=856,independent_matrix_cases=256,transformed_model_callers=64,scale_calls=64,translate_calls=96,mesh_calls=160,regression_lifecycles=120,regression_stages=984,regression_reaches_new_matrix_helpers=False,external_game_service_fixtures=0,scope=body['scope']+' '+callers['scope'],limitations=body['limitations']+callers['limitations']+life['limitations'],next_task='Recover remaining AL_MAIN draw callback dependencies, beginning 8c038d00 and 8c043a20. Actual AL_MAIN drawing/natural creation/task scheduling and hardware pixels remain unfinished.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();added=0
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if old is not None:assert old['source']==body['source'],'Preserve earlier canonical entries instead of replacing them'
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_matrix_registers.ps1',verifier_script='verify_draw_matrix_registers.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete guarded static C; first registry entry. Existing limited scale/implicit-translation sources preserved. Full original state, independent matrix algebra and actual native model callers checked.',comparison_level=body['scope'],limitations=body['limitations'])
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_matrix_registers']=integration
 for address in (0x8c62113c,0x8c6215ba,0x8c6209a8):
  entry=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
  entry.setdefault('integration_evidence',{})['native_draw_matrix_registers']=dict(integration,new_recovery_credit=0)
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_matrix_registers']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} first-time complete matrix entries; {len(registry["routines"])} total. No duplicate credit for prior limited source or reused renderer.')
if __name__=='__main__':main()
