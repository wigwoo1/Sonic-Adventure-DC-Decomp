"""Register seven complete original polygon/clipping helpers after original-code comparisons."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_polygon_lower_verification.json';MANIFEST='analysis/draw_polygon_lower_translation.json';CALLER='analysis/overlay_draw_polygon_lower_verification.json';CONNECTION='analysis/draw_polygon_lower_connection.json'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 body,manifest,caller,connection=[json.loads((PROJECT/p).read_text()) for p in (BODY,MANIFEST,CALLER,CONNECTION)]
 for report in (body,manifest,caller,connection):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert body['routines']==manifest['routines'] and manifest['instruction_rows']==1720 and len(manifest['routines'])==7
 assert all(not r['missing_rows'] and not r['missing_edges'] for r in body['coverage_by_routine'])
 assert body['rows_covered']==1720 and body['conditional_edges_covered']==46
 assert body['cases_passed']==576 and body['independent_cases']==389 and body['independent_checks']==1730 and connection['compiled_source_count']==108
 assert caller['cases_passed']==408 and caller['direct_service_cases']==64 and caller['candidate_static_rows']==911
 assert caller['candidate_rows_covered']>=476 and caller['candidate_rows_covered']+caller['candidate_unexecuted_rows']==911
 assert caller['recovered_caller_credit']==0 and caller['external_game_service_fixtures']==0
 assert '0x8c64f840' not in caller['first_unconnected_targets']
 assert caller['target_invocations']['0x8c64f840']>0 and caller['new_polygon_rows_reached']>0
 for report in (body,caller):assert all(c['full_state_match'] and c['ordered_boundaries_match'] and c['queue_match'] for c in report['cases'])
 assert digest(PROJECT/body['source'])==body['source_sha256'] and digest(ROOT/'work/sonic-adventure/build/draw_polygon_lower.dll')==body['dll_sha256']==caller['dll_sha256']
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),caller_verification=CALLER,caller_verification_sha256=digest(PROJECT/CALLER),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=7,complete_rows=1720,complete_conditional_outcomes=46,existing_lower_services_reused=['0x8c606a08','0x8c605e68','0x8c605f3c','0x8c6069a4'],existing_lower_service_recovery_credit=0,body_comparisons=body['cases_passed'],independent_cases=body['independent_cases'],caller_comparisons=408,caller_candidate_rows_covered=caller['candidate_rows_covered'],caller_candidate_unexecuted_rows=caller['candidate_unexecuted_rows'],caller_polygon_rows_reached=caller['new_polygon_rows_reached'],caller_queue_flushes=caller['queue_flushes'],caller_queue_events=caller['queue_events'],caller_stored_queue_event_prefix=caller['stored_queue_event_prefix'],caller_unstored_queue_event_count=caller['unstored_queue_event_count'],first_unconnected_targets=caller['first_unconnected_targets'],draw_candidate_recovery_credit=0,external_game_service_fixtures=0,scope=body['scope']+' '+caller['scope'],limitations=body['limitations']+caller['limitations'],next_task='Recover first missing lower drawing service from the saved original-caller prefixes. Preserve original data and explicit runtime metadata, numerical and queue limits. Incomplete effect bodies and AL_MAIN parent receive no registration credit.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();added=0
 entries={int(r['address'],16) for r in manifest['routines']}
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_polygon_lower.ps1',verifier_script='verify_draw_polygon_lower.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete original polygon helper: all exported rows and conditional outcomes checked against original instructions; full reached RAM/machine state comparisons. Rendering pipeline remains partial.',comparison_level=body['scope'],limitations=body['limitations'])
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:assert old['source']==body['source'];old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_polygon_lower']=integration
 reused={int(a,16) for report in (body,caller) for a in report['target_invocations']}-entries;attached=0
 for address in sorted(reused):
  old=next((r for r in registry['routines'] if int(r['address'],16)==address),None)
  if old is not None:old.setdefault('integration_evidence',{})['native_draw_polygon_lower']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_polygon_lower']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} polygon helper entries; {len(registry["routines"])} total. Existing evidence linked to {attached} entries with zero duplicate credit; incomplete effect/parent bodies remain unregistered.')
if __name__=='__main__':main()
