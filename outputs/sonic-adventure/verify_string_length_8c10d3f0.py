import ctypes,hashlib,json,random
from verify_startup import ROOT,State,reference
ROOT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c10d3f0;LENGTH=16;RAM_SIZE=16*1024*1024
binary=(ROOT.parent.parent/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH];assert len(span)==LENGTH
lib=ctypes.CDLL(str(ROOT.parent.parent/'work'/'sonic-adventure'/'build'/'string_length_8c10d3f0.dll'));native=lib.sa_string_length_8c10d3f0;native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);lengths=[0,1,2,3,7,8,15,16,31,32,63,64,127,255,1024];cases=[]
for case in range(128):
 ram=bytearray(RAM_SIZE);ram[BASE-0x8c000000:BASE-0x8c000000+len(binary)]=binary
 if case==0:start=0x8c000000
 elif case==1:start=0x8cffffff-1024
 elif case==2:start=FUNCTION+0x80
 else:start=0x8c740000+case*0x1000
 wanted=lengths[case%len(lengths)] if case<len(lengths) else rng.randrange(0,512)
 off=(start&0x1fffffff)-0x0c000000;assert 0<=off and off+wanted<RAM_SIZE
 for i in range(wanted):ram[off+i]=rng.choice([1,65,127,128,255])
 ram[off+wanted]=0
 s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=start;s.sr=rng.getrandbits(32);s.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);s.pc=FUNCTION
 er=bytearray(ram);es=State.from_buffer_copy(s);reference(er,es,instruction_budget=wanted*8+100)
 ar=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(ar);actual=State.from_buffer_copy(s);native(view,ctypes.byref(actual))
 assert ar==er,f'RAM mismatch case={case} length={wanted}';assert bytes(actual)==bytes(es),f'CPU mismatch case={case} length={wanted}: '+str([(i,hex(actual.r[i]),hex(es.r[i])) for i in range(16) if actual.r[i]!=es.r[i]])+f' sr={actual.sr:#x}/{es.sr:#x}'
 cases.append({'case':case,'string_length':wanted,'start':hex(start),'ram_and_cpu_match':True})
source=ROOT/'source'/'sa_string_length_8c10d3f0.c';report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC state','scope':'Exact 16-byte retail string-length helper; tests empty, varied, long, code-backed, and boundary-adjacent strings.','limitations':['String encoding and caller length constraints remain unidentified.'],'cases':cases}
(ROOT/'analysis'/'string_length_8c10d3f0_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{len(cases)} string length RAM/CPU comparisons passed')
