"""Register the qualified complete view builder; attach reused services at zero credit."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent;BODY='analysis/draw_view_complete_verification.json';REGISTERS='analysis/draw_view_registers_verification.json';CONNECTION='analysis/draw_view_registers_connection.json';ENTRY=0x8c604ba0
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 body,registers,connection=[json.loads((PROJECT/p).read_text()) for p in (BODY,REGISTERS,CONNECTION)]
 for report in (body,registers,connection):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert body['dll_sha256']==registers['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/draw_view_registers.dll')
 assert connection['compiled_source_count']==126 and registers['newly_registered_entries']==registers['reused_routine_credit']==0
 assert registers['cases_passed']==142 and registers['rows_covered']==19 and not registers['missing_rows'] and registers['independent_checks']==402
 assert registers['status_counts']=={'0':134,'-2':2,'-3':6}
 assert registers['bus_verification']==dict(operations_passed=18447,register_words=2048,aliases=3,events=18444,stored_event_prefix=4096,later_event_count=14348,invalid_writes_mutation_free=4,queue_flushes=1)
 assert body['cases_passed']==len(body['cases'])==28 and body['status_counts']=={'0':14,'-1':6,'-3':6,'-2':2}
 assert body['rows_static']==body['rows_covered']==166 and body['conditional_edges_covered']==6 and not body['missing_rows'] and not body['missing_edges']
 assert body['independent_checks']==70 and body['independent_copies']==28 and body['register_write_events']==16
 assert all(r['full_state_match'] and r['ordered_boundaries_match'] and r['queue_match'] and r['register_map_match'] for report in (body,registers) for r in report['cases'])
 assert all(body['target_invocations'].get(hex(a),0)>0 for a in (ENTRY,0x8c652d3a,0x8c653444,0x8c10cddc,0x8c604fd0,0x8c605006,0x8c64f548,0x8c652f66,0x8c654e60,0x8c655620,0x8c63eeb0))
 assert body['external_game_service_fixtures']==0 and digest(PROJECT/body['source'])==body['source_sha256']
 routine,=body['routines'];assert int(routine['address'],16)==ENTRY
 data=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();end=int(routine['end_exclusive'],16)
 assert hashlib.sha256(data[ENTRY-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),register_verification=REGISTERS,register_verification_sha256=digest(PROJECT/REGISTERS),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=1,new_entry_static_rows=166,new_entry_conditional_outcomes=6,body_comparisons=28,qualified_returns=14,independent_checks=70,independent_192_byte_copies=28,register_service_comparisons=142,register_service_independent_checks=402,backend_operations=18447,body_ordered_boundaries=body['boundary_comparisons'],register_service_ordered_boundaries=registers['boundary_comparisons'],body_register_writes=16,remaining_partial_geometry_proposals=2,remaining_partial_motion_proposals=15,remaining_partial_primitive_proposals=0,external_game_service_fixtures=0,reused_routine_credit=0,scope=body['scope'],limitations=body['limitations']+registers['limitations'],guide='DRAW_VIEW_COMPLETE.txt',next_task='Recover the real frame initializer 8c0544e2 and its reached dependencies, retaining explicit software register effects and distinguishing them from physical GPU execution and natural whole-engine startup.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'))
 entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_view_registers.ps1',verifier_script='verify_draw_view_complete.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete original view builder within qualified domains: every row/outcome, real native dependencies, full state/RAM/register storage and independent screen/stack/copy/write checks. Host register storage is not GPU execution.',comparison_level=body['scope'],limitations=integration['limitations'])
 old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==ENTRY),None);added=int(old is None)
 if old is None:registry['routines'].append(entry);old=entry
 else:assert old['source']==body['source'];old.update(entry)
 old.setdefault('integration_evidence',{})['native_draw_view_complete']=integration
 attached=0
 for a in body['target_invocations']:
  if int(a,16)==ENTRY:continue
  reused=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==int(a,16)),None)
  if reused is not None:reused.setdefault('integration_evidence',{})['native_draw_view_complete']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_view_complete']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} complete view builder; {len(registry["routines"])} total. {attached} services linked at zero duplicate credit. Physical GPU/gameplay and ports remain incomplete.')
if __name__=='__main__':main()
