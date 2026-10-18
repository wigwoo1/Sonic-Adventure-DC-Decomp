"""Original callback installation, with only zero-callback service hooked."""
import ctypes,hashlib,itertools,json,struct,sys
from verify_startup import ROOT,State,reference
Remove=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32))
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
    native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    sr=ctypes.c_uint32();events=[];original=[];errors=[]
    @Remove
    def remove(ctx,driver,index,status):
        if ctx!=0x1234:errors.append('context')
        events.append((driver,index,status[0]))
        if mutate:status[0]^=0x010000a1;struct.pack_into('<I',native,0xa00230,0xdeadbeef)
    def hook(memory,regs,status):
        original.append((regs[4],regs[5],status))
        if mutate:status^=0x010000a1;struct.pack_into('<I',memory,0xa00230,0xdeadbeef)
        return status
    dll_path=ROOT/'work/sonic-adventure/build/callback_install.dll'
    fn=ctypes.CDLL(str(dll_path)).sa_callback_install_8c6412c2
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),*([ctypes.c_uint32]*4),ctypes.c_void_p,Remove];fn.restype=None
    connected='--connected' in sys.argv
    if connected:
        native_fn=ctypes.CDLL(str(dll_path)).sa_callback_install_connected
        native_fn.argtypes=fn.argtypes[:6];native_fn.restype=None
        fn=lambda *args:native_fn(*args[:6])
    cases=0
    for driver,index,callback,argument,initial_sr,mutate in itertools.product([0x8ca00000,0xaca00000],[0,1,2,3,4,5,6,7,8,9,127,0x20000000,0x40000001],[0,0x8c645b1e,0xffffffff],[0,0xffffffff],[0,1,0x60,0xf0,0x700083f3],[False,True]):
        for memory in (old,native):memory[0xa00000:0xa01000]=b'\xa5'*4096
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4:8]=[driver,index,callback,argument]
        regs.r[15]=0x8cf00000;regs.pc=0x8c6412c2;regs.pr=0x8c000100;regs.sr=initial_sr
        saved=list(regs.r[8:]);sr.value=initial_sr;events.clear();original.clear()
        reference(old,regs,{} if connected else {0x8c6411b0:hook})
        fn(view,ctypes.byref(sr),driver,index,callback,argument,0x1234,remove)
        assert events==original and sr.value==regs.sr,(driver,index,callback,argument,initial_sr,mutate,events,original,sr.value,regs.sr)
        assert old[0xa00000:0xa01000]==native[0xa00000:0xa01000]
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        cases+=1
    assert not errors,errors
    out=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':cases,'routine_start':'0x8c6412c2','routine_end_exclusive':'0x8c64133e','retail_span_sha256':hashlib.sha256(binary[0x6312c2:0x63133e]).hexdigest(),'source_sha256':hashlib.sha256((out/'source/callback_install.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Slot stores, wrapping index arithmetic, RAM aliases, interrupt mask restoration, T-bit effects and zero-callback service mutations versus original instructions. Zero-callback removal service hooked; invalid unmapped pointers and CPU exception delivery unimplemented.'}
    if connected:
        report['scope']='Original installer and original default-restoration body both executed, with no service hooks; RAM stores, preserved arguments, index boundaries, aliases and SR effects compared. CPU exception delivery unimplemented.'
        report['default_restoration']={'routine_start':'0x8c6411b0','routine_end_exclusive':'0x8c6412c2','retail_span_sha256':hashlib.sha256(binary[0x6311b0:0x6312c2]).hexdigest(),'executed_cases':cases//3,'nonzero_installation_cases':cases*2//3}
    name='callback_install_connected_verification.json' if connected else 'callback_install_verification.json'
    (out/'analysis'/name).write_text(json.dumps(report,indent=2)+'\n')
    print(f'{cases} callback installation comparisons passed')
if __name__=='__main__':main()
