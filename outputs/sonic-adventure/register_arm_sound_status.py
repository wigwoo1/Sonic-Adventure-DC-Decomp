"""Record connected status recovery only after original-byte and build evidence checks."""
import csv,hashlib,json
from pathlib import Path
from verify_startup import ROOT
P=ROOT/'outputs/sonic-adventure'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
verification='analysis/arm_sound_status_connected_verification.json'
report=json.loads((P/verification).read_text())
assert report['connected'] and not report['native_service_callback']
assert (report['cases_passed'],report['budget_cases_passed'],report['fault_cases_passed'])==(1072,24,4)
for name,value in report['sha256'].items():
    path=Path(name)
    assert sha(path if path.is_absolute() else P/path)==value,name
boot=json.loads((P/'analysis/arm_sound_boot_native_verification.json').read_text())
assert boot['dll_sha256']==sha(ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll')
assert boot['status_source_sha256']==sha(P/'source/sa_arm_sound_status.c')
assert boot['status_translation_sha256']==sha(P/'analysis/arm_sound_status_translation.json')
assert boot['source_sha256']==sha(P/'source/sa_arm_sound_boot_native.c')
assert boot['builder_sha256']==sha(P/'build_arm_sound_boot_native.ps1')
translation=json.loads((P/'analysis/arm_sound_status_translation.json').read_text())
payloads=json.loads((P/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
path=P/'analysis/arm_sound_driver_recovered_routines.json';registry=json.loads(path.read_text())
registry['routines']=[r for r in registry['routines'] if r['verification']!=verification]
assert len(registry['routines'])==50
for driver,rows in enumerate(translation['drivers']):
    image=(ROOT/payloads[driver]['payload_path']).read_bytes()
    assert sha(ROOT/payloads[driver]['payload_path'])==payloads[driver]['upload_sha256']
    with (P/'analysis/arm-sound-drivers'/(Path(payloads[driver]['path']).name+'.arm.bin')/'function_index.tsv').open() as f:
        entries={int(r['address'],16) for r in csv.DictReader(f,delimiter='\t')}
    assert len(rows)==95
    for row in rows:
        address=int(row['address'],16)
        assert image[address:address+int(row['length'])].hex()==row['bytes']
    for address in translation['entries']:
        assert int(address,16) in entries
        registry['routines'].append({'module':payloads[driver]['path'],'address':address,
          'source':'source/sa_arm_sound_status.c','verification':verification,
          'classification':'Complete native main-loop status routine, connected without ARM fallback',
          'limitations':'Finite instruction/state comparisons; no physical timing, synthesis or playback proof.'})
registry['connected_flows']['status']={'verification':verification,'classification':'Both status routines connected in both drivers; reset and 16 main-loop fixtures compared separately',
 'boot_verification':'analysis/arm_sound_boot_native_verification.json'}
path.write_text(json.dumps(registry,indent=2)+'\n')
path=P/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['arm_sound_status_native']={'verification':verification,'comparisons':1072,'budget_cases':24,'constructed_bus_fault_cases':4,
 'total_arm_module_local_native_routines':54,'classification':'Two more connected routines per driver; no whole-project milestone credit'}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded connected status recovery: 1072 comparisons, 28 guards; 54 module-local ARM routines')
