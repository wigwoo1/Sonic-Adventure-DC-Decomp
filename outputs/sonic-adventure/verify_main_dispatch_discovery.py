"""Validate dispatcher discovery against retail bytes and all byte selectors."""
import csv,hashlib,json,struct
from pathlib import Path
from verify_startup import ROOT,State,reference
P=Path(__file__).resolve().parent
image_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';image=image_path.read_bytes();base=0x8c010000
assert hashlib.sha256(image).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
old=P/'analysis/main-failed-function-evidence/8c0a966c_instructions.tsv';enriched=P/'analysis/main-dispatch-enrichment/8c0a966c_instructions.tsv'
old_rows=list(csv.DictReader(old.open(),delimiter='\t'));rows=list(csv.DictReader(enriched.open(),delimiter='\t'))
for row in rows:
 address=int(row['address'],16);length=int(row['length']);assert image[address-base:address-base+length].hex()==row['bytes'],row
known={int(r['address'],16) for r in rows};before={int(r['address'],16) for r in old_rows}
assert before<=known
entries=list(csv.DictReader((P/'analysis/main-dispatch-enrichment/table_targets.tsv').open(),delimiter='\t'));assert len(entries)==83
targets=[]
for index,row in enumerate(entries):
 offset=struct.unpack_from('<h',image,0x8c0a979c-base+index*2)[0];target=(0x8c0a9796+offset)&0xffffffff
 assert int(row['index'])==index and int(row['signed_offset'])==offset and int(row['target'],16)==target and target in known
 targets.append(target)
# Execute the existing retail prefix only. Counter=1 selects a path with no
# external callees, ending before the selected handler executes.
records=[]
for selector in range(256):
 ram=bytearray(0x1000000);ram[0x10000:0x10000+len(image)]=image
 obj=0x8c300000
 ram[0x300000]=selector;struct.pack_into('<HH',ram,0x300004,0xffff,1)
 state=State();state.r[:]=[0x12340000+i for i in range(16)];state.r[4]=obj;state.r[5]=0x8c310000;state.r[6]=0;state.r[15]=0x8cf00000;state.pr=0x8cf10000;state.pc=0x8c0a966c;state.sr=selector&1
 target=targets[selector-1] if 1<=selector<=83 else 0x8c0ae0ac
 result=reference(ram,state,stop_pc=target,instruction_budget=500)
 actual=state
 assert actual.pc==target and actual.r[15]==0x8cefffb8
 assert struct.unpack_from('<HH',ram,0x300004)==(0xfffb,0)
 records.append({'selector':selector,'target':hex(target)})
paths=[Path(__file__),P/'verify_startup.py',image_path,old,enriched,P/'analysis/main-dispatch-enrichment/table_targets.tsv',P/'analysis/main-dispatch-enrichment/body_change.tsv',P/'ghidra/SeedMainDispatchTable.java',P/'ghidra/ExportFailedFunctionEvidence.java',P/'ghidra/enrich_main_dispatch.ps1']
report={'selector_cases_passed':len(records),'table_entries':83,'unique_targets':len(set(targets)),'old_instruction_rows':len(old_rows),'enriched_instruction_rows':len(rows),'new_instruction_rows':len(known-before),'enriched_instruction_bytes':sum(int(r['length']) for r in rows),'cases':records,'sha256':{str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},'scope':'Read-only/transient Ghidra switch override exposes original main function body. Exact instruction bytes and signed-offset BRAF target calculation checked; existing SH4 subset interpreter executes prefix for all 256 byte selectors, stopping before handler bodies. No native handler recovery, handler behavior, complete code discovery, hardware or playable-port claim.'}
(P/'analysis/main_dispatch_discovery_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(len(records),'selector prefixes passed;',len(known-before),'additional original instruction rows exposed')
