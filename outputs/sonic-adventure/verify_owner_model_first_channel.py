import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ROOT/'work/sonic-adventure/build/owner_model_first_channel.dll';call=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,*([ctypes.c_uint32]*5),ctypes.POINTER(ctypes.c_uint32));fn=ctypes.CDLL(str(dll)).sa_owner_model_first_channel;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32)]+[ctypes.POINTER(ctypes.c_uint32)]*2+[ctypes.c_uint32]*6+[ctypes.c_void_p,call,ctypes.POINTER(ctypes.c_uint32)];cases=0
 for current,previous,success,index,sr0,phase,mode in itertools.product([0,0x8ca01000],[0,0x8ca01100],[False,True],[0,1,7],[0,0x700083f3],[0.0,2.0,6.0,10.0],range(2)):
  for mem in [old,native]:
   mem[0xa00000:0xa04000]=bytes([0xa5])*0x4000;mem[0xf00000:0xf00100]=bytes([0xa5])*256
   struct.pack_into('<I',mem,0xa00000,current);struct.pack_into('<I',mem,0xa00100,previous);struct.pack_into('<I',mem,0xf00028,index);struct.pack_into('<I',mem,0xf00034+index*4,0x8ca03000)
   struct.pack_into('<f',mem,0xa00310,phase);struct.pack_into('<I',mem,0xa00108,3);struct.pack_into('<I',mem,0xa00104,12);struct.pack_into('<I',mem,0xf0001c,0x8ca00100)
   for i in range(3):struct.pack_into('<I3f',mem,0xa01100+i*16,i*4,float(i*8-4),float(16-i*4),float(i+1))
   struct.pack_into('<I3f',mem,0xa01000,0,1.0,-2.0,4.0);struct.pack_into('<3f',mem,0xa03008,8.0,16.0,32.0)
  regs=State();regs.r[12]=0x8ca00300;regs.r[8]=2;regs.r[14]=0;regs.r[9]=0x8ca00000;regs.r[10]=0x8ca00100;regs.r[11]=0x8ca00200;regs.r[15]=0x8cf00000;regs.r[0]=255;regs.pc=0x8c079d26;regs.pr=0x8c000100;regs.sr=sr0;events=[];hit=[];fpu=Fpu();fpu.fpscr=0x40000|mode
  def allocate(memory,r,s):events.append((r[4],r[0]));r[0]=0x8ca02000 if success else 0;return s^0x01000030
  def stop(address):
   def invoke(memory,r,s):hit.append(address);regs.pr=0x8c000100;return s
   return invoke
  reference(old,regs,{0x8c098a80:allocate,**{a:stop(a) for a in [0x8c079f18]}},fpu)
  native_events=[]
  @call
  def alloc(ctx,address,argc,a,b,r0,s):assert address==0x8c098a80 and argc==1;native_events.append((a,r0));s[0]^=0x01000030;return 0x8ca02000 if success else 0
  status=ctypes.c_uint32(sr0);fpscr=ctypes.c_uint32(0x40000|mode);fpul=ctypes.c_uint32();out=(ctypes.c_uint32*3)();result=fn(view,ctypes.byref(status),ctypes.byref(fpscr),ctypes.byref(fpul),0x8ca00300,0x8cf00000,0x8ca00000,0x8ca00100,0x8ca00200,255,None,alloc,out)
  assert result==0 and fpscr.value==fpu.fpscr and fpul.value==fpu.fpul
  assert list(out)==[regs.r[13],regs.r[0],hit[0]] and status.value==regs.sr and events==native_events
  assert old[0xa00000:0xa04000]==native[0xa00000:0xa04000] and old[0xf00000:0xf00100]==native[0xf00000:0xf00100];cases+=1
 outdir=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((outdir/'source/owner_model_first_channel.c').read_bytes()).hexdigest(),scope='Integrated first-channel allocation, selection, interpolation and default-copy path. Source presence, allocation failure/success, record indices and callback SR mutation; buffer/slot/frame RAM, descriptor, r0/SR, next branch and allocation arguments. Only allocator and next-channel entry hooked. Full worker and other channels unrecovered.')
 (outdir/'analysis/owner_model_first_channel_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} first channel allocation comparisons passed')
if __name__=='__main__':main()
