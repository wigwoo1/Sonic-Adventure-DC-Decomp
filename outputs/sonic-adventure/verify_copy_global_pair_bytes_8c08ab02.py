import ctypes,hashlib,json,random
from verify_startup import ROOT,State,reference

PROJECT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c08ab02;LENGTH=14;RAM_SIZE=16*1024*1024
binary=(ROOT/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH]
assert len(span)==LENGTH and span==bytes.fromhex('28 d2 28 d0 20 63 30 25 00 61 0b 00 10 24')
native=ctypes.CDLL(str(ROOT/'work'/'sonic-adventure'/'build'/'copy_global_pair_bytes_8c08ab02.dll')).sa_copy_global_pair_bytes_8c08ab02
native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);patterns=[(0,0),(1,0x7f),(0x80,0xff),(0xff,0x80),(0x7f,1),(0x55,0xaa)];cases=[]
for case in range(96):
    ram=bytearray(RAM_SIZE);ram[0x10000:0x10000+len(binary)]=binary
    ram[0x752be1],ram[0x752be2]=patterns[case%len(patterns)]
    dest_a=0x8c760100+case*2;dest_b=dest_a+1
    initial=State();initial.r[:]=[rng.getrandbits(32) for _ in range(16)]
    initial.r[4]=dest_a;initial.r[5]=dest_b;initial.sr=rng.getrandbits(32)
    initial.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);initial.pc=FUNCTION
    expected=State.from_buffer_copy(initial);expected_ram=bytearray(ram);steps=reference(expected_ram,expected)
    actual=State.from_buffer_copy(initial);actual_ram=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(actual_ram);native(view,ctypes.byref(actual))
    assert actual_ram==expected_ram,f'RAM mismatch case {case}'
    assert bytes(actual)==bytes(expected),f'CPU mismatch case {case}: r0-r5={list(map(hex,actual.r[:6]))} != {list(map(hex,expected.r[:6]))}'
    cases.append({'case':case,'source_bytes':list(patterns[case%len(patterns)]),'retail_steps':steps,'ram_and_cpu_match':True})

source=PROJECT/'source'/'sa_copy_global_pair_bytes_8c08ab02.c'
report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'source_pointer_literals':{'first':'0x8c08aba4','second':'0x8c08aba8'},'source_addresses':['0x8c752be2','0x8c752be1'],'cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC state','scope':'Copies the signed-byte values from two literal-loaded source addresses to the caller pointers in R5 and R4, respectively, reproducing sign-extended register effects and RTS delay-slot behavior.','limitations':['The semantic identities of the two source bytes are not established from this leaf function.','Synthetic interpreter comparisons are not full-game validation.'],'cases':cases}
(PROJECT/'analysis'/'copy_global_pair_bytes_8c08ab02_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(f'{len(cases)} global pair byte copy RAM/CPU comparisons passed')
