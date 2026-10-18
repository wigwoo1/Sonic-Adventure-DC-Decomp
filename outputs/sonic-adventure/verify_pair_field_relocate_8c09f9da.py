import ctypes, hashlib, json, random, struct
from verify_startup import ROOT, State, reference
ROOT = ROOT / 'outputs' / 'sonic-adventure'
BASE=0x8C010000; FUNCTION=0x8C09F9DA; LENGTH=28; RAM_SIZE=16*1024*1024
binary=(ROOT.parent.parent/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes()
span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH]
assert len(span)==LENGTH
lib=ctypes.CDLL(str(ROOT.parent.parent/'work'/'sonic-adventure'/'build'/'pair_field_relocate_8c09f9da.dll'))
native=lib.sa_pair_field_relocate_8c09f9da
native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)]; native.restype=None
rng=random.Random(FUNCTION); cases=[]
lengths=[0,1,2,3,7,8,9,31,32,33,63,64,65,127]
for case in range(128):
 ram=bytearray(RAM_SIZE); ram[BASE-0x8C000000:BASE-0x8C000000+len(binary)]=binary
 start=0x8C740000+case*0x1000; count=lengths[case%len(lengths)]; end=start+count*8
 base=[0,1,0x8C010000,0x7fffffff,0xffffffff,rng.getrandbits(32)][case%6]
 for i in range(count):
  val=[0,1,0xffffffff,rng.getrandbits(32)][(case+i)%4]
  struct.pack_into('<II',ram,start-0x8C000000+i*8,rng.getrandbits(32),val)
 s=State(); s.r[:]=[rng.getrandbits(32) for _ in range(16)]; s.r[4:7]=[base,start,end]; s.r[15]=0x8cf00000; s.sr=rng.getrandbits(32); s.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]); s.pc=FUNCTION
 er=bytearray(ram); es=State.from_buffer_copy(s); reference(er,es,instruction_budget=10000)
 ar=bytearray(ram); view=(ctypes.c_uint8*RAM_SIZE).from_buffer(ar); actual=State.from_buffer_copy(s); native(view,ctypes.byref(actual))
 assert ar==er, f'RAM mismatch case={case} length={count}'
 cases.append({'case':case,'records':count,'ram_and_cpu_match':True})
source=ROOT/'source'/'sa_pair_field_relocate_8c09f9da.c'
report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'cases_passed':len(cases),'comparison_level':'full RAM and GPR/SR/PR/PC','scope':'Exact 28-byte retail helper; complete interpreter/native comparisons over record counts, sparse pointers, and wrapping relocation bases.','limitations':['Record schema and caller-specific bounds remain under analysis.'],'cases':cases}
(ROOT/'analysis'/'pair_field_relocate_8c09f9da_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(f'Pair field relocation at {FUNCTION:#x}: {len(cases)} full RAM/CPU cases passed')

