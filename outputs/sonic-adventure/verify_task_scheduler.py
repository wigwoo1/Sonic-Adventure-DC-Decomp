"""Compare task dispatch ordering, callback global and mutation to retail opcodes."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_task_create import Task
from verify_task_lists import Lists
Call=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_size_t,ctypes.POINTER(Task))
class Scheduler(ctypes.Structure):
    _fields_=[('lists',ctypes.POINTER(Lists)),('active_callback',ctypes.c_size_t),('context',ctypes.c_void_p),('invoke',Call)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    tasks=(Task*3)();addresses=[0x8c880000+i*0x100 for i in range(3)]
    native_addresses=[ctypes.addressof(t) for t in tasks]
    events=[];original=[];errors=[];lists=Lists()
    @Call
    def invoke(context,token,pointer):
        i=native_addresses.index(ctypes.addressof(pointer.contents))
        if context!=0x1234 or scheduler.active_callback!=token:errors.append('callback state')
        events.append((i,token,scheduler.active_callback))
        if mutate:
            tasks[i].slots[0]=0
            if i<2:tasks[i+1].slots[4]=0x8c000300
            scheduler.active_callback=0x12345678
    def hook(memory,regs,sr):
        i=addresses.index(regs[4]);token=regs[14]
        original.append((i,token,struct.unpack_from('<I',memory,0x1c1d40)[0]))
        if mutate:
            struct.pack_into('<I',memory,(addresses[i]&0xffffff),0)
            if i<2:struct.pack_into('<I',memory,(addresses[i+1]&0xffffff)+16,0x8c000300)
            struct.pack_into('<I',memory,0x1c1d40,0x12345678)
        return sr
    scheduler=Scheduler(ctypes.pointer(lists),0,0x1234,invoke)
    dll_path=ROOT/'work/sonic-adventure/build/task_scheduler.dll'
    function=ctypes.CDLL(str(dll_path)).sa_task_dispatch_8c09878c
    function.argtypes=[ctypes.POINTER(Scheduler),ctypes.c_uint32];function.restype=None
    count=0
    for level,length,callbacks,mutate in itertools.product(range(9),range(4),itertools.product([0,0x8c000200],repeat=3),[False,True]):
        for i in range(3):
            tasks[i].slots[:]=[0]*12
            tasks[i].slots[0]=native_addresses[i+1] if i+1<length else 0
            tasks[i].slots[4]=callbacks[i]
            values=[0]*12;values[0]=addresses[i+1] if i+1<length else 0;values[4]=callbacks[i]
            struct.pack_into('<12I',ram,addresses[i]&0xffffff,*values)
        for i in range(8):
            lists.level_heads[i]=ctypes.pointer(tasks[0]) if length and i==(7 if level==8 else level) else ctypes.POINTER(Task)()
            struct.pack_into('<I',ram,0x79f04c+4*i,addresses[0] if lists.level_heads[i] else 0)
        scheduler.active_callback=0xdeadbeef;struct.pack_into('<I',ram,0x1c1d40,0xdeadbeef)
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=level;state.r[15]=0x8cf00000;state.pc=0x8c09878c;state.pr=0x8c000100
        saved=list(state.r[8:]);events.clear();original.clear()
        reference(ram,state,{0x8c000200:hook,0x8c000300:hook})
        function(ctypes.byref(scheduler),level)
        assert events==original,(level,length,callbacks,mutate,events,original)
        assert scheduler.active_callback==struct.unpack_from('<I',ram,0x1c1d40)[0]
        for i in range(3):
            values=[p or 0 for p in tasks[i].slots]
            if values[0] in native_addresses:values[0]=addresses[native_addresses.index(values[0])]
            assert values==list(struct.unpack_from('<12I',ram,addresses[i]&0xffffff))
        assert list(state.r[8:])==saved and state.pr==0x8c000100
        count+=1
    assert not errors,errors
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':count,'routine_start':'0x8c09878c','routine_end_exclusive':'0x8c0987ce',
      'retail_span_sha256':hashlib.sha256(binary[0x8878c:0x887ce]).hexdigest(),
      'source_sha256':hashlib.sha256((root/'source/task_scheduler.c').read_bytes()).hexdigest(),
      'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
      'scope':'Levels 0..8, empty to three tasks, null callbacks, saved-next traversal, callback replacement and diagnostic-global mutation versus original instructions. Callback bodies hooked; no frame timing or gameplay validation.'}
    (root/'analysis/task_scheduler_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{count} task scheduler comparisons passed')
if __name__=='__main__':main()
