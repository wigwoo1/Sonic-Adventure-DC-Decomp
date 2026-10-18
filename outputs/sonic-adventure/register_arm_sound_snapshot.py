"""Record standalone or connected snapshot evidence with original byte checks."""
import csv,hashlib,json,sys
from pathlib import Path
from verify_startup import ROOT
P=ROOT/'outputs/sonic-adventure'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
connected='--connected' in sys.argv
verification='analysis/arm_sound_snapshot_connected_verification.json' if connected else 'analysis/arm_sound_snapshot_verification.json'
report=json.loads((P/verification).read_text())
assert report['connected']==connected and not report['native_service_callback']
assert [report[k] for k in ('cases_passed','budget_cases_passed','mmio_failure_cases_passed','orrs_flag_cases_passed')]==[384,16,6,32]
for path,value in report['sha256'].items():assert sha(Path(path))==value,path
translation=json.loads((P/'analysis/arm_sound_snapshot_translation.json').read_text())
payloads=json.loads((P/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
for driver,rows in enumerate(translation['drivers']):
    path=ROOT/payloads[driver]['payload_path'];image=path.read_bytes()
    assert sha(path)==payloads[driver]['upload_sha256']
    assert len(rows)==137
    assert int(translation['entries'][driver],16)==0x63f8+8*driver
    for row in rows:
        address=int(row['address'],16)
        assert image[address:address+int(row['length'])].hex()==row['bytes']
if connected:
    boot=json.loads((P/'analysis/arm_sound_boot_native_verification.json').read_text())
    for path,key in ((ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll','dll_sha256'),
      (P/'source/sa_arm_sound_boot_native.c','source_sha256'),(P/'source/sa_arm_sound_snapshot.c','snapshot_source_sha256'),
      (P/'analysis/arm_sound_snapshot_translation.json','snapshot_translation_sha256'),(P/'build_arm_sound_boot_native.ps1','builder_sha256')):
        assert sha(path)==boot[key],path
    path=P/'analysis/arm_sound_driver_recovered_routines.json';registry=json.loads(path.read_text())
    registry['routines']=[r for r in registry['routines'] if r['verification']!=verification]
    assert len(registry['routines'])==56
    for driver,payload in enumerate(payloads):
        with (P/'analysis/arm-sound-drivers'/(Path(payload['path']).name+'.arm.bin')/'function_index.tsv').open() as f:
            entries={int(r['address'],16) for r in csv.DictReader(f,delimiter='\t')}
        address=translation['entries'][driver];assert int(address,16) in entries
        registry['routines'].append({'module':payload['path'],'address':address,'source':'source/sa_arm_sound_snapshot.c',
          'verification':verification,'classification':'Complete connected native sound-state snapshot routine; no ARM callback',
          'limitations':'Finite CPU/state/modeled MMIO comparisons; physical timing, synthesis and audible playback remain unproven.'})
    registry['connected_flows']['snapshot']={'verification':verification,'classification':'Three snapshot loops execute natively for both drivers',
      'boot_verification':'analysis/arm_sound_boot_native_verification.json'}
    path.write_text(json.dumps(registry,indent=2)+'\n')
path=P/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['arm_sound_snapshot_native' if connected else 'arm_sound_snapshot_standalone']={'verification':verification,'comparisons':384,'budget_cases':16,
 'retail_mmio_failure_cases':6,'orrs_flag_cases':32,'classification':'Connected native snapshot proof; 58 module-local routines; no whole-project milestone credit' if connected else 'Standalone native snapshot proof; see connected evidence for current integration status; no separate milestone credit'}
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded', 'connected snapshot evidence: 58 module-local routines' if connected else 'standalone snapshot evidence', ': 384 comparisons and 54 guard/flag checks')
