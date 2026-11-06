"""Independent ADX recurrence with original binary32 rounding at decoder-step boundaries."""
import ctypes as C,struct
from sh4_binary32 import multiply,multiply_add,truncate_i32

def bits(value):return struct.unpack('<I',struct.pack('<f',value))[0]
def signed(value):return C.c_int32(value).value

class StereoADX:
    def __init__(self,coefficients):
        self.coefficients=tuple(bits(x/4096) for x in coefficients)
        self.history=[[0,0],[0,0]]
    def decode_step(self,payload):
        assert payload and len(payload)%36==0
        values=[[],[]];new=[bits(h[0]*65536) for h in self.history];old=[bits(h[1]*65536) for h in self.history]
        for index in range(len(payload)//18):
            ch=index%2;scale=struct.unpack_from('>H',payload,index*18)[0];assert not scale&0x8000
            gain=bits((scale&8191)+1)
            for byte in payload[index*18+2:index*18+18]:
                for code in (byte>>4,byte&15):
                    step=code if code<8 else code-16
                    pred=multiply_add(self.coefficients[0],new[ch],multiply(self.coefficients[1],old[ch],1)[0],1)[0]
                    value=multiply_add(gain,bits(step*65536),pred,1)[0]
                    values[ch].append(C.c_int16(signed(truncate_i32(value)[0])>>16).value)
                    old[ch],new[ch]=new[ch],value
        for ch in range(2):
            self.history[ch]=[C.c_int16(signed(truncate_i32(v)[0])>>16).value for v in (new[ch],old[ch])]
        return [struct.pack('<'+'h'*len(channel),*channel) for channel in values]
