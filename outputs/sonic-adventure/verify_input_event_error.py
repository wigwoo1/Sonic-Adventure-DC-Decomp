import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_event_error.dll')).sa_input_event_error
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 @Call
 def call(*args):return -9
 for channel,present,fill,bank in itertools.product(range(8),[0,1,255],[0,0x5a,255],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;record=0x89c550+channel*0x148
  ram[record:record+0x148]=bytes([fill])*0x148;ram[record+2]=present;before=bytes(ram[record:record+0x148])
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c64850a
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  wanted=bytearray(before)
  if present:wanted[4:13]=bytes(9)
  assert native[record:record+0x148]==wanted and r[0]==(0 if present else 0xffffff01) and macl.value==channel*0x148
  count+=1
 report=dict(cases_passed=count,instruction_count=21,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_event_error.c').read_bytes()).hexdigest(),scope='All eight channels, three presence values, three initial record patterns and both FPU banks. Full RAM/register/FPU comparison and independent cleared-field and return checks; no injected services.')
 (out/'analysis/input_event_error_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'event error comparisons passed')
if __name__=='__main__':main()
