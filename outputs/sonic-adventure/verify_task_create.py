"""Task creation wrapper equivalence; base creation, allocation and disposal are hooks."""
import ctypes,hashlib,json,struct
from verify_startup import ROOT,State,reference
class Task(ctypes.Structure):
    _fields_=[('slots',ctypes.c_void_p*12)]
Create=ctypes.CFUNCTYPE(ctypes.c_void_p,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_size_t)
Allocate=ctypes.CFUNCTYPE(ctypes.c_void_p,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint32)
Dispose=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.POINTER(Task))
class Services(ctypes.Structure):
    _fields_=[('context',ctypes.c_void_p),('create',Create),('allocate',Allocate),('dispose',Dispose)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    task=Task();native_events=[];original_events=[];errors=[]
    native_count=original_count=0
    @Create
    def create(context,level,callback):
        native_events.append(('create',level,callback))
        if context!=0x1234:errors.append('context')
        return None if fail==0 else ctypes.addressof(task)
    @Allocate
    def allocate(context,kind,size):
        nonlocal native_count
        native_count+=1;native_events.append(('allocate',kind,size))
        if context!=0x1234:errors.append('context')
        return None if fail==native_count else 0x8c881000+native_count*0x100
    @Dispose
    def dispose(context,pointer):
        native_events.append(('dispose',))
        if context!=0x1234 or ctypes.addressof(pointer.contents)!=ctypes.addressof(task):errors.append('dispose arguments')
        pointer.contents.slots[0]=0x98765432
    def original_create(memory,regs,sr):
        original_events.append(('create',regs[4],regs[5]));regs[0]=0 if fail==0 else 0x8c880000
        return sr
    def original_allocate(memory,regs,sr):
        nonlocal original_count
        original_count+=1;original_events.append(('allocate',regs[4],regs[5]))
        regs[0]=0 if fail==original_count else 0x8c881000+original_count*0x100
        return sr
    def original_dispose(memory,regs,sr):
        assert regs[4]==0x8c880000
        original_events.append(('dispose',));struct.pack_into('<I',memory,0x880000,0x98765432)
        return sr
    hooks={0x8c0985ec:original_create,0x8c098b0c:original_allocate,0x8c09865c:original_dispose}
    services=Services(0x1234,create,allocate,dispose)
    dll_path=ROOT/'work/sonic-adventure/build/task_create.dll'
    function=ctypes.CDLL(str(dll_path)).sa_task_create_8c09852e
    function.argtypes=[ctypes.c_uint32,ctypes.c_uint32,ctypes.c_size_t,ctypes.POINTER(Services)];function.restype=ctypes.c_void_p
    cases=[(flags,-1,3,0x0c900d76) for flags in range(65536)]
    cases += [(flags,fail,level,callback) for flags in range(16) for fail in range(5)
              for level,callback in [(0,0),(0xffffffff,0xffffffff),(1,0x8c107864)]]
    cases += [(flags,-1,2,0x0c900d76) for flags in [0x10000,0x80000002,0xffffffff]]
    for flags,fail,level,callback in cases:
        native_events.clear();original_events.clear();native_count=original_count=0
        task.slots[:]=[0x12340000+i for i in range(12)]
        struct.pack_into('<12I',ram,0x880000,*task.slots)
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=flags;state.r[5]=level;state.r[6]=callback;state.r[15]=0x8cf00000
        state.pc=0x8c09852e;state.pr=0x8c000100;state.sr=0x700000f0
        saved=list(state.r[8:]);reference(ram,state,hooks)
        result=function(flags,level,callback,ctypes.byref(services))
        assert bool(result)==bool(state.r[0]),(flags,fail,'return')
        if result:assert result==ctypes.addressof(task) and state.r[0]==0x8c880000
        assert list(task.slots)==list(struct.unpack_from('<12I',ram,0x880000)),(flags,fail,'slots')
        assert native_events==original_events,(flags,fail,'events',native_events,original_events)
        assert list(state.r[8:])==saved and state.pr==0x8c000100
    assert not errors,errors
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':len(cases),'flag_values_exhausted':65536,'failure_paths_checked':True,
            'routine_start':'0x8c09852e','routine_end_exclusive':'0x8c0985ec',
            'retail_span_sha256':hashlib.sha256(binary[0x8852e:0x885ec]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/task_create.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'Projected task slots, return and external call sequence compared with original integer instructions. Allocator, base task creation and disposal are hooks; no scheduler/gameplay/hardware verification.'}
    (root/'analysis/task_create_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{len(cases)} task creation wrapper comparisons passed')
if __name__=='__main__':main()
