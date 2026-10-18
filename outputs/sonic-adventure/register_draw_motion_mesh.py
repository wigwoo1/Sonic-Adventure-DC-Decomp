"""Register distinct motion-mesh bodies with qualified original-model evidence."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_motion_mesh_verification.json';MANIFEST='analysis/draw_motion_mesh_translation.json';CALLER='analysis/overlay_draw_motion_mesh_verification.json';CONNECTION='analysis/draw_motion_mesh_connection.json'

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 body,manifest,caller,connection=[json.loads((PROJECT/p).read_text()) for p in (BODY,MANIFEST,CALLER,CONNECTION)]
 audit_path=PROJECT/'analysis/draw_motion_reuse_publication_audit.json';audit=json.loads(audit_path.read_text())
 assert audit['copies_verified']==61 and audit['all_original_guest_logic_unchanged'] and audit['newly_recovered_entries']==0
 for p,h in audit['sha256'].items():assert digest(p)==h,p
 for k,v in dict(cases_passed=130,rows_covered=278,conditional_edges_covered=0,boundary_comparisons=2145,independent_matrix_light_checks=98,queue_flushes=604,queue_events=7020,newly_recovered_entries=2,reused_routine_credit=0,external_game_service_fixtures=0).items():assert body[k]==v,k
 assert body['status_counts']=={'0':94,'-3':12,'-1':16,'1':8}
 assert body['routines']==manifest['routines'] and len(manifest['routines'])==2
 assert connection['compiled_source_count']==102 and connection['reused_stop_publication_copies']==61
 for k,v in dict(cases_passed=408,direct_service_cases=64,candidate_static_rows=911,candidate_rows_covered=461,candidate_unexecuted_rows=450,boundary_comparisons=111684,independent_cases=388,independent_distance_prefixes=200,queue_flushes=138384,queue_events=1524614,external_game_service_fixtures=0,newly_recovered_entries=0,reused_routine_credit=0,recovered_caller_credit=0).items():assert caller[k]==v,k
 assert caller['status_counts']=={'-2':274,'0':114,'-1':8,'-3':8,'1':4}
 assert caller['first_unconnected_targets']=={'0xc90c384':94,'0x8c0583a0':132,'0x8c0409c0':16,'0x8c5fde20':16,'0x8c041c92':16}
 assert caller['candidate_rows_covered']>=421 and caller['candidate_rows_covered']+caller['candidate_unexecuted_rows']==911
 assert caller['new_motion_mesh_rows_reached']==278 and caller['queue_flushes']>0
 assert not {'0x8c621574','0x8c617c34'}&set(caller['first_unconnected_targets'])
 assert caller['target_invocations']['0x8c617c34']>0 and caller['target_invocations']['0x8c621574']>0
 for report in (body,caller):assert all(c['full_state_match'] and c['ordered_boundaries_match'] and c['queue_match'] for c in report['cases'])
 for report in (body,manifest,caller,connection):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert digest(PROJECT/body['source'])==body['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/draw_motion_mesh.dll')==body['dll_sha256']==caller['dll_sha256']
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),caller_verification=CALLER,caller_verification_sha256=digest(PROJECT/CALLER),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=2,original_instruction_rows=278,body_comparisons=130,independent_matrix_light_checks=98,caller_comparisons=408,candidate_rows_covered=caller['candidate_rows_covered'],candidate_unexecuted_rows=caller['candidate_unexecuted_rows'],new_motion_mesh_rows_reached=caller['new_motion_mesh_rows_reached'],caller_queue_flushes=caller['queue_flushes'],caller_queue_events=caller['queue_events'],reused_source_reporting_copies=61,draw_candidate_recovery_credit=0,external_game_service_fixtures=0,first_unconnected_targets=caller['first_unconnected_targets'],scope=body['scope']+' '+caller['scope'],limitations=body['limitations']+caller['limitations'],next_task='Recover remaining original draw services and natural renderer/texture initialization. Retain original models/animation and explicitly qualify supplied renderer masks and populated texture-descriptor metadata. Continue unexecuted parent paths without premature registration; software packets are not GPU pixels or gameplay.')
 integration.update(reuse_publication_audit='analysis/draw_motion_reuse_publication_audit.json',reuse_publication_audit_sha256=digest(audit_path),caller_stored_queue_event_prefix=sum(min(4096,r['queue_events']) for r in caller['cases']),caller_unstored_queue_event_count=sum(max(0,r['queue_events']-4096) for r in caller['cases']))
 assert (integration['caller_stored_queue_event_prefix'],integration['caller_unstored_queue_event_count'])==(632532,892082)
 integration['next_task']='Recover newly reached 8c0583a0 (132 caller stops) and 0c90c384 (94), then remaining 8c0409c0/8c5fde20/8c041c92 and natural renderer/texture initialization. 450 parent rows remain unexecuted; do not register parent prematurely. Preserve qualified supplied metadata and software queue trace-prefix limits.'
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));added=0;binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();new={int(r['address'],16) for r in manifest['routines']}
 assert not any(r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==0xc9058a0 for r in registry['routines'])
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if old is not None:assert old['source']==body['source']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_motion_mesh.ps1',verifier_script='verify_draw_motion_mesh.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete guarded original motion mesh preprocessing/wrapper; all rows, full state and native material/queue integration within qualified valid-data/numerical domains. No natural renderer setup or GPU-pixel claim.',comparison_level=body['scope'],limitations=body['limitations'])
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_motion_mesh']=integration
 reused={int(a,16) for report in (body,caller) for a in report['target_invocations']}-new-{0xc9058a0};attached=0
 for address in sorted(reused):
  module='AL_MAIN.BIN' if address==0xc905618 else '1ST_READ.BIN'
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==module and int(r['address'],16)==address),None)
  if old is not None:old.setdefault('integration_evidence',{})['native_draw_motion_mesh']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_motion_mesh']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} first-time motion mesh entries; {len(registry["routines"])} total. Reused evidence attached to {attached} existing entries; zero duplicate/candidate credit.')
if __name__=='__main__':main()
