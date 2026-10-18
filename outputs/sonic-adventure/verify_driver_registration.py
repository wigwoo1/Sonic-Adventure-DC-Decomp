"""Original VBlank driver wrapper, enable stub, installer and default body."""
import ctypes,hashlib,itertools,json,struct,sys
from verify_startup import ROOT,State,reference
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    secondary='--secondary' in sys.argv
    helper_start=0x8c658c24 if secondary else 0x8c658c20
    wrapper_start=0x8c64137c if secondary else 0x8c64135a
    wrapper_end=0x8c6413a6 if secondary else 0x8c64137c
    # This helper really is a constant-return stub in this retail build.
    assert struct.unpack_from('<2H',binary,helper_start-0x8c010000)==(0x000b,0xe001)
    old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
    native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    sr=ctypes.c_uint32();dll_path=ROOT/'work/sonic-adventure/build/callback_install.dll';dll=ctypes.CDLL(str(dll_path))
    fn=getattr(dll,'sa_secondary_callback_register_8c64137c' if secondary else 'sa_vblank_callback_register_8c64135a')
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32];fn.restype=ctypes.c_uint32
    enable=getattr(dll,'sa_driver_enable_8c658c24' if secondary else 'sa_driver_enable_8c658c20');enable.argtypes=[ctypes.c_uint32];enable.restype=ctypes.c_uint32
    helper_cases=0
    for value in [0,1,2,0x7fffffff,0x80000000,0xffffffff]:
        regs=State();regs.r[4]=value;regs.r[15]=0x8cf00000;regs.pc=helper_start;regs.pr=0x8c000100;regs.sr=0x40000001
        reference(old,regs)
        assert enable(value)==regs.r[0]==1 and regs.sr==0x40000001
        helper_cases+=1
    cases=0
    for driver,callback,argument,initial_sr in itertools.product([0x8ca00000,0xaca00000,0x8ca00100],[0,1,0x8c645b1e,0xffffffff],[0,1,0xffffffff],[0,1,0x60,0xf0,0x40000001,0x700083f3]):
        for memory in (old,native):
            memory[0xa00000:0xa01000]=b'\xa5'*4096
            struct.pack_into('<I',memory,0x67c004,driver)
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=callback;regs.r[5]=argument
        regs.r[15]=0x8cf00000;regs.pc=wrapper_start;regs.pr=0x8c000100;regs.sr=initial_sr
        saved=list(regs.r[8:]);sr.value=initial_sr
        reference(old,regs) # No service hooks: entire original chain executes.
        assert fn(view,ctypes.byref(sr),callback,argument)==regs.r[0]==0
        assert sr.value==regs.sr
        assert old[0xa00000:0xa01000]==native[0xa00000:0xa01000]
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        cases+=1
    out=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':cases,'enable_stub_cases_passed':helper_cases,'routine_start':'0x8c64135a','routine_end_exclusive':'0x8c64137c','retail_span_sha256':hashlib.sha256(binary[0x63135a:0x63137c]).hexdigest(),'enable_stub':{'routine_start':'0x8c658c20','routine_end_exclusive':'0x8c658c24','retail_span_sha256':hashlib.sha256(binary[0x648c20:0x648c24]).hexdigest()},'source_sha256':hashlib.sha256((out/'source/callback_install.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Four original bodies executed without service hooks: VBlank wrapper, constant-return enable stub, callback installer and default restoration. Compare slot memory, return, SR, saved registers and RAM aliases. Actual hardware interrupts and invalid-pointer exceptions unimplemented.'}
    report['routine_start']=hex(wrapper_start);report['routine_end_exclusive']=hex(wrapper_end)
    report['retail_span_sha256']=hashlib.sha256(binary[wrapper_start-0x8c010000:wrapper_end-0x8c010000]).hexdigest()
    report['enable_stub']={'routine_start':hex(helper_start),'routine_end_exclusive':hex(helper_start+4),'retail_span_sha256':hashlib.sha256(binary[helper_start-0x8c010000:helper_start-0x8c010000+4]).hexdigest()}
    report['secondary_registration']=secondary
    if secondary:report['scope']=report['scope'].replace('VBlank wrapper','secondary driver wrapper')
    name='secondary_driver_registration_verification.json' if secondary else 'driver_registration_verification.json'
    (out/'analysis'/name).write_text(json.dumps(report,indent=2)+'\n')
    print(f'{cases} driver registration and {helper_cases} enable stub comparisons passed')
if __name__=='__main__':main()
