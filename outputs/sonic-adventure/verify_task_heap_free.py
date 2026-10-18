"""Heap release/coalescing comparisons using complete arena bytes after rebasing."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_task_heap import Heap
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    dll_path=ROOT/'work/sonic-adventure/build/task_heap.dll'
    function=ctypes.CDLL(str(dll_path)).sa_task_heap_free_8c098b42
    function.argtypes=[ctypes.POINTER(Heap),ctypes.c_void_p];function.restype=None
    blocks=[16+i*64 for i in range(6)];terminal=400;positions=blocks+[terminal]
    cases=[(None,())]
    for released in blocks:
        others=[b for b in blocks if b!=released]
        for length in range(6):
            cases.extend((released,order) for order in itertools.permutations(others,length))
    outcomes=set()
    for released,order in cases:
        arena=bytearray([0xa5])*512
        for i,block in enumerate(blocks):
            struct.pack_into('<4I',arena,block,0,0,blocks[i+1] if i<5 else terminal,blocks[i-1] if i else 0)
        struct.pack_into('<4I',arena,terminal,0,0,0,blocks[-1])
        for i,block in enumerate(order):
            struct.pack_into('<2I',arena,block,order[i+1] if i+1<len(order) else 0,order[i-1] if i else 0)
        original=bytearray(arena)
        for position in positions:
            for field in range(4):
                value=struct.unpack_from('<I',original,position+field*4)[0]
                if value:struct.pack_into('<I',original,position+field*4,0x8c880000+value)
        ram[0x880000:0x880200]=original
        head=order[0] if order else 0
        struct.pack_into('<I',ram,0x79f03c,0x8c880000+head if head else 0)
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=0x8c880000+released+16 if released is not None else 0
        state.r[15]=0x8cf00000;state.pc=0x8c098b42;state.pr=0x8c000100;state.sr=0x700000f0
        saved=list(state.r[8:]);reference(ram,state)
        buffer=(ctypes.c_uint8*len(arena)).from_buffer(arena);heap=Heap(buffer,head)
        function(ctypes.byref(heap),ctypes.addressof(buffer)+released+16 if released is not None else None)
        expected=bytearray(ram[0x880000:0x880200])
        for position in positions:
            for field in range(4):
                value=struct.unpack_from('<I',expected,position+field*4)[0]
                if 0x8c880000<=value<0x8c880200:struct.pack_into('<I',expected,position+field*4,value-0x8c880000)
        assert arena==expected,(released,order,'arena')
        original_head=struct.unpack_from('<I',ram,0x79f03c)[0]
        assert heap.free_head==(original_head-0x8c880000 if original_head else 0),(released,order,'head')
        assert list(state.r[8:])==saved and state.pr==0x8c000100
        outcomes.add(bytes(arena))
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':len(cases),'distinct_arena_outcomes':len(outcomes),
            'routine_start':'0x8c098b42','routine_end_exclusive':'0x8c098c2c',
            'retail_span_sha256':hashlib.sha256(binary[0x88b42:0x88c2c]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/task_heap.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'All 512 arena bytes, free head and callee-saved registers compared with subset SH-4 execution for six physical blocks and all free-list permutations excluding the released block, plus null. Covers left/right/both/no merge and singleton free-list behavior. Invalid-pointer and original infinite-loop path not executed; hardware/gameplay unverified.'}
    (root/'analysis/task_heap_free_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{len(cases)} heap release/coalescing comparisons passed')
if __name__=='__main__':main()
