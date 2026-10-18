"""Original fixed heap initialization and connected allocation/release traces."""
import ctypes,hashlib,json,random,struct,sys
from verify_startup import ROOT,State,reference
from verify_task_heap import Heap
def main(zero_fill=False):
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    dll_path=ROOT/('work/sonic-adventure/build/task_heap_connected.dll' if zero_fill else 'work/sonic-adventure/build/task_heap.dll');dll=ctypes.CDLL(str(dll_path))
    initialize=dll.sa_task_heap_initialize_8c098a10;initialize.argtypes=[ctypes.POINTER(Heap)];initialize.restype=None
    allocate=dll.sa_task_heap_allocate_8c098a80;allocate.argtypes=[ctypes.POINTER(Heap),ctypes.c_uint32];allocate.restype=ctypes.c_void_p
    if zero_fill:
        zero=dll.sa_task_allocate_from_heap;zero.argtypes=[ctypes.POINTER(Heap),ctypes.c_uint32,ctypes.c_uint32];zero.restype=ctypes.c_void_p
    release=dll.sa_task_heap_free_8c098b42;release.argtypes=[ctypes.POINTER(Heap),ctypes.c_void_p];release.restype=None
    retail_base=0x8c7ee8d0;memory_base=retail_base-0x8c000000;length=0x90030
    arena=bytearray(length);buffer=(ctypes.c_uint8*length).from_buffer(arena)
    heap=Heap(buffer,0x1234,0x5678);positions={16,0x90010}
    def run(address,argument=0,second=0):
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=argument;state.r[5]=second;state.r[15]=0x8cf00000;state.pc=address;state.pr=0x8c000100;state.sr=0x700000f0
        saved=list(state.r[8:]);reference(ram,state)
        assert list(state.r[8:])==saved and state.pr==0x8c000100
        return state.r[0]
    def compare(label):
        expected=bytearray(ram[memory_base:memory_base+length])
        for position in positions:
            for field in range(4):
                value=struct.unpack_from('<I',expected,position+field*4)[0]
                if retail_base<value<retail_base+length:
                    struct.pack_into('<I',expected,position+field*4,value-retail_base)
        assert arena==expected,(label,'arena')
        head=struct.unpack_from('<I',ram,0x79f03c)[0]
        assert heap.free_head==(head-retail_base if head else 0),(label,'head')
        assert heap.field_8c79f040==struct.unpack_from('<I',ram,0x79f040)[0],(label,'field')
    for pattern in [0,0x35,0xa5]:
        arena[:]=bytes([pattern])*length
        ram[memory_base:memory_base+length]=arena
        struct.pack_into('<2I',ram,0x79f03c,0x1234,0x5678)
        run(0x8c098a10);initialize(ctypes.byref(heap));compare(('initialize',pattern))
    rng=random.Random(0x98a10);live=[];operations=[]
    requests=[1,15,16,17,64,28,0x8fff0,0x90000,0,4096,65536]
    for step in range(160):
        if live and (step%3==2 or rng.randrange(4)==0):
            offset=live.pop(rng.randrange(len(live)))
            run(0x8c098b42,retail_base+offset)
            release(ctypes.byref(heap),ctypes.addressof(buffer)+offset)
            operations.append({'operation':'free','offset':offset})
        else:
            size=requests[step] if step<len(requests) else rng.randrange(1,32769)
            if zero_fill:
                count=1 if step<len(requests) else rng.randrange(0,5)
                result=run(0x8c098b0c,count,size);native=zero(ctypes.byref(heap),count,size)
            else:
                count=1;result=run(0x8c098a80,size);native=allocate(ctypes.byref(heap),size)
            offset=result-retail_base if result else None
            assert offset==(native-ctypes.addressof(buffer) if native else None),(step,'return')
            if offset is not None:
                block=offset-16;positions.add(block)
                end=struct.unpack_from('<I',ram,memory_base+block+8)[0]-retail_base
                positions.add(end);live.append(offset)
            operations.append({'operation':'allocate','count':count,'size':size,'offset':offset})
        compare(step)
    for offset in live:
        run(0x8c098b42,retail_base+offset);release(ctypes.byref(heap),ctypes.addressof(buffer)+offset)
        operations.append({'operation':'free','offset':offset});compare('drain')
    root=ROOT/'outputs/sonic-adventure'
    report={'initialization_cases_passed':3,'connected_operations_passed':len(operations),
            'routine_start':'0x8c098a10','routine_end_exclusive':'0x8c098a5c',
            'retail_span_sha256':hashlib.sha256(binary[0x88a10:0x88a5c]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/task_heap.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'operations':operations,'zero_fill_wrapper_connected':zero_fill,
            'scope':'Actual retail initialization literals and connected original allocation/free instructions compared with all native arena bytes, guards, free head and additional cleared global after pointer rebasing. Subset interpreter; no hardware or gameplay verification.'}
    filename='task_heap_zero_connected_verification.json' if zero_fill else 'task_heap_lifecycle_verification.json'
    if zero_fill:
        report['connected_source_hashes']={name:hashlib.sha256((root/'source'/name).read_bytes()).hexdigest()
                                          for name in ['task_allocate.c','task_heap.c','task_heap_adapter.c']}
    (root/'analysis'/filename).write_text(json.dumps(report,indent=2))
    print(f'3 heap initializations and {len(operations)} connected operations passed')
if __name__=='__main__':main('--zero-fill' in sys.argv)
