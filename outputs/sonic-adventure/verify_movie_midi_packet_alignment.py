"""Prove emitted alternative va_arg blocks unreachable with a four-aligned SP."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 listing=P/'analysis/movie-post-audio-controls-frontier/8c643c18_instructions.tsv'
 rows={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(listing.open(),delimiter='\t')}
 # Check the exact prologue and every instruction that establishes an alignment
 # predicate or restores its just-written pointer. These checks bind the algebra
 # to the actual byte-checked listing, rather than assuming a C ABI from a draft.
 blocks={
 0x8c643c18:['mov.l r14,@-r15','mov.l r13,@-r15','mov.l r12,@-r15','mov.l r11,@-r15','sts.l PR,@-r15','add #-0xc,r15'],
 0x8c643c56:['mov r15,r11','add #0x20,r11','add #0x1,r11','mov #0x3,r6','tst r6,r11','bt 0x8c643c6a','mov r15,r11','add #0x20,r11','bra 0x8c643c70','add #0x4,r11'],
 0x8c643c70:['mov r11,r7','add #0x4,r7','tst r6,r7','bt/s 0x8c643c80','mov.l r7,@r15'],
 0x8c643c80:['mov.l @r15,r7','mov r7,r3','add #-0x4,r3','tst r6,r3','bt/s 0x8c643c92','mov.l r3,@(0x4,r15)'],
 0x8c643c92:['mov.l @(0x4,r15),r2'],
 0x8c643cae:['mov r7,r5','add #0x4,r5','tst r6,r5','bt/s 0x8c643cbe','mov.l r5,@r15'],
 0x8c643cbe:['mov.l @r15,r5','mov r5,r12','add #-0x4,r12','tst r6,r12','bt/s 0x8c643cd0','mov.l r12,@(0x4,r15)'],
 0x8c643cd0:['mov.l @(0x4,r15),r12','mov.l @r12,r12']}
 for start,texts in blocks.items():
  for i,text in enumerate(texts):assert rows[start+2*i]==text,(hex(start+2*i),text,rows[start+2*i])
 assert not any(text.startswith(('jsr ','bsr ')) for pc,text in rows.items() if pc<0x8c643ce2)
 # Residues are exact modulo4 for every uint32 SP, including wrapping arithmetic.
 # Five pushes and12localbytes preserve residue0. No external call can alter
 # these values before argument extraction. A fault terminates earlier execution.
 entry_residue=0;local_residue=(entry_residue-5*4-12)%4
 candidate=(local_residue+32+1)%4;assert candidate==1
 r11=(local_residue+32+4)%4;assert r11==0
 r7=(r11+4)%4;assert r7==0
 r3=(r7-4)%4;assert r3==0
 r5=(r7+4)%4;assert r5==0
 r12=(r5-4)%4;assert r12==0
 impossible_edges=[(0x8c643c60,0x8c643c6a),(0x8c643c76,0x8c643c7a),(0x8c643c88,0x8c643c8c),(0x8c643cb4,0x8c643cb8),(0x8c643cc6,0x8c643cca)]
 impossible_rows={start+2*i for _,start in impossible_edges for i in range(3)}
 direct=P/'analysis/movie_midi_packet_verification.json';d=json.loads(direct.read_text())
 assert d['aligned_stack_covered_rows']==147 and set(map(lambda x:int(x,16),d['aligned_stack_missing_rows']))==impossible_rows
 assert {tuple(int(x,16) for x in row) for row in d['aligned_stack_missing_edges']}==set(impossible_edges)
 manifest=P/'analysis/movie_midi_packet_translation.json';t=json.loads(manifest.read_text());body=set(int(a,16) for a in t['routines'][0]['reachable_addresses'])
 assert len(body)==162 and len(impossible_rows)==15 and impossible_rows<=body
 # Confirm the alternative blocks have no other explicit incoming branch.
 for pc,text in rows.items():
  if text.startswith(('bt ','bt/s ','bf ','bf/s ','bra ')):
   target=int(text.split()[-1],16)
   if target in impossible_rows:assert (pc,target) in impossible_edges
 files=[Path(__file__),listing,direct,manifest]
 r=dict(aligned_entry_sp_modulus=4,aligned_entry_sp_residue=0,prologue_bytes=32,abi_rows_executed=147,abi_conditional_outcomes_executed=17,impossible_abi_rows=[hex(a) for a in sorted(impossible_rows)],impossible_abi_edges=[[hex(a),hex(b)] for a,b in impossible_edges],whole_body_rows=162,whole_body_conditional_outcomes=22,experimental_unaligned_cases=36,sha256={str(p):digest(p) for p in files},scope='Exact emitted pointer arithmetic proves15alternative rows/5outcomes unreachable for every four-aligned entrySP,until any earlier access fault terminates. Other147rows/17outcomes execute with aligned stack. Separate36experimental byte-backed RAM fixtures execute all alternative instructions;they do not establish hardware SH4address-error behavior. Original alternatives remain intact in native source;no unreachable row is removed or replaced.')
 (P/'analysis/movie_midi_packet_alignment_verification.json').write_text(json.dumps(r,indent=2)+'\n');print('Aligned ABI147rows17outcomes;proved15rows5outcomes unreachable;nohardwareunalignedclaim')
if __name__=='__main__':main()
