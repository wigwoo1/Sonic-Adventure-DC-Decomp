"""Heap release/coalescing comparisons using complete arena bytes after rebasing."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_task_heap import Heap
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    dll_path=ROOT/'work/sonic-adventure/build/heap_free_ram.dll'
    blocks=[16+i*64 for i in range(6)];terminal=400;positions=blocks+[terminal]
    cases=[(None,())]
    for released in blocks:
        others=[b for b in blocks if b!=released]
        for length in range(6):
            cases.extend((released,order) for order in itertools.permutations(others,length))
    native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/heap_free_ram.dll')).sa_heap_free_ram_8c098b42
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32];fn.restype=ctypes.c_uint32
    verified=0
    for released,order in cases:
        original=bytearray([0xa5])*512
        for i,block in enumerate(blocks):struct.pack_into('<4I',original,block,0,0,blocks[i+1] if i<5 else terminal,blocks[i-1] if i else 0)
        struct.pack_into('<4I',original,terminal,0,0,0,blocks[-1])
        for i,block in enumerate(order):struct.pack_into('<2I',original,block,order[i+1] if i+1<len(order) else 0,order[i-1] if i else 0)
        for position in positions:
            for field in range(4):
                value=struct.unpack_from('<I',original,position+field*4)[0]
                if value:struct.pack_into('<I',original,position+field*4,0x8c880000+value)
        head=order[0] if order else 0
        for initial_sr,incoming_r0 in itertools.product([0,0x700083f3],[0,0xabcdef12]):
            for memory in (ram,native):
                memory[0x880000:0x880200]=original;struct.pack_into('<I',memory,0x79f03c,0x8c880000+head if head else 0)
            state=State();state.r[:]=[0x12340000+i for i in range(16)];state.r[0]=incoming_r0
            state.r[4]=0x8c880000+released+16 if released is not None else 0
            state.r[15]=0x8cf00000;state.pc=0x8c098b42;state.pr=0x8c000100;state.sr=initial_sr
            saved=list(state.r[8:]);sr=ctypes.c_uint32(initial_sr);reference(ram,state)
            assert fn(view,ctypes.byref(sr),0x8c880000+released+16 if released is not None else 0,incoming_r0)==state.r[0] and sr.value==state.sr,(released,order)
            assert ram[0x880000:0x880200]==native[0x880000:0x880200] and ram[0x79f03c:0x79f040]==native[0x79f03c:0x79f040]
            assert list(state.r[8:])==saved and state.pr==0x8c000100
            verified+=1
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':verified,
            'routine_start':'0x8c098b42','routine_end_exclusive':'0x8c098c2c',
            'retail_span_sha256':hashlib.sha256(binary[0x88b42:0x88c2c]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/heap_free_ram.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'All 512 arena bytes, free head, return r0, full SR and callee-saved registers compared with subset SH-4 execution for six physical blocks and all free-list permutations excluding the released block, plus null. Covers left/right/both/no merge and singleton free-list behavior. Invalid-pointer and original infinite-loop path not executed; hardware/gameplay unverified.'}
    (root/'analysis/heap_free_ram_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{verified} RAM heap release/coalescing comparisons passed')
if __name__=='__main__':main()
