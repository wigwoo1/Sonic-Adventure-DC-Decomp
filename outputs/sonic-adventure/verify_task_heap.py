"""Compare native arena offsets with rebased retail heap block pointers."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
class Heap(ctypes.Structure):
    _fields_=[('arena',ctypes.POINTER(ctypes.c_uint8)),('free_head',ctypes.c_uint32),('field_8c79f040',ctypes.c_uint32)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    dll_path=ROOT/'work/sonic-adventure/build/task_heap.dll'
    function=ctypes.CDLL(str(dll_path)).sa_task_heap_allocate_8c098a80
    function.argtypes=[ctypes.POINTER(Heap),ctypes.c_uint32];function.restype=ctypes.c_void_p
    sizes=list(range(0,273))+[511,512,513,0x7fffffff,0xffffffdf,0xffffffe0,0xffffffe1,0xfffffff0,0xffffffff]
    orders=[()]+[p for length in range(1,4) for p in itertools.permutations([16,512,2048],length)]
    available={16:64,512:256,2048:128};records=[]
    for order in orders:
      for size in sizes:
       for missing_end in [False,True]:
        arena=bytearray([0xa5])*4096;positions=set()
        for i,block in enumerate(order):
            end=block+available[block];positions.update([block,end])
            struct.pack_into('<4I',arena,block,order[i+1] if i+1<len(order) else 0,
                             order[i-1] if i else 0,0 if missing_end and i==0 else end,0)
            struct.pack_into('<4I',arena,end,0,0,0,block)
        head=order[0] if order else 0;total=((size+31)&0xffffffff)&~15
        for block in order:
            if total<available[block]:positions.add(block+total)
        original=bytearray(arena)
        # Only initialized header fields contain offsets before allocation.
        initialized={p for block in order for p in [block,block+available[block]]}
        for position in initialized:
            for field in range(4):
                value=struct.unpack_from('<I',original,position+field*4)[0]
                if value:struct.pack_into('<I',original,position+field*4,0x8c880000+value)
        ram[0x880000:0x881000]=original
        struct.pack_into('<I',ram,0x79f03c,0x8c880000+head if head else 0)
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=size;state.r[15]=0x8cf00000;state.pc=0x8c098a80
        state.pr=0x8c000100;state.sr=0x700000f0;saved=list(state.r[8:])
        reference(ram,state)
        buffer=(ctypes.c_uint8*len(arena)).from_buffer(arena);heap=Heap(buffer,head)
        result=function(ctypes.byref(heap),size)
        original_result=state.r[0]-0x8c880000 if state.r[0] else None
        native_result=result-ctypes.addressof(buffer) if result else None
        label=(order,size,missing_end)
        assert original_result==native_result,(label,'return')
        expected=bytearray(ram[0x880000:0x881000])
        for position in positions:
            for field in range(4):
                value=struct.unpack_from('<I',expected,position+field*4)[0]
                if 0x8c880000<=value<0x8c881000:
                    struct.pack_into('<I',expected,position+field*4,value-0x8c880000)
        assert arena==expected,(label,'arena bytes')
        original_head=struct.unpack_from('<I',ram,0x79f03c)[0]
        assert heap.free_head==(original_head-0x8c880000 if original_head else 0),(label,'head')
        assert list(state.r[8:])==saved and state.pr==0x8c000100
        records.append(original_result is not None)
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':len(records),'successful_allocations':sum(records),
            'free_list_orders_checked':len(orders),'request_sizes_checked':len(sizes),
            'routine_start':'0x8c098a80','routine_end_exclusive':'0x8c098b0c',
            'retail_span_sha256':hashlib.sha256(binary[0x88a80:0x88b0c]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/task_heap.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'Complete 4096-byte arena, free head and return compared after pointer rebasing against a subset integer interpreter. Includes zero/overflow requests, split/exact/skip cases and missing end markers. Valid arena metadata required; initialization/free/coalescing, hardware and gameplay unverified.'}
    (root/'analysis/task_heap_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{len(records)} heap allocation comparisons passed')
if __name__=='__main__':main()
