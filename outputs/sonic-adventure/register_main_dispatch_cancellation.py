"""Register verified cancellation/restore and their native connected chain."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/main_dispatch_cancellation_verification.json'
TRANSLATION='analysis/main_dispatch_cancellation_translation.json'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def main():
 report=json.loads((PROJECT/REPORT).read_text());translation=json.loads((PROJECT/TRANSLATION).read_text())
 assert report['cases_passed']==4284
 assert report['counts']==dict(notify=678,create=168,ready=72,gate=870,spawn=512,set=504,cancel=648,restore=832)
 for key,value in dict(rows_covered=209,conditional_edges_covered=12,call_sites_covered=3,
                       boundary_comparisons=30484,partial_guard_error_cases=387,
                       independent_invariants=3761,completed_without_external_calls=3897,
                       external_game_service_calls=0).items():assert report[key]==value,key
 assert report['routines']==translation['routines']
 assert len(report['cases'])==4284 and all(c['full_state_match'] and c['external_calls']==[] for c in report['cases'])
 for item in (report,translation):
  for p,h in item['sha256'].items():assert digest(Path(p))==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/main_dispatch_cancellation.dll')==report['dll_sha256']
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 assert hashlib.sha256(binary).hexdigest()==report['binary_sha256']
 registry_path=PROJECT/'analysis/recovered_routines.json'
 registry=json.loads(registry_path.read_text(encoding='utf-8-sig'));added=0
 for routine in translation['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0x8C010000:end-0x8C010000]).hexdigest()==routine['retail_span_sha256']
  entry=dict(module='1ST_READ.BIN',load_address='0x8c010000',name=routine['name'],address=routine['address'],
             end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],
             retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,
             native_build_script='build_main_dispatch_cancellation.ps1',verifier_script='verify_main_dispatch_cancellation.py',
             instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,
             status='All original rows, branches and call/tail-call sites covered by bounded native comparisons and independent state/table/task invariants.',
             comparison_level=report['scope'],limitations=report['limitations'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);added+=1
  else:old.update(entry)
 record=dict(verification=REPORT,verification_sha256=digest(PROJECT/REPORT),translation=TRANSLATION,
             translation_sha256=digest(PROJECT/TRANSLATION),cases_passed=4284,completed_without_external_calls=3897,
             native_dependencies=['0x8c01ae60','0x8c01aae0'],remaining_tested_prefix_services=[],
             deferred_stored_callbacks=['0x8c01b358','0x8c044680','dynamic values from 0x8c19c32c'],
             scope=report['scope'],limitations=report['limitations'])
 for address in (0x8C0AFB10,0x8C07B320,0x8C07B2B8,0x8C109FDE,0x8C109E50,0x8C64BED8,
                 0x8C09852E,0x8C0985EC,0x8C09865C,0x8C098A80,0x8C098B0C,0x8C098B42):
  parent=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==address)
  parent.setdefault('integration_evidence',{})['native_cancellation_and_context_restore']=record
 registry_path.write_text(json.dumps(registry,indent=2)+'\n')
 status_path=PROJECT/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_cancellation_and_context_restore']=record
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 status_path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered cancellation and restore: {added} new original routines; {len(registry["routines"])} entries. Tested prefix has no external game-service fixture calls; stored callbacks remain unfinished.')


if __name__=='__main__':main()
