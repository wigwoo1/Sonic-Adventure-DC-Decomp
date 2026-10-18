import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_state_native.dll'));count=0
 @Call
 def call(*args):return -9
 for channel,value,bank,mark in itertools.product(range(8),[0,1,128,255],[0,0x200000],[0,1]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0x8a83b4,0x8ca00000);record=0xa00028+44*channel;ram[record+12]=value;ram[record+13]=value
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c646b3c if mark else 0x8c646b36
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();reference(ram,s,fpu=f)
  fn=getattr(lib,'sa_input_device_mark_native' if mark else 'sa_input_device_busy_native');fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert r[0]==(1 if mark else value) and native[record+13]==(1 if mark else value) and macl.value==channel*44
  count+=1
 report=dict(cases_passed=count,instruction_count=30,scope='Busy read and request mark wrappers plus record pointer execute natively without injected services. Full retail state comparisons and independent byte/return/MACL checks across eight channels, byte values and FPU banks.')
 (out/'analysis/input_device_state_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'device state comparisons passed')
if __name__=='__main__':main()
