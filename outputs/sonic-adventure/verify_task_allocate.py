"""Original size/alignment/zero-fill instructions versus native allocation wrapper."""
import ctypes,hashlib,json,random,struct
from verify_startup import ROOT,State,reference
Allocate=ctypes.CFUNCTYPE(ctypes.c_void_p,ctypes.c_void_p,ctypes.c_uint32)
class Allocator(ctypes.Structure):
    _fields_=[('context',ctypes.c_void_p),('allocate',Allocate)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    storage=(ctypes.c_uint32*2052)();address=ctypes.addressof(storage)+8
    native_calls=[];original_calls=[];errors=[]
    @Allocate
    def allocate(context,size):
        native_calls.append(size)
        if context!=0x1234:errors.append('context')
        return None if failure else address
    def original_allocate(memory,regs,sr):
        original_calls.append(regs[4]);regs[0]=0 if failure else 0x8c880008
        return sr
    dll_path=ROOT/'work/sonic-adventure/build/task_allocate.dll'
    function=ctypes.CDLL(str(dll_path)).sa_task_allocate_zero_8c098b0c
    function.argtypes=[ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(Allocator)];function.restype=ctypes.c_void_p
    allocator=Allocator(0x1234,allocate)
    values=[0,1,2,3,4,5,7,8,15,16,28,64,255,256,0x3fffffff,0x40000000,0x7fffffff,0x80000000,0xfffffffc,0xfffffffd,0xfffffffe,0xffffffff]
    cases=[]
    for count in values:
        for size in values:
            total=(((size+3)&0xffffffff)&~3)*count & 0xffffffff
            cases.append((count,size,True))
            if total<=8192:cases.append((count,size,False))
    rng=random.Random(0x98b0c)
    cases += [(rng.randrange(0,9),rng.randrange(0,1025),False) for _ in range(128)]
    for count,size,failure in cases:
        native_calls.clear();original_calls.clear()
        ctypes.memset(ctypes.addressof(storage),0xa5,ctypes.sizeof(storage))
        ram[0x880000:0x880000+ctypes.sizeof(storage)]=bytes(storage)
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=count;state.r[5]=size;state.r[15]=0x8cf00000
        state.pc=0x8c098b0c;state.pr=0x8c000100;state.sr=0x700000f0
        saved=list(state.r[8:]);reference(ram,state,{0x8c098a80:original_allocate})
        result=function(count,size,ctypes.byref(allocator))
        assert native_calls==original_calls,(count,size,'requested size')
        assert result==(None if failure else address) and state.r[0]==(0 if failure else 0x8c880008)
        assert bytes(storage)==ram[0x880000:0x880000+ctypes.sizeof(storage)],(count,size,'output/guards')
        assert list(state.r[8:])==saved and state.pr==0x8c000100
    assert not errors,errors
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':len(cases),'wrapping_alignment_and_product_boundaries':True,
            'routine_start':'0x8c098b0c','routine_end_exclusive':'0x8c098b42',
            'retail_span_sha256':hashlib.sha256(binary[0x88b0c:0x88b42]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/task_allocate.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'Subset integer instruction interpreter comparisons of allocator requests, output bytes including guards, return and callee-saved registers. Underlying heap allocator is a hook. Large successful allocations and hardware/gameplay unverified.'}
    (root/'analysis/task_allocate_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{len(cases)} task allocation/zero-fill comparisons passed')
if __name__=='__main__':main()
