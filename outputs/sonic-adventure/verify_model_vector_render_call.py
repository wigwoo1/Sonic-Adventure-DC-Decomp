import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 callback=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32));fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_vector_render_call.dll')).sa_model_vector_render_call;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu)]+[ctypes.POINTER(ctypes.c_uint32)]*2+[ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_void_p,callback];cases=0
 lists=[[1,2,3,4,5,6,7,8,9],[255,0,10,9,3,2,1,6,4]]
 for kinds,sr0,paired,cached,seed,normalize in itertools.product(lists,[0,0x700083f3],[False,True],[0,255],range(4),[False,True]):
  # Mesh-vector branches1..3 require two descriptor banks; single-descriptor
  # records use attachment branches, as their mesh pointers are not prepared.
  if not paired:kinds=[kind+3 if 1<=kind<=3 else kind for kind in kinds]
  rng=random.Random(seed);config=0x8ca00000;record=0x8ca00100;frame=0x8cf00000;entity=0x8ca12000;root=0x8ca08000;first=root+64;other=root+128;count=[0,1,3,8][seed];identity=struct.pack('<16f',*[float(i%5==0) for i in range(16)])
  for m in (old,native):
   m[0xa00000:0xa13000]=bytes(0x13000);m[0xf00000:0xf00100]=bytes([0x5a])*256;m[0xefff00:0xf00000]=bytes([0x55])*256;m[0x789448:0x789648]=identity*8;m[0x798698:0x798798]=identity*4
   struct.pack_into('<H',m,0xa00004,0);struct.pack_into('<I',m,0xa00020,0x8ca01000);struct.pack_into('<I',m,0xa01000,0x8ca00600);struct.pack_into('<2I',m,0xa00600,root,0x8ca09000);m[0xa01004]=2;struct.pack_into('<2I',m,0xa09000,0x8ca09100,4);struct.pack_into('<H',m,0xa09008,3);
   for node in [root,first,other]:
    struct.pack_into('<I',m,(node&0xffffff),7);struct.pack_into('<3f',m,(node&0xffffff)+8,0,0,0);struct.pack_into('<3I',m,(node&0xffffff)+20,0,0,0);struct.pack_into('<3f',m,(node&0xffffff)+32,1,1,1)
   struct.pack_into('<I',m,0xa0802c,first);struct.pack_into('<I',m,0xa08070,other)
   struct.pack_into('<I',m,(first&0xffffff)+4,0x8ca00610);struct.pack_into('<I',m,(other&0xffffff)+4,0x8ca00620);struct.pack_into('<2I',m,0xa00610,0x8ca0a000,0x8ca0b000);struct.pack_into('<2I',m,0xa00620,0x8ca0c000,0x8ca0d000)
   for i,kind in enumerate(kinds):
    off=0xa00100+i*24;struct.pack_into('<3I',m,off,root if i%4!=seed%4 else root+256,first,other if paired else 0);m[off+12]=count;m[off+13]=kind;m[off+14]=cached;m[off+15]=cached;struct.pack_into('<2I',m,off+16,0x8ca02000+i*512,0x8ca07000+i*64)
    for j in range(count):struct.pack_into('<2H',m,0xa07000+i*64+j*4,(j*3)%16,(j*7)%16)
    for j in range(32):struct.pack_into('<3f',m,0xa02000+i*512+j*12,1+j/16,-2-j/32,3+j/64)
   table=[(68,0x8c798698),(64,0x8c7986d8),(60,0x8c798718),(56,0x8c798758),(52,0x8c798698),(48,0x8c7986d8),(44,0x8c798718),(40,0x8c798758),(88,0x8c798698),(104,0x8c798698),(80,0x8c798718),(116,0x8c7986d8),(92,0x8c7986d8),(96,0x8c798758),(120,0x8c798698),(84,0x8c798758),(100,0x8c798698),(108,0x8c798718),(112,0x8c7986d8)]
   for off,value in table:struct.pack_into('<I',m,0xf00000+off,value)
   struct.pack_into('<I',m,0x78b448,8);struct.pack_into('<2I',m,0x88f698,2 if seed<2 else 16,1);struct.pack_into('<I',m,0x88f5f8,0x8ca10000);struct.pack_into('<I',m,0x890070,int(normalize))
  for m in (old,native):
   m[0xa12120:0xa12160]=m[0xa00000:0xa00040];struct.pack_into('<H',m,0xa12122,1);struct.pack_into('<I',m,0xa12144,record)
  f=Fpu();f.fpscr=((seed%2)<<21)|0x40000|(seed//2);f.fpul=0x12345678
  for j in range(2):f.banks[j][:]=[rng.getrandbits(32) for _ in range(16)]
  bank=seed%2;f.banks[bank][13:16]=[0,0x3f800000,0x3f000000];nf=Fpu.from_buffer_copy(f);state=State();state.r[8]=config;state.r[14]=record;state.r[15]=frame+192;state.r[6]=entity;state.r[10]=entity;state.r[9]=0x8c639b98;state.r[12]=0x8c638f28;state.r[13]=0x8c638ecc;state.r[11]=0;state.sr=sr0;state.pc=0x8c07a55c;state.pr=0x8c000100;registers=(ctypes.c_uint32*16)(*state.r);events=[]
  def primitive(word,fp):
   events.append((word,bytes(fp)));active=(fp.fpscr>>21)&1
   if word&0xf3ff==0xf1fd:
    start=(word>>8)&12;fp.banks[active][start:start+4]=[0x3f800000,0x40000000,0x40800000,0]
   elif word==0xf37d:fp.banks[active][3]=0x3fa00000
   else:fp.banks[active][((word>>8)&12)+3]=0x40000001+len(events)%10
   fp.fpscr^=0x1000
  def stop(m,r,s):state.pr=0x8c000100;return s
  reference(old,state,{},f,vector_instruction=primitive);expected=events[:];events.clear()
  @callback
  def service(ctx,word,fp,current_fpul):fp.contents.fpul=current_fpul[0];primitive(word,fp.contents);return 0
  sr=ctypes.c_uint32(sr0);r0=ctypes.c_uint32();end=ctypes.c_uint32();fpul=ctypes.c_uint32(nf.fpul);status=fn(view,ctypes.byref(nf),ctypes.byref(fpul),ctypes.byref(sr),registers,0x8c000100,None,service);nf.fpul=fpul.value
  assert status==0 and registers[0]==state.r[0] and sr.value==state.sr and list(registers)[8:16]==list(state.r)[8:16] and bytes(nf)==bytes(f) and events==expected and native[0xa00100:0xa00200]==old[0xa00100:0xa00200] and native[0xa0a000:0xa13000]==old[0xa0a000:0xa13000] and native[0xf00000:0xf00100]==old[0xf00000:0xf00100] and native[0x789448:0x789508]==old[0x789448:0x789508] and native[0x798698:0x798798]==old[0x798698:0x798798] and native[0x88f5f8:0x88f6a0]==old[0x88f5f8:0x88f6a0],(kinds,paired,cached,seed,normalize,status)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_vector_render_call_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_vector_render_call.c').read_bytes()).hexdigest(),scope='Original helper state1 from entry through return with native frame/rendering/hierarchy/record paths. r0/SR/FPU/FPUL/callee-preserved integer registers/SP/frame/matrix/mesh/attachment RAM and vector input states compared. No original function hooks; vector outputs injected. Volatile integer scratch/full nested stack RAM excluded, states0/cleanup and numerical fidelity pending.'),indent=2)+'\n');print(f'{cases} model vector render call comparisons passed')
if __name__=='__main__':main()
