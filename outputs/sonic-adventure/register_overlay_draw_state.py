"""Record expanded partial caller integration; never register the candidate."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/overlay_draw_state_verification.json';CONNECTION='analysis/overlay_draw_state_connection.json'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 report=json.loads((PROJECT/REPORT).read_text());connection=json.loads((PROJECT/CONNECTION).read_text())
 for k,v in dict(cases_passed=244,direct_service_cases=64,candidate_rows_covered=273,candidate_unexecuted_rows=638,candidate_static_rows=911,boundary_comparisons=1506,independent_cases=224,external_game_service_fixtures=0,newly_recovered_entries=0,reused_routine_credit=0,recovered_caller_credit=0).items():assert report[k]==v,k
 assert report['status_counts']=={'-2':98,'0':130,'-1':8,'-3':4,'1':4}
 assert report['first_unconnected_targets']=={'0xc90c384':38,'0x8c6215d6':20,'0x8c640128':40}
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in report['cases'])
 assert len(connection['reused_service_targets'])==14 and connection['reused_service_rows']==411 and connection['compiled_source_count']==25
 assert connection['newly_recovered_entries']==connection['reused_routine_credit']==0
 assert all(report['target_invocations'].get(a,0)>0 for a in connection['reused_service_targets'])
 for item in (report,connection):
  for p,h in item['sha256'].items():assert digest(p)==h,p
 assert digest(ROOT/'work/sonic-adventure/build/overlay_draw_state.dll')==report['dll_sha256']
 integration=dict(verification=REPORT,verification_sha256=digest(PROJECT/REPORT),connection=CONNECTION,connection_sha256=digest(PROJECT/CONNECTION),cases_passed=244,draw_caller_cases=180,direct_service_cases=64,independent_cases=224,candidate_rows_covered=273,candidate_unexecuted_rows=638,boundary_comparisons=1506,first_unconnected_targets=report['first_unconnected_targets'],newly_recovered_entries=0,reused_routine_credit=0,draw_candidate_recovery_credit=0,external_game_service_fixtures=0,scope=report['scope'],limitations=report['limitations'],next_task='Recover distance helper 8c640128 and square-root tail 8c63a9c4; recover AL_MAIN helper 0c90c384 and connect existing particle wrapper 8c6215d6. Continue untested draw paths; do not register the 911-row candidate before adequate full-path verification.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));before=len(registry['routines']);updated=0
 assert not any(r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==0xc9058a0 for r in registry['routines'])
 targets={('1ST_READ.BIN',int(a,16)) for a in connection['reused_service_targets']}|{('1ST_READ.BIN',a) for a in (0x8c038d00,0x8c043a20,0x8c60edf0)}|{('AL_MAIN.BIN',0xc905618)}
 for module,address in sorted(targets):
  routine=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==module and int(r['address'],16)==address)
  old=json.loads((PROJECT/routine['verification']).read_text());source=PROJECT/routine['source']
  expected=old.get('source_sha256') or old.get('dependency_source_sha256',{}).get(source.name)
  if not expected:
   scoped={r.get('source_sha256') for r in old.get('routines',[]) if r.get('source')==routine['source'] and r.get('source_sha256')}
   if len(scoped)==1:expected=scoped.pop()
  assert expected and digest(source)==expected,(routine['name'],'prior canonical source hash')
  routine.setdefault('integration_evidence',{})['native_overlay_draw_state']=integration;updated+=1
 assert len(registry['routines'])==before and updated==18
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_overlay_draw_state']=integration;status['verified_progress']['recovered_buildable_routines']=before;path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered partial integration evidence on {updated} existing routines; {before} total. Zero new entries; candidate remains unregistered.')
if __name__=='__main__':main()
