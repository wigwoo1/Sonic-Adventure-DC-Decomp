"""Differential execution of the retail countdown tick; service bodies hooked."""
import ctypes,hashlib,itertools,json,struct,sys
from verify_startup import ROOT,State,reference
from verify_frame_countdown import Countdown
Query=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p)
Dispatch=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_uint32)
Call=ctypes.CFUNCTYPE(None,ctypes.c_void_p)
class Services(ctypes.Structure):
    _fields_=[('context',ctypes.c_void_p),('query',Query),('dispatch',Dispatch),('zero',Call),('every',Call),('finish',Call)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    state=Countdown();mode=ctypes.c_uint32();table=(ctypes.c_uint32*2296)()
    for i in range(len(table)):table[i]=0x8cb00000+i*4
    struct.pack_into('<2296I',ram,0x19e3e4,*table)
    events=[];original=[]
    def snapshot():return tuple(getattr(state,n) for n,_ in state._fields_)+(mode.value,)
    @Query
    def query(ctx):
        events.append(('query',snapshot()))
        if mutate:mode.value=9;state.current_ticks=7
        return variant
    @Dispatch
    def dispatch(ctx,target):
        events.append(('dispatch',target,snapshot()))
        if mutate:state.current_ticks=11;mode.value=0
    def service(name,delta):
        @Call
        def call(ctx):
            events.append((name,snapshot()))
            if mutate:state.iteration_index+=delta
        return call
    zero=service('zero',1);every=service('every',2);finish=service('finish',4)
    services=Services(None,query,dispatch,zero,every,finish)
    def old_snapshot(memory):return struct.unpack_from('<4I',memory,0x754ebc)+(struct.unpack_from('<I',memory,0x760970)[0],)
    def query_hook(memory,regs,sr):
        original.append(('query',old_snapshot(memory)))
        if mutate:struct.pack_into('<I',memory,0x760970,9);struct.pack_into('<I',memory,0x754ec0,7)
        regs[0]=variant
        return sr
    def dispatch_hook(target):
        def hook(memory,regs,sr):
            original.append(('dispatch',target,old_snapshot(memory)))
            if mutate:struct.pack_into('<I',memory,0x754ec0,11);struct.pack_into('<I',memory,0x760970,0)
            return sr
        return hook
    def old_service(name,delta):
        def hook(memory,regs,sr):
            original.append((name,old_snapshot(memory)))
            if mutate:
                index=struct.unpack_from('<I',memory,0x754ec8)[0]
                struct.pack_into('<I',memory,0x754ec8,index+delta)
            return sr
        return hook
    hooks={0x8c04f7e0:query_hook,0x8c643f98:old_service('zero',1),0x8c602cb8:old_service('every',2),0x8c09ce40:old_service('finish',4)}
    hooks.update({target:dispatch_hook(target) for target in table})
    use_runtime='--runtime' in sys.argv
    dll_path=ROOT/('work/sonic-adventure/build/frame_runtime.dll' if use_runtime else 'work/sonic-adventure/build/frame_countdown.dll')
    fn=ctypes.CDLL(str(dll_path)).sa_frame_countdown_tick_8c051810
    fn.argtypes=[ctypes.POINTER(Countdown),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(Services)];fn.restype=None
    if use_runtime:
        from verify_frame_registration import Runtime,Registration
        runtime=Runtime()
        runtime.state=ctypes.pointer(state);runtime.mode=ctypes.pointer(mode);runtime.table=table
        ctypes.memmove(ctypes.byref(runtime.tick),ctypes.byref(services),ctypes.sizeof(services))
        deliver=ctypes.CDLL(str(dll_path)).sa_frame_runtime_deliver_tick
        deliver.argtypes=[ctypes.POINTER(Runtime)];deliver.restype=None
        fn=lambda *_:deliver(ctypes.byref(runtime))
    cases=0
    for ticks,initial_mode,variant,mutate in itertools.product([0,1,2,0x7fffffff,0x80000000,0xffffffff],[0,4,5,9,0xffffffff],[0,1,7,8,255,256,0xffff,0xdeadffff],[False,True]):
        state.initial_ticks=3;state.current_ticks=ticks;state.iteration_limit=2;state.iteration_index=0
        mode.value=initial_mode;struct.pack_into('<4I',ram,0x754ebc,*snapshot()[:4]);struct.pack_into('<I',ram,0x760970,initial_mode)
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[15]=0x8cf00000;regs.pc=0x8c051810;regs.pr=0x8c000100
        saved=list(regs.r[8:]);events.clear();original.clear()
        reference(ram,regs,hooks)
        fn(ctypes.byref(state),ctypes.byref(mode),table,ctypes.byref(services))
        assert events==original,(ticks,initial_mode,hex(variant),mutate,events,original)
        assert snapshot()==old_snapshot(ram)
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        cases+=1
    out=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':cases,'routine_start':'0x8c051810','routine_end_exclusive':'0x8c05187e','retail_span_sha256':hashlib.sha256(binary[0x41810:0x4187e]).hexdigest(),'source_sha256':hashlib.sha256((out/'source/frame_countdown.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Boundary countdowns, modes, table indices including overlapping rows, callback order and state mutation compared against original opcodes; service implementations hooked. Interrupt delivery and cadence remain unimplemented.'}
    report['native_runtime_adapter']=use_runtime
    name='frame_runtime_tick_verification.json' if use_runtime else 'frame_tick_verification.json'
    (out/'analysis'/name).write_text(json.dumps(report,indent=2)+'\n')
    print(f'{cases} countdown tick comparisons passed')
if __name__=='__main__':main()
