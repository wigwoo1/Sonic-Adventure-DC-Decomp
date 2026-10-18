import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll')).sa_input_device_available_native;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for channel,present,busy,flags,bank in itertools.product(range(8),[0,1,255],[0,1,0xffffffff],[0,1,2,3],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0x6724f4+channel*4,7-channel);ram[0xa00000]=present;struct.pack_into('<I',ram,0xa01000,flags)
  mapped=7-channel;struct.pack_into('<I',ram,0x8a83b4,0x8ca00000);ram[0x893bd0+mapped*24]=present;ram[0xa00028+mapped*44+12]=busy&255;struct.pack_into('<I',ram,0xa0044c+mapped*120,flags)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c660dec
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(ctypes.c_uint32()),ctypes.byref(ctypes.c_uint32()),None,call)==0
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert r[0]==int(bool(present and not busy and flags&2));count+=1
 report=dict(cases_passed=count,source_sha256=hashlib.sha256((out/'source/sa_input_device_available.c').read_bytes()).hexdigest(),scope='Retail full RAM/CPU/FPU state comparison across eight remapped channels, presence bytes, busy results, capability flags and FPU banks. Independent boolean result checks. All status/info/busy/pointer routines native; no injected calls. Device tables constructed as fixtures.')
 (out/'analysis/input_device_available_native_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'native device availability comparisons passed')
if __name__=='__main__':main()
