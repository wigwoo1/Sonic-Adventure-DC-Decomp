"""Checked CFG/delay-slot/register proofs for original graphics mode control."""
import collections,csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def load_rows(body,image):
 rows={int(r['address'],16):r for r in csv.DictReader(Path(body['listing']).open(),delimiter='\t')}
 for a,r in rows.items():assert image[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(r['bytes'])
 return rows
def text(rows,pc):return rows[pc]['instruction'].lstrip('_')
def graph(rows,entry,forbidden=(),required=None):
 # A virtual node executes each delay slot in its owning branch/call context.
 # A separate ordinary node handles any direct entry to the same instruction.
 edges={};payload={};queue=[(entry,0)];forbidden=set(forbidden);required=required or {}
 while queue:
  node=queue.pop()
  if node in edges:continue
  pc,owner=node;assert pc in rows,(hex(pc),hex(owner));payload[node]=pc
  if owner:
   instruction=text(rows,owner);op=instruction.split()[0]
   if op in ('rts',):destinations=[]
   elif op in ('jsr','bsr'):destinations=[owner+4]
   elif op=='bra':destinations=[int(instruction.split()[-1],16)]
   elif op in ('bt/s','bf/s'):destinations=[int(instruction.split()[-1],16),owner+4]
   else:raise AssertionError(instruction)
   destinations=[d for d in destinations if (owner,d) not in forbidden and (owner not in required or d in required[owner])];next_nodes=[(d,0) for d in destinations]
  else:
   instruction=text(rows,pc);op=instruction.split()[0]
   if op in ('rts','jsr','bsr','bra','bt/s','bf/s'):next_nodes=[(pc+2,pc)]
   elif op in ('bt','bf'):
    destinations=[int(instruction.split()[-1],16),pc+2];next_nodes=[(d,0) for d in destinations if (pc,d) not in forbidden and (pc not in required or d in required[pc])]
   else:next_nodes=[(pc+2,0)]
  edges[node]=next_nodes;queue.extend(next_nodes)
 return edges,payload
def dominators(edges,payload,entry):
 start=(entry,0);nodes=set(edges);dom={n:({payload[n]} if n==start else set(payload.values())) for n in nodes};parents=collections.defaultdict(list)
 for source,targets in edges.items():
  for target in targets:parents[target].append(source)
 changed=True
 while changed:
  changed=False
  for node in nodes-{start}:
   value=set.intersection(*(dom[p] for p in parents[node]))|{payload[node]}
   if value!=dom[node]:dom[node]=value;changed=True
 return dom
def ancestors(edges,target):
 reverse=collections.defaultdict(list)
 for a,targets in edges.items():
  for b in targets:reverse[b].append(a)
 seen=set();pending=[(target,0)]
 while pending:
  node=pending.pop()
  if node in seen:continue
  seen.add(node);pending.extend(reverse[node])
 return seen
def writes_register(instruction,register):
 # These are all opcodes in the checked four functions. Stores,comparisons,
 # branches and MACL operations do not write the named GPR.
 op=instruction.split()[0]
 assert op in {'mov','mov.l','mov.w','add','and','or','not','neg','shld','shll','shll2','shlr','cmp/eq','cmp/hi','cmp/hs','tst','bf','bf/s','bt','bt/s','bra','bsr','jsr','rts','nop','lds.l','sts.l'}
 assert '@r%d+'%register not in instruction and '@-r%d'%register not in instruction,'Unexpected update of counted register'
 if op in ('cmp/eq','cmp/hi','cmp/hs','tst','bf','bf/s','bt','bt/s','bra','bsr','jsr','rts','nop','lds.l','sts.l'):return False
 return instruction.rsplit(',',1)[-1].strip()=='r%d'%register
def reaching_writers(rows,edges,payload,target,register):
 return {payload[node] for node in ancestors(edges,target) if node in payload and writes_register(text(rows,payload[node]),register)}
def dominates(dom,payload,target,source):
 nodes=[n for n in dom if payload[n]==target];assert nodes,'Dominance query must be reachable'
 return all(source in dom[n] for n in nodes)
def main():
 mp=P/'analysis/movie_graphics_init_translation.json';manifest=json.loads(mp.read_text());retail=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';image=retail.read_bytes();bodies={int(r['address'],16):r for r in manifest['routines']}
 rows={e:load_rows(bodies[e],image) for e in (0x8c65614e,0x8c656660,0x8c657760,0x8c6574b8)};files=[Path(__file__),mp,retail]+[Path(bodies[e]['listing']) for e in rows]
 one,two,mode,child=(rows[e] for e in (0x8c65614e,0x8c656660,0x8c657760,0x8c6574b8))
 graphs={e:graph(r,e) for e,r in rows.items()};doms={e:dominators(*graphs[e],e) for e in rows}
 def literal(a):return int.from_bytes(image[a-0x8c010000:a-0x8c010000+4],'little')
 # The actual pointer builder has one normal return,postdominated by MOV0,R0.
 assert [a for a in child if text(child,a)=='rts']==[0x8c65772c]
 assert dominates(doms[0x8c6574b8],graphs[0x8c6574b8][1],0x8c65772c,0x8c65771c)
 assert text(child,0x8c65771c)=='mov #0x0,r0'
 assert not any(writes_register(text(child,a),0) for a in range(0x8c65771e,0x8c657730,2))
 assert literal(0x8c656330)==0x8c6574b8
 assert [text(one,a) for a in range(0x8c6562ec,0x8c6562fc,2)]==['mov.l 0x8c656330,r3','mov.l @(r0,r15),r7','mov.l @(0x14,r15),r6','jsr @r3','mov.l @(0xc,r15),r4','mov r0,r4','tst r4,r4','bt/s 0x8c656340']
 assert dominates(doms[0x8c65614e],graphs[0x8c65614e][1],0x8c6562fa,0x8c6562f2)
 # CountR12 is always1or2 at the ONE loop's initial pretest.
 g,p=graphs[0x8c65614e];assert dominates(doms[0x8c65614e],p,0x8c656432,0x8c656192)
 assert reaching_writers(one,g,p,0x8c656432,12)=={0x8c656192,0x8c6562e0}
 assert text(one,0x8c656192)=='mov #0x2,r12' and text(one,0x8c6562e0)=='mov #0x1,r12'
 assert [text(one,a) for a in range(0x8c65642e,0x8c656448,2)]==['mov #0x0,r2','mov.l 0x8c6564c8,r3','cmp/hs r12,r2','mov.l r13,@r4','mov.l r13,@(0x4,r4)','mov.l r13,@(0x8,r4)','mov.l r13,@(0xc,r4)','mov r5,r4','mov.l 0x8c6564cc,r9','mov.l r5,@r3','mov.l 0x8c6564d0,r5','mov.l r14,@r9','bt/s 0x8c656456']
 assert dominates(doms[0x8c65614e],p,0x8c656432,0x8c65642e)
 # Dispatcher countR11 is2,or1copiedfromR8=1;the pretest compareszero.
 g,p=graphs[0x8c657760];assert dominates(doms[0x8c657760],p,0x8c6577c0,0x8c65777c)
 assert reaching_writers(mode,g,p,0x8c6577c0,11)=={0x8c65777c,0x8c6577ba}
 assert text(mode,0x8c65777c)=='mov #0x2,r11' and text(mode,0x8c6577b8)=='mov #0x1,r8' and text(mode,0x8c6577ba)=='mov r8,r11'
 assert dominates(doms[0x8c657760],p,0x8c6577ba,0x8c6577b8)
 assert [text(mode,a) for a in range(0x8c6577bc,0x8c6577c4,2)]==['mov #0x0,r10','mov r10,r3','cmp/hs r11,r3','bt/s 0x8c657828']
 assert all(not (0>=count) for count in (1,2)) and (0&0)==0
 # Decode the two unsigned comparisons,independently of textual register names.
 for table,pc,n,m in ((one,0x8c656432,2,12),(mode,0x8c6577c0,3,11)):
  opcode=int.from_bytes(bytes.fromhex(table[pc]['bytes']),'little');assert opcode&0xf00f==0x3002 and (opcode>>8)&15==n and (opcode>>4)&15==m
 forbidden={0x8c65614e:[(0x8c6562fa,0x8c6562fe),(0x8c656446,0x8c656456)],0x8c657760:[(0x8c6577c2,0x8c657828)]}
 unreachable={}
 for entry,edges in forbidden.items():
  _,payload=graph(rows[entry],entry,edges);missing=set(rows[entry])-set(payload.values());unreachable[hex(entry)]=[hex(a) for a in sorted(missing)]
 assert unreachable=={'0x8c65614e':['0x8c6562fe','0x8c656300'],'0x8c657760':[]}
 # Preserve the original deferred live-in R12 return,not a guessed zero result.
 g,p=graph(two,0x8c656660,required={0x8c656686:[0x8c65668a],0x8c6567ce:[0x8c6567de]})
 assert not reaching_writers(two,g,p,0x8c6567de,12) and text(two,0x8c6567de)=='mov r12,r0'
 direct_path=P/'analysis/movie_graphics_mode_controls_verification.json';direct=json.loads(direct_path.read_text());files.append(direct_path)
 for entry,edges in forbidden.items():
  actual=next(r for r in direct['coverage_by_routine'] if r['address']==hex(entry));assert actual['missing_outcomes']==[[hex(a),hex(b)] for a,b in edges] and actual['missing_rows']==unreachable[hex(entry)]
 for entry in (0x8c656660,0x8c657760):
  live=[r for r in direct['cases'] if r['input']['entry']==entry and 'r12_value' in r['input']['frame_fixture']]
  assert {r['input']['frame_fixture']['r12_value'] for r in live}=={0,1,0x12345678,0xffffffff}
  assert all(r['r0']==r['input']['frame_fixture']['r12_value'] for r in live)
 report=dict(proven_infeasible_edges={hex(e):[[hex(a),hex(b)] for a,b in edges] for e,edges in forbidden.items()},proven_infeasible_rows=unreachable,own_infeasible_rows=2,own_infeasible_outcomes=3,retail_instruction_rows_checked=sum(map(len,rows.values())),pointer_builder_normal_return_r0=0,callee_saved_count_domains={'ONE_R12':[1,2],'MODE_R11':[1,2]},deferred_two_returns_incoming_r12=True,full_delay_slot_cfg_checked=True,sha256={str(p):digest(p) for p in files},assumptions=['Full original entry with immutable checked instruction/literal bytes.','Normal callees preserve SH4callee-saved R8..R15;valid mapped distinct caller stack/data and no asynchronous register/memory mutation.','Faulting/missing-service calls stop before the caller normal-return continuation;no helper result is supplied at runtime.'],scope='Explicit delay-slot CFG/dominators,register-write sets,checked epilogues,literals and raw unsigned compare opcodes establish three infeasible outcomes andtwo deadreturn rows. Two nonempty-loop pretests comparezero againstcount1or2;actual pointer builder normalreturnsR0zero. Removing onlytheseedges yields exactlytwo unreachable instructions. A separate restricted original deferred path contains noR12assignment beforeMOVR12,R0;fourlive-in values execute onbothoriginal/native. Offlineproofonly:original/native code,branches andruntimecallbackresults unchanged.')
 (P/'analysis/movie_graphics_mode_infeasible_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Verified3infeasibleedges/2rows and originaldeferredR12return')
if __name__=='__main__':main()
