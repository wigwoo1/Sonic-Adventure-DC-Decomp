"""Register only the six fully checked profiles; leave renderer bodies uncredited."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_object_effects_verification.json';MANIFEST='analysis/draw_object_effects_translation.json';CALLER='analysis/overlay_draw_object_effects_verification.json';CONNECTION='analysis/draw_object_effects_connection.json'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 body,manifest,caller,connection=[json.loads((PROJECT/p).read_text()) for p in (BODY,MANIFEST,CALLER,CONNECTION)]
 for report in (body,manifest,caller,connection):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert body['routines']==manifest['routines'] and manifest['instruction_rows']==2425 and len(manifest['routines'])==13
 entries={int(a,16) for a in body['registerable_entries']};wanted={int(r['address'],16) for r in manifest['routines'] if r['leaf_profile']};assert entries==wanted and len(entries)==6
 coverage={int(r['address'],16):r for r in body['coverage_by_routine']}
 for a in entries:assert not coverage[a]['missing_rows'] and not coverage[a]['missing_edges']
 assert sum(coverage[a]['rows_covered'] for a in entries)==1492 and sum(coverage[a]['conditional_edges_covered'] for a in entries)==130
 assert body['cases_passed']==1720 and body['status_counts']=={'0':1506,'-3':41,'-2':33,'-1':60,'1':80}
 assert body['boundary_comparisons']==2352 and body['independent_cases']==707 and body['independent_checks']==777
 assert body['unconnected_targets']=={'0x8c654e60':32,'0x8c64f5bc':1}
 assert connection['compiled_source_count']==106 and body['external_game_service_fixtures']==0
 assert caller['cases_passed']==408 and caller['direct_service_cases']==64 and caller['candidate_static_rows']==911
 assert caller['candidate_rows_covered']>=476 and caller['candidate_rows_covered']+caller['candidate_unexecuted_rows']==911
 assert caller['recovered_caller_credit']==0 and caller['external_game_service_fixtures']==0 and '0xc909d80' not in caller['first_unconnected_targets']
 assert caller['target_invocations']['0xc909d80']>0 and caller['new_effect_rows_reached']>0
 for report in (body,caller):assert all(c['full_state_match'] and c['ordered_boundaries_match'] and c['queue_match'] for c in report['cases'])
 assert digest(PROJECT/body['source'])==body['source_sha256'] and digest(ROOT/'work/sonic-adventure/build/draw_object_effects.dll')==body['dll_sha256']==caller['dll_sha256']
 partial=[r['address'] for r in manifest['routines'] if not r['leaf_profile'] and not r['existing_registered_entry']]
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),caller_verification=CALLER,caller_verification_sha256=digest(PROJECT/CALLER),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=6,complete_profile_rows=1492,complete_profile_conditional_outcomes=130,compiled_partial_entries=partial,compiled_partial_registration_credit=0,existing_phase_extension_credit=0,reused_rng_credit=0,body_comparisons=1720,independent_cases=707,independent_checks=777,caller_comparisons=408,caller_candidate_rows_covered=caller['candidate_rows_covered'],caller_candidate_unexecuted_rows=caller['candidate_unexecuted_rows'],caller_effect_rows_reached=caller['new_effect_rows_reached'],caller_dispatcher_invocations=caller['target_invocations']['0xc909d80'],caller_queue_flushes=caller['queue_flushes'],caller_queue_events=caller['queue_events'],caller_stored_queue_event_prefix=caller['stored_queue_event_prefix'],caller_unstored_queue_event_count=caller['unstored_queue_event_count'],first_unconnected_targets=caller['first_unconnected_targets'],draw_candidate_recovery_credit=0,external_game_service_fixtures=0,scope=body['scope']+' '+caller['scope'],limitations=body['limitations']+caller['limitations']+['The reused random helper retains its original valid-RAM/stack contract; its canonical source and proof are preserved. Only the six profiles are newly registered.'],next_task='Recover newly reached lower renderer/texture/header services and all remaining paths of the six compiled partial bodies, using the saved native prefixes. Preserve original records, the qualified 14-name runtime texture metadata and numerical/queue limits. No premature dispatcher/emitter/main-service/parent credit.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));binary=(ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN').read_bytes();added=0
 excluded={int(a,16) for a in partial}|{0xc9058a0}
 excluded_pairs={(r['module'],int(r['address'],16)) for r in manifest['routines'] if r['address'] in partial}|{('AL_MAIN.BIN',0xc9058a0)}
 assert not any((r.get('module','1ST_READ.BIN'),int(r['address'],16)) in excluded_pairs for r in registry['routines'])
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  if start not in entries:continue
  assert hashlib.sha256(binary[start-0xc900000:end-0xc900000]).hexdigest()==routine['retail_span_sha256']
  old=next((r for r in registry['routines'] if r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==start),None)
  entry=dict(module='AL_MAIN.BIN',load_address='0x0c900000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_object_effects.ps1',verifier_script='verify_draw_object_effects.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete original effect profile; all rows/conditional outcomes and full reached machine/RAM states checked. Original initialization, countdown, springs and phase behavior retained. Renderer remains explicitly partial.',comparison_level=body['scope'],limitations=body['limitations'])
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:assert old['source']==body['source'];old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_object_effects']=integration
 reused={int(a,16) for report in (body,caller) for a in report['target_invocations']}-entries-excluded;attached=0
 for address in sorted(reused):
  module='AL_MAIN.BIN' if address==0xc905618 else '1ST_READ.BIN'
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==module and int(r['address'],16)==address),None)
  if old is not None:
   extra=dict(integration,new_recovery_credit=0)
   if address==0x8c63a9b8:extra.update(alternative_source=body['source'],alternative_source_sha256=body['source_sha256'],adapter='Four original instructions with shared real FSCA core; canonical polynomial source/proof unchanged. Mathematical manual-error-bound validation, not measured bit-exact hardware output.')
   old.setdefault('integration_evidence',{})['native_draw_object_effects']=extra;attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_object_effects']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} first profile entries; {len(registry["routines"])} total. Six partial bodies/parent unregistered; phase/RNG/reused evidence linked to {attached} existing entries with zero duplicate credit.')
if __name__=='__main__':main()
