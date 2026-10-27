"""Remaining subdevice retry/sentinel paths and a byte-checked dead-edge proof."""
import csv,hashlib,itertools,json
from pathlib import Path
import movie_input_pump_fixtures as fx
v=fx.v;P=v.P;put=fx.put;REC=0x8cc68000;INFO=0x8cc69000;PORT=0x8cc6a000;RAW=0x8cc6b000

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def dead_edge_proof():
 path=P/'analysis/movie-input-pump-canonical/8c66005c_instructions.tsv';rows={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(path.open(),delimiter='\t')};binary=(v.ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 for r in csv.DictReader(path.open(),delimiter='\t'):
  a=int(r['address'],16);assert binary[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(r['bytes'])
 # Compute normal-return intra-function dominance independently from emitter.
 slots={a+2 for a,t in rows.items() if t.startswith(('bra ','bsr ','jsr ','bt/s ','bf/s ')) or t=='rts'};nodes=set(rows)-slots;graph={}
 for a in nodes:
  t=rows[a]
  if t=='rts':targets=[]
  elif t.startswith('bra '):targets=[int(t.split()[-1],16)]
  elif t.startswith(('bt ','bf ','bt/s ','bf/s ')):targets=[int(t.split()[-1],16),a+(4 if '/s ' in t else 2)]
  else:targets=[a+(4 if t.startswith(('bsr ','jsr ')) else 2)]
  assert set(targets)<=nodes,(hex(a),targets);graph[a]=set(targets)
 entry=0x8c66005c;dom={a:({entry} if a==entry else set(nodes)) for a in nodes}
 while True:
  fresh={}
  for a in nodes:
   pred=[b for b in nodes if a in graph[b]]
   fresh[a]={entry} if a==entry else {a}|set.intersection(*(dom[b] for b in pred))
  if fresh==dom:break
  dom=fresh
 assert 0x8c6600be in dom[0x8c660114]
 # If record[7]==0 at the dominating test, both paths bypass 660114:
 # present !=0 ->660116, present==0 ->66012a. Thus reaching114 implies nonzero.
 expected={0x8c6600ba:'mov.b @(0x7,r14),r0',0x8c6600bc:'tst r0,r0',0x8c6600be:'bf 0x8c6600ca',0x8c6600c0:'mov.b @r14,r2',0x8c6600c2:'tst r2,r2',0x8c6600c4:'bf 0x8c660116',0x8c6600c6:'bra 0x8c66012a',0x8c6600c8:'nop',0x8c660110:'mov.b @(0x7,r14),r0',0x8c660112:'tst r0,r0',0x8c660114:'bt 0x8c66012a'}
 for a,t in expected.items():assert rows[a]==t
 between={a for a in nodes if 0x8c6600ba<=a<=0x8c660114}
 for a in between:
  t=rows[a];assert not t.startswith(('jsr ','bsr '))
  if t.startswith('mov') and '@' in t and t.split(',')[-1]=='r14':raise AssertionError('Record pointer reassigned')
 stores=[t for a,t in rows.items() if 0x8c6600ba<=a<=0x8c660114 and t.startswith('mov') and ',@' in t]
 assert set(stores)=={'mov.b r0,@r14','mov.b r13,@r14','mov.b r0,@(0x2,r14)','mov.b r0,@(0x3,r14)','mov.b r0,@(0x4,r14)','mov.l r3,@(0x24,r14)','mov.l r13,@(0x24,r14)'}
 return dict(edge=['0x8c660114','0x8c66012a'],dominating_test='0x8c6600be',immutable_field='r14+7',no_calls_or_pointer_reassignment=True,field_stores_disjoint=True,domain='Single guest executor;no concurrent external modification of caller-owned record.',listing_sha256=digest(path),source_routine='0x8c66005c',scope='Trueedge114->12a impossible:reaching114requiresrecord[7]!=0atdominatingbe;nointerveningcall/write7/pointerchange. Other114falseedge executable;no instruction row is omitted.')

def main():
 ram=v.seed_ram();records=[];checks=0;seen=set();edges=set();proof=dead_edge_proof()
 for command,kind,retries in itertools.product((0,1,2,4),('normal','ff','ff00'),(0,3)):
  c,m,s,f=fx.setup(ram,0x8c66005c,[1,PORT,INFO,REC],0,0x400000a1);fx.configure(m);put(m,fx.CTX+0x10c4,1)
  threshold=int.from_bytes(v.sample(m,0x8c66011e,2),'little',signed=True);put(m,fx.CTX+threshold,3)
  m[REC&0xffffff:(REC&0xffffff)+44]=bytes(44);put(m,REC,1,1);put(m,REC+4,9,1);put(m,REC+7,command,1);put(m,REC+0x24,retries);m[INFO&0xffffff:(INFO&0xffffff)+120]=bytes([0xa5])*120;put(m,PORT,1,1);put(m,PORT+2,0,1);put(m,PORT+3,0,1)
  header=bytes([8,0x20,1,30]);raw=header[::-1] if kind=='normal' else (0xffffffff if kind=='ff' else 0xffffff00).to_bytes(4,'little');m[RAW&0xffffff:(RAW&0xffffff)+4]=raw;m[(RAW+4)&0xffffff:(RAW&0xffffff)+260]=bytes(range(256));put(m,s.r[15],RAW)
  before=type(s).from_buffer_copy(s);r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3;assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
  assert s.r[0]==0 and s.r[15]==before.r[15] and s.pr==s.pc==before.pr and list(s.r)[8:15]==list(before.r)[8:15];n+=4
  assert v.sample(a,REC+8,4)==raw[::-1] and v.load(a,REC+0x1c)==v.load(m,REC+0x1c) and v.load(a,REC+0x20)==v.load(m,REC+0x20);n+=3
  checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((hex(pc),hex(path[j+distance])))
  records.append(dict(command=command,kind=kind,retries=retries,status=r['status'],stop_pc=r['stop_pc'],independent_checks=n))
 assert tuple(proof['edge']) not in edges
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
 files=[Path(__file__),Path(fx.__file__),Path(v.__file__),Path(fx.fast.__file__),v.MP,P/'analysis/movie_input_pump_connection.json',v.effects.DLL,P/'analysis/movie-input-pump-canonical/8c66005c_instructions.tsv']
 report=dict(original_native_comparisons=len(records),independent_checks=checks,structurally_unreachable_edges=[proof],cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Remainingactualsubdevicepositive-retry/genericcommand/specialrawerrorword paths;independentstack/header/token preservation pluscompleteoriginal/nativecomparison. Bytechecked CFGdominance/disjoint-store proof establishesoneinfeasiblebranchwithoutremoving source/state checks. No resultstub.')
 (P/'analysis/movie_input_pump_subdevice_edges_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('INPUT PUMP SUBDEVICE EDGES PASS',len(records),checks,'oneproveninfeasibleedge')
if __name__=='__main__':main()
