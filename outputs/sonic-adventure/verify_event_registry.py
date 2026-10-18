"""Differential event insertion, with explicit MMIO words and driver hooks."""
import ctypes,hashlib,itertools,json,struct,sys
from verify_startup import ROOT,State,reference
Install=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32))
class Services(ctypes.Structure):
    _fields_=[('context',ctypes.c_void_p),('install',Install)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
    native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    sr=ctypes.c_uint32();mask_a=ctypes.c_uint32();mask_b=ctypes.c_uint32();events=[];original=[]
    def alter(memory,status):
        if mutate:
            status^=0x01000031
            struct.pack_into('<I',memory,head_offset,0xdeadbeef)
        return status
    @Install
    def install(ctx,service,callback,arg,status):
        events.append((service,callback,arg,status[0]));status[0]=alter(native,status[0])
    def hook(service):
        def invoke(memory,regs,status):
            original.append((service,regs[4],regs[5],status));return alter(memory,status)
        return invoke
    hooks={target:hook(target) for target in [0x8c64135a,0x8c64137c]}
    services=Services(None,install);dll_path=ROOT/'work/sonic-adventure/build/event_registry.dll'
    connected='--connected' in sys.argv
    fn=getattr(ctypes.CDLL(str(dll_path)),'sa_event_register_connected' if connected else 'sa_event_register_8c64594c')
    if connected:hooks.clear()
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),*([ctypes.c_uint32]*4),ctypes.POINTER(Services)];fn.restype=ctypes.c_uint32
    cases=0
    for event,priority,chain,full,initial_sr,mutate in itertools.product([0x200,0x220,0x900,0x920,0x940,0xa00,0xae0,0xb00,0xbe0,0xffff,0x8000,0x12340900],[0,8,16,0x7fffffff],range(4),[False,True],[0,0x700083f3],[False,True]):
        signed_event=(event&65535)-(65536 if event&32768 else 0)
        head_offset=0x893840+(signed_event>>4)*4
        for memory in (old,native):
            memory[0x890000:0x894000]=bytes(0x4000)
            memory[0x9ff000:0xa01000]=b'\xa5'*0x2000
            struct.pack_into('<I',memory,0x67c004,0x8ca00100)
            # Reserve slot one; first free slot varies with chain length.
            struct.pack_into('<4I',memory,0x892850,0x8cb00100,0,9,0x80000001)
            for i in range(chain):
                address=0x8c892860+i*16
                successor=address+16 if i+1<chain else 0
                struct.pack_into('<4I',memory,address-0x8c000000,0x8cb00000+i*4,successor,100+i,0x80000000+(i+1)*8)
            if full:
                for i in range(1,256):struct.pack_into('<I',memory,0x892840+i*16+12,0x80000018)
            struct.pack_into('<I',memory,head_offset,0x8c892860 if chain else 0)
        mmio={0xa05f6920:0x12345678,0xa05f6924:0x87654321}
        mask_a.value=mmio[0xa05f6920];mask_b.value=mmio[0xa05f6924]
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4:8]=[event,0x8c051810,priority,0xffffffff]
        regs.r[15]=0x8cf00000;regs.pc=0x8c64594c;regs.pr=0x8c000100;regs.sr=initial_sr
        saved=list(regs.r[8:]);sr.value=initial_sr;events.clear();original.clear()
        reference(old,regs,hooks,mmio=mmio,vbr=0x8ca00000)
        result=fn(view,ctypes.byref(sr),0x8ca00000,ctypes.byref(mask_a),ctypes.byref(mask_b),event,0x8c051810,priority,0xffffffff,ctypes.byref(services))
        context=(hex(event),priority,chain,full,hex(initial_sr),mutate)
        assert result==regs.r[0],(context,result,regs.r[0])
        assert events==original,(context,events,original)
        assert sr.value==regs.sr,(context,hex(sr.value),hex(regs.sr))
        assert mask_a.value==mmio[0xa05f6920] and mask_b.value==mmio[0xa05f6924],context
        assert old[0x890000:0x894000]==native[0x890000:0x894000],context
        assert old[0x9ff000:0xa01000]==native[0x9ff000:0xa01000],context
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        cases+=1
    out=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':cases,'routine_start':'0x8c64594c','routine_end_exclusive':'0x8c645afa','retail_span_sha256':hashlib.sha256(binary[0x63594c:0x635afa]).hexdigest(),'source_sha256':hashlib.sha256((out/'source/event_registry.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Empty/front/middle/tail insertion, priority ties, exhaustion, signed event truncation, vector writes, hardware mask words, SR restoration and driver callback mutations versus original instructions. Driver registration bodies hooked; MMIO is a word model, not actual hardware.'}
    if connected:
        report['scope']='Event insertion connected to both original special driver wrappers, enable stubs and installer. No service hooks on either 0x900 or 0x920 path. Compare registry and driver slot memory, vectors, MMIO word model and SR. Hardware interrupt delivery unimplemented.'
        report['adapter_source_sha256']=hashlib.sha256((out/'source/event_registry_runtime.c').read_bytes()).hexdigest()
    name='event_registry_connected_verification.json' if connected else 'event_registry_verification.json'
    (out/'analysis'/name).write_text(json.dumps(report,indent=2)+'\n')
    print(f'{cases} event registry insertion comparisons passed')
if __name__=='__main__':main()
