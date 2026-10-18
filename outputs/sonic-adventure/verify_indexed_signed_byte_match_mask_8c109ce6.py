import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference

PROJECT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c109ce6;LENGTH=24;RAM_SIZE=16*1024*1024
binary=(ROOT/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH]
assert len(span)==LENGTH
native=ctypes.CDLL(str(ROOT/'work'/'sonic-adventure'/'build'/'indexed_signed_byte_match_mask_8c109ce6.dll')).sa_indexed_signed_byte_match_mask_8c109ce6
native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);bytes_to_check=[0,1,7,8,0x7f,0x80,0xff];cases=[]
for case in range(128):
    ram=bytearray(RAM_SIZE);ram[0x10000:0x10000+len(binary)]=binary
    initial=State();initial.r[:]=[rng.getrandbits(32) for _ in range(16)]
    initial.r[4]=0x8c780000;initial.sr=rng.getrandbits(32);initial.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);initial.pc=FUNCTION
    global_value=rng.choice([0,1,7,8,0xffffffff,rng.getrandbits(32)])
    field=bytes_to_check[case%len(bytes_to_check)]
    struct.pack_into('<I',ram,0x7a64e4,global_value);ram[0x780602]=field
    expected=State.from_buffer_copy(initial);expected_ram=bytearray(ram);steps=reference(expected_ram,expected)
    actual=State.from_buffer_copy(initial);actual_ram=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(actual_ram);native(view,ctypes.byref(actual))
    assert actual_ram==expected_ram,f'RAM mismatch case {case}'
    assert bytes(actual)==bytes(expected),f'CPU mismatch case {case}: r0/r2/r3={actual.r[0]:#x}/{actual.r[2]:#x}/{actual.r[3]:#x} != {expected.r[0]:#x}/{expected.r[2]:#x}/{expected.r[3]:#x}; SR={actual.sr:#x}/{expected.sr:#x}'
    cases.append({'case':case,'global_word':global_value,'field_byte':field,'retail_steps':steps,'ram_and_cpu_match':True})

source=PROJECT/'source'/'sa_indexed_signed_byte_match_mask_8c109ce6.c'
report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'pointer_literal_address':'0x8c109de4','global_word_address':'0x8c7a64e4','field_offset':'0x602','mask':'0x7','cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC state','scope':'Compares the low three bits of a global word with the sign-extended byte at R4+0x602; returns 1 or 0 and retains the retail R2/R3/SR.T side effects.','limitations':['The caller-level meaning of the state byte and global word is not established by this routine alone.','Synthetic interpreter comparisons are not full-game validation.'],'cases':cases}
(PROJECT/'analysis'/'indexed_signed_byte_match_mask_8c109ce6_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(f'{len(cases)} signed-byte mask comparison RAM/CPU cases passed')
