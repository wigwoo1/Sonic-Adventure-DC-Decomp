import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference

PROJECT=ROOT/'outputs'/'sonic-adventure';BASE=0x8c010000;FUNCTION=0x8c64e0ac;LENGTH=8;RAM_SIZE=16*1024*1024
binary=(ROOT/'work'/'sonic-adventure'/'extracted'/'1ST_READ.BIN').read_bytes();span=binary[FUNCTION-BASE:FUNCTION-BASE+LENGTH]
assert span==bytes.fromhex('2b d2 21 d3 2b 42 32 64')
lib=ctypes.CDLL(str(ROOT/'work'/'sonic-adventure'/'build'/'callback_tail_8c64e0ac.dll'))
DISPATCH=ctypes.CFUNCTYPE(None,ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State),ctypes.c_uint32,ctypes.c_void_p)
native=lib.sa_callback_tail_8c64e0ac;native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(State),DISPATCH,ctypes.c_void_p];native.restype=None
def callback_effect(memory,regs,sr,case,target):
    marker=(target^regs[2]^regs[3]^regs[4]^(case*0x1020304))&0xffffffff
    at=0xb80000+case*4;data=marker.to_bytes(4,'little')
    if isinstance(memory,bytearray):memory[at:at+4]=data
    else:ctypes.memmove(ctypes.addressof(memory)+at,data,4)
    regs[0]=(marker^0x13579bdf)&0xffffffff;regs[1]=(regs[1]+marker)&0xffffffff
    regs[2]^=0x2468ace0;regs[4]=(regs[4]-case-1)&0xffffffff
    regs[5]^=marker;regs[6]=(regs[6]+0x11111111)&0xffffffff;regs[7]^=0x87654321
    return (sr^marker)&0xffffffff

original=bytearray(b'\xa5')*RAM_SIZE;original[0x10000:0x10000+len(binary)]=binary
target=struct.unpack_from('<I',binary,0x8c64e15c-BASE)[0];arg_pointer=struct.unpack_from('<I',binary,0x8c64e134-BASE)[0]
rng=random.Random(FUNCTION);cases=[]
for case in range(32):
    expected=State();expected.r[:]=[rng.getrandbits(32) for _ in range(16)]
    expected.sr=rng.getrandbits(32);expected.pr=0x8c000100+case*4;expected.pc=FUNCTION
    initial=State.from_buffer_copy(expected)
    struct.pack_into('<I',original,arg_pointer-0x8c000000,0x55000000+case)
    retail_ram,actual_ram=bytearray(original),bytearray(original)
    calls=[]
    def hook(memory,regs,sr):
        assert regs[2]==target and regs[3]==arg_pointer and regs[4]==0x55000000+case
        calls.append((target,regs[4],expected.pr))
        return callback_effect(memory,regs,sr,case,target)
    steps=reference(retail_ram,expected,hooks={target:hook})
    actual=State.from_buffer_copy(initial)
    actual_ram=bytearray(original);view=(ctypes.c_uint8*RAM_SIZE).from_buffer(actual_ram);native_calls=[]
    @DISPATCH
    def dispatch(memory_pointer,state_pointer,call_target,context):
        assert call_target==target
        state=state_pointer.contents;regs=list(state.r)
        memory=(ctypes.c_uint8*RAM_SIZE).from_address(ctypes.addressof(memory_pointer.contents))
        native_calls.append((call_target,regs[4],state.pr))
        result=callback_effect(memory,regs,state.sr,case,call_target)
        for index,value in enumerate(regs):state.r[index]=value
        state.sr=result
    native(view,ctypes.byref(actual),dispatch,None)
    assert steps==4,(steps,case)
    assert actual_ram==retail_ram,f'RAM mismatch case {case}'
    assert bytes(actual)==bytes(expected),f'CPU mismatch case {case}'
    assert native_calls==calls,f'Callback mismatch case {case}: native={native_calls}, retail={calls}'
    assert initial.pr==expected.pr
    cases.append({'case':case,'retail_steps':steps,'callback_target':hex(target),'full_ram_cpu_and_callback_match':True})

source=PROJECT/'source'/'sa_callback_tail_8c64e0ac.c'
report={'module':'1ST_READ.BIN','load_address':hex(BASE),'address':hex(FUNCTION),'end_exclusive':hex(FUNCTION+LENGTH),'retail_span_sha256':hashlib.sha256(span).hexdigest(),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'callback_target':hex(target),'argument_pointer':hex(arg_pointer),'cases_passed':len(cases),'comparison_level':'complete 16 MiB RAM and all GPR/SR/PR/PC state, including callback execution','scope':'The two PC-relative literals load a callback target and a global argument pointer; the delay-slot load places the argument in R4 before the indirect tail jump. Callback effects are compared through an instrumented deterministic hook.','limitations':['Callback behavior is supplied by a test hook and is not part of this eight-byte routine.','Synthetic states are not hardware or full-game validation.'],'cases':cases}
(PROJECT/'analysis'/'callback_tail_8c64e0ac_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(f'{len(cases)} callback tail RAM/CPU comparisons passed')
