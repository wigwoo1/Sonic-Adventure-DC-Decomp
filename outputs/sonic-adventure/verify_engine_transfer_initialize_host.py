import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
from verify_engine_cache_writeback import IO,Read,Write

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_initialize_host.dll')).sa_engine_transfer_initialize_host
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call,ctypes.POINTER(IO),ctypes.c_uint32]
 cases=0
 for fill,bank,mask in itertools.product([0,0x55],[0,0x200000],[0,7,15]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  ram[0x7ac7a0:0x7ac8b4]=bytes([fill])*0x114
  struct.pack_into('<I',ram,0x67c004,0x8cad0000)
  registers={0xa05f6920:0xffffffff,**{0xa05f7800+o:0 for o in range(0,0xc0,4)}}
  native_registers=dict(registers)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Read
  def read(ctx,a,n):
   if a not in native_registers or n!=4:errors.append(('read',a,n));return 0
   return native_registers[a]
  @Write
  def write(ctx,a,v,n):
   if a not in native_registers or n!=4:errors.append(('write',a,v,n))
   native_registers[a]=v
  io=IO(None,read,write)
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=0x8c10d5a8;s.r[4]=0x8ca00000;s.r[5]=0x8ca00100
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  special={'macl':0};reference(ram,s,fpu=f,mmio=registers,vbr=0x8ca08000,special=special)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call,ctypes.byref(io),0x8ca08000)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert native_registers==registers and macl.value==special['macl']
  assert r[0]==0 and struct.unpack_from('<I',native,0x5fbb14)[0]==0x8c7ac7a0
  assert struct.unpack_from('<I',native,0x7ac7a0)[0]==0xa05f7800
  for channel in range(4):
   slot=0x7ac7a4+channel*52
   assert struct.unpack_from('<I',native,slot)[0]==channel
   assert struct.unpack_from('<I',native,slot+24)[0]==3
   assert struct.unpack_from('<I',native,slot+36)[0]!=0
   assert native_registers[0xa05f7800+channel*32+20]==0
  assert native_registers[0xa05f78bc]==0x4659404f
  cases+=1
 (out/'analysis/engine_transfer_initialize_host_verification.json').write_text(json.dumps(dict(cases_passed=cases,recovered_instructions=312,scope='Native transfer subsystem initializer with memory fill, descriptor resets/address window and IRQ registration/handler setup. Full retail RAM/CPU/FPU/MACL/MMIO comparisons; independent fixed table, four descriptor indices/states/registration handles, hardware channel clearing and address window checks. Engine context pointer and register values remain fixtures. No actual transfer execution or IRQ delivery; full game boot not implemented.'),indent=2)+'\n')
 print(cases,'transfer initialization comparisons passed')
if __name__=='__main__':main()

