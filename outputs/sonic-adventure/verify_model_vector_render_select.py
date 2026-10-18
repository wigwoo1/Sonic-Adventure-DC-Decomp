import ctypes,hashlib,json,struct
from verify_startup import ROOT,State,reference

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_vector_render_select.dll')).sa_model_vector_render_select;fn.restype=ctypes.c_uint32;fn.argtypes=[ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)];targets=[0x8c07a986,0x8c07ab40,0x8c07ac0e,0x8c07acde,0x8c07ad00,0x8c07ad22,0x8c07ad80,0x8c07ada2,0x8c07adc4,0x8c07ade6];cases=0
 for kind in range(256):
  for sr0 in [0,0x700083f3]:
   ram[0xa0000d]=kind;state=State();state.r[14]=0x8ca00000;state.pc=0x8c07a93c;state.pr=0x8c000100;state.sr=sr0;hit=[]
   def stop(address):
    def invoke(memory,r,s):hit.append(address);state.pr=0x8c000100;return s
    return invoke
   reference(ram,state,{a:stop(a) for a in targets});sr=ctypes.c_uint32(sr0);r0=ctypes.c_uint32();pc=fn(kind,ctypes.byref(sr),ctypes.byref(r0))
   assert pc==hit[0] and sr.value==state.sr and r0.value==state.r[0],kind;cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_vector_render_select_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_vector_render_select.c').read_bytes()).hexdigest(),scope='All256 signed-byte record types and SR patterns; dispatch r0/SR/selected PC compared against original instructions. Selected rendering bodies stop-hooked and unrecovered.'),indent=2)+'\n');print(f'{cases} rendering branch comparisons passed')
if __name__=='__main__':main()
