import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_packet_mode_set.dll')).sa_input_packet_mode_set
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for mode,bank in itertools.product([0,1,2,3,0x80000000,0xffffffff],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x8a83b4,0x8ca00000)
  struct.pack_into('<I',ram,0x89c54c,0x8ca10000)
  ram[0xa10000:0xa10200]=bytes([0x55])*512
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  @Call
  def call(ctx,t,m,f,sr,r,pr):raise AssertionError(hex(t))
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=mode;s.r[5]=0x8ca30000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c647de8
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert struct.unpack_from('<I',native,0xa10000)[0]==mode&1
  assert struct.unpack_from('<2I',native,0xa1000c)==((0x8c66080c,0x8c66081e) if mode&1 else (0x8c660816,0x8c660826))
  count+=1
 (out/'analysis/input_packet_mode_verification.json').write_text(json.dumps(dict(cases_passed=count,instruction_count=23,scope='Full retail RAM/CPU/FPU comparison; Native packet mode setter; independent mode masking and retail read/write function pointer selection. No injected calls.'),indent=2)+'\n')
 print(count,'packet mode comparisons passed')
if __name__=='__main__':main()

