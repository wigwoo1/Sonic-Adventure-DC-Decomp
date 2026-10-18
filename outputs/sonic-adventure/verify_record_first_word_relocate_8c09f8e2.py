import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference
ROOT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c09f8e2;LENGTH=30;RAM_SIZE=16*1024*1024
binary=(ROOT.parent.parent/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH];assert len(span)==LENGTH
lib=ctypes.CDLL(str(ROOT.parent.parent/'work'/'sonic-adventure'/'build'/'record_first_word_relocate_8c09f8e2.dll'));native=lib.sa_record_first_word_relocate_8c09f8e2;native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);cases=[];counts=[0,1,2,3,7,8,9,31,32,33,63,64,65,127]
for case in range(128):
 ram=bytearray(RAM_SIZE);ram[BASE-0x8c000000:BASE-0x8c000000+len(binary)]=binary
 start=0x8c740000+case*0x1000;count=counts[case%len(counts)];end=start+count*12;base=[0,1,0x8c010000,0x7fffffff,0xffffffff,rng.getrandbits(32)][case%6]
 for i in range(count):
  value=[0,1,0xffffffff,rng.getrandbits(32)][(case+i)%4]
  struct.pack_into('<III',ram,start-0x8c000000+i*12,value,rng.getrandbits(32),rng.getrandbits(32))
 s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=base;s.r[5]=start;s.r[6]=end;s.r[7]=(start-0x53)&0xffffffff;s.r[15]=0x8cf00000;s.sr=rng.getrandbits(32);s.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);s.pc=FUNCTION
 er=bytearray(ram);es=State.from_buffer_copy(s);reference(er,es,instruction_budget=10000)
 ar=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(ar);actual=State.from_buffer_copy(s);native(view,ctypes.byref(actual))
 assert ar==er,f'RAM mismatch case={case} records={count}'
 assert bytes(actual)==bytes(es),f'CPU mismatch case={case}: '+str([(i,hex(actual.r[i]),hex(es.r[i])) for i in range(16) if actual.r[i]!=es.r[i]])+f' sr={actual.sr:#x}/{es.sr:#x}'
 cases.append({'case':case,'records':count,'ram_and_cpu_match':True})
source=ROOT/'source'/'sa_record_first_word_relocate_8c09f8e2.c';report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'cases_passed':len(cases),'comparison_level':'full RAM and GPR/SR/PR/PC','scope':'Exact 30-byte retail routine; full interpreter/native comparisons across record counts, sparse pointers, and wrapping relocation bases.','limitations':['Caller parameter meaning and 12-byte record schema remain under analysis.'],'cases':cases}
(ROOT/'analysis'/'record_first_word_relocate_8c09f8e2_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'Record first-word relocation at {FUNCTION:#x}: {len(cases)} full RAM/CPU cases passed')
