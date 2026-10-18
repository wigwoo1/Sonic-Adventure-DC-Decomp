import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_input_device_backend_host import IO,Read,Write

class Registers(dict):
 def __init__(self,busy,release,step,clock):
  super().__init__({0xffd8000c:clock,**{0xa05f6c00+o:0x12340000 for o in [4,0x10,0x14,0x80,0x8c]},0xa05f6c18:busy})
  self.release=release;self.step=step;self.busy_reads=0;self.trace=[]
 def __getitem__(self,address):
  value=super().__getitem__(address)
  if address==0xffd8000c:super().__setitem__(address,(value-self.step)&0xffffffff)
  if address==0xa05f6c18:
   self.busy_reads+=1
   if self.release>=0 and self.busy_reads>=self.release:value=0;super().__setitem__(address,0)
  self.trace.append((address,value));return value

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_shutdown_host.dll')).sa_input_device_shutdown_host
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call,ctypes.POINTER(IO)];count=0
 scenarios=[(0,-1,0),(1,0,1),(1,3,1),(1,3,25000),(1,-1,100001)]
 for (busy,release,step),clock,bank in itertools.product(scenarios,[0,0xffffffff],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x8a83b4,0x8ca10000);struct.pack_into('<I',ram,0xa110f4,0xa05f6c00)
  hardware=Registers(busy,release,step,clock);native_registers=Registers(busy,release,step,clock)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
  @Read
  def read(ctx,address,width):
   if width!=4 or address not in native_registers:errors.append(('read',address,width));return 0
   return native_registers[address]
  @Write
  def write(ctx,address,value,width):
   if width!=4 or address not in native_registers:errors.append(('write',address,width));return
   native_registers[address]=value
  io=IO(None,read,write)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)]
  s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65f2a4
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f,mmio=hardware)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call,ctypes.byref(io))==0 and not errors
  assert native==ram and native_registers==hardware and native_registers.trace==hardware.trace
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and r[0]==0
  expected={0x18:0,0x14:0,0x10:0,0x8c:0x6155404f,0x80:0xc3500000,4:0x0cff0000}
  for off,value in expected.items():assert dict.__getitem__(native_registers,0xa05f6c00+off)==value
  count+=1
 (out/'analysis/input_device_shutdown_host_verification.json').write_text(json.dumps(dict(cases_passed=count,scope='Native shutdown with recovered timer read and elapsed subtraction; full retail RAM/CPU/FPU/register and register-read trace comparisons. Idle, immediate/delayed completion, timeout, timer wrap and both FPU banks. Independent final control-register values. No injected game services. Timer and transport registers use a synthetic host model.'),indent=2)+'\n')
 print(count,'native device shutdown comparisons passed')

if __name__=='__main__':main()
