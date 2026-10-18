import ctypes,itertools,json,hashlib,struct,random
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b;view=(ctypes.c_uint8*len(ram)).from_buffer(ram);P=ctypes.POINTER(ctypes.c_uint32);fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_hierarchy_channel_prepare.dll')).sa_model_hierarchy_channel_prepare;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,ctypes.c_uint];cases=0
 for bank,rm,sr0,mode,keys,phase,channel,total in itertools.product(range(2),range(2),[0,0x700083f3],[2,3],[(0,2,7,12,100),(0,1,2,3,100)],[-0.5,0.0,0.25,1.0,1.75,2.0,2.5,3.0,7.0,11.5,12.0],range(3),[4,13]):
  if phase>keys[-2]:continue
  rng=random.Random(cases);f=Fpu();f.fpscr=0x40000|(bank<<21)|rm;f.fpul=0x12345678
  for k in range(2):f.banks[k][:]=[rng.getrandbits(32) for _ in range(16)]
  f.banks[bank][14]=0x4f000000;nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[8]=0x8ca01000;state.r[7]=channel*4;state.r[14]=0x8ca00100;state.pc=0x8c057c80 if mode==2 else 0x8c057ed0;state.pr=0x8c000100;state.sr=sr0;r=(ctypes.c_uint32*16)(*state.r)
  for i,key in enumerate(keys):struct.pack_into('<I',ram,0xa02000+i*16,key)
  struct.pack_into('<I',ram,0xa01000+channel*4,0x8ca02000);struct.pack_into('<I',ram,0xa0010c,total);struct.pack_into('<f',ram,0xa00110,phase)
  def stop(m,regs,s):state.pr=0x8c000100;return s
  before=bytes(ram[0xa00100:0xa02100]);reference(ram,state,{0x8c057d46 if mode==2 else 0x8c057f6a:stop},f);sr=ctypes.c_uint32(sr0);fpul=ctypes.c_uint32(0x12345678);assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(fpul),mode)==0;nf.fpul=fpul.value
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f) and ram[0xa00100:0xa02100]==before,(mode,keys,phase,total);cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_hierarchy_channel_prepare_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_hierarchy_channel_prepare.c').read_bytes()).hexdigest(),scope='Both layout nonempty channel frame conversion/key selection/factor connected through value dispatch. Native scalar arithmetic no injected services. All integer/FPU/SR/FPUL and input RAM compared, fractional/key-boundary/terminal-wrap cases, banks/RM0/1. Value dispatch/full generator and exceptional arithmetic pending.'),indent=2)+'\n');print(cases,'native channel preparation comparisons passed')
if __name__=='__main__':main()
