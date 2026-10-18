"""Child cleanup wrapper branch, reload and callback-token comparisons."""
import ctypes,hashlib,json,struct
from verify_startup import ROOT,State,reference
from verify_task_create import Task
Call=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.POINTER(Task))
class Services(ctypes.Structure):
    _fields_=[('context',ctypes.c_void_p),('process',Call),('dispose',Call)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    task=Task();events=[];original=[];errors=[];native_index=original_index=0
    @Call
    def process(context,pointer):
        if context!=0x1234 or ctypes.addressof(pointer.contents)!=ctypes.addressof(task):errors.append('process args')
        events.append(('process',))
        if clear:task.slots[3]=0
        if replace_callback:task.slots[4]=0x12345678
    @Call
    def dispose(context,pointer):
        nonlocal native_index
        if context!=0x1234:errors.append('context')
        events.append(('dispose',ctypes.cast(pointer,ctypes.c_void_p).value))
        native_index+=1;task.slots[3]=children[native_index] if native_index<len(children) else 0
    def original_process(memory,regs,sr):
        assert regs[4]==0x8c880000;original.append(('process',))
        if clear:struct.pack_into('<I',memory,0x88000c,0)
        if replace_callback:struct.pack_into('<I',memory,0x880010,0x12345678)
        return sr
    def original_dispose(memory,regs,sr):
        nonlocal original_index
        original.append(('dispose',regs[4]));original_index+=1
        struct.pack_into('<I',memory,0x88000c,children[original_index] if original_index<len(children) else 0)
        return sr
    services=Services(0x1234,process,dispose)
    dll_path=ROOT/'work/sonic-adventure/build/task_children.dll'
    function=ctypes.CDLL(str(dll_path)).sa_task_children_8c098e60
    function.argtypes=[ctypes.POINTER(Task),ctypes.POINTER(Services)];function.restype=None
    count=0
    for callback in [0,0x8c098e60,0x8c09865c,0xffffffff]:
     for length in range(7):
      for clear in [False,True]:
       for replace_callback in [False,True]:
        children=[0x8c881000+i*0x100 for i in range(length)]
        task.slots[:]=[0x12340000+i for i in range(12)]
        task.slots[3]=children[0] if children else 0;task.slots[4]=callback
        struct.pack_into('<12I',ram,0x880000,*[p or 0 for p in task.slots])
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=0x8c880000;state.r[15]=0x8cf00000;state.pc=0x8c098e60;state.pr=0x8c000100;state.sr=0x700000f0
        saved=list(state.r[8:]);events.clear();original.clear();native_index=original_index=0
        reference(ram,state,{0x8c098e98:original_process,0x8c09865c:original_dispose})
        function(ctypes.byref(task),ctypes.byref(services))
        assert events==original,(callback,length,clear,replace_callback,'calls')
        assert [p or 0 for p in task.slots]==list(struct.unpack_from('<12I',ram,0x880000))
        assert list(state.r[8:])==saved and state.pr==0x8c000100
        count+=1
    assert not errors,errors
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':count,'routine_start':'0x8c098e60','routine_end_exclusive':'0x8c098e98',
            'retail_span_sha256':hashlib.sha256(binary[0x88e60:0x88e98]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/task_children.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'Projected task slots and child disposal/process calls versus original integer instructions. Checks callback identity, empty to six-child lists, head reloads, helper clearing child head and replacing callback. Child processing/disposal are hooks; callback tokens unresolved; no scheduler/hardware/gameplay validation.'}
    (root/'analysis/task_children_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{count} child task cleanup comparisons passed')
if __name__=='__main__':main()
