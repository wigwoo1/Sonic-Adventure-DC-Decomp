"""Byte-check nine exported draw dependencies; leave two boundaries unresolved."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__),PROJECT/'ghidra/export_overlay_draw_missing_helpers.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java'];routines=[]
 for listing in sorted((PROJECT/'analysis/overlay-draw-missing-helpers').glob('*_instructions.tsv')):
  entry=int(listing.name.split('_')[0],16);overlay=entry<0x10000000
  module=ROOT/('work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN' if overlay else 'work/sonic-adventure/extracted/1ST_READ.BIN');base=0xc900000 if overlay else 0x8c010000;binary=module.read_bytes();rows=list(csv.DictReader(listing.open(),delimiter='\t'))
  for row in rows:
   a=int(row['address'],16);assert int(row['length'])==2 and binary[a-base:a-base+2]==bytes.fromhex(row['bytes'])
  routines.append(dict(module='AL_MAIN.BIN' if overlay else '1ST_READ.BIN',entry=hex(entry),instruction_rows=len(rows),listing=listing.relative_to(PROJECT).as_posix(),call_sites=[dict(site=r['address'],instruction=r['instruction'],static_flows=r['flows'].strip()) for r in rows if r['instruction'].lstrip('_').startswith(('jsr','bsr'))]));files.extend((listing,module))
 assert len(routines)==9
 index=PROJECT/'analysis/ghidra-all/function_index.tsv';entries={int(r['address'],16) for r in csv.DictReader(index.open(),delimiter='\t')}
 assert not {0x8c5fde20,0x8c60e5e4}&entries;files.append(index)
 report=dict(routines=routines,instruction_rows=sum(r['instruction_rows'] for r in routines),newly_recovered_entries=0,unresolved_unindexed_targets=['0x8c5fde20','0x8c60e5e4'],next_task='Recover culling/predicate 8c038d00 and small render-state helper 8c043a20, then other draw dependencies. Discover valid code boundaries for the two targets missing from the snapshot; never infer a complete function merely from a neighboring index address.',scope='Read-only evidence exports checked against original binary bytes. Draft pseudocode and static call annotations add no recovered-C or runtime-coverage credit.',sha256={str(p):digest(p) for p in files})
 (PROJECT/'analysis/overlay_draw_missing_helpers_inventory.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Nine next draw helpers:',report['instruction_rows'],'byte-checked instruction rows; two unindexed targets unresolved; zero recovery credit.')
if __name__=='__main__':main()
