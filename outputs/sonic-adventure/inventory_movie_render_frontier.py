"""Pin the next reached bookkeeping helper without claiming new recovery."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 folder=P/'analysis/movie-render-config-frontier';listing=folder/'8c653386_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));assert len(rows)==5
 binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes()
 for row in rows:
  a=int(row['address'],16);n=int(row['length']);assert raw[a-0x8c010000:a-0x8c010000+n].hex()==row['bytes']
 notes=list(csv.DictReader((folder/'diagnostics.tsv').open(),delimiter='\t'));assert len(notes)==1 and notes[0]['completed']=='true' and not notes[0]['error']
 files=[Path(__file__),binary,listing,folder/'diagnostics.tsv',folder/'8c653386_draft.c',P/'ghidra/export_movie_render_frontier.ps1']
 report=dict(entry='0x8c653386',instruction_rows=5,newly_registered_entries=0,scope='Read-only byte-checked next helper decrements its R4 argument, stores the value through its original global literal and returns zero. Its existing canonical recovery is preserved; native integration and subsequent renderer dependencies remain next work.',sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_render_frontier.json').write_text(json.dumps(report,indent=2)+'\n');print('Pinned next five-row original helper 8c653386; no recovery credit')
if __name__=='__main__':main()
