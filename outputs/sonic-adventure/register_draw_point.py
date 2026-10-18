"""Register the distinct original point-transform body with bounded caller evidence."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_point_verification.json';MANIFEST='analysis/draw_point_translation.json';CALLER='analysis/overlay_draw_point_verification.json';CONNECTION='analysis/draw_point_connection.json'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 body,manifest,caller,connection=[json.loads((PROJECT/p).read_text()) for p in (BODY,MANIFEST,CALLER,CONNECTION)]
 for report in (body,manifest,caller,connection):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert body['routines']==manifest['routines'] and len(manifest['routines'])==1
 assert body['cases_passed']==435 and body['rows_covered']==44 and body['conditional_edges_covered']==2 and body['independent_cases']==124 and body['boundary_comparisons']==435
 assert body['status_counts']=={'0':269,'-1':140,'-3':20,'1':6} and body['external_game_service_fixtures']==0
 assert connection['compiled_source_count']==105 and body['newly_registered_entries']==1
 assert caller['cases_passed']==408 and caller['direct_service_cases']==64 and caller['candidate_static_rows']==911
 assert caller['candidate_rows_covered']>=472 and caller['candidate_rows_covered']+caller['candidate_unexecuted_rows']==911
 assert caller['recovered_caller_credit']==0 and caller['external_game_service_fixtures']==0 and '0x8c638ecc' not in caller['first_unconnected_targets']
 assert caller['target_invocations']['0x8c638ecc']>0 and caller['new_point_rows_reached']>0
 for report in (body,caller):assert all(c['full_state_match'] and c['ordered_boundaries_match'] and c['queue_match'] for c in report['cases'])
 assert digest(PROJECT/body['source'])==body['source_sha256'] and digest(ROOT/'work/sonic-adventure/build/draw_point.dll')==body['dll_sha256']==caller['dll_sha256']
 body['limitations'].append('Direct point fixtures inherit DN=1; special-value fpscr_bits labels are ORed into that state and do not establish DN=0 coverage. This qualification does not alter the saved direct report or executed cases.')
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),caller_verification=CALLER,caller_verification_sha256=digest(PROJECT/CALLER),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=1,original_instruction_rows=44,conditional_outcomes=2,body_comparisons=435,independent_dyadic_cases=124,caller_comparisons=408,caller_candidate_rows_covered=caller['candidate_rows_covered'],caller_candidate_unexecuted_rows=caller['candidate_unexecuted_rows'],caller_new_point_rows_reached=caller['new_point_rows_reached'],caller_point_invocations=caller['target_invocations']['0x8c638ecc'],caller_queue_flushes=caller['queue_flushes'],caller_queue_events=caller['queue_events'],caller_stored_queue_event_prefix=caller['stored_queue_event_prefix'],caller_unstored_queue_event_count=caller['unstored_queue_event_count'],first_unconnected_targets=caller['first_unconnected_targets'],draw_candidate_recovery_credit=0,reused_routine_credit=0,external_game_service_fixtures=0,scope=body['scope']+' '+caller['scope'],limitations=body['limitations']+caller['limitations'],next_task='Recover the next reached original draw services and remaining parent paths, then natural renderer/texture initialization. Keep original geometry/motion and explicit numerical/queue limits. Parent remains unregistered; no pixels/frame/gameplay/playable-port claim.')
 integration['next_family_inventory']='analysis/draw_object_effects_inventory.json'
 integration['next_family_inventory_credit']=0
 integration['next_task']='Recover reached object-effect dispatcher 0c909d80 and its original services, with the 12-body / 2421-row read-only inventory as analysis only. Resolve remaining draw boundaries and natural renderer/texture initialization; retain original records and numerical/queue limits. No premature parent or draft-source credit.'
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));routine=manifest['routines'][0];start,end=int(routine['address'],16),int(routine['end_exclusive'],16);binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 assert hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
 assert not any(r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==0xc9058a0 for r in registry['routines'])
 old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None);added=int(old is None)
 entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_point.ps1',verifier_script='verify_draw_point.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete guarded original point transform; all rows/outcomes, full reached state and real numerical support checked. Original-object draw integration with explicit remaining service boundaries; no GPU-pixel claim.',comparison_level=body['scope'],limitations=body['limitations'])
 if old is None:registry['routines'].append(entry);old=entry
 else:assert old['source']==body['source'];old.update(entry)
 old.setdefault('integration_evidence',{})['native_draw_point']=integration
 attached=0
 for text in sorted(caller['target_invocations']):
  address=int(text,16)
  if address in (start,0xc9058a0):continue
  module='AL_MAIN.BIN' if address==0xc905618 else '1ST_READ.BIN'
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==module and int(r['address'],16)==address),None)
  if old is not None:old.setdefault('integration_evidence',{})['native_draw_point']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_point']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} first point-transform entry; {len(registry["routines"])} total. Reused evidence attached to {attached} existing entries; zero duplicate/parent credit.')
if __name__=='__main__':main()
