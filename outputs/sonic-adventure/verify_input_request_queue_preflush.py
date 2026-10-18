import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_request_queue_native.dll')).sa_input_request_queue_native;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for channel,busy,mode,length,token,bank,trailing,padding,prior,group in itertools.product(range(8),[0,11],[0,1],[0,2],[1,7,0x7fffffff],[0,0x200000],[0,2],[0,2],[0,3],[0,1]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0x8a83b4,0x8ca00000);record=0xa00028+44*channel;ram[record+6]=busy;ram[0x6724c4+channel]=0x40
  for off,value in [(0xfcc,0x8ca10000),(0xfd8,0),(0xfdc,0),(0xfe0,token),(0xfe8,0),(0x10c4,mode)]:struct.pack_into('<I',ram,0xa00000+off,value)
  struct.pack_into('<2I',ram,0xa20000,0x12345678,0x89abcdef);struct.pack_into('<2I',ram,0xf00000,0x8ca21000,trailing);struct.pack_into('<2I',ram,0xa21000,0x10203040,0x8090a0b0)
  struct.pack_into('<I',ram,0xa00fe8+channel*4,0x8ca30000)
  struct.pack_into('<I',ram,0xa010cc,padding);struct.pack_into('<I',ram,0xa00fd8,prior);struct.pack_into('<I',ram,0xa10000,group<<16)
  flushed=bool(padding and prior and group==1);advance=prior+(padding if flushed else 0);dest=0xa10000+advance*4
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[5]=11;s.r[6]=0x8ca20000;s.r[7]=length;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65f810
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();reference(ram,s,fpu=f)
  combo=(channel,busy,mode,length,token,bank,trailing,padding,prior,group)
  result=fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
  if result!=0:raise AssertionError(f'{combo}: function result {result}')
  if errors or native!=ram or list(r)!=list(s.r) or sr.value!=s.sr or pr.value!=s.pr or bytes(nf)!=bytes(f):
   mismatch=next((i for i,(a,b) in enumerate(zip(native,ram)) if a!=b),None)
   raise AssertionError(f'{combo}: full-state mismatch errors={errors!r} first_ram_byte={mismatch} regs_match={list(r)==list(s.r)} sr_match={sr.value==s.sr} pr_match={pr.value==s.pr} fpu_match={bytes(nf)==bytes(f)}')
  if r[0]!=(0xffffffff if busy else token):raise AssertionError(f'{combo}: unexpected result register {r[0]:08x}')
  if not busy:
   if native[record+6]!=11 or struct.unpack_from('<I',native,record+24)[0]!=token:raise AssertionError(f'{combo}: channel record mismatch')
   actual_queue=struct.unpack_from('<3I',native,0xa00fd8);expected_queue=(length+trailing+3,(length+trailing+3)*4+(padding*4 if flushed else 0),1 if token==0x7fffffff else token+1)
   if actual_queue!=expected_queue:raise AssertionError(f'{combo}: queue state {actual_queue!r} != {expected_queue!r}')
   actual_header=struct.unpack_from('<2I',native,dest);expected_header=((1<<16)|(length+trailing),0x0ca30000)
   if actual_header!=expected_header:raise AssertionError(f'{combo}: command header {actual_header!r} != {expected_header!r}')
   if length:
    expected_payload=[0x12345678,0x89abcdef] if mode else [0x78563412,0xefcdab89]
    if list(struct.unpack_from('<2I',native,dest+12))!=expected_payload:raise AssertionError(f'{combo}: payload mismatch')
  if not busy and trailing:
   expected_tail=[0x40302010,0xb0a09080] if mode else [0x10203040,0x8090a0b0]
   if list(struct.unpack_from('<2I',native,dest+12+length*4))!=expected_tail:raise AssertionError(f'{combo}: trailing payload mismatch')
   if struct.unpack_from('<I',native,0xa00fcc)[0]!=0x8c000000+dest:raise AssertionError(f'{combo}: queue address mismatch')
   if flushed and native[0xa10000+prior*4:dest]!=struct.pack('<I',0x700)*padding:raise AssertionError(f'{combo}: preflush mismatch')
  count+=1
 report=dict(cases_passed=count,instruction_count=288,scope='Native queue full RAM/CPU/FPU state versus retail plus independent command buffer size, token wrap, pending flag and payload-endian checks across channels, busy state, encoding modes, lengths and FPU banks. Address conversion native; no injected calls; preflush match and mismatch paths verified with zero/nonzero prior words and padding; trailing payload byte order independently checked.')
 (out/'analysis/input_request_queue_preflush_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'native queue preflush comparisons passed')
if __name__=='__main__':main()

