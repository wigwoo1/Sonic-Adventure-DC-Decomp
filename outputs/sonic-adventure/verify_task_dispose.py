"""Task disposal pointer/list mutations and external-call ordering comparisons."""
import ctypes,hashlib,json,struct
from verify_startup import ROOT,State,reference
from verify_task_create import Task
from verify_task_lists import Lists
TaskCall=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.POINTER(Task))
Destructor=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_size_t,ctypes.POINTER(Task))
Free=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_void_p)
class Services(ctypes.Structure):
    _fields_=[('context',ctypes.c_void_p),('destructor',Destructor),('children',TaskCall),('work',TaskCall),('free',Free)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    tasks=(Task*5)();pointers=[ctypes.pointer(tasks[i]) for i in range(5)]
    addresses=[0x8c880000+i*0x100 for i in range(5)]
    attachment=(ctypes.c_uint8*4)();events=[];original=[];errors=[]
    address_map={ctypes.addressof(tasks[i]):addresses[i] for i in range(5)}
    address_map[ctypes.addressof(attachment)]=0x8c884000
    def retail(value):return address_map.get(value,value or 0)
    def check(context,pointer):
        if context!=0x1234 or ctypes.addressof(pointer.contents)!=ctypes.addressof(tasks[0]):errors.append('arguments')
    @Destructor
    def destructor(context,token,pointer):
        check(context,pointer);events.append(('destructor',token))
    @TaskCall
    def children(context,pointer):
        check(context,pointer);events.append(('children',))
        if any(pointer.contents.slots[i] for i in [4,5,6]):errors.append('callbacks not cleared before children')
    @TaskCall
    def work(context,pointer):
        check(context,pointer);events.append(('work',));pointer.contents.slots[8]=0
    @Free
    def free(context,pointer):
        if context!=0x1234:errors.append('context')
        events.append(('free',pointer))
    services=Services(0x1234,destructor,children,work,free)
    def hook_destructor(memory,regs,sr):
        assert regs[4]==addresses[0];original.append(('destructor',0x8c000200));return sr
    def hook_children(memory,regs,sr):
        assert regs[4]==addresses[0]
        assert struct.unpack_from('<3I',memory,0x880010)==(0,0,0)
        original.append(('children',));return sr
    def hook_work(memory,regs,sr):
        assert regs[4]==addresses[0];original.append(('work',));struct.pack_into('<I',memory,0x880020,0);return sr
    def hook_free(memory,regs,sr):original.append(('free',regs[4]));return sr
    hooks={0x8c000200:hook_destructor,0x8c098e60:hook_children,0x8c098998:hook_work,0x8c098b42:hook_free}
    dll_path=ROOT/'work/sonic-adventure/build/task_dispose.dll'
    function=ctypes.CDLL(str(dll_path)).sa_task_dispose_8c09865c
    function.argtypes=[ctypes.POINTER(Lists),ctypes.POINTER(Task),ctypes.POINTER(Services)];function.restype=None
    count=0
    for links in range(4):
     for parent in [False,True]:
      for child in [False,True]:
       for free_head in [False,True]:
        for mask in range(16):
         for flags in [0,1,0xffff]:
          for head_index in [None,0,7]:
           for task in tasks:task.slots[:]=[0]*12
           task=tasks[0];task.slots[0]=ctypes.addressof(tasks[1]) if links&1 else None
           task.slots[1]=ctypes.addressof(tasks[2]) if links&2 else None
           task.slots[2]=ctypes.addressof(tasks[3]) if parent else None
           task.slots[3]=0x8c885000 if child else None
           task.slots[4]=0x12345678;task.slots[5]=0x23456789
           task.slots[6]=0x8c000200 if mask&1 else None
           attachment[:]=[91,92,flags&255,flags>>8];task.slots[7]=ctypes.addressof(attachment)
           task.slots[8]=0x8c886000 if mask&2 else None
           task.slots[9]=0x8c887000 if mask&4 else None
           task.slots[10]=0x8c888000 if mask&8 else None
           task.slots[11]=0x8c889000
           lists=Lists();lists.free_head=pointers[4] if free_head else ctypes.POINTER(Task)()
           for i in range(8):lists.level_heads[i]=pointers[0] if head_index==i else ctypes.POINTER(Task)()
           for i in range(5):struct.pack_into('<12I',ram,addresses[i]-0x8c000000,*[retail(p) for p in tasks[i].slots])
           ram[0x884000:0x884004]=bytes(attachment)
           struct.pack_into('<I',ram,0x79f048,addresses[4] if free_head else 0)
           struct.pack_into('<8I',ram,0x79f04c,*[addresses[0] if i==head_index else 0 for i in range(8)])
           state=State();state.r[:]=[0x12340000+i for i in range(16)]
           state.r[4]=addresses[0];state.r[15]=0x8cf00000;state.pc=0x8c09865c;state.pr=0x8c000100;state.sr=0x700000f0
           saved=list(state.r[8:]);events.clear();original.clear();reference(ram,state,hooks)
           function(ctypes.byref(lists),pointers[0],ctypes.byref(services))
           label=(links,parent,child,free_head,mask,flags,head_index)
           assert events==original,(label,'calls',events,original)
           for i in range(5):assert [retail(p) for p in tasks[i].slots]==list(struct.unpack_from('<12I',ram,addresses[i]-0x8c000000)),(label,i,'slots')
           assert bytes(attachment)==ram[0x884000:0x884004]
           assert retail(ctypes.cast(lists.free_head,ctypes.c_void_p).value)==struct.unpack_from('<I',ram,0x79f048)[0]
           assert [retail(ctypes.cast(p,ctypes.c_void_p).value) for p in lists.level_heads]==list(struct.unpack_from('<8I',ram,0x79f04c)),(label,'heads')
           assert list(state.r[8:])==saved and state.pr==0x8c000100
           count+=1
    assert not errors,errors
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':count,'routine_start':'0x8c09865c','routine_end_exclusive':'0x8c098786',
            'retail_span_sha256':hashlib.sha256(binary[0x8865c:0x88786]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/task_dispose.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'All 12 projected slots in five native tasks, eight active heads, free head, attachment bytes and service ordering versus original integer instructions. Destructor/child/work/free bodies hooked. Callback mutation beyond clearing primary work, full scheduler, hardware and gameplay unverified.'}
    (root/'analysis/task_dispose_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{count} task disposal comparisons passed')
if __name__=='__main__':main()
