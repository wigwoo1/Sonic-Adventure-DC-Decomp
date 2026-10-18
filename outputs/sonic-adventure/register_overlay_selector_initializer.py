"""Register two new initializer/reset bodies and reused selector/stub integration."""
import collections,hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/overlay_selector_initializer_verification.json'
TRANSLATION='analysis/overlay_selector_initializer_translation.json'

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 report=json.loads((PROJECT/REPORT).read_text());manifest=json.loads((PROJECT/TRANSLATION).read_text())
 expected=dict(cases_passed=2528,rows_covered=154,caller_rows_covered=145,marker_rows_covered=9,reused_selector_rows_covered=88,reused_stub_rows_covered=2,conditional_edges_covered=6,call_sites_covered=11,boundary_comparisons=20616,fixture_invocations=13060,selector_invocations=2508,stub_invocations=2524,marker_invocations=2524,complete_cases=2496,partial_cases=32,newly_recovered_entries=2,reused_selector_new_credit=0,reused_stub_new_credit=0)
 for k,v in expected.items():assert report[k]==v,k
 assert report['status_counts']=={'0':2496,'-2':16,'-17':16}
 assert len(report['remaining_service_targets'])==8 and len(report['routines'])==2 and report['routines']==manifest['routines']
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] and c['independent_initializer_selection_checks']==(c['status']==0) for c in report['cases'])
 for item in (report,manifest):
  for p,h in item['sha256'].items():assert digest(p)==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/overlay_selector_initializer.dll')==report['dll_sha256']
 binary=(ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN').read_bytes()
 assert binary[0xc90d210-0xc900000:0xc90d214-0xc900000]==bytes.fromhex('0b000900')
 integration=dict(verification=REPORT,verification_sha256=digest(PROJECT/REPORT),translation=TRANSLATION,translation_sha256=digest(PROJECT/TRANSLATION),body_comparisons=2528,newly_recovered_entries=2,new_instruction_rows=154,caller_rows=145,marker_rows=9,reused_selector_new_credit=0,reused_stub_new_credit=0,remaining_service_targets=report['remaining_service_targets'],scope=report['scope'],limitations=report['limitations'],next_task='Recover object setup 0c904d22 and its recursive object-copy/table-walk dependencies 0c90451c / 0c9044e2, to replace the constructed entity object pointers before connecting the draw path.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));added=0
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0xc900000:end-0xc900000]).hexdigest()==routine['retail_span_sha256']
  entry=dict(module='AL_MAIN.BIN',load_address='0x0c900000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,native_build_script='build_overlay_selector_initializer.ps1',verifier_script='verify_overlay_selector_initializer.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete static C body covered against original full state, with native selector/empty-stub/reset integration; remaining eight callees are explicit fixtures.',comparison_level=report['scope'],limitations=report['limitations'])
  old=next((r for r in registry['routines'] if r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_overlay_selector_initializer']=integration
 for target in (0xc905548,0xc90d210):
  routine=next(r for r in registry['routines'] if r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==target)
  routine.setdefault('integration_evidence',{})['native_overlay_selector_initializer']=integration
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_overlay_selector_initializer']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} initializer/reset entries; {len(registry["routines"])} routines. Existing selector and empty stub receive zero new credit.')
if __name__=='__main__':main()
