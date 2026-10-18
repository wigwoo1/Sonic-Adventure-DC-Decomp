"""Byte-check unimplemented clipper CPU-state dependencies; zero recovery credit."""
import csv,hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent;ROOT=PROJECT.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';data=binary.read_bytes()
 spec=ROOT/'work/sonic-adventure/toolchain/ghidra_12.1.4_PUBLIC/Ghidra/Processors/SuperH4/data/languages/SuperH4.sinc'
 text=spec.read_text();assert 'MACH = temp[32,32]' in text and "We don't currently simulate this behavior" in text
 reference_path=PROJECT/'analysis/sh4_clipping_architecture_reference.json';architecture=json.loads(reference_path.read_text())
 files=[Path(__file__),binary,spec,reference_path,PROJECT/'verify_startup.py',PROJECT/'source/sa_draw_primitive_stream_connected.c'];records=[]
 for name in ('8c037ee0','8c037ff4','8c038516','8c038802'):
  listing=PROJECT/f'analysis/draw-primitive-frontier/{name}_instructions.tsv';files.append(listing);selected=[]
  for r in csv.DictReader(listing.open(),delimiter='\t'):
   a=int(r['address'],16);raw=bytes.fromhex(r['bytes']);assert data[a-0x8c010000:a-0x8c010000+2]==raw
   t=r['instruction'].lstrip('_')
   if t.startswith(('dmuls.l ','flds ')) or t.startswith('ldc ') and t.endswith(',SR'):selected.append(dict(address=hex(a),instruction=t,bytes=r['bytes']))
  records.append(dict(entry='0x'+name,state_dependency_instructions=selected,native_recovery_credit=0))
 report=dict(routines=records,dmuls_instructions=sum(x['instruction'].startswith('dmuls.l ') for r in records for x in r['state_dependency_instructions']),sr_load_instructions=sum(x['instruction'].startswith('ldc ') for r in records for x in r['state_dependency_instructions']),native_recovery_credit=0,architecture_reference=architecture,scope='Static dependency/proof-gap inventory only. Current native bridge publishes MACL/GBR but no MACH or alternate integer-bank array. Renesas documents the missing state semantics; implementation and native/original bank/privilege proof remain pending. Ghidra explicitly says its integer-banking simulation is incomplete, so its pseudocode alone is insufficient evidence for clipping completion.',next_task='Add caller-owned MACH and integer-bank state to a new preserved native ABI and original executor, verify signed product halves and bank transitions independently, then recover/verify the four remaining clipper bodies. Preserve old ABIs/DLLs/proofs.',sha256={str(p):digest(p) for p in files})
 (PROJECT/'analysis/draw_primitive_cpu_frontier.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Primitive CPU frontier:',report['dmuls_instructions'],'DMULS /',report['sr_load_instructions'],'SR loads; static evidence only, zero recovery credit.')
if __name__=='__main__':main()
