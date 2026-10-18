"""Connected countdown replacement through original registry and driver bodies."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
    native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    sr=ctypes.c_uint32();mask_a=ctypes.c_uint32();mask_b=ctypes.c_uint32()
    dll_path=ROOT/'work/sonic-adventure/build/countdown_registry_runtime.dll'
    fn=ctypes.CDLL(str(dll_path)).sa_countdown_register_in_registry
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32];fn.restype=None
    cases=0;calls=0
    sequences=[(0,), (0x8c051810,), (0x8c051810,0), (0x8c051810,0x8cb00000,0), (0,0x8c051810,0xffffffff,0x8c051810)]
    for chain_length,priority,full,initial_sr,sequence in itertools.product(range(4),[0,0x58,0x59],[False,True],[0,1,0x60,0xf0,0x700083f3],sequences):
        for memory in (old,native):
            memory[0x890000:0x894000]=bytes(0x4000);memory[0xa00000:0xa01000]=b'\xa5'*4096
            struct.pack_into('<I',memory,0x67c004,0x8ca00100)
            for i in range(chain_length):
                node=0x8c892850+i*16;successor=node+16 if i+1<chain_length else 0
                struct.pack_into('<4I',memory,node-0x8c000000,0x8cb00000+i*4,successor,100+i,0x80000000|priority)
            struct.pack_into('<I',memory,0x893a80,0x8c892850 if chain_length else 0)
            if full:
                for i in range(1,256):struct.pack_into('<I',memory,0x892840+i*16+12,0x80000000|priority)
        mmio={0xa05f6920:0x12345678,0xa05f6924:0x87654321}
        mask_a.value=mmio[0xa05f6920];mask_b.value=mmio[0xa05f6924];sr.value=initial_sr
        for callback in sequence:
            regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=callback;regs.r[15]=0x8cf00000;regs.pc=0x8c605234;regs.pr=0x8c000100;regs.sr=sr.value
            saved=list(regs.r[8:]);reference(old,regs,mmio=mmio,vbr=0x8ca00000)
            fn(view,ctypes.byref(sr),0x8ca00000,ctypes.byref(mask_a),ctypes.byref(mask_b),callback)
            context=(chain_length,priority,full,initial_sr,sequence,callback)
            assert sr.value==regs.sr,(context,hex(sr.value),hex(regs.sr))
            assert old[0x890000:0x894000]==native[0x890000:0x894000],context
            assert old[0xa00000:0xa01000]==native[0xa00000:0xa01000],context
            assert mask_a.value==mmio[0xa05f6920] and mask_b.value==mmio[0xa05f6924],context
            assert list(regs.r[8:])==saved and regs.pr==0x8c000100
            calls+=1
        cases+=1
    out=ROOT/'outputs/sonic-adventure'
    report={'sequences_passed':cases,'original_calls_compared':calls,'source_sha256':hashlib.sha256((out/'source/countdown_registry_runtime.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Original countdown registration, event insert/remove and VBlank driver service bodies execute without service hooks. Compare replacement/clearing sequences amid competing callbacks and exhausted allocation; registry, driver slots, mask words and SR. Delivery of hardware interrupts and game callback execution not covered.'}
    (out/'analysis/countdown_registry_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{cases} sequences and {calls} original registration calls passed')
if __name__=='__main__':main()
