"""Execute original event-chain dispatcher against recovered native source."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
Invoke=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32))
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    original_ram=bytearray(16*1024*1024);original_ram[0x10000:0x10000+len(binary)]=binary
    native_ram=bytearray(original_ram)
    view=(ctypes.c_uint8*len(native_ram)).from_buffer(native_ram)
    sr=ctypes.c_uint32();events=[];original=[];errors=[]
    def offset(address):return (address&0x1fffffff)-0xc000000
    def mutate_callback(memory,index,status):
        if index==0 and count>1:
            if mutation=='cut':struct.pack_into('<I',memory,0xa00004,0)
            elif mutation=='skip':struct.pack_into('<I',memory,0xa00004,nodes[2] if count>2 else 0)
            elif mutation=='head':struct.pack_into('<I',memory,0x893a80,0)
            elif mutation=='argument':struct.pack_into('<I',memory,0xa00018,0xdeadbeef)
        # Callback changes SR; dispatcher must preserve that change.
        return (status^0x31) if mutation!='none' else status
    @Invoke
    def invoke(ctx,token,arg,status):
        if ctx!=0x1234:errors.append('context')
        index=(token-0x8cb00000)//4
        events.append((token,arg,status[0]))
        status[0]=mutate_callback(native_ram,index,status[0])
    def hook(index):
        def callback(memory,regs,status):
            original.append((0x8cb00000+index*4,regs[4],status))
            return mutate_callback(memory,index,status)
        return callback
    hooks={0x8cb00000+i*4:hook(i) for i in range(6)}
    dll_path=ROOT/'work/sonic-adventure/build/vblank_dispatch.dll'
    fn=ctypes.CDLL(str(dll_path)).sa_vblank_dispatch_8c645b1e
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_void_p,Invoke];fn.restype=None
    cases=0
    for count,mutation,initial_sr,alias in itertools.product(range(7),['none','cut','skip','head','argument'],[0,1,0x60,0xf0,0x40000001,0x700083f3],[0x8c000000,0xac000000]):
        nodes=[alias+0xa00000+i*16 for i in range(count)]
        for memory in (original_ram,native_ram):
            memory[0xa00000:0xa00100]=bytes(256)
            struct.pack_into('<I',memory,0x893a80,nodes[0] if nodes else 0)
            for i,node in enumerate(nodes):
                struct.pack_into('<4I',memory,offset(node),0x8cb00000+i*4,nodes[i+1] if i+1<count else 0,0x98760000+i,0x80000058)
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[15]=0x8cf00000;regs.sr=initial_sr;regs.pc=0x8c645b1e;regs.pr=0x8c000100
        saved=list(regs.r[8:]);sr.value=initial_sr;events.clear();original.clear()
        reference(original_ram,regs,hooks)
        fn(view,ctypes.byref(sr),0x1234,invoke)
        assert events==original,(count,mutation,initial_sr,alias,events,original)
        assert sr.value==regs.sr
        assert native_ram[0xa00000:0xa00100]==original_ram[0xa00000:0xa00100]
        assert native_ram[0x893a80:0x893a84]==original_ram[0x893a80:0x893a84]
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        cases+=1
    assert not errors,errors
    out=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':cases,'routine_start':'0x8c645b1e','routine_end_exclusive':'0x8c645b44','retail_span_sha256':hashlib.sha256(binary[0x635b1e:0x635b44]).hexdigest(),'source_sha256':hashlib.sha256((out/'source/vblank_dispatch.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Empty and multi-node event chains, RAM address aliases, interrupt mask changes, callback SR changes and post-callback list mutations matched against original instructions. Callback bodies hooked; CPU privileged exception behavior and hardware interrupt delivery unimplemented.'}
    (out/'analysis/vblank_dispatch_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{cases} event-chain dispatch comparisons passed')
if __name__=='__main__':main()
