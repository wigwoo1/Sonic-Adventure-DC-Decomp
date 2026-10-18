"""Verify standalone sequencer evidence, indirect tables and enriched return blocks."""
import csv,hashlib,json,struct,sys
from pathlib import Path
from verify_startup import ROOT
P=ROOT/'outputs/sonic-adventure'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
connected='--connected' in sys.argv
verification='analysis/arm_sound_sequencer_connected_verification.json' if connected else 'analysis/arm_sound_sequencer_verification.json'
report=json.loads((P/verification).read_text())
assert report['connected']==connected and not report['native_service_callback']
assert [report[k] for k in ('cases_passed','budget_cases_passed','queue_cases_passed','flag_cases_passed')]==[2056,48,48,1024]
for path,value in report['sha256'].items():assert sha(Path(path))==value,path
carry_path='analysis/arm_sound_sequencer_carry_connected_verification.json' if connected else 'analysis/arm_sound_sequencer_carry_verification.json'
carry=json.loads((P/carry_path).read_text());assert carry['connected']==connected
assert carry['cases_passed']==1024 and carry['carry_injections']==1736
for path,value in carry['sha256'].items():assert sha(Path(path))==value,path
translation=json.loads((P/'analysis/arm_sound_sequencer_translation.json').read_text())
payloads=json.loads((P/'analysis/arm_sound_driver_payloads.json').read_text())['drivers']
branches={'b','bl','beq','bne','bcc','bcs','bhi','bmi','bpl','bgt','blt','ble','bge','blne','bleq'}
provenance={}
for driver,rows in enumerate(translation['drivers']):
    path=ROOT/payloads[driver]['payload_path'];image=path.read_bytes()
    assert sha(path)==payloads[driver]['upload_sha256'];assert len(rows)==405
    directory=P/'analysis/arm-sequencer-enrichment'/(Path(payloads[driver]['path']).name+'.arm.bin')
    with (directory/'instructions.tsv').open() as f:enriched={int(r['address'],16):r for r in csv.DictReader(f,delimiter='\t')}
    with (directory/'program_metadata.tsv').open() as f:metadata={r['key']:r['value'] for r in csv.DictReader(f,delimiter='\t')}
    assert metadata['image_sha256']==sha(path)
    addresses={int(r['address'],16) for r in rows}
    for row in rows:
        address=int(row['address'],16);assert enriched[address]==row
        assert image[address:address+int(row['length'])].hex()==row['bytes']
        mnemonic,operand=row['instruction'].split(' ',1)
        if mnemonic in branches:assert int(operand,16) in addresses,(driver,row)
    for table in translation['tables_by_driver'][driver]:
        pointer=struct.unpack_from('<I',image,table['literal'])[0];assert pointer==table['pointer']
        assert image[pointer:pointer+4*table['count']].hex()==table['bytes']
        assert list(struct.unpack_from('<'+'I'*table['count'],image,pointer))==table['targets']
        assert set(table['targets'])<=addresses
        assert not any(pointer<=a<pointer+4*table['count'] for a in addresses)
    for lo,hi in ((0x4290,0x4298),(0x47fc,0x4810)):
        assert set(range(lo+driver*8,hi+driver*8+4,4))<=addresses
    for name in ('instructions.tsv','program_metadata.tsv'):provenance[str(directory/name)]=sha(directory/name)
for name in ('ghidra/SeedArmSequencerReturns.java','ghidra/recover_arm_sequencer_returns.ps1','ghidra/ExportArmSoundDriverEvidence.java'):
    provenance[name]=sha(P/name)
(P/('analysis/arm_sound_sequencer_connected_provenance.json' if connected else 'analysis/arm_sound_sequencer_provenance.json')).write_text(json.dumps({'verification':verification,'verification_sha256':sha(P/verification),'carry_verification':carry_path,'carry_verification_sha256':sha(P/carry_path),'additional_inputs_sha256':provenance,
 'scope':'Separate enrichment exports preserve baseline snapshots. All translated rows/bytes and table targets checked; original return-skipping blocks included. MLAS C follows TI925T reference only, not hardware proof.'},indent=2)+'\n')
