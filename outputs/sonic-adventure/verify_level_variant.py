"""Check the packed-field getter against original opcodes over both 16-bit axes."""
import ctypes,hashlib,json,pathlib,struct
from verify_startup import State,reference,signed
ROOT=pathlib.Path(__file__).resolve().parents[2]
class Fields(ctypes.Structure):_fields_=[('first',ctypes.c_int16),('second',ctypes.c_int16)]
def main():
    root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
    path=ROOT/'work/sonic-adventure/build/level_variant.dll';dll=ctypes.CDLL(str(path));native=dll.sa_level_variant_8c04f7e0
    native.argtypes=[ctypes.POINTER(Fields)];native.restype=ctypes.c_uint32
    boundaries=[0,1,127,128,255,256,32767,32768,65534,65535]
    def cases():
        for first in boundaries:
            for second in boundaries:yield first,second
        for second in range(65536):yield 37,second
        for first in range(65536):yield first,7
    count=0;steps=0;fields=Fields()
    for first,second in cases():
        struct.pack_into('<HH',ram,0x7493BA,first,second)
        state=State();state.r[:]=[0x12340000+i for i in range(16)];state.r[15]=0x8CF00000
        state.pr=0x8C000100;state.pc=0x8C04F7E0;state.sr=0x700000F1
        steps+=reference(ram,state)
        fields.first=signed(first,16);fields.second=signed(second,16);before=bytes(fields)
        actual=native(ctypes.byref(fields))
        assert actual==state.r[0],(first,second,hex(actual),hex(state.r[0]))
        assert bytes(fields)==before and state.sr==0x700000F1
        assert list(state.r[8:15])==[0x12340000+i for i in range(8,15)] and state.r[15]==0x8CF00000
        count+=1
    report={'routine':'0x8c04f7e0','cases':count,'original_instruction_steps':steps,
            'axes_exhausted':['all 65536 second-field bit patterns with first=37','all 65536 first-field bit patterns with second=7'],
            'boundary_cross_product':100,'output_and_input_preservation_match':True,
            'source_sha256':hashlib.sha256((root/'source/level_variant.c').read_bytes()).hexdigest(),
            'binary_sha256':hashlib.sha256(binary).hexdigest(),'dll_sha256':hashlib.sha256(path.read_bytes()).hexdigest(),
            'scope':'Original getter opcodes versus native value function. Global-field writers and complete gameplay remain unresolved.'}
    (root/'analysis/level_variant_verification.json').write_text(json.dumps(report,indent=2));print(count,'native/original packed-field comparisons passed')
if __name__=='__main__':main()
