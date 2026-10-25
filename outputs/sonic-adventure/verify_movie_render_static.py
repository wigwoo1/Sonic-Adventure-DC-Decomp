"""Prove an unreachable original configuration branch with conservative dataflow."""
import collections,csv,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];ENTRY,TEST,BRANCH,TARGET=0x8c6526bc,0x8c6527c4,0x8c6527c6,0x8c6527cc
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def signed(value,bits):return value-(1<<bits) if value&(1<<(bits-1)) else value
def main():
 listing=P/'analysis/movie-context-frontier/8c6526bc_instructions.tsv';rows={int(r['address'],16):r for r in csv.DictReader(listing.open(),delimiter='\t')};binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes()
 for pc,row in rows.items():assert raw[pc-0x8c010000:pc-0x8c010000+2].hex()==row['bytes']
 assert rows[0x8c65270e]['bytes']=='01e4' and rows[TEST]['bytes']=='4824' and rows[BRANCH]['bytes']=='0189'
 # State: PC, delayed destination, whether a returning call clobbers R4,
 # and a constant R4 (None means unknown). All conditional edges are explored;
 # no test observations are used to select paths. Delay instructions execute
 # before transfer. Every returning call conservatively destroys R4 knowledge.
 todo=collections.deque([(ENTRY,None,False,None)]);seen=set();at_test=set()
 while todo:
  pc,pending,clobber,r4=todo.popleft();state=(pc,pending,clobber,r4)
  if state in seen:continue
  seen.add(state);assert pc in rows,hex(pc)
  row=rows[pc];word=int.from_bytes(bytes.fromhex(row['bytes']),'little');text=row['instruction'].lstrip('_');mnemonic,_,operands=text.partition(' ')
  if pc==TEST:at_test.add(r4)
  if word&0xff00==0xe400:r4=signed(word&255,8)&0xffffffff
  elif '@r4+' in operands or '@-r4' in operands:r4=None
  elif (operands=='r4' or operands.endswith(',r4')) and not mnemonic.startswith(('cmp/','tst')):
   if text!='mov r4,r4':r4=None
  if pending is not None:
   if pending!=-1:todo.append((pending,None,False,None if clobber else r4))
   continue
  if word==0x000b:todo.append((pc+2,-1,False,r4))
  elif word&0xf0ff==0x400b or word>>12==0xb:todo.append((pc+2,pc+4,True,r4))
  elif word>>12==0xa:todo.append((pc+2,pc+4+signed(word&0xfff,12)*2,False,r4))
  elif word>>8 in (0x8d,0x8f):
   for target in (pc+4,pc+4+signed(word&255,8)*2):todo.append((pc+2,target,False,r4))
  elif word>>8 in (0x89,0x8b):
   for target in (pc+2,pc+4+signed(word&255,8)*2):todo.append((target,None,False,r4))
  else:
   assert word&0xf0ff!=0x402b,'Unresolved indirect tail jump requires another model';todo.append((pc+2,None,False,r4))
 assert at_test=={1},at_test
 assert BRANCH==TEST+2 and int(rows[BRANCH]['flows'].strip(),16)==TARGET
 files=[Path(__file__),listing,binary]
 report=dict(entry=hex(ENTRY),original_rows_byte_checked=len(rows),abstract_states_explored=len(seen),constant_definition='0x8c65270e',constant_r4=1,test_pc=hex(TEST),test_instruction='tst r4,r4',t_after_test=0,branch_pc=hex(BRANCH),proven_unreachable_edges=[[hex(BRANCH),hex(TARGET)]],scope='Conservative control-flow exploration of original byte-checked instructions from the routine entry. Both outcomes of every conditional are explored, branch delay slots run on each path, and returning calls clobber R4 knowledge. Every reached abstract state at the test has R4=1; TST sets T=0, so the immediately following BT cannot be taken.',preconditions=['Entry at 8c6526bc with immutable code/literals; no asynchronous register changes or branch into the function interior.','Called services may clobber R4 arbitrarily when returning. Non-returning calls cannot reach the later test.'],sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_render_static_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Proved original branch 8c6527c6 -> 8c6527cc unreachable;',len(seen),'conservative states')
if __name__=='__main__':main()
