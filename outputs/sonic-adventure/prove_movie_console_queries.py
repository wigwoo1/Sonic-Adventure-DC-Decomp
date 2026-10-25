"""Original-byte range lemma for the one impossible cable-query outcome."""
import csv,hashlib,json
from pathlib import Path
from verify_startup import State,reference
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def main():
 paths=[P/'analysis/movie-initialize-queries/8c10c044_instructions.tsv',P/'analysis/movie-initialize-caller-lower/8c10c07e_instructions.tsv']
 raw_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=raw_path.read_bytes();bodies=[]
 for path in paths:
  body={}
  for row in csv.DictReader(path.open(),delimiter='\t'):
   pc=int(row['address'],16);assert raw[pc-0x8c010000:pc-0x8c010000+2]==bytes.fromhex(row['bytes']);body[pc]=row['instruction'].lstrip('_')
  bodies.append(body)
 region,query=bodies
 def graph(rows):
  edges={}
  for pc,text in rows.items():
   if text.startswith(('bt ','bf ')):nexts=(int(text.split()[-1],16),pc+2)
   elif text.startswith(('bt/s ','bf/s ')):nexts=(int(text.split()[-1],16),pc+4)
   elif text.startswith('bra '):nexts=(int(text.split()[-1],16),)
   elif text.startswith(('bsr ','jsr ')):nexts=(pc+4,)
   elif text=='rts' or text.startswith('jmp '):nexts=()
   else:nexts=(pc+2,)
   edges[pc]={n for n in nexts if n in rows}
  return edges
 def reachable(edges,start,omit=None):
  seen=set();pending=[start]
  while pending:
   pc=pending.pop()
   if pc==omit or pc in seen:continue
   seen.add(pc);pending.extend(edges[pc]-seen)
  return seen
 rg=graph(region);qg=graph(query)
 returns=[pc for pc,text in region.items() if text=='rts'];assert returns==[0x8c10c07a]
 assert returns[0] not in reachable(rg,0x8c10c044,0x8c10c052)
 assert region[0x8c10c052]=='mov r0,r4' and region[0x8c10c054]=='mov.b @(0xc,r4),r0'
 assert region[0x8c10c07c]=='mov r4,r0'
 # Execute the actual post-call suffix for every possible byte. This is a
 # range lemma, not a substituted return in the full-function comparisons.
 ram=bytearray(16<<20);ram[0x10000:0x10000+len(raw)]=raw
 header,stack,stop=0x8cda0000,0x8ceff000,0x8cfed000;ram[stack&0xffffff:(stack&0xffffff)+4]=stop.to_bytes(4,'little');values=set()
 for byte in range(256):
  ram[(header+12)&0xffffff]=byte;s=State();s.pc=0x8c10c052;s.r[0]=header;s.r[15]=stack
  reference(ram,s,stop_pc=stop,instruction_budget=64)
  expected=byte-ord('0') if byte in (ord('1'),ord('2'),ord('3')) else 0
  assert s.r[0]==expected and s.pc==stop;values.add(s.r[0])
 assert values=={0,1,2,3}
 expected={0x8c10c088:'bsr 0x8c10c044',0x8c10c08e:'mov r0,r4',0x8c10c09c:'mov r4,r0',0x8c10c09e:'cmp/eq #0x1,r0',0x8c10c0a0:'bt 0x8c10c0b2',0x8c10c0a2:'cmp/eq #0x3,r0',0x8c10c0a4:'bt 0x8c10c0b2',0x8c10c0a6:'cmp/eq #0x2,r0',0x8c10c0a8:'bt 0x8c10c0ba',0x8c10c0aa:'cmp/eq #0x0,r0',0x8c10c0ac:'bt 0x8c10c0ba',0x8c10c0ae:'bra 0x8c10c0ba',0x8c10c0b0:'nop'}
 for pc,text in expected.items():assert query[pc]==text
 assert 0x8c10c0ac not in reachable(qg,0x8c10c07e,0x8c10c08e)
 corridor={pc:text for pc,text in query.items() if 0x8c10c08e<pc<=0x8c10c0ac}
 assert not any(text.endswith(',r4') or text.startswith(('bsr ','jsr ','jmp ')) for text in corridor.values())
 remaining=values-{1,3,2};assert remaining=={0}
 files=paths+[Path(__file__),P/'verify_startup.py',P/'analysis/movie_initialize_translation.json',P/'source/sa_movie_initialize.c',raw_path]
 report=dict(region_return_values=sorted(values),exhaustive_original_suffix_inputs=256,checked_original_rows=sum(map(len,bodies)),region_suffix_dominates_normal_return=True,query_callee_result_dominates_test=True,unreachable_instruction_rows=['0x8c10c0ae','0x8c10c0b0'],unreachable_edge=['0x8c10c0ac','0x8c10c0ae'],argument='All normal region-helper returns are 0/1/2/3. The query preserves that result while eliminating 1, 3 and 2 before its final equality-to-zero check. Reaching that check leaves only zero; its false outcome and the following branch/delay slot cannot execute. Every original instruction and branch remains in C.',assumptions=['Immutable original instructions and normal call/return ABI.','Invalid reads or failed host services stop execution.','No asynchronous CPU-register changes or stack corruption.'],scope='Byte-checked original CFG domination and exhaustive execution of the original post-call byte-classification suffix. This range lemma is separate from full-function execution and does not substitute a helper/BIOS result in any integration case.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 (P/'analysis/movie_console_query_invariant.json').write_text(json.dumps(report,indent=2)+'\n');print('Proved one impossible query outcome and two unreachable rows; all original code retained')
if __name__=='__main__':main()
