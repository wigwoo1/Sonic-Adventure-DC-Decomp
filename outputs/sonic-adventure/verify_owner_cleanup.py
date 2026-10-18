"""Owner cleanup with cached pointers, branch and callback mutation coverage."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
Call=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32))
Detach=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32))
class Services(ctypes.Structure):
    _fields_=[('context',ctypes.c_void_p),('free',Call),('helper',Call),('detach',Detach),('finish',Call)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
    native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    sr=ctypes.c_uint32();events=[];original=[];errors=[]
    def effect(memory,service,args,status):
        if mutation=='banks' and service==0x8c098b42:
            for base in [0x78c608,0x78c628,0x78c648,0x78c668]:struct.pack_into('<I',memory,base+owner*4,0x8ca04000)
        elif mutation=='fields':
            if service==0x8c079476:struct.pack_into('<I',memory,0xa02144,0x1234)
            if service==0x8c07d6c0:struct.pack_into('<I',memory,0xa03000,0 if flags&8 else 0x8ca05100)
        return status^(0x01000031 if mutation!='none' else 0),(service^args[0])&0xffffffff
    def call(service):
        @Call
        def native_call(ctx,arg,status):
            if ctx!=0x1234:errors.append('context')
            events.append((service,(arg,),status[0]));status[0],result=effect(native,service,(arg,),status[0]);return result
        return native_call
    free=call(0x8c098b42);helper=call(0x8c079476);finish=call(0x8c07d6c0)
    @Detach
    def detach(ctx,first,resource,entity,status):
        if ctx!=0x1234:errors.append('context')
        args=(first,resource,entity);events.append((0x8c07a55c,args,status[0]));status[0],result=effect(native,0x8c07a55c,args,status[0]);return result
    def hook(service):
        def callback(memory,regs,status):
            args=tuple(regs[4:7]) if service==0x8c07a55c else (regs[4],)
            original.append((service,args,status));status,regs[0]=effect(memory,service,args,status);return status
        return callback
    hooks={target:hook(target) for target in [0x8c098b42,0x8c079476,0x8c07a55c,0x8c07d6c0]}
    services=Services(0x1234,free,helper,detach,finish)
    dll_path=ROOT/'work/sonic-adventure/build/owner_cleanup.dll';fn=ctypes.CDLL(str(dll_path)).sa_owner_cleanup_8c06c5ca
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.POINTER(Services)];fn.restype=ctypes.c_uint32
    cases=0
    for owner,flags,mutation,initial_sr in itertools.product([0,1,7,8,31,255],range(16),['none','banks','fields'],[0,0x700083f3]):
        for memory in (old,native):
            memory[0x78c600:0x78cc00]=b'\xa5'*0x600;memory[0xa02000:0xa06000]=bytes(0x4000)
            for base,value in [(0x78c608,0x8ca01000),(0x78c628,0x8ca03000),(0x78c648,0x87654321),(0x78c668,0x8ca02000)]:struct.pack_into('<I',memory,base+owner*4,value)
            for offset,bit in [(0xa02064,1),(0xa02148,2),(0xa02144,4),(0xa03000,8)]:struct.pack_into('<I',memory,offset,0x8ca05000 if flags&bit else 0)
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=owner;regs.r[15]=0x8cf00000;regs.pc=0x8c06c5ca;regs.pr=0x8c000100;regs.sr=initial_sr
        saved=list(regs.r[8:]);sr.value=initial_sr;events.clear();original.clear();reference(old,regs,hooks)
        result=fn(view,ctypes.byref(sr),owner,ctypes.byref(services))
        context=(owner,flags,mutation,initial_sr)
        assert events==original and result==regs.r[0] and sr.value==regs.sr,(context,events,original,hex(result),hex(regs.r[0]))
        assert old[0x78c600:0x78cc00]==native[0x78c600:0x78cc00],context
        assert old[0xa02000:0xa06000]==native[0xa02000:0xa06000],context
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        cases+=1
    assert not errors,errors
    out=ROOT/'outputs/sonic-adventure'
    report={'routine_start':'0x8c06c5ca','routine_end_exclusive':'0x8c06c660','retail_span_sha256':hashlib.sha256(binary[0x5c5ca:0x5c660]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/owner_cleanup.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Four parallel owner tables, all branch combinations, cached pointers, later-field reload after callbacks, arguments, memory effects, return and SR versus original instructions. Heap free and three helper bodies hooked; null required pointer exception behavior unimplemented.'}
    (out/'analysis/owner_cleanup_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{cases} owner cleanup comparisons passed')
if __name__=='__main__':main()
