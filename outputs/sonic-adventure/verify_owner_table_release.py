"""Compare owner-table release with retail instructions and entire table memory."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
    native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
    sr=ctypes.c_uint32();dll_path=ROOT/'work/sonic-adventure/build/owner_table_release.dll'
    fn=ctypes.CDLL(str(dll_path)).sa_owner_table_release_8c0945d0
    fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32];fn.restype=ctypes.c_uint32
    cases=0
    for owner,match,flags,initial_sr in itertools.product([0,1,0x8ca02000,0xffffffff],range(-2,38),[0,0x10,0xffffffff],[0,0x700083f3]):
        for memory in (old,native):
            memory[0x79e400:0x79ec00]=b'\xa5'*0x800
            for i in range(38):
                row=0x79e40c+i*52
                row_owner=owner if match==-2 or i==match else owner^0x12345678
                struct.pack_into('<3I',memory,row+4,0x56780000+i,row_owner,flags)
        regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=owner;regs.r[15]=0x8cf00000;regs.pc=0x8c0945d0;regs.pr=0x8c000100;regs.sr=initial_sr
        saved=list(regs.r[8:]);sr.value=initial_sr;reference(old,regs)
        assert fn(view,ctypes.byref(sr),owner)==regs.r[0]==0
        assert old[0x79e400:0x79ec00]==native[0x79e400:0x79ec00] and sr.value==regs.sr
        assert list(regs.r[8:])==saved and regs.pr==0x8c000100
        cases+=1
    out=ROOT/'outputs/sonic-adventure'
    report={'routine_start':'0x8c0945d0','routine_end_exclusive':'0x8c094600','retail_span_sha256':hashlib.sha256(binary[0x845d0:0x84600]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/owner_table_release.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Every match position, no matches, all matches, owner and flag boundaries; full table/neighbor bytes, return and SR versus original instructions, no hooks. Table semantic role beyond owner matching remains unconfirmed.'}
    (out/'analysis/owner_table_release_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{cases} owner table release comparisons passed')
if __name__=='__main__':main()
