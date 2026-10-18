import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference
ROOT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c10c10c;LENGTH=10;COUNTER=0x8c10c1bc;RAM_SIZE=16*1024*1024
binary=(ROOT.parent.parent/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH];assert len(span)==LENGTH
lib=ctypes.CDLL(str(ROOT.parent.parent/'work'/'sonic-adventure'/'build'/'delay_counter_loop_8c10c10c.dll'));native=lib.sa_delay_counter_loop_8c10c10c;native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);counts=[1,2,3,4,7,8,9,15,16,31,32,63,64,127,255,511,1000];cases=[]
for case in range(64):
 ram=bytearray(RAM_SIZE);ram[BASE-0x8c000000:BASE-0x8c000000+len(binary)]=binary;count=counts[case%len(counts)] if case<len(counts) else rng.randint(1,1000);struct.pack_into('<I',ram,COUNTER-0x8c000000,count)
 s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.sr=rng.getrandbits(32);s.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);s.pc=FUNCTION
 er=bytearray(ram);es=State.from_buffer_copy(s);reference(er,es,instruction_budget=count*8+100)
 ar=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(ar);actual=State.from_buffer_copy(s);native(view,ctypes.byref(actual))
 assert ar==er,f'RAM mismatch case={case} count={count}';assert bytes(actual)==bytes(es),f'CPU mismatch case={case}: '+str([(i,hex(actual.r[i]),hex(es.r[i])) for i in range(16) if actual.r[i]!=es.r[i]])+f' sr={actual.sr:#x}/{es.sr:#x}'
 cases.append({'case':case,'delay_count':count,'ram_and_cpu_match':True})
source=ROOT/'source'/'sa_delay_counter_loop_8c10c10c.c';report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPRs, SR, PR, and return PC','scope':'Exact 10-byte retail delay loop; compares complete RAM and CPU state for positive countdown values from 1 through 1000.','limitations':['A zero initial counter requires 2^32 decrement iterations in the retail loop and is excluded from bounded interpreter cases. Timing calibration and caller context remain unidentified.'],'cases':cases}
(ROOT/'analysis'/'delay_counter_loop_8c10c10c_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{len(cases)} delay counter loop full RAM/CPU comparisons passed')
