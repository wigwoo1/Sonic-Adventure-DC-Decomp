"""Original event removal, separately hooked and fully connected driver paths."""
import ctypes,hashlib,itertools,json,struct,sys
from verify_startup import ROOT,State,reference
from verify_event_registry import Install,Services
def main():
    connected='--connected' in sys.argv
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
    native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    sr=ctypes.c_uint32();mask_a=ctypes.c_uint32();mask_b=ctypes.c_uint32();events=[];original=[]
    def alter(memory,status):
        if mutate:
            status^=0x010000b1
            struct.pack_into('<I',memory,head_address-0x8c000000,0xdeadbeef)
        return status
    @Install
    def install(ctx,service,callback,arg,status):
        events.append((service,callback,arg,status[0]));status[0]=alter(native,status[0])
    def hook(service):
        def invoke(memory,regs,status):
            original.append((service,regs[4],regs[5],status));return alter(memory,status)
        return invoke
    hooks={} if connected else {target:hook(target) for target in [0x8c64135a,0x8c64137c]}
    services=Services(None,install);dll_path=ROOT/'work/sonic-adventure/build/event_registry.dll'
    fn=getattr(ctypes.CDLL(str(dll_path)),'sa_event_remove_connected' if connected else 'sa_event_remove_8c645c54')
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.POINTER(Services)];fn.restype=ctypes.c_uint32
    cases=0
    for handle,head,placement,initial_sr,mutate in itertools.product([1,2,255,0x10000001],[32,34,144,146,160,174,176,190,191,192],['absent','predecessor','inactive_predecessor','sole_head','head_successor','both'],[0,1,0x60,0xf0,0x700083f3],[False,True]):
        target=(0x8c892840+(handle<<4))&0xffffffff;head_address=0x8c893840+head*4
        predecessor=0x8c892840+(3 if (handle&255)!=3 else 4)*16
        successor=0x8c892880
        for memory in (old,native):
            memory[0x890000:0x894000]=bytes(0x4000)
            memory[0x9ff000:0xa01000]=b'\xa5'*0x2000
            struct.pack_into('<I',memory,0x67c004,0x8ca00100)
            struct.pack_into('<4I',memory,target-0x8c000000,0x8c051810,successor if placement=='head_successor' else 0,0xdeadabcd,0x80000058)
            if placement in ['predecessor','inactive_predecessor','both']:
                struct.pack_into('<4I',memory,predecessor-0x8c000000,0x8cb00000,target,0x1234,0x58 if placement=='inactive_predecessor' else 0x80000058)
            if placement in ['sole_head','head_successor','both']:
                struct.pack_into('<I',memory,head_address-0x8c000000,target)
        mmio={0xa05f6920:0xffffffff,0xa05f6924:0xffffffff}
        mask_a.value=mask_b.value=0xffffffff
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=handle;regs.r[15]=0x8cf00000;regs.pc=0x8c645c54;regs.pr=0x8c000100;regs.sr=initial_sr
        saved=list(regs.r[8:]);sr.value=initial_sr;events.clear();original.clear()
        reference(old,regs,hooks,mmio=mmio,vbr=0x8ca00000)
        result=fn(view,ctypes.byref(sr),0x8ca00000,ctypes.byref(mask_a),ctypes.byref(mask_b),handle,ctypes.byref(services))
        context=(handle,head,placement,initial_sr,mutate)
        assert result==regs.r[0] and sr.value==regs.sr,(context,hex(result),hex(regs.r[0]),hex(sr.value),hex(regs.sr))
        assert events==original,(context,events,original)
        assert mask_a.value==mmio[0xa05f6920] and mask_b.value==mmio[0xa05f6924],context
        assert old[0x890000:0x894000]==native[0x890000:0x894000],context
        assert old[0x9ff000:0xa01000]==native[0x9ff000:0xa01000],context
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        cases+=1
    out=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':cases,'routine_start':'0x8c645c54','routine_end_exclusive':'0x8c645dbe','retail_span_sha256':hashlib.sha256(binary[0x635c54:0x635dbe]).hexdigest(),'source_sha256':hashlib.sha256((out/'source/event_registry.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'connected':connected,'scope':'Active/inactive predecessors, head removal with and without successor, absent and out-of-search-range heads, wrapping handles, hardware mask words, vector reset, SR and callback mutation versus original instructions. Driver bodies '+('executed with no service hooks.' if connected else 'hooked.')+' Actual hardware interrupts and invalid-pointer exceptions unimplemented.'}
    name='event_remove_connected_verification.json' if connected else 'event_remove_verification.json'
    (out/'analysis'/name).write_text(json.dumps(report,indent=2)+'\n')
    print(f'{cases} event removal comparisons passed (connected={connected})')
if __name__=='__main__':main()
