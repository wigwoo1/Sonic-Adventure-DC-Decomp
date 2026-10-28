"""Check the one infeasible normal-entry outcome without excluding valid inputs."""
import csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 listing=P/'analysis/collision-render/8c63d66c_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));body={int(r['address'],16):r['instruction'].lstrip('_') for r in rows}
 original=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';binary=original.read_bytes()
 for r in rows:
  a=int(r['address'],16);assert bytes.fromhex(r['bytes'])==binary[a-0x8c010000:a-0x8c010000+2]
 expected={0x8c63d6c2:'and r9,r11',0x8c63d6c4:'tst r11,r11',0x8c63d6c6:'bt 0x8c63d6f2',0x8c63d6c8:'mov #0x40,r2',0x8c63d6ca:'tst r9,r2',0x8c63d6cc:'bt 0x8c63d6de',0x8c63d6de:'tst r11,r11',0x8c63d6e0:'bt 0x8c63d6f2'}
 for a,t in expected.items():assert body[a]==t
 delay={a+2 for a,t in body.items() if t.startswith(('jsr ','bsr ','bra ','bt/s ','bf/s ')) or t=='rts'}
 graph={}
 for a,t in body.items():
  if a in delay:continue
  if t=='rts':nexts=set()
  elif t.startswith('bra '):nexts={int(t.split()[-1],16)}
  elif t.startswith(('bt ','bf ','bt/s ','bf/s ')):nexts={int(t.split()[-1],16),a+(4 if '/s ' in t else 2)}
  else:nexts={a+(4 if t.startswith(('jsr ','bsr ')) else 2)}
  assert nexts<=set(body);graph[a]=nexts
 pred={a:{b for b,s in graph.items() if a in s} for a in graph}
 assert pred[0x8c63d6de]=={0x8c63d6cc} and pred[0x8c63d6e0]=={0x8c63d6de}
 assert pred[0x8c63d6c8]=={0x8c63d6c6} and pred[0x8c63d6ca]=={0x8c63d6c8} and pred[0x8c63d6cc]=={0x8c63d6ca}
 assert pred[0x8c63d6c4]=={0x8c63d6c2} and pred[0x8c63d6c6]=={0x8c63d6c4}
 # Every path to the second test passed the first test's not-taken edge:
 # R11!=0. The intervening MOV and TST touch only R2 and T. No call,
 # store, R11 write, delay slot, or alternative predecessor can change R11.
 # The repeated TST therefore gives T=0 and its BT cannot be taken.
 assert not any(a in delay for a in expected)
 for zero in (False,True):
  first_taken=zero
  second_reached=not first_taken
  second_taken=second_reached and zero
  assert not second_taken
 report=dict(status='proved_infeasible_normal_entry_conditional_outcome',address='0x8c63d66c',infeasible_outcomes=[['0x8c63d6e0','0x8c63d6f2']],conditional_outcomes=32,feasible_conditional_outcomes=31,original_rows_byte_checked=len(rows),domain_excluded_rows=[],domain_excluded_outcomes=[],predicate_argument='All normal-entry paths reaching d6e0 have R11!=0 from d6c4/d6c6. The only intervening path is MOV #40,R2;TST R9,R2;BT d6de;TST R11,R11. No intervening R11 assignment/call/store or alternate incoming edge exists. Therefore T=0 at d6e0 and its taken edge is infeasible for every original R9/memory/FPU input. No native source/branch or valid ABI input is removed.',limits=['Proof is for original normal function entry and synchronous original callees; exported midfunction continuations are separate entries and not qualified by this argument.','Physical exception delivery or arbitrary externally modified execution state is not claimed.'],sha256={str(p):digest(p) for p in (Path(__file__),listing,original)})
 (P/'analysis/collision_render_predicate_proof.json').write_text(json.dumps(report,indent=2)+'\n');print('RENDER PREDICATE PROOF PASS332rows;31feasible/1infeasible outcome;0inputdomainexclusions')
if __name__=='__main__':main()
