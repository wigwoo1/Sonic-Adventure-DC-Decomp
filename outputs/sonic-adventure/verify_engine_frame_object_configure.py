import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_object_configure.dll'))
 fn=lib.sa_engine_frame_object_configure
 error_fn=lib.sa_engine_error_record
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 error_fn.argtypes=fn.argtypes
 state_fn=lib.sa_engine_object_state_get;state_fn.argtypes=fn.argtypes
 setters={}
 for address,name in [(0x8c65a5cc,'sa_engine_object_state_set'),(0x8c65a5ac,'sa_engine_object_address_set'),(0x8c65a5d4,'sa_engine_object_length_set'),(0x8c65a5b4,'sa_engine_object_callback_set')]:
  setter=getattr(lib,name);setter.argtypes=fn.argtypes;setters[address]=setter
 cases=0
 for rejection,bank,mask in itertools.product(['context','object','state','success'],[0,0x200000],[0,7,15]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x67c1d0,0 if rejection=='context' else 0x8ca00200)
  struct.pack_into('<I',ram,0xa00004,2 if rejection=='state' else 1)
  struct.pack_into('<I',ram,0xf00000,0x12345678)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t in setters:return setters[t](m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   if t==0x8c65a568:return state_fn(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   if t==0x8c65acac:return error_fn(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   errors.append(t);return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=0x8c643624;s.r[4]=0 if rejection=='object' else 0x8ca00000;s.r[5]=0x8ca02000;s.r[6]=96;s.r[7]=0x8c64241a
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  expected={'context':0x0d000001,'object':0x07000103,'state':0x7fffffff,'success':0}[rejection]
  assert r[0]==expected
  if expected:assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c643624,expected)
  else:assert struct.unpack_from('<5I',native,0xa00004)==(0,0x8ca02000,96,0x8c64241a,0x12345678)
  cases+=1
 (out/'analysis/engine_frame_object_configure_verification.json').write_text(json.dumps(dict(cases_passed=cases,recovered_instructions=110,scope='Full retail CPU/RAM/FPU comparisons for object configuration with native getter/state/address/length/callback setters and error recorder. Missing context/null object/busy state rejection and successful field updates with independent return/error and five object-field checks across FPU banks/interrupt masks. Object allocation/release not executed in these cases.'),indent=2)+'\n')
 print(cases,'object configuration comparisons passed')
if __name__=='__main__':main()
