import ctypes,hashlib,json,random
from verify_startup import ROOT,State,reference

PROJECT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c6578e0;LENGTH=12;RAM_SIZE=16*1024*1024
binary=(ROOT/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH]
assert len(span)==LENGTH and span==bytes.fromhex('1b d5 52 63 4b 23 32 25 0b 00 33 60')
native=ctypes.CDLL(str(ROOT/'work'/'sonic-adventure'/'build'/'global_mask_accumulate_8c6578e0.dll')).sa_global_mask_accumulate_8c6578e0
native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);cases=[]
for case in range(128):
    ram=bytearray(RAM_SIZE);ram[0x10000:0x10000+len(binary)]=binary
    initial=State();initial.r[:]=[rng.getrandbits(32) for _ in range(16)]
    initial.r[3]=rng.choice([0,1,0x80000000,0xffffffff,rng.getrandbits(32)])
    initial.r[4]=rng.choice([0,1,0x7fffffff,0x80000001,0xffffffff,rng.getrandbits(32)])
    initial.sr=rng.getrandbits(32);initial.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);initial.pc=FUNCTION
    expected=State.from_buffer_copy(initial);expected_ram=bytearray(ram);steps=reference(expected_ram,expected)
    actual=State.from_buffer_copy(initial);actual_ram=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(actual_ram);native(view,ctypes.byref(actual))
    assert actual_ram==expected_ram,f'RAM mismatch case {case}'
    assert bytes(actual)==bytes(expected),f'CPU mismatch case {case}: r0/r3/r5={actual.r[0]:#x}/{actual.r[3]:#x}/{actual.r[5]:#x} != {expected.r[0]:#x}/{expected.r[3]:#x}/{expected.r[5]:#x}'
    cases.append({'case':case,'retail_steps':steps,'ram_and_cpu_match':True})

source=PROJECT/'source'/'sa_global_mask_accumulate_8c6578e0.c'
report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'literal_address':'0x8c657950','literal_value':'0x8c67c154','cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC state','scope':'Loads the destination pointer through the retail literal, ORs R4 into the incoming R3 mask, stores the result, returns it in R0, and preserves all other state as the retail instructions do.','limitations':['The call-site meaning of the mask accumulator and destination word is not established by this leaf routine.','Synthetic interpreter comparisons are not full-game validation.'],'cases':cases}
(PROJECT/'analysis'/'global_mask_accumulate_8c6578e0_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(f'{len(cases)} global mask accumulator RAM/CPU comparisons passed')
