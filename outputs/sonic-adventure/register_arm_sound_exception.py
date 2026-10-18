"""Record bounded standalone exception/input recovery, not integration credit."""
import csv,hashlib,json,re,struct
import sys
from pathlib import Path
from verify_startup import ROOT
P=ROOT/'outputs/sonic-adventure'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
connected='--connected' in sys.argv
verification='analysis/arm_sound_exception_connected_verification.json' if connected else 'analysis/arm_sound_exception_verification.json';report=json.loads((P/verification).read_text());assert report['connected']==connected
assert [report[k] for k in ('cases_passed','resume_cases_passed','resume_checkpoints_passed','atomic_guards_passed','budget_cases_passed','mmio_fault_cases_passed')]==[1052,18,638,14,54,20]
assert all(not rows for rows in report['uncovered_rows'])
for file,value in report['sha256'].items():assert sha(Path(file))==value,file
translation=json.loads((P/'analysis/arm_sound_exception_translation.json').read_text())
inputs=json.loads((P/'analysis/arm_sound_input_translation.json').read_text())
payloads=json.loads((P/'analysis/arm_sound_driver_payloads.json').read_text())['drivers'];tables=[]
source=(P/'source/sa_arm_sound_exception.c').read_text().split('if(driver==0){switch(r[15]){',1)[1].split('}}else{switch(r[15]){',1)
for driver,rows in enumerate(translation['drivers']):
 assert len(rows)==79
 image=(ROOT/payloads[driver]['payload_path']).read_bytes()
 assert sha(ROOT/payloads[driver]['payload_path'])==payloads[driver]['upload_sha256']
 with (P/'analysis/arm-sequencer-enrichment'/(Path(payloads[driver]['path']).name+'.arm.bin')/'instructions.tsv').open() as f:exported={int(r['address'],16):r for r in csv.DictReader(f,delimiter='\t')}
 addresses={int(r['address'],16) for r in rows}
 assert addresses=={int(a,16) for a in re.findall(r'case (0x[0-9a-f]+)u:',source[driver])}
 input_addresses={int(r['address'],16) for r in inputs['drivers'][driver]}
 for row in rows:
  address=int(row['address'],16);assert exported[address]==row and image[address:address+4].hex()==row['bytes']
  mnemonic,operand=row['instruction'].split(' ',1)
  if mnemonic in ('b','bl','bne','beq'):assert int(operand,16) in addresses|input_addresses
 targets=[]
 for address in range(0x284,0x2a4,4):
  word=struct.unpack_from('<I',image,address)[0];assert word>>24==0xea
  delta=word&0xffffff
  if delta&0x800000:delta-=1<<24
  target=address+8+4*delta;assert target in addresses;targets.append(target)
 tables.append({'driver':driver,'base':0x284,'count':8,'targets':targets})
 assert image[0x240:0x244].hex()=='0080dde8' and image[0x364:0x368].hex()=='0080fde8'
provenance={'connected':connected,'verification':verification,'verification_sha256':sha(P/verification),'instruction_sites_per_driver':79,'branch_tables':tables,'architecture_reference':report['architecture_reference'],'classification':'Selected-library native vectors/generic exception/FIQ handler plus input call. Explicit already-entered exception context and SVC/ARM return bank snapshots. All selected sites exercised; finite timer/input/register-preservation/acknowledgement assertions, budgets, faults, resumptions and atomic guards. No automatic IRQ injection, nesting, hardware timing, complete host event scheduling, synthesis or complete-game proof; no connected routine credit.'}
provenance['selected_library']='Main native sound driver' if connected else 'Standalone native module'
(P/('analysis/arm_sound_exception_connected_provenance.json' if connected else 'analysis/arm_sound_exception_provenance.json')).write_text(json.dumps(provenance,indent=2)+'\n')
path=P/'project_status.json';status=json.loads(path.read_text());status.setdefault('verified_progress',{})['arm_sound_exception_connected' if connected else 'arm_sound_exception_standalone']=provenance;path.write_text(json.dumps(status,indent=2)+'\n')
print('Main-library' if connected else 'Standalone', '79 exception/vector sites per driver; 1052 cases, 18 resume flows/638 checkpoints, 88 guard/budget/fault checks; no connected credit')
