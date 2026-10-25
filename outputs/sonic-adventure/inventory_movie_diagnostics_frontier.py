"""Pin the reached constructor and its static sibling dependencies."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def main():
 folder=P/'analysis/movie-diagnostics-frontier';raw_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=raw_path.read_bytes();files=[Path(__file__),raw_path,folder/'diagnostics.tsv',folder/'references.tsv',P/'ghidra/export_movie_diagnostics_frontier.ps1'];routines=[]
 for address in ('8c65a568',):
  path=folder/f'{address}_instructions.tsv';rows=list(csv.DictReader(path.open(),delimiter='\t'))
  for row in rows:
   pc=int(row['address'],16);assert raw[pc-0x8c010000:pc-0x8c010000+int(row['length'])]==bytes.fromhex(row['bytes'])
  calls=[dict(address=r['address'],instruction=r['instruction'],targets=r['flows'].strip()) for r in rows if r['instruction'].lstrip('_').startswith(('jsr ','bsr ','jmp '))]
  routines.append(dict(address='0x'+address,instruction_rows=len(rows),calls=calls));files.extend((path,folder/f'{address}_draft.c'))
 report=dict(reached_boundary='0x8c65a568',partial_parent='0x8c643624',routines=routines,new_registry_credit=0,scope='Read-only byte-checked request-state getter at the actual advancing caller frontier. No execution or new routine credit.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 (P/'analysis/movie_diagnostics_frontier.json').write_text(json.dumps(report,indent=2)+'\n');print('Pinned actual request-state frontier; no new recovery credit')
if __name__=='__main__':main()
