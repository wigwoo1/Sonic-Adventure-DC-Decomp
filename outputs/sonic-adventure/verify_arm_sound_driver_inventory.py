"""Validate draft ARM analysis against the exact original upload bytes."""
import collections,csv,hashlib,json,re
from pathlib import Path
from verify_startup import ROOT
project=ROOT/'outputs/sonic-adventure'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
payload_report=project/'analysis/arm_sound_driver_payloads.json'
payloads=json.loads(payload_report.read_text())
assert payloads['inventory_sha256']==sha(project/'disc_inventory.json')
assert payloads['preparer_sha256']==sha(project/'prepare_arm_sound_drivers.py')
assert payloads['upload_source_sha256']==sha(project/'source/sa_sound_driver_transfer.c')
results=[];outputs={}
for driver in payloads['drivers']:
    image=(ROOT/driver['payload_path']).read_bytes()
    assert len(image)==driver['upload_size'] and hashlib.sha256(image).hexdigest()==driver['upload_sha256']
    directory=project/'analysis/arm-sound-drivers'/(Path(driver['path']).name+'.arm.bin')
    def rows(name):
        with (directory/name).open(newline='') as stream:return list(csv.DictReader(stream,delimiter='\t'))
    metadata={r['key']:r['value'] for r in rows('program_metadata.tsv')}
    assert metadata['language']==driver['language']=='ARM:LE:32:v4'
    assert int(metadata['image_base'],16)==0 and int(metadata['image_size'])==len(image)
    assert metadata['image_sha256']==driver['upload_sha256']
    index=rows('function_index.tsv');instructions=rows('instructions.tsv')
    assert index and instructions
    addresses=[int(r['address'],16) for r in index]
    assert len(addresses)==len(set(addresses))
    assert all(0<=a<len(image) and a%4==0 for a in addresses)
    assert all(int(r['body_bytes'])>0 for r in index)
    statuses=dict(collections.Counter(r['decompilation_status'] for r in index))
    assert not set(statuses)-{'draft_pseudocode','failed'}
    covered=set();listing={}
    for row in instructions:
        address=int(row['address'],16);length=int(row['length']);raw=bytes.fromhex(row['bytes'])
        assert length==len(raw)==4 and address%4==0
        assert 0<=address<address+length<=len(image)
        assert image[address:address+length]==raw
        assert not covered.intersection(range(address,address+length))
        covered.update(range(address,address+length));listing[address]=row
    assert len(instructions)==int(metadata['instruction_rows'])
    assert len(covered)==int(metadata['instruction_bytes'])
    for vector in driver['vectors']:
        address=int(vector['address'],16);target=int(vector['branch_target'],16)
        assert address in listing and target in listing
        assert listing[address]['instruction'].startswith('b ')
        assert target in [int(value,16) for value in listing[address]['flows'].split()]
    code=(directory/'initial_pseudocode.c').read_text()
    assert len(re.findall(r'/\* Address: [0-9a-fA-F]+ \*/',code))==statuses.get('draft_pseudocode',0)
    results.append({'path':driver['path'],'candidate_functions':len(index),'statuses':statuses,
      'instructions':len(instructions),'decoded_bytes':len(covered),'upload_bytes':len(image),
      'bytes_outside_current_instruction_listing':len(image)-len(covered),
      'draft_warning_count':code.count('WARNING'),'vectors_checked':len(driver['vectors'])})
    for name in ('program_metadata.tsv','function_index.tsv','instructions.tsv','initial_pseudocode.c'):
        path=directory/name;outputs[str(path.relative_to(ROOT)).replace('\\','/')]=sha(path)
tool=ROOT/'work/sonic-adventure/toolchain/ghidra_12.1.4_PUBLIC'
exporter=tool/'Ghidra/Extensions/SonicAdventureScripts/ghidra_scripts/ExportInitialDecompilation.java'
# Ghidra resolves the installed script before the local copy; pin that actual input.
assert exporter.read_text().strip()==(project/'ghidra/ExportInitialDecompilation.java').read_text().strip()
inputs={str(path.relative_to(ROOT)).replace('\\','/'):sha(path) for path in (
 payload_report,project/'prepare_arm_sound_drivers.py',project/'verify_arm_sound_driver_inventory.py',
 project/'ghidra/analyze_arm_sound_drivers.ps1',project/'ghidra/SeedArmSoundDriver.java',
 project/'ghidra/ExportArmSoundDriverEvidence.java',exporter,
 tool/'Ghidra/Processors/ARM/data/languages/ARM.ldefs',
 tool/'Ghidra/Processors/ARM/data/languages/ARM4_le.sla')}
report={'drivers':results,'inputs':inputs,'outputs':outputs,'ghidra_version':'12.1.4_PUBLIC',
 'scope':'Both actual SDRV upload spans, all sixteen vector branches, every exported instruction byte, and complete current candidate/draft exports validated against original input hashes.',
 'limitations':'Draft pseudocode is not buildable recovered source. Function boundaries/code-data separation and indirect targets are incomplete; listing counts are not coverage. No ARM execution or playback verification. Existing SH-4 routine registry and weighted milestone percentage are unchanged.'}
(project/'analysis/arm_sound_driver_inventory_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(results))
