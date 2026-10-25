"""Pin the actual driver entry reached after original heap initialization."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 folder=P/'analysis/movie-startup-driver-frontier';listing=folder/'8c604702_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'))
 assert len(rows)==255;binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes()
 assert digest(binary)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 for row in rows:
  at=int(row['address'],16);assert int(row['length'])==2 and raw[at-0x8c010000:at-0x8c010000+2].hex()==row['bytes']
 notes=list(csv.DictReader((folder/'diagnostics.tsv').open(),delimiter='\t'));assert len(notes)==1 and notes[0]['completed']=='true' and not notes[0]['error']
 files=[Path(__file__),binary,listing,folder/'diagnostics.tsv',folder/'8c604702_draft.c',P/'ghidra/export_movie_startup_driver.ps1']
 report=dict(entry='0x8c604702',instruction_rows=255,newly_registered_entries=0,
  scope='Read-only byte-checked next driver setup entry reached by original 8c010180 after its real heap initializer. Exports contain original flag/FPU/global/pool setup and calls; this inventory does not connect it, initialize a driver, supply results or award recovery credit.',sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_startup_frontier.json').write_text(json.dumps(report,indent=2)+'\n');print('Pinned original 255-row driver setup frontier; no recovery credit')
if __name__=='__main__':main()
