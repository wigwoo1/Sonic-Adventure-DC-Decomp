"""Pin the renderer configuration body reached after original initialization."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 folder=P/'analysis/movie-context-frontier';listing=folder/'8c6526bc_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));assert len(rows)==164
 binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes()
 for row in rows:
  a=int(row['address'],16);n=int(row['length']);assert raw[a-0x8c010000:a-0x8c010000+n].hex()==row['bytes']
 notes=list(csv.DictReader((folder/'diagnostics.tsv').open(),delimiter='\t'));assert len(notes)==1 and notes[0]['completed']=='true' and not notes[0]['error']
 targets=sorted({'0x'+target for row in rows if row['instruction'].lstrip('_').startswith(('jsr ','bsr ')) for target in row['flows'].split()})
 files=[Path(__file__),binary,listing,folder/'diagnostics.tsv',folder/'8c6526bc_draft.c',P/'ghidra/export_movie_context_frontier.ps1']
 report=dict(entry='0x8c6526bc',instruction_rows=164,newly_registered_entries=0,listed_call_targets=targets,scope='Read-only byte-checked renderer configuration frontier. Its research draft refers to KAMUI DisplayMode, AntiAlias and Dither options. This identifies the next dependency; it does not establish option parsing, physical video configuration, output pixels or a recovered body.',sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_context_frontier.json').write_text(json.dumps(report,indent=2)+'\n');print('Pinned next renderer configuration frontier: 164 rows; zero recovery credit')
if __name__=='__main__':main()
