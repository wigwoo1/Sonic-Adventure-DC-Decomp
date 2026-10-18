"""Full original states and independent animation/appearance/accessory oracles."""
import collections,csv,ctypes,hashlib,inspect,itertools,json,random,sys
from pathlib import Path
import verify_overlay_model_blend as morph
from verify_startup import ROOT,State
from verify_fpu_cleanup_transfer import Fpu
from verify_main_dispatch_active_query import OBSERVE
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
MODULE=ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN';MAIN=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
ANIM,EMOTION,PARTS=0xc90bd40,0xc90853e,0xc9048ec
TAILS=(0xc90fb82,0xc90fca0,0xc90fd42);LOWERS=(0xc9084e6,0xc9084fc,0xc908530);RNG=0x8c10d334
ENTITY,APP=morph.ENTITY,morph.APP
SEED=0x8c5fbb08
DLL=ROOT/'work/sonic-adventure/build/overlay_initializer_finish.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_overlay_initializer_finish_connected
fn.argtypes=morph.fn.argtypes;fn.restype=ctypes.c_int
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def rows():
 out={}
 for group in ('overlay-initializer-dependencies','overlay-initializer-finish'):
  for listing in (PROJECT/'analysis'/group).glob('*_instructions.tsv'):
   if int(listing.name.split('_')[0],16) in (ANIM,EMOTION,PARTS)+TAILS+LOWERS:
    for r in csv.DictReader(listing.open(),delimiter='\t'):out[int(r['address'],16)]=r['instruction'].lstrip('_')
 for r in csv.DictReader((PROJECT/'analysis/instructions/work_memory_services.tsv').open(),delimiter='\t'):
  if RNG<=int(r['address'],16)<0x8c10d352:out[int(r['address'],16)]=r['instruction'].lstrip('_')
 return out
ROWS=rows();RETURNS={0xc90bd92,0xc908650,0xc908656,0xc90865e}
# Reuse the independent reference's arithmetic/boundary handling without
# modifying its pinned source or globals. This family has different nested PRs.
namespace=dict(morph.__dict__,ROWS=ROWS)
reference_code=inspect.getsource(morph.reference_run).replace('pr not in (entry_pr,0xc90545a)', 'pr not in RETURNS|{entry_pr}')
namespace['RETURNS']=RETURNS;exec(compile(reference_code,'finish_reference','exec'),namespace)
reference_run=namespace['reference_run']
def appearance_oracle(m,entity,entry,arguments=None):
 if entry==EMOTION:
  app=load(m,entity+0xec);categories=[]
  for off in (0x5c,0x5d,0x5e):
   n=load(m,app+off,1);n=n if n<128 else n-256;categories.append(0 if n<-35 else 1 if n<35 else 2)
  index=categories[0]*3+categories[1]*9+categories[2]
  mode=load(m,load(m,0xc908740)+index,1);pair=load(m,load(m,0xc908744)+index,1)
  first=load(m,load(m,0xc908748)+index*4);second=load(m,load(m,0xc90874c)+index*4)
  for off,v in ((0x7f4,first),(0x7e4,second),(0x7e8,second),(0x7f0,second),(0x800,first),(0x7d0,0xffffffff),(0x7d4,0xffffffff),(0x7d8,0xffffffff)):store(m,entity+off,v)
 elif entry==LOWERS[0]:mode=arguments[0];pair=None
 elif entry==LOWERS[1]:mode=None;pair=arguments[0]
 else:
  store(m,entity+0x7f0,arguments[0]);store(m,entity+0x800,arguments[1]);return
 if entry!=LOWERS[1]:store(m,entity+0x804,load(m,load(m,0xc9085a0)+mode,1),2)
 if pair is not None:
  base=load(m,0xc9085a4)+pair*2;store(m,entity+0x808,load(m,base,1),2);store(m,entity+0x80a,load(m,base+1,1),2)
def accessory_oracle(m,entity):
 app=load(m,entity+0xec);character=load(m,app,1);writes=[]
 if character==6:return writes
 table=load(m,0xc904a34 if character==0 else 0xc904a38)
 kinds=[load(m,load(m,0xc904a30)+i*2,2) for i in range(12)]
 destinations=[load(m,load(m,0xc904a28)+i*2,2) for i in range(3)]+[load(m,load(m,0xc904a2c)+i*2,2) for i in range(9)]
 for i,(kind,index) in enumerate(zip(kinds,destinations)):
  selection=load(m,app+0x4c+kind,1)
  if selection==255:continue
  signed=selection if selection<128 else selection-256
  source=load(m,table+signed*96+(kind if i<3 else i)*8+4)
  if not source:continue
  dest=load(m,entity+0x24c+index*4)
  if i<3:
   model=load(m,source+4);store(m,dest+4,model)
  else:
   child=load(m,source+44);store(m,dest+44,child);model=load(m,child+4)
  materials=load(m,model+16);count=load(m,model+22,2)
  for j in range(count):
   for channel in range(3):
    a=materials+j*20+channel;store(m,a,255,1);writes.append(a)
 return writes
