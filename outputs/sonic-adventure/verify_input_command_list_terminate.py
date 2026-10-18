import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_command_list_terminate.dll')).sa_input_command_list_terminate
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for mode,bank in itertools.product([0,1,2,3,0x80000000,0xffffffff],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x8a83b4,0x8ca00000)
  struct.pack_into('<I',ram,0x8a83b4,0x8ca10000)
  ram[0xa10000:0xa10200]=bytes([0x55])*512
  struct.pack_into('<I',ram,0xa10fcc,0x8ca20000);struct.pack_into('<I',ram,0xa20000,mode)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  @Call
  def call(ctx,t,m,f,sr,r,pr):raise AssertionError(hex(t))
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=mode;s.r[5]=0x8ca30000;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c660794
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert struct.unpack_from('<I',native,0xa20000)[0]==(mode|0x80000000)
  assert struct.unpack_from('<I',native,0xa10fcc)[0]==0x8ca20000 and r[0]==0
  count+=1
 (out/'analysis/input_command_list_terminate_verification.json').write_text(json.dumps(dict(cases_passed=count,instruction_count=10,scope='Full retail RAM/CPU/FPU comparison; Native command-list terminator full retail RAM/CPU/FPU comparison with independent high-bit preservation, current-buffer pointer and return checks. No injected calls. RAM-backed command buffer tested.'),indent=2)+'\n')
 print(count,'packet mode comparisons passed')
if __name__=='__main__':main()

