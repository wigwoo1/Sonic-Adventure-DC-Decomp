"""Register mixed-module morph/copy bodies and native initializer integration."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/overlay_model_blend_verification.json'
COMBINED='analysis/overlay_model_blend_initializer_verification.json'
TRANSLATION='analysis/overlay_model_blend_translation.json'
NOTES='analysis/overlay_model_blend_coverage_notes.json'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 report=json.loads((PROJECT/REPORT).read_text());combined=json.loads((PROJECT/COMBINED).read_text());manifest=json.loads((PROJECT/TRANSLATION).read_text());notes=json.loads((PROJECT/NOTES).read_text())
 expected=dict(cases_passed=1626,rows_covered=856,blend_rows_covered=798,copy_rows_covered=58,conditional_edges_covered=67,conditional_edges_total=74,boundary_comparisons=2368,independent_cases=1444,native_external_game_services=0,newly_recovered_entries=2)
 for key,value in expected.items():assert report[key]==value,key
 assert report['status_counts']=={'0':1538,'-1':72,'1':8,'-3':8}
 assert report['routines']==manifest['routines'] and len(report['routines'])==2
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] for c in report['cases'])
 assert notes['uncovered_rows']==report['uncovered_rows'] and notes['uncovered_edges']==report['uncovered_edges']
 assert notes['verification_sha256']==digest(PROJECT/REPORT) and notes['newly_recovered_entries']==0
 for key,value in dict(cases_passed=228,complete_cases=176,partial_cases=52,boundary_comparisons=117509,native_rows_covered=1668,conditional_edges_covered=114,call_sites_covered=41,independent_cases=176,newly_recovered_entries=0,reused_routine_credit=0).items():assert combined[key]==value,key
 assert combined['status_counts']=={'0':176,'-2':12,'-17':12,'-1':15,'-3':13}
 assert len(combined['remaining_service_targets'])==6
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] and c['independent_checks']==(c['status']==0) for c in combined['cases'])
 for item in (report,combined,manifest,notes):
  for p,h in item['sha256'].items():assert digest(p)==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/overlay_model_blend.dll')==report['dll_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/overlay_model_blend_initializer.dll')==combined['dll_sha256']
 integration=dict(verification=COMBINED,verification_sha256=digest(PROJECT/COMBINED),body_verification=REPORT,body_verification_sha256=digest(PROJECT/REPORT),translation=TRANSLATION,translation_sha256=digest(PROJECT/TRANSLATION),coverage_notes=NOTES,coverage_notes_sha256=digest(PROJECT/NOTES),newly_recovered_entries=2,new_instruction_rows=871,body_comparisons=1626,combined_comparisons=228,remaining_service_targets=combined['remaining_service_targets'],reused_routine_credit=0,scope=combined['scope'],limitations=combined['limitations'],next_task='Recover remaining initializer helpers 0c90bd40 / 0c90853e (including their lower setters) / 0c9048ec, then outer tail-state helpers and object draw helper 0c905666.')
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));added=0
 for routine in manifest['routines']:
  start,end,base=(int(routine[k],16) for k in ('address','end_exclusive','load_address'));module=routine['module']
  binary=(ROOT/('work/sonic-adventure/extracted/1ST_READ.BIN' if module=='1ST_READ.BIN' else 'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN')).read_bytes()
  assert hashlib.sha256(binary[start-base:end-base]).hexdigest()==routine['retail_span_sha256']
  entry=dict(module=module,load_address=routine['load_address'],name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,native_build_script='build_overlay_model_blend.ps1',verifier_script='verify_overlay_model_blend.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete static C with original guarded-state comparisons, independent dyadic geometry/material/copy checks and native shipped-object initializer integration; exact dynamic exclusions recorded.',comparison_level=report['scope'],limitations=report['limitations']+['15 morph rows / seven outcomes remain unexecuted; qualified ordinary-contract explanation in '+NOTES+'. No universal hardware or exceptional-state reachability claim.'])
  old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==module and int(r['address'],16)==start),None)
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_overlay_model_blend']=integration
 for module,targets in (('AL_MAIN.BIN',(0xc90fde4,0xc909d40,0xc905548,0xc90d210,0xc904d22,0xc90451c,0xc9044e2,0xc9044a0,0xc904aea,0xc904bbc,0xc904c80)),('1ST_READ.BIN',(0x8c098a80,0x8c098b42,0x8c604500))):
  for target in targets:
   entry=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==module and int(r['address'],16)==target)
   entry.setdefault('integration_evidence',{})['native_overlay_model_blend']=integration
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_overlay_model_blend']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} morph/copy entries; {len(registry["routines"])} routines. Reused initializer/object/selector/heap code receives no new credit.')
if __name__=='__main__':main()
