"""Retail registration and configuration chain, including service mutation."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_frame_countdown import Countdown
from verify_frame_tick import Services as TickServices,Call
Remove=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_uint32)
Register=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,*([ctypes.c_uint32]*4))
class Registration(ctypes.Structure):
    _fields_=[('context',ctypes.c_void_p),('remove',Remove),('register',Register)]
class Runtime(ctypes.Structure):
    _fields_=[('state',ctypes.POINTER(Countdown)),('handle',ctypes.POINTER(ctypes.c_uint32)),('registration',Registration),('context',ctypes.c_void_p),('configured',Call),('mode',ctypes.POINTER(ctypes.c_uint32)),('table',ctypes.POINTER(ctypes.c_uint32)),('tick',TickServices)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    handle=ctypes.c_uint32();state=Countdown();events=[];original=[];errors=[]
    def snap():return (handle.value,)+tuple(getattr(state,n) for n,_ in state._fields_)
    def old_snap(memory):return (struct.unpack_from('<I',memory,0x8900fc)[0],)+struct.unpack_from('<4I',memory,0x754ebc)
    @Remove
    def remove(ctx,value):
        if ctx!=0x1234:errors.append('remove context')
        events.append(('remove',value,snap()))
        if mutate:handle.value=0xdeadbeef;state.current_ticks=11
    @Register
    def register(ctx,event,callback,priority,arg):
        if ctx!=0x1234:errors.append('register context')
        events.append(('register',event,callback,priority,arg,snap()))
        if mutate:handle.value=0xbad;state.iteration_index=3
        return new_handle
    @Call
    def configured(ctx):
        if ctx!=0x1234:errors.append('configured context')
        events.append(('configured',snap()))
        if mutate:state.initial_ticks=17
    def old_remove(memory,regs,sr):
        original.append(('remove',regs[4],old_snap(memory)))
        if mutate:struct.pack_into('<I',memory,0x8900fc,0xdeadbeef);struct.pack_into('<I',memory,0x754ec0,11)
        return sr
    def old_register(memory,regs,sr):
        original.append(('register',*regs[4:8],old_snap(memory)))
        if mutate:struct.pack_into('<I',memory,0x8900fc,0xbad);struct.pack_into('<I',memory,0x754ec8,3)
        regs[0]=new_handle
        return sr
    def old_configured(memory,regs,sr):
        original.append(('configured',old_snap(memory)))
        if mutate:struct.pack_into('<I',memory,0x754ebc,17)
        return sr
    hooks={0x8c645c54:old_remove,0x8c64594c:old_register,0x8c055c44:old_configured}
    registration=Registration(0x1234,remove,register)
    runtime=Runtime(ctypes.pointer(state),ctypes.pointer(handle),registration,0x1234,configured,None,None,TickServices())
    dll_path=ROOT/'work/sonic-adventure/build/frame_runtime.dll';dll=ctypes.CDLL(str(dll_path))
    standalone=dll.sa_countdown_register_8c605234
    standalone.argtypes=[ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.POINTER(Registration)];standalone.restype=None
    connected=dll.sa_frame_runtime_configure
    connected.argtypes=[ctypes.POINTER(Runtime),ctypes.c_uint32,ctypes.c_uint32];connected.restype=None
    counts={'registration':0,'connected_configuration':0}
    for path,previous,callback,new_handle,mutate in itertools.product(['registration','connected_configuration'],[0,1,0xffffffff],[0,0x8c051810,0xffffffff],[0,0x8c892840,0xffffffff],[False,True]):
        handle.value=previous;state.initial_ticks=5;state.current_ticks=6;state.iteration_limit=7;state.iteration_index=8
        struct.pack_into('<I',ram,0x8900fc,previous);struct.pack_into('<4I',ram,0x754ebc,*snap()[1:])
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[15]=0x8cf00000;regs.pr=0x8c000100
        if path=='registration':regs.pc=0x8c605234;regs.r[4]=callback
        else:regs.pc=0x8c051760;regs.r[4]=callback;regs.r[5]=2
        saved=list(regs.r[8:]);events.clear();original.clear();reference(ram,regs,hooks)
        if path=='registration':standalone(ctypes.byref(handle),callback,ctypes.byref(registration))
        else:connected(ctypes.byref(runtime),callback,2)
        assert events==original,(path,previous,callback,new_handle,mutate,events,original)
        assert snap()==old_snap(ram)
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        counts[path]+=1
    assert not errors,errors
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':counts,'routine_start':'0x8c605234','routine_end_exclusive':'0x8c60526a','retail_span_sha256':hashlib.sha256(binary[0x5f5234:0x5f526a]).hexdigest(),'source_sha256':hashlib.sha256((root/'source/frame_countdown.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Original registration and original configuration-to-registration chain executed; compare state, arguments, call order and service mutation. Event registry removal/insertion and final configuration service are hooks. Hardware delivery unimplemented.'}
    (root/'analysis/frame_registration_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{counts} comparisons passed')
if __name__=='__main__':main()
