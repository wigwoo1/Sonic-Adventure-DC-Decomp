import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_descriptor_acquire.dll')).sa_engine_transfer_descriptor_acquire
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 cases=0
 for channel,occupied,bank,mask in itertools.product(range(4),[0,1],[0,0x200000],[0,7,15]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x5fbb14,0x8ca00200)
  slot=0xa00204+channel*52
  ram[slot:slot+52]=bytes([0x55])*52
  struct.pack_into('<I',ram,slot,channel)
  struct.pack_into('<I',ram,slot+40,occupied)
  struct.pack_into('<I',ram,0xa00000,0x12345678)
  before_slot=bytes(ram[slot:slot+52])
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=0x8c10ec40;s.r[4]=0x8ca00000;s.r[5]=channel
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  special={'macl':0};reference(ram,s,fpu=f,special=special)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert macl.value==special['macl']
  assert r[0]==(0x90101 if occupied else 0)
  if occupied:
   assert struct.unpack_from('<I',native,0xa00000)[0]==0x12345678
   assert native[slot:slot+52]==before_slot
  else:
   assert struct.unpack_from('<I',native,0xa00000)[0]==0x8c000000+slot
   expected=[channel,0,0,0,0,0,3,0,0,0x55555555,1,0,0]
   assert list(struct.unpack_from('<13I',native,slot))==expected
  cases+=1
 (out/'analysis/engine_transfer_descriptor_acquire_verification.json').write_text(json.dumps(dict(cases_passed=cases,recovered_instructions=133,scope='Full retail RAM/CPU/FPU/MACL comparison for descriptor acquisition across four slots, occupied/free state, interrupt masks and FPU banks. Independent output pointer, initialized descriptor fields and occupied-slot error checks. Channel table is a fixture; allocation here reserves existing fixed slots and does not claim heap allocation or hardware execution.'),indent=2)+'\n')
 print(cases,'descriptor acquisition comparisons passed')
if __name__=='__main__':main()
