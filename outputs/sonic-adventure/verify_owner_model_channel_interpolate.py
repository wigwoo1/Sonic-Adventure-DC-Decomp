import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ROOT/'work/sonic-adventure/build/owner_model_channel_interpolate.dll';fn=ctypes.CDLL(str(dll)).sa_owner_model_channel_interpolate;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8)]+[ctypes.POINTER(ctypes.c_uint32)]*3+[ctypes.c_uint32]*6+[ctypes.POINTER(ctypes.c_uint32)];fn.restype=ctypes.c_int;cases=0
 for times,phase,mode,bank,values in itertools.product([[0,4,8]],[0.25,1.0,2.0,3.5,4.25,6.0,7.5,8.25,10.0,11.5],range(2),range(2),[[0.0,4.0,8.0],[-16.0,4.0,64.0],[1.0000001192092896,-1.0,0.0],[1e20,-1e20,1e-20]]):
  for m in [old,native]:
   m[0xa00000:0xa03000]=bytes([0xa5])*0x3000;m[0xf00000:0xf00100]=bytes([0xa5])*256
   struct.pack_into('<f',m,0xa00010,phase);struct.pack_into('<3I',m,0xa00100,0x8ca01000,0,len(times));struct.pack_into('<I',m,0xf0000c,0x8ca02000)
   for i,t in enumerate(times):struct.pack_into('<I3f',m,0xa01000+i*16,t,values[i],-values[i],values[(i+1)%3])
  lower=int(phase)//4;upper=(lower+1)%3
  for m in [old,native]:
   struct.pack_into('<I',m,0xf00004,0x8ca01000+lower*16);struct.pack_into('<I',m,0xf0001c,0x8ca00100);struct.pack_into('<I',m,0xa00104,12)
   m[0xa02004:0xa02010]=m[0xa01004+lower*16:0xa01010+lower*16]
  state=State();state.r[10]=0x8ca00100;state.r[12]=0x8ca00000;state.r[14]=0;state.r[15]=0x8cf00000;state.r[4]=upper;state.r[5]=lower;state.r[6]=0x8ca01000;state.pc=0x8c079e10;state.pr=0x8c000100;fpu=Fpu();fpu.fpscr=0x40000|mode|(bank<<21);hit=[]
  def stop(address):
   def invoke(memory,r,s):hit.append(address);state.pr=0x8c000100;return s
   return invoke
  reference(old,state,{a:stop(a) for a in [0x8c079eb8]},fpu)
  sr=ctypes.c_uint32(0);fpscr=ctypes.c_uint32(0x40000|mode|(bank<<21));fpul=ctypes.c_uint32();out=(ctypes.c_uint32*1)()
  assert fn(view,ctypes.byref(sr),ctypes.byref(fpscr),ctypes.byref(fpul),0x8ca00000,0x8ca00100,0x8cf00000,0x8ca01000,lower,upper,out)==0
  assert out[0]==state.r[0]
  assert sr.value==state.sr and fpscr.value==fpu.fpscr and fpul.value==fpu.fpul
  assert old[0xa00000:0xa03000]==native[0xa00000:0xa03000] and old[0xf00000:0xf00100]==native[0xf00000:0xf00100];cases+=1
 outdir=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((outdir/'source/owner_model_channel_interpolate.c').read_bytes()).hexdigest(),scope='First-channel interpolation with native divide/add/sub/FMAC, native unsigned conversion and arithmetic. Three-key interior and wrap segments, rounding modes and banks, rounding modes/banks. RAM, indices, key pointer, next branch, SR/FPSCR/FPUL. Following record processing stop hook; full worker incomplete.')
 (outdir/'analysis/owner_model_channel_interpolate_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} channel interpolation comparisons passed')
if __name__=='__main__':main()
