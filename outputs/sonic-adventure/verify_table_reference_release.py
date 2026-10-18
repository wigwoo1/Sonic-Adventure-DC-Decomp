"""Signed reference-count bytes and callback table traversal versus retail."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
Release=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32))
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
    native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    sr=ctypes.c_uint32();events=[];original=[];errors=[]
    def effect(memory,arg,status):
        if mutation=='zero':memory[0x1c24b4+index]=0
        elif mutation=='negative':memory[0x1c24b4+index]=255
        elif mutation=='cut' and arg==0x8cb00000:struct.pack_into('<I',memory,0xa0200c,0)
        return status^0x01000031,(arg^0x87654321)&0xffffffff
    @Release
    def release(ctx,arg,status):
        if ctx!=0x1234:errors.append('context')
        events.append((arg,status[0]));status[0],result=effect(native,arg,status[0]);return result
    def hook(memory,regs,status):
        original.append((regs[4],status));status,regs[0]=effect(memory,regs[4],status);return status
    dll_path=ROOT/'work/sonic-adventure/build/table_reference_release.dll'
    fn=ctypes.CDLL(str(dll_path)).sa_table_reference_release_8c098ffc
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_void_p,Release];fn.restype=ctypes.c_uint32
    cases=0
    for count,index,length,mutation,initial_sr in itertools.product(range(256),[0,4,7],[-1,0,1,3],['none','zero','negative','cut'],[0,0x700083f3]):
        for memory in (old,native):
            memory[0x1c24b4:0x1c24bc]=bytes(8);memory[0x1c2584:0x1c25a4]=bytes(32);memory[0xa02000:0xa02100]=bytes(256)
            memory[0x1c24b4+index]=count
            struct.pack_into('<I',memory,0x1c2584+index*4,(0xaca02000 if index==7 else 0x8ca02000) if length>=0 else 0)
            for i in range(max(length,0)):struct.pack_into('<2I',memory,0xa02000+i*8,0x1234,0x8cb00000+i*4)
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=index;regs.r[15]=0x8cf00000;regs.pc=0x8c098ffc;regs.pr=0x8c000100;regs.sr=initial_sr
        saved=list(regs.r[8:]);sr.value=initial_sr;events.clear();original.clear()
        reference(old,regs,{0x8c6088bc:hook})
        result=fn(view,ctypes.byref(sr),index,0x1234,release)
        assert events==original and result==regs.r[0] and sr.value==regs.sr,(count,index,length,mutation,initial_sr,events,original,hex(result),hex(regs.r[0]))
        assert old[0x1c24b4:0x1c25a4]==native[0x1c24b4:0x1c25a4]
        assert old[0xa02000:0xa02100]==native[0xa02000:0xa02100]
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        cases+=1
    assert not errors,errors
    out=ROOT/'outputs/sonic-adventure'
    report={'routine_start':'0x8c098ffc','routine_end_exclusive':'0x8c099048','retail_span_sha256':hashlib.sha256(binary[0x88ffc:0x89048]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/table_reference_release.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Every count byte, null/empty/multi-entry tables, RAM aliases, live counter and table mutations, callback order, return and SR versus original instructions. Callback service 8c6088bc hooked; complete table semantics not yet established.'}
    (out/'analysis/table_reference_release_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{cases} reference table release comparisons passed')
if __name__=='__main__':main()
