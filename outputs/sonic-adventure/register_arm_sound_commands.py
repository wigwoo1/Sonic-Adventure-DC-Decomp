"""Record native command paths without treating partial dispatch as a full routine."""
import csv,hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure'
verification='analysis/arm_sound_commands_verification.json'
guard_path='analysis/arm_sound_commands_guards_verification.json'
report=json.loads((project/verification).read_text());guards=json.loads((project/guard_path).read_text())
boot_path=project/'analysis/arm_sound_boot_native_verification.json';boot=json.loads(boot_path.read_text())
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
assert [report[k] for k in ('command_cases_passed','queue_reader_cases_passed','batch_cases_passed','frontier_cases_passed','pitch_helper_cases_passed')]==[720,144,96,0,48]
assert guards['instruction_budget_cases_passed']==22 and guards['mmio_failure_cases_passed']==4
assert report['dll_sha256']==boot['dll_sha256']==guards['dll_sha256']==sha(ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll')
assert report['source_sha256']==boot['commands_source_sha256']==guards['source_sha256']==sha(project/'source/sa_arm_sound_commands.c')
assert report['translation_sha256']==boot['commands_translation_sha256']==sha(project/'analysis/arm_sound_commands_translation.json')
assert report['boot_evidence_sha256']==sha(boot_path)
assert report['verifier_sha256']==sha(project/'verify_arm_sound_commands.py')
assert guards['verifier_sha256']==sha(project/'verify_arm_sound_commands_guards.py')
assert guards['commands_evidence_sha256']==sha(project/verification)
translation=json.loads((project/'analysis/arm_sound_commands_translation.json').read_text())['drivers']
payloads=json.loads((project/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
path=project/'analysis/arm_sound_driver_recovered_routines.json';registry=json.loads(path.read_text())
# Idempotent registration, preserving independently verified boot/reset entries.
registry['routines']=[r for r in registry['routines'] if r['source']!='source/sa_arm_sound_commands.c']
assert len(registry['routines'])==38
for driver,data in enumerate(translation):
    directory=project/'analysis/arm-sound-drivers'/(payloads[driver]['path'].split('/')[-1]+'.arm.bin')
    with (directory/'function_index.tsv').open(newline='') as stream:entries={int(r['address'],16) for r in csv.DictReader(stream,delimiter='\t')}
    for key in ('queue_reader','pitch_helper'):
        assert int(data[key],16) in entries
        registry['routines'].append({'module':payloads[driver]['path'],'address':data[key],
          'source':'source/sa_arm_sound_commands.c','verification':verification,
          'classification':'Complete native ARM queue reader' if key=='queue_reader' else 'Complete native ARM pitch helper',
          'limitations':report['limitations']})
registry['connected_flows']['commands']={'verification':verification,'native_types':list(range(8)),
 'external_types':[],'limitations':'Types 1/3/4 use separate note/control/bank-selection suites. Other main-loop services and physical sound synthesis remain separate.'}
path.write_text(json.dumps(registry,indent=2)+'\n')
status_path=project/'project_status.json';status=json.loads(status_path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['arm_sound_commands_native']={
 'verification':verification,'guard_verification':guard_path,'comparisons':1008,'guard_comparisons':26,
 'additional_module_local_routines':4,'total_arm_module_local_native_routines':42,
 'classification':'Native queued-command paths with three explicit remaining handlers; no milestone credit',
 'limitations':report['limitations']}
status_path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded 1008 command comparisons, 26 guards and four additional ARM routines; total 42 module-local entries')
