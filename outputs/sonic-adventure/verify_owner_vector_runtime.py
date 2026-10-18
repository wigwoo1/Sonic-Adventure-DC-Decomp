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
            if service==0x8c079476:struct.pack_into('<I',memory,0xa02144,0x8ca04000)
            pass
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
    hooks={0x8c079476:hook(0x8c079476)}
    services=Services(0x1234,free,helper,detach,finish)
    dll_path=ROOT/'work/sonic-adventure/build/owner_vector_runtime.dll';fn=ctypes.CDLL(str(dll_path)).sa_owner_cleanup_with_vector
    from verify_fpu_cleanup_transfer import Fpu
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32,ctypes.c_void_p,Call,ctypes.POINTER(ctypes.c_int)];fn.restype=ctypes.c_uint32
    cases=0
    for owner,flags,mutation,initial_sr in itertools.product([0,1,7,8,31,255],range(32),['none','fields'],[0,0x700083f3]):
        for memory in (old,native):
            memory[0x78c600:0x78cc00]=b'\xa5'*0x600;memory[0xa02000:0xa06000]=bytes(0x4000)
            for base,value in [(0x78c608,0x8ca01000),(0x78c628,0x8ca03000),(0x78c648,0x87654321),(0x78c668,0x8ca02000)]:struct.pack_into('<I',memory,base+owner*4,value)
            for offset,bit,value in [(0xa02064,1,0x8c880020),(0xa02148,2,0x1234),(0xa02144,4,0x8ca04000),(0xa03000,8,0x8c8800a0),(0xa02078,16,0x8c880060)]:struct.pack_into('<I',memory,offset,value if flags&bit else 0)
            memory[0x880000:0x880200]=bytes(512)
            positions=[16+i*64 for i in range(6)]+[400]
            for i,block in enumerate(positions):
                struct.pack_into('<4I',memory,0x880000+block,0,0,0x8c880000+positions[i+1] if i<6 else 0,0x8c880000+positions[i-1] if i else 0)
            struct.pack_into('<I',memory,0x79f03c,0)
            if flags&4:
                struct.pack_into('<3I',memory,0xa04000,0x8ca05000,0x8ca04100,0x8ca04200);struct.pack_into('<I',memory,0xa04010,0x8c880120);struct.pack_into('<I',memory,0xa04104,0x8ca04300);struct.pack_into('<I',memory,0xa04204,0x8ca04300)
        f=Fpu();f.fpul=0x12345678;nf=Fpu.from_buffer_copy(f);fpul=ctypes.c_uint32(nf.fpul);status=ctypes.c_int()
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=owner;regs.r[15]=0x8cf00000;regs.pc=0x8c06c5ca;regs.pr=0x8c000100;regs.sr=initial_sr
        saved=list(regs.r[8:]);sr.value=initial_sr;events.clear();original.clear();reference(old,regs,hooks,f)
        result=fn(view,ctypes.byref(nf),ctypes.byref(fpul),ctypes.byref(sr),owner,0x8cef0000,0x1234,helper,ctypes.byref(status));nf.fpul=fpul.value
        assert status.value==0 and bytes(nf)==bytes(f)
        context=(owner,flags,mutation,initial_sr)
        assert events==original and result==regs.r[0] and sr.value==regs.sr,(context,events,original,hex(result),hex(regs.r[0]))
        assert old[0x78c600:0x78cc00]==native[0x78c600:0x78cc00],context
        assert old[0xa02000:0xa06000]==native[0xa02000:0xa06000],context
        assert old[0x880000:0x880200]==native[0x880000:0x880200] and old[0x79f03c:0x79f040]==native[0x79f03c:0x79f040],context
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        cases+=1
    assert not errors,errors
    out=ROOT/'outputs/sonic-adventure'
    report={'routine_start':'0x8c06c5ca','routine_end_exclusive':'0x8c06c660','retail_span_sha256':hashlib.sha256(binary[0x5c5ca:0x5c660]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/owner_vector_runtime.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Four parallel owner tables, all branch combinations, cached pointers, later-field reload after callbacks, arguments, memory effects, return and SR versus original instructions. Owner cleanup, entity allocation release and heap free execute without hooks; separate owner helper remains hooked; detach uses native model-vector cleanup; null required pointer exception behavior unimplemented.'}
    (out/'analysis/owner_vector_runtime_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{cases} owner cleanup with native vector comparisons passed')
if __name__=='__main__':main()

