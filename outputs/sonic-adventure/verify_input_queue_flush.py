import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_queue_flush.dll')).sa_input_queue_flush
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for words,padding,total,bank in itertools.product([0,1,3,17],[0,1,4],[0,8],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x8a83b4,0x8ca00000)
  for off,value in [(0xfcc,0x8ca10000),(0xfd8,words),(0xfdc,total)]:struct.pack_into('<I',ram,0xa00000+off,value)
  ram[0xa10000:0xa10200]=bytes([0x55])*512
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  @Call
  def call(ctx,t,m,f,sr,r,pr):return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=padding;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65f7cc
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert (struct.unpack_from('<I',native,0xa00fcc)[0],*struct.unpack_from('<2I',native,0xa00fd8))==(0x8ca10000+words*4,padding,total+padding*4)
  assert native[0xa10000+words*4:0xa10000+(words+padding)*4]==struct.pack('<I',0x700)*padding
  assert r[0]==0
  count+=1
 (out/'analysis/input_queue_flush_verification.json').write_text(json.dumps(dict(cases_passed=count,instruction_count=34,scope='Full retail RAM/CPU/FPU comparison without injected calls; independent buffer advancement, padding marker words, pending count and total byte count checks. Nonnegative padding counts tested.'),indent=2)+'\n')
 print(count,'queue flush comparisons passed')
if __name__=='__main__':main()

