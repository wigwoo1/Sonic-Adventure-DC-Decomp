"""Pin genuine completion wait and retained mode-zero/panic draft frontiers."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def main():
 folder=P/'analysis/movie-transfer-frontier';retail=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=retail.read_bytes();files=[Path(__file__),retail,folder/'diagnostics.tsv',folder/'references.tsv',P/'ghidra/export_movie_transfer_frontier.ps1'];routines=[]
 for address in ('8c669088','8c666e06'):
  listing=folder/f'{address}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'))
  for row in rows:
   a=int(row['address'],16);assert raw[a-0x8c010000:a-0x8c010000+int(row['length'])]==bytes.fromhex(row['bytes'])
  routines.append(dict(address='0x'+address,instruction_rows=len(rows),calls=[dict(pc=r['address'],instruction=r['instruction'],targets=r['flows'].strip()) for r in rows if r['instruction'].lstrip('_').startswith(('jsr ','bsr ','jmp '))]));files.extend((listing,folder/f'{address}_draft.c'))
 listing=P/'analysis/movie-bulk-family/8c65a5dc_instructions.tsv';files.append(listing);rows=list(csv.DictReader(listing.open(),delimiter='\t'));wait=[r for r in rows if int(r['address'],16) in (0x8c65a6e4,0x8c65a6e6,0x8c65a6e8,0x8c65a6ea,0x8c65a6ec)]
 for r in wait:
  a=int(r['address'],16);assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(r['bytes'])
 assert len(wait)==5
 report=dict(cold_frontier='0x8c65a6ec',cold_frontier_kind='Actual completion wait after original transfer command; finite research observation, not a missing-call return',warm_frontier='0x8c64bddc',mode_zero_frontier='0x8c669088',busy_error_counter_frontier='0x8c666e06',wait_instructions=wait,routines=routines,new_registry_credit=0,scope='The actual cold path now submits a channel and waits for real completion; software DMA/IRQ acceptance is not implemented or supplied. Read-only original mode-zero and error-counter drafts remain uncredited. The newly exported five-row666e06 increments a global and returns; earlier speculative panic terminology is rejected by its original instructions. Existing completion callback is verified as an explicit ABI entry, not falsely invoked as physical interrupt delivery.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 (P/'analysis/movie_transfer_start_frontier.json').write_text(json.dumps(report,indent=2)+'\n');print('Pinned actual wait and two retained draft frontiers; zero extra credit')
if __name__=='__main__':main()
