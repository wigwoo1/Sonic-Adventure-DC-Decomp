"""Original motion formats/keyframes: full state and independent dyadic curves."""
import collections,csv,ctypes,hashlib,itertools,json,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_motion_setup as previous
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
MANIFEST=PROJECT/'analysis/draw_motion_formats_translation.json';manifest=json.loads(MANIFEST.read_text())
NEW={int(r['address'],16) for r in manifest['routines']};SAMPLERS=(0x8c60c322,0x8c60c488,0x8c60c526,0x8c60c3b8);CURVES=(0x8c60beb0,0x8c60bf5e,0x8c60c026,0x8c60c19c)
FORMATS=(0x8c60d054,0x8c60d094,0x8c60d0d4,0x8c60d140,0x8c60d180,0x8c60d19e,0x8c60d1c8)
HELPERS={0x8c10cc80,0x8c10cd38,0x8c10cd8e};CONNECTED=previous.CONNECTED|NEW|HELPERS
ROWS=dict(previous.ROWS);OWN={}
for r in manifest['routines']:
 for row in csv.DictReader(Path(r['listing']).open(),delimiter='\t'):
  a=int(row['address'],16);OWN[a]=row['instruction'].lstrip('_');ROWS[a]=OWN[a]
for a in HELPERS:
 for row in csv.DictReader((PROJECT/f'analysis/draw-motion-formats/{a:08x}_instructions.tsv').open(),delimiter='\t'):ROWS[int(row['address'],16)]=row['instruction'].lstrip('_')
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS)
code=previous.code;exec(compile(code,'motion_formats_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=previous.ROOT/'work/sonic-adventure/build/draw_motion_formats.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_motion_formats_connected;fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
State,Fpu,OBSERVE=previous.State,previous.Fpu,previous.OBSERVE
NODE,DATA,MATRIX,MOTION=previous.NODE,previous.DATA,previous.MATRIX,previous.MOTION
POINTERS,COUNTS,KEYS,OUTPUT=0x8cc80000,0x8cc81000,0x8cc90000,0x8cca0000
bits=previous.state.gate.support.morph.bits;value=previous.state.gate.support.morph.value

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def labels(c):return [0x80000000+i*1024 for i in range(4)] if c.get('high') else [0,4,8,12]

def key_values(channel,i,angles=False):
 if angles:return ((i-2)*1024,(i*i-3)*512,(3-i)*256)
 if channel==2:return (Fraction(i+2,4),Fraction(3-i,4),Fraction(i*i+1,4))
 return (Fraction(i-2,4),Fraction(i*i-3,8),Fraction(3-i,2))

def prepare(ram,c):
 m,s,f=previous.prepare(ram,dict(c,entry=c.get('entry',previous.SETUP)))
 for channel in range(3):
  a=KEYS+channel*0x100
  for i,key in enumerate(labels(c)):
   store(m,a+i*16,key)
   for j,v in enumerate(key_values(channel,i,channel==1)):store(m,a+i*16+4+j*4,int(v)&0xffffffff if channel==1 else bits(v))
 store(m,MOTION,DATA);store(m,MOTION+4,POINTERS);store(m,MOTION+8,COUNTS);store(m,MOTION+12,bits(c.get('frame',5)));store(m,MOTION+16,16);store(m,MOTION+20,0);store(m,MOTION+24,MATRIX if c.get('explicit') else 0)
 for i in range(8):store(m,POINTERS+i*4,0);store(m,COUNTS+i*4,4)
 cubic=c.get('cubic',False);store(m,0x8c89001c,CURVES[2] if cubic else CURVES[0]);store(m,0x8c890020,CURVES[3] if cubic else CURVES[1]);store(m,0x8c890024,0x8c621574);store(m,0x8c88fe3c,previous.NOOP);store(m,0x8c88fe5c,DATA)
 entry=s.pc
 if entry in CURVES:
  channel=1 if entry in (CURVES[1],CURVES[3]) else c.get('channel',0)
  s.r[4]=KEYS+channel*0x100;s.r[5]=c.get('index',1);s.r[6]=4;s.r[7]=OUTPUT
 elif entry in SAMPLERS:
  channel=1 if entry in SAMPLERS[1:3] else 2 if entry==SAMPLERS[3] else 0
  slot=c.get('slot',2);store(m,MOTION+20,slot);store(m,POINTERS+slot*4,0 if c.get('absent') else KEYS+channel*0x100)
 elif entry in FORMATS[:4]:
  kind=FORMATS.index(entry)+1;typ=c.get('type',7 if kind>=3 else (3 if kind==2 else 1));active=[i for i in range(3) if typ>>i&1]
  assert len(active)<=kind
  for j,channel in enumerate(active):store(m,DATA+j*4,0 if c.get('absent') else KEYS+channel*0x100);store(m,DATA+kind*4+j*4,4)
  for slot,a in enumerate((0x8c88fe2c,0x8c88fe30,0x8c88fe34)):
   sampler=SAMPLERS[0] if slot==0 else SAMPLERS[2] if c.get('order') else SAMPLERS[1] if slot==1 else SAMPLERS[3]
   if slot==2:sampler=SAMPLERS[3]
   base=previous.BASE[slot] if slot==0 else previous.BASE[2] if slot==1 and c.get('order') else previous.BASE[1] if slot==1 else previous.BASE[3]
   store(m,a,sampler if typ>>slot&1 else base)
 elif entry==previous.SETUP:
  # A valid compact format-two record with position+rotation channels.
  store(m,previous.DESC,DATA);store(m,previous.DESC+4,16);store(m,previous.DESC+8,3,2);store(m,previous.DESC+10,2|(0x40 if cubic else 0),2)
  for j in range(2):store(m,DATA+j*4,KEYS+j*0x100);store(m,DATA+8+j*4,4)
  store(m,NODE,c.get('flags',0)|(0x20 if c.get('order') else 0))
 if c.get('bad')=='key' and entry in CURVES:s.r[4]=0x8cfffffc
 if c.get('bad')=='output' and entry in CURVES:s.r[7]=0x8cfffffc
 if c.get('bad')=='node':s.r[4]=0
 if c.get('bad')=='stack':s.r[15]=0
 return m,s,f

def curve_oracle(c,channel,index,cubic):
 keys=labels(c);next_index=(index+1)%4;delta=(keys[next_index]-keys[index])&0xffffffff
 signed_delta=delta if delta<0x80000000 else delta-0x100000000
 # The original chooses the copied default 1.0 for nonpositive signed spans.
 denominator=delta if signed_delta>0 else 1
 t=(Fraction(c.get('frame',5))-keys[index])/denominator
 if cubic:
  weights=(t*t-t/2-t*t*t/2,1+3*t*t*t/2-5*t*t/2,t/2+2*t*t-3*t*t*t/2,(t*t*t-t*t)/2)
  indices=((index-1)%4,index,next_index,(index+2)%4)
 else:weights=(1-t,t);indices=(index,next_index)
 vals=[sum(w*key_values(channel,i,channel==1)[j] for w,i in zip(weights,indices)) for j in range(3)]
 return [int(v)&0xffffffff if channel==1 else bits(v) for v in vals]

def cases():
 for entry,bank,index,frame in itertools.product(CURVES,range(2),range(4),(0,1,5,9,12)):
  # Fractions stay dyadic, including the original extrapolation/wrap rule.
  yield dict(entry=entry,bank=bank,index=index,frame=frame,semantic=True)
 for entry,bank,cubic,absent,explicit in itertools.product(SAMPLERS,range(2),(False,True),(False,True),(False,True)):
  yield dict(entry=entry,bank=bank,cubic=cubic,absent=absent,explicit=explicit,semantic=True)
 for entry,bank,cubic,absent in itertools.product(FORMATS[:4],range(2),(False,True),(False,True)):
  yield dict(entry=entry,bank=bank,cubic=cubic,absent=absent,semantic=True)
 for entry,bank in itertools.product(FORMATS[5:],range(2)):yield dict(entry=entry,bank=bank,semantic=True)
 for bank,flags,mesh in itertools.product(range(2),(0,8),(False,True)):yield dict(entry=FORMATS[4],bank=bank,flags=flags,mesh=mesh,semantic=True)
 for bank,cubic,order in itertools.product(range(2),(False,True),range(2)):yield dict(bank=bank,cubic=cubic,order=order,frame=5,semantic=True)
 for entry,bank in itertools.product(SAMPLERS,range(2)):yield dict(entry=entry,bank=bank,high=True,frame=2147485696)
 for entry,bank in itertools.product(CURVES,range(2)):yield dict(entry=entry,bank=bank,high=True,index=3,frame=2147485696)
 for entry,mode in itertools.product(CURVES+SAMPLERS,range(8)):yield dict(entry=entry,mode=mode,frame=5)
 for entry,bad in itertools.product(CURVES,('key','output','stack')):yield dict(entry=entry,bad=bad)
 for entry,frame,rm,enable in itertools.product((CURVES[0],CURVES[2]),(5,1.5),range(4),(0x80,0x800)):yield dict(entry=entry,frame=frame,rm=rm,enable=enable,probe=True)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_motion_formats_connection.json',PROJECT/'analysis/draw_motion_setup_verification.json']
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 binary=previous.state.gate.support.morph.MAIN.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(binary)]=binary
 selected=list(cases());records=[];coverage=set();edges=set();boundaries=0;targets=collections.Counter();independent=0;sample_checks=0
 if '--smoke' in sys.argv:selected=[dict(entry=a,frame=5,index=1,semantic=True) for a in CURVES]+[dict(entry=FORMATS[1],semantic=True),dict(entry=SAMPLERS[0],semantic=True),dict(frame=5,cubic=True,semantic=True)]
 for number,c in enumerate(selected):
  m,s,f=prepare(ram,c);entry=s.pc;other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
  status,path,wanted,expected_macl=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED));got=[];errors=[];checks=[];ml=ctypes.c_uint32(0x12345678)
  @OBSERVE
  def observe(ctx,target,mem,fp,sr,r,pr):
   try:
    got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,hashlib.sha256(ctypes.string_at(ctypes.addressof(mem.contents),16<<20)).digest()))
    if entry in SAMPLERS and c.get('semantic') and not c.get('absent') and target in (0x8c63a878,0x8c63a69c,0x8c639cf4,0x8c639ff8):
     channel=1 if entry in SAMPLERS[1:3] else 2 if entry==SAMPLERS[3] else 0
     index=max((i for i,k in enumerate(labels(c)) if k<=int(c.get('frame',5))),default=0)
     want=curve_oracle(c,channel,index,c.get('cubic',False));bank=fp.contents.fpscr>>21&1
     have=list(r[5:8]) if channel==1 else list(fp.contents.banks[bank][4:7]);assert have==want,(c,'independent sample arguments',have,want);checks.append(True)
   except Exception as exc:errors.append(repr(exc))
  rc=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),previous.state.gate.support.morph.P),ctypes.byref(ml),entry,None,observe)
  assert not errors and rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full original motion format state',rc,status,hex(s.pc),hex(other.pc),errors,len(got),len(wanted))
  if not any(k in c for k in ('mode','probe','bad')):
   assert rc==(-2 if entry==FORMATS[4] and c.get('mesh') and not c.get('flags',0)&8 else 0),(c,'intended completed path',rc,hex(s.pc))
  semantic=False
  if c.get('semantic') and rc in (0,-2):
   if entry in CURVES and not rc:
    channel=1 if entry in (CURVES[1],CURVES[3]) else c.get('channel',0);want=curve_oracle(c,channel,c.get('index',1),entry in CURVES[2:]);have=[load(actual,OUTPUT+j*4) for j in range(3)]
    assert have==want,(c,'independent interpolation',have,want)
   elif entry in SAMPLERS and not rc:assert load(actual,MOTION+20)==c.get('slot',2)+1 and (c.get('absent') or checks),(c,'sample counter')
   elif entry in FORMATS[:4] and not rc:
    kind=FORMATS.index(entry)+1;typ=c.get('type',7 if kind>=3 else 3 if kind==2 else 1)
    assert [load(actual,MOTION+j) for j in (0,4,8,20)]==[DATA+kind*8,DATA,DATA+kind*4,typ.bit_count()],(c,'compact record/counter advances')
   elif entry in FORMATS[5:] and not rc:assert load(actual,0x8c88fe5c)==DATA+(8 if entry==FORMATS[5] else 16) and load(actual,0x8c88fe70)==0,(c,'alternate record advance')
   elif entry==FORMATS[4]:
    assert rc==(-2 if c.get('mesh') and not c.get('flags',0)&8 else 0) and load(actual,MOTION)==load(m,MOTION),(c,'mesh-skip predicate')
   elif entry==previous.SETUP and not rc:
    assert [load(actual,MOTION+j) for j in (0,4,8,20)]==[DATA+16,DATA,DATA+8,2],(c,'setup-to-format record/counter advances')
   else:raise AssertionError((c,'missing independent check'))
   semantic=True;independent+=1
  sample_checks+=len(checks);coverage.update(path);boundaries+=len(got);targets.update(a for a in path if a in CONNECTED)
  for i,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  records.append(dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,independent_checks=semantic,independent_sample_boundaries=len(checks),stop_pc=hex(s.pc),steps=len(path)))
  if (number+1)%64==0:print(number+1,'motion format/interpolation original comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Motion format/interpolation smoke passed:',len(records));return
 wanted_edges={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert set(OWN)<=coverage and edges==wanted_edges,(list(map(hex,set(OWN)-coverage)),wanted_edges-edges)
 files=[Path(__file__),PROJECT/'verify_draw_motion_setup.py',PROJECT/'sh4_binary32.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),rows_covered=len(set(OWN)&coverage),conditional_edges_covered=len(edges),boundary_comparisons=boundaries,independent_cases=independent,independent_sample_boundaries=sample_checks,target_invocations={hex(a):n for a,n in sorted(targets.items())},unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),newly_recovered_entries=15,reused_routine_credit=0,external_game_service_fixtures=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Fifteen complete guarded original format/keyframe/interpolation bodies; all rows/outcomes and full original machine/RAM state. Independent exact dyadic linear/Catmull-Rom results, sampled transform arguments and record/counter advances.',limitations=['Format mesh handler uses supplied original motion-mesh target as an explicit -2 boundary. Alternate record handlers use original native empty callback as supplied callback data, proving control flow rather than morph output.','Legacy native remainder/copy/state/matrix services retain valid-RAM and bounded ABI support. Shared vector numerical reference is qualified separately; exact dyadic curve checks are independent.','No physical GPU pixels, natural scene/player initialization, full draw/frame/gameplay, high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/draw_motion_formats_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','boundary_comparisons','independent_cases','independent_sample_boundaries','unconnected_targets')},indent=2))
if __name__=='__main__':main()
