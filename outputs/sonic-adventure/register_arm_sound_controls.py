"""Pin control paths, secondary branch bytes and the complete DSP helper."""
import csv,hashlib,json
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure'
paths={k:'analysis/'+v for k,v in {
 'controls':'arm_sound_controls_verification.json','scales':'arm_sound_control_scales_verification.json',
 'dsp':'arm_sound_dsp_config_verification.json','guards':'arm_sound_controls_guards_verification.json'}.items()}
reports={k:json.loads((project/v).read_text()) for k,v in paths.items()}
control,scale,dsp,guard=[reports[k] for k in ('controls','scales','dsp','guards')]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
assert (control['control_cases_passed'],control['native_batch_cases_passed'])==(6144,512)
assert scale['cases_passed']==216 and (dsp['cases_passed'],dsp['independently_checked_successes'])==(960,128)
assert (guard['instruction_budget_cases_passed'],guard['mmio_failure_cases_passed'])==(24,6)
assert guard['addition_flag_cases_passed']==32
assert len(control['fixture_hashes'])==8
assert all(f['active_slots']==(4 if f['active'] else 0) for f in control['fixture_hashes'])
boot=json.loads((project/'analysis/arm_sound_boot_native_verification.json').read_text())
for key,report in reports.items():
    assert report['dll_sha256']==boot['dll_sha256']==sha(ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll')
    assert report['source_sha256']==boot['controls_source_sha256']==sha(project/'source/sa_arm_sound_controls.c')
    name={'controls':'verify_arm_sound_controls.py','scales':'verify_arm_sound_control_scales.py','dsp':'verify_arm_sound_dsp_config.py','guards':'verify_arm_sound_controls_guards.py'}[key]
    assert report['verifier_sha256']==sha(project/name)
for report in (control,scale,dsp):assert report['translation_sha256']==boot['controls_translation_sha256']==sha(project/'analysis/arm_sound_controls_translation.json')
assert control['generator_sha256']==sha(project/'create_arm_sound_controls.py')
for report in (control,dsp):
    assert report['adapter_sha256']==sha(project/'arm_legacy_alignment_reference.py')
    assert report['header_sha256']==boot['memory_header_sha256']==sha(project/'source/arm_sound_memory.h')
translation=json.loads((project/'analysis/arm_sound_controls_translation.json').read_text())['drivers']
payloads=json.loads((project/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
for driver,data in enumerate(translation):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    assert hashlib.sha256(image).hexdigest()==payloads[driver]['upload_sha256']
    assert len(data['derived_branch_table'])==9
    for row in data['derived_branch_table']:
        address=int(row['address'],16);raw=image[address:address+4]
        assert raw.hex()==row['bytes'] and int.from_bytes(raw,'little')>>24==0xea
        immediate=int.from_bytes(raw,'little')&0xffffff
        if immediate&0x800000:immediate-=1<<24
        assert int(row['flows'],16)==address+8+immediate*4
path=project/'analysis/arm_sound_driver_recovered_routines.json';registry=json.loads(path.read_text())
registry['routines']=[r for r in registry['routines'] if r['source']!='source/sa_arm_sound_controls.c']
assert len(registry['routines'])==42
for driver,data in enumerate(translation):
    directory=project/'analysis/arm-sound-drivers'/(payloads[driver]['path'].split('/')[-1]+'.arm.bin')
    with (directory/'function_index.tsv').open(newline='') as stream:entries={int(r['address'],16) for r in csv.DictReader(stream,delimiter='\t')}
    assert int(data['dsp_helper'],16) in entries
    registry['routines'].append({'module':payloads[driver]['path'],'address':data['dsp_helper'],
      'source':'source/sa_arm_sound_controls.c','verification':paths['dsp'],
      'classification':'Complete native DSP configuration helper; DSP program execution is separate',
      'limitations':dsp['limitations']})
registry['connected_flows']['controls']={'verification':paths['controls'],'secondary_dispatch_verification':paths['scales'],
 'classification':'Complete type-3 command subpath, not a separately identified complete function',
 'limitations':control['limitations']}
path.write_text(json.dumps(registry,indent=2)+'\n')
path=project/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['arm_sound_controls_native']={
 'verification':paths['controls'],'secondary_dispatch_verification':paths['scales'],'dsp_verification':paths['dsp'],'guard_verification':paths['guards'],
 'control_cases':6144,'queued_cases':512,'secondary_dispatch_cases':216,'dsp_cases':960,'guard_and_flag_cases':62,
 'total_arm_module_local_native_routines':44,'classification':'Native type-3 control subpath and complete DSP helper; no milestone credit',
 'limitations':control['limitations']}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded native controls/DSP: 7832 comparisons, 62 guard/flag checks, 44 module-local ARM routines')
