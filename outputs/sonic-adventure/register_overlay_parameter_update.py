"""Register two new bodies, preserving all reused initializer/morph evidence."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/overlay_parameter_update_verification.json'
LIFECYCLE='analysis/overlay_parameter_lifecycle_verification.json'
TRANSLATION='analysis/overlay_parameter_update_translation.json'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 report=json.loads((PROJECT/REPORT).read_text());life=json.loads((PROJECT/LIFECYCLE).read_text());manifest=json.loads((PROJECT/TRANSLATION).read_text())
 for k,v in dict(cases_passed=1000,rows_covered=275,conditional_edges_covered=44,boundary_comparisons=2444,independent_cases=902,external_game_service_fixtures=0,newly_recovered_entries=2,reused_morph_copy_credit=0).items():assert report[k]==v,k
 assert report['status_counts']=={'0':928,'-1':48,'1':16,'-3':8}
 assert report['routines']==manifest['routines'] and len(manifest['routines'])==2
 for k,v in dict(cases_passed=32,constructor_comparisons=32,update_stages=512,independent_stages=512,boundary_comparisons=896,morph_calls=192,external_game_service_fixtures=0,newly_recovered_entries=0,reused_routine_credit=0).items():assert life[k]==v,k
 assert life['status_counts']=={'0':512}
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in report['cases'])
 assert all(c['constructor_full_state_match'] and len(c['updates'])==16 and all(s['full_state_match'] and s['ordered_boundaries_match'] and s['independent_checks'] for s in c['updates']) for c in life['cases'])
 for item in (report,life,manifest):
  for p,h in item['sha256'].items():assert digest(p)==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/overlay_parameter_update.dll')==report['dll_sha256']==life['dll_sha256']
 integration=dict(verification=LIFECYCLE,verification_sha256=digest(PROJECT/LIFECYCLE),body_verification=REPORT,body_verification_sha256=digest(PROJECT/REPORT),translation=TRANSLATION,translation_sha256=digest(PROJECT/TRANSLATION),newly_recovered_entries=2,instruction_rows=275,focused_comparisons=1000,original_model_lifecycles=32,update_stages=512,independent_stages=512,boundary_comparisons=life['boundary_comparisons'],external_game_service_fixtures=0,reused_morph_copy_credit=0,scope=life['scope'],limitations=life['limitations'],next_task='Recover the 0c9058a0 draw callback and remaining matrix/render dependencies, plus task update 0c9009d0/natural scheduling. The known bounded primitive families are already native; these two new bodies update parameters and scale, without drawing pixels.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));binary=(ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN').read_bytes();added=0
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0xc900000:end-0xc900000]).hexdigest()==routine['retail_span_sha256']
  old=next((r for r in registry['routines'] if r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==start),None)
  entry=dict(module='AL_MAIN.BIN',load_address='0x0c900000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,native_build_script='build_overlay_parameter_update.ps1',verifier_script='verify_overlay_parameter_update.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete guarded native C; all rows/outcomes tested against original full state. Independent narrow data checks and repeated original-model initialization/update lifecycles.',comparison_level=report['scope'],limitations=report['limitations'])
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_overlay_parameter_lifecycle']=integration
 for module,addresses in (('AL_MAIN.BIN',(0xc90fde4,0xc904e80)),('1ST_READ.BIN',(0x8c10cddc,))):
  for address in addresses:
   entry=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==module and int(r['address'],16)==address)
   entry.setdefault('integration_evidence',{})['native_overlay_parameter_lifecycle']=dict(integration,new_recovery_credit=0)
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'));status.setdefault('verified_progress',{})['native_overlay_parameter_lifecycle']=integration;status['verified_progress']['recovered_buildable_routines']=len(registry['routines']);path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} new entries; {len(registry["routines"])} routines. Existing initializer/morph/copy credit: zero.')
if __name__=='__main__':main()
