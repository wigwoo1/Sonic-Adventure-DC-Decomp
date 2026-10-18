"""Track ARM native recovery separately from the existing SH-4 registry."""
import hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure'
verification='analysis/arm_sound_boot_native_verification.json'
guards_path='analysis/arm_sound_boot_native_guards_verification.json'
report=json.loads((project/verification).read_text());guards=json.loads((project/guards_path).read_text())
reset_path='analysis/arm_sound_reset_services_verification.json'
reset=json.loads((project/reset_path).read_text())
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
assert report['leaf_cases_passed']==192 and report['boot_cases_passed']==4 and report['loop_cases_passed']==16
assert len(report['leaf_entries'])==12
assert (guards['atomic_guards_passed'],guards['instruction_budget_cases_passed'],guards['mmio_failure_cases_passed'])==(9,24,8)
assert reset['cases_passed']==336 and all(c['fully_native'] and c['service_calls']==0 for c in report['boot_cases'])
assert report['reference_version']=='2.1.4' and report['reference_model']=='UC_CPU_ARM_TI925T'
for path,key in [(ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll','dll_sha256'),
 (project/'source/sa_arm_sound_boot_native.c','source_sha256'),
 (project/'source/sa_arm_sound_reset_services.c','reset_services_source_sha256'),
 (project/'analysis/arm_sound_reset_services_translation.json','reset_translation_sha256'),
 (project/'source/sa_arm_sound_commands.c','commands_source_sha256'),
 (project/'analysis/arm_sound_commands_translation.json','commands_translation_sha256'),
 (project/'source/sa_arm_sound_bank_select.c','bank_select_source_sha256'),
 (project/'source/arm_sound_memory.h','memory_header_sha256'),
 (project/'source/sa_arm_sound_controls.c','controls_source_sha256'),
 (project/'analysis/arm_sound_controls_translation.json','controls_translation_sha256'),
 (project/'source/sa_arm_sound_notes.c','notes_source_sha256'),
 (project/'source/sa_arm_sound_status.c','status_source_sha256'),
 (project/'source/sa_arm_sound_update.c','update_source_sha256'),
 (project/'source/sa_arm_sound_snapshot.c','snapshot_source_sha256'),
 (project/'source/sa_arm_sound_ramps.c','ramps_source_sha256'),
 (project/'source/sa_arm_sound_effects.c','effects_source_sha256'),
 (project/'source/sa_arm_sound_stream_update.c','stream_source_sha256'),
 (project/'source/sa_arm_sound_sequencer.c','sequence_source_sha256'),
 (project/'analysis/arm_sound_sequencer_translation.json','sequence_translation_sha256'),
 (project/'analysis/arm_sound_stream_update_translation.json','stream_translation_sha256'),
 (project/'analysis/arm_sound_effects_translation.json','effects_translation_sha256'),
 (project/'analysis/arm_sound_ramps_translation.json','ramps_translation_sha256'),
 (project/'analysis/arm_sound_snapshot_translation.json','snapshot_translation_sha256'),
 (project/'analysis/arm_sound_update_translation.json','update_translation_sha256'),
 (project/'analysis/arm_sound_status_translation.json','status_translation_sha256'),
 (project/'analysis/arm_sound_notes_translation.json','notes_translation_sha256'),
 (project/'build_arm_sound_boot_native.ps1','builder_sha256'),
 (project/'verify_arm_sound_boot_native.py','verifier_sha256'),
 (project/'analysis/arm_sound_driver_inventory_verification.json','inventory_evidence_sha256'),
 (project/'analysis/arm_sound_driver_payloads.json','payload_evidence_sha256'),
 (project/'analysis/arm_sound_boot_static_translation.json','translation_metadata_sha256')]:assert sha(path)==report[key],path
for path,value in report['reference_files'].items():assert sha(ROOT/path)==value,path
assert guards['dll_sha256']==report['dll_sha256'] and guards['source_sha256']==report['source_sha256']
assert guards['verifier_sha256']==sha(project/'verify_arm_sound_boot_native_guards.py')
assert guards['native_evidence_sha256']==sha(project/verification)
assert reset['dll_sha256']==report['dll_sha256']
assert reset['source_sha256']==report['reset_services_source_sha256']==guards['reset_services_source_sha256']
assert reset['translation_sha256']==report['reset_translation_sha256']
assert reset['verifier_sha256']==sha(project/'verify_arm_sound_reset_services.py')
assert reset['boot_evidence_sha256']==sha(project/verification)
payloads=json.loads((project/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
registry={'architecture':'ARM:LE:32:v4','routines':[
 {'module':driver['path'],'address':entry,'source':'source/sa_arm_sound_boot_native.c',
  'verification':verification,'classification':'Fully native leaf helper compared with original ARM execution',
  'limitations':'Active SVC/ARM register state, modeled MMIO; finite input fixtures, no physical sound-chip timing proof'}
 for driver in payloads for entry in report['leaf_entries']],
 'connected_flows':{'verification':verification,'fully_native_reset_cases':4,'main_loop_cases':16,
  'limitations':'Reset is native; remaining main-loop callees use original ARM execution in the verification harness. Not complete native driver recovery.'}}
registry['routines'] += [
 {'module':payloads[driver]['path'],'address':entry,'source':'source/sa_arm_sound_reset_services.c',
  'verification':reset_path,'classification':'Complete native ARM reset routine/dependency compared with original execution',
  'limitations':'Active SVC/ARM registers and modeled MMIO; finite fixtures, no physical sound-chip timing proof'}
 for driver,entries in enumerate(reset['entries_by_driver']) for entry in entries]
(project/'analysis/arm_sound_driver_recovered_routines.json').write_text(json.dumps(registry,indent=2)+'\n')
status_path=project/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['arm_sound_boot_native']={
 'verification':verification,'guard_verification':guards_path,'reset_services_verification':reset_path,'module_local_native_entries':38,
 'helper_cases':528,'fully_native_reset_cases':4,'main_loop_cases':16,'guard_cases':41,
 'classification':'Native ARM leaf and control-flow recovery; separate ARM registry, no whole-project milestone credit',
 'limitations':report['limitations']}
status_path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded 38 module-local native ARM routines, fully native reset, main-loop flows and 41 guard comparisons')
