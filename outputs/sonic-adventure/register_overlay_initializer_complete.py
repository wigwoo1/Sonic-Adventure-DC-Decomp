"""Register eight new bodies and fixture-free native constructor integration."""
import hashlib,json
from pathlib import Path
from verify_startup import ROOT
PROJECT=Path(__file__).resolve().parent
REPORT='analysis/overlay_initializer_finish_verification.json'
COMBINED='analysis/overlay_initializer_complete_verification.json'
TRANSLATION='analysis/overlay_initializer_finish_translation.json'
REUSED_SETTER=0xc908530
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 report=json.loads((PROJECT/REPORT).read_text());combined=json.loads((PROJECT/COMBINED).read_text());manifest=json.loads((PROJECT/TRANSLATION).read_text())
 for key,value in dict(cases_passed=4464,rows_covered=733,conditional_edges_covered=64,boundary_comparisons=9748,independent_cases=4364,shared_asset_byte_writes=3708,native_external_game_services=0,newly_recovered_entries=8,reused_setter_credit=0,reused_rng_credit=0).items():assert report[key]==value,key
 assert report['status_counts']=={'0':4364,'-1':56,'-3':40,'1':4}
 assert report['retranslated_existing_entry']=='0xc908530' and manifest['newly_recovered_entries']==8 and manifest['reused_setter_credit']==0
 assert report['routines']==manifest['routines'] and len(report['routines'])==9
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] and c['independent_checks']==(c['status']==0) for c in report['cases'])
 for key,value in dict(cases_passed=402,complete_cases=378,partial_cases=24,boundary_comparisons=214076,native_rows_covered=2393,conditional_edges_covered=167,call_sites_covered=45,independent_cases=378).items():assert combined[key]==value,key
 assert combined['status_counts']=={'0':378,'-1':8,'-3':12,'1':4}
 assert combined['target_invocations']['0x8c10d334']==386
 assert combined['remaining_service_targets']==[] and combined['external_game_service_fixtures']==0
 assert combined['newly_recovered_entries']==0 and combined['reused_routine_credit']==0
 assert all(c['full_state_match'] and c['ordered_boundaries_match'] and c['independent_checks']==(c['status']==0) for c in combined['cases'])
 for item in (report,combined,manifest):
  for p,h in item['sha256'].items():assert digest(p)==h,p
 assert digest(PROJECT/report['source'])==report['source_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/overlay_initializer_finish.dll')==report['dll_sha256']
 assert digest(ROOT/'work/sonic-adventure/build/overlay_initializer_complete.dll')==combined['dll_sha256']
 integration=dict(verification=COMBINED,verification_sha256=digest(PROJECT/COMBINED),body_verification=REPORT,body_verification_sha256=digest(PROJECT/REPORT),translation=TRANSLATION,translation_sha256=digest(PROJECT/TRANSLATION),newly_recovered_entries=8,new_instruction_rows=726,reused_guarded_setter_rows=7,reused_setter_credit=0,reused_rng_credit=0,body_comparisons=4464,combined_comparisons=402,remaining_service_targets=[],external_game_service_fixtures=0,scope=combined['scope'],limitations=combined['limitations'],next_task='Recover parameter clamp/update helper 0c905666 (which conditionally tail-branches to the morph), trace its 0c9009d0 caller and natural creation/scheduling, then identify the actual object draw path. Earlier draw labeling was inferred and is superseded by the exported instructions.')
 binary=(ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN').read_bytes()
 path=PROJECT/'analysis/recovered_routines.json';registry=json.loads(path.read_text(encoding='utf-8-sig'));added=0
 for routine in manifest['routines']:
  start,end=int(routine['address'],16),int(routine['end_exclusive'],16)
  assert hashlib.sha256(binary[start-0xc900000:end-0xc900000]).hexdigest()==routine['retail_span_sha256']
  old=next((r for r in registry['routines'] if r.get('module')=='AL_MAIN.BIN' and int(r['address'],16)==start),None)
  if start==REUSED_SETTER:
   assert old is not None and old['source']=='source/al_main_set_secondary_callback_fields.c'
   old.setdefault('integration_evidence',{})['native_overlay_initializer_complete']=dict(integration,guarded_alternate_source=report['source'],guarded_alternate_name=routine['name'],new_recovery_credit=0)
   continue
  entry=dict(module='AL_MAIN.BIN',load_address='0x0c900000',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],indexed_instruction_bytes=routine['instruction_bytes'],retail_span_sha256=routine['retail_span_sha256'],source=report['source'],verification=REPORT,native_build_script='build_overlay_initializer_finish.ps1',verifier_script='verify_overlay_initializer_finish.py',instruction_inventory=Path(routine['listing']).relative_to(PROJECT).as_posix(),descriptive_name_is_inferred=True,status='Complete guarded native C; all rows/outcomes matched against original full state, independent data effects and fixture-free constructor integration.',comparison_level=report['scope'],limitations=report['limitations'])
  if old is None:registry['routines'].append(entry);old=entry;added+=1
  else:old.update(entry)
  old.setdefault('integration_evidence',{})['native_overlay_initializer_complete']=integration
 for module,targets in (('AL_MAIN.BIN',(0xc90fde4,0xc909d40,0xc905548,0xc90d210,0xc904d22,0xc90451c,0xc9044e2,0xc9044a0,0xc904aea,0xc904bbc,0xc904c80,0xc904e80)),('1ST_READ.BIN',(0x8c098a80,0x8c098b42,0x8c604500,0x8c10cddc,0x8c10d334))):
  for target in targets:
   entry=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')==module and int(r['address'],16)==target)
   entry.setdefault('integration_evidence',{})['native_overlay_initializer_complete']=integration
 path.write_text(json.dumps(registry,indent=2)+'\n')
 path=PROJECT/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
 status.setdefault('verified_progress',{})['native_overlay_initializer_complete']=integration
 status['verified_progress']['recovered_buildable_routines']=len(registry['routines'])
 path.write_text(json.dumps(status,indent=2)+'\n')
 print(f'Registered {added} new entries; {len(registry["routines"])} routines. Existing setter/RNG and all reused dependencies receive zero new credit.')
if __name__=='__main__':main()
