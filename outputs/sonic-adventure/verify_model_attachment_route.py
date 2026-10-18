import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 call=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,*([ctypes.c_uint32]*6),ctypes.POINTER(ctypes.c_uint32));fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_attachment_route.dll')).sa_model_attachment_route;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.c_uint32]*3+[ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_void_p,call,ctypes.POINTER(ctypes.c_uint32)];cases=0
 starts=[0x8c07acde,0x8c07ad00,0x8c07ad22,0x8c07ad80,0x8c07ada2,0x8c07adc4]
 for kind,entity,frame,sr0,mutation in itertools.product(range(4,10),[0x8ca00000,0x8ca00100],[0x8cf00000,0x8cf00100],[0,0x700083f3],[False,True]):
  for m in [old,native]:
   m[0xa00000:0xa00400]=bytes([0xa5])*1024;m[0xf00000:0xf00400]=bytes([0x5a])*1024;struct.pack_into('<3I',m,(frame&0xffffff)+136,1,0x80000000,0xffffffff);struct.pack_into('<3I',m,(frame&0xffffff)+124,0x3f800000,0,0x7fffffff)
  state=State();state.r[10]=entity;state.r[15]=frame;state.r[13]=0x8c638ecc;state.r[12]=0x8c638f28;state.r[9]=0x8c639b98;state.r[0]=kind;state.sr=sr0;state.pc=starts[kind-4];state.pr=0x8c000100;events=[]
  def invoke(memory,addr,argc,a,b,c,r0,s):
   events.append((addr,argc,a,b if argc==3 else 0,c if argc==3 else 0,r0))
   if argc==3:memory[c&0xffffff:(c&0xffffff)+12]=memory[b&0xffffff:(b&0xffffff)+12]
   return 0x12340000+len(events),s^0x01000031 if mutation else s
  def hook(addr,argc):
   def run(memory,r,s):r[0],s=invoke(memory,addr,argc,r[4],r[5],r[6],r[0],s);return s
   return run
  def stop(memory,r,s):state.pr=0x8c000100;return s
  reference(old,state,{0x8c638ecc:hook(0x8c638ecc,3),0x8c638f28:hook(0x8c638f28,3),0x8c639b98:hook(0x8c639b98,1),0x8c07ade6:stop});expected=events[:];events.clear()
  @call
  def service(ctx,addr,argc,a,b,c,r0,s):result,s[0]=invoke(native,addr,argc,a,b,c,r0,s[0]);return result
  sr=ctypes.c_uint32(sr0);r0=ctypes.c_uint32();result=fn(kind,entity,frame,ctypes.byref(sr),kind,None,service,ctypes.byref(r0))
  assert result==0 and r0.value==state.r[0] and sr.value==state.sr and events==expected and native[0xa00000:0xa00400]==old[0xa00000:0xa00400],kind;cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_attachment_route_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_attachment_route.c').read_bytes()).hexdigest(),scope='Record types4..9 fixed destination routing and finishing call. Exact meaningful arguments/incoming r0, callback return and SR mutations, memory checked. Transform and matrix-stack services hooked; actual mathematics/drawing unrecovered.'),indent=2)+'\n');print(f'{cases} attachment routing comparisons passed')
if __name__=='__main__':main()
