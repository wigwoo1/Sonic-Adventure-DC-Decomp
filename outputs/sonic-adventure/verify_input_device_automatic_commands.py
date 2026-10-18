import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_input_device_backend_host import IO,Read,Write
from verify_input_command_transport import Exchange,load_transport

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();out=ROOT/'outputs/sonic-adventure'
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_submit_host.dll')).sa_input_device_submit_host
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call,ctypes.POINTER(IO)]
 transport=load_transport();count=0
 states=[(1,0),(2,0),(3,0),(4,0),(9,0),(9,1),(9,2),(9,0x01000000)]
 for (status,features),channel,mode,bank,token in itertools.product(states,[0,18],[0,1],[0,0x200000],[1,7,0x7fffffff]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;base=0xa10000;rec=base+0x28+channel*44
  struct.pack_into('<I',ram,0x8a83b4,0x8ca10000);struct.pack_into('<I',ram,0x6607f4,0x8ca80000)
  for off,val in [(0xfc0,1),(0x10f4,0xa05f6c00),(0x10a8,0x8ca50000),(0x10ac,0x8ca60000),(0x10b4,0),(0xfcc,0x8ca50000),(0xfe0,token),(0x10c4,mode),(0xfe8+channel*4,0x8ca70000)]:struct.pack_into('<I',ram,base+off,val)
  ram[base+0xf90+channel]=1;ram[rec+4]=status;struct.pack_into('<I',ram,base+0x44c+channel*120,features)
  hardware={0xa05f6c00+off:0 for off in [4,0x14,0x18,0x80]};native_registers=dict(hardware);errors=[]
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
  @Read
  def read(ctx,addr,width):
   if width!=4 or addr not in native_registers:errors.append(('read',addr,width));return 0
   return native_registers[addr]
  @Write
  def write(ctx,addr,value,width):
   if width!=4 or addr not in native_registers:errors.append(('write',addr,width));return
   native_registers[addr]=value
  io=IO(None,read,write)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)]
  s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65efda
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'mach':0,'macl':0,'gbr':0}
  reference(ram,s,fpu=f,special=special,mmio=hardware)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call,ctypes.byref(io))==0 and not errors
  assert native==ram and native_registers==hardware and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
  assert struct.unpack_from('<I',native,rec+28)[0]==token and struct.unpack_from('<I',native,rec+24)[0]==0
  assert native[rec+6]==0 and native[rec+7]==status
  assert struct.unpack_from('<I',native,base+0xfe0)[0]==(((token+1)&0x7fffffff) or 1)
  seen=[]
  @Exchange
  def exchange(ctx,request,response,capacity):
   q=request.contents;seen.append((q.port,q.command,q.destination,q.source,q.payload_words,q.response_address,q.packet_mode))
   if status==9:
    value=int.from_bytes(bytes(q.payload[:4]),'little' if mode else 'big')
    if value!=(1 if features&1 else features):errors.append('feature payload')
   return 0
  assert transport(view,len(native),native_registers[0xa05f6c04],1024,mode,None,exchange)==1
  assert seen==[(channel//6,status,(channel//6)*64+32,(channel//6)*64,int(status==9),0x0ca70000,mode)] and not errors,seen
  count+=1
 (out/'analysis/input_device_automatic_commands_verification.json').write_text(json.dumps(dict(cases_passed=count,scope='Native command preparation/submission executes commands 1/2/3/4 and feature command 9 across both packet modes, FPU banks, ports and token wrap. Full retail RAM/CPU/FPU/MACL/register comparison plus independent token/command and host-transport-decoded header/payload checks. No injected game calls. Device states and host registers are test fixtures; no response behavior verified here.'),indent=2)+'\n')
 print(count,'automatic device command comparisons passed')

if __name__=='__main__':main()
