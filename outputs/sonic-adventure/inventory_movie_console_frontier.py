"""Byte-check the actual next original caller dependency; no recovery credit."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def main():
 folder=P/'analysis/movie-console-frontier';listing=folder/'8c092fa0_instructions.tsv';draft=folder/'8c092fa0_draft.c';raw_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=raw_path.read_bytes();rows=list(csv.DictReader(listing.open(),delimiter='\t'))
 for row in rows:
  pc=int(row['address'],16);assert raw[pc-0x8c010000:pc-0x8c010000+int(row['length'])]==bytes.fromhex(row['bytes'])
 calls=[dict(address=r['address'],instruction=r['instruction'],targets=r['flows'].strip()) for r in rows if r['instruction'].lstrip('_').startswith(('jsr ','bsr ','jmp '))]
 report=dict(address='0x8c092fa0',original_rows=len(rows),calls=calls,source_proposal=False,new_registry_credit=0,scope='Original next boundary reached by all three configured initialization-caller profiles. Export/draft is research only; intended subsystem behavior and its dependencies are not inferred from its address.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in (Path(__file__),listing,draft,folder/'diagnostics.tsv',folder/'references.tsv',P/'ghidra/export_movie_console_frontier.ps1',raw_path)})
 (P/'analysis/movie_console_frontier.json').write_text(json.dumps(report,indent=2)+'\n');print('Pinned next original boundary 8c092fa0:',len(rows),'rows,',len(calls),'calls')
if __name__=='__main__':main()
