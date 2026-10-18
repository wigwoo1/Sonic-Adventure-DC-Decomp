"""Byte-check remaining initializer exports; analysis only, no recovery credit."""
import collections,csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def main():
 binary_path=PROJECT.parents[1]/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN';binary=binary_path.read_bytes()
 rows=[];files=[Path(__file__),binary_path,PROJECT/'ghidra/export_overlay_initializer_dependencies.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java']
 for listing in sorted((PROJECT/'analysis/overlay-initializer-dependencies').glob('*_instructions.tsv')):
  entries=list(csv.DictReader(listing.open(),delimiter='\t'))
  for entry in entries:
   address=int(entry['address'],16)
   assert binary[address-0xc900000:address-0xc900000+2]==bytes.fromhex(entry['bytes']) and int(entry['length'])==2
  calls=[dict(address=e['address'],instruction=e['instruction'],flows=e['flows']) for e in entries if e['instruction'].lstrip('_').startswith(('jsr','bsr'))]
  floating=collections.Counter(e['instruction'].lstrip('_').split()[0] for e in entries if e['instruction'].lstrip('_').startswith('f'))
  rows.append(dict(address='0x'+listing.name.split('_')[0],exported_instruction_rows=len(entries),listing=listing.relative_to(PROJECT).as_posix(),arithmetic_and_transfer_mnemonics=dict(floating),call_sites=calls,status='Analysis inventory only; not recovered C and no new routine credit.'))
  files.append(listing)
 assert len(rows)==7 and sum(r['exported_instruction_rows'] for r in rows)==1510
 report=dict(module='AL_MAIN.BIN',load_address='0x0c900000',routines=rows,exported_instruction_rows=1510,newly_recovered_entries=0,next_task='Recover 0c904e80 from original instructions; its Ghidra draft warnings and floating-point modes require Sol High and explicit guards/tests. Other six remaining helpers are exported for dependency ordering.',sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
 (PROJECT/'analysis/overlay_initializer_dependencies_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Seven remaining helper exports byte-checked: 1510 rows; analysis-only, zero recovered credit.')
if __name__=='__main__':main()
