"""Nested secondary traversal against original instructions, with graph mutation."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_task_create import Task
from verify_task_lists import Lists
from verify_task_scheduler import Call,Scheduler
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    tasks=(Task*6)();addresses=[0x8c880000+i*0x100 for i in range(6)]
    native=[ctypes.addressof(t) for t in tasks];events=[];original=[];errors=[]
    lists=Lists();links={0:(1,2),1:(None,5),2:(3,4),3:(None,None),4:(None,None),5:(None,None)}
    @Call
    def invoke(context,token,pointer):
        i=native.index(ctypes.addressof(pointer.contents))
        if context!=0x1234:errors.append('context')
        events.append((i,token))
        if mode&1:tasks[i].slots[0]=0
        if mode&2:tasks[i].slots[3]=0
        if mode&4 and i<5:tasks[i+1].slots[5]=0x8c000300
    def hook(memory,regs,sr):
        i=addresses.index(regs[4]);original.append((i,regs[13]))
        offset=addresses[i]&0xffffff
        if mode&1:struct.pack_into('<I',memory,offset,0)
        if mode&2:struct.pack_into('<I',memory,offset+12,0)
        if mode&4 and i<5:struct.pack_into('<I',memory,(addresses[i+1]&0xffffff)+20,0x8c000300)
        return sr
    scheduler=Scheduler(ctypes.pointer(lists),0xdeadbeef,0x1234,invoke)
    dll_path=ROOT/'work/sonic-adventure/build/task_scheduler.dll';dll=ctypes.CDLL(str(dll_path))
    functions=[dll.sa_task_secondary_dispatch_8c098800,dll.sa_task_secondary_children_8c098842]
    functions[0].argtypes=[ctypes.POINTER(Scheduler),ctypes.c_uint32]
    functions[1].argtypes=[ctypes.POINTER(Scheduler),ctypes.POINTER(Task)]
    for fn in functions:fn.restype=None
    counts=[0,0]
    for entry in range(2):
     for level,callbacks,mode in itertools.product(range(9) if entry==0 else [0],itertools.product([0,0x8c000200],repeat=6),range(8)):
        for i in range(6):
            tasks[i].slots[:]=[0]*12;values=[0]*12
            for slot,target in zip([0,3],links[i]):
                tasks[i].slots[slot]=native[target] if target is not None else 0
                values[slot]=addresses[target] if target is not None else 0
            tasks[i].slots[5]=values[5]=callbacks[i]
            struct.pack_into('<12I',ram,addresses[i]&0xffffff,*values)
        for i in range(8):
            selected=i==(7 if level==8 else level)
            lists.level_heads[i]=ctypes.pointer(tasks[0]) if selected else ctypes.POINTER(Task)()
            struct.pack_into('<I',ram,0x79f04c+4*i,addresses[0] if selected else 0)
        struct.pack_into('<I',ram,0x1c1d40,0xdeadbeef)
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=level if entry==0 else addresses[0]
        state.r[15]=0x8cf00000;state.pc=[0x8c098800,0x8c098842][entry];state.pr=0x8c000100
        saved=list(state.r[8:]);events.clear();original.clear()
        reference(ram,state,{0x8c000200:hook,0x8c000300:hook})
        functions[entry](ctypes.byref(scheduler),level if entry==0 else ctypes.pointer(tasks[0]))
        assert events==original,(entry,level,callbacks,mode,events,original)
        assert scheduler.active_callback==struct.unpack_from('<I',ram,0x1c1d40)[0]==0xdeadbeef
        for i in range(6):
            values=[p or 0 for p in tasks[i].slots]
            for slot in [0,3]:
                if values[slot] in native:values[slot]=addresses[native.index(values[slot])]
            assert values==list(struct.unpack_from('<12I',ram,addresses[i]&0xffffff))
        assert list(state.r[8:])==saved and state.pr==0x8c000100
        counts[entry]+=1
    assert not errors,errors
    root=ROOT/'outputs/sonic-adventure'
    for entry,(start,end,name) in enumerate([(0x8c098800,0x8c098842,'task_secondary_dispatch'),(0x8c098842,0x8c098876,'task_secondary_children')]):
        report={'cases_passed':counts[entry],'routine_start':hex(start),'routine_end_exclusive':hex(end),
          'retail_span_sha256':hashlib.sha256(binary[start-0x8c010000:end-0x8c010000]).hexdigest(),
          'source_sha256':hashlib.sha256((root/'source/task_scheduler.c').read_bytes()).hexdigest(),
          'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
          'scope':'Six-task nested graph, all null/non-null callback combinations, saved sibling, post-callback child-head reload, callback replacement, levels 0..8 for level entry. Original recursion executes; callbacks hooked. No graphics or frame timing validation.'}
        (root/f'analysis/{name}_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{counts} secondary dispatcher/child traversal comparisons passed')
if __name__=='__main__':main()
