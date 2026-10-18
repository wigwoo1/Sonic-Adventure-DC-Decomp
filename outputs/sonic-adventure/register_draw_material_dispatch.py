"""Register the complete qualified dispatcher and preserve partial outer parents."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent;BODY='analysis/draw_material_dispatch_verification.json';ENTRY=0x8c0376d0
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 report=json.loads((PROJECT/BODY).read_text())
 for p,h in report['sha256'].items():assert digest(p)==h,p
 assert report['cases_passed']==len(report['cases'])==116 and report['status_counts']=={'0':112,'-3':4}
 assert report['rows_static']==report['rows_covered']==149 and report['conditional_edges_covered']==20
 assert not report['missing_rows'] and not report['missing_edges'] and report['independent_checks']==336
 assert report['boundary_comparisons']==1022 and report['queue_flushes']==589 and report['queue_events']==report['stored_queue_event_prefix']==4988 and report['unstored_queue_event_count']==0
 assert report['external_game_service_fixtures']==report['newly_registered_entries']==0
 assert all(r['full_state_match'] and r['ordered_boundaries_match'] and r['queue_match'] for r in report['cases'])
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/draw_clipping_complete.dll')==report['dll_sha256']
 routine,=report['routines'];assert int(routine['address'],16)==ENTRY
 data=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();end=int(routine['end_exclusive'],16)
 assert hashlib.sha256(data[ENTRY-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
 assert all(report['target_invocations'].get(hex(a),0)>0 for a in (ENTRY,0x8c037990,0x8c037c3c,0x8c037ccc,0x8c038668,0x8c0382ac,0x8c037ee0,0x8c037ff4,0x8c038516,0x8c038802))
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),newly_registered_entries=1,new_entry_static_rows=149,new_entry_conditional_outcomes=20,body_comparisons=116,normal_returns=112,typed_memory_stops=4,independent_checks=336,ordered_boundaries=1022,queue_flushes=589,queue_events=4988,unstored_queue_event_count=0,remaining_partial_geometry_proposals=3,remaining_partial_motion_proposals=15,remaining_partial_primitive_proposals=0,recovered_caller_credit=0,reused_routine_credit=0,external_game_service_fixtures=0,scope=report['scope'],limitations=report['limitations'],guide='DRAW_MATERIAL_DISPATCH.txt',next_task='Recover the original view builder 8c604ba0 and its real dependencies; continue distinguishing object rendering paths, camera setup and natural startup from supplied fixtures.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'))
 entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=BODY,native_build_script='build_draw_clipping_complete.ps1',verifier_script='verify_draw_material_dispatch.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete original material-block dispatcher within qualified domains: all 149 rows/20 outcomes, real native children, independent header versus original primitive selection, strip-table variants and loop/stack/stride checks.',comparison_level=integration['scope'],limitations=integration['limitations'])
 old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==ENTRY),None);added=int(old is None)
 if old is None:registry['routines'].append(entry);old=entry
 else:assert old['source']==report['source'];old.update(entry)
 old.setdefault('integration_evidence',{})['native_draw_material_dispatch']=integration
 attached=0
 for a in report['target_invocations']:
  if int(a,16)==ENTRY:continue
  reused=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==int(a,16)),None)
  if reused is not None:reused.setdefault('integration_evidence',{})['native_draw_material_dispatch']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_material_dispatch']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} material dispatcher; {len(registry["routines"])} total; {attached} reused entries linked with zero duplicate credit.')
if __name__=='__main__':main()
