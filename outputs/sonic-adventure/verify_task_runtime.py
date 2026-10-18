"""Connected recursive disposal and child processing on a two-level task tree."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_task_create import Task
from verify_task_lists import Lists
Invoke=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_size_t,ctypes.POINTER(Task))
Work=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.POINTER(Task))
Free=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_void_p)
class Runtime(ctypes.Structure):
    _fields_=[('lists',ctypes.POINTER(Lists)),('context',ctypes.c_void_p),('invoke',Invoke),('work',Work),('free',Free)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    tasks=(Task*6)();pointers=[ctypes.pointer(tasks[i]) for i in range(6)]
    addresses=[0x8c880000+i*0x100 for i in range(6)]
    native_indices={ctypes.addressof(tasks[i]):i for i in range(6)}
    map_addresses={ctypes.addressof(tasks[i]):addresses[i] for i in range(6)}
    events=[];original=[];errors=[]
    @Invoke
    def invoke(context,token,pointer):
        if context!=0x1234:errors.append('context')
        assert token==0x8c000200
        events.append((token,native_indices[ctypes.addressof(pointer.contents)]))
    @Work
    def work(context,pointer):errors.append('unexpected work')
    @Free
    def free(context,pointer):errors.append('unexpected allocation')
    def hooked(memory,regs,sr):
        original.append((0x8c000200,addresses.index(regs[4])));return sr
    dll_path=ROOT/'work/sonic-adventure/build/task_runtime.dll';dll=ctypes.CDLL(str(dll_path))
    functions=[dll.sa_task_runtime_dispose,dll.sa_task_runtime_children]
    for function in functions:function.argtypes=[ctypes.POINTER(Runtime),ctypes.POINTER(Task)];function.restype=None
    count=0
    for callbacks in itertools.product([0x8c000200,0x8c098e60,0x8c09865c],repeat=5):
     for destructor in [False,True]:
      for entry in range(2):
        for task in tasks:task.slots[:]=[0]*12
        # Root -> children 1,2; child 1 -> grandchildren 3,4; task 5 initially free.
        for parent,children in [(0,[1,2]),(1,[3,4])]:
            tasks[parent].slots[3]=ctypes.addressof(tasks[children[0]])
            for position,index in enumerate(children):
                tasks[index].slots[0]=ctypes.addressof(tasks[children[position+1]]) if position+1<len(children) else 0
                tasks[index].slots[1]=ctypes.addressof(tasks[children[position-1]]) if position else 0
                tasks[index].slots[2]=ctypes.addressof(tasks[parent])
        for i in range(5):tasks[i].slots[4]=callbacks[i];tasks[i].slots[6]=0x8c000200 if destructor else 0
        lists=Lists();lists.free_head=pointers[5]
        for i in range(8):lists.level_heads[i]=pointers[0] if i==2 else ctypes.POINTER(Task)()
        runtime=Runtime(ctypes.pointer(lists),0x1234,invoke,work,free)
        for i in range(6):struct.pack_into('<12I',ram,addresses[i]-0x8c000000,*[map_addresses.get(p,p or 0) for p in tasks[i].slots])
        struct.pack_into('<I',ram,0x79f048,addresses[5])
        struct.pack_into('<8I',ram,0x79f04c,*[addresses[0] if i==2 else 0 for i in range(8)])
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=addresses[0];state.r[15]=0x8cf00000;state.pc=0x8c09865c if entry==0 else 0x8c098e60;state.pr=0x8c000100;state.sr=0x700000f0
        saved=list(state.r[8:]);events.clear();original.clear()
        reference(ram,state,{0x8c000200:hooked})
        functions[entry](ctypes.byref(runtime),pointers[0]);label=(callbacks,destructor,entry)
        assert events==original,(label,'external callbacks')
        for i in range(6):assert [map_addresses.get(p,p or 0) for p in tasks[i].slots]==list(struct.unpack_from('<12I',ram,addresses[i]-0x8c000000)),(label,i,'slots')
        assert map_addresses.get(ctypes.cast(lists.free_head,ctypes.c_void_p).value,0)==struct.unpack_from('<I',ram,0x79f048)[0]
        assert [map_addresses.get(ctypes.cast(p,ctypes.c_void_p).value,0) for p in lists.level_heads]==list(struct.unpack_from('<8I',ram,0x79f04c)),(label,'heads')
        assert list(state.r[8:])==saved and state.pr==0x8c000100
        count+=1
    assert not errors,errors
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':count,'connected_original_routines':['0x8c09865c','0x8c098e60','0x8c098e98'],
            'additional_original_routines':0,'tree_depth':2,
            'source_hashes':{name:hashlib.sha256((root/'source'/name).read_bytes()).hexdigest() for name in ['task_runtime.c','task_dispose.c','task_children.c']},
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'Six native task records, free head, eight active heads and external callbacks versus original recursive call chain. Task bodies execute without hooks; only no-op external callback hooked. Both cleanup entry points, all callback-token combinations in a fixed two-level tree and optional destructors checked. Work/resource integration, token resolver beyond known functions, scheduler/hardware/gameplay remain unverified.'}
    (root/'analysis/task_runtime_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{count} connected nested-task lifecycle comparisons passed')
if __name__=='__main__':main()
