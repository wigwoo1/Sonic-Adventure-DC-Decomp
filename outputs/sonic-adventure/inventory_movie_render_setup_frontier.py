"""Byte-check the actual post-renderer allocation boundary without credit."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 folder=P/'analysis/movie-render-setup-frontier';binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes()
 assert digest(binary)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 files=[Path(__file__),binary,folder/'diagnostics.tsv',P/'ghidra/export_movie_render_setup_frontier.ps1'];routines=[]
 for entry,count in ((0x8c6021d2,2),(0x8c6021d6,74)):
  listing=folder/f'{entry:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));assert len(rows)==count
  for row in rows:
   a=int(row['address'],16);n=int(row['length']);assert n==2 and raw[a-0x8c010000:a-0x8c010000+n].hex()==row['bytes']
  routines.append(dict(address=hex(entry),instruction_rows=count));files.extend((listing,folder/f'{entry:08x}_draft.c'))
 assert rows[0]['address']=='8c6021d6'
 thunk=list(csv.DictReader((folder/'8c6021d2_instructions.tsv').open(),delimiter='\t'))
 assert thunk[0]['instruction']=='bra 0x8c6021d6' and thunk[1]['instruction']=='_nop'
 notes=list(csv.DictReader((folder/'diagnostics.tsv').open(),delimiter='\t'));assert len(notes)==2 and all(n['completed']=='true' and not n['error'] for n in notes)
 report=dict(entry='0x8c6021d2',tail_target='0x8c6021d6',routines=routines,newly_registered_entries=0,
  scope='Read-only next allocation boundary: two-row thunk and 74-row free-list allocator that rounds to 32-byte units, searches a circular list, removes an exact block or splits a larger block, and returns a payload or null. Neither is connected by this inventory; no allocation result or heap-root fixture is fabricated.',sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_render_setup_frontier.json').write_text(json.dumps(report,indent=2)+'\n');print('Pinned post-renderer allocation frontier: two-row thunk plus 74-row allocator')
if __name__=='__main__':main()
