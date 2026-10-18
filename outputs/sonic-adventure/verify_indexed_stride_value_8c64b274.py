import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference

PROJECT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c64b274;LENGTH=24;RAM_SIZE=16*1024*1024
binary=(ROOT/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH]
assert len(span)==LENGTH
native=ctypes.CDLL(str(ROOT/'work'/'sonic-adventure'/'build'/'indexed_stride_value_8c64b274.dll')).sa_indexed_stride_value_8c64b274
native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State)];native.restype=None
rng=random.Random(FUNCTION);indices=[0,1,2,7,31,63,127,255,0xfffffffe,0xffffffff];cases=[]
stride=0x148;table=0x8c89c550
for case in range(128):
    ram=bytearray(RAM_SIZE);ram[0x10000:0x10000+len(binary)]=binary
    index=indices[case%len(indices)];address=(table+index*stride+0x28)&0xffffffff
    offset=(address&0x1fffffff)-0x0c000000
    struct.pack_into('<I',ram,offset,rng.getrandbits(32))
    initial=State();initial.r[:]=[rng.getrandbits(32) for _ in range(16)]
    initial.r[4]=index;initial.r[5]=rng.choice([0,1,0x1fffffff,0x20000000,0x7fffffff,0x80000000,0xffffffff,rng.getrandbits(32)])
    initial.sr=rng.getrandbits(32);initial.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);initial.pc=FUNCTION
    expected=State.from_buffer_copy(initial);expected_ram=bytearray(ram);steps=reference(expected_ram,expected)
    actual=State.from_buffer_copy(initial);actual_ram=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(actual_ram);native(view,ctypes.byref(actual))
    assert actual_ram==expected_ram,f'RAM mismatch case {case}'
    assert bytes(actual)==bytes(expected),f'CPU mismatch case {case}: r0-r5={list(map(hex,actual.r[:6]))} != {list(map(hex,expected.r[:6]))}; SR={actual.sr:#x}/{expected.sr:#x}'
    cases.append({'case':case,'index':index,'value_argument':initial.r[5],'retail_steps':steps,'ram_and_cpu_match':True})

source=PROJECT/'source'/'sa_indexed_stride_value_8c64b274.c'
report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'stride_literal_address':'0x8c64b2e0','table_base_literal_address':'0x8c64b2e4','stride':stride,'field_offset':'0x28','cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC state','scope':'Computes the table entry address from R4 and the literal stride/base, reads the word at entry+0x28, adds R5<<5, and returns the result while preserving retail temporary-register and shift-carry effects.','limitations':['The semantic identity and valid index range of the table are not established by this leaf function.','Synthetic interpreter comparisons are not full-game validation.'],'cases':cases}
(PROJECT/'analysis'/'indexed_stride_value_8c64b274_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(f'{len(cases)} indexed stride value RAM/CPU comparisons passed')
