"""Register seven complete qualified clipping/primitive bodies, never prefixes."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_clipping_complete_verification.json'
ENTRIES={0x8c037ccc,0x8c037ee0,0x8c037ff4,0x8c0382ac,0x8c038516,0x8c038668,0x8c038802}

def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def main():
 report=json.loads((PROJECT/BODY).read_text())
 for path,expected in report['sha256'].items():assert digest(path)==expected,path
 assert digest(ROOT/'work/sonic-adventure/build/draw_clipping_complete.dll')==report['dll_sha256']
 assert {int(r['address'],16) for r in report['routines']}==ENTRIES and len(report['routines'])==7
 assert report['cases_passed']==len(report['cases'])==682
 assert report['rows_covered']==report['rows_static']==1739 and report['conditional_edges_covered']==138
 assert len(report['coverage_by_routine'])==7 and {int(r['address'],16) for r in report['coverage_by_routine']}==ENTRIES
 assert all(r['rows_covered']==r['static_rows'] and r['conditional_edges_covered']==r['static_conditional_edges'] and not r['missing_rows'] and not r['missing_edges'] for r in report['coverage_by_routine'])
 assert report['status_counts']=={'0':618,'-1':32,'-3':23,'-4':6,'1':3}
 assert report['independent_cases']==554 and report['independent_checks']==13220 and report['independent_clip_invocations']==854
 assert not report['normal_bank1_live_in'] and report['external_game_service_fixtures']==0
 assert report['boundary_comparisons']==1926 and all(report['target_invocations'].get(hex(a),0)>0 for a in ENTRIES)
 assert all(r['full_state_match'] and r['ordered_boundaries_match'] and r['queue_match'] for r in report['cases'])
 assert report['queue_events']==report['stored_queue_event_prefix']==31369 and report['unstored_queue_event_count']==0
 assert report['queue_flushes']==3469 and report['new_dll_original_initializer_stages']==1 and report['new_dll_original_initializer_rows']==41
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),newly_registered_entries=7,new_entry_static_rows=1739,new_entry_conditional_outcomes=138,body_comparisons=682,independent_cases=554,independent_checks=13220,independent_clip_invocations=854,ordered_boundary_comparisons=1926,queue_flushes=3469,queue_events=31369,unstored_queue_event_count=0,checked_current_dll_initializer_stages=1,checked_current_dll_initializer_rows=41,observed_initial_inactive_bank_live_ins=[],inactive_bank_liveness_is_dynamic=True,compiled_source_count=122,remaining_partial_primitive_proposals=0,remaining_partial_motion_proposals=15,remaining_partial_geometry_proposals=4,draw_candidate_recovery_credit=0,reused_routine_credit=0,external_game_service_fixtures=0,scope=report['scope'],limitations=report['limitations'],guide='DRAW_CLIPPING_COMPLETE.txt',next_task='Broaden actual original object/model/camera callers through the completed clipping family and recover remaining renderer/geometry dependencies; prove natural startup/frame behavior without substituting game services.')
 registry_path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(registry_path.read_text(encoding='utf-8-sig'))
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();added=0
 for r in report['routines']:
  start,end=int(r['address'],16),int(r['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()==r['retail_span_sha256']
  assert digest(PROJECT/r['source'])==r['source_sha256']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=r['name'],address=r['address'],end_exclusive=r['end_exclusive'],indexed_instruction_bytes=r['instruction_bytes'],retail_span_sha256=r['retail_span_sha256'],source=r['source'],verification=BODY,native_build_script='build_draw_clipping_complete.ps1',verifier_script='verify_draw_clipping_complete.py',instruction_inventory=Path(r['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete original body within qualified processor/numerical domains: all exported rows and conditional outcomes, full machine/RAM/ordered queue state and independent geometry/packet checks. Shared 8c037ff4 uses the actual tail-continuation frame.',comparison_level=integration['scope'],limitations=integration['limitations'])
  old=next((e for e in registry['routines'] if e.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(e['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:assert old['source']==r['source'];old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_clipping_complete']=integration
 init=next(e for e in registry['routines'] if e.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(e['address'],16)==0x8c036f60)
 init.setdefault('integration_evidence',{})['native_draw_clipping_complete_initializer']=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),new_recovery_credit=0,checked_current_dll_initializer_stages=1,checked_current_dll_initializer_rows=41,scope='One full original/native setup stage on the current 122-source DLL, separately counted from the 682 clipping/parent cases.')
 registry_path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_draw_clipping_complete']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} clipping/primitive entries; {len(registry["routines"])} total. Full game/startup/GPU and ports remain incomplete.')

if __name__=='__main__':main()
