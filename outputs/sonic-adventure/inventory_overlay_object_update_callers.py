"""Byte-check next object callers and correct an earlier inferred draw label."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def main():
 module=PROJECT.parents[1]/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN';binary=module.read_bytes()
 files=[Path(__file__),module,PROJECT/'ghidra/export_overlay_object_update_callers.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java'];records=[]
 for listing in sorted((PROJECT/'analysis/overlay-object-update-callers').glob('*_instructions.tsv')):
  rows=list(csv.DictReader(listing.open(),delimiter='\t'))
  for row in rows:
   a=int(row['address'],16);assert int(row['length'])==2 and binary[a-0xc900000:a-0xc900000+2]==bytes.fromhex(row['bytes'])
  calls=[dict(address=r['address'],instruction=r['instruction'],flows=r['flows'].strip()) for r in rows if r['instruction'].lstrip('_').startswith(('jsr','bsr'))]
  records.append(dict(address='0x'+listing.name.split('_')[0],instruction_rows=len(rows),listing=listing.relative_to(PROJECT).as_posix(),call_sites=calls,status='Read-only analysis only; no recovered-C credit.'));files.append(listing)
 assert len(records)==4 and sum(r['instruction_rows'] for r in records)==1554
 update=PROJECT/'analysis/overlay-selector-initializer/0c905666_instructions.tsv';rows=list(csv.DictReader(update.open(),delimiter='\t'));assert len(rows)==236
 for row in rows:
  a=int(row['address'],16);assert binary[a-0xc900000:a-0xc900000+2]==bytes.fromhex(row['bytes'])
 assert not any(r['instruction'].lstrip('_').startswith(('jsr','bsr')) for r in rows)
 tail=next(r for r in rows if r['address']=='0c90586a');assert tail['instruction']=='bra 0x0c904e80'
 files.append(update)
 report=dict(module='AL_MAIN.BIN',routines=records,caller_instruction_rows=1554,update_instruction_rows=236,newly_recovered_entries=0,correction='0c905666 is a parameter clamp/update helper that conditionally tail-branches to morph 0c904e80. Earlier inferred draw-helper labels are superseded by these original instructions.',next_task='Recover the update helper and trace its 0c9009d0 task callback. 0c9058a0 is the draw-callback candidate with matrix/renderer calls; natural creation via 0c900100 and alternate constructor 0c9141a0 remain to be connected.',limitations=['Static calls/references and decompiler drafts do not prove scene execution or visible rendering.'],sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 (PROJECT/'analysis/overlay_object_update_callers_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
 print('1554 caller rows + 236 update rows byte-checked; prior draw label corrected; zero new recovery credit.')
if __name__=='__main__':main()
