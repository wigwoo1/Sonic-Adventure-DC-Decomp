"""Register three material bodies and the combined initializer integration."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/overlay_object_variants_verification.json'
COMBINED='analysis/overlay_object_initializer_verification.json'
TRANSLATION='analysis/overlay_object_variants_translation.json'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 report=json.loads((PROJECT/REPORT).read_text());combined=json.loads((PROJECT/COMBINED).read_text());manifest=json.loads((PROJECT/TRANSLATION).read_text())
 for key,value in dict(cases_passed=2177,rows_covered=267,conditional_edges_covered=26,call_sites_covered=0,external_services=0,materials_updated=47438,newly_recovered_entries=3).items():assert report[key]==value,key
 assert report['routines']==manifest['routines'] and len(report['routines'])==3
 assert all(c['full_state_match'] and c['independent_materials_match'] for c in report['cases'])
 assert combined['cases_passed']==432 and combined['complete_cases']==368 and combined['partial_cases']==64
 assert combined['status_counts']=={'0':368,'-2':14,'-17':14,'-1':4,'-3':32}
 assert combined['target_invocations']['0x8c098b42']==8
 assert combined['independent_cases']==368 and len(combined['remaining_service_targets'])==7
 assert combined['newly_recovered_entries']==0 and combined['reused_routine_credit']==0
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] and c['independent_checks']==(c['status']==0) for c in combined['cases'])
 for item in (report,combined,manifest):
  for p,h in item['sha256'].items():assert digest(p)==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 dll=ROOT/'work/sonic-adventure/build/overlay_object_initializer.dll'
 assert digest(dll)==report['dll_sha256']==combined['dll_sha256']
 integration=dict(verification=COMBINED,verification_sha256=digest(PROJECT/COMBINED),material_verification=REPORT,material_verification_sha256=digest(PROJECT/REPORT),translation=TRANSLATION,translation_sha256=digest(PROJECT/TRANSLATION),newly_recovered_entries=3,new_instruction_rows=267,body_comparisons=2177,combined_comparisons=432,remaining_service_targets=combined['remaining_service_targets'],reused_routine_credit=0,scope=combined['scope'],limitations=combined['limitations'],next_task='Recover remaining initializer helpers, starting with initializer dependency 0c904e80, then connect object draw helper 0c905666 with the existing native renderer.')
 binary=(ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN').read_bytes()
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));added=0
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0xc900000:end-0xc900000]).hexdigest()==routine['retail_span_sha256']
  entry=dict(module='AL_MAIN.BIN',load_address='0x0c900000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,native_build_script='build_overlay_object_initializer.ps1',verifier_script='verify_overlay_object_variants.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete integer-only static C body; all rows/outcomes compared against original state and independently checked material updates. Native in combined initializer/object setup.',comparison_level=report['scope'],limitations=report['limitations'])
  old=next((r for r in registry['routines'] if r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_overlay_object_initializer']=integration
 for module,targets in (('AL_MAIN.BIN',(0xc90fde4,0xc909d40,0xc905548,0xc90d210,0xc904d22,0xc90451c,0xc9044e2,0xc9044a0)),('1ST_READ.BIN',(0x8c098a80,0x8c098b42,0x8c604500))):
  for target in targets:
   entry=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==module and int(r['address'],16)==target)
   entry.setdefault('integration_evidence',{})['native_overlay_object_initializer']=integration
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_overlay_object_initializer']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} material bodies; {len(registry["routines"])} routines. Existing object/initializer/heap/selector bodies receive no new credit.')
if __name__=='__main__':main()
