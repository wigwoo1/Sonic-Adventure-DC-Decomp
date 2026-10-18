import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_channels_arm_native.dll'))
 fn=lib.sa_engine_transfer_channels_arm
 channel_fn=lib.sa_engine_transfer_channel_arm;busy_fn=lib.sa_engine_transfer_busy_get
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 channel_fn.argtypes=busy_fn.argtypes=fn.argtypes
 cases=0
 for bitmap,bank in itertools.product(range(16),[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x5fbb14,0x8ca00200);struct.pack_into('<I',ram,0xa00200,0x8ca00400)
  for channel in range(4):
   struct.pack_into('<I',ram,0xa00204+channel*52,channel)
   struct.pack_into('<I',ram,0xa0021c+channel*52,0x11223344)
   struct.pack_into('<2I',ram,0xa00400+channel*32+24,1 if bitmap&(1<<channel) else 0,0x55667788)
  struct.pack_into('<I',ram,0x7ac8c0,0)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):
   target={0x8c10fa60:channel_fn,0x8c1101f0:busy_fn}.get(t)
   if target:return target(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
   errors.append(t);return -9
  f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c10ee9c;s.r[4]=0x8ca00000;s.r[5]=0x8ca00100
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert r[0]==0 and struct.unpack_from('<I',native,0x7ac8c0)[0]==1
  for channel in range(4):
   active=bool(bitmap&(1<<channel))
   assert struct.unpack_from('<I',native,0xa0021c+channel*52)[0]==(2 if active else 0x11223344)
   assert struct.unpack_from('<I',native,0xa00400+channel*32+28)[0]==(1 if active else 0x55667788)
  cases+=1
 (out/'analysis/engine_transfer_channels_arm_verification.json').write_text(json.dumps(dict(cases_passed=cases,recovered_instructions=71,scope='Full retail RAM/CPU/FPU comparison for all-four-channel arm with native channel helper and busy getter. Independent per-channel armed flag/descriptor state and global flag checks across all 16 busy bitmaps and both FPU banks. Busy channels are marked; idle channels remain unchanged. Python dispatcher routes native helpers. No hardware transfer completion or readiness delivery.'),indent=2)+'\n')
 print(cases,'four-channel arm comparisons passed')
if __name__=='__main__':main()
