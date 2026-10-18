import ctypes,hashlib,json,random
from verify_startup import ROOT,State,reference

PROJECT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c10c116;LENGTH=24;RAM_SIZE=16*1024*1024
binary=(ROOT/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH]
assert len(span)==LENGTH
native=ctypes.CDLL(str(ROOT/'work'/'sonic-adventure'/'build'/'range_nonzero_validate_8c10c116.dll')).sa_range_nonzero_validate_8c10c116
native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);values=[-0x80000000,-2,-1,0,1,2,127,254,255,256,257,0x7fffffff]
cases=[]
for case in range(128):
    ram=bytearray(RAM_SIZE);ram[0x10000:0x10000+len(binary)]=binary
    initial=State();initial.r[:]=[rng.getrandbits(32) for _ in range(16)]
    initial.r[4]=values[case%len(values)]&0xffffffff;initial.r[5]=rng.choice([0,1,0xffffffff])
    initial.sr=rng.getrandbits(32);initial.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);initial.pc=FUNCTION
    expected=State.from_buffer_copy(initial);expected_ram=bytearray(ram);steps=reference(expected_ram,expected)
    actual=State.from_buffer_copy(initial);actual_ram=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(actual_ram)
    native(view,ctypes.byref(actual))
    assert actual_ram==expected_ram,f'RAM mismatch case {case}'
    assert bytes(actual)==bytes(expected),f'CPU mismatch case {case}: r0/r2={actual.r[0]:#x}/{actual.r[2]:#x} != {expected.r[0]:#x}/{expected.r[2]:#x}; SR={actual.sr:#x}/{expected.sr:#x}'
    cases.append({'case':case,'argument_index':initial.r[4],'required_value':initial.r[5],'retail_steps':steps,'ram_and_cpu_match':True})

source=PROJECT/'source'/'sa_range_nonzero_validate_8c10c116.c'
report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'limit_literal_address':'0x8c10c1b8','limit':256,'cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC state','scope':'Validates signed index in [0,256) and a nonzero second parameter. Returns 0 on success or -3 on failure; differential tests cover signed boundaries, range edges, both flag outcomes, and arbitrary initial register/PR state.','limitations':['The caller-level meaning of the two parameters and error code is not established from this routine alone.','Synthetic interpreter comparisons are not full-game validation.'],'cases':cases}
(PROJECT/'analysis'/'range_nonzero_validate_8c10c116_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(f'{len(cases)} range/nonzero validation RAM/CPU comparisons passed')
