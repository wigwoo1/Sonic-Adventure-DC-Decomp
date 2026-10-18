"""Check resource release ordering, work reloads and final task field clearing."""
import ctypes,hashlib,json,struct
from verify_startup import ROOT,State,reference
from verify_task_create import Task
Release=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_void_p)
class Services(ctypes.Structure):
    _fields_=[('context',ctypes.c_void_p),('first',Release),('second',Release),('free',Release)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    works=[(ctypes.c_uint32*16)() for _ in range(3)]
    native_addresses=[ctypes.addressof(w) for w in works]
    original_addresses=[0x8c881000+i*0x100 for i in range(3)]
    task=Task();events=[];original=[];errors=[]
    def callback(kind,pointer):
        events.append((kind,native_addresses.index(pointer)))
        if kind=='first' and mutation & 1:task.slots[8]=native_addresses[1]
        if kind=='second' and mutation & 2:task.slots[8]=native_addresses[2]
        if kind=='free':task.slots[8]=native_addresses[0]
    callbacks=[]
    for kind in ['first','second','free']:
        def wrapped(context,pointer,kind=kind):
            if context!=0x1234:errors.append('context')
            callback(kind,pointer)
        callbacks.append(Release(wrapped))
    services=Services(0x1234,*callbacks)
    hooks={}
    for address,kind in [(0x8c0344a0,'first'),(0x8c04b468,'second'),(0x8c098b42,'free')]:
        def hooked(memory,regs,sr,kind=kind):
            original.append((kind,original_addresses.index(regs[4])))
            if kind=='first' and mutation & 1:struct.pack_into('<I',memory,0x880020,original_addresses[1])
            if kind=='second' and mutation & 2:struct.pack_into('<I',memory,0x880020,original_addresses[2])
            if kind=='free':struct.pack_into('<I',memory,0x880020,original_addresses[0])
            return sr
        hooks[address]=hooked
    dll_path=ROOT/'work/sonic-adventure/build/task_work_release.dll'
    function=ctypes.CDLL(str(dll_path)).sa_task_work_release_8c098998
    function.argtypes=[ctypes.POINTER(Task),ctypes.POINTER(Services)];function.restype=None
    count=0
    for first in [0,1,0x80000000,0xffffffff]:
     for second in [0,1,0x80000000,0xffffffff]:
      for replacement_second in [0,1]:
       for mutation in range(4):
        for i,work in enumerate(works):work[:]=[0xabc00000+i*0x100+j for j in range(16)]
        works[0][14]=first;works[0][15]=second;works[1][15]=replacement_second
        for i,work in enumerate(works):ram[original_addresses[i]-0x8c000000:original_addresses[i]-0x8c000000+64]=bytes(work)
        task.slots[:]=[0x12340000+i for i in range(12)];task.slots[8]=native_addresses[0]
        initial=[0x12340000+i for i in range(12)];initial[8]=original_addresses[0]
        struct.pack_into('<12I',ram,0x880000,*initial);events.clear();original.clear()
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=0x8c880000;state.r[15]=0x8cf00000;state.pc=0x8c098998;state.pr=0x8c000100;state.sr=0x700000f0
        saved=list(state.r[8:]);reference(ram,state,hooks)
        function(ctypes.byref(task),ctypes.byref(services))
        assert events==original,(first,second,replacement_second,mutation,events,original)
        assert [p or 0 for p in task.slots]==list(struct.unpack_from('<12I',ram,0x880000))
        for i,work in enumerate(works):assert bytes(work)==ram[original_addresses[i]-0x8c000000:original_addresses[i]-0x8c000000+64]
        assert list(state.r[8:])==saved and state.pr==0x8c000100
        count+=1
    assert not errors,errors
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':count,'routine_start':'0x8c098998','routine_end_exclusive':'0x8c0989c8',
            'retail_span_sha256':hashlib.sha256(binary[0x88998:0x889c8]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/task_work_release.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'Original integer instructions versus native projected task/work fields and resource/free calls, including pointer replacement during callbacks. Three external bodies are hooks; hardware/gameplay unverified.'}
    (root/'analysis/task_work_release_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{count} task work release comparisons passed')
if __name__=='__main__':main()
