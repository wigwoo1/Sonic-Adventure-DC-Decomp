"""Validate standalone stream-update evidence and original helper byte spans."""
import csv,hashlib,json,sys
from pathlib import Path
from verify_startup import ROOT
P=ROOT/'outputs/sonic-adventure'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
connected='--connected' in sys.argv
verification='analysis/arm_sound_stream_update_connected_verification.json' if connected else 'analysis/arm_sound_stream_update_verification.json'
report=json.loads((P/verification).read_text())
assert report['connected']==connected and not report['native_service_callback']
assert [report[k] for k in ('cases_passed','budget_cases_passed','mmio_failure_cases_passed','flag_cases_passed')]==[576,16,12,640]
for path,value in report['sha256'].items():assert sha(Path(path))==value,path
translation=json.loads((P/'analysis/arm_sound_stream_update_translation.json').read_text())
payloads=json.loads((P/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
branches={'b','bl','beq','bne','bcc','bcs','bhi','bmi','bpl','bgt','blt','ble','bge','blne','bleq'}
for driver,rows in enumerate(translation['drivers']):
    path=ROOT/payloads[driver]['payload_path'];image=path.read_bytes()
    assert sha(path)==payloads[driver]['upload_sha256'];assert len(rows)==209
    addresses={int(r['address'],16) for r in rows}
    for row in rows:
        address=int(row['address'],16)
        assert image[address:address+int(row['length'])].hex()==row['bytes']
        mnemonic,operand=row['instruction'].split(' ',1)
        if mnemonic in branches:assert int(operand,16) in addresses,(driver,row)
    with (P/'analysis/arm-sound-drivers'/(Path(payloads[driver]['path']).name+'.arm.bin')/'function_index.tsv').open() as f:
        entries={int(r['address'],16) for r in csv.DictReader(f,delimiter='\t')}
    assert int(translation['entries'][driver],16) in entries
if connected:
    boot=json.loads((P/'analysis/arm_sound_boot_native_verification.json').read_text())
    for path,key in ((ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll','dll_sha256'),
      (P/'source/sa_arm_sound_boot_native.c','source_sha256'),(P/'source/sa_arm_sound_stream_update.c','stream_source_sha256'),
      (P/'analysis/arm_sound_stream_update_translation.json','stream_translation_sha256'),(P/'build_arm_sound_boot_native.ps1','builder_sha256')):
        assert sha(path)==boot[key],path
    flow_path='analysis/arm_sound_stream_effect_flow_verification.json';flow=json.loads((P/flow_path).read_text())
    assert [flow[k] for k in ('flow_cases_passed','tick_cycles_passed','batch_checkpoints_passed')]==[8,128,1408]
    for path,value in flow['sha256'].items():assert sha(Path(path))==value,path
    path=P/'analysis/arm_sound_driver_recovered_routines.json';registry=json.loads(path.read_text())
    registry['routines']=[r for r in registry['routines'] if r['verification']!=verification]
    assert len(registry['routines'])==76
    for driver,payload in enumerate(payloads):
        with (P/'analysis/arm-sound-drivers'/(Path(payload['path']).name+'.arm.bin')/'function_index.tsv').open() as f:
            entries={int(r['address'],16) for r in csv.DictReader(f,delimiter='\t')}
        for address in (0x70a4+driver*8,0x7100+driver*8,0x72d8+driver*8,0x7350+driver*8,0x7d64+driver*4,0x817c+driver*4,0x8188+driver*4):
            assert address in entries
            assert address in {int(r['address'],16) for r in translation['drivers'][driver]}
            registry['routines'].append({'module':payload['path'],'address':hex(address),'source':'source/sa_arm_sound_stream_update.c',
              'verification':verification,'additional_verification':[flow_path],
              'classification':'Complete connected stream-update/helper routine; shared tails included; no ARM callback',
              'limitations':'Finite SVC/state/modeled MMIO proof; sequencer gap explicitly skipped in interaction fixture. Not whole-loop/interrupt/timing/synthesis/playback proof.'})
    registry['connected_flows']['stream_update']={'verification':verification,'persistent_flow_verification':flow_path,
      'classification':'24-stream update and direct helpers connected; repeated stream/effects original-site flow with explicit sequencer gap'}
    path.write_text(json.dumps(registry,indent=2)+'\n')
path=P/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['arm_sound_stream_update_native' if connected else 'arm_sound_stream_update_standalone']={'verification':verification,'comparisons':576,'budget_cases':16,'retail_mmio_failure_cases':12,'condition_shift_cases':640,
 'classification':'Complete connected stream update/helpers; 90 module-local routines; no whole-project milestone credit' if connected else 'Standalone stream update/helpers; see connected evidence for current integration; no separate milestone credit'}
if connected:status['verified_progress']['arm_sound_stream_update_native'].update({'persistent_flows':8,'tick_cycles':128,'checkpoints':1408,'persistent_verification':flow_path})
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded', 'connected stream update: 90 module-local routines, 1408 interaction checkpoints' if connected else 'standalone stream update', ': 576 comparisons and 668 guard/condition checks')
