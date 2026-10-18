import ctypes,hashlib,json,random
from verify_startup import ROOT,State,reference

PROJECT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c646eee;LENGTH=16;RAM_SIZE=16*1024*1024
binary=(ROOT/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH]
assert len(span)==LENGTH and span==bytes.fromhex('1f e3 36 34 02 89 1b d0 08 44 56 04 0b 00 09 00')
native=ctypes.CDLL(str(ROOT/'work'/'sonic-adventure'/'build'/'bounded_array_store_8c646eee.dll')).sa_bounded_array_store_8c646eee
native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);indices=[0,1,2,30,31,32,33,0x7fffffff,0x80000000,0xffffffff];cases=[]
for case in range(128):
    ram=bytearray(RAM_SIZE);ram[0x10000:0x10000+len(binary)]=binary
    base=0x8c893b50;at=base-0x8c000000;ram[at-16:at+32*4+16]=bytes(rng.getrandbits(8) for _ in range(32*4+32))
    initial=State();initial.r[:]=[rng.getrandbits(32) for _ in range(16)]
    initial.r[4]=indices[case%len(indices)];initial.r[5]=rng.getrandbits(32)
    initial.sr=rng.getrandbits(32);initial.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);initial.pc=FUNCTION
    expected=State.from_buffer_copy(initial);expected_ram=bytearray(ram);steps=reference(expected_ram,expected)
    actual=State.from_buffer_copy(initial);actual_ram=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(actual_ram);native(view,ctypes.byref(actual))
    assert actual_ram==expected_ram,f'RAM mismatch case {case}'
    assert bytes(actual)==bytes(expected),f'CPU mismatch case {case}: r0/r3/r4={actual.r[0]:#x}/{actual.r[3]:#x}/{actual.r[4]:#x} != {expected.r[0]:#x}/{expected.r[3]:#x}/{expected.r[4]:#x}; SR={actual.sr:#x}/{expected.sr:#x}'
    cases.append({'case':case,'index':initial.r[4],'retail_steps':steps,'ram_and_cpu_match':True})

source=PROJECT/'source'/'sa_bounded_array_store_8c646eee.c'
report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'base_literal_address':'0x8c646f64','array_base':'0x8c893b50','maximum_index':31,'cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC state','scope':'Stores R5 into a 32-entry word array only when the unsigned R4 index is below 32. Differential cases verify valid indices, rejection boundaries, high-bit indices, all CPU side effects, and surrounding RAM.','limitations':['The semantic identity of the 32-entry array is not established by this leaf function.','Synthetic interpreter comparisons are not full-game validation.'],'cases':cases}
(PROJECT/'analysis'/'bounded_array_store_8c646eee_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(f'{len(cases)} bounded array store RAM/CPU comparisons passed')
