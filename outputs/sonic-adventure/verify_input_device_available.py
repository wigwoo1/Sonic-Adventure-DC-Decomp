import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_device_available.dll')).sa_input_device_available;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for channel,present,busy,flags,bank in itertools.product(range(8),[0,1,255],[0,1,0xffffffff],[0,1,2,3],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;struct.pack_into('<I',ram,0x6724f4+channel*4,7-channel);ram[0xa00000]=present;struct.pack_into('<I',ram,0xa01000,flags)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);expected=[];actual=[];errors=[]
  def effect(r,t,pr):
   trace=(t,tuple(r[i] for i in range(4,8)),r[15],pr);r[0]={0x8c646856:0x8ca00000,0x8c646868:0x8ca01000,0x8c646b36:busy}[t];return trace
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   if t not in [0x8c646856,0x8c646868,0x8c646b36]:errors.append(hex(t));return -9
   actual.append(effect(r,t,pr[0]));return 0
  def hook(t):
   def body(mem,r,sr):expected.append(effect(r,t,s.pr));return sr
   return body
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=channel;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c660dec
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();reference(ram,s,{t:hook(t) for t in [0x8c646856,0x8c646868,0x8c646b36]},f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and actual==expected and bytes(nf)==bytes(f)
  assert r[0]==int(bool(present and not busy and flags&2)) and [x[0] for x in actual]==[0x8c646856,0x8c646868]+([0x8c646b36] if present else [])
  assert all(x[1][0]==7-channel for x in actual);count+=1
 report=dict(cases_passed=count,source_sha256=hashlib.sha256((out/'source/sa_input_device_available.c').read_bytes()).hexdigest(),scope='Retail full state/call comparison across eight remapped channels, presence bytes, busy results, capability flags and FPU banks. Independent result/call-order checks. Three subordinate services injected.')
 (out/'analysis/input_device_available_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'device availability comparisons passed')
if __name__=='__main__':main()
