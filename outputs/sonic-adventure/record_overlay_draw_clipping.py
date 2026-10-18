"""Record complete real-object regression evidence, with zero parent recovery credit."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent;BODY='analysis/overlay_draw_clipping_verification.json'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 report=json.loads((PROJECT/BODY).read_text());prior=json.loads((PROJECT/'analysis/overlay_draw_vector_blend_verification.json').read_text())
 for p,h in report['sha256'].items():assert digest(p)==h,p
 assert digest(ROOT/'work/sonic-adventure/build/draw_clipping_complete.dll')==report['dll_sha256']
 assert report['cases_passed']==len(report['cases'])==156 and report['status_counts']=={'0':156} and not report['first_unconnected_targets'] and not report['normal_bank1_live_in']
 assert report['preserved_prior_corpus_cases']==len(prior['cases'])==108 and report['near_plane_profiles']==48
 for i,r in enumerate(report['cases'][:108]):
  assert r['input']['prior_108_index']==i
  assert all(r['input'][k]==v for k,v in prior['cases'][i]['input'].items()),(i,'retained source inputs')
 assert all(r['input'].get('near_plane_profile') for r in report['cases'][108:])
 assert report['candidate_static_rows']==911 and report['candidate_rows_covered']==527 and report['candidate_unexecuted_rows']==384 and report['recovered_caller_credit']==0
 assert report['clipping_rows_reached']==1081 and report['boundary_comparisons']==168470
 assert report['queue_flushes']==113370 and report['queue_events']==1200306 and report['stored_queue_event_prefix']==474820 and report['unstored_queue_event_count']==725486
 assert report['native_original_initializer_stages']==156 and report['native_original_viewport_stages']==96
 assert report['newly_registered_entries']==report['external_game_service_fixtures']==0
 assert all(r['full_state_match'] and r['ordered_boundaries_match'] and r['queue_match'] for r in report['cases'])
 clipping={a:report['target_invocations'].get(a,0) for a in ('0x8c037ccc','0x8c037ee0','0x8c037ff4','0x8c0382ac','0x8c038516','0x8c038668','0x8c038802')}
 assert clipping=={'0x8c037ccc':40,'0x8c037ee0':316,'0x8c037ff4':344,'0x8c0382ac':8,'0x8c038516':28,'0x8c038668':0,'0x8c038802':0}
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),compiled_source_count=122,dll_sha256=report['dll_sha256'],body_comparisons=156,retained_prior_inputs=108,additional_saved_near_plane_profiles=48,normal_returns=156,candidate_static_rows=911,candidate_rows_covered=527,candidate_unexecuted_rows=384,clipping_rows_reached=1081,local_clipping_invocations=clipping,ordered_boundaries=168470,queue_flushes=113370,queue_events=1200306,stored_queue_event_prefix=474820,unstored_queue_event_count=725486,checked_current_dll_initializer_stages=156,checked_current_dll_viewport_stages=96,newly_registered_entries=0,recovered_caller_credit=0,reused_routine_credit=0,external_game_service_fixtures=0,scope=report['scope'],limitations=report['limitations']+['The textured/environment near-plane paths do reach the new clipping family. The solid clipper/parent do not execute in this outer corpus; their separate full-body proof remains distinct.','This is the preserved 122-source clipping DLL, not the later 126-source complete-view DLL. Their verification scopes and RAM/register backends must not be combined into an unexecuted all-in-one result.'],guide='OBJECT_DRAW_CLIPPING.txt')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));before=len(registry['routines']);attached=0
 assert not any(r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==0x0c9058a0 for r in registry['routines'])
 for a in report['target_invocations']:
  address=int(a,16);module='AL_MAIN.BIN' if address<0x80000000 else '1ST_READ.BIN'
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==module and int(r['address'],16)==address),None)
  if old is not None:old.setdefault('integration_evidence',{})['native_overlay_draw_clipping']=dict(integration,new_recovery_credit=0);attached+=1
 assert len(registry['routines'])==before
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_overlay_draw_clipping']=integration;path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Recorded 156 complete object comparisons; {attached} existing entries linked, zero new registrations. AL_MAIN draw parent remains partial.')
if __name__=='__main__':main()
