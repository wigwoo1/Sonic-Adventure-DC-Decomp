"""Record qualified control-state progress without registering partial bodies."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 names=('sh4_integer_state_verification.json','draw_clipping_state_translation.json','draw_clipping_state_connection.json','draw_clipping_state_verification.json')
 reports=[json.loads((PROJECT/'analysis'/n).read_text()) for n in names]
 for report in reports:
  for p,h in report['sha256'].items():assert digest(p)==h,p
 kernel,translation,connection,body=reports
 assert kernel['cases_passed']==12868 and kernel['canonical_physical_banks'] and kernel['complete_state_checked_after_each_operation']
 assert len(translation['routines'])==4 and translation['instruction_rows']==1007 and connection['compiled_source_count']==122
 assert body['sequence_cases_passed']==2816 and body['original_sequence_rows_covered']==98 and body['full_proposal_unexecuted_rows']==909
 assert body['complete_game_entries_verified']==[] and body['newly_recovered_game_entries']==0 and body['probe_api_guard_cases']==3
 assert body['status_counts']=={'-4':128,'2':2688}
 assert len(body['reused_helper_regressions'])==8 and all(r['full_state_match'] and r['extra_state_preserved'] and r['independent_checks']==6 for r in body['reused_helper_regressions'])
 assert body['source_sha256']==digest(PROJECT/translation['source'])
 registry=PROJECT/'analysis/recovered_routines.json';before=registry.read_bytes();registered=json.loads(before.decode('utf-8-sig'))
 # A later full-body milestone may register these entries. This earlier
 # prefix-only stage must still grant zero credit and leave the registry alone.
 integration=dict(reports={n:digest(PROJECT/'analysis'/n) for n in names},core_operation_comparisons=12868,core_stateful_sequences=64,original_sequence_comparisons=2816,original_sequence_rows_covered=98,full_proposal_static_rows=1007,full_proposal_unexecuted_rows=909,reused_helper_comparisons=8,probe_api_guard_cases=3,newly_registered_entries=0,whole_clipper_recovery_credit=0,registry_sha256=digest(registry),scope=body['scope'],next_task='Determine inactive-bank live-ins and their real initialisation where needed; verify complete clipper bodies, independent clipping geometry/packets and primitive callers. Keep exception-vector delivery/interrupt/timing limits explicit.',guide='DRAW_CLIPPING_STATE.txt',plan='CLIPPING_CPU_STATE_PLAN.txt')
 architecture=PROJECT/'analysis/sh4_integer_state_architecture_reference.json'
 integration['architecture_reference']='analysis/sh4_integer_state_architecture_reference.json';integration['architecture_reference_sha256']=digest(architecture)
 status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_clipping_state']=integration;status_path.write_text(json.dumps(status,indent=2)+'\n')
 assert registry.read_bytes()==before
 print('Recorded clipping control-state progress; zero game entries registered; registry unchanged.')
if __name__=='__main__':main()
