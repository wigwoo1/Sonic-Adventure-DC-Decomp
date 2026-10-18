"""Configuration global writes and hook mutation versus retail SH4 instructions."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
class Countdown(ctypes.Structure):
    _fields_=[(name,ctypes.c_uint32) for name in ['initial_ticks','current_ticks','iteration_limit','iteration_index']]
Register=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_size_t)
Finish=ctypes.CFUNCTYPE(None,ctypes.c_void_p)
class Services(ctypes.Structure):
    _fields_=[('context',ctypes.c_void_p),('register',Register),('finish',Finish)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    state=Countdown();events=[];original=[];errors=[]
    def values():return tuple(getattr(state,name) for name,_ in state._fields_)
    @Register
    def register(context,token):
        if context!=0x1234:errors.append('context')
        events.append(('register',token,values()))
        if mutate:state.current_ticks=0x80000000;state.iteration_index=7
    @Finish
    def finish(context):
        if context!=0x1234:errors.append('context')
        events.append(('finish',values()))
        if mutate:state.initial_ticks=0x87654321
    def register_hook(memory,regs,sr):
        original.append(('register',regs[4],struct.unpack_from('<4I',memory,0x754ebc)))
        if mutate:struct.pack_into('<I',memory,0x754ec0,0x80000000);struct.pack_into('<I',memory,0x754ec8,7)
        return sr
    def finish_hook(memory,regs,sr):
        original.append(('finish',struct.unpack_from('<4I',memory,0x754ebc)))
        if mutate:struct.pack_into('<I',memory,0x754ebc,0x87654321)
        return sr
    services=Services(0x1234,register,finish);dll_path=ROOT/'work/sonic-adventure/build/frame_countdown.dll'
    fn=ctypes.CDLL(str(dll_path)).sa_frame_countdown_configure_8c051760
    fn.argtypes=[ctypes.POINTER(Countdown),ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(Services)];fn.restype=None
    cases=0
    for ticks,limit,mutate in itertools.product([0,1,2,3,0x7fffffff,0x80000000,0xffffffff],[0,1,2,3,0x7fffffff,0x80000000,0xffffffff],[False,True]):
        for i,(name,_) in enumerate(state._fields_):setattr(state,name,0x12345678+i)
        struct.pack_into('<4I',ram,0x754ebc,*values())
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)]
        regs.r[4]=ticks;regs.r[5]=limit;regs.r[15]=0x8cf00000;regs.pc=0x8c051760;regs.pr=0x8c000100
        saved=list(regs.r[8:]);events.clear();original.clear()
        reference(ram,regs,{0x8c605234:register_hook,0x8c055c44:finish_hook})
        fn(ctypes.byref(state),ticks,limit,ctypes.byref(services))
        assert events==original and values()==struct.unpack_from('<4I',ram,0x754ebc)
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        cases+=1
    assert not errors,errors
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':cases,'routine_start':'0x8c051760','routine_end_exclusive':'0x8c051784',
      'retail_span_sha256':hashlib.sha256(binary[0x41760:0x41784]).hexdigest(),
      'source_sha256':hashlib.sha256((root/'source/frame_countdown.c').read_bytes()).hexdigest(),
      'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
      'scope':'32-bit boundary settings and service mutation; compare global state and call order against original instructions. Registration and finish service bodies hooked; interrupt cadence and native concurrency unimplemented.'}
    (root/'analysis/frame_countdown_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{cases} countdown configuration comparisons passed')
if __name__=='__main__':main()

