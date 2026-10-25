"""Pin the original heap-initialization caller, without replacing it."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes();folder=P/'analysis/movie-heap-caller'
 assert digest(binary)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 files=[Path(__file__),binary,P/'ghidra/export_movie_heap_caller.ps1',folder/'diagnostics.tsv'];routines=[];all_rows={}
 for entry in (0x8c010180,0x8c6024d0,0x8c02e720):
  listing=folder/f'{entry:08x}_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'))
  for row in rows:
   a=int(row['address'],16);assert int(row['length'])==2 and raw[a-0x8c010000:a-0x8c010000+2].hex()==row['bytes'];all_rows[a]=row['instruction']
  routines.append(dict(address=hex(entry),instruction_rows=len(rows)));files.extend((listing,folder/f'{entry:08x}_draft.c'))
 assert all_rows[0x8c0101c2]=='jsr @r3' and all_rows[0x8c0101c4]=='_add r2,r4'
 def literal(a):return int.from_bytes(raw[a-0x8c010000:a-0x8c010000+4],'little')
 assert literal(0x8c0102bc)==0x8c602128 and literal(0x8c0102a8)==0x100000
 notes=list(csv.DictReader((folder/'diagnostics.tsv').open(),delimiter='\t'));assert len(notes)==3 and all(n['completed']=='true' and not n['error'] for n in notes)
 references=P/'analysis/movie-heap-initialization/references/scheduler_references.tsv';files.append(references)
 refs=list(csv.DictReader(references.open(),delimiter='\t'));assert any(r['target']=='8c602128' and r['source']=='8c0101c2' and r['containing_function']=='8c010180' for r in refs)
 report=dict(caller='0x8c010180',heap_initializer='0x8c602128',call_site='0x8c0101c2',routines=routines,
  preceding_calls=['0x8c6024d0','0x8c02e720'],pool_bytes=literal(0x8c0102a8),
  pool_argument_expression='((load32(0x8c66c2f8) & 0xe0000000) | 0x0d000000) + 0xfff00000, modulo 2^32',
  next_after_heap='0x8c604702',newly_registered_entries=0,
  scope='Original byte-checked caller and preceding dependencies exported only. The original caller computes its one-MiB heap argument and invokes 8c602128 before driver setup. No partial caller is registered, no argument expression is executed as a replacement, and no heap is inserted into boot.',sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_heap_frontier.json').write_text(json.dumps(report,indent=2)+'\n');print('Pinned original initialization caller and heap call site; no recovery credit')
if __name__=='__main__':main()
