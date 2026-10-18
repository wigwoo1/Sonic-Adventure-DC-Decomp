import ctypes,hashlib,json,random
from verify_startup import ROOT,State,reference
ROOT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c0548e0;LENGTH=20;RAM_SIZE=16*1024*1024
binary=(ROOT.parent.parent/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH];assert len(span)==LENGTH
lib=ctypes.CDLL(str(ROOT.parent.parent/'work'/'sonic-adventure'/'build'/'signed_u8_clamp_8c0548e0.dll'));native=lib.sa_signed_u8_clamp_8c0548e0;native.argtypes=[ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);values=[0,1,254,255,256,257,0x7fffffff,0x80000000,0xffffff00,0xffffffff]+[rng.getrandbits(32) for _ in range(246)];cases=[]
ram=bytearray(RAM_SIZE);ram[BASE-0x8c000000:BASE-0x8c000000+len(binary)]=binary
for case,value in enumerate(values):
 s=State();s.r[:]=[rng.getrandbits(32) for _ in range(16)];s.r[4]=value;s.sr=rng.getrandbits(32);s.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);s.pc=FUNCTION
 es=State.from_buffer_copy(s);reference(bytearray(ram),es)
 actual=State.from_buffer_copy(s);native(ctypes.byref(actual))
 assert bytes(actual)==bytes(es),f'CPU mismatch case={case} value={value:#x}: '+str([(i,hex(actual.r[i]),hex(es.r[i])) for i in range(16) if actual.r[i]!=es.r[i]])+f' sr={actual.sr:#x}/{es.sr:#x}'
 cases.append({'case':case,'input':hex(value),'return_value':hex(actual.r[0]),'cpu_match':True})
source=ROOT/'source'/'sa_signed_u8_clamp_8c0548e0.c';report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'cases_passed':len(cases),'comparison_level':'all GPRs, SR, PR, and return PC; full RAM unchanged','scope':'Exact 20-byte signed-to-unsigned-byte clamp; boundary and randomized inputs compared against the retail interpreter.','limitations':['The caller-specific semantic meaning of the clamped value remains unknown.'],'cases':cases}
(ROOT/'analysis'/'signed_u8_clamp_8c0548e0_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{len(cases)} signed byte clamp CPU comparisons passed')
