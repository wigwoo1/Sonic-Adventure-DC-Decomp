"""Check ordered helper calls and reread owning resource after callbacks."""
import ctypes,hashlib,json,struct
from verify_startup import ROOT,State,reference
class Owner(ctypes.Structure):
    _fields_=[('resource',ctypes.c_void_p)]
Call=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_void_p)
class Services(ctypes.Structure):
    _fields_=[('context',ctypes.c_void_p),('first',Call),('second',Call),('free',Call)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    owner=Owner();events=[];original=[];errors=[];callbacks=[];hooks={}
    for name,address in [('first',0x8c04b48c),('second',0x8c04b82e),('free',0x8c098b42)]:
        def callback(context,pointer,name=name):
            if context!=0x1234:errors.append('context')
            if name=='free':events.append((name,pointer or 0));owner.resource=0x76543210
            else:
                if pointer!=ctypes.addressof(owner):errors.append('owner argument')
                events.append((name,))
                if name=='first' and replace1 is not None:owner.resource=replace1
                if name=='second' and replace2 is not None:owner.resource=replace2
        callbacks.append(Call(callback))
        def hooked(memory,regs,sr,name=name):
            if name=='free':original.append((name,regs[4]));struct.pack_into('<I',memory,0x88003c,0x76543210)
            else:
                assert regs[4]==0x8c880000
                original.append((name,))
                if name=='first' and replace1 is not None:struct.pack_into('<I',memory,0x88003c,replace1)
                if name=='second' and replace2 is not None:struct.pack_into('<I',memory,0x88003c,replace2)
            return sr
        hooks[address]=hooked
    services=Services(0x1234,*callbacks)
    dll_path=ROOT/'work/sonic-adventure/build/secondary_resource_release.dll'
    function=ctypes.CDLL(str(dll_path)).sa_secondary_resource_release_8c04b468
    function.argtypes=[ctypes.POINTER(Owner),ctypes.POINTER(Services)];function.restype=None
    count=0
    for initial in [0,0x8c881000,0xffffffff]:
     for replace1 in [None,0,0x8c882000,0xffffffff]:
      for replace2 in [None,0,0x8c883000,0xffffffff]:
        owner.resource=initial;struct.pack_into('<I',ram,0x88003c,initial)
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=0x8c880000;state.r[15]=0x8cf00000;state.pc=0x8c04b468;state.pr=0x8c000100;state.sr=0x700000f0
        events.clear();original.clear();saved=list(state.r[8:]);reference(ram,state,hooks)
        function(ctypes.byref(owner),ctypes.byref(services))
        assert events==original,(initial,replace1,replace2,events,original)
        assert not owner.resource and struct.unpack_from('<I',ram,0x88003c)[0]==0
        assert list(state.r[8:])==saved and state.pr==0x8c000100
        count+=1
    assert not errors,errors
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':count,'routine_start':'0x8c04b468','routine_end_exclusive':'0x8c04b48c',
            'retail_span_sha256':hashlib.sha256(binary[0x3b468:0x3b48c]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/secondary_resource_release.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'Original integer instructions versus native projected owner field and external helper/free calls, including null and callback replacement of resource pointers. Helpers and heap free are hooks; hardware/gameplay unverified.'}
    (root/'analysis/secondary_resource_release_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{count} secondary resource release comparisons passed')
if __name__=='__main__':main()
