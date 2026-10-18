"""Register three helpers and qualified partial draw-entry evidence only."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
BODY='analysis/draw_predicate_support_verification.json';MANIFEST='analysis/draw_predicate_support_translation.json';GATE='analysis/overlay_draw_gate_verification.json';CANDIDATE='analysis/overlay_draw_candidate_translation.json'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 body,manifest,gate,candidate=[json.loads((PROJECT/p).read_text()) for p in (BODY,MANIFEST,GATE,CANDIDATE)]
 for k,v in dict(cases_passed=676,rows_covered=95,conditional_edges_covered=14,independent_cases=516,boundary_comparisons=740,newly_recovered_entries=3,external_game_service_fixtures=0).items():assert body[k]==v,k
 assert body['status_counts']=={'0':548,'1':16,'-1':48,'-3':64}
 assert body['routines']==manifest['routines'] and len(manifest['routines'])==3
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in body['cases'])
 for k,v in dict(cases_passed=100,candidate_rows_covered=86,candidate_static_rows=911,candidate_unexecuted_rows=825,boundary_comparisons=290,independent_cases=80,newly_recovered_entries=0,recovered_caller_credit=0,external_game_service_fixtures=0).items():assert gate[k]==v,k
 assert gate['status_counts']=={'-2':18,'0':66,'-1':8,'-3':4,'1':4}
 assert gate['first_unconnected_target']=='0x8c056990' and candidate['registration_credit']==0
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in gate['cases'])
 for report in (body,manifest,gate,candidate):
  for p,h in report['sha256'].items():assert digest(p)==h,p
 assert digest(PROJECT/body['source'])==body['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/draw_predicate_support.dll')==body['dll_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/overlay_draw_gate.dll')==gate['dll_sha256']
 integration=dict(verification=BODY,verification_sha256=digest(PROJECT/BODY),translation=MANIFEST,translation_sha256=digest(PROJECT/MANIFEST),gate_verification=GATE,gate_verification_sha256=digest(PROJECT/GATE),candidate_translation=CANDIDATE,candidate_translation_sha256=digest(PROJECT/CANDIDATE),newly_recovered_entries=3,helper_instruction_rows=95,helper_comparisons=676,independent_helper_cases=516,draw_gate_cases=100,draw_candidate_rows_covered=86,draw_candidate_unexecuted_rows=825,draw_candidate_recovery_credit=0,first_unconnected_target='0x8c056990',external_game_service_fixtures=0,scope=body['scope']+' '+gate['scope'],limitations=body['limitations']+gate['limitations'],next_task='Wire the existing render-state service 8c056990 into the partial draw bridge, then other recovered callees and remaining unregistered draw services. Complete the 825 unexecuted candidate rows; do not register 0c9058a0 prematurely.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();added=0
 assert not any(r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==0xc9058a0 for r in registry['routines'])
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest()==routine['retail_span_sha256']
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if old is not None:assert old['source']==body['source']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=body['source'],verification=BODY,native_build_script='build_draw_predicate_support.ps1',verifier_script='verify_draw_predicate_support.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete guarded static C; all helper rows/outcomes compared against original full state, independent bounded predicate/copy checks. Caller integration remains partial.',comparison_level=body['scope'],limitations=body['limitations'])
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_draw_predicate_support']=integration
 spring=next(r for r in registry['routines'] if r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==0xc905618)
 spring.setdefault('integration_evidence',{})['native_draw_gate']=dict(integration,new_recovery_credit=0)
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_draw_predicate_support']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} helper entries; {len(registry["routines"])} total. Partial 911-row draw candidate remains unregistered / zero recovery credit.')
if __name__=='__main__':main()
