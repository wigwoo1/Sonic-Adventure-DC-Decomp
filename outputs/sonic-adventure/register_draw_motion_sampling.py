"""Register eight search/curve/dispatch and seventeen completed sampler/format bodies."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_motion_sampling_verification.json'
EXTENDED='analysis/draw_motion_sampling_extended_verification.json'
MANIFEST='analysis/draw_motion_sampling_translation.json'
CALLER='analysis/overlay_draw_motion_sampling_verification.json'
CONNECTION='analysis/draw_motion_sampling_connection.json'
PRIOR_BODY='analysis/draw_motion_variants_verification.json'
PRIOR_CALLER='analysis/overlay_draw_motion_variants_verification.json'
REUSED='analysis/draw_motion_sampling_reused_verification.json'
NEW={0x8c03feb8,0x8c040a60,0x8c03ff2c,0x8c03ff90,0x8c040c5c,0x8c040d60,0x8c60b596,0x8c60b4a8}
OLD={0x8c0400a0,0x8c0400f8,0x8c040150,0x8c0401a8,0x8c040df0,0x8c040e74,0x8c040ef8,0x8c0405b2,0x8c0405d2,0x8c040612,0x8c0406a0,0x8c0406e0,0x8c041516,0x8c04154e,0x8c041600,0x8c041666,0x8c0416cc}

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 body,extended,manifest,caller,connection,prior,prior_caller=[json.loads((PROJECT/p).read_text()) for p in (BODY,EXTENDED,MANIFEST,CALLER,CONNECTION,PRIOR_BODY,PRIOR_CALLER)]
 for report in (body,extended,manifest,caller,connection,prior,prior_caller):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert len(manifest['routines'])==9 and manifest['instruction_rows']==673 and body['routines']==manifest['routines']
 assert body['cases_passed']==1916 and extended['cases_passed']==524 and extended['independent_cases']==492
 assert body['independent_cases']==1086 and not extended['unconnected_targets'] and connection['compiled_source_count']==111
 assert caller['original_corpus_cases']==420 and caller['affected_cases_retested']==48
 assert caller['label_event_profiles']==12 and caller['direct_service_cases']==64
 indices=caller['selected_case_indices'];assert indices==sorted(set(indices)) and len(indices)==caller['cases_passed']
 affected={i for i,r in enumerate(prior_caller['cases']) if r['status']==-2 and int(r['stop_pc'],16) in NEW}
 assert len(affected)==48 and affected<=set(indices)
 assert all(r['input']==prior_caller['cases'][i]['input'] for i,r in zip(indices,caller['cases']))
 assert not ({'0x8c03feb8','0x8c60b596','0x8c040a60'}&set(caller['first_unconnected_targets']))
 assert all(caller['target_invocations'].get(hex(a),0)>0 for a in (0x8c03feb8,0x8c60b596,0x8c040a60))
 for report in (body,extended,caller):assert all(c['full_state_match'] and c['ordered_boundaries_match'] and c['queue_match'] for c in report['cases'])
 assert digest(PROJECT/body['source'])==body['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/draw_motion_sampling.dll')==body['dll_sha256']==extended['dll_sha256']==caller['dll_sha256']
 selected=[r for r in manifest['routines'] if int(r['address'],16) in NEW]+[r for r in prior['routines'] if int(r['address'],16) in OLD]
 assert len(selected)==25 and sum(r['instruction_rows'] for r in selected)==1150 and sum(len(r['conditional_edges']) for r in selected)==38
 assert len(NEW)==8 and len(OLD)==17 and NEW.isdisjoint(OLD)
 for routine in selected:
  a=routine['address'];coverage=next(r for r in body['coverage_by_routine'] if r['address']==a)
  assert not coverage['missing_rows'] and not coverage['missing_edges'] and a in body['registerable_entries']
  tests=[r for report in (body,extended) for r in report['cases'] if r['input']['entry']==int(a,16)]
  assert tests and all(r['status']!=-2 for r in tests),(a,'incomplete direct-entry dependency')
 # Prove the prior native dispatch remains text-identical after removing the
 # nine added declarations/routes and restoring the public entry name.
 source=(PROJECT/connection['source']).read_text()
 for r in manifest['routines']:
  decl=f'DECL({r["name"]});\n';route=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop);\n'
  assert source.count(decl)==source.count(route)==1;source=source.replace(decl,'').replace(route,'')
 assert source.replace('sa_draw_motion_sampling_connected','sa_draw_motion_variants_connected')==(PROJECT/'source/sa_draw_motion_variants_connected.c').read_text()
 files=[Path(__file__)]+[PROJECT/p for p in (BODY,EXTENDED,MANIFEST,CALLER,CONNECTION,PRIOR_BODY,PRIOR_CALLER)]
 reused=dict(source=prior['source'],source_sha256=digest(PROJECT/prior['source']),routines=[r for r in selected if int(r['address'],16) in OLD],coverage_by_routine=[r for r in body['coverage_by_routine'] if int(r['address'],16) in OLD],cases_passed=body['cases_passed']+extended['cases_passed'],complete_routine_count=17,complete_instruction_rows=749,complete_conditional_outcomes=14,dll_sha256=body['dll_sha256'],sha256={str(p):digest(p) for p in files},scope='Seventeen previously compiled sampler/format proposals now have complete original instruction/outcome and reached native dependency verification through real key search/linear interpolation. Original source and earlier partial evidence are preserved.',limitations=body['limitations'])
 (PROJECT/REUSED).write_text(json.dumps(reused,indent=2)+'\n')
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),supplemental_verification=EXTENDED,supplemental_verification_sha256=digest(PROJECT/EXTENDED),reused_source_verification=REUSED,reused_source_verification_sha256=digest(PROJECT/REUSED),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),caller_verification=CALLER,caller_verification_sha256=digest(PROJECT/CALLER),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=25,new_source_entries=8,previously_partial_entries_now_complete=17,new_entry_static_rows=1150,new_entry_conditional_outcomes=38,remaining_partial_proposals=15,body_comparisons=body['cases_passed']+extended['cases_passed'],independent_cases=body['independent_cases']+extended['independent_cases'],independent_checks=body['independent_checks']+extended['independent_checks'],caller_comparisons=caller['cases_passed'],prior_original_corpus_cases=420,affected_original_cases_retested=48,selected_prior_regression_cases=caller['regression_cases'],full_original_corpus_freshly_retested=caller['full_original_corpus_freshly_retested'],caller_sampling_rows_reached=caller['new_sampling_rows_reached'],caller_candidate_rows_covered=caller['candidate_rows_covered'],caller_candidate_unexecuted_rows=caller['candidate_unexecuted_rows'],first_unconnected_targets=caller['first_unconnected_targets'],caller_queue_flushes=caller['queue_flushes'],caller_queue_events=caller['queue_events'],reused_routine_credit=0,external_game_service_fixtures=0,draw_candidate_recovery_credit=0,scope=body['scope']+' '+extended['scope']+' '+caller['scope'],limitations=body['limitations']+caller['limitations'],next_task='Recover the first remaining original renderer/geometry/blend service in the affected initialized-model cases. Preserve explicit runtime metadata, original model/motion records, GBR special-register state where used and qualified numerical contracts. Partial hierarchy/mesh/blend bodies and AL_MAIN parent remain unregistered.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();added=0
 for routine in selected:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  original=start in OLD;source=prior['source'] if original else body['source']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=source,verification=REUSED if original else BODY,native_build_script='build_draw_motion_sampling.ps1',verifier_script='verify_draw_motion_sampling.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete original motion key-search/interpolation/sampler/format/buffer-dispatch helper: every exported instruction and conditional outcome checked with real reached native services. Qualified numerical/valid-input domains retained; renderer remains partial.',comparison_level=integration['scope'],limitations=integration['limitations'])
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:assert old['source']==source;old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_motion_sampling']=integration
 reused_targets={int(a,16) for report in (body,caller) for a in report['target_invocations']}-NEW-OLD;attached=0
 for address in sorted(reused_targets):
  old=next((r for r in registry['routines'] if int(r['address'],16)==address),None)
  if old is not None:old.setdefault('integration_evidence',{})['native_draw_motion_sampling']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_motion_sampling']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} motion sampling entries ({len(NEW)} new source + {len(OLD)} completed earlier proposals); {len(registry["routines"])} total. Linked {attached} existing entries with zero duplicate credit; 15 proposals/parent remain partial.')
if __name__=='__main__':main()
