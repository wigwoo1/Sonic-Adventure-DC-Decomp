import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_request_allocate_native.dll')).sa_input_request_allocate_queued_native;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for channel,lock,notify,param,bank,mode,busy,padding in itertools.product(range(8),[0,1,128],[0,1],[0,1],[0,0x200000],[0,1],[0,11],[0,2]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0x8a83b4,0x8ca00000);struct.pack_into('<I',ram,0xa010e8,notify);struct.pack_into('<I',ram,0x6607f4,0x8ca02000);ram[0xa02000]=lock;struct.pack_into('<I',ram,0xf00000,0x12345678)
  ram[0x6724c4+channel]=0x40;record=0xa00028+channel*44;ram[record+6]=busy
  for off,value in [(0xfcc,0x8ca10000),(0xfd8,3),(0xfdc,12),(0xfe0,7),(0x10c4,mode),(0x10cc,padding),(0xfe8+channel*4,0x8ca30000)]:struct.pack_into('<I',ram,0xa00000+off,value)
  struct.pack_into('<I',ram,0xa10000,1<<16)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  def effect(mem,r,t,pr):
   payload=struct.unpack_from('<2I',mem,r[6]&0xffffff) if t==0x8c65f810 else ()
   trace=(t,tuple(r[i] for i in range(4,8)),payload,r[15],pr);r[0]=7 if t==0x8c65f810 else 0x87654321;return trace
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in [0x8c65f348,0x8c646654]:errors.append(hex(t));return -9
   actual.append(effect(native,r,t,pr[0]));return 0
  def hook(t):
   def body(mem,r,sr):expected.append(effect(mem,r,t,s.pr));return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[5]=2;s.r[6]=param;s.r[7]=3;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c6469c8
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();reference(ram,s,{t:hook(t) for t in [0x8c65f348,0x8c646654]},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and actual==expected and bytes(nf)==bytes(f)
  assert r[0]==(0xfffffffe if lock else (0xffffffff if busy else 7)) and native[0xa02000]==(lock|128 if lock else 0)
  if not lock and not busy:
   dest=0xa10000+(3+padding)*4
   payload=[2,(param<<24)|(3<<16)|0x12345678]
   if not mode:payload=[int.from_bytes(v.to_bytes(4,'little'),'big') for v in payload]
   assert list(struct.unpack_from('<2I',native,dest+12))==payload
   assert native[record+6]==11 and struct.unpack_from('<I',native,record+24)[0]==7
   assert struct.unpack_from('<3I',native,0xa00fd8)==(5,32+padding*4,8)
  count+=1
 report=dict(cases_passed=count,instruction_count=395,scope='Native allocation wrapper, lock and payload packer compared with retail full state/call/payload trace. Independent lock/result/packed-word checks. Queue, address conversion and preflush execute natively; contention logger and optional notification injected. Single-thread instruction semantics; host concurrency behavior unverified.')
 (out/'analysis/input_request_allocate_queued_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'queued allocation comparisons passed')
if __name__=='__main__':main()
