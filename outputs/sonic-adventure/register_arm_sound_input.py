"""Record standalone parser/producer evidence without connected routine credit."""
import csv,hashlib,json,struct
import sys
from pathlib import Path
from verify_startup import ROOT
P=ROOT/'outputs/sonic-adventure'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
connected='--connected' in sys.argv
verification='analysis/arm_sound_input_connected_verification.json' if connected else 'analysis/arm_sound_input_verification.json';report=json.loads((P/verification).read_text());assert report['connected']==connected
assert [report[k] for k in ('cases_passed','producer_cases_passed','persistent_flows_passed','persistent_checkpoints_passed','budget_cases_passed','mmio_fault_cases_passed','atomic_guards_passed')]==[1304,96,8,56,16,12,9]
assert all(not rows for rows in report['uncovered_rows'])
for file,value in report['sha256'].items():assert sha(Path(file))==value,file
translation=json.loads((P/'analysis/arm_sound_input_translation.json').read_text())
payloads=json.loads((P/'analysis/arm_sound_driver_payloads.json').read_text())['drivers'];tables=[]
for driver,rows in enumerate(translation['drivers']):
 assert len(rows)==109
 image=(ROOT/payloads[driver]['payload_path']).read_bytes()
 assert sha(ROOT/payloads[driver]['payload_path'])==payloads[driver]['upload_sha256']
 with (P/'analysis/arm-sequencer-enrichment'/(Path(payloads[driver]['path']).name+'.arm.bin')/'instructions.tsv').open() as f:exported={int(r['address'],16):r for r in csv.DictReader(f,delimiter='\t')}
 addresses={int(r['address'],16) for r in rows}
 for row in rows:
  address=int(row['address'],16);assert exported[address]==row
  assert image[address:address+4].hex()==row['bytes']
  mnemonic,operand=row['instruction'].split(' ',1)
  if mnemonic in ('b','bl','bne','beq'):assert int(operand,16) in addresses
 for base,count in ((0x1600+driver*12,8),(0x1718+driver*12,16)):
  targets=[]
  for address in range(base,base+4*count,4):
   word=struct.unpack_from('<I',image,address)[0];assert word>>24==0xea and address in addresses
   delta=word&0xffffff
   if delta&0x800000:delta-=1<<24
   target=address+8+4*delta;assert target in addresses;targets.append(target)
  tables.append({'driver':driver,'base':base,'count':count,'targets':targets})
 for literal in (0x1620,0x1624,0x1628):assert struct.unpack_from('<I',image,literal+driver*12)[0] in addresses
provenance={'connected':connected,'verification':verification,'verification_sha256':sha(P/verification),'instruction_sites_per_driver':109,'branch_tables':tables,'classification':'Selected-library static native parser and compact-ring producer, with scripted MMIO input; all selected instruction sites exercised. No complete host event scheduling, exception/interrupt, physical input timing, synthesis or game-completion proof; no connected routine credit.'}
provenance['selected_library']='Main native sound driver' if connected else 'Standalone native module'
(P/('analysis/arm_sound_input_connected_provenance.json' if connected else 'analysis/arm_sound_input_provenance.json')).write_text(json.dumps(provenance,indent=2)+'\n')
path=P/'project_status.json';status=json.loads(path.read_text());status.setdefault('verified_progress',{})['arm_sound_input_connected' if connected else 'arm_sound_input_standalone']=provenance;path.write_text(json.dumps(status,indent=2)+'\n')
print('Main-library' if connected else 'Standalone', 'parser/producer: 109 original instruction sites per driver, 1304 parser/96 producer cases, 56 persistent checkpoints and 37 guard/fault/budget checks; no connected credit')
