"""Retail insertion -> indexed driver dispatch -> event traversal, callback hooks only."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_driver_dispatch import Invoke
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
    native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    sr=ctypes.c_uint32();mask_a=ctypes.c_uint32();mask_b=ctypes.c_uint32()
    events=[];original=[];handles=[];errors=[]
    def effect(memory,token,arg,status):
        index=(token-0x8cb00000)//4
        if index==0 and len(handles)>1:
            if mutation=='cut':struct.pack_into('<I',memory,0x892840+handles[0]*16+4,0)
            elif mutation=='skip':
                link=0x892840+handles[0]*16+4
                next_node=struct.unpack_from('<I',memory,link)[0]
                after_next=struct.unpack_from('<I',memory,next_node-0x8c000000+4)[0] if next_node else 0
                struct.pack_into('<I',memory,link,after_next)
            elif mutation=='argument':struct.pack_into('<I',memory,0x892840+handles[1]*16+8,0xdeadbeef)
            elif mutation=='head':struct.pack_into('<I',memory,0x893a80,0)
        return (status^0x01000031 if mutation!='none' else status),(arg^token)&0xffffffff
    @Invoke
    def invoke(ctx,token,arg,status):
        if ctx!=0x1234:errors.append('context')
        events.append((token,arg,status[0]));status[0],result=effect(native,token,arg,status[0]);return result
    def hook(token):
        def callback(memory,regs,status):
            original.append((token,regs[4],status));status,regs[0]=effect(memory,token,regs[4],status);return status
        return callback
    hooks={0x8cb00000+i*4:hook(0x8cb00000+i*4) for i in range(4)}
    dll_path=ROOT/'work/sonic-adventure/build/driver_event_runtime.dll';dll=ctypes.CDLL(str(dll_path))
    insert=dll.sa_event_register_connected
    insert.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),*([ctypes.c_uint32]*4),ctypes.c_void_p];insert.restype=ctypes.c_uint32
    dispatch=dll.sa_driver_event_dispatch_connected
    dispatch.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_void_p,Invoke];dispatch.restype=ctypes.c_uint32
    register_driver=dll.sa_vblank_callback_register_8c64135a
    register_driver.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32];register_driver.restype=ctypes.c_uint32
    cases=0;insertions=0
    for count,order,mutation,initial_sr in itertools.product(range(5),['ascending','descending','ties'],['none','cut','skip','argument','head'],[0,1,0x60,0xf0,0x700083f3]):
        for memory in (old,native):
            memory[0x890000:0x894000]=bytes(0x4000)
            memory[0x8a2d00:0x8a3100]=bytes(0x400)
            struct.pack_into('<I',memory,0x67c004,0x8c8a2d54)
        mmio={0xa05f6920:0,0xa05f6924:0};mask_a.value=mask_b.value=0;sr.value=initial_sr;handles.clear()
        if count==0:
            regs=State();regs.r[4]=0x8c645b1e;regs.r[5]=0;regs.r[15]=0x8cf00000;regs.pc=0x8c64135a;regs.pr=0x8c000100;regs.sr=sr.value
            reference(old,regs)
            assert register_driver(view,ctypes.byref(sr),0x8c645b1e,0)==regs.r[0]
            assert sr.value==regs.sr
        for i in range(count):
            priority=i if order=='ascending' else count-i if order=='descending' else 8
            regs=State();regs.r[4:8]=[0x900,0x8cb00000+i*4,priority,0x98760000+i];regs.r[15]=0x8cf00000;regs.pc=0x8c64594c;regs.pr=0x8c000100;regs.sr=sr.value
            reference(old,regs,mmio=mmio,vbr=0x8ca00000)
            handle=insert(view,ctypes.byref(sr),0x8ca00000,ctypes.byref(mask_a),ctypes.byref(mask_b),0x900,0x8cb00000+i*4,priority,0x98760000+i,None)
            assert handle==regs.r[0] and sr.value==regs.sr
            handles.append(handle);insertions+=1
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=1;regs.r[15]=0x8cf00000;regs.pc=0x8c6524d2;regs.pr=0x8c000100;regs.sr=sr.value
        saved=list(regs.r[8:]);events.clear();original.clear();reference(old,regs,hooks)
        result=dispatch(view,ctypes.byref(sr),1,0x1234,invoke)
        context=(count,order,mutation,initial_sr)
        assert events==original and result==regs.r[0] and sr.value==regs.sr,(context,events,original,hex(result),hex(regs.r[0]))
        assert old[0x890000:0x894000]==native[0x890000:0x894000],context
        assert old[0x8a2d00:0x8a3100]==native[0x8a2d00:0x8a3100],context
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        cases+=1
    assert not errors,errors
    out=ROOT/'outputs/sonic-adventure'
    report={'sequences_passed':cases,'original_insertions_compared':insertions,'source_sha256':hashlib.sha256((out/'source/driver_event_runtime.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Original insertion/driver installation bodies, indexed dispatch and VBlank event traversal execute in sequence. Only final event callback bodies hooked; compare priority ordering, mutation, captured arguments, returns, registry/driver memory and SR. Hardware interrupt entry and FPU wrapper not covered.'}
    (out/'analysis/driver_event_runtime_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{cases} installed event dispatch sequences and {insertions} insertions passed')
if __name__=='__main__':main()
