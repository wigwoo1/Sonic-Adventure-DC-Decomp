"""Byte-check temporary CFG proposals, retaining unresolved computed tails."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 binary_path=PROJECT.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=binary_path.read_bytes();index=PROJECT/'analysis/ghidra-all/function_index.tsv'
 indexed={int(r['address'],16) for r in csv.DictReader(index.open(),delimiter='\t')};assert not {0x8c60e5e4,0x8c5fde20}&indexed
 files=[Path(__file__),binary_path,index,PROJECT/'ghidra/export_overlay_draw_unindexed_targets.ps1',PROJECT/'ghidra/SeedCallbackFunctions.java',PROJECT/'ghidra/ExportFailedFunctionEvidence.java'];routines=[]
 for entry in (0x8c60e5e4,0x8c5fde20):
  listing=PROJECT/f'analysis/overlay-draw-unindexed-targets/{entry:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'))
  for row in rows:
   a=int(row['address'],16);assert int(row['length'])==2 and binary[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes'])
  routines.append(dict(entry=hex(entry),instruction_rows=len(rows),listing=listing.relative_to(PROJECT).as_posix(),computed_transfer_sites=[dict(site=r['address'],instruction=r['instruction'],static_flows=r['flows'].strip()) for r in rows if r['instruction'].lstrip('_').startswith(('jmp','jsr'))],boundary_status='Temporary Ghidra CFG proposal; computed transfers/callees and complete code/data ownership remain to verify.'));files.append(listing)
 report=dict(routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_recovered_entries=0,function_index_modified=False,saved_project_changes='Headless session used -readOnly and reported discarding temporary function discovery changes.',scope='Two previously unindexed static call targets temporarily disassembled as function proposals and exported. Original bytes checked; no recovered-C, exhaustive boundary, runtime-path or game-completion credit.',sha256={str(p):digest(p) for p in files})
 (PROJECT/'analysis/overlay_draw_unindexed_targets_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Temporary unindexed-target proposals:',report['instruction_rows'],'byte-checked rows; frozen index unchanged, zero recovery credit.')
if __name__=='__main__':main()
