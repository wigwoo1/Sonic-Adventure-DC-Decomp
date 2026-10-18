import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_interrupt_constructor import IO,Read,Write
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();dll=ROOT/'work/sonic-adventure/build/interrupt_remove.dll'
 fn=ctypes.CDLL(str(dll)).sa_interrupt_callback_remove
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.POINTER(IO),ctypes.c_void_p,Call]
 count=0
 for event,kind,mask in itertools.product([0x200,0x220,0x8e0,0x900,0x920,0x940,0xa00,0xa20,0xb00,0xb20],['only','head','middle','tail','absent'],[0,15]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  pool=0x8c892840;head=0x8c893840+(event>>4)*4;slots=[1] if kind=='only' else [1,2,3]
  handle={'only':1,'head':1,'middle':2,'tail':3,'absent':7}[kind]
  for j,slot in enumerate(slots):struct.pack_into('<4I',ram,(pool+slot*16)&0xffffff,0x8c010000,pool+slots[j+1]*16 if j+1<len(slots) else 0,0x1234,0x80000020+j)
  struct.pack_into('<I',ram,head&0xffffff,pool+16)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  hardware={0xa05f6920:0xffffffff,0xa05f6924:0xffffffff};nhardware=dict(hardware);calls=[];ncalls=[];errors=[]
  @Read
  def read(ctx,a,value):
   if a not in nhardware:errors.append(hex(a));return -3
   value[0]=nhardware[a];return 0
  @Write
  def write(ctx,a,value):
   if a not in nhardware:errors.append(hex(a));return -3
   nhardware[a]=value;return 0
  @Call
  def call(ctx,target,m,f,sr,r,pr):ncalls.append((target,tuple(r[4:8]),sr[0],pr[0]));return 0
  def hook(target):
   def body(m,r,sr):calls.append((target,tuple(r[4:8]),sr,s.pr));return sr
   return body
  f=Fpu();s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=handle;s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=0x8c645c54
  nf=Fpu.from_buffer_copy(f);r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();io=IO(0x8ca08000,0,None,read,write)
  reference(ram,s,{t:hook(t) for t in [0x8c64135a,0x8c64137c]},f,mmio=hardware,vbr=io.vbr)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)==0
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and ram==native and hardware==nhardware and calls==ncalls and bytes(nf)==bytes(f),(event,kind,mask)
  assert not errors,errors
  node=struct.unpack_from('<I',native,head&0xffffff)[0];remaining=[]
  while node:
   remaining.append((node-pool)//16);node=struct.unpack_from('<I',native,(node+4)&0xffffff)[0]
  assert remaining==[slot for slot in slots if slot!=handle]
  if kind!='absent':assert struct.unpack_from('<I',native,(pool+handle*16+12)&0xffffff)[0]==0
  if kind=='only':
   expected_targets=[0x8c64135a] if event==0x900 else [0x8c64137c] if event==0x920 else []
   assert [x[0] for x in ncalls]==expected_targets
   if event>>8 in [10,11]:
    address=0xa05f6920 if event>>8==10 else 0xa05f6924;bit=((event&255)>>5)+(12 if event>>8==10 else 0)
    assert nhardware[address]==(0xffffffff^(1<<bit))
  else:assert not ncalls and nhardware=={0xa05f6920:0xffffffff,0xa05f6924:0xffffffff}
  count+=1
 report=dict(cases_passed=count,instruction_count=157,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_interrupt_callback_remove.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Full retail state/RAM/MMIO/service comparisons; independent queue unlink, free marker, final-handler uninstall and hardware mask checks. Valid, absent and interior handles. Host interrupt scheduling incomplete.')
 (out/'analysis/interrupt_remove_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'interrupt removal comparisons passed')
if __name__=='__main__':main()
