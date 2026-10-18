import ctypes,hashlib,json,random
from verify_startup import ROOT,State,reference

PROJECT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c608914;LENGTH=16;RAM_SIZE=16*1024*1024
binary=(ROOT/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH]
assert len(span)==LENGTH and span==bytes.fromhex('28 46 52 24 74 14 f2 63 35 14 0b 00 61 14 03 d3')
native=ctypes.CDLL(str(ROOT/'work'/'sonic-adventure'/'build'/'initialize_record_fields_8c608914.dll')).sa_initialize_record_fields_8c608914
native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);cases=[]
for case in range(96):
    ram=bytearray(RAM_SIZE);ram[0x10000:0x10000+len(binary)]=binary
    record=0x8c780000+case*0x40;stack=0x8c790000+case*0x40
    # Seed the full record and argument stack so exact changed bytes and alias-sensitive ordering are checked.
    at=record-0x8c000000;ram[at:at+32]=bytes(rng.getrandbits(8) for _ in range(32))
    stack_at=stack-0x8c000000;ram[stack_at:stack_at+32]=bytes(rng.getrandbits(8) for _ in range(32))
    initial=State();initial.r[:]=[rng.getrandbits(32) for _ in range(16)]
    initial.r[4]=record;initial.r[5]=rng.getrandbits(32);initial.r[6]=rng.getrandbits(32);initial.r[7]=rng.getrandbits(32);initial.r[15]=stack
    initial.sr=rng.getrandbits(32);initial.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);initial.pc=FUNCTION
    expected=State.from_buffer_copy(initial);expected_ram=bytearray(ram);steps=reference(expected_ram,expected)
    actual=State.from_buffer_copy(initial);actual_ram=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(actual_ram);native(view,ctypes.byref(actual))
    assert actual_ram==expected_ram,f'RAM mismatch case {case}'
    assert bytes(actual)==bytes(expected),f'CPU mismatch case {case}: r3/r6={actual.r[3]:#x}/{actual.r[6]:#x} != {expected.r[3]:#x}/{expected.r[6]:#x}'
    cases.append({'case':case,'retail_steps':steps,'ram_and_cpu_match':True})

source=PROJECT/'source'/'sa_initialize_record_fields_8c608914.c'
report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC state','scope':'Initializes record words 0, 1, 4, and 5 from R5, R6<<16, R7, and the stack argument at [R15], preserving exact store ordering and R3/R6 side effects.','limitations':['The semantic type and call-site role of this record are not established by the leaf function alone.','Synthetic interpreter comparisons are not full-game validation.'],'cases':cases}
(PROJECT/'analysis'/'initialize_record_fields_8c608914_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(f'{len(cases)} record field initializer RAM/CPU comparisons passed')
