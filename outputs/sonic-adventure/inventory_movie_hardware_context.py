"""Pin the actual context producer without claiming its startup integration."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes();folder=P/'analysis/movie-hardware-initializer';listing=folder/'8c10d5a8_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));assert len(rows)==193
 for row in rows:
  a=int(row['address'],16);n=int(row['length']);assert raw[a-0x8c010000:a-0x8c010000+n].hex()==row['bytes']
 references=P/'analysis/movie-hardware-root/references.tsv';refs=list(csv.DictReader(references.open(),delimiter='\t'));writers=[r for r in refs if r['reference_type']=='WRITE'];assert any(r['reference_from']=='8c10d5c8' and r['function']=='8c10d5a8' for r in writers)
 literals={hex(a):hex(int.from_bytes(raw[a-0x8c010000:a-0x8c010000+4],'little')) for a in (0x8c10d744,0x8c10d748,0x8c10d750)};assert literals=={'0x8c10d744':'0x8c5fbb14','0x8c10d748':'0x8c7ac7ab','0x8c10d750':'0xa05f7800'}
 files=[Path(__file__),binary,listing,folder/'8c10d5a8_draft.c',folder/'diagnostics.tsv',references,P/'analysis/movie-hardware-root/targets.tsv',P/'ghidra/ExportResourceReferences.java',P/'ghidra/export_movie_hardware_root.ps1',P/'ghidra/export_movie_hardware_initializer.ps1']
 report=dict(context_pointer='0x8c5fbb14',producer='0x8c10d5a8',write_pc='0x8c10d5c8',original_context_base='0x8c7ac7a0',instruction_rows=193,literals=literals,call_targets=sorted({'0x'+target for row in rows if row['instruction'].lstrip('_').startswith(('jsr ','bsr ')) for target in row['flows'].split()}),newly_registered_entries=0,scope='Read-only references identify the original producer of the null context in current startup prefixes. All 193 initializer instructions are byte-checked. Existing canonical initializer recovery is preserved; this export is research input for native startup integration, not evidence that initialization ran.',limitations=['The actual initializer and its descriptor/event dependencies must execute before treating the current boot fixture as initialized. No context pointer or readiness result is inserted into the continuous startup proof.'],sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_hardware_context_frontier.json').write_text(json.dumps(report,indent=2)+'\n');print('Verified context producer 8c10d5a8:',len(rows),'rows; original aligned base 8c7ac7a0; zero new credit')
if __name__=='__main__':main()
