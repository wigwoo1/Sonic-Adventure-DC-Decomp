import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 root=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_frame_request_enqueue.dll'))
 abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 fn=lib.sa_engine_frame_request_enqueue;fn.argtypes=abi
 move=lib.sa_memory_move;move.argtypes=abi
 error=lib.sa_engine_error_record;error.argtypes=abi
 cases=0
 for count,available,bank,mask in itertools.product([0,1,31,32],[1,0xffffffff],[0,0x200000],[0,7,15]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x67c1d0,0x8ca01000)
  struct.pack_into('<2I',ram,0xa01158,0x8ca02000,count);struct.pack_into('<I',ram,0xa0116c,available)
  ram[0xa02000:0xa02200]=bytes([0x55])*512
  ram[0xa00000:0xa0001c]=bytes((i*13+7)&255 for i in range(28));request=bytes(ram[0xa00000:0xa0001c])
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   function=move if t==0x8c10d25c else error if t==0x8c65acac else None
   if function is None:errors.append(t);return -9
   return function(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=0x8c666ba8;s.r[4]=0x8ca00000
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  expected=bytearray([0x55]*512)
  if count<32:
   expected[count*16]=request[0];expected[count*16+2:count*16+16]=request[4:18]
   assert r[0]==0 and struct.unpack_from('<I',native,0xa0115c)[0]==count+1
   assert struct.unpack_from('<I',native,0xa0116c)[0]==(available-1)&0xffffffff
   assert native[0xa00000:0xa00014]==request[:20] and struct.unpack_from('<I',native,0xa00014)[0]==0
  else:
   assert r[0]==0x0a000001 and native[0xa00000:0xa0001c]==request
   assert struct.unpack_from('<I',native,0xa0115c)[0]==count and struct.unpack_from('<I',native,0xa0116c)[0]==available
   assert struct.unpack_from('<2I',native,0x67c1c8)==(0x8c666ba8,0x0a000001)
  assert native[0xa02000:0xa02200]==expected
  cases+=1
 (root/'analysis/engine_frame_request_enqueue_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='Full retail CPU/RAM/FPU comparison of request enqueue and native move/error helpers. Independent 16-byte slot layout, 14 copied source bytes, preserved byte/tail, request release field, counters and capacity rejection across counts 0/1/31/32, available counters, banks and masks. Context/buffer/request are fixtures; native request acquisition and consumer not executed.'),indent=2)+'\n')
 print(cases,'request enqueue comparisons passed')
if __name__=='__main__':main()
