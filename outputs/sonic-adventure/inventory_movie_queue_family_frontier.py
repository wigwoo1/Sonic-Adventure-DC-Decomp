"""Pin the next transfer-start/callback evidence; drafts earn no credit."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def main():
 folder=P/'analysis/movie-queue-hal';rp=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=rp.read_bytes();files=[Path(__file__),rp,folder/'diagnostics.tsv',folder/'references.tsv',P/'ghidra/export_movie_queue_hal.ps1'];routines=[]
 for address in ('8c10faa4','8c668f40'):
  path=folder/f'{address}_instructions.tsv';rows=list(csv.DictReader(path.open(),delimiter='\t'))
  for row in rows:
   pc=int(row['address'],16);assert raw[pc-0x8c010000:pc-0x8c010000+int(row['length'])]==bytes.fromhex(row['bytes'])
  calls=[dict(address=r['address'],instruction=r['instruction'],targets=r['flows'].strip()) for r in rows if r['instruction'].lstrip('_').startswith(('jsr ','bsr ','jmp '))]
  routines.append(dict(address='0x'+address,instruction_rows=len(rows),calls=calls));files.extend((path,folder/f'{address}_draft.c'))
 report=dict(reached_boundaries={'cold_valid_driver':'0x8c10faa4','warm_missing_file':'0x8c64bddc','mode_zero_direct':'0x8c669088'},partial_parents=['0x8c666d10','0x8c666c2c'],callback_pointer='0x8c668f40',routines=routines,new_registry_credit=0,scope='Read-only byte-checked next transfer-start and callback evidence. The actual next cold boundary is 10faa4 after the original queue operation and descriptor setup. Mode-zero submission retains 669088; warm retains 64bddc. Drafts have no recovery or original/native execution credit. No asynchronous completion, hardware or callback return is supplied.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 (P/'analysis/movie_queue_family_frontier.json').write_text(json.dumps(report,indent=2)+'\n');print('Pinned next transfer-start/callback drafts; zero credit')
if __name__=='__main__':main()
