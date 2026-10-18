"""Verify the selector's guarded missing-node boundary and unchanged valid path."""
import csv,ctypes,hashlib,itertools,json,random
from pathlib import Path
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
MODULE=ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN'
DLL=ROOT/'work/sonic-adventure/build/overlay_object_initializer.dll'
ENTRY,STOP=0xc905548,0xc905572
ENTITY,APP,NODE=0x8cc20000,0x8cc30000,0x8cc40000
LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_overlay_selector_guard
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State),ctypes.POINTER(Fpu)];fn.restype=ctypes.c_int
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
class Stop(Exception):pass
def main():
 binary=MODULE.read_bytes();ram=bytearray(16<<20);ram[0x900000:0x900000+len(binary)]=binary
 previous_path=PROJECT/'analysis/overlay_face_model_selector_verification.json';previous=json.loads(previous_path.read_text())
 for p,h in previous['sha256'].items():assert digest(p)==h,p
 listing=PROJECT/'analysis/overlay-face-model-selector/0c905548_instructions.tsv';prefix=[]
 for row in csv.DictReader(listing.open(),delimiter='\t'):
  a=int(row['address'],16)
  if a<STOP:
   assert binary[a-0xc900000:a-0xc900000+2]==bytes.fromhex(row['bytes']);prefix.append(a)
 assert len(prefix)==21
 records=[];rng=random.Random(STOP)
 nodes=(NODE,0xc000000|(NODE&0xffffff),0xac000000|(NODE&0xffffff),0,0x08000000,0xe0000000)
 for node,flags,mode in itertools.product(nodes,(0,1,2,4,8,16,31,255),range(8)):
  bad=node in nodes[3:];m=bytearray(ram);store(m,ENTITY+0xec,APP);store(m,ENTITY+0x2d4,node);store(m,APP+0x34,flags,1)
  for i in range(13):store(m,NODE+i*4,0xaabb0000+i*0x101)
  s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[5]=ENTITY;s.r[15]=0x8cf00000;s.sr=rng.getrandbits(32);s.pc=ENTRY;s.pr=0x8cfed000
  other=State.from_buffer_copy(s);f=Fpu();f.fpscr=0x40000|(mode<<19)
  for bank in range(2):
   for i in range(16):f.banks[bank][i]=rng.getrandbits(32)
  ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m);path=[]
  def trace(pc,w,r,sr,pr,fp):
   if bad and pc==STOP:other.r[:]=r;other.sr=sr;other.pc=pc;raise Stop()
   path.append(pc)
  try:reference(expected,other,fpu=ef,trace=trace,instruction_budget=1000)
  except Stop:pass
  status=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(s),ctypes.byref(f))
  assert status==(-3 if bad else 0) and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef),(hex(node),flags,mode,'full original state')
  if bad:assert path==prefix and actual==m and s.pc==STOP
  records.append(dict(node=hex(node),flags=flags,mode=mode,status=status,full_state_match=True,guard_stops_before_memory_write=bad))
 assert len(records)==384 and sum(r['status']==-3 for r in records)==192
 files=[Path(__file__),previous_path,listing,PROJECT/'source/sa_overlay_selector_guard.c',PROJECT/'source/sa_overlay_face_model_selector_0c905548.c',PROJECT/'source/fpu_cleanup_transfer.h',PROJECT/'source/stage00_task.h',PROJECT/'build_overlay_object_initializer.ps1',PROJECT/'verify_startup.py',PROJECT/'verify_fpu_cleanup_transfer.py',PROJECT/'verify_overlay_selector_initializer.py',MODULE]
 report=dict(cases_passed=384,valid_cases=192,guarded_cases=192,original_prefix_rows=21,newly_recovered_entries=0,reused_selector_credit=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Unchanged native selector on 192 valid physical/cached/uncached destinations and exact original 21-instruction prefix on 192 invalid destinations; all eight FR/SZ/PR modes and representative selector flags. Full RAM/CPU/FPU matches; guard precedes first original FMOV store at 0c905572.',limitations=['Support guard only, with valid entity/appearance records. It reports an unsupported original write rather than delivering a hardware exception or completing a failed copy.'],cases=records)
 (PROJECT/'analysis/overlay_selector_guard_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Selector guard: 384 original comparisons passed (192 valid, 192 guarded).')
if __name__=='__main__':main()
