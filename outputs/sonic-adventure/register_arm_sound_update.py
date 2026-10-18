"""Register the native pending-update routines only with current connected evidence."""
import csv,hashlib,json
from pathlib import Path
from verify_startup import ROOT
P=ROOT/'outputs/sonic-adventure'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
verification='analysis/arm_sound_update_connected_verification.json'
report=json.loads((P/verification).read_text())
assert report['connected'] and not report['native_service_callback']
assert (report['cases_passed'],report['budget_cases_passed'],report['mmio_failure_cases_passed'])==(128,16,6)
for name,value in report['sha256'].items():assert sha(Path(name))==value,name
boot=json.loads((P/'analysis/arm_sound_boot_native_verification.json').read_text())
for path,key in ((ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll','dll_sha256'),
 (P/'source/sa_arm_sound_update.c','update_source_sha256'),(P/'analysis/arm_sound_update_translation.json','update_translation_sha256'),
 (P/'source/sa_arm_sound_boot_native.c','source_sha256'),(P/'source/sa_arm_sound_reset_services.c','reset_services_source_sha256'),
 (P/'build_arm_sound_boot_native.ps1','builder_sha256')):assert sha(path)==boot[key],path
translation=json.loads((P/'analysis/arm_sound_update_translation.json').read_text())
payloads=json.loads((P/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
path=P/'analysis/arm_sound_driver_recovered_routines.json';registry=json.loads(path.read_text())
registry['routines']=[r for r in registry['routines'] if r['verification']!=verification]
assert len(registry['routines'])==54
for driver,rows in enumerate(translation['drivers']):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    assert sha(ROOT/payloads[driver]['payload_path'])==payloads[driver]['upload_sha256']
    assert len(rows)==66
    for row in rows:
        address=int(row['address'],16)
        assert image[address:address+int(row['length'])].hex()==row['bytes']
    with (P/'analysis/arm-sound-drivers'/(Path(payloads[driver]['path']).name+'.arm.bin')/'function_index.tsv').open() as f:
        entries={int(r['address'],16) for r in csv.DictReader(f,delimiter='\t')}
    address=translation['entries'][driver];assert int(address,16) in entries
    registry['routines'].append({'module':payloads[driver]['path'],'address':address,'source':'source/sa_arm_sound_update.c',
      'sources':['source/sa_arm_sound_update.c','source/sa_arm_sound_reset_services.c','source/arm_sound_memory.h'],
      'verification':verification,'classification':'Complete connected native pending-update routine and recovered helper calls; no ARM callback',
      'limitations':'Finite SVC instruction/state and modeled MMIO comparisons. No physical timing, synthesis or playback proof.'})
registry['connected_flows']['pending_update']={'verification':verification,'classification':'Pending update, descriptor upload and reset-helper calls execute in native C for both drivers',
 'boot_verification':'analysis/arm_sound_boot_native_verification.json'}
path.write_text(json.dumps(registry,indent=2)+'\n')
path=P/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['arm_sound_update_native']={'verification':verification,'comparisons':128,'budget_cases':16,'retail_mmio_failure_cases':6,
 'total_arm_module_local_native_routines':56,'classification':'One more connected routine per driver; no whole-project milestone credit'}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded connected pending update: 128 comparisons, 22 guards; 56 module-local ARM routines')
