import ctypes,itertools,json,hashlib,struct,random
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b;view=(ctypes.c_uint8*len(ram)).from_buffer(ram);P=ctypes.POINTER(ctypes.c_uint32);fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_hierarchy_channel_nonempty.dll')).sa_model_hierarchy_channel_nonempty;CB=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu),ctypes.c_uint32);fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,ctypes.c_uint,ctypes.c_void_p,CB];cases=0
 for bank,rm,sr0,mode,keys,phase,channel,total in itertools.product(range(2),range(2),[0,0x700083f3],[2,3],[(0,2,7,12,100),(0,1,2,3,100)],[-0.5,0.0,0.25,1.0,1.75,2.0,2.5,3.0,7.0,11.5,12.0],range(3),[4,13]):
  if phase>keys[-2]:continue
  rng=random.Random(cases);f=Fpu();f.fpscr=0x40000|(bank<<21)|rm;f.fpul=0x12345678
  for k in range(2):f.banks[k][:]=[struct.unpack("<I",struct.pack("<f",rng.randrange(-16,17)/4))[0] for _ in range(16)]
  f.banks[bank][14]=0x4f000000;nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[8]=0x8ca01000;state.r[7]=channel*4;state.r[14]=0x8ca00100;state.r[9]=channel;state.r[12]=0x8c055d4e;state.r[11]=0x8c639cf4;state.r[15]=0x8cefff80;state.pc=0x8c057c80 if mode==2 else 0x8c057ed0;state.pr=0x8c000100;state.sr=sr0;r=(ctypes.c_uint32*16)(*state.r)
  for i,key in enumerate(keys):
   struct.pack_into('<I',ram,0xa02000+i*16,key)
   if channel==1:struct.pack_into('<3I',ram,0xa02004+i*16,(i*30000)&0xffff,(i*20000)&0xffff,(i*40000)&0xffff)
   else:struct.pack_into('<3f',ram,0xa02004+i*16,i/4,-i/2,i+1)
  struct.pack_into('<I',ram,0xa01000+channel*4,0x8ca02000);struct.pack_into('<I',ram,0xa0010c,total);struct.pack_into('<f',ram,0xa00110,phase)
  def stop(m,regs,s):state.pr=0x8c000100;return s
  ram[0xefff80:0xefff98]=bytes([0x55])*24;native=bytearray(ram);nv=(ctypes.c_uint8*len(native)).from_buffer(native);ev=[];av=[]
  def apply(word,fp,fpul,log):
   log.append((word,fpul,bytes(fp)));active=(fp.fpscr>>21)&1
   if word==0xf0fd:fp.banks[active][0:2]=[0x3f000001,0x3f800002]
   else:
    start=(word>>8)&12;fp.banks[active][start:start+4]=[0x3f800000+len(log)*4+i for i in range(4)]
  def vector(word,fp):apply(word,fp,fp.fpul,ev)
  @CB
  def instruction(ctx,word,fp,fpul):fp.contents.fpul=fpul;apply(word,fp.contents,fpul,av);return 0
  reference(ram,state,{0x8c057dfe if mode==2 else 0x8c05808a:stop},f,vector_instruction=vector);sr=ctypes.c_uint32(sr0);fpul=ctypes.c_uint32(0x12345678);assert fn(nv,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(fpul),mode,None,instruction)==0;nf.fpul=fpul.value
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f) and ram[0xa00100:0xa02100]==native[0xa00100:0xa02100] and ram[0xefff80:0xefff98]==native[0xefff80:0xefff98] and ev==av,(mode,keys,phase,total);cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_hierarchy_channel_nonempty_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_hierarchy_channel_nonempty.c').read_bytes()).hexdigest(),scope='Both layout complete nonempty channel bodies through increment boundary connected to native conversion/key selection/factor/position/angle/scale/transforms. No original function hooks; FSCA/FTRV numerical outputs injected. Full integer/FPU/SR/FPUL/scratch/input RAM and instruction input snapshots compared. Fractional/key-boundary/terminal-wrap cases, banks/RM0/1; complete generator/numerical fidelity pending.'),indent=2)+'\n');print(cases,'native channel preparation comparisons passed')
if __name__=='__main__':main()
