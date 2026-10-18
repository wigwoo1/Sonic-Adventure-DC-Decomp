"""Complete original selector bytes/state and independent bit-priority checks."""
import collections,csv,ctypes,hashlib,json,random,struct
from pathlib import Path
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
PROJECT=Path(__file__).resolve().parent
MODULE=ROOT/'work/sonic-adventure/decoded/SONICADV/AL_MAIN.BIN';MAIN=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN'
BASE,ENTRY,END=0x0c900000,0x0c905548,0x0c905618
LISTING=PROJECT/'analysis/overlay-face-model-selector/0c905548_instructions.tsv'
SOURCE=PROJECT/'source/sa_overlay_face_model_selector_0c905548.c'
DLL=ROOT/'work/sonic-adventure/build/overlay_face_model_selector.dll'
LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_overlay_face_model_selector_0c905548
fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State),ctypes.POINTER(Fpu)];fn.restype=ctypes.c_int

def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 binary=MODULE.read_bytes();rows=list(csv.DictReader(LISTING.open(),delimiter='\t'));assert len(rows)==88
 want={int(row['address'],16) for row in rows}
 for row in rows:
  a=int(row['address'],16);assert int(row['length'])==2 and binary[a-BASE:a-BASE+2]==bytes.fromhex(row['bytes'])
 ram=bytearray(16*1024*1024);ram[0x10000:0x10000+MAIN.stat().st_size]=MAIN.read_bytes();ram[0x900000:0x900000+len(binary)]=binary
 load=lambda m,a:struct.unpack_from('<I',m,a&0xffffff)[0]
 def store(m,a,v):struct.pack_into('<I',m,a&0xffffff,v&0xffffffff)
 roots=[load(ram,a) for a in (0xc9055ec,0xc9055f0,0xc9055f4,0xc9055f8,0xc905740)]
 assert roots[0]==0xca50ce4 and load(ram,roots[0]+4)==0xca50cbc
 rng=random.Random(ENTRY);records=[];covered=set();edges=set();writes=collections.Counter()
 inputs=[(flags,mode,0x8c000000) for flags in range(256) for mode in range(8)]
 inputs.extend((flags,mode,alias) for flags in (0,1,2,4,8,16,31,255) for mode in range(8) for alias in (0,0xa0000000))
 for n,(flags,mode,alias) in enumerate(inputs):
  actual=bytearray(ram);expected=bytearray(ram);entity=alias|0x0cc00000;appearance=alias|0x0cc10000;node=alias|0x0cc20000
  for memory in (actual,expected):
   store(memory,entity+0xec,appearance);store(memory,entity+0x2d4,node);memory[(appearance+0x34)&0xffffff]=flags
   for j in range(13):store(memory,node+j*4,0xaabb0000+j*0x101)
  state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[5]=entity;state.r[15]=0x8cf00000;state.sr=rng.getrandbits(32);state.pr=0x8cfed000;state.pc=ENTRY
  initial=State.from_buffer_copy(state);other=State.from_buffer_copy(state)
  fpu=Fpu();fpu.fpscr=0x40000|(mode<<19)
  for bank in range(2):
   for i in range(16):fpu.banks[bank][i]=rng.getrandbits(32)
  reference_fpu=Fpu.from_buffer_copy(fpu);path=[]
  def trace(pc,*_):assert pc in want,hex(pc);path.append(pc)
  reference(expected,other,fpu=reference_fpu,trace=trace,instruction_budget=200)
  status=fn((ctypes.c_uint8*len(actual)).from_buffer(actual),ctypes.byref(state),ctypes.byref(fpu))
  assert status==0 and actual==expected and bytes(state)==bytes(other) and bytes(fpu)==bytes(reference_fpu),(flags,mode,hex(alias),'original state')
  for off in (0x20,0x24,0x28):assert load(actual,node+off)==0x3e999999
  assert load(actual,node+0x2c)==(roots[0] if mode&2 else 0xaabb0000+11*0x101)
  first=next((i for i,b in enumerate((16,8,4,2,1)) if flags&b),None)
  selected=load(ram,roots[first]+4) if first is not None else 0xaabb0101
  assert load(actual,node+4)==selected,(flags,'independent priority')
  assert state.r[15]==initial.r[15] and state.pr==initial.pr and state.sr==(initial.sr&~1)|int(first is None)
  writes[str((16,8,4,2,1)[first]) if first is not None else 'unchanged']+=1
  covered.update(path)
  for i,a in enumerate(path[:-1]):
   w=struct.unpack_from('<H',binary,a-BASE)[0]
   if w>>8 in (0x89,0x8b):edges.add((a,path[i+1]))
  records.append(dict(flags=flags,fpscr_mode=hex(0x40000|(mode<<19)),ram_alias=hex(alias),steps=len(path),selected_model=hex(selected),full_state_match=True,independent_model_scale_checks=True))
  if (n+1)%512==0:print(n+1,'original selector comparisons passed',flush=True)
 assert covered==want and len(edges)==10
 files=[Path(__file__),SOURCE,LISTING,PROJECT/'analysis/overlay-face-model-selector/diagnostics.tsv',PROJECT/'ghidra/export_overlay_face_model_selector.ps1',PROJECT/'ghidra/ExportFailedFunctionEvidence.java',PROJECT/'build_overlay_face_model_selector.ps1',PROJECT/'source/fpu_cleanup_transfer.h',MAIN,MODULE]
 report=dict(module='AL_MAIN.BIN',load_address=hex(BASE),address=hex(ENTRY),end_exclusive=hex(END),cases_passed=len(records),instruction_rows_covered=len(covered),conditional_edges_covered=len(edges),external_call_sites=0,selection_outcomes=dict(writes),retail_span_sha256=hashlib.sha256(binary[ENTRY-BASE:END-BASE]).hexdigest(),source='source/'+SOURCE.name,source_sha256=digest(SOURCE),sha256={str(p):digest(p) for p in files},dll_sha256=digest(DLL),newly_recovered_entries=1,scope='Complete 88-row object scale/model selector against original full RAM/CPU/FPU state, all 256 flag bytes and all FR/SZ/PR combinations, physical/cached/uncached entity aliases, independent priority/scale/retained-mesh assertions. Original bit-16 branch selects the inventoried face-strip model.',limitations=['Constructed valid entity/appearance/object pointers and runtime state. No hardware alignment/fault handling, actual caller scheduling, full scene/GPU output or complete gameplay proof. Wide FMOV retains the original overlapping stores rather than repairing them.'],cases=records)
 (PROJECT/'analysis/overlay_face_model_selector_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','instruction_rows_covered','conditional_edges_covered','selection_outcomes')},indent=2))
if __name__=='__main__':main()
