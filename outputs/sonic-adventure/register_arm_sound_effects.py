"""Record standalone five-effect evidence without connected-driver credit."""
import csv,hashlib,json,sys
from pathlib import Path
from verify_startup import ROOT
P=ROOT/'outputs/sonic-adventure'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
connected='--connected' in sys.argv
verification='analysis/arm_sound_effects_connected_verification.json' if connected else 'analysis/arm_sound_effects_verification.json'
report=json.loads((P/verification).read_text())
assert report['connected']==connected and not report['native_service_callback']
assert [report[k] for k in ('cases_passed','budget_cases_passed','mmio_failure_cases_passed')]==[1040,60,12]
for path,value in report['sha256'].items():assert sha(Path(path))==value,path
translation=json.loads((P/'analysis/arm_sound_effects_translation.json').read_text())
payloads=json.loads((P/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
for driver,rows in enumerate(translation['drivers']):
    path=ROOT/payloads[driver]['payload_path'];image=path.read_bytes()
    assert sha(path)==payloads[driver]['upload_sha256'];assert len(rows)==300
    with (P/'analysis/arm-sound-drivers'/(Path(payloads[driver]['path']).name+'.arm.bin')/'function_index.tsv').open() as f:
        entries={int(r['address'],16) for r in csv.DictReader(f,delimiter='\t')}
    assert {int(a,16) for a in translation['entries_by_driver'][driver]}<=entries
    for row in rows:
        address=int(row['address'],16)
        assert image[address:address+int(row['length'])].hex()==row['bytes']
if connected:
    boot=json.loads((P/'analysis/arm_sound_boot_native_verification.json').read_text())
    for path,key in ((ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll','dll_sha256'),
      (P/'source/sa_arm_sound_boot_native.c','source_sha256'),(P/'source/sa_arm_sound_effects.c','effects_source_sha256'),
      (P/'analysis/arm_sound_effects_translation.json','effects_translation_sha256'),(P/'build_arm_sound_boot_native.ps1','builder_sha256')):
        assert sha(path)==boot[key],path
    flow_path='analysis/arm_sound_effect_flow_verification.json';flow=json.loads((P/flow_path).read_text())
    assert [flow[k] for k in ('flow_cases_passed','tick_cycles_passed','batch_checkpoints_passed')]==[8,128,1280]
    for path,value in flow['sha256'].items():assert sha(Path(path))==value,path
    path=P/'analysis/arm_sound_driver_recovered_routines.json';registry=json.loads(path.read_text())
    registry['routines']=[r for r in registry['routines'] if r['verification']!=verification]
    assert len(registry['routines'])==66
    for driver,payload in enumerate(payloads):
        for address in translation['entries_by_driver'][driver]:
            registry['routines'].append({'module':payload['path'],'address':address,'source':'source/sa_arm_sound_effects.c',
              'verification':verification,'additional_verification':[flow_path],
              'classification':'Complete connected native effect routine and recovered pitch dependency; no ARM callback',
              'limitations':'Finite CPU/state/modeled MMIO comparisons and contiguous loop segment proof; not whole-loop/interrupt/timing/synthesis/playback proof.'})
    registry['connected_flows']['effects']={'verification':verification,'persistent_flow_verification':flow_path,
      'classification':'Ten-call original main-loop update segment executes natively; five effect routines per driver connected'}
    path.write_text(json.dumps(registry,indent=2)+'\n')
path=P/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['arm_sound_effects_native' if connected else 'arm_sound_effects_standalone']={'verification':verification,'comparisons':1040,'budget_cases':60,'retail_mmio_failure_cases':12,
 'classification':'Five connected effects per driver; 76 module-local routines; no whole-project milestone credit' if connected else 'Five standalone effects per driver; see connected evidence for current integration; no separate milestone credit'}
if connected:status['verified_progress']['arm_sound_effects_native'].update({'persistent_flows':8,'tick_cycles':128,'checkpoints':1280,'persistent_verification':flow_path})
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded', 'connected effects: 76 module-local routines, 1280 original-site checkpoints' if connected else 'standalone effects', ': 1040 comparisons and 72 guards')
