import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

Read=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint)
Write=ctypes.CFUNCTYPE(None,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint32,ctypes.c_uint)
class IO(ctypes.Structure):
 _fields_=[("user",ctypes.c_void_p),("read",Read),("write",Write)]
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_service_shutdown_host.dll')).sa_input_service_start_contiguous_host
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call,ctypes.POINTER(IO),ctypes.c_uint32];cases=0
 for count,bank,align,revision in itertools.product([0],[0,0x200000],[0,1,31],[0,1,2,255]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x67c004,0x8cad0000)
  struct.pack_into('<I',ram,0x8a83b4,0x8ca00000)
  struct.pack_into('<I',ram,0x89c54c,0x8ca10000)
  ram[0xa10000:0xa10200]=bytes([0x55])*512
  struct.pack_into('<2I',ram,0xa20000,0,3);struct.pack_into('<2I',ram,0xa30000,0x8ca50000+align,0x8ca60000+align)
  hardware={0xa05f6920:0,0xa05f689c:revision,**{0xa05f6c00+off:0 for off in [0,0x10,0x14,0x80,0x8c]}}
  host_registers=dict(hardware);accesses=[];errors=[]
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  @Read
  def io_read(ctx,addr,width):
   accesses.append(('read',addr,width))
   if width!=4 or addr not in host_registers:errors.append(('read',addr,width));return 0
   return host_registers[addr]
  @Write
  def io_write(ctx,addr,value,width):
   accesses.append(('write',addr,width,value))
   if width!=4 or addr not in host_registers:errors.append(('write',addr,width));return
   host_registers[addr]=value
  io=IO(None,io_read,io_write)
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   errors.append(hex(t));return -9
  def hook(mem,r,sr):r[0]=0;return sr
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8ca50000+align;s.r[5]=0x8ca40000+align;s.r[6]=0;s.r[7]=0;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c646304
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f,mmio=hardware,vbr=0x8ca08000)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call,ctypes.byref(io),0x8ca08000)==0
  assert not errors,errors
  assert host_registers==hardware and accesses
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert struct.unpack_from('<I',native,0x8a83b4)[0]==0x8c8a72b8
  base=0x8a72b8
  assert struct.unpack_from('<2I',native,base+0xfc4)==(0xac000000+((0xa40000+align+31)&~31),0xac000000+((0xa50000+align+31)&~31))
  assert struct.unpack_from('<I',native,base+0xfc0)[0]==1
  assert struct.unpack_from('<I',native,0x894090)[0]==1
  assert host_registers[0xa05f6920]==0x3000
  shutdown=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_service_shutdown_host.dll')).sa_input_device_service_shutdown_host
  shutdown.argtypes=fn.argtypes
  hardware[0xffd8000c]=0xffffffff;host_registers[0xffd8000c]=0xffffffff
  for offset in [4,0x18]:hardware[0xa05f6c00+offset]=0;host_registers[0xa05f6c00+offset]=0
  s.pc=0x8c6467f0;s.pr=0x8c000100;pr.value=s.pr
  reference(ram,s,fpu=f,mmio=hardware,vbr=0x8ca08000)
  assert shutdown(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call,ctypes.byref(io),0x8ca08000)==0 and not errors
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and host_registers==hardware
  assert native[0x893bd0:0x893bd0+576]==bytes(576)
  assert host_registers[0xa05f6920]==0
  for event in [0xa00,0xa20]:assert struct.unpack_from('<I',native,0x893840+(event>>4)*4)[0]==0
  cases+=1
 (out/'analysis/input_service_contiguous_host_verification.json').write_text(json.dumps(dict(cases_passed=cases,instruction_count=159,scope='Recovered contiguous-buffer initialization executes natively with recovered lower helpers and explicit host register callbacks. Complete retail RAM/CPU/FPU and final register comparisons across revisions 0/1/2/255, alignments 0/1/31 and both FPU banks. Independent rounded uncached buffer addresses and enabled-state assertions. No injected game calls. Outer service startup, native registration, status publication and connected shutdown are covered. Independent empty published table, interrupt mask and registration-head checks after shutdown. Exception-table base and hardware behavior remain synthetic fixtures.'),indent=2)+'\n')
 print(cases,'packet mode comparisons passed')
if __name__=='__main__':main()


