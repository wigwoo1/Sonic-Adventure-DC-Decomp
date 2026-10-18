"""Compare native base task creation and list mutations with original instructions."""
import ctypes,hashlib,json,struct
from verify_startup import ROOT,State,reference
from verify_task_create import Task
class Lists(ctypes.Structure):
    _fields_=[('free_head',ctypes.POINTER(Task)),('level_heads',ctypes.POINTER(Task)*8)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    tasks=(Task*3)();pointers=[ctypes.pointer(tasks[i]) for i in range(3)]
    addresses=[0x8c880000+i*0x100 for i in range(3)]
    address_map={ctypes.addressof(tasks[i]):addresses[i] for i in range(3)}
    def retail(pointer):return address_map.get(pointer,pointer or 0)
    dll_path=ROOT/'work/sonic-adventure/build/task_lists.dll'
    function=ctypes.CDLL(str(dll_path)).sa_base_task_create_8c0985ec
    function.argtypes=[ctypes.POINTER(Lists),ctypes.c_uint32,ctypes.c_size_t];function.restype=ctypes.c_void_p
    count=0
    for level in range(9):
      for free_count in range(3):
       for active_head in [None,0,1,2]:
        for callback in [0,0x0c900d76,0xffffffff]:
            for i in range(3):tasks[i].slots[:]=[0x12340000+i*0x100+j for j in range(12)]
            tasks[0].slots[0]=ctypes.addressof(tasks[1]) if free_count==2 else None
            tasks[0].slots[1]=None
            tasks[1].slots[0]=None;tasks[1].slots[1]=ctypes.addressof(tasks[0])
            tasks[2].slots[0]=None;tasks[2].slots[1]=None
            lists=Lists();lists.free_head=pointers[0] if free_count else ctypes.POINTER(Task)()
            for i in range(8):lists.level_heads[i]=ctypes.POINTER(Task)()
            target=7 if level==8 else level
            if active_head is not None:lists.level_heads[target]=pointers[active_head]
            struct.pack_into('<I',ram,0x79f048,addresses[0] if free_count else 0)
            struct.pack_into('<8I',ram,0x79f04c,*[addresses[active_head] if active_head is not None and i==target else 0 for i in range(8)])
            for i in range(3):struct.pack_into('<12I',ram,addresses[i]-0x8c000000,*[retail(p) for p in tasks[i].slots])
            state=State();state.r[:]=[0x12340000+i for i in range(16)]
            state.r[4]=level;state.r[5]=callback;state.r[15]=0x8cf00000
            state.pc=0x8c0985ec;state.pr=0x8c000100;state.sr=0x700000f0
            saved=list(state.r[8:]);reference(ram,state)
            result=function(ctypes.byref(lists),level,callback)
            label=(level,free_count,active_head,callback)
            assert retail(result)==state.r[0],(label,'return')
            free_address=ctypes.cast(lists.free_head,ctypes.c_void_p).value
            assert retail(free_address)==struct.unpack_from('<I',ram,0x79f048)[0],(label,'free')
            native_heads=[retail(ctypes.cast(p,ctypes.c_void_p).value) for p in lists.level_heads]
            assert native_heads==list(struct.unpack_from('<8I',ram,0x79f04c)),(label,'heads')
            for i in range(3):
                assert [retail(p) for p in tasks[i].slots]==list(struct.unpack_from('<12I',ram,addresses[i]-0x8c000000)),(label,i,'task fields')
            assert list(state.r[8:])==saved and state.pr==0x8c000100
            count+=1
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':count,'levels_checked':list(range(9)),
            'empty_single_two_task_free_lists':True,'active_head_alias_cases':True,
            'routine_start':'0x8c0985ec','routine_end_exclusive':'0x8c09865c',
            'retail_span_sha256':hashlib.sha256(binary[0x885ec:0x8865c]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/task_lists.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'Native pointer mapping, all 12 fields of three tasks and list heads versus a subset instruction interpreter. Levels outside 0..8 unsupported. Default callback remains a retail address token; scheduler execution and hardware unverified.'}
    (root/'analysis/task_lists_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{count} base task creation/list comparisons passed')
if __name__=='__main__':main()
