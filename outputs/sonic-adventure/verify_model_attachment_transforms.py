import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 call=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(Fpu));fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_attachment_transforms.dll')).sa_model_attachment_with_transforms;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu)]+[ctypes.c_uint32]*3+[ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_void_p,call,ctypes.POINTER(ctypes.c_uint32)];cases=0
 starts=[0x8c07acde,0x8c07ad00,0x8c07ad22,0x8c07ad80,0x8c07ada2,0x8c07adc4]
 for kind,entity,frame,sr0,mutation,depth in itertools.product(range(4,10),[0x8ca00000,0x8ca00100],[0x8cf00000,0x8cf00100],[0,0x700083f3],[False,True],[1,2,3]):
  for m in [old,native]:
   m[0xa00000:0xa00400]=bytes([0xa5])*1024;m[0xf00000:0xf00400]=bytes([0x5a])*1024;struct.pack_into('<3I',m,(frame&0xffffff)+136,1,0x80000000,0xffffffff);struct.pack_into('<3I',m,(frame&0xffffff)+124,0x3f800000,0,0x7fffffff)
   struct.pack_into('<I',m,0x890070,int(mutation));struct.pack_into('<I',m,0x88f69c,depth);struct.pack_into('<I',m,0x88f5f8,0x8ca03000);m[0xa02fc0:0xa03000]=bytes(range(64))
  fpu=Fpu();fpu.fpscr=0x40001;fpu.fpul=0x12345678
  for bank in range(2):fpu.banks[bank][:]=[0x3f800000+bank*100+i for i in range(16)]
  native_fpu=Fpu.from_buffer_copy(fpu)
  state=State();state.r[10]=entity;state.r[15]=frame;state.r[13]=0x8c638ecc;state.r[12]=0x8c638f28;state.r[9]=0x8c639b98;state.r[0]=kind;state.sr=sr0;state.pc=starts[kind-4];state.pr=0x8c000100;events=[]
  def primitive(word,f):
   events.append((word,bytes(f)));bank=(f.fpscr>>21)&1
   if word==0xf1fd:f.banks[bank][:4]=[0x3f800000,0x40000000,0x40800000,0]
   elif word==0xf37d:f.banks[bank][3]=0x3fa00000
   else:f.banks[bank][((word>>8)&12)+3]=0x3f800000+len(events)
   f.fpscr^=0x1000
  def stop(memory,r,s):state.pr=0x8c000100;return s
  reference(old,state,{0x8c07ade6:stop},fpu,vector_instruction=primitive);expected=events[:];events.clear()
  @call
  def service(ctx,word,fp):primitive(word,fp.contents);return 0
  sr=ctypes.c_uint32(sr0);r0=ctypes.c_uint32();result=fn(view,ctypes.byref(native_fpu),kind,entity,frame,ctypes.byref(sr),kind,None,service,ctypes.byref(r0))
  assert result==0 and r0.value==state.r[0] and sr.value==state.sr and events==expected and native[0xa00000:0xa00400]==old[0xa00000:0xa00400] and native[0x88f5f8:0x88f6a0]==old[0x88f5f8:0x88f6a0] and bytes(native_fpu)==bytes(fpu),kind;cases+=1
 failures=0
 for kind in range(4,10):
  calls=[];before=bytes(native)
  @call
  def unavailable(ctx,word,fp):calls.append(word);return -1
  f=Fpu();sr=ctypes.c_uint32();r0=ctypes.c_uint32()
  result=fn(view,ctypes.byref(f),kind,entity,frame,ctypes.byref(sr),kind,None,unavailable,ctypes.byref(r0))
  assert result==-1 and calls==[0xf1fd] and bytes(native)==before
  failures+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_attachment_transforms_verification.json').write_text(json.dumps(dict(cases_passed=cases,failure_propagation_cases_passed=failures,source_sha256=hashlib.sha256((out/'source/model_attachment_transforms.c').read_bytes()).hexdigest(),scope='Record types4..9 fixed destination routing and finishing call. Exact meaningful arguments/incoming r0, callback return and SR mutations, memory checked. Point/direction transform bodies and matrix-stack pop execute without original function hooks. FIPR/FTRV/FSRRA primitive results injected; arithmetic fidelity/drawing incomplete.'),indent=2)+'\n');print(f'{cases} connected attachment transform comparisons and {failures} failure propagation checks passed')
if __name__=='__main__':main()
