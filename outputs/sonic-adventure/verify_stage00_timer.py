"""Original timer integer instructions versus native C; service bodies are hooks."""
import ctypes, hashlib, json, struct
from verify_startup import ROOT, State, reference
from verify_stage00_task import Work, Task
Spawn=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_int32,ctypes.c_int32,ctypes.c_uint32)
StateCall=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_int32)
TaskCall=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.POINTER(Task))
class Services(ctypes.Structure):
    _fields_=[('context',ctypes.c_void_p),('spawn',Spawn),('state',StateCall),('task',TaskCall)]

def main():
    root=ROOT/'outputs/sonic-adventure'
    overlay=(ROOT/'work/sonic-adventure/decoded/SONICADV/STG00.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x900000:0x900000+len(overlay)]=overlay
    dll_path=ROOT/'work/sonic-adventure/build/stage00_timer.dll'
    native=ctypes.CDLL(str(dll_path)).sa_stage00_timer_0c900d76
    native.argtypes=[ctypes.POINTER(Task),ctypes.POINTER(Services)];native.restype=None
    cases=[(counter,47,False) for counter in range(65536)]
    cases += [(counter,state,replace) for counter in [0,1,32768,32769,65535]
              for state in range(256) for replace in [False,True]]
    work=Work();replacement=Work();task=Task(ctypes.pointer(work),0)
    events=[];original=[];errors=[]
    @Spawn
    def spawn(context,a,b,c):
        events.append(('spawn',a,b,c))
        if context!=0x1234:errors.append('context')
        if replace:
            work.state=255;task.work=ctypes.pointer(replacement)
    @StateCall
    def state_call(context,value):
        events.append(('state',value))
        if context!=0x1234:errors.append('context')
    @TaskCall
    def task_call(context,pointer):
        events.append(('task',))
        if context!=0x1234 or ctypes.addressof(pointer.contents)!=ctypes.addressof(task):errors.append('task')
    services=Services(0x1234,spawn,state_call,task_call)
    def original_spawn(memory,regs,sr):
        original.append(('spawn',regs[4],regs[5],regs[6]))
        if replace:
            memory[0x880100]=255;struct.pack_into('<I',memory,0x880020,0x8c881100)
        return sr
    def original_state(memory,regs,sr):
        value=regs[4] if regs[4]<0x80000000 else regs[4]-0x100000000
        original.append(('state',value));return sr
    def original_task(memory,regs,sr):
        assert regs[4]==0x8c880000
        original.append(('task',));return sr
    hooks={0x8c09852e:original_spawn,0x8c107ac6:original_state,0x8c09865c:original_task}
    for counter,initial,replace in cases:
        events.clear();original.clear()
        work.state=initial;work.reserved[:]=[11,22,33,44,55];work.cached_variant=counter
        replacement.state=99;replacement.cached_variant=0x1234
        task.work=ctypes.pointer(work)
        ram[0x880100:0x880108]=bytes(work)
        struct.pack_into('<I',ram,0x880020,0x8c880100)
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=0x8c880000;state.r[15]=0x8cf00000
        state.sr=0x700000f0;state.pr=0x8c000100;state.pc=0x0c900d76
        saved=list(state.r[8:]);reference(ram,state,hooks)
        native(ctypes.byref(task),ctypes.byref(services))
        assert bytes(work)==ram[0x880100:0x880108],(counter,initial,replace,'fields')
        assert events==original,(counter,initial,replace,'calls',events,original)
        assert list(state.r[8:])==saved and state.pr==0x8c000100
        assert bool(ctypes.addressof(task.work.contents)==ctypes.addressof(replacement)) == bool(replace and ((counter-1)&65535)>=32768)
    assert not errors,errors
    report={'cases_passed':len(cases),'counter_values_exhausted':65536,
            'signed_state_values_checked':256,'work_replacement_during_first_call_checked':True,
            'routine_start':'0x0c900d76','routine_end_exclusive':'0x0c900db2',
            'retail_span_sha256':hashlib.sha256(overlay[0xd76:0xdb2]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/stage00_timer.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'Projected native fields and external call sequence match a subset SH-4 instruction interpreter. External service bodies are hooks; no hardware or gameplay validation.'}
    (root/'analysis/stage00_timer_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{len(cases)} timer comparisons passed')
if __name__=='__main__':main()
