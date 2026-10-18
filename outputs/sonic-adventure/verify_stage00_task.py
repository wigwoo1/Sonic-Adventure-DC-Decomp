"""Compare native task fields/callbacks with STG00's original integer instructions."""
import ctypes, hashlib, json, pathlib, struct
from verify_startup import State, reference
ROOT=pathlib.Path(__file__).resolve().parents[2]
class Work(ctypes.Structure):
    _fields_=[('state',ctypes.c_uint8),('reserved',ctypes.c_uint8*5),('cached_variant',ctypes.c_uint16)]
class Task(ctypes.Structure):
    _fields_=[('work',ctypes.POINTER(Work)),('destructor',ctypes.c_void_p)]
Query=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p)
Initialize=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.POINTER(Task))
class Services(ctypes.Structure):
    _fields_=[('context',ctypes.c_void_p),('query_variant',Query),('initialize',Initialize)]
def main():
    root=ROOT/'outputs/sonic-adventure';dll_path=ROOT/'work/sonic-adventure/build/stage00_task.dll'
    dll=ctypes.CDLL(str(dll_path));update=dll.sa_stage00_update_0c900000
    update.argtypes=[ctypes.POINTER(Task),ctypes.POINTER(Services)];update.restype=None
    destructor=dll.sa_stage00_destructor_0c900d72
    destructor.argtypes=[ctypes.POINTER(Task)];destructor.restype=None
    destructor_address=ctypes.cast(destructor,ctypes.c_void_p).value
    overlay=(ROOT/'work/sonic-adventure/decoded/SONICADV/STG00.BIN').read_bytes()
    assert overlay[0xD72:0xD76]==b'\x0b\x00\x09\x00','Retail destructor is not rts/nop'
    base_ram=bytearray(16*1024*1024);base_ram[0x900000:0x900000+len(overlay)]=overlay
    destructor_ram=bytearray(base_ram);destructor_state=State()
    destructor_state.r[:]=[0x12340000+i for i in range(16)]
    destructor_state.sr=0x700000F1;destructor_state.pr=0x8C000100;destructor_state.pc=0x0C900D72
    destructor_before=(list(destructor_state.r),destructor_state.sr,destructor_state.pr)
    assert reference(destructor_ram,destructor_state)==2
    assert destructor_ram==base_ram and destructor_before==(list(destructor_state.r),destructor_state.sr,destructor_state.pr)
    cases=[(state,0x76,0x12345676,False) for state in range(256)]
    cases.extend((state,cache,query,False) for state in range(4)
                 for cache in [0,1,255,256,32768,65535] for query in [0,1,255,256,0x80000000,0xffffffff])
    cases.extend((1,cache,query,True) for cache in [0,255,65535] for query in [0,0xffffffff])
    results=[]
    for initial,cache,query,replace in cases:
        work=Work(initial,(ctypes.c_uint8*5)(11,22,33,44,55),cache)
        replacement=Work(99,(ctypes.c_uint8*5)(66,77,88,99,111),0xABCD)
        task=Task(ctypes.pointer(work),0x12345678);events=[];errors=[]
        @Query
        def query_callback(context):
            if context!=0x1234:errors.append('query context differs')
            events.append('query');return query
        @Initialize
        def initialize_callback(context,task_pointer):
            if context!=0x1234 or ctypes.addressof(task_pointer.contents)!=ctypes.addressof(task):errors.append('initialize arguments differ')
            events.append('initialize')
            if replace:
                work.state=254;work.cached_variant=0x1234
                task_pointer.contents.work=ctypes.pointer(replacement)
        services=Services(0x1234,query_callback,initialize_callback)
        ram=bytearray(base_ram);ram[0x880100:0x880108]=bytes(work);ram[0x881100:0x881108]=bytes(replacement)
        struct.pack_into('<I',ram,0x880000+0x20,0x8C880100)
        struct.pack_into('<I',ram,0x880000+0x18,0x12345678)
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=0x8C880000;state.r[15]=0x8CF00000;state.sr=0x700000F0
        state.pr=0x8C000100;state.pc=0x0C900000;original_events=[]
        def query_hook(memory,regs,sr):
            original_events.append('query');regs[0]=query;regs[4]=0xDEAD0004;return sr^1
        def initialize_hook(memory,regs,sr):
            assert regs[4]==0x8C880000
            original_events.append('initialize')
            if replace:
                memory[0x880100]=254;struct.pack_into('<H',memory,0x880106,0x1234)
                struct.pack_into('<I',memory,0x880020,0x8C881100)
            regs[0]=0xBAD00000;regs[4]=0xBAD00004;return sr^1
        steps=reference(ram,state,{0x8C04F7E0:query_hook,0x0C900D00:initialize_hook})
        update(ctypes.byref(task),ctypes.byref(services))
        assert not errors,errors
        assert events==original_events,'Callback sequence differs'
        assert bytes(work)==ram[0x880100:0x880108],'Original work fields differ'
        assert bytes(replacement)==ram[0x881100:0x881108],'Replacement work fields differ'
        expected_pointer=struct.unpack_from('<I',ram,0x880020)[0]
        assert ctypes.addressof(task.work.contents)==(ctypes.addressof(replacement) if expected_pointer==0x8C881100 else ctypes.addressof(work))
        expected_destructor=struct.unpack_from('<I',ram,0x880018)[0]
        assert task.destructor==(destructor_address if expected_destructor==0x0C900D72 else expected_destructor),'Destructor installation differs'
        assert state.r[13]==0x1234000D and state.r[14]==0x1234000E and state.r[15]==0x8CF00000 and state.pr==0x8C000100
        # The recovered empty destructor must not change this native task view.
        snapshot=(bytes(task),bytes(work),bytes(replacement));destructor(ctypes.byref(task))
        assert snapshot==(bytes(task),bytes(work),bytes(replacement))
        results.append({'initial_state':initial,'cached_variant':cache,'query_result':query,'replace_work_in_callback':replace,
                        'original_steps':steps,'task_fields_and_callbacks_match':True})
    report={'module':'STG00.BIN','routine':'0x0c900000','destructor':'0x0c900d72',
            'overlay_sha256':hashlib.sha256(overlay).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/stage00_task.c').read_bytes()).hexdigest(),'cases':results,
            'scope':'Projected task/work fields and callback sequence compared with original instructions. Query and initialization use hooks; complete task ABI and gameplay remain unverified.'}
    (root/'analysis/stage00_task_verification.json').write_text(json.dumps(report,indent=2))
    print(len(results),'STG00 native/original task comparisons passed')
if __name__=='__main__':main()
