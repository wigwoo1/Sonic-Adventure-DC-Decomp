import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_context_parameters_initialize.dll')).sa_engine_context_parameters_initialize
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 cases=0
 for fill,bank,mask in itertools.product([0,0x55,0xff],[0,0x200000],[0,15]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  ram[0x8a2df8:0x8a2e78]=bytes([fill])*128
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=0x8c66120c
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert struct.unpack_from('<2I',native,0x8a2df8)==(0x80800000,0x20800000)
  assert struct.unpack_from('<I',native,0x8a2f00)[0]==0x01000000
  assert struct.unpack_from('<I',native,0x8a2f04)[0]==0x3727c5ac
  cases+=1
 (out/'analysis/engine_context_parameters_initialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,recovered_instructions=48,scope='Complete retail RAM/CPU/FPU comparisons; independent initial parameter words and final control values. No injected services. Full engine context initialization remains unconnected.'),indent=2)+'\n')
 print(cases,'interrupt defaults comparisons passed')
if __name__=='__main__':main()
