"""Register four complete view dependencies; keep the actual view parent partial."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent;BODY='analysis/draw_view_setup_verification.json';CALLER='analysis/draw_view_prefix_verification.json';CONNECTION='analysis/draw_view_setup_connection.json'
ENTRIES={0x8c64f548,0x8c605006,0x8c652f66,0x8c604fd0}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 body,caller,connection=[json.loads((PROJECT/p).read_text()) for p in (BODY,CALLER,CONNECTION)]
 for report in (body,caller,connection):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert digest(ROOT/'work/sonic-adventure/build/draw_view_setup.dll')==body['dll_sha256']==caller['dll_sha256']
 assert connection['compiled_source_count']==123 and connection['reused_routine_credit']==0
 assert body['cases_passed']==len(body['cases'])==182 and body['status_counts']=={'0':167,'-1':12,'-3':3}
 assert body['rows_covered']==217 and body['conditional_edges_covered']==2
 assert len(body['routines'])==len(body['coverage_by_routine'])==4 and {int(r['address'],16) for r in body['routines']}==ENTRIES
 assert all(r['rows_covered']==r['static_rows'] and not r['missing_rows'] and not r['missing_edges'] for r in body['coverage_by_routine'])
 assert body['independent_cases']==146 and body['independent_checks']==158 and body['queue_events']==body['queue_flushes']==0
 assert caller['cases_passed']==len(caller['cases'])==8 and caller['status_counts']=={'-2':8} and caller['first_unconnected_targets']=={'0x8c652d3a':8}
 assert caller['view_parent_recovery_credit']==0 and caller['parent_unexecuted_rows']>0 and caller['external_game_service_fixtures']==body['external_game_service_fixtures']==0
 assert all(caller['target_invocations'].get(hex(a),0)>0 for a in (0x8c64f548,0x8c605006,0x8c652f66,0x8c63eeb0))
 assert body['target_invocations'].get('0x8c655620',0)>0 and body['target_invocations'].get('0x8c654e60',0)>0
 assert all(r['full_state_match'] and r['ordered_boundaries_match'] and r['queue_match'] for report in (body,caller) for r in report['cases'])
 assert digest(PROJECT/body['source'])==body['source_sha256']
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),caller_verification=CALLER,caller_verification_sha256=digest(PROJECT/CALLER),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=4,new_entry_static_rows=217,new_entry_conditional_outcomes=2,body_comparisons=182,independent_cases=146,independent_checks=158,body_ordered_boundaries=body['boundary_comparisons'],caller_comparisons=8,caller_ordered_boundaries=caller['boundary_comparisons'],caller_rows_covered=caller['parent_rows_covered'],caller_unexecuted_rows=caller['parent_unexecuted_rows'],first_unconnected_targets=caller['first_unconnected_targets'],remaining_partial_geometry_proposals=3,remaining_partial_motion_proposals=15,remaining_partial_primitive_proposals=0,view_parent_recovery_credit=0,reused_routine_credit=0,external_game_service_fixtures=0,scope=body['scope']+' '+caller['scope'],limitations=body['limitations']+caller['limitations'],guide='DRAW_VIEW_SETUP.txt',next_task='Connect existing original FR4 dispatcher wrapper 8c652d3a and register writer 8c653444 with explicit GPU-register effects; preserve the view-builder stop until actual dependencies are checked, then continue full initialization.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));data=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();added=0
 # A later whole-view proof may register the parent. This earlier eight-prefix
 # proof still grants zero parent credit and must preserve that later evidence.
 for r in body['routines']:
  start,end=int(r['address'],16),int(r['end_exclusive'],16)
  assert hashlib.sha256(data[start-0x8c010000:end-0x8c010000]).hexdigest()==r['retail_span_sha256']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=r['name'],address=r['address'],end_exclusive=r['end_exclusive'],indexed_instruction_bytes=r['instruction_bytes'],retail_span_sha256=r['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_view_setup.ps1',verifier_script='verify_draw_view_setup.py',instruction_inventory=Path(r['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete original dependency within qualified domains: every row/outcome, full state/RAM/ordered boundaries and independent reset/default/colour/slot semantics. Actual native header dependencies execute.',comparison_level=integration['scope'],limitations=integration['limitations'])
  old=next((e for e in registry['routines'] if e.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(e['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:assert old['source']==body['source'];old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_view_setup']=integration
 reused={int(a,16) for report in (body,caller) for a in report['target_invocations']}-ENTRIES;attached=0
 for address in reused:
  old=next((e for e in registry['routines'] if e.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(e['address'],16)==address),None)
  if old is not None:old.setdefault('integration_evidence',{})['native_draw_view_setup']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_view_setup']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} complete view dependencies; {len(registry["routines"])} total. {attached} reused entries linked; this earlier prefix proof retains zero view-parent recovery credit.')
if __name__=='__main__':main()
