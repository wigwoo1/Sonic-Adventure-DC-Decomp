"""Motion globals/recursive base transforms against original complete states."""
import collections,csv,ctypes,hashlib,itertools,json,random,sys
from fractions import Fraction
from pathlib import Path
import sh4_binary32 as arithmetic
import verify_overlay_draw_distance as previous
import verify_overlay_draw_state as state
from verify_startup import ROOT,State
from verify_fpu_cleanup_transfer import Fpu
from verify_main_dispatch_active_query import OBSERVE
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
SETUP,DRIVER=0x8c60e5e4,0x8c60e31c
BASE=(0x8c60cfc4,0x8c60cfe2,0x8c60cffc,0x8c60d016);FORMAT=0x8c60d034;NOOP=0x8c611708
SAVE,RESTORE=0x8c605dac,0x8c605e0a;ROTATE={0x8c639cf4,0x8c639ff8}
NODE,DESC,DATA,MATRIX,STACK,STATE_A=0x8cc00000,0x8cc20000,0x8cc30000,0x8cc40000,0x8ce00000,0x8ce80000
MOTION,KIND,TYPE,DUAL=0x8c88fe40,0x8c88fe78,0x8c88fe7a,0x8c890028
MANIFEST=PROJECT/'analysis/draw_motion_setup_translation.json'
manifest=json.loads(MANIFEST.read_text());NEW={int(r['address'],16) for r in manifest['routines']}
CONNECTED=previous.CONNECTED|NEW|{SAVE,RESTORE,NOOP}|ROTATE
ROWS=dict(previous.ROWS);OWN={}
for r in manifest['routines']:
 for row in csv.DictReader(Path(r['listing']).open(),delimiter='\t'):
  a=int(row['address'],16);OWN[a]=row['instruction'].lstrip('_');ROWS[a]=OWN[a]
for a in (SAVE,RESTORE,*ROTATE):
 for row in csv.DictReader((PROJECT/f'analysis/draw-motion-setup/{a:08x}_instructions.tsv').open(),delimiter='\t'):ROWS[int(row['address'],16)]=row['instruction'].lstrip('_')
