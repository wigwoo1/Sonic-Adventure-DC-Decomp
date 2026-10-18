import ctypes, hashlib, json, random, struct
from verify_startup import ROOT, State, reference
from verify_fpu_cleanup_transfer import Fpu
from sh4_binary32 import truncate_i32,truncate_i32_double

PROJECT=ROOT/'outputs'/'sonic-adventure'; BASE=0x8c010000; FUNCTION=0x8c02cfc0; LENGTH=72; RAM_SIZE=16*1024*1024
binary=(ROOT/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes()
span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH]
assert len(span)==LENGTH
native=ctypes.CDLL(str(ROOT/'work'/'sonic-adventure'/'build'/'fpu_bucket_allocate_8c02cfc0.dll')).sa_fpu_bucket_allocate_8c02cfc0
native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State),ctypes.POINTER(Fpu)];native.restype=None
rng=random.Random(FUNCTION)
# Includes signed truncation, both index clamps, all normal bucket ranges, and FTRC edge encodings.
single_values=[0x00000000,0x80000000,0x3f800000,0xbf800000,0x41200000,0xc1200000,0x42fe0000,0xc2fe0000,0x4effffff,0xceffffff,0x4f000000,0xcf000000,0x7f800000,0xff800000,0x7fc12345]
double_numbers=[0.0,-0.0,1.0,-1.0,10.0,-10.0,127.5,-127.5,2147483648.0,-2147483648.0,float('inf'),float('-inf'),float('nan'),2147483647.5,-2147483648.5]
double_values=[]
for number in double_numbers:
    bits=struct.unpack('<Q',struct.pack('<d',number))[0]
    double_values.append((bits>>32,bits&0xffffffff))
manual_expected=[0,0,1,0xffffffff,10,0xfffffff6,127,0xffffff81,0x7fffffff,0x80000000,0x7fffffff,0x80000000,0x80000000,0x7fffffff,0x80000000]
for (high,low),expected in zip(double_values,manual_expected):
    result,invalid=truncate_i32_double(high,low)
    assert result==expected,(hex(high),hex(low),hex(result),hex(expected))
cases=[]
for case in range(96):
    ram=bytearray(RAM_SIZE);ram[0x10000:0x10000+len(binary)]=binary
    state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[15]=0x8c7ff000
    state.sr=rng.getrandbits(32)&~1;state.pr=rng.choice([0x8cfed000,0x8c010000,0x8c74913c]);state.pc=FUNCTION
    precision=case%2
    fpu=Fpu();fpu.fpscr=0x40000|rng.randrange(0x10000);fpu.fpscr&=~0x80800
    if precision:fpu.fpscr|=0x80000
    fpu.fpul=rng.getrandbits(32)
    for b in range(2):fpu.banks[b][:]=[rng.getrandbits(32) for _ in range(16)]
    if precision:
        high,low=double_values[(case//2)%len(double_values)]
        fpu.banks[(fpu.fpscr>>21)&1][4:6]=[high,low]
    else:fpu.banks[(fpu.fpscr>>21)&1][4]=single_values[(case//2)%len(single_values)]
    # One to four reusable blocks, optionally empty bucket, with exact forward/back links.
    head=0x8c700000;node_count=1+case%5
    struct.pack_into('<I',ram,0x2d540,head)
    for i in range(node_count):
        address=head+i*8;next_address=(address+8) if i+1<node_count else 0
        struct.pack_into('<II',ram,address-0x8c000000,next_address,0xa5000000+i)
    active=(fpu.fpscr>>21)&1
    if precision:converted,_=truncate_i32_double(fpu.banks[active][4],fpu.banks[active][5])
    else:converted,_=truncate_i32(fpu.banks[active][4])
    signed=converted-(1<<32) if converted&0x80000000 else converted
    bucket_index=max(0,min(127,(signed>>4)+64));bucket=0x8c02d560+bucket_index*8
    old_head=0 if case%2==0 else 0x8c710000
    struct.pack_into('<II',ram,bucket-0x8c000000,old_head,old_head or 0)
    if old_head:struct.pack_into('<II',ram,old_head-0x8c000000,0x8c720000,0)
    expected_ram=bytearray(ram);expected_state=State.from_buffer_copy(state);expected_fpu=Fpu.from_buffer_copy(fpu)
    reference(expected_ram,expected_state,fpu=expected_fpu)
    actual_ram=bytearray(ram);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(actual_ram)
    actual_state=State.from_buffer_copy(state);actual_fpu=Fpu.from_buffer_copy(fpu)
    native(view,ctypes.byref(actual_state),ctypes.byref(actual_fpu))
    if actual_ram!=expected_ram:
        first=next(i for i,(a,b) in enumerate(zip(actual_ram,expected_ram)) if a!=b)
        raise AssertionError(f'RAM mismatch case {case} at {first:#x}: {actual_ram[first:first+16].hex()} != {expected_ram[first:first+16].hex()}')
    assert bytes(actual_state)==bytes(expected_state),f'CPU mismatch case {case}: regs={list(map(hex,actual_state.r))}/{list(map(hex,expected_state.r))} sr={actual_state.sr:#x}/{expected_state.sr:#x} pc={actual_state.pc:#x}/{expected_state.pc:#x}'
    assert bytes(actual_fpu)==bytes(expected_fpu),f'FPU mismatch case {case}: {bytes(actual_fpu).hex()} != {bytes(expected_fpu).hex()}'
    cases.append({'case':case,'ram_cpu_fpu_match':True,'free_nodes':node_count,'bucket_prepopulated':bool(old_head)})

source=PROJECT/'source'/'sa_fpu_bucket_allocate_8c02cfc0.c'; report={
 'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),
 'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),
 'cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC plus all FPU registers/FPSCR/FPUL',
 'primary_source':'https://www.renesas.com/en/document/mas/sh-4','manual_revision':'6.00','manual_sections':['6','9.46'],
 'scope':'Retail FPU bucket allocation routine compared against instruction interpreter over both PR precision modes, populated and empty freelists, populated and empty destination buckets, index clamps, and conversion edge values.',
 'limitations':['Synthetic states are differential evidence, not full-game validation or hardware comparison.'],
 'cases':cases}
(PROJECT/'analysis'/'fpu_bucket_allocate_8c02cfc0_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(f'{len(cases)} retail FPU bucket allocator RAM/CPU/FPU comparisons passed')
