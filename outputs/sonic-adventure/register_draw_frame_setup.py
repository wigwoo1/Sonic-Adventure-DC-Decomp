"""Register three complete setup helpers while keeping initializer prefixes partial."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent;BODY='analysis/draw_frame_setup_verification.json';CALLER='analysis/draw_frame_prefix_verification.json';CONNECTION='analysis/draw_frame_dependencies_connection.json'
ENTRIES={0x8c638fe4,0x8c640528,0x8c6403b0}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 body,caller,connection=[json.loads((PROJECT/p).read_text()) for p in (BODY,CALLER,CONNECTION)]
 for report in (body,caller,connection):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert connection['compiled_source_count']==127
 assert digest(ROOT/'work/sonic-adventure/build/draw_frame_dependencies.dll')==body['dll_sha256']==caller['dll_sha256']
 assert body['cases_passed']==len(body['cases'])==92 and body['status_counts']=={'0':81,'-1':8,'-3':3}
 assert len(body['routines'])==len(body['coverage_by_routine'])==3 and {int(r['address'],16) for r in body['routines']}==ENTRIES
 assert body['rows_covered']==131 and all(r['rows_covered']==r['static_rows'] and not r['missing_rows'] for r in body['coverage_by_routine'])
 assert all(not r['conditional_edges'] for r in body['routines'])
 assert body['independent_cases']==62 and body['independent_checks']==174 and body['queue_events']==body['register_write_events']==0
 assert body['boundary_comparisons']==92 and caller['boundary_comparisons']==152
 assert caller['cases_passed']==len(caller['cases'])==8 and caller['status_counts']=={'-2':6,'-3':2} and caller['first_unconnected_targets']=={'0x8c661300':6}
 assert caller['parent_static_rows']==141 and caller['parent_rows_covered']==56 and caller['parent_unexecuted_rows']==85 and caller['dependency_rows_reached']==722
 assert caller['independent_checks']==12 and caller['register_write_events']==0 and caller['parent_recovery_credit']==0
 assert all(caller['target_invocations'].get(hex(a),0)>0 for a in ENTRIES)
 assert all(r['full_state_match'] and r['ordered_boundaries_match'] and r['queue_match'] and r['register_map_match'] for report in (body,caller) for r in report['cases'])
 assert body['external_game_service_fixtures']==caller['external_game_service_fixtures']==0 and digest(PROJECT/body['source'])==body['source_sha256']
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),caller_verification=CALLER,caller_verification_sha256=digest(PROJECT/CALLER),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=3,new_entry_static_rows=131,new_entry_conditional_outcomes=0,body_comparisons=92,independent_cases=62,independent_checks=174,body_ordered_boundaries=92,caller_comparisons=8,caller_ordered_boundaries=152,caller_independent_checks=12,parent_rows_covered=56,parent_unexecuted_rows=85,reached_dependency_rows=722,first_unconnected_targets=caller['first_unconnected_targets'],remaining_unverified_local_proposals=33,remaining_partial_geometry_proposals=2,remaining_partial_motion_proposals=15,remaining_partial_primitive_proposals=0,parent_recovery_credit=0,reused_routine_credit=0,external_game_service_fixtures=0,scope=body['scope']+' '+caller['scope'],limitations=body['limitations']+caller['limitations'],guide='DRAW_FRAME_SETUP.txt',next_task='Recover the actual compiler-table callback at 8c661300 reached by 8c653020. Preserve original descriptor/input records and the explicit stop until its real dependencies are connected. Complete initializer/frame/boot remain separate requirements.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));data=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();added=0
 for r in body['routines']:
  start,end=int(r['address'],16),int(r['end_exclusive'],16)
  assert hashlib.sha256(data[start-0x8c010000:end-0x8c010000]).hexdigest()==r['retail_span_sha256']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=r['name'],address=r['address'],end_exclusive=r['end_exclusive'],indexed_instruction_bytes=r['instruction_bytes'],retail_span_sha256=r['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_frame_dependencies.ps1',verifier_script='verify_draw_frame_setup.py',instruction_inventory=Path(r['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete original matrix-stack/screen/default-camera helper within qualified domains: all rows, full state/RAM and independent staged memory/floating-bank/register effects. Actual initializer prefixes separately verified; parent remains partial.',comparison_level=body['scope'],limitations=integration['limitations'])
  old=next((e for e in registry['routines'] if e.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(e['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:assert old['source']==body['source'];old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_frame_setup']=integration
 attached=0
 for a in caller['target_invocations']:
  if int(a,16) in ENTRIES:continue
  old=next((e for e in registry['routines'] if e.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(e['address'],16)==int(a,16)),None)
  if old is not None:old.setdefault('integration_evidence',{})['native_draw_frame_setup']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_frame_setup']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} complete matrix/screen/camera helpers; {len(registry["routines"])} total. {attached} reused entries linked with zero new credit; initializer parent receives no recovery credit.')
if __name__=='__main__':main()
