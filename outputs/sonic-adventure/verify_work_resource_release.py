"""Check resource ownership flag, retained pointers and child/resource free order."""
import ctypes,hashlib,json,struct
from verify_startup import ROOT,State,reference
class Resource(ctypes.Structure):
    _fields_=[('flags',ctypes.c_uint16),('child',ctypes.c_void_p)]
class Owner(ctypes.Structure):
    _fields_=[('resource',ctypes.POINTER(Resource))]
Free=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_void_p)
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    resource=Resource();replacement=Resource(99,None);owner=Owner();events=[];original=[];errors=[]
    @Free
    def free(context,pointer):
        if context!=0x1234:errors.append('context')
        events.append('resource' if pointer==ctypes.addressof(resource) else 'child')
        if mutate:
            resource.child=0x12345678;owner.resource=ctypes.pointer(replacement)
    def hook(memory,regs,sr):
        original.append('resource' if regs[4]==0x8c881000 else 'child')
        if mutate:
            struct.pack_into('<I',memory,0x88100c,0x12345678)
            struct.pack_into('<I',memory,0x880038,0x8c882000)
        return sr
    dll_path=ROOT/'work/sonic-adventure/build/work_resource_release.dll'
    function=ctypes.CDLL(str(dll_path)).sa_work_resource_release_8c0344a0
    function.argtypes=[ctypes.POINTER(Owner),ctypes.c_void_p,Free];function.restype=None
    cases=[(True,flag,True,False) for flag in range(65536)]
    cases += [(present,flag,child,mutate) for present in [False,True] for flag in [0,32,0xffff]
              for child in [False,True] for mutate in [False,True]]
    for present,flags,child,mutate in cases:
        resource.flags=flags;resource.child=0x8c883000 if child else None
        owner.resource=ctypes.pointer(resource) if present else ctypes.POINTER(Resource)()
        struct.pack_into('<I',ram,0x880038,0x8c881000 if present else 0)
        struct.pack_into('<H',ram,0x881002,flags);struct.pack_into('<I',ram,0x88100c,0x8c883000 if child else 0)
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=0x8c880000;state.r[15]=0x8cf00000;state.pc=0x8c0344a0;state.pr=0x8c000100;state.sr=0x700000f0
        events.clear();original.clear();saved=list(state.r[8:]);reference(ram,state,{0x8c098b42:hook})
        function(ctypes.byref(owner),0x1234,free)
        assert events==original,(present,flags,child,mutate,events,original)
        assert not owner.resource and struct.unpack_from('<I',ram,0x880038)[0]==0
        assert (resource.child or 0)==struct.unpack_from('<I',ram,0x88100c)[0]
        assert resource.flags==struct.unpack_from('<H',ram,0x881002)[0]
        assert list(state.r[8:])==saved and state.pr==0x8c000100
    assert not errors,errors
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':len(cases),'flag_values_exhausted':65536,
            'routine_start':'0x8c0344a0','routine_end_exclusive':'0x8c0344dc',
            'retail_span_sha256':hashlib.sha256(binary[0x244a0:0x244dc]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/work_resource_release.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'Projected native resource fields/free order versus original integer instructions with a hooked heap free. Includes flags, null fields and callbacks replacing child and owner pointers. Actual freed-memory writes, hardware and gameplay unverified.'}
    (root/'analysis/work_resource_release_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{len(cases)} resource release comparisons passed')
if __name__=='__main__':main()
