import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference
ROOT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c666e06;LENGTH=10;POINTER_LITERAL=0x8c666e4c;RAM_SIZE=16*1024*1024
binary=(ROOT.parent.parent/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH];assert len(span)==LENGTH
lib=ctypes.CDLL(str(ROOT.parent.parent/'work'/'sonic-adventure'/'build'/'global_counter_increment_8c666e06.dll'));native=lib.sa_global_counter_increment_8c666e06;native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);targets=[0x8c730000,POINTER_LITERAL,FUNCTION,0x8c000000,0x8cfffffc,0x8c720000,0x8c666e52];cases=[]
for case in range(128):
 ram=bytearray(RAM_SIZE);ram[BASE-0x8c000000:BASE-0x8c000000+len(binary)]=binary
 target=targets[case%len(targets)] if case< len(targets) else 0x8c740000+(case-len(targets))*4;target&=0xffffffff;off=(target&0x1fffffff)-0x0c000000;assert 0<=off<=RAM_SIZE-4
 if not BASE<=target<BASE+len(binary) and target!=POINTER_LITERAL:struct.pack_into('<I',ram,off,rng.getrandbits(32))
 struct.pack_into('<I',ram,POINTER_LITERAL-0x8c000000,target)
 s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.sr=rng.getrandbits(32);s.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);s.pc=FUNCTION
 er=bytearray(ram);es=State.from_buffer_copy(s);reference(er,es)
 ar=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(ar);actual=State.from_buffer_copy(s);native(view,ctypes.byref(actual))
 assert ar==er,f'RAM mismatch case={case} target={target:#x}';assert bytes(actual)==bytes(es),f'CPU mismatch case={case}: '+str([(i,hex(actual.r[i]),hex(es.r[i])) for i in range(16) if actual.r[i]!=es.r[i]])
 cases.append({'case':case,'target':hex(target),'ram_and_cpu_match':True})
source=ROOT/'source'/'sa_global_counter_increment_8c666e06.c';report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'binary_sha256':hashlib.sha256(binary).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPRs, SR, PR, and return PC','scope':'Exact 10-byte retail pointer-indirect global increment; tests wraparound, pointer-literal aliases, executable-memory aliases, and RAM boundaries.','limitations':['The global counter’s game meaning and update lifecycle remain unidentified.'],'cases':cases}
(ROOT/'analysis'/'global_counter_increment_8c666e06_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{len(cases)} global counter increment full RAM/CPU comparisons passed')
