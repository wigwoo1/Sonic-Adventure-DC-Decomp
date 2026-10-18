import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference
ROOT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c10c962;LENGTH=14;RAM_SIZE=16*1024*1024
binary=(ROOT.parent.parent/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH];assert len(span)==LENGTH
lib=ctypes.CDLL(str(ROOT.parent.parent/'work'/'sonic-adventure'/'build'/'nullable_field_get_8c10c962.dll'));native=lib.sa_nullable_field_get_8c10c962;native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);pointers=[0,0x8c740000,0x8c000000,0x8cffffff-0x27,FUNCTION,0x8c10c962+0x40];cases=[]
def off(a):return (a&0x1fffffff)-0x0c000000
for case in range(128):
 ram=bytearray(RAM_SIZE);ram[BASE-0x8c000000:BASE-0x8c000000+len(binary)]=binary
 if case<len(pointers):obj=pointers[case]
 elif case%7==0:obj=0
 else:obj=0x8c740000+(case-6)*0x1000
 target=(obj+0x24)&0xffffffff
 if obj:
  at=off(target);assert 0<=at<=RAM_SIZE-4,(case,hex(obj),hex(target),hex(at))
  if not(BASE<=target<BASE+len(binary)):struct.pack_into('<I',ram,at,rng.getrandbits(32))
 s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=obj;s.sr=rng.getrandbits(32);s.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);s.pc=FUNCTION
 er=bytearray(ram);es=State.from_buffer_copy(s);reference(er,es)
 ar=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(ar);actual=State.from_buffer_copy(s);native(view,ctypes.byref(actual))
 assert ar==er,f'RAM mismatch case={case} obj={obj:#x}';assert bytes(actual)==bytes(es),f'CPU mismatch case={case}: '+str([(i,hex(actual.r[i]),hex(es.r[i])) for i in range(16) if actual.r[i]!=es.r[i]])+f' sr={actual.sr:#x}/{es.sr:#x}'
 cases.append({'case':case,'object':hex(obj),'value':hex(actual.r[0]),'ram_and_cpu_match':True})
source=ROOT/'source'/'sa_nullable_field_get_8c10c962.c';report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC state','scope':'Exact 14-byte nullable field getter; tests null sentinel, object field loads, code-backed data, and mapped boundaries.','limitations':['The object field meaning and caller-defined null handling remain unidentified.'],'cases':cases}
(ROOT/'analysis'/'nullable_field_get_8c10c962_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{len(cases)} nullable field getter RAM/CPU comparisons passed')

