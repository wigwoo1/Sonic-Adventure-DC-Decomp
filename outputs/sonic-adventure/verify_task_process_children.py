"""Child callback dispatch with sibling/head/callback mutation during invocation."""
import ctypes,hashlib,json,struct
from verify_startup import ROOT,State,reference
from verify_task_create import Task
Invoke=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_size_t,ctypes.POINTER(Task))
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    tasks=(Task*7)();pointers=[ctypes.pointer(tasks[i]) for i in range(7)]
    addresses=[0x8c880000+i*0x100 for i in range(7)]
    native_indices={ctypes.addressof(tasks[i]):i for i in range(7)}
    map_addresses={ctypes.addressof(tasks[i]):addresses[i] for i in range(7)}
    events=[];original=[];errors=[]
    @Invoke
    def invoke(context,token,pointer):
        if context!=0x1234:errors.append('context')
        index=native_indices[ctypes.addressof(pointer.contents)]
        events.append((token,index));next_address=tasks[index].slots[0]
        if mode&1:tasks[index].slots[0]=0
        if mode&2 and next_address:tasks[native_indices[next_address]].slots[4]=0x8c000240
        if mode&4:tasks[0].slots[3]=0
    hooks={}
    for token in [0x8c000200,0x8c000220,0x8c000240]:
        def hooked(memory,regs,sr,token=token):
            index=addresses.index(regs[4]);offset=addresses[index]-0x8c000000
            original.append((token,index));next_address=struct.unpack_from('<I',memory,offset)[0]
            if mode&1:struct.pack_into('<I',memory,offset,0)
            if mode&2 and next_address:struct.pack_into('<I',memory,next_address-0x8c000000+16,0x8c000240)
            if mode&4:struct.pack_into('<I',memory,0x88000c,0)
            return sr
        hooks[token]=hooked
    dll_path=ROOT/'work/sonic-adventure/build/task_children.dll'
    function=ctypes.CDLL(str(dll_path)).sa_task_process_children_8c098e98
    function.argtypes=[ctypes.POINTER(Task),ctypes.c_void_p,Invoke];function.restype=None
    count=0
    for length in range(7):
     for reverse in [False,True]:
      for mode in range(8):
        order=list(range(1,length+1));order=order[::-1] if reverse else order
        for i in range(7):tasks[i].slots[:]=[0x12340000+i*0x100+j for j in range(12)]
        tasks[0].slots[3]=ctypes.addressof(tasks[order[0]]) if order else 0
        for position,index in enumerate(order):
            tasks[index].slots[0]=ctypes.addressof(tasks[order[position+1]]) if position+1<len(order) else 0
            tasks[index].slots[4]=0x8c000200 if index&1 else 0x8c000220
        for i in range(7):struct.pack_into('<12I',ram,addresses[i]-0x8c000000,*[map_addresses.get(p,p or 0) for p in tasks[i].slots])
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=addresses[0];state.r[15]=0x8cf00000;state.pc=0x8c098e98;state.pr=0x8c000100;state.sr=0x700000f0
        saved=list(state.r[8:]);events.clear();original.clear();reference(ram,state,hooks)
        function(pointers[0],0x1234,invoke)
        label=(length,reverse,mode)
        assert events==original and [index for _,index in events]==order,(label,'calls')
        for i in range(7):assert [map_addresses.get(p,p or 0) for p in tasks[i].slots]==list(struct.unpack_from('<12I',ram,addresses[i]-0x8c000000)),(label,i,'slots')
        assert list(state.r[8:])==saved and state.pr==0x8c000100
        count+=1
    assert not errors,errors
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':count,'routine_start':'0x8c098e98','routine_end_exclusive':'0x8c098eba',
            'retail_span_sha256':hashlib.sha256(binary[0x88e98:0x88eba]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/task_children.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'Seven projected task records, dispatch tokens/order and saved registers versus original integer instructions. Callbacks can clear current next, clear parent child head and replace next callback. Callback bodies hooked; valid acyclic list and executable callback tokens required. Scheduler/hardware/gameplay unverified.'}
    (root/'analysis/task_process_children_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{count} child callback traversal comparisons passed')
if __name__=='__main__':main()
