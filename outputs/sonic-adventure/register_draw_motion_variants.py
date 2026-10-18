"""Register twelve complete motion primitives; retain 31 partial proposals."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_motion_variants_verification.json'
EXTENDED='analysis/draw_motion_variants_extended_verification.json'
MANIFEST='analysis/draw_motion_variants_translation.json'
CALLER='analysis/overlay_draw_motion_variants_verification.json'
CONNECTION='analysis/draw_motion_variants_connection.json'
ENTRIES={0x8c03fe1a,0x8c03fe60,0x8c040b90,0x8c040c02,0x8c04057a,0x8c040588,0x8c040596,0x8c0405a4,0x8c0414a6,0x8c0414cc,0x8c0414f0,0x8c041732}

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 body,extended,manifest,caller,connection=[json.loads((PROJECT/p).read_text()) for p in (BODY,EXTENDED,MANIFEST,CALLER,CONNECTION)]
 for report in (body,extended,manifest,caller,connection):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert body['routines']==manifest['routines'] and manifest['instruction_rows']==2070 and len(manifest['routines'])==43
 assert body['cases_passed']==1272 and extended['cases_passed']==498
 assert body['independent_cases']==735 and extended['independent_cases']==300 and not extended['unconnected_targets']
 assert connection['compiled_source_count']==110
 assert caller['cases_passed']==420 and caller['label_event_profiles']==12 and caller['direct_service_cases']==64
 assert caller['candidate_static_rows']==911 and caller['candidate_rows_covered']+caller['candidate_unexecuted_rows']==911
 assert caller['recovered_caller_credit']==0 and caller['external_game_service_fixtures']==0
 assert not ({'0x8c0409c0','0x8c5fde20','0x8c041c92'}&set(caller['first_unconnected_targets']))
 assert all(caller['target_invocations'].get(hex(a),0)>0 for a in (0x8c0409c0,0x8c5fde20,0x8c041c92))
 for report in (body,extended,caller):assert all(c['full_state_match'] and c['ordered_boundaries_match'] and c['queue_match'] for c in report['cases'])
 assert digest(PROJECT/body['source'])==body['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/draw_motion_variants.dll')==body['dll_sha256']==extended['dll_sha256']==caller['dll_sha256']
 extra_rows=set(extended['instruction_coverage']);extra_edges=set(map(tuple,extended['conditional_outcomes']))
 selected=[r for r in manifest['routines'] if int(r['address'],16) in ENTRIES]
 assert len(selected)==12 and sum(r['instruction_rows'] for r in selected)==286 and sum(len(r['conditional_edges']) for r in selected)==30
 for routine in selected:
  a=routine['address'];coverage=next(r for r in body['coverage_by_routine'] if r['address']==a)
  assert set(coverage['missing_rows'])<=extra_rows and set(map(tuple,coverage['missing_edges']))<=extra_edges
  tests=[r for report in (body,extended) for r in report['cases'] if r['input']['entry']==int(a,16)]
  assert tests and all(r['status']!=-2 for r in tests),(a,'unconnected complete-body case')
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),supplemental_verification=EXTENDED,supplemental_verification_sha256=digest(PROJECT/EXTENDED),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),caller_verification=CALLER,caller_verification_sha256=digest(PROJECT/CALLER),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=12,new_entry_static_rows=286,new_entry_reachable_rows=286,new_entry_conditional_outcomes=30,translated_proposals=43,translated_instruction_rows=2070,remaining_unregistered_proposals=31,proposal_rows_executed=body['rows_covered'],proposal_conditional_outcomes_executed=body['conditional_edges_covered'],body_comparisons=1770,independent_cases=1035,independent_checks=3926,caller_comparisons=420,caller_motion_rows_reached=caller['new_motion_rows_reached'],caller_candidate_rows_covered=caller['candidate_rows_covered'],caller_candidate_unexecuted_rows=caller['candidate_unexecuted_rows'],first_unconnected_targets=caller['first_unconnected_targets'],caller_queue_flushes=caller['queue_flushes'],caller_queue_events=caller['queue_events'],draw_candidate_recovery_credit=0,external_game_service_fixtures=0,reused_routine_credit=0,scope=body['scope']+' '+extended['scope']+' '+caller['scope'],limitations=body['limitations']+caller['limitations'],next_task='Recover the first missing original geometry/motion service from saved initialized-model prefixes. Preserve unmodified model/motion records, explicit runtime metadata and partial-body boundaries. No complete-renderer or AL_MAIN parent registration credit.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();added=0
 for routine in selected:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_motion_variants.ps1',verifier_script='verify_draw_motion_variants.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete original motion selector/setup/default-pose or model-buffer primitive: every exported instruction and conditional outcome checked. Original rendering pipeline remains partial.',comparison_level=integration['scope'],limitations=integration['limitations'])
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:assert old['source']==body['source'];old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_motion_variants']=integration
 reused={int(a,16) for report in (body,caller) for a in report['target_invocations']}-ENTRIES;attached=0
 for address in sorted(reused):
  old=next((r for r in registry['routines'] if int(r['address'],16)==address),None)
  if old is not None:old.setdefault('integration_evidence',{})['native_draw_motion_variants']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_motion_variants']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} complete motion primitive entries; {len(registry["routines"])} total. Linked {attached} reused entries with zero duplicate credit. Other 31 proposals and parent remain unregistered.')
if __name__=='__main__':main()
