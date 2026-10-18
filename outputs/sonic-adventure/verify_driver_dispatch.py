"""Tail callback dispatch versus original code; callback bodies hooked."""
import ctypes,hashlib,itertools,json,struct,sys
from verify_startup import ROOT,State,reference
Invoke=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32))
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
    native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    sr=ctypes.c_uint32();events=[];original=[];errors=[]
    def callback_effect(memory,status,arg):
        if mutate:
            status^=0x010000f1
            struct.pack_into('<2I',memory,slot_offset,0x8cb00004,0xdeadbeef)
        return status,(arg^0x87654321)&0xffffffff
    @Invoke
    def invoke(ctx,token,arg,status):
        if ctx!=0x1234:errors.append('context')
        events.append((token,arg,status[0]))
        status[0],result=callback_effect(native,status[0],arg)
        return result
    def hook(token):
        def callback(memory,regs,status):
            original.append((token,regs[4],status))
            status,regs[0]=callback_effect(memory,status,regs[4])
            return status
        return callback
    hooks={token:hook(token) for token in [0x8cb00000,0x8cb00004,0x8c645b1e]}
    dll_path=ROOT/'work/sonic-adventure/build/driver_dispatch.dll'
    fn=ctypes.CDLL(str(dll_path)).sa_driver_dispatch_8c6524d2
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_void_p,Invoke];fn.restype=ctypes.c_uint32
    installed='--installed' in sys.argv
    if installed:
        install=ctypes.CDLL(str(dll_path)).sa_callback_install_connected
        install.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),*([ctypes.c_uint32]*4)];install.restype=None
    cases=0
    for index,token,arg,initial_sr,mutate in itertools.product([0,1,2,8,127,0x20000001,0x40000002],[0x8cb00000,0x8cb00004,0x8c645b1e],[0,1,0xffffffff],[0,1,0xf0,0x700083f3],[False,True]):
        slot_offset=0x8a2f84+((index<<3)&0xffffffff)
        for memory in (old,native):struct.pack_into('<2I',memory,slot_offset,token,arg)
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=index;regs.r[15]=0x8cf00000;regs.pc=0x8c6524d2;regs.pr=0x8c000100;regs.sr=initial_sr
        saved=list(regs.r[8:]);sr.value=initial_sr;events.clear();original.clear()
        if installed:
            regs.pc=0x8c6412c2;regs.r[4:8]=[0x8c8a2d54,index,token,arg]
            reference(old,regs)
            install(view,ctypes.byref(sr),0x8c8a2d54,index,token,arg)
            assert sr.value==regs.sr
            regs.pc=0x8c6524d2;regs.r[4]=index
        reference(old,regs,hooks)
        result=fn(view,ctypes.byref(sr),index,0x1234,invoke)
        assert events==original and result==regs.r[0] and sr.value==regs.sr,(index,token,arg,initial_sr,mutate)
        assert old[slot_offset:slot_offset+8]==native[slot_offset:slot_offset+8]
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        cases+=1
    assert not errors,errors
    out=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':cases,'routine_start':'0x8c6524d2','routine_end_exclusive':'0x8c6524ec','retail_span_sha256':hashlib.sha256(binary[0x6424d2:0x6424ec]).hexdigest(),'source_sha256':hashlib.sha256((out/'source/driver_dispatch.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Index wrapping, callback and argument capture, tail return, SHLL T-bit effect and callback mutation versus original instructions. Callback bodies hooked; FPU preservation wrapper and actual hardware delivery not covered.'}
    if installed:report['scope']='Original callback installer and dispatcher executed in sequence; only final callback bodies hooked. Compare installed slot, argument and return capture, wrapping indices, SR and callback mutation. FPU preservation wrapper and actual hardware delivery not covered.'
    name='driver_dispatch_installed_verification.json' if installed else 'driver_dispatch_verification.json'
    (out/'analysis'/name).write_text(json.dumps(report,indent=2)+'\n')
    print(f'{cases} indexed callback dispatch comparisons passed')
if __name__=='__main__':main()
