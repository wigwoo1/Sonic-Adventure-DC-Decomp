"""Check the unchanged R11 invariant behind one impossible branch outcome."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def main():
 paths=[P/'analysis/movie-disc-paths/8c64573e_instructions.tsv',P/'analysis/movie-disc-paths-lower/8c645374_instructions.tsv',P/'analysis/boot-file-ascii/8c645348_instructions.tsv']
 raw_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=raw_path.read_bytes();bodies=[]
 for path in paths:
  body={}
  for r in csv.DictReader(path.open(),delimiter='\t'):
   a=int(r['address'],16);assert raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(r['bytes']);body[a]=r['instruction'].lstrip('_')
  bodies.append(body)
 rows,equal,upper=bodies
 expected={0x8c6457f0:'mov r11,r0',0x8c6457f2:'cmp/eq #0x1,r0',0x8c6457f4:'bt 0x8c645860',0x8c64581a:'bsr 0x8c645374',0x8c645850:'mov r11,r0',0x8c645852:'cmp/eq #0x1,r0',0x8c645854:'bt 0x8c645858',0x8c645864:'add #0x1,r11'}
 for a,t in expected.items():assert rows[a]==t
 assert all('r11' not in t for t in equal.values()) and all('r11' not in t for t in upper.values())
 equal_calls=[t for t in equal.values() if t.startswith(('bsr ','jsr ','jmp '))]
 assert equal_calls==['bsr 0x8c645348','bsr 0x8c645348'] and not any(t.startswith(('bsr ','jsr ','jmp ')) for t in upper.values())
 # Control-transfer nodes collapse their delay slots. This is sufficient for
 # the dominance/corridor facts, and every slot in the corridor is separately
 # checked for writes to R11 below. Normal call returns are the declared ABI.
 graph={}
 for pc,text in rows.items():
  if text.startswith(('bt/s ','bf/s ')):successors=(int(text.split()[-1],16),pc+4)
  elif text.startswith(('bt ','bf ')):successors=(int(text.split()[-1],16),pc+2)
  elif text.startswith('bra '):successors=(int(text.split()[-1],16),)
  elif text.startswith(('bsr ','jsr ')):successors=(pc+4,)
  elif text in ('rts','rte') or text.startswith('jmp '):successors=()
  else:successors=(pc+2,)
  graph[pc]=set(x for x in successors if x in rows)
 def reach(start,omit=None,reverse=False):
  chosen=graph
  if reverse:chosen={a:{b for b,edges in graph.items() if a in edges} for a in graph}
  visited=set();pending=[start]
  while pending:
   a=pending.pop()
   if a==omit or a in visited:continue
   visited.add(a);pending.extend(chosen[a]-visited)
  return visited
 entry=0x8c64573e;guard=0x8c6457f4;test=0x8c645850
 assert test in reach(entry) and test not in reach(entry,guard)
 assert test not in reach(0x8c645860,guard)
 corridor=reach(0x8c6457f6,guard)&reach(test,guard,True)
 slots={a+2 for a in corridor if rows[a].startswith(('bsr ','jsr ','bra ','bt/s ','bf/s '))}
 checked=corridor|slots
 assert not any(t.endswith(',r11') or t.endswith(' r11') for a,t in rows.items() if a in checked)
 assert [rows[a] for a in sorted(corridor) if rows[a].startswith(('bsr ','jsr ','jmp '))]==['bsr 0x8c645374']
 files=paths+[Path(__file__),raw_path,P/'analysis/movie_disc_paths_translation.json',P/'source/sa_movie_disc_paths.c']
 report=dict(unreachable_edge=['0x8c645854','0x8c645858'],unreachable_instruction_rows=[],guard='0x8c6457f4',guard_dominates_later_test=True,taken_guard_path_cannot_reach_test_without_rechecking_guard=True,write_free_corridor_rows=len(checked),checked_original_rows=sum(map(len,bodies)),checked_critical_rows=len(expected),callee_preserves_r11=True,
  argument='Reaching the later test requires the earlier BT to fall through, so R11 differs from one. The intervening corridor and its delay slots do not write R11. Its sole callee is the bounded comparison, whose original body and sole uppercase callee also do not reference R11. The later equal-to-one branch cannot be taken. No counter-wrap or sampled-media coincidence is used.',
  assumptions=['Immutable original instructions.','Normal original call/return ABI; faults stop rather than resume with invented results.','No asynchronous modification of CPU registers.'],
  graph_scope='Original direct control flow with delay-slot control effects collapsed; all relevant slot instructions are checked separately for register writes. Callee preservation is checked from original instruction bytes, not assumed from an injected return.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 (P/'analysis/movie_disc_paths_invariant.json').write_text(json.dumps(report,indent=2)+'\n');print('Proved one unreachable outcome; no original instruction or branch is removed')
if __name__=='__main__':main()