def tail_oracle(m,entity,entry):
 literal={TAILS[0]:0xc90fc7c,TAILS[1]:0xc90fec0,TAILS[2]:0xc90fec8}[entry];p=load(m,literal)
 colour=load(m,load(m,entity+0xec)+0x192,1);colour=colour if colour<4 else 0
 ident=load(m,load(m,0xc90fc80)+colour*2,2) if entry==TAILS[0] else 158 if entry==TAILS[1] else 161
 while (i:=load(m,p))!=0xffffffff:
  node=load(m,entity+0x24c+i*4);model=load(m,node+4);materials=load(m,model+16)
  for j in range(load(m,model+22,2)):
   store(m,materials+j*20+12,ident);store(m,materials+j*20+16,load(m,materials+j*20+16)|0x400000)
  p+=4
def animation_oracle(m,entity):
 seed=(load(m,SEED)*1103515245+12345)&0xffffffff;store(m,SEED,seed);random_value=seed>>16&32767
 table=load(m,0xc90bf00)
 for off in (0x11e,0x120,0x122):store(m,entity+off,0,2)
 store(m,entity+0x138,table);store(m,entity+0x118,load(m,table+4,2),2);store(m,entity+0x134,morph.bits(1))
 assert load(m,table+8)==0 and load(m,table+12)==morph.bits(8)
 store(m,entity+0x124,morph.bits(random_value/4096));store(m,entity+0x11a,load(m,entity+0x11a,2)&0xffc0,2)
def cases():
 for axis,raw,bank in itertools.product(range(3),range(256),range(2)):
  triple=[0,0,0];triple[axis]=raw;yield dict(entry=EMOTION,emotion=triple,mode=bank*4)
 for triple,mode in itertools.product(itertools.product((-36,0,35),repeat=3),range(8)):yield dict(entry=EMOTION,emotion=triple,mode=mode)
 for seed,mode,rm in itertools.product((0,1,0x7fffffff,0x80000000,0xffffffff,0x12345678),range(8),range(2)):yield dict(entry=ANIM,seed=seed,mode=mode,rm=rm)
 for mode,rm,enable in itertools.product((0,2,4,6),(0,1,2,3),(0x80,0x800)):yield dict(entry=ANIM,seed=1,mode=mode,rm=rm,enable=enable,probe=True)
 for entry,arg,mode in itertools.product(LOWERS,range(8),range(8)):yield dict(entry=entry,arg=arg,mode=mode)
 for colour,mode in itertools.product(range(256),range(8)):yield dict(entry=TAILS[0],colour=colour,mode=mode)
 for entry,count,mode in itertools.product(TAILS,(0,1,3),range(8)):yield dict(entry=entry,materials=count,mode=mode)
 for entry,mode in itertools.product(TAILS,range(8)):yield dict(entry=entry,empty=True,mode=mode)
 for character,part,mode in itertools.product((0,1,6,7),range(15),(0,4)):yield dict(entry=PARTS,type=character,parts=[part]*7,mode=mode)
 for character,kind,part,mode in itertools.product((0,1),range(7),(-1,0,6,14),(0,4)):
  parts=[-1]*7;parts[kind]=part;yield dict(entry=PARTS,type=character,parts=parts,mode=mode)
 for character,mode in itertools.product((0,1),range(8)):yield dict(entry=PARTS,type=character,parts=[6]*7,mode=mode,zero_accessory_materials=True)
