import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_render_record_cache.dll')).sa_model_render_record_cache;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32)]+[ctypes.c_uint32]*3+[ctypes.POINTER(ctypes.c_uint32)]*2;cases=0;revisits=0
 for sr0,index,match,cached,targetpos in itertools.product([0,0x700083f3],[0,65535],[False,True],[0,254,255],[0,1,127,254,255,256,269,-1,-2]):
  config=0x8ca00000;record=0x8ca00100;frame=0x8cf00000;root=0x8ca02000;entry=0x8ca01000+index*16;target=root+targetpos*64 if targetpos>=0 else 0x8ca0f000 if targetpos==-1 else 0
  for m in (old,native):
   m[0xa00000:0xa00120]=bytes([0x5a])*288;m[0xeffff4:0xf00000]=bytes([0x55])*12
   struct.pack_into('<H',m,0xa00004,index);struct.pack_into('<I',m,0xa00020,0x8ca01000);struct.pack_into('<I',m,entry&0xffffff,0x8ca00600);struct.pack_into('<I',m,0xa00600,root if match else 0x8ca05000);struct.pack_into('<2I',m,0xa00100,root,target);m[0xa0010e]=cached
   for i in range(270):struct.pack_into('<2I',m,0xa02000+i*64+44,root+(i+1)*64 if i<269 else 0,0)
  state=State();state.r[8]=config;state.r[14]=record;state.r[15]=frame;state.sr=sr0;state.pc=0x8c07a800;state.pr=0x8c000100;dest=[]
  def hook(addr):
   def stop(m,r,s):dest.append(addr);state.pr=0x8c000100;return s
   return stop
  reference(old,state,{a:hook(a) for a in [0x8c07ade6,0x8c07a832]})
  sr=ctypes.c_uint32(sr0);r0=ctypes.c_uint32();nextpc=ctypes.c_uint32();assert fn(view,ctypes.byref(sr),config,record,frame,ctypes.byref(r0),ctypes.byref(nextpc))==0
  assert r0.value==state.r[0] and sr.value==state.sr and nextpc.value==dest[0] and native[0xa00100:0xa00118]==old[0xa00100:0xa00118] and native[0xeffff4:0xeffffc]==old[0xeffff4:0xeffffc],(index,match,cached,targetpos)
  if match and cached==255:
   assert native[0xa0010e]==((targetpos if targetpos>=0 else -1)&255)
   assert nextpc.value==0x8c07ade6
   stored=native[0xa0010e];dest.clear();state.pc=0x8c07a800;state.pr=0x8c000100
   reference(old,state,{a:hook(a) for a in [0x8c07ade6,0x8c07a832]})
   assert fn(view,ctypes.byref(sr),config,record,frame,ctypes.byref(r0),ctypes.byref(nextpc))==0
   assert r0.value==state.r[0] and sr.value==state.sr and nextpc.value==dest[0] and native[0xa00100:0xa00118]==old[0xa00100:0xa00118] and native[0xeffff4:0xeffffc]==old[0xeffff4:0xeffffc],('revisit',targetpos)
   assert nextpc.value==(0x8c07ade6 if stored==255 else 0x8c07a832)
   revisits+=1
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_render_record_cache_verification.json').write_text(json.dumps(dict(cases_passed=cases,revisits_passed=revisits,source_sha256=hashlib.sha256((out/'source/model_render_record_cache.c').read_bytes()).hexdigest(),scope='Ownership/cache gate connected to native descriptor lookup; original lookup executes without hooks. r0/SR/cache record/counter/root scratch/branch compared. 270-node acyclic trees, index boundary values, success/failure/null targets, descriptor indices255/256 wrap to byte, mismatches/cached IDs. Second visits verify lookup-marker retries and cached-ready behavior. Volatile callee registers/nested saves excluded; matrix construction pending.'),indent=2)+'\n');print(f'{cases} connected rendering descriptor cache comparisons and {revisits} revisits passed')
if __name__=='__main__':main()
