import ctypes,hashlib,itertools,json,random
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native);rng=random.Random(0x638ecc)
 call=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu));fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_direction_transform.dll')).sa_model_direction_transform;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32),ctypes.c_void_p,call];cases=0
 for matrix,bank,sr0,alias,seed,normalize in itertools.product([0,0x8ca01000],range(2),[0,0x700083f3],[False,True],range(32),[0,1]):
  data=rng.randbytes(0x3000)
  for m in [old,native]:m[0xa00000:0xa03000]=data
  import struct
  for mem in [old,native]:struct.pack_into("<I",mem,0x890070,normalize)
  fpu=Fpu();fpu.fpscr=0x40001|(bank<<21);fpu.fpul=0x12345678
  for k in range(2):fpu.banks[k][:]=[rng.getrandbits(32) for _ in range(16)]
  expected=Fpu.from_buffer_copy(fpu);state=State();state.r[:]=[0x12340000+i for i in range(16)];state.r[4]=matrix;state.r[5]=0x8ca02000;state.r[6]=0x8ca02000 if alias else 0x8ca02100;state.pc=0x8c638f28;state.pr=0x8c000100;state.sr=sr0;r=(ctypes.c_uint32*16)(*state.r);sr=ctypes.c_uint32(sr0);events=[]
  def primitive(word,f):
   events.append((word,bytes(f)))
   active=(f.fpscr>>21)&1
   if word==0xf1fd:f.banks[active][:4]=[0x3f800000,0x80000000,0x40800000,0x12345678]
   elif word==0xf37d:f.banks[active][3]=0x3fa00000
   else:f.banks[active][((word>>8)&12)+3]=0x3f800000+len(events)
   f.fpscr^=0x1000
  reference(old,state,{},expected,vector_instruction=primitive);wanted=events[:];events.clear()
  @call
  def service(ctx,word,fp):primitive(word,fp.contents);return 0
  result=fn(view,ctypes.byref(fpu),ctypes.byref(sr),r,None,service)
  assert result==0 and list(r)==list(state.r) and sr.value==state.sr and bytes(fpu)==bytes(expected) and events==wanted and native[0xa00000:0xa03000]==old[0xa00000:0xa03000],(matrix,bank,sr0,alias);cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_direction_transform_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_direction_transform.c').read_bytes()).hexdigest(),scope='Original point-transform register/RAM dataflow through return with explicit instruction services for FIPR/FTRV/FSRRA. Supplied/implicit matrix, both banks, source/destination alias, raw register values and SR. Exact vector-operation inputs/order, RAM and full integer/FPU state compared. Injected primitive results verify orchestration only; vector product/reciprocal-root arithmetic remains unrecovered; final native FMULs execute.'),indent=2)+'\n');print(f'{cases} direction-transform dataflow comparisons passed')
if __name__=='__main__':main()
