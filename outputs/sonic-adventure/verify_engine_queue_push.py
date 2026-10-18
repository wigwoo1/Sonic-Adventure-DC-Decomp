import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_queue_push.dll'))
 fn=lib.sa_engine_queue_push
 error_fn=lib.sa_engine_error_record
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 error_fn.argtypes=fn.argtypes
 cases=0
 for present,index,capacity,count,bank,mask in itertools.product([False,True],[0,3],[4],[0,3,4,5],[0,0x200000],[0,7,15]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x67c1d0,0x8ca00200 if present else 0)
  struct.pack_into('<5I',ram,0xa00394,0x8ca00400,capacity,index,2,count)
  struct.pack_into('<2I',ram,0xa00400+index*8,0x8ca02000,0x76543210)
  struct.pack_into('<2I',ram,0xa00000,0x12345678,0xabcdef01)
  queue_before=bytes(ram[0xa00200:0xa00500]);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t==0x8c65acac:return error_fn(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   errors.append(t);return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=0x8c667080;s.r[4]=0x8ca02000;s.r[5]=0x76543210
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  expected=0x0d000001 if not present else (0x05000101 if count>=capacity else 0)
  assert r[0]==expected
  if expected==0:
   assert struct.unpack_from('<2I',native,0xa00400+index*8)==(0x8ca02000,0x76543210)
   assert struct.unpack_from('<I',native,0xa0039c)[0]==(index+1)%capacity
   assert struct.unpack_from('<I',native,0xa003a4)[0]==count+1
  else:
   assert native[0xa00200:0xa00500]==queue_before
   assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c667080,expected)
  cases+=1
 (out/'analysis/engine_queue_push_verification.json').write_text(json.dumps(dict(cases_passed=cases,recovered_instructions=95,scope='Retail RAM/CPU/FPU comparisons for push, wrap, full/over-capacity and missing context, using native error recording. Independent pair, index, count and error checks. Corrupt or aliased structures not covered.'),indent=2)+'\n')
 print(cases,'queue push comparisons passed')
if __name__=='__main__':main()