def main():
 manifest_path=PROJECT/'analysis/overlay_initializer_finish_translation.json';manifest=json.loads(manifest_path.read_text())
 for p,h in manifest['sha256'].items():assert digest(p)==h,p
 old=json.loads((PROJECT/'analysis/random_u15_8c10d334_verification.json').read_text());assert digest(PROJECT/'source/sa_random_u15_8c10d334.c')==old['source_sha256']
 ram=bytearray(16<<20);mainbytes=MAIN.read_bytes();binary=MODULE.read_bytes();ram[0x10000:0x10000+len(mainbytes)]=mainbytes;ram[0x900000:0x900000+len(binary)]=binary
 coverage=set();edges=set();records=[];counts=collections.Counter();independent=0;boundaries=0;asset_writes=0;rng=random.Random(ANIM);selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=ANIM,mode=0,seed=1),dict(entry=ANIM,mode=2),dict(entry=EMOTION,emotion=(-36,35,0),mode=7),dict(entry=PARTS,type=0,parts=[6]*7,mode=0),dict(entry=TAILS[0],colour=3,mode=7)]
 for number,c in enumerate(selected):
  entry=c['entry'];m=morph.fixture(ram,dict(type=c.get('type',0),materials=c.get('materials',2)))
  for i,raw in enumerate(c.get('emotion',(0,0,0))):store(m,APP+0x5c+i,raw,1)
  for i,raw in enumerate(c.get('parts',[-1]*7)):store(m,APP+0x4c+i,raw,1)
  store(m,APP+0x192,c.get('colour',0),1);store(m,SEED,c.get('seed',0x12345678));store(m,ENTITY+0x11a,0xaabbccdd)
  if c.get('empty'):store(m,load(m,{TAILS[0]:0xc90fc7c,TAILS[1]:0xc90fec0,TAILS[2]:0xc90fec8}[entry]),0xffffffff)
  if c.get('zero_accessory_materials'):
   # Data-only zero-material fixture, retaining original source graph pointers.
   table=load(m,0xc904a34 if c['type']==0 else 0xc904a38)
   for slot in range(12):
    source=load(m,table+6*96+slot*8+4)
    if source:store(m,load(m,(source if slot<3 else load(m,source+44))+4)+22,0,2)
  s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=ENTITY if entry in LOWERS else 0x8cbff000;s.r[5]=c.get('arg',0) if entry in LOWERS else ENTITY;s.r[6]=0x89abcdef;s.r[15]=0x8cf00000;s.pr=0x8cfed000;s.pc=entry;s.sr=rng.getrandbits(32)
  f=Fpu();f.fpscr=0x40000|(c.get('mode',0)<<19)|c.get('rm',0)|c.get('enable',0);f.fpul=rng.getrandbits(32)
  for bank in range(2):
   for i in range(16):f.banks[bank][i]=morph.bits((i+1)/8)
  other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
  status,path,want,expected_macl=reference_run(expected,other,ef,c,tracked=(ANIM,EMOTION,PARTS,RNG)+TAILS+LOWERS)
  ml=ctypes.c_uint32(0x12345678);got=[];errors=[]
  @OBSERVE
  def observe(ctx,target,mem,fp,sr,r,pr):
   try:got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,hashlib.sha256(ctypes.string_at(ctypes.addressof(mem.contents),16<<20)).digest()))
   except Exception as exc:errors.append(repr(exc))
  rc=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),morph.P),ctypes.byref(ml),entry,None,observe)
  assert not errors and rc==status and got==want,(c,'status/boundaries',rc,status,errors)
  assert actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full original state',hex(s.pc),hex(other.pc))
  semantic=False
  if rc==0:
   semantic=True;oracle=bytearray(m)
   if entry==ANIM:animation_oracle(oracle,ENTITY)
   elif entry in (EMOTION,)+LOWERS:appearance_oracle(oracle,ENTITY,entry,(c.get('arg',0),0x89abcdef))
   elif entry==PARTS:asset_writes+=len(accessory_oracle(oracle,ENTITY))
   else:tail_oracle(oracle,ENTITY,entry)
   # The original stack saves/copy remain visible; other regions must match
   # the independent data algorithm exactly, including shared asset writes.
   for a,n in ((0xc900000,len(binary)),(ENTITY,0x80000),(SEED,4)):
    assert morph.bytes_at(actual,a,n)==morph.bytes_at(oracle,a,n),(c,'independent data',hex(a))
   independent+=1
  coverage.update(path)
  for i,a in enumerate(path[:-1]):
   image,base=(binary,0xc900000) if a<0x10000000 else (mainbytes,0x8c010000);w=int.from_bytes(image[a-base:a-base+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  counts[rc]+=1;boundaries+=len(got);records.append(dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,independent_checks=semantic,stop_pc=hex(s.pc),steps=len(path)))
  if (number+1)%512==0:print(number+1,'initializer-dependency comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Initializer dependency smoke passed:',len(records));return
 own={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']};wanted={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 assert own<=coverage and edges==wanted,(list(map(hex,own-coverage)),wanted-edges)
 files=[Path(__file__),manifest_path,PROJECT/'source/sa_overlay_initializer_finish_connected.c',PROJECT/'source/sa_random_u15_8c10d334.c',PROJECT/'analysis/random_u15_8c10d334_verification.json',PROJECT/'analysis/instructions/work_memory_services.tsv',PROJECT/'build_overlay_initializer_finish.ps1',PROJECT/'verify_overlay_model_blend.py',PROJECT/'verify_overlay_selector_initializer.py']+[Path(p) for p in manifest['sha256']]
 baseline=json.loads((PROJECT/'analysis/overlay_model_blend_verification.json').read_text());files.extend(Path(p) for p in baseline['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(counts),rows_covered=len(coverage&own),conditional_edges_covered=len(edges),boundary_comparisons=boundaries,independent_cases=independent,shared_asset_byte_writes=asset_writes,native_external_game_services=0,newly_recovered_entries=8,retranslated_existing_entry='0xc908530',reused_setter_credit=0,reused_rng_credit=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Nine full guarded native bodies with reused native RNG; full RAM/CPU/FPU/FPUL/MACL, ordered entries and independent animation, signed appearance-table, field-preservation, accessory shared-material and tail-material checks.',limitations=['Constructed valid entity/node records and data-only empty/zero-material fixtures. Full initializer integration is checked separately.','Arithmetic/memory guards report exact reached partial state, not CPU exceptions or full hardware FPSCR coverage. No game scheduling or visible-pixel proof.'],cases=records)
 (PROJECT/'analysis/overlay_initializer_finish_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','boundary_comparisons','independent_cases','shared_asset_byte_writes')},indent=2))
if __name__=='__main__':main()
