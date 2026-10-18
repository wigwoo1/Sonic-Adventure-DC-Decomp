import ctypes,hashlib,json,random
from verify_startup import ROOT,State,reference
ROOT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c10c970;LENGTH=14;RAM_SIZE=16*1024*1024
binary=(ROOT.parent.parent/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH];assert len(span)==LENGTH
lib=ctypes.CDLL(str(ROOT.parent.parent/'work'/'sonic-adventure'/'build'/'nullable_signed_byte_get_8c10c970.dll'));native=lib.sa_nullable_signed_byte_get_8c10c970;native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);objects=[0,0x8c740000,0x8c000000,0x8cffffff-1,FUNCTION,0x8c10c970+0x40];values=[0,1,0x7f,0x80,0xff];cases=[]
def off(a):return (a&0x1fffffff)-0x0c000000
for case in range(128):
 ram=bytearray(RAM_SIZE);ram[BASE-0x8c000000:BASE-0x8c000000+len(binary)]=binary
 if case<len(objects):obj=objects[case]
 elif case%7==0:obj=0
 else:obj=0x8c740000+(case-6)*0x1000
 if obj:
  at=off(obj+1);assert 0<=at<RAM_SIZE,(case,hex(obj),hex(at))
  if not(BASE<=obj+1<BASE+len(binary)):ram[at]=values[case%len(values)] if case<len(objects)+len(values) else rng.getrandbits(8)
 s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=obj;s.sr=rng.getrandbits(32);s.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);s.pc=FUNCTION
 er=bytearray(ram);es=State.from_buffer_copy(s);reference(er,es)
 ar=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(ar);actual=State.from_buffer_copy(s);native(view,ctypes.byref(actual))
 assert ar==er,f'RAM mismatch case={case} obj={obj:#x}';assert bytes(actual)==bytes(es),f'CPU mismatch case={case}: '+str([(i,hex(actual.r[i]),hex(es.r[i])) for i in range(16) if actual.r[i]!=es.r[i]])+f' sr={actual.sr:#x}/{es.sr:#x}'
 cases.append({'case':case,'object':hex(obj),'value':hex(actual.r[0]),'ram_and_cpu_match':True})
source=ROOT/'source'/'sa_nullable_signed_byte_get_8c10c970.c';report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC state','scope':'Exact 14-byte nullable signed-byte getter; tests null sentinel, negative and positive bytes, code-backed data, and mapped boundaries.','limitations':['The object field meaning and caller-defined null handling remain unidentified.'],'cases':cases}
(ROOT/'analysis'/'nullable_signed_byte_get_8c10c970_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{len(cases)} nullable signed-byte getter RAM/CPU comparisons passed')
