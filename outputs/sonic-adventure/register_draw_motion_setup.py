"""Register seven qualified motion/base bodies; no duplicate caller credit."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_motion_setup_verification.json';MANIFEST='analysis/draw_motion_setup_translation.json';CALLER='analysis/overlay_draw_motion_verification.json';CONNECTION='analysis/draw_motion_setup_connection.json'

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 body,manifest,caller,connection=[json.loads((PROJECT/p).read_text()) for p in (BODY,MANIFEST,CALLER,CONNECTION)]
 for k,v in dict(cases_passed=544,rows_covered=286,conditional_edges_covered=30,boundary_comparisons=4688,independent_cases=506,independent_world_matrix_checks=472,newly_recovered_entries=7,reused_routine_credit=0,external_game_service_fixtures=0).items():assert body[k]==v,k
 assert body['status_counts']=={'0':484,'-2':26,'-1':20,'-3':6,'1':8}
 assert body['routines']==manifest['routines'] and len(manifest['routines'])==7 and connection['compiled_source_count']==33
 for k,v in dict(cases_passed=408,direct_service_cases=64,candidate_rows_covered=421,candidate_unexecuted_rows=490,candidate_static_rows=911,boundary_comparisons=5050,independent_cases=388,independent_distance_prefixes=200,new_motion_rows_reached=104,external_game_service_fixtures=0,newly_recovered_entries=0,reused_routine_credit=0,recovered_caller_credit=0).items():assert caller[k]==v,k
 assert caller['status_counts']=={'-2':258,'0':130,'-1':8,'-3':8,'1':4}
 assert caller['first_unconnected_targets']=={'0xc90c384':38,'0x8c6215d6':20,'0x8c60d094':152,'0x8c0409c0':16,'0x8c5fde20':16,'0x8c041c92':16}
 assert caller['target_invocations']['0x8c60e5e4']==152 and caller['target_invocations']['0x8c60e31c']==152
 for report in (body,caller):assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in report['cases'])
 for report in (body,manifest,caller,connection):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert digest(PROJECT/body['source'])==body['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/draw_motion_setup.dll')==body['dll_sha256']==caller['dll_sha256']
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),caller_verification=CALLER,caller_verification_sha256=digest(PROJECT/CALLER),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=7,original_instruction_rows=286,body_comparisons=544,independent_world_matrix_checks=472,caller_comparisons=408,candidate_rows_covered=421,candidate_unexecuted_rows=490,new_motion_rows_reached=104,draw_candidate_recovery_credit=0,external_game_service_fixtures=0,first_unconnected_targets=caller['first_unconnected_targets'],scope=body['scope']+' '+caller['scope'],limitations=body['limitations']+caller['limitations'],next_task='Recover format-two callback 8c60d094 and its motion interpolation chain. Then recover distinct motion mesh wrapper 8c621574 / renderer 8c617c34 without substituting static mesh 8c6209a8. Other draw dependencies and 490 unexecuted parent rows remain; do not register parent prematurely.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));added=0;binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();new={int(r['address'],16) for r in manifest['routines']}
 assert not any(r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==0xc9058a0 for r in registry['routines'])
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if old is not None:assert old['source']==body['source']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_motion_setup.ps1',verifier_script='verify_draw_motion_setup.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete guarded motion/control/base-transform body; full original row/outcome coverage within documented support. Interpolation/nonzero formats and motion mesh drawing remain lower boundaries.',comparison_level=body['scope'],limitations=body['limitations'])
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_motion_setup']=integration
 # Only already registered reused entries receive attached integration evidence.
 # Unregistered legacy combined rotations get no new entry/credit here.
 reused={int(a,16) for report in (body,caller) for a in report['target_invocations']}-new-{0xc9058a0}
 attached=0
 for address in sorted(reused):
  module='AL_MAIN.BIN' if address==0xc905618 else '1ST_READ.BIN'
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==module and int(r['address'],16)==address),None)
  if old is not None:old.setdefault('integration_evidence',{})['native_draw_motion_setup']=dict(integration,new_recovery_credit=0);attached+=1
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_motion_setup']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} first-time motion/base entries; {len(registry["routines"])} total. Reused evidence attached to {attached} existing entries; zero duplicate/candidate credit.')
if __name__=='__main__':main()
