import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference
ROOT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c6611a8;LENGTH=20;MASK_LITERAL=0x8c661290;ARRAY_LITERAL=0x8c661298;RAM_SIZE=16*1024*1024
binary=(ROOT.parent.parent/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH];assert len(span)==LENGTH
lib=ctypes.CDLL(str(ROOT.parent.parent/'work'/'sonic-adventure'/'build'/'masked_array_store_8c6611a8.dll'));native=lib.sa_masked_array_store_8c6611a8;native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);special=[(0x8c740000,0),(0x8cfffff8,1),(FUNCTION,0),(ARRAY_LITERAL,0),(0x8c000000,0),(0x8c740000,0xffffffff),(0x8c740000,0x7fffffff),(0x8c740000,0x80000000)];cases=[]
def off(a):return (a&0x1fffffff)-0x0c000000
for case in range(128):
 ram=bytearray(RAM_SIZE);ram[BASE-0x8c000000:BASE-0x8c000000+len(binary)]=binary
 if case<len(special):array,index=special[case]
 else:array=0x8c740000+(case-8)*0x100;index=rng.randrange(32)
 mask=[0,1,0x400000,0x01000000,0xffffffff][case%5] if case<5 else rng.getrandbits(32)
 address=(array+index*4)&0xffffffff;ai=off(address);enabled=(rng.getrandbits(32)&mask)!=0
 if enabled:
  assert 0<=ai<=RAM_SIZE-4,(case,hex(array),hex(index),hex(address),hex(ai))
  if not(BASE<=address<BASE+len(binary)) and address not in [MASK_LITERAL,ARRAY_LITERAL]:struct.pack_into('<I',ram,ai,rng.getrandbits(32))
 struct.pack_into('<I',ram,MASK_LITERAL-0x8c000000,mask);struct.pack_into('<I',ram,ARRAY_LITERAL-0x8c000000,array)
 s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=rng.getrandbits(32);s.r[5]=index;s.r[6]=rng.getrandbits(32);s.sr=rng.getrandbits(32);s.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);s.pc=FUNCTION
 er=bytearray(ram);es=State.from_buffer_copy(s);reference(er,es)
 ar=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(ar);actual=State.from_buffer_copy(s);native(view,ctypes.byref(actual))
 assert ar==er,f'RAM mismatch case={case} mask={mask:#x} index={index:#x}';assert bytes(actual)==bytes(es),f'CPU mismatch case={case}: '+str([(i,hex(actual.r[i]),hex(es.r[i])) for i in range(16) if actual.r[i]!=es.r[i]])+f' sr={actual.sr:#x}/{es.sr:#x}'
 cases.append({'case':case,'mask':hex(mask),'array':hex(array),'index':hex(index),'enabled':enabled,'ram_and_cpu_match':True})
source=ROOT/'source'/'sa_masked_array_store_8c6611a8.c';report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC state','scope':'Exact 20-byte retail conditional array store; varied masks, array bases, indices, stored values, and code/literal aliases.','limitations':['The array contents, mask meaning, and caller index range remain unidentified.'],'cases':cases}
(ROOT/'analysis'/'masked_array_store_8c6611a8_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{len(cases)} masked array-store RAM/CPU comparisons passed')
