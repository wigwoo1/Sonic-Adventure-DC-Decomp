import ctypes,itertools,json,hashlib,struct,random
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;P=ctypes.POINTER(ctypes.c_uint32);CB=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu),ctypes.c_uint32);fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_hierarchy_generator.dll')).sa_model_hierarchy_generator;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,ctypes.c_uint32,ctypes.c_void_p,CB];cases=0
 for bank,rm,sr0,mode,mask,limit,phase in itertools.product(range(2),range(2),[0,0x700083f3],[2,3],[0,7],[0,1,2,4,8],[0.0,1.5,3.0]):
  rng=random.Random(cases);f=Fpu();f.fpscr=0x40000|(bank<<21)|rm;f.fpul=0xabcdef12
  for k in range(2):f.banks[k][:]=[struct.unpack('<I',struct.pack('<f',rng.randrange(-16,17)/4))[0] for _ in range(16)]
  nf=Fpu.from_buffer_copy(f);state=State();state.r[:]=[rng.getrandbits(32) for _ in range(16)];state.r[4]=0x8ca04000;state.r[5]=0x8ca00100;state.r[15]=0x8cf00000;state.pc=0x8c057bc0;state.pr=0x8c000100;state.sr=sr0;r=(ctypes.c_uint32*16)(*state.r)
  old[0xa00100:0xa10000]=bytes([0])*0xff00;old[0xeffc00:0xf00000]=bytes([0x55])*0x400
  # root->childA->grandchild, childA sibling childB; root sibling returned only.
  nodes=[0x8ca04000+i*0x100 for i in range(4)]
  for i,node in enumerate(nodes):
   at=node&0xffffff;struct.pack_into('<I',old,at,i&7);struct.pack_into('<3f',old,at+8,1,2,3);struct.pack_into('<3I',old,at+20,0,0xffff,0x8000);struct.pack_into('<3f',old,at+32,0.5,1,2)
  struct.pack_into('<I',old,0xa0402c,nodes[1]);struct.pack_into('<I',old,0xa0412c,nodes[3]);struct.pack_into('<I',old,0xa04130,nodes[2]);struct.pack_into('<I',old,0xa04030,0x8ca05000)
  stride=16 if mode==2 else 24
  for i in range(4):
   for c in range(mode):
    ptr=0x8ca08000+c*0x100;struct.pack_into('<I',old,0xa01000+i*stride+c*4,ptr if mask&(1<<c) else 0)
  for c in range(3):
   for j,key in enumerate([0,1,3,100]):
    at=0xa08000+c*0x100+j*16;struct.pack_into('<I',old,at,key)
    if c==1:struct.pack_into('<3I',old,at+4,j*20000,j*30000,j*40000)
    else:struct.pack_into('<3f',old,at+4,j/4,-j/2,j+1)
  struct.pack_into('<4I',old,0xa00100,0x8ca01000,0x8ca02000,3 if mode==2 else 7,4);struct.pack_into('<f',old,0xa00110,phase);struct.pack_into('<2I',old,0x78b448,0,limit);struct.pack_into('<2I',old,0x88f698,16,1);struct.pack_into('<I',old,0x88f5f8,0x8ca03000)
  native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native);ev=[];av=[]
  def apply(word,fp,fpul,log):
   log.append((word,fpul,bytes(fp)));active=(fp.fpscr>>21)&1
   if word==0xf0fd:fp.banks[active][0:2]=[0x3f000001,0x3f800002]
   else:
    start=(word>>8)&12;fp.banks[active][start:start+4]=[0x3f800000+len(log)*4+i for i in range(4)]
  def vector(word,fp):apply(word,fp,fp.fpul,ev)
  @CB
  def instruction(ctx,word,fp,fpul):fp.contents.fpul=fpul;apply(word,fp.contents,fpul,av);return 0
  reference(old,state,{},f,vector_instruction=vector);sr=ctypes.c_uint32(sr0);fpul=ctypes.c_uint32(0xabcdef12);assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(fpul),0x8c000100,None,instruction)==0;nf.fpul=fpul.value
  assert list(r)==list(state.r) and sr.value==state.sr and bytes(nf)==bytes(f) and ev==av,(mode,mask,limit,phase)
  for lo,hi in [(0xa00100,0xa10000),(0xeffc00,0xf00000),(0x78b448,0x78b450),(0x88f5f8,0x88f6a0)]:assert old[lo:hi]==native[lo:hi],(mode,mask,limit,phase,hex(lo))
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_hierarchy_generator_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_hierarchy_generator.c').read_bytes()).hexdigest(),scope='Native recursive generator layouts2/3 through return, no original function hooks. Full integer/FPU/SR/FPUL and canonical recursive stack/matrix/cursor/global RAM plus vector input states compared. Four-node child/sibling/depth tree, empty/animated channels and count-limit rejection. FSCA/FTRV outputs injected; unsupported layout self-loop reported as -2. Numerical fidelity/exceptional arithmetic/hardware pending.'),indent=2)+'\n');print(cases,'native recursive generator comparisons passed')
if __name__=='__main__':main()
