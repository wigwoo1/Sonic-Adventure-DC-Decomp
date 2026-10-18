"""Byte-check the next distance/square-root analysis exports, without credit."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 binary_path=PROJECT.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=binary_path.read_bytes()
 files=[Path(__file__),binary_path,PROJECT/'ghidra/export_overlay_draw_distance_helpers.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java'];routines=[]
 for entry in (0x8c640128,0x8c63a9c4):
  listing=PROJECT/f'analysis/overlay-draw-distance-helpers/{entry:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'))
  for row in rows:
   a=int(row['address'],16);assert int(row['length'])==2 and binary[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(row['bytes'])
  routines.append(dict(entry=hex(entry),instruction_rows=len(rows),listing=listing.relative_to(PROJECT).as_posix()));files.append(listing)
 report=dict(routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_recovered_entries=0,scope='Read-only next-dependency exports checked against original bytes. Static analysis and draft pseudocode only; not recovered C or runtime proof.',sha256={str(p):digest(p) for p in files})
 (PROJECT/'analysis/overlay_draw_distance_helpers_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Distance/square-root analysis:',report['instruction_rows'],'byte-checked rows; zero recovery credit.')
if __name__=='__main__':main()