if connected:
    boot=json.loads((P/'analysis/arm_sound_boot_native_verification.json').read_text())
    for path,key in ((ROOT/'work/sonic-adventure/build/arm_sound_boot_native.dll','dll_sha256'),
      (P/'source/sa_arm_sound_boot_native.c','source_sha256'),(P/'source/sa_arm_sound_sequencer.c','sequence_source_sha256'),
      (P/'analysis/arm_sound_sequencer_translation.json','sequence_translation_sha256'),(P/'build_arm_sound_boot_native.ps1','builder_sha256')):
        assert sha(path)==boot[key],path
    flow_path='analysis/arm_sound_sequence_effect_flow_verification.json';flow=json.loads((P/flow_path).read_text())
    assert [flow[k] for k in ('flow_cases_passed','tick_cycles_passed','batch_checkpoints_passed')]==[8,128,1536]
    for path,value in flow['sha256'].items():assert sha(Path(path))==value,path
    path=P/'analysis/arm_sound_driver_recovered_routines.json';registry=json.loads(path.read_text())
    registry['routines']=[r for r in registry['routines'] if r['verification']!=verification]
    assert len(registry['routines'])==90
    for driver,payload in enumerate(payloads):
        with (P/'analysis/arm-sound-drivers'/(Path(payload['path']).name+'.arm.bin')/'function_index.tsv').open() as f:
            entries={int(r['address'],16) for r in csv.DictReader(f,delimiter='\t')}
        for address in (0x4210+driver*8,0x4874+driver*8,0x48a0+driver*8,0x63dc+driver*8):
            assert address in entries
            registry['routines'].append({'module':payload['path'],'address':hex(address),'source':'source/sa_arm_sound_sequencer.c',
              'verification':verification,'additional_verification':[carry_path,flow_path],
              'classification':'Complete connected sequencer/helper routine with indirect handlers and return-skipping blocks; no ARM callback',
              'limitations':'Finite constructed sequences and contiguous segment proof; MLAS C is TI925T convention, not hardware validation. No whole-loop/interrupt/timing/synthesis/playback proof.'})
    registry['connected_flows']['sequencer']={'verification':verification,'carry_verification':carry_path,'persistent_flow_verification':flow_path,
      'classification':'Sequencer connected; contiguous stream/sequencer/channel segment without skipped calls in that segment'}
    path.write_text(json.dumps(registry,indent=2)+'\n')
path=P/'project_status.json';status=json.loads(path.read_text(encoding='utf-8-sig'))
status.setdefault('verified_progress',{})['arm_sound_sequencer_native' if connected else 'arm_sound_sequencer_standalone']={'verification':verification,'comparisons':2056,'budget_cases':48,'queue_cases':48,'multiply_vs_cases':1024,
 'forced_carry_comparisons':1024,'carry_injections':1736,'carry_verification':carry_path,
 'classification':'Connected sequencer/helpers; 98 module-local routines; no whole-project milestone credit' if connected else 'Standalone sequencer/handlers; see connected evidence for current integration; no separate milestone credit',
 'limitations':'Finite constructed sequences; MLAS C convention matches TI925T, not ARMv4 hardware verification.'}
if connected:status['verified_progress']['arm_sound_sequencer_native'].update({'persistent_flows':8,'tick_cycles':128,'checkpoints':1536,'persistent_verification':flow_path})
path.write_text(json.dumps(status,indent=2)+'\n')
print('Recorded', 'connected sequencer: 98 module-local routines, 1536 interaction checkpoints' if connected else 'standalone sequencer', ': 2056 comparisons, 1120 focused checks and 1024 forced-carry comparisons')
