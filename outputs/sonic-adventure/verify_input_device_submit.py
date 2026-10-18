import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_submit_native.dll')).sa_input_device_submit_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for bank,enabled,busy,locked,channels,rate in itertools.product([0,0x200000],[0,1],[0,1],[0,128],[0,1,24],[None,0,60]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;base=0xa10000
  struct.pack_into('<I',ram,0x8a83b4,0x8ca10000)
  struct.pack_into('<I',ram,0x6607f4,0x8ca70000);ram[0xa70000]=locked
  for off,val in [(0xfc0,enabled),(0x10f4,0x8ca40000),(0x10a8,0x8ca50000),(0x10ac,0x8ca60000),(0x10b4,0),(0xfcc,0x8ca50010),(0xfd8,5),(0xfdc,20),(0x10b8,0),(0x10c8,0),(0xfac,0 if rate is None else rate),(0xfb0,0 if rate is None else 1)]:
   struct.pack_into('<I',ram,base+off,val)
  struct.pack_into('<I',ram,0xa40018,busy)
  for ch in range(24):
   rec=base+0x28+ch*44
   ram[base+0xf90+ch]=int(ch<channels);ram[rec+6]=10
   struct.pack_into('<I',ram,rec+24,7+ch);struct.pack_into('<I',ram,rec+28,99)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)]
  s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65efda
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();special={'mach':0,'macl':0,'gbr':0}
  reference(ram,s,fpu=f,special=special)
  case=(bank,enabled,busy,locked,channels,rate)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors,(case,errors)
  assert native==ram,(case,'RAM')
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl'],(case,'CPU/FPU/MACL')
  success=enabled and not busy and not locked
  assert r[0]==(0 if success else (0xfffffffd if not enabled else 0xffffffff if busy else 0xfffffffe))
  if success:
   assert struct.unpack_from('<I',native,0xa40004)[0]==0x0ca50000
   assert struct.unpack_from('<I',native,base+0xfcc)[0]==0x8ca60000
  for ch in range(24):
   rec=base+0x28+ch*44;moved=success and ch<channels
   assert struct.unpack_from('<I',native,rec+24)[0]==(0 if moved else 7+ch)
   assert struct.unpack_from('<I',native,rec+28)[0]==(7+ch if moved else 99)
   assert native[rec+6]==(0 if moved else 10) and native[rec+7]==(10 if moved else 0)
  count+=1
 (out/'analysis/input_device_submit_verification.json').write_text(json.dumps(dict(cases_passed=count,scope='Full retail RAM/CPU/FPU/MACL comparisons; independent submission token/command transitions and return codes. Enabled/disabled, hardware busy, software lock, 0/1/24 selected channels, rate changes and FPU banks. No injected calls in these cases. Transport register block uses RAM fixture. Automatic command generation for other device states and enabled error formatting/callbacks remain unverified.'),indent=2)+'\n')
 print(count,'native device submission comparisons passed')

if __name__=='__main__':main()
