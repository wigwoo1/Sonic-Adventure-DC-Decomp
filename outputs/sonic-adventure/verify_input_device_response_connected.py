import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_response_native.dll')).sa_input_device_response_complete_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 count=0
 for channel,mode,bank,sub,present,command,status,token in itertools.product([0,3],[0,1],[0,0x200000],[0,1],[0,1],[1,4],[0,5,8,255],[0,7]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  base=0xa10000;rec=base+0x28+channel*44;packet=0xa30000
  struct.pack_into('<I',ram,0x8a83b4,0x8ca10000)
  struct.pack_into('<I',ram,base+0x10c4,mode)
  struct.pack_into('<I',ram,base+0x10b4,bank>>21)
  struct.pack_into('<I',ram,base+0xfe8+(1-(bank>>21))*96+channel*4,0x8ca30000)
  ram[0x6724c4+channel]=sub
  ram[base+4:base+12]=bytes([1,0,1,1,0,0,0,0])
  ram[rec]=present;ram[rec+7]=command;ram[rec+4]=9
  ram[rec+12]=1;ram[rec+13]=1
  struct.pack_into('<I',ram,rec+0x1c,token)
  struct.pack_into('<I',ram,rec+0x20,99)
  header=bytes([status,0x20,0x01,3])
  ram[packet:packet+4]=header[::-1] if mode else header
  ram[packet+4:packet+260]=bytes(range(256))
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)]
  s.r[4]=channel;s.r[5]=0;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c65fe14
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  special={'macl':0,'mach':0,'gbr':0}
  reference(ram,s,fpu=f,special=special)
  result=fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
  case=(channel,mode,bank,sub,present,command,status,token)
  assert result==0 and not errors,(case,result,errors)
  assert native==ram,(case,'RAM',next((hex(i),a,b) for i,(a,b) in enumerate(zip(native,ram)) if a!=b))
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f),(case,'CPU/FPU')
  assert macl.value==special['macl'],(case,'MACL')
  assert struct.unpack_from('<I',native,rec+0x1c)[0]==0
  assert struct.unpack_from('<I',native,rec+0x20)[0]==(token or 99)
  count+=1
 (out/'analysis/input_device_response_connected_verification.json').write_text(json.dumps(dict(cases_passed=count,scope='Connected native completion path with primary/subdevice response, presence/header/info decoding, word swap and info initialization. Full retail RAM/CPU/FPU/MACL comparisons, independent submitted/completed token transition; no injected calls. Synthetic response packets; not host-device emulation or full game verification.'),indent=2)+'\n')
 print(count,'connected device response comparisons passed')

if __name__=='__main__':main()
