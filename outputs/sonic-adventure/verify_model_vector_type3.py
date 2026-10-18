import ctypes,hashlib,itertools,json,random,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 callback=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,*([ctypes.c_uint32]*6),ctypes.POINTER(ctypes.c_uint32))
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/model_vector_type3.dll')).sa_model_vector_type3;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32),ctypes.c_void_p,callback,ctypes.POINTER(ctypes.c_uint32)]
 cases=0;record=0x8ca00000;frame=0x8cf00000
 for count,sr0,mutate,seed in itertools.product([0,1,2,8,32,255],[0,0x700083f3],[False,True],range(4)):
  rng=random.Random(seed+count*100);region=bytes(rng.randrange(256) for _ in range(0x10000))
  for m in (old,native):
   m[0xa00000:0xa10000]=region;m[0xf00000:0xf00100]=bytes([0x5a])*256;m[0xa0000c]=count
   struct.pack_into('<2I',m,0xa00010,0x8ca02000,0x8ca01000)
   for i in range(count):struct.pack_into('<2H',m,0xa01000+i*4,(i*7+seed)%64,(i*13+seed)%64)
   for off,value in [(20,0x8ca05000),(24,0x8ca06000),(28,0x8ca07000),(32,0x8ca08000),(108,0x8ca09000),(112,0x8ca09100)]:struct.pack_into('<I',m,0xf00000+off,value)
  events=[]
  def invoke(m,addr,argc,a,b,c,r0,s):
   events.append((addr,argc,a,b if argc>=2 else 0,c if argc==3 else 0,r0,s))
   if argc==3:m[c&0xffffff:(c&0xffffff)+12]=m[b&0xffffff:(b&0xffffff)+12]
   return 0x12340000+len(events),s^0x01000031 if mutate else s
  state=State();state.pc=0x8c07ac0e;state.pr=0x8c000100;state.r[14]=record;state.r[15]=frame;state.r[11]=0;state.r[13]=0x8c638ecc;state.r[12]=0x8c638f28;state.r[9]=0x8c639b98;state.sr=sr0
  def hook(addr,argc):
   def run(m,r,s):r[0],s=invoke(m,addr,argc,r[4],r[5],r[6],r[0],s);return s
   return run
  def stop(m,r,s):state.pr=0x8c000100;return s
  hooks={a:hook(a,n) for a,n in [(0x8c639c70,1),(0x8c63997c,2),(0x8c638ecc,3),(0x8c638f28,3),(0x8c639b98,1)]};hooks[0x8c07ade6]=stop
  reference(old,state,hooks);expected=events[:];events.clear()
  @callback
  def service(ctx,addr,argc,a,b,c,r0,s):out,s[0]=invoke(native,addr,argc,a,b,c,r0,s[0]);return out
  sr=ctypes.c_uint32(sr0);result=ctypes.c_uint32();assert fn(view,record,frame,ctypes.byref(sr),None,service,ctypes.byref(result))==0
  assert events==expected,(count,seed,events,expected)
  assert sr.value==state.sr and result.value==state.r[0] and native[0xa00000:0xa10000]==old[0xa00000:0xa10000] and native[0xf00000:0xf00100]==old[0xf00000:0xf00100],(count,seed)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/model_vector_type3_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/model_vector_type3.c').read_bytes()).hexdigest(),scope='Original type3 branch through shared stack-pop; full fixture RAM, r0/SR and service argument/order differential checks. Stack push, rotation, point/direction transforms and pop remain services; arithmetic and rendering integration incomplete.'),indent=2)+'\n');print(f'{cases} type3 rendering branch comparisons passed')
if __name__=='__main__':main()


