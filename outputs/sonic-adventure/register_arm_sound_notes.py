"""Record complete note/allocation and command/intake recovery with finite evidence."""
import csv,hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure'
paths={key:'analysis/'+name for key,name in {
 'notes':'arm_sound_notes_verification.json','streams':'arm_sound_command_stream_verification.json','guards':'arm_sound_notes_guards_verification.json'}.items()}
reports={k:json.loads((project/v).read_text()) for k,v in paths.items()}
notes,streams,guards=[reports[k] for k in ('notes','streams','guards')]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
assert [notes[k] for k in ('note_cases_passed','layer_cases_passed','filter_cases_passed','disabled_program_cases_passed','native_batch_cases_passed')]==[1920,40,128,12,96]
assert [streams[k] for k in ('stream_cases_passed','batch_checkpoints_passed','commands_consumed')]==[8,64,256]
assert [guards[k] for k in ('instruction_budget_cases_passed','mmio_failure_cases_passed','cmn_flag_cases_passed')]==[36,8,16]
boot=json.loads((project/'analysis/arm_sound_boot_native_verification.json').read_text())
for key,report in reports.items():
    assert report['dll_sha256']==boot['dll_sha256']==sha(ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll')
    assert report['source_sha256']==boot['notes_source_sha256']==sha(project/'source/sa_arm_sound_notes.c')
    name={'notes':'verify_arm_sound_notes.py','streams':'verify_arm_sound_command_stream.py','guards':'verify_arm_sound_notes_guards.py'}[key]
    assert report['verifier_sha256']==sha(project/name)
assert notes['generator_sha256']==sha(project/'create_arm_sound_notes.py')
assert notes['emitter_sha256']==sha(project/'create_arm_sound_controls.py')
assert notes['translation_sha256']==boot['notes_translation_sha256']==sha(project/'analysis/arm_sound_notes_translation.json')
assert notes['adapter_sha256']==sha(project/'arm_legacy_alignment_reference.py')
assert notes['header_sha256']==boot['memory_header_sha256']==sha(project/'source/arm_sound_memory.h')
translation=json.loads((project/'analysis/arm_sound_notes_translation.json').read_text())['drivers']
payloads=json.loads((project/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
path=project/'analysis/arm_sound_driver_recovered_routines.json';registry=json.loads(path.read_text())
registry['routines']=[r for r in registry['routines'] if r['verification']!=paths['notes']]
assert len(registry['routines'])==44
sources=['source/'+n for n in ('sa_arm_sound_boot_native.c','sa_arm_sound_reset_services.c','sa_arm_sound_commands.c','sa_arm_sound_bank_select.c','sa_arm_sound_controls.c','sa_arm_sound_notes.c','arm_sound_memory.h')]
for driver,data in enumerate(translation):
    directory=project/'analysis/arm-sound-drivers'/(payloads[driver]['path'].split('/')[-1]+'.arm.bin')
    with (directory/'function_index.tsv').open(newline='') as stream:entries={int(r['address'],16) for r in csv.DictReader(stream,delimiter='\t')}
    for address,source,classification in (
       (data['allocation_helper'],'source/sa_arm_sound_notes.c','Complete native voice-allocation helper'),
       ('0x9d0','source/sa_arm_sound_boot_native.c','Complete native command dispatcher across split recovered C components'),
       ('0x9a4','source/sa_arm_sound_commands.c','Complete native four-command intake with all eight command handlers')):
        assert int(address,16) in entries
        registry['routines'].append({'module':payloads[driver]['path'],'address':address,'source':source,
          'sources':sources,'verification':paths['notes'],'additional_verification':[paths['streams'],paths['guards']],
          'classification':classification,'limitations':notes['limitations']})
registry['connected_flows']['notes']={'verification':paths['notes'],'mixed_stream_verification':paths['streams'],
 'classification':'All eight command types execute in native C, with no ARM callback in supported command runs',
 'limitations':'Remaining main-loop, interrupt/timing and sound-chip synthesis work is separate.'}
path.write_text(json.dumps(registry,indent=2)+'\n')
path=project/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['arm_sound_notes_native']={
 'verification':paths['notes'],'mixed_stream_verification':paths['streams'],'guard_verification':paths['guards'],
 'note_comparisons':2196,'stream_cases':8,'batch_checkpoints':64,'stream_commands':256,'guard_cases':60,
 'total_arm_module_local_native_routines':50,'classification':'Complete native command/intake/allocation recovery, not complete driver or game recovery; no milestone credit',
 'limitations':notes['limitations']}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded native note/allocation/complete command intake: 2196 note comparisons, 64 stream checkpoints, 60 guards; 50 module-local routines')