ROWS[NOOP]='rts';ROWS[NOOP+2]='nop'
RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
LEGACY=previous.namespace['LEGACY']|ROTATE
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=LEGACY)
code=previous.code.replace('pc in MISSING_TARGETS','pc not in ROWS')
exec(compile(code,'motion_setup_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=ROOT/'work/sonic-adventure/build/draw_motion_setup.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_motion_setup_connected
fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def graph(c):
 if c.get('graph')=='tree':return [(1,2),(None,None),(3,None),(None,None)]
 return [(None,None)]
def prepare(ram,c):
 m=bytearray(ram);rng=random.Random(SETUP);s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=NODE;s.r[5]=0 if c.get('null_motion') else DESC;s.r[15]=0x8cf00000;s.pr=0x8cfed000;s.pc=c.get('entry',SETUP);s.sr=0x700003f2
 for i,(child,sibling) in enumerate(graph(c)):
  node=NODE+i*0x100;flags=c.get('flags',0x40)|(0x20 if c.get('order') else 0)
  store(m,node,flags);store(m,node+4,0x8cc50000 if c.get('mesh') else 0)
  for j,v in enumerate(((i+1)*.25,-.5,.75)):store(m,node+8+j*4,state.gate.support.morph.bits(v))
  for j in range(3):store(m,node+0x14+j*4,0)
  for j,v in enumerate((.5,1,2)):store(m,node+0x20+j*4,state.gate.support.morph.bits(v))
  store(m,node+0x2c,NODE+child*0x100 if child is not None else 0);store(m,node+0x30,NODE+sibling*0x100 if sibling is not None else 0)
 store(m,DESC,DATA);store(m,DESC+4,c.get('count',8));store(m,DESC+8,c.get('type',0),2);store(m,DESC+10,c.get('format',0)|c.get('dual',0),2)
 for i in range(7):store(m,MOTION+i*4,0xa5000000+i*0x1111)
 store(m,MOTION+4,DATA+0x1000);store(m,MOTION+8,DATA+0x2000)
 store(m,KIND,0xaabbccdd);store(m,DUAL,0x11223344)
 for a in (0x8c88fe24,0x8c88fe2c,0x8c88fe30,0x8c88fe34,0x8c89001c,0x8c890020):store(m,a,0x13579bdf)
 store(m,0x8c88fcb4,STATE_A)
 for i in range(4):store(m,STATE_A+0x90+i*4,0x12345600+i)
 store(m,0x8c88fcd4,STATE_A+0x100)
 for i in range(5):store(m,STATE_A+0x100+i*4,0xabc00000+i)
 store(m,0x8c88f62c,0);store(m,0x8c88f698,16);store(m,0x8c88f69c,1);store(m,0x8c88f5f8,STACK)
 store(m,0x8c890074,NOOP if c.get('optional') else 0)
 for i in range(16):store(m,MATRIX+i*4,state.gate.support.morph.bits(1 if i%5==0 else 0))
 f=Fpu();f.fpscr=0x40000|(c.get('mode',c.get('bank',0)*4)<<19)|c.get('rm',0)|c.get('enable',0);f.fpul=0xabcdef12
 for b in range(2):
  for i in range(16):f.banks[b][i]=state.gate.support.morph.bits((i+1)/8)
 bank=f.fpscr>>21&1;f.banks[bank^1][:]=[state.gate.support.morph.bits(1 if i%5==0 else 0) for i in range(16)]
 f.banks[bank][4]=state.gate.support.morph.bits(c.get('frame',.5))
 if s.pc!=SETUP:
  store(m,KIND,0,2);store(m,DUAL,0,2);store(m,TYPE,0,2);store(m,MOTION+0x18,MATRIX if c.get('explicit') else 0)
  for a,v in ((0x8c88fe24,FORMAT),(0x8c88fe2c,BASE[0]),(0x8c88fe30,BASE[2] if c.get('order') else BASE[1]),(0x8c88fe34,BASE[3])):store(m,a,v)
 if c.get('bad')=='root':s.r[4]=0
 if c.get('bad')=='motion':s.r[5]=0x8cfffffc
 if c.get('bad')=='stack':s.r[15]=0
 return m,s,f

def setup_oracle(m,c):
 out=bytearray(m)
 if c.get('null_motion'):kind=typ=dual=0
 else:
  n=c.get('count',8);signed=n if n<0x80000000 else n-0x100000000;rm=c.get('rm',0)
  raw=arithmetic.round_dyadic(signed,0,rm)[0]
  if signed<0:raw=arithmetic.add_sub(raw,0x4f800000,False,rm)[0]
  if not state.gate.support.morph.value(raw)>state.gate.support.morph.value(state.gate.support.morph.bits(c.get('frame',.5))):return out,False
  kind=c.get('format',0);typ=c.get('type',0);dual=int(bool(c.get('dual',0)&0xc0))
  store(out,MOTION,DATA);store(out,MOTION+16,n)
 for a,v in ((KIND,kind),(TYPE,typ),(DUAL,dual)):store(out,a,v,2)
 store(out,MOTION+12,state.gate.support.morph.bits(c.get('frame',.5)));store(out,MOTION+20,0);store(out,MOTION+24,0)
 store(out,0x8c88fe24,load(m,0x8c673540+kind*4))
 store(out,0x8c88fe2c,0x8c60c322 if typ&1 else BASE[0])
 store(out,0x8c88fe30,(0x8c60c526 if typ&2 else BASE[2]) if c.get('order') else (0x8c60c488 if typ&2 else BASE[1]))
 store(out,0x8c88fe34,0x8c60c3b8 if typ&4 else BASE[3])
 for a,t in ((0x8c89001c,0x8c673564),(0x8c890020,0x8c67356c)):store(out,a,load(m,t+dual*4))
 return out,True

def matrix_map(m,c):
 # Exact diagonal scales and dyadic positions, zero rotations. This checks
 # world transforms independently of the native/vector instruction adapters.
 out={}
 def visit(i,scale,translation):
  while i is not None:
   a=NODE+i*0x100;local=[Fraction(state.gate.support.morph.value(load(m,a+8+j*4))) for j in range(3)];s=[Fraction(state.gate.support.morph.value(load(m,a+0x20+j*4))) for j in range(3)]
   ws=[x*y for x,y in zip(scale,s)];wt=[p+x*y for p,x,y in zip(translation,scale,local)]
   out[a]=[ws[k//4] if k<12 and k%4==k//4 else wt[k-12] if 12<=k<15 else Fraction(1) if k==15 else Fraction(0) for k in range(16)]
   child,sibling=graph(c)[i]
   if child is not None:visit(child,ws,wt)
   i=sibling
 visit(0,[Fraction(1)]*3,[Fraction(0)]*3);return out

def cases():
 for kind,typ,order,bank in itertools.product(range(9),range(8),range(2),range(2)):
  yield dict(format=kind,type=typ,order=order,bank=bank,semantic=True)
 for kind,dual,bank in itertools.product(range(9),(0x40,0x80,0xc0),range(2)):
  yield dict(format=kind,dual=dual,bank=bank,type=7,semantic=True)
 for count,frame,bank,rm in itertools.product((0,1,0x7fffffff,0x80000000,0xffffffff),(-1,.5,4294967296),range(2),range(2)):
  yield dict(count=count,frame=frame,bank=bank,rm=rm,semantic=True)
 for flags,order,bank,optional,tree in itertools.product((0,0x40),range(2),range(2),(False,True),('single','tree')):
  yield dict(null_motion=True,flags=flags,order=order,bank=bank,optional=optional,graph=tree,semantic=True)
 for flags,mesh,optional in itertools.product((0,8,0x40,0x48),(False,True),(False,True)):
  yield dict(null_motion=True,flags=flags,mesh=mesh,optional=optional,semantic=True)
 for entry,explicit,order,bank in itertools.product(BASE,(False,True),range(2),range(2)):
  yield dict(entry=entry,explicit=explicit,order=order,bank=bank,semantic=True)
 for mode,entry in itertools.product(range(8),(SETUP,DRIVER)):
  yield dict(entry=entry,mode=mode,null_motion=True)
 for bad,bank in itertools.product(('root','motion','stack'),range(2)):yield dict(bad=bad,bank=bank)
 for bank,rm,enable in itertools.product(range(2),range(4),(0x80,0x800)):
  yield dict(bank=bank,rm=rm,enable=enable,count=0xffffffff,probe=True)
 # Interpolation is an explicit boundary. These cases test callback selection,
 # not fake interpolation output or successful drawing.
 for kind,typ,bank in itertools.product((0,1,8),(0,1,2,4),range(2)):
  yield dict(format=kind,type=typ,bank=bank,flags=0,semantic=True)

def main():
 connection=PROJECT/'analysis/draw_motion_setup_connection.json';proofs=[MANIFEST,connection,PROJECT/'analysis/overlay_draw_distance_verification.json',PROJECT/'analysis/draw_distance_helpers_verification.json']
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 binary=state.gate.support.morph.MAIN.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(binary)]=binary;assert ram[NOOP&0xffffff:(NOOP&0xffffff)+4]==bytes.fromhex('0b000900')
 selected=list(cases());records=[];coverage=set();edges=set();targets=collections.Counter();boundaries=0;independent=0;world_checks=0
 if '--smoke' in sys.argv:selected=[dict(null_motion=True,flags=0,semantic=True),dict(format=8,type=7,dual=0xc0,order=1,semantic=True),dict(null_motion=True,flags=0x40,graph='tree',optional=True,semantic=True),dict(frame=8,semantic=True),dict(null_motion=True,mesh=True,flags=0,semantic=True)]
 for number,c in enumerate(selected):
  m,s,f=prepare(ram,c);entry=s.pc;other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m);oracle=None;accepted=False;world=matrix_map(m,c)
  if c.get('semantic') and entry==SETUP:oracle,accepted=setup_oracle(m,c)
  status,path,wanted,expected_macl=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED));got=[];errors=[];checks=[];ml=ctypes.c_uint32(0x12345678)
  @OBSERVE
  def observe(ctx,target,mem,fp,sr,r,pr):
   try:
    got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,hashlib.sha256(ctypes.string_at(ctypes.addressof(mem.contents),16<<20)).digest()))
    if target==0x8c639b98 and c.get('semantic') and r[14] in world:
     bank=fp.contents.fpscr>>21&1;values=[Fraction(state.gate.support.morph.value(x)) for x in fp.contents.banks[bank^1]];assert values==world[r[14]],(c,'independent pre-pop world matrix',hex(r[14]));checks.append(True)
   except Exception as exc:errors.append(repr(exc))
  rc=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),state.gate.support.morph.P),ctypes.byref(ml),entry,None,observe)
  assert not errors and rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full original motion state',rc,status,hex(s.pc),hex(other.pc),errors,len(got),len(wanted))
  semantic=False
  if c.get('semantic') and rc in (0,-2):
   if entry==SETUP:
    for a,n in ((MOTION,28),(KIND,4),(DUAL,4),(0x8c88fe24,4),(0x8c88fe2c,12),(0x8c89001c,8)):
     assert state.gate.support.morph.bytes_at(actual,a,n)==state.gate.support.morph.bytes_at(oracle,a,n),(c,'independent motion globals',hex(a))
    if not rc and accepted:assert checks and load(actual,0x8c88f69c)==1 and load(actual,0x8c88f5f8)==STACK,(c,'independent recursive stack restoration')
   elif entry in BASE:
    expected_matrix=[Fraction(1 if i%5==0 else 0) for i in range(16)]
    if entry==BASE[0]:expected_matrix[12:15]=[Fraction(.25),Fraction(-.5),Fraction(.75)]
    elif entry==BASE[3]:expected_matrix[0]=Fraction(.5);expected_matrix[5]=Fraction(1);expected_matrix[10]=Fraction(2)
    raw=[load(actual,MATRIX+i*4) for i in range(16)] if c.get('explicit') else list(f.banks[(f.fpscr>>21&1)^1]);assert [Fraction(state.gate.support.morph.value(x)) for x in raw]==expected_matrix,(c,'independent direct base matrix')
   semantic=True;independent+=1
  world_checks+=len(checks);coverage.update(path);boundaries+=len(got);targets.update(t for t in path if t in CONNECTED)
  for i,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  records.append(dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,independent_checks=semantic,independent_world_matrices=len(checks),stop_pc=hex(s.pc),steps=len(path)))
  if (number+1)%128==0:print(number+1,'motion setup/base-dispatch original comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Motion setup/base-dispatch smoke passed:',len(records));return
 wanted_edges={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert set(OWN)<=coverage and edges==wanted_edges,(list(map(hex,set(OWN)-coverage)),wanted_edges-edges)
 files=[Path(__file__),PROJECT/'verify_overlay_draw_distance.py',PROJECT/'verify_overlay_draw_state.py',PROJECT/'sh4_binary32.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 files.extend(PROJECT/f'analysis/draw-motion-setup/{a:08x}_instructions.tsv' for a in (SAVE,RESTORE,*ROTATE))
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),rows_covered=len(set(OWN)&coverage),conditional_edges_covered=len(edges),boundary_comparisons=boundaries,independent_cases=independent,independent_world_matrix_checks=world_checks,target_invocations={hex(a):n for a,n in sorted(targets.items())},unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),newly_recovered_entries=7,reused_routine_credit=0,external_game_service_fixtures=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Seven complete guarded motion/control/base-transform bodies. All original rows/outcomes, full RAM/CPU/FPU/FPUL/MACL/stop states, native state-save/restore and matrix services, independent unsigned frame-limit/globals/callback selection and exact dyadic world/base matrices.',limitations=['Interpolation handlers, nonzero format callbacks and motion mesh wrapper 8c621574 remain explicit -2 boundaries. No service results are fabricated; successful recursion uses real base transforms with zero rotations, supplied acyclic nodes and no rendered mesh.','Optional-callback invocation uses the original native RTS/NOP routine as supplied callback data, not an injected interpreter hook. Natural callback/player/frame initialization remains unproved.','Inherits legacy matrix/valid-RAM/narrow-FPU and pinned vector numerical support limits. No physical GPU pixels, complete animated drawing, high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/draw_motion_setup_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','boundary_comparisons','independent_cases','independent_world_matrix_checks','unconnected_targets')},indent=2))
if __name__=='__main__':main()
