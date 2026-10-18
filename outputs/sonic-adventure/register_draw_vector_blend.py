"""Register one complete original vector blend; keep partial renderer credit zero."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_vector_blend_verification.json'
MANIFEST='analysis/draw_vector_blend_translation.json'
CALLER='analysis/overlay_draw_vector_blend_verification.json'
CONNECTION='analysis/draw_vector_blend_connection.json'
PRIOR='analysis/overlay_draw_angle_curve_verification.json'
ENTRY=0x8c60a1f6

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 body,manifest,caller,connection,prior=[json.loads((PROJECT/p).read_text()) for p in (BODY,MANIFEST,CALLER,CONNECTION,PRIOR)]
 for report in (body,manifest,caller,connection,prior):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert body['routines']==manifest['routines'] and len(manifest['routines'])==1 and manifest['instruction_rows']==27
 routine=manifest['routines'][0];assert int(routine['address'],16)==ENTRY
 assert body['cases_passed']==666 and body['rows_covered']==27 and body['conditional_edges_covered']==0
 assert body['independent_cases']==644 and body['independent_checks']==3864 and not body['unconnected_targets']
 assert body['registerable_entries']==[hex(ENTRY)] and all(not r['missing_rows'] and not r['missing_edges'] for r in body['coverage_by_routine'])
 assert connection['compiled_source_count']==118
 assert caller['cases_passed']==108 and caller['prior_corpus_cases']==108 and caller['affected_prior_cases_retested']==48 and caller['unaffected_regressions']==60
 indices=caller['selected_prior_case_indices'];affected=caller['affected_prior_case_indices']
 assert len(indices)==len(set(indices))==108 and len(affected)==48 and set(affected)<=set(indices)
 assert affected==[i for i,r in enumerate(prior['cases']) if r['status']==-2 and int(r['stop_pc'],16)==ENTRY]
 assert [r['input'] for r in caller['cases']]==[prior['cases'][i]['input'] for i in indices]
 assert all(r['status']==prior['cases'][i]['status'] for i,r in zip(indices,caller['cases']) if i not in affected)
 assert caller['direct_service_cases']==28 and caller['label_event_profiles']==12 and caller['camera_profiles']==48
 assert caller['native_original_initializer_stages']==108 and caller['native_original_viewport_stages']==48
 assert hex(ENTRY) not in caller['first_unconnected_targets'] and caller['target_invocations'][hex(ENTRY)]>0
 assert caller['new_curve_rows_reached']>0 and caller['recovered_caller_credit']==0 and caller['external_game_service_fixtures']==0
 for report in (body,caller):
  assert all(r['full_state_match'] and r['ordered_boundaries_match'] and r['queue_match'] and r['gbr_final_and_ordered_entries_match'] for r in report['cases'])
 assert all(r['native_original_initializer_stage_match'] for r in caller['cases'])
 assert digest(PROJECT/body['source'])==body['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/draw_vector_blend.dll')==body['dll_sha256']==caller['dll_sha256']
 source=(PROJECT/connection['source']).read_text()
 declaration=f'BDECL({routine["name"]});\n'
 route=f' if(target=={routine["address"]}u)return {routine["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus);\n'
 assert source.count(declaration)==source.count(route)==1
 source=source.replace(declaration,'').replace(route,'').replace('sa_draw_vector_blend_connected','sa_draw_angle_curve_connected')
 assert source==(PROJECT/'source/sa_draw_angle_curve_connected.c').read_text()
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),caller_verification=CALLER,caller_verification_sha256=digest(PROJECT/CALLER),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=1,new_entry_static_rows=27,new_entry_conditional_outcomes=0,body_comparisons=666,independent_cases=644,independent_checks=3864,caller_comparisons=108,prior_corpus_cases=108,affected_prior_cases_retested=48,unaffected_regressions=60,checked_renderer_initializer_preparation_stages=774,checked_viewport_preparation_stages=48,caller_candidate_rows_covered=caller['candidate_rows_covered'],caller_candidate_unexecuted_rows=caller['candidate_unexecuted_rows'],first_unconnected_targets=caller['first_unconnected_targets'],caller_queue_flushes=caller['queue_flushes'],caller_queue_events=caller['queue_events'],remaining_partial_motion_proposals=15,remaining_partial_primitive_proposals=3,remaining_partial_geometry_proposals=4,reused_routine_credit=0,draw_candidate_recovery_credit=0,external_game_service_fixtures=0,scope=body['scope']+' '+caller['scope'],limitations=body['limitations']+caller['limitations'],next_task='Recover the next original dependency recorded in first_unconnected_targets without replacing original input records or missing game services. Keep remaining clipping/MACH/integer-bank work required; incomplete parents remain unregistered.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();end=int(routine['end_exclusive'],16)
 assert hashlib.sha256(binary[ENTRY-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
 entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_vector_blend.ps1',verifier_script='verify_draw_vector_blend.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete original two-vector blend: all exported instructions/outcomes, reached/final state and independent sequential-overlap/staged binary32 formula/footprints checked within qualified domains.',comparison_level=integration['scope'],limitations=integration['limitations'])
 old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==ENTRY),None);added=int(old is None)
 if old is None:registry['routines'].append(entry);old=entry
 else:assert old['source']==body['source'];old.update(entry)
 old.setdefault('integration_evidence',{})['native_draw_vector_blend']=integration
 reused={int(a,16) for report in (body,caller) for a in report['target_invocations']}-{ENTRY};attached=0
 for address in sorted(reused):
  module='AL_MAIN.BIN' if address<0x80000000 else '1ST_READ.BIN'
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==module and int(r['address'],16)==address),None)
  if old is not None:old.setdefault('integration_evidence',{})['native_draw_vector_blend']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_vector_blend']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} vector blend entries; {len(registry["routines"])} total. Linked {attached} existing entries with zero duplicate credit; remaining motion/renderer parents stay partial.')
if __name__=='__main__':main()
