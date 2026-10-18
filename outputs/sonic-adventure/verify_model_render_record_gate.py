import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_render_record_gate.dll')).sa_model_render_record_gate;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8)]+[ctypes.POINTER(ctypes.c_uint32)]*3;cases=0
 for sr0,index,match,cached,seed in itertools.product([0,0x700083f3],[0,7,65535],[False,True],[0,1,127,254,255],range(8)):
  rng=random.Random(seed);cfg=0x8ca00000;record=0x8ca00100;entry=0x8ca01000+index*16
  for m in (old,native):
   struct.pack_into('<H',m,0xa00004,index);struct.pack_into('<I',m,0xa00020,0x8ca01000);struct.pack_into('<I',m,entry&0xffffff,0x8ca00600);struct.pack_into('<I',m,0xa00600,0x8ca04000);struct.pack_into('<I',m,0xa00100,0x8ca04000 if match else 0x8ca05000);m[0xa0010e]=cached
  state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[8]=cfg;state.r[14]=record;state.sr=sr0;state.pc=0x8c07a800;state.pr=0x8c000100;r=(ctypes.c_uint32*16)(*state.r);dest=[]
  def hook(addr):
   def stop(m,regs,s):dest.append(addr);state.pr=0x8c000100;return s
   return stop
  reference(old,state,{a:hook(a) for a in [0x8c07ade6,0x8c07a826,0x8c07a832]})
  sr=ctypes.c_uint32(sr0);nextpc=ctypes.c_uint32();assert fn(view,ctypes.byref(sr),r,ctypes.byref(nextpc))==0
  assert list(r)==list(state.r) and sr.value==state.sr and nextpc.value==dest[0],(index,match,cached,seed)
  assert nextpc.value==(0x8c07ade6 if not match else 0x8c07a826 if cached==255 else 0x8c07a832)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_render_record_gate_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_render_record_gate.c').read_bytes()).hexdigest(),scope='Ownership/cache gate8c07a800..8c07a826: full integer/SR and all branch destinations plus independent disposition assertion. Unsigned16-bit index limits, root mismatch/match and cached ID boundary cases. Lookup and per-record matrix construction integration incomplete.'),indent=2)+'\n');print(f'{cases} rendering record gate comparisons passed')
if __name__=='__main__':main()
