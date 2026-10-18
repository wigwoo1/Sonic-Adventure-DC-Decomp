import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_render_frame_setup.dll')).sa_model_render_frame_setup;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)];cases=0
 for bank,sr0,root,alias,seed in itertools.product(range(2),[0,0x700083f3],[0,0x8ca04000],[False,True],range(16)):
  rng=random.Random(seed);frame=0x8cf00000;record=frame+68 if alias else 0x8ca00000
  for m in (old,native):m[0xf00000:0xf00100]=bytes([seed])*256;struct.pack_into('<I',m,record&0xffffff,root)
  f=Fpu();f.fpscr=(bank<<21)|(seed%2)|0x40000;f.fpul=rng.getrandbits(32)
  for j in range(2):f.banks[j][:]=[rng.getrandbits(32) for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[14]=record;state.r[15]=frame;state.sr=sr0;state.pc=0x8c07a786;state.pr=0x8c000100;r=(ctypes.c_uint32*16)(*state.r);dest=[]
  def hook(addr):
   def stop(m,regs,s):dest.append(addr);state.pr=0x8c000100;return s
   return stop
  reference(old,state,{a:hook(a) for a in [0x8c07a800,0x8c07aeda]},f)
  sr=ctypes.c_uint32(sr0);nextpc=ctypes.c_uint32();assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(nextpc))==0
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f) and native[0xf00000:0xf00100]==old[0xf00000:0xf00100] and nextpc.value==dest[0],(bank,root,alias,seed)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_render_frame_setup_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_render_frame_setup.c').read_bytes()).hexdigest(),scope='Rendering matrix frame setup8c07a786..8c07a800, full integer/FPU/SR/RAM and branch destination. Empty/nonempty lists, FR banks, record/frame alias. Prior hierarchy preparation and per-record matrix construction remain incomplete.'),indent=2)+'\n');print(f'{cases} rendering frame setup comparisons passed')
if __name__=='__main__':main()
