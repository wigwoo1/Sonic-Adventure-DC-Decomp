import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 callback=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,*([ctypes.c_uint32]*6),ctypes.POINTER(ctypes.c_uint32));fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_render_record_setup.dll')).sa_model_render_record_setup;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32)]+[ctypes.c_uint32]*3+[ctypes.c_void_p,callback]+[ctypes.POINTER(ctypes.c_uint32)]*2;cases=0
 for sr0,match,second,cached1,cached2,seed in itertools.product([0,0x700083f3],[False,True],[False,True],[0,254,255],[1,254,255],range(4)):
  rng=random.Random(seed);config=0x8ca00000;record=0x8ca00100;frame=0x8cf00000;root=0x8ca04000;first=root+64;other=root+128;data=rng.randbytes(512);matrices=rng.randbytes(256)
  for m in (old,native):
   m[0xa00000:0xa05000]=bytes(0x5000);m[0xf00000:0xf00100]=bytes([0x5a])*256;m[0xeffff4:0xf00000]=bytes([0x55])*12;m[0x789448:0x789648]=data;m[0x798698:0x798798]=matrices
   struct.pack_into('<H',m,0xa00004,0);struct.pack_into('<I',m,0xa00020,0x8ca01000);struct.pack_into('<I',m,0xa01000,0x8ca00600);struct.pack_into('<I',m,0xa00600,root if match else root+256);struct.pack_into('<3I',m,0xa00100,root,first,other if second else 0);m[0xa0010c]=seed;m[0xa0010e]=cached1;m[0xa0010f]=cached2
   struct.pack_into('<I',m,0xa0402c,first);struct.pack_into('<I',m,0xa04070,other)
   struct.pack_into('<I',m,(first&0xffffff)+4,0x8ca00200);struct.pack_into('<I',m,(other&0xffffff)+4,0x8ca00210);struct.pack_into('<2I',m,0xa00200,0x8ca06000,0x8ca07000);struct.pack_into('<2I',m,0xa00210,0x8ca08000,0x8ca09000)
   for off,value in [(88,0x8c798698),(104,0x8c798698),(80,0x8c798718),(116,0x8c7986d8),(92,0x8c7986d8),(96,0x8c798758),(120,0x8c798698)]:struct.pack_into('<I',m,0xf00000+off,value)
   struct.pack_into('<I',m,0x78b448,8);struct.pack_into('<2I',m,0x88f698,1 if seed<2 else 4,1);struct.pack_into('<I',m,0x88f5f8,0x8ca0a000);m[0xa0a000:0xa0a080]=bytes([0x55])*128
  f=Fpu();f.fpscr=((seed%2)<<21)|0x40000|(seed//2);f.fpul=0x12345678
  for j in range(2):f.banks[j][:]=[rng.getrandbits(32) for _ in range(16)]
  bank=seed%2;f.banks[bank][13]=0;f.banks[bank][14]=0x3f800000;nf=Fpu.from_buffer_copy(f);state=State();state.r[8]=config;state.r[14]=record;state.r[15]=frame;state.sr=sr0;state.pc=0x8c07a800;state.pr=0x8c000100;events=[];dest=[]
  def invoke(m,a,incoming,s):
   events.append((a,incoming,s));m[(a&0xffffff)+60]^=0x55;return 0x12340000+len(events),s^0x01000031
  def inversion(m,r,s):r[0],s=invoke(m,r[4],r[0],s);return s
  def hook(addr):
   def stop(m,r,s):dest.append(addr);state.pr=0x8c000100;return s
   return stop
  reference(old,state,{0x8c6390b0:inversion,**{a:hook(a) for a in [0x8c07ade6,0x8c07a93c]}},f);expected=events[:];events.clear()
  @callback
  def service(ctx,addr,argc,a,b,c,incoming,s):
   assert addr==0x8c6390b0 and argc==1
   result,s[0]=invoke(native,a,incoming,s[0]);return result
  sr=ctypes.c_uint32(sr0);r0=ctypes.c_uint32();nextpc=ctypes.c_uint32();assert fn(view,ctypes.byref(nf),ctypes.byref(sr),config,record,frame,None,service,ctypes.byref(r0),ctypes.byref(nextpc))==0
  assert r0.value==state.r[0] and sr.value==state.sr and nextpc.value==dest[0] and bytes(nf)==bytes(f) and events==expected and native[0xa00100:0xa00118]==old[0xa00100:0xa00118] and native[0xf00000:0xf00100]==old[0xf00000:0xf00100] and native[0xeffff4:0xeffffc]==old[0xeffff4:0xeffffc] and native[0x798698:0x798798]==old[0x798698:0x798798] and native[0xa0a000:0xa0a080]==old[0xa0a000:0xa0a080] and native[0x88f5f8:0x88f6a0]==old[0x88f5f8:0x88f6a0],(match,second,cached1,cached2,seed)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_render_record_setup_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_render_record_setup.c').read_bytes()).hexdigest(),scope='Per-record setup8c07a800 through skip/dispatch, native cache/descriptor lookup/matrix retrieval/copy/single-mesh stack push. r0/SR/FPU/record/frame/canonical scratch/matrix/stack RAM and inversion service inputs compared. Root mismatches, missing/valid/out-of-range caches, single/paired descriptors, banks/RM and capacity cases. Only inversion function hooked; full integration/inversion mathematics pending.'),indent=2)+'\n');print(f'{cases} connected per-record matrix setup comparisons passed')
if __name__=='__main__':main()
