"""Pin both distinct caller frontiers without execution/recovery credit."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def main():
 folder=P/'analysis/movie-request-fields-frontier';raw_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=raw_path.read_bytes();files=[Path(__file__),raw_path,folder/'diagnostics.tsv',folder/'references.tsv',P/'ghidra/export_movie_request_fields_frontier.ps1'];routines=[]
 for address in ('8c64bddc','8c65a200'):
  path=folder/f'{address}_instructions.tsv';rows=list(csv.DictReader(path.open(),delimiter='\t'))
  for row in rows:
   pc=int(row['address'],16);assert raw[pc-0x8c010000:pc-0x8c010000+int(row['length'])]==bytes.fromhex(row['bytes'])
  calls=[dict(address=r['address'],instruction=r['instruction'],targets=r['flows'].strip()) for r in rows if r['instruction'].lstrip('_').startswith(('jsr ','bsr ','jmp '))]
  routines.append(dict(address='0x'+address,instruction_rows=len(rows),calls=calls));files.extend((path,folder/f'{address}_draft.c'))
 report=dict(reached_boundaries={'cold_valid_driver':'0x8c65a200','warm_missing_file':'0x8c64bddc'},partial_parents={'cold_valid_driver':'0x8c643e6c','warm_missing_file':'0x8c5ff31e'},routines=routines,new_registry_credit=0,scope='Read-only byte-checked evidence for the distinct valid-driver cold path and missing-file warm control. The cold routine changes the interrupt mask, calls two original services, sets a word bit and counts a delay. The warm routine calls configuration/status services. Drafts alone establish no completed recovery or execution credit; nested services remain unsubstituted.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 (P/'analysis/movie_request_fields_frontier.json').write_text(json.dumps(report,indent=2)+'\n');print('Pinned both caller frontiers; no new recovery credit')
if __name__=='__main__':main()
