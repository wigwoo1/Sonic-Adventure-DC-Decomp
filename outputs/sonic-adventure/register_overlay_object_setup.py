"""Register bounded native object setup evidence without double-counting helpers."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/overlay_object_setup_verification.json'
TRANSLATION='analysis/overlay_object_setup_translation.json'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 report=json.loads((PROJECT/REPORT).read_text());manifest=json.loads((PROJECT/TRANSLATION).read_text())
 assert report['routines']==manifest['routines'] and len(report['routines'])==4
 assert report['newly_recovered_entries']==4 and report['reused_heap_copy_new_credit']==0
 assert report['cases_passed']==1552 and report['rows_covered']==463 and report['instruction_rows_total']==465
 assert report['status_counts']=={'0':1532,'-1':4,'-3':4,'-2':6,'-17':6}
 assert report['independent_cases']==624 and report['boundary_comparisons']==167449
 assert report['excluded_rows']==['0xc904536','0xc904538']
 assert report['conditional_edges_covered']==66 and len(report['uncovered_edges'])==2
 assert report['call_sites_covered']==52 and report['conditional_edges_total']==68
 assert len(report['allocation_failure_sites_covered'])==7
 assert len(report['remaining_service_targets'])==3
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] and c['source_preserved'] for c in report['cases'])
 for item in (manifest,report):
  for p,h in item['sha256'].items():assert digest(p)==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/overlay_object_setup.dll')==report['dll_sha256']
 integration=dict(verification=REPORT,verification_sha256=digest(PROJECT/REPORT),translation=TRANSLATION,translation_sha256=digest(PROJECT/TRANSLATION),newly_recovered_entries=4,new_instruction_rows=465,body_comparisons=report['cases_passed'],rows_covered=report['rows_covered'],uncovered_edges=report['uncovered_edges'],reused_heap_copy_new_credit=0,remaining_service_targets=report['remaining_service_targets'],scope=report['scope'],limitations=report['limitations'],next_task='Connect these object dependencies to the previously recovered initializer; recover appearance-variant helpers 0c904aea / 0c904bbc / 0c904c80 before broader object draw integration.')
 binary=(ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN').read_bytes()
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));added=0
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0xc900000:end-0xc900000]).hexdigest()==routine['retail_span_sha256']
  entry=dict(module='AL_MAIN.BIN',load_address='0x0c900000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,native_build_script='build_overlay_object_setup.ps1',verifier_script='verify_overlay_object_setup.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Static native C with original full-state comparisons for bounded object graphs, native heap/free/copy and explicit guarded/remaining-service limitations.',comparison_level=report['scope'],limitations=report['limitations'])
  old=next((r for r in registry['routines'] if r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_overlay_object_setup']=integration
 for target in (0x8c098a80,0x8c098b42,0x8c604500):
  routine=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and int(r['address'],16)==target)
  routine.setdefault('integration_evidence',{})['native_overlay_object_setup']=integration
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_overlay_object_setup']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} new object entries; {len(registry["routines"])} routines. Heap, release and byte copy receive no new credit.')
if __name__=='__main__':main()
