"""Verify standalone counter/ramp evidence and original-byte identity."""
import csv,hashlib,json,sys
from pathlib import Path
from verify_startup import ROOT
P=ROOT/'outputs/sonic-adventure'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
connected='--connected' in sys.argv
verification='analysis/arm_sound_ramps_connected_verification.json' if connected else 'analysis/arm_sound_ramps_verification.json'
report=json.loads((P/verification).read_text())
assert report['connected']==connected and not report['native_service_callback']
assert [report[k] for k in ('cases_passed','budget_cases_passed','flag_cases_passed')]==[960,64,64]
for path,value in report['sha256'].items():assert sha(Path(path))==value,path
translation=json.loads((P/'analysis/arm_sound_ramps_translation.json').read_text())
payloads=json.loads((P/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
for driver,rows in enumerate(translation['drivers']):
    path=ROOT/payloads[driver]['payload_path'];image=path.read_bytes()
    assert sha(path)==payloads[driver]['upload_sha256'];assert len(rows)==134
    with (P/'analysis/arm-sound-drivers'/(Path(payloads[driver]['path']).name+'.arm.bin')/'function_index.tsv').open() as f:
        entries={int(r['address'],16) for r in csv.DictReader(f,delimiter='\t')}
    assert {int(a,16) for a in translation['entries_by_driver'][driver]}<=entries
    for row in rows:
        address=int(row['address'],16)
        assert image[address:address+int(row['length'])].hex()==row['bytes']
if connected:
    boot=json.loads((P/'analysis/arm_sound_boot_native_verification.json').read_text())
    for path,key in ((ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll','dll_sha256'),
      (P/'source/sa_arm_sound_boot_native.c','source_sha256'),(P/'source/sa_arm_sound_ramps.c','ramps_source_sha256'),
      (P/'analysis/arm_sound_ramps_translation.json','ramps_translation_sha256'),(P/'build_arm_sound_boot_native.ps1','builder_sha256')):
        assert sha(path)==boot[key],path
    flow_path='analysis/arm_sound_ramp_snapshot_flow_verification.json'
    flow=json.loads((P/flow_path).read_text())
    assert [flow[k] for k in ('flow_cases_passed','tick_cycles_passed','batch_checkpoints_passed')]==[8,128,640]
    for path,value in flow['sha256'].items():assert sha(Path(path))==value,path
    path=P/'analysis/arm_sound_driver_recovered_routines.json';registry=json.loads(path.read_text())
    registry['routines']=[r for r in registry['routines'] if r['verification']!=verification]
    assert len(registry['routines'])==58
    for driver,payload in enumerate(payloads):
        for address in translation['entries_by_driver'][driver]:
            registry['routines'].append({'module':payload['path'],'address':address,'source':'source/sa_arm_sound_ramps.c',
              'verification':verification,'additional_verification':[flow_path],
              'classification':'Complete connected native counter/channel-ramp routine; no ARM callback',
              'limitations':'Finite CPU/state comparisons; explicit persistent routine sequence is not complete loop/interrupt/timing/synthesis/playback proof.'})
    registry['connected_flows']['ramps']={'verification':verification,'persistent_flow_verification':flow_path,
      'classification':'Counter/ramps connected; repeated ramps/snapshot flow matches original ARM with no fallback callback'}
    path.write_text(json.dumps(registry,indent=2)+'\n')
path=P/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['arm_sound_ramps_native' if connected else 'arm_sound_ramps_standalone']={'verification':verification,'comparisons':960,'budget_cases':64,'flag_cases':64,
 'classification':'Four complete connected native routines per driver; 66 module-local routines; no whole-project milestone credit' if connected else 'Four standalone native routines per driver; see connected evidence for current integration; no separate milestone credit'}
if connected:status['verified_progress']['arm_sound_ramps_native'].update({'persistent_flows':8,'tick_cycles':128,'checkpoints':640,'persistent_verification':flow_path})
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded', 'connected counter/ramps: 66 module-local routines, 640 persistent checkpoints' if connected else 'standalone counter/ramps', ': 960 comparisons and 128 budget/flag checks')
