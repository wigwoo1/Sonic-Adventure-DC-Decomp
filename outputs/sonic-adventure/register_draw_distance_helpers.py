"""Register distance/signed-sqrt bodies with qualified expanded caller proof."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_distance_helpers_verification.json';MANIFEST='analysis/draw_distance_helpers_translation.json';CALLER='analysis/overlay_draw_distance_verification.json';CONNECTION='analysis/overlay_draw_distance_connection.json'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 body,manifest,caller,connection=[json.loads((PROJECT/p).read_text()) for p in (BODY,MANIFEST,CALLER,CONNECTION)]
 for k,v in dict(cases_passed=578,rows_covered=42,conditional_edges_covered=2,boundary_comparisons=736,independent_cases=218,independent_sqrt_core_checks=2112,external_game_service_fixtures=0,newly_registered_entries=2,new_distance_instruction_rows=32,completed_existing_sqrt_rows=10,prior_limited_sqrt_credit=0).items():assert body[k]==v,k
 assert body['status_counts']=={'0':278,'-1':272,'1':16,'-3':12}
 assert body['routines']==manifest['routines'] and len(manifest['routines'])==2
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in body['cases'])
 for k,v in dict(cases_passed=408,direct_service_cases=64,candidate_rows_covered=421,candidate_unexecuted_rows=490,candidate_static_rows=911,boundary_comparisons=4442,independent_cases=388,independent_distance_prefixes=200,external_game_service_fixtures=0,newly_recovered_entries=0,reused_routine_credit=0,recovered_caller_credit=0).items():assert caller[k]==v,k
 assert caller['status_counts']=={'-2':258,'0':130,'-1':8,'-3':8,'1':4}
 assert caller['first_unconnected_targets']=={'0xc90c384':38,'0x8c6215d6':20,'0x8c60e5e4':152,'0x8c0409c0':16,'0x8c5fde20':16,'0x8c041c92':16}
 assert caller['target_invocations']['0x8c640128']==204 and caller['target_invocations']['0x8c63a9c4']==200
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in caller['cases'])
 for report in (body,manifest,caller,connection):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert digest(PROJECT/body['source'])==body['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/draw_distance_helpers.dll')==body['dll_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/overlay_draw_distance.dll')==caller['dll_sha256']
 old_sqrt=json.loads((PROJECT/'analysis/scalar_sqrt_wrapper_verification.json').read_text());assert digest(PROJECT/'source/scalar_sqrt_wrapper.c')==old_sqrt['source_sha256']
 integration=dict(body_verification=BODY,body_verification_sha256=digest(PROJECT/BODY),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),caller_verification=CALLER,caller_verification_sha256=digest(PROJECT/CALLER),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),newly_registered_entries=2,new_distance_instruction_rows=32,completed_existing_sqrt_rows=10,prior_limited_sqrt_credit=0,body_comparisons=578,independent_sqrt_core_checks=2112,caller_comparisons=408,candidate_rows_covered=421,candidate_unexecuted_rows=490,independent_distance_prefixes=200,draw_candidate_recovery_credit=0,external_game_service_fixtures=0,first_unconnected_targets=caller['first_unconnected_targets'],scope=body['scope']+' '+caller['scope'],limitations=body['limitations']+caller['limitations'],next_task='Recover newly reached motion/render helper 8c60e5e4 and its dynamic tails, plus 0c90c384, 8c0409c0/8c041c92 and 8c5fde20; connect existing 8c6215d6/vertex bus. Read-only unindexed-target CFG proposals are analysis only. Continue remaining 490 draw rows without premature candidate registration.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));added=0;binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 assert not any(r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==0xc9058a0 for r in registry['routines'])
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if old is not None:assert old['source']==body['source']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_distance_helpers.ps1',verifier_script='verify_draw_distance_helpers.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete guarded native original bodies; full reached state and independent rounded distance/rational square-root checks. Existing limited sqrt source/backend preserved; supported numerical-domain limits remain.',comparison_level=body['scope'],limitations=body['limitations'])
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_overlay_draw_distance']=integration
 reused={('1ST_READ.BIN',int(a,16)) for a in caller['target_invocations'] if int(a,16) not in (0xc9058a0,0xc905618,0x8c640128,0x8c63a9c4)}|{('AL_MAIN.BIN',0xc905618)}
 for module,address in sorted(reused):
  old=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==module and int(r['address'],16)==address)
  old.setdefault('integration_evidence',{})['native_overlay_draw_distance']=dict(integration,new_recovery_credit=0)
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_overlay_draw_distance']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} first-time distance/sqrt entries; {len(registry["routines"])} total. Prior limited sqrt/reused backend/candidate receive zero duplicate credit.')
if __name__=='__main__':main()
