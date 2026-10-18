"""Original instruction and independent material-update checks for three variants."""
import collections,ctypes,hashlib,itertools,json,random,sys
from pathlib import Path
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
MODULE=ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN'
DLL=ROOT/'work/sonic-adventure/build/overlay_object_initializer.dll'
ENTRIES=(0xc904aea,0xc904bbc,0xc904c80)
LIST_LITERALS=(0xc904c60,0xc904c6c,0xc904e60)
ENTITY,APP,NODES,MODELS,MATERIALS=0x8cc10000,0x8cc20000,0x8cc40000,0x8cc50000,0x8cc60000
LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_overlay_object_variant_connected
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State),ctypes.POINTER(Fpu),ctypes.c_uint32];fn.restype=ctypes.c_int
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def indices(m,entry):
 p=load(m,LIST_LITERALS[ENTRIES.index(entry)]);out=[]
 while load(m,p)!=0xffffffff:
  out.append(load(m,p));p+=4;assert len(out)<20
 return out
def apply_materials(m,entity,entry,colour):
 palette=[load(m,load(m,0xc904c64)+i*2,2) for i in range(4)]
 value=palette[colour if colour<=3 else 0] if entry==ENTRIES[0] else (158 if entry==ENTRIES[1] else 161)
 changed=0
 for i in indices(m,entry):
  node=load(m,entity+0x24c+i*4);model=load(m,node+4);count=load(m,model+22,2);materials=load(m,model+16)
  for j in range(count):
   a=materials+j*20;store(m,a+12,value);store(m,a+16,load(m,a+16)|0x400000);changed+=1
 return changed
def cases():
 for colour,mode in itertools.product(range(256),range(8)):yield dict(entry=ENTRIES[0],colour=colour,mode=mode,counts='mixed')
 for entry,count,mode in itertools.product(ENTRIES,(0,1,2,4),range(8)):yield dict(entry=entry,colour=3,mode=mode,counts=count)
 for entry,mode in itertools.product(ENTRIES,range(8)):yield dict(entry=entry,colour=255,mode=mode,counts=4,empty=True)
 for entry,alias in itertools.product(ENTRIES,(0,0x80000000,0xa0000000)):yield dict(entry=entry,colour=2,mode=0,counts='mixed',alias=alias)
def main():
 manifest_path=PROJECT/'analysis/overlay_object_variants_translation.json';manifest=json.loads(manifest_path.read_text())
 for p,h in manifest['sha256'].items():assert digest(p)==h,p
 binary=MODULE.read_bytes();ram=bytearray(16<<20);ram[0x900000:0x900000+len(binary)]=binary
 own={int(a,16) for r in manifest['routines'] for a in r['reachable_addresses']};wanted={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']}
 rows=set();edges=set();counts=collections.Counter();records=[];rng=random.Random(ENTRIES[0]);selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,colour=colour,mode=7,counts='mixed',empty=empty) for e,colour,empty in itertools.product(ENTRIES,(0,3,255),(False,True))]
 for n,c in enumerate(selected):
  m=bytearray(ram);entity=ENTITY
  if 'alias' in c:entity=(ENTITY&0x1fffffff)|c['alias']
  store(m,entity+0xec,APP);store(m,APP+0x192,c['colour'],1)
  for i in range(39):
   store(m,entity+0x24c+i*4,NODES+i*64);store(m,NODES+i*64+4,MODELS+i*64)
   count=i%5 if c['counts']=='mixed' else c['counts']
   store(m,MODELS+i*64+16,MATERIALS+i*128);store(m,MODELS+i*64+22,count,2)
   for j in range(128):m[(MATERIALS&0xffffff)+i*128+j]=(i*29+j*79)&255
  if c.get('empty'):store(m,load(m,LIST_LITERALS[ENTRIES.index(c['entry'])]),0xffffffff)
  before=State();before.r[:]=[rng.getrandbits(32) for _ in range(16)];before.r[5]=entity;before.r[15]=0x8cf00000;before.pr=0x8cfed000;before.pc=c['entry'];before.sr=rng.getrandbits(32)
  s=State.from_buffer_copy(before);other=State.from_buffer_copy(before);f=Fpu();f.fpscr=0x40000|(c['mode']<<19)
  for bank in range(2):
   for i in range(16):f.banks[bank][i]=rng.getrandbits(32)
  initial_fpu=bytes(f);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m);path=[]
  def trace(pc,w,r,sr,pr,fp):assert pc in own;path.append(pc)
  reference(expected,other,fpu=ef,trace=trace,instruction_budget=10000)
  status=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(s),ctypes.byref(f),c['entry'])
  assert status==0 and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef),(c,'original full state')
  assert bytes(f)==initial_fpu and s.pr==before.pr and s.r[15]==before.r[15]
  semantic=bytearray(m);changed=apply_materials(semantic,entity,c['entry'],c['colour'])
  for a,size in ((MATERIALS,39*128),(MODELS,39*64),(NODES,39*64),(entity,0x900),(APP,0x200)):
   off=a&0xffffff;assert actual[off:off+size]==semantic[off:off+size],(c,hex(a),'independent memory')
  rows.update(path)
  for i,a in enumerate(path[:-1]):
   w=int.from_bytes(binary[a-0xc900000:a-0xc900000+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):edges.add((a,path[i+(2 if w>>8 in (0x8d,0x8f) else 1)]))
  counts[c['entry']]+=1;records.append(dict(input=c,full_state_match=True,independent_materials_match=True,materials_updated=changed))
  if (n+1)%512==0:print(n+1,'material variant comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Material variants smoke passed:',len(records));return
 assert rows==own and edges==wanted,(len(rows),len(own),wanted-edges)
 files=[Path(__file__),manifest_path,PROJECT/'build_overlay_object_initializer.ps1',PROJECT/'source/sa_overlay_object_variants_connected.c',PROJECT/'source/fpu_cleanup_transfer.h',PROJECT/'source/stage00_task.h',PROJECT/'verify_startup.py',PROJECT/'verify_fpu_cleanup_transfer.py',PROJECT/'verify_overlay_selector_initializer.py']+[Path(p) for p in manifest['sha256']]
 report=dict(module='AL_MAIN.BIN',load_address='0x0c900000',source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),rows_covered=len(rows),conditional_edges_covered=len(edges),call_sites_covered=0,external_services=0,entry_counts={hex(k):v for k,v in counts.items()},materials_updated=sum(r['materials_updated'] for r in records),newly_recovered_entries=3,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='All 267 rows and 26 outcomes in three integer material variants, full original RAM/CPU/FPU comparisons plus independent selected-node texture-id/mask updates and untouched-field checks. All 256 appearance bytes in all eight modes for palette variant; zero/multiple materials, empty lists and RAM aliases.',limitations=['Constructed valid node/model/material tables. Empty-list tests alter data, not instructions; shipped tables are checked in combined initializer tests.','No texture sampling, visible pixels, full scene scheduling or gameplay proof.'],cases=records)
 (PROJECT/'analysis/overlay_object_variants_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','rows_covered','conditional_edges_covered','materials_updated','entry_counts')},indent=2))
if __name__=='__main__':main()
