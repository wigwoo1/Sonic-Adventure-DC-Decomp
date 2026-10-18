"""Compare native PRS output and return value with original instruction execution."""
import ctypes, hashlib, json, pathlib, struct
from verify_startup import State, reference
from decompress_prs import decompress
ROOT=pathlib.Path(__file__).resolve().parents[2]
class Stream:
    def __init__(self): self.data=bytearray();self.used=8;self.control=0
    def bit(self,value):
        if self.used==8:self.control=len(self.data);self.data.append(0);self.used=0
        self.data[self.control]|=value<<self.used;self.used+=1
    def literal(self,value):self.bit(1);self.data.append(value)
    def short(self,distance,length):
        self.bit(0);self.bit(0);self.bit((length-2)>>1);self.bit((length-2)&1)
        self.data.append(256-distance)
    def long(self,distance,length,extended=False):
        self.bit(0);self.bit(1);count=0 if extended else length-2
        self.data.extend(struct.pack('<H',((8192-distance)<<3)|count))
        if extended:self.data.append(length-1)
    def finish(self):self.bit(0);self.bit(1);self.data.extend(b'\0\0');return bytes(self.data)
def synthetic():
    for shift in range(8):
        stream=Stream()
        for value in range(256+shift):stream.literal(value&255)
        stream.short(256,5);stream.short(1,2);stream.long(255,9);stream.long(1,256,True)
        yield f'bit_boundary_{shift}',stream.finish()
    stream=Stream()
    for value in range(8192):stream.literal(value&255)
    stream.long(8192,1,True);stream.long(8192,256,True)
    yield 'maximum_distance_and_extended_lengths',stream.finish()
    yield 'empty',Stream().finish()
def main():
    root=ROOT/'outputs/sonic-adventure'
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    dll_path=ROOT/'work/sonic-adventure/build/prs_decompress.dll';dll=ctypes.CDLL(str(dll_path))
    native=dll.sa_prs_decompress_8c04c5e0
    native.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint8)];native.restype=ctypes.c_uint32
    records=json.loads((root/'analysis/prs_inventory.json').read_text())
    selected=sorted((r for r in records if r['decoded_bytes']<=2048),key=lambda r:r['source'])[:24]
    selected.append(next(r for r in records if r['source']=='SONICADV/STG00.PRS'))
    cases=list(synthetic())+[(r['source'],(ROOT/'work/sonic-adventure/extracted/files'/r['source']).read_bytes()) for r in selected]
    results=[]
    for name,source in cases:
        decoded,_=decompress(source)
        ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
        ram[0xD00000:0xD00000+len(source)]=source
        state=State();state.r[:]=[0x12340000+i for i in range(16)]
        state.r[4]=0x8CD00000;state.r[5]=0x8C900000;state.r[15]=0x8CF00000
        state.sr=0x700000F0;state.pr=0x8C000100;state.pc=0x8C04C5E0
        steps=reference(ram,state)
        assert state.r[8]==0x12340008 and state.r[9]==0x12340009 and state.r[15]==0x8CF00000,'Callee-saved state differs'
        assert state.r[0]==len(decoded),name+': original return size differs'
        expected=bytes(ram[0x900000:0x900000+state.r[0]])
        assert expected==decoded,name+': reference output disagrees with format decoder'
        encoded=ctypes.create_string_buffer(source);output=ctypes.create_string_buffer(len(decoded)+64)
        ctypes.memset(output,0xA5,len(output))
        size=native(ctypes.cast(encoded,ctypes.POINTER(ctypes.c_uint8)),ctypes.cast(ctypes.byref(output,32),ctypes.POINTER(ctypes.c_uint8)))
        assert size==state.r[0] and output.raw[32:32+size]==expected,name+': native/original outputs differ'
        assert output.raw[:32]==bytes([0xA5])*32 and output.raw[-32:]==bytes([0xA5])*32,'Guard changed'
        results.append({'case':name,'original_instruction_steps':steps,'decoded_bytes':size,'output_and_return_match':True})
    report={'routine':'0x8c04c5e0','source_sha256':hashlib.sha256((root/'source/prs_decompress.c').read_bytes()).hexdigest(),
            'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'binary_sha256':hashlib.sha256(binary).hexdigest(),
            'cases':results,'scope':'Original opcodes executed by an independent subset interpreter. Output, return value and original callee-save restoration checked; not hardware or complete-game verification.'}
    (root/'analysis/prs_instruction_verification.json').write_text(json.dumps(report,indent=2))
    print(len(results),'original-instruction/native PRS comparisons passed')
if __name__=='__main__':main()
