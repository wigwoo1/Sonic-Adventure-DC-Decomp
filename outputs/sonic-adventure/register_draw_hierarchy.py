"""Register five first hierarchy entries; extend existing bank-store evidence."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_hierarchy_verification.json';MANIFEST='analysis/draw_hierarchy_translation.json';CALLER='analysis/overlay_draw_hierarchy_verification.json';CONNECTION='analysis/draw_hierarchy_connection.json';AUDIT='analysis/draw_hierarchy_bounds_audit.json'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 body,manifest,caller,connection,audit=[json.loads((PROJECT/p).read_text()) for p in (BODY,MANIFEST,CALLER,CONNECTION,AUDIT)]
 for report in (body,manifest,caller,connection,audit):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert body['routines']==manifest['routines'] and len(manifest['routines'])==6
 assert body['cases_passed']==595 and body['rows_covered']==836 and body['static_rows']==844 and body['conditional_edges_covered']==98
 assert len(body['unreachable_rows'])==audit['unreachable_rows']==8 and len(body['unreachable_conditional_edges'])==audit['unreachable_conditional_outcomes']==4
 assert body['newly_registered_entries']==5 and body['existing_entry_extensions']==1 and body['external_game_service_fixtures']==0
 assert connection['compiled_source_count']==104 and connection['research_define']=='SA_BOUND_RETAIL_STALLS=1'
 assert caller['cases_passed']==408 and caller['direct_service_cases']==64 and caller['candidate_static_rows']==911
 assert caller['candidate_rows_covered']>=461 and caller['candidate_rows_covered']+caller['candidate_unexecuted_rows']==911
 assert caller['recovered_caller_credit']==0 and caller['external_game_service_fixtures']==0 and '0x8c0583a0' not in caller['first_unconnected_targets']
 for address in (0x8c0583a0,0x8c057bc0):assert caller['target_invocations'][hex(address)]>0
 for report in (body,caller):assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in report['cases'])
 assert all(c['queue_match'] for c in caller['cases'])
 assert digest(PROJECT/body['source'])==body['source_sha256'] and digest(ROOT/'work/sonic-adventure/build/draw_hierarchy.dll')==body['dll_sha256']==caller['dll_sha256']
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),caller_verification=CALLER,caller_verification_sha256=digest(PROJECT/CALLER),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),bounds_audit=AUDIT,bounds_audit_sha256=digest(PROJECT/AUDIT),alternative_source=body['source'],alternative_source_sha256=body['source_sha256'],newly_registered_entries=5,existing_entry_extensions=1,original_static_rows=844,qualified_reachable_rows=836,unreachable_rows=body['unreachable_rows'],conditional_outcomes_covered=98,body_comparisons=595,independent_cases=body['independent_cases'],independent_world_matrices=body['independent_world_matrices'],independent_angle_handoffs=body['independent_angle_handoffs'],caller_comparisons=408,caller_candidate_rows_covered=caller['candidate_rows_covered'],caller_candidate_unexecuted_rows=caller['candidate_unexecuted_rows'],caller_queue_flushes=caller['queue_flushes'],caller_queue_events=caller['queue_events'],caller_stored_queue_event_prefix=caller['stored_queue_event_prefix'],caller_unstored_queue_event_count=caller['unstored_queue_event_count'],first_unconnected_targets=caller['first_unconnected_targets'],retail_stall=body['retail_stall'],draw_candidate_recovery_credit=0,reused_routine_credit=0,external_game_service_fixtures=0,scope=body['scope']+' '+caller['scope'],limitations=body['limitations']+caller['limitations']+audit['assumptions'],next_task='Recover the next original draw-service boundary and remaining parent paths, then natural renderer/texture initialization. Retain original geometry/motion and explicit software queue limits. Parent remains unregistered; no whole-game, pixels, frame-rate or playable-port claim.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));added=0;binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 assert not any(r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==0xc9058a0 for r in registry['routines'])
 new={int(r['address'],16) for r in manifest['routines'] if r['first_registry_entry']}
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if not routine['first_registry_entry']:
   assert old is not None and old['source']=='source/model_matrix_bank_save.c'
   old.setdefault('integration_evidence',{})['native_draw_hierarchy']=dict(integration,new_recovery_credit=0)
   continue
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_hierarchy.ps1',verifier_script='verify_draw_hierarchy.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete guarded original body; qualified reachable paths and explicit retained fallback/stall behavior checked. Real vector backend; original-object caller integration. No natural renderer initialization or GPU-pixel claim.',comparison_level=body['scope'],limitations=body['limitations']+audit['assumptions'])
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:assert old['source']==body['source'];old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_hierarchy']=integration
 reused={int(a,16) for report in (body,caller) for a in report['target_invocations']}-new-{0xc9058a0,0x8c638f94};attached=0
 for address in sorted(reused):
  module='AL_MAIN.BIN' if address==0xc905618 else '1ST_READ.BIN'
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==module and int(r['address'],16)==address),None)
  if old is not None:old.setdefault('integration_evidence',{})['native_draw_hierarchy']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_hierarchy']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} first hierarchy entries; {len(registry["routines"])} total. Existing bank-store extended, {attached} other reused entries linked with zero duplicate/parent credit.')
if __name__=='__main__':main()
