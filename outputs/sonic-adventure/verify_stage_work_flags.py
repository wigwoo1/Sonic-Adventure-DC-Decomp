"""Instruction comparison for stage initialization's work flag dependency."""
import ctypes,hashlib,json,struct
from verify_startup import ROOT,State,reference
class Work(ctypes.Structure):
    _fields_=[('field0',ctypes.c_uint8),('field1',ctypes.c_uint8),('reserved',ctypes.c_uint8*2),('flags',ctypes.c_uint16)]
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    records=(Work*256)()
    table=(ctypes.POINTER(Work)*256)(*[ctypes.pointer(records[i]) for i in range(256)])
    for i in range(256):struct.pack_into('<I',ram,0x78c608+i*4,0x8c880100+i*16)
    dll_path=ROOT/'work/sonic-adventure/build/stage_work_flags.dll'
    function=ctypes.CDLL(str(dll_path)).sa_stage_work_flags_8c07c65e
    function.argtypes=[ctypes.POINTER(ctypes.POINTER(Work)),ctypes.c_uint32,ctypes.c_uint32];function.restype=None
    cases=[(flag,0,24) for flag in range(65536)]
    cases += [(0x8001,index,value) for index in list(range(256))+[256,511,0xffffffff,0x80000000]
              for value in [0,127,128,255,256,0xffffffff]]
    for flags,index,value in cases:
        work=records[index & 255];offset=0x880100+(index & 255)*16
        work.field0=91;work.field1=92;work.reserved[:]=[93,94];work.flags=flags
        before=bytes(records)
        ram[offset:offset+6]=bytes(work)
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=index;state.r[5]=value;state.sr=0x700000f1
        state.pr=0x8c000100;state.pc=0x8c07c65e
        saved=list(state.r[8:]);reference(ram,state)
        function(table,index,value)
        expected=bytearray(before);start=(index & 255)*ctypes.sizeof(Work)
        expected[start:start+6]=ram[offset:offset+6]
        assert bytes(records)==expected,(flags,index,value)
        assert list(state.r[8:])==saved and state.sr==0x700000f1
    root=ROOT/'outputs/sonic-adventure'
    report={'cases_passed':len(cases),'flag_values_exhausted':65536,'index_low_bytes_exhausted':256,
            'routine_start':'0x8c07c65e','routine_end_exclusive':'0x8c07c676',
            'retail_span_sha256':hashlib.sha256(binary[0x6c65e:0x6c676]).hexdigest(),
            'source_sha256':hashlib.sha256((root/'source/stage_work_flags.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),
            'scope':'Native projected work fields versus original integer instructions using a subset interpreter; valid table pointers required. No hardware/gameplay validation.'}
    (root/'analysis/stage_work_flags_verification.json').write_text(json.dumps(report,indent=2))
    print(f'{len(cases)} work flag comparisons passed')
if __name__=='__main__':main()
