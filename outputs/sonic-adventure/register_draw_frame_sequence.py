"""Register the complete context helpers and qualified renderer initializer."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent;CONTEXT='analysis/draw_frame_context_verification.json';SEQUENCE='analysis/draw_frame_sequence_verification.json';CONNECTION='analysis/draw_frame_context_connection.json'
ENTRIES={0x8c661300,0x8c66118a,0x8c661160,0x8c0544e2}
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 body,sequence,connection=[json.loads((PROJECT/p).read_text()) for p in (CONTEXT,SEQUENCE,CONNECTION)]
 for report in (body,sequence,connection):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert body['dll_sha256']==sequence['dll_sha256']==digest(ROOT/'work/sonic-adventure/build/draw_frame_context.dll') and connection['compiled_source_count']==128
 assert body['cases_passed']==len(body['cases'])==103 and body['status_counts']=={'0':100,'-3':3}
 assert body['rows_covered']==166 and body['conditional_edges_covered']==10 and body['independent_cases']==100 and body['independent_checks']==128
 assert all(r['rows_covered']==r['static_rows'] and not r['missing_rows'] and not r['missing_edges'] for r in body['coverage_by_routine'])
 assert sequence['sequence_cases_passed']==len(sequence['cases'])==6 and sequence['stage_comparisons']==18 and sequence['view_producer_returns']==6 and sequence['initializer_returns']==12 and sequence['warm_initializer_returns']==6
 assert sequence['rows_covered']==sequence['rows_static']==141 and sequence['conditional_edges_covered']==0 and not sequence['missing_rows'] and not sequence['missing_edges']
 assert sequence['independent_checks']==42 and sequence['independent_background_packets']==12 and sequence['register_write_events']==sequence['stored_event_prefix']==1578 and sequence['unstored_event_count']==0
 assert len(sequence['cold_missing_producer_cases'])==2 and all(r['status']==-3 and r['stop_pc']=='0x8c60548e' and r['full_state_match'] and r['ordered_boundaries_match'] and r['queue_match'] for r in sequence['cold_missing_producer_cases'])
 assert sequence['original_boot_call_order_proved'] is False and sequence['external_game_service_fixtures']==body['external_game_service_fixtures']==0
 assert all(r['full_state_match'] and r['ordered_boundaries_match'] and r['queue_match'] and r['register_map_match'] for r in body['cases'])
 assert all(c['all_state_carried'] and all(s['status']==0 and s['full_state_match'] and s['ordered_boundaries_match'] and s['register_queue_match'] for s in c['stages']) for c in sequence['cases'])
 for report in (body,sequence):assert digest(PROJECT/report['source'])==report['source_sha256']
 pairs=[(r,body,CONTEXT,'verify_draw_frame_context.py') for r in body['routines']]+[(r,sequence,SEQUENCE,'verify_draw_frame_sequence.py') for r in sequence['routines']]
 assert len(pairs)==4 and {int(r['address'],16) for r,_,_,_ in pairs}==ENTRIES
 integration=dict(context_verification=CONTEXT,context_verification_sha256=digest(PROJECT/CONTEXT),sequence_verification=SEQUENCE,sequence_verification_sha256=digest(PROJECT/SEQUENCE),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=4,new_entry_static_rows=307,new_entry_conditional_outcomes=10,context_comparisons=103,context_independent_checks=128,sequence_cases=6,stage_comparisons=18,view_producer_returns=6,initializer_returns=12,warm_initializer_returns=6,sequence_independent_checks=42,independent_background_packets=12,sequence_register_writes=1578,unstored_sequence_event_count=0,cold_missing_producer_cases=2,original_boot_call_order_proved=False,remaining_partial_geometry_proposals=1,remaining_unverified_frame_dependency_proposals=33,remaining_partial_motion_proposals=15,reused_routine_credit=0,external_game_service_fixtures=0,scope=body['scope']+' '+sequence['scope'],limitations=body['limitations']+sequence['limitations'],guide='DRAW_FRAME_SEQUENCE.txt',next_task='Recover the real boot caller 8c10b840 and its actual reached setup/loading dependencies. Prove natural boot ordering rather than treating the checked view/initializer sequence as a complete boot/frame/gameplay proof.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));data=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();added=0
 for r,report,verification,verifier in pairs:
  start,end=int(r['address'],16),int(r['end_exclusive'],16);assert hashlib.sha256(data[start-0x8c010000:end-0x8c010000]).hexdigest()==r['retail_span_sha256']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=r['name'],address=r['address'],end_exclusive=r['end_exclusive'],indexed_instruction_bytes=r['instruction_bytes'],retail_span_sha256=r['retail_span_sha256'],source=report['source'],verification=verification,native_build_script='build_draw_frame_context.ps1',verifier_script=verifier,instruction_inventory=Path(r['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete original body within qualified domains: all exported rows/outcomes and actual native dependencies. Context has independent packet/tag footprints; initializer has persistent producer and repeat-call evidence. Natural boot scheduling/GPU execution remain unproved.',comparison_level=integration['scope'],limitations=integration['limitations'])
  old=next((e for e in registry['routines'] if e.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(e['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:assert old['source']==report['source'];old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_frame_sequence']=integration
 reused={int(a,16) for a in sequence['target_invocations']}-ENTRIES;attached=0
 for a in reused:
  old=next((e for e in registry['routines'] if e.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(e['address'],16)==a),None)
  if old is not None:old.setdefault('integration_evidence',{})['native_draw_frame_sequence']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_frame_sequence']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} complete context/initializer entries; {len(registry["routines"])} total. {attached} existing entries linked at zero duplicate credit; whole boot/GPU/gameplay remain incomplete.')
if __name__=='__main__':main()
