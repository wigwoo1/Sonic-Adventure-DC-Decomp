"""Cleanup dependency of Tikal's entry; original service bodies hooked."""
import ctypes,hashlib,itertools,json,struct,sys
from verify_startup import ROOT,State,reference
from verify_driver_dispatch import Invoke
def main():
    module=(ROOT/'work/sonic-adventure/decoded/SONICADV/TIKAL_PROG.BIN').read_bytes()
    old=bytearray(16*1024*1024)
    main_binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    old[0x10000:0x10000+len(main_binary)]=main_binary
    old[0xb00000:0xb00000+len(module)]=module
    native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    sr=ctypes.c_uint32();events=[];original=[];errors=[]
    def offset(address):return (address&0x1fffffff)-0xc000000
    def effect(memory,service,arg,status):
        if mutate=='replace' and service==0x8c06c5ca:struct.pack_into('<I',memory,offset(task)+32,0x8ca03000)
        elif mutate=='owner' and service==0x8c06c5ca:memory[0xa02008]=owner^255
        if mutate!='none':status^=0x01000031
        return status,(service^arg)&0xffffffff
    @Invoke
    def invoke(ctx,service,arg,status):
        if ctx!=0x1234:errors.append('context')
        events.append((service,arg,status[0]));status[0],result=effect(native,service,arg,status[0]);return result
    def hook(service):
        def callback(memory,regs,status):
            original.append((service,regs[4],status));status,regs[0]=effect(memory,service,regs[4],status);return status
        return callback
    hooks={target:hook(target) for target in [0x8c06c5ca,0x8c0945d0,0x8c098ffc,0x8c6088bc]}
    connected='--connected' in sys.argv
    if connected:
        del hooks[0x8c0945d0];del hooks[0x8c098ffc]
    dll_path=ROOT/'work/sonic-adventure/build/tikal_cleanup.dll'
    fn=getattr(ctypes.CDLL(str(dll_path)),'sa_tikal_cleanup_with_owner_release' if connected else 'sa_tikal_cleanup_0cb00780')
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_void_p,Invoke];fn.restype=ctypes.c_uint32
    cases=0
    for owner,mutate,task,initial_sr in itertools.product(range(256),['none','replace','owner'],[0x8ca01000,0xaca01000],[0,0x700083f3]):
        for memory in (old,native):
            memory[0xa01000:0xa04000]=bytes(0x3000)
            struct.pack_into('<2I',memory,0xa01020,0x8ca02000,0x12345678)
            memory[0xa02008]=owner
            for i in range(38):struct.pack_into('<3I',memory,0x79e410+i*52,0x56780000+i,0x8ca02000 if i%3==0 else 0x8ca03000,0x87654321)
            struct.pack_into('<I',memory,0x1c2594,0x8ca04000)
            memory[0x1c24b8]=1
            struct.pack_into('<6I',memory,0xa04000,0x1234,0x8cb00000,0x1234,0x8cb00004,0,0)
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=task;regs.r[15]=0x8cf00000;regs.pc=0x0cb00780;regs.pr=0x8c000100;regs.sr=initial_sr
        saved=list(regs.r[8:]);sr.value=initial_sr;events.clear();original.clear()
        reference(old,regs,hooks)
        result=fn(view,ctypes.byref(sr),task,0x1234,invoke)
        assert events==original and result==regs.r[0] and sr.value==regs.sr,(owner,mutate,hex(task),initial_sr)
        assert old[0xa01000:0xa04000]==native[0xa01000:0xa04000]
        assert old[0x79e40c:0x79ebc4]==native[0x79e40c:0x79ebc4]
        assert old[0x1c24b4:0x1c25a4]==native[0x1c24b4:0x1c25a4]
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        cases+=1
    assert not errors,errors
    out=ROOT/'outputs/sonic-adventure'
    report={'module':'TIKAL_PROG.BIN','module_sha256':hashlib.sha256(module).hexdigest(),'load_address':'0x0cb00000','routine_start':'0x0cb00780','routine_end_exclusive':'0x0cb007a0','retail_span_sha256':hashlib.sha256(module[0x780:0x7a0]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/tikal_cleanup.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Every owner byte, cached work pointer despite callback replacement, service order, return and SR effects versus original module instructions. All three service bodies hooked. Tikal entry state machine and rendering remain unrecovered.'}
    if connected:report['scope']='Original Tikal cleanup, owner table release and reference table release executed. First service and inner callback service 8c6088bc remain hooked; compare callback order, cached work pointer after replacement, entire owner table, count bytes, task RAM, return and SR. Tikal entry state machine and rendering remain unrecovered.'
    name='tikal_cleanup_connected_verification.json' if connected else 'tikal_cleanup_verification.json'
    (out/'analysis'/name).write_text(json.dumps(report,indent=2)+'\n')
    print(f'{cases} Tikal cleanup comparisons passed')
if __name__=='__main__':main()
