import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ROOT/'work/sonic-adventure/build/owner_model_channel_select.dll';fn=ctypes.CDLL(str(dll)).sa_owner_model_channel_select;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8)]+[ctypes.POINTER(ctypes.c_uint32)]*3+[ctypes.c_uint32]*3+[ctypes.POINTER(ctypes.c_uint32)];fn.restype=ctypes.c_int;cases=0
 for times,phase,mode,bank in itertools.product([[0],[0,4],[0,4,8],[0,0x80000000,0xffffffff]],[-1.0,0.0,2.0,4.0,6.0,8.0,10.0,4294967296.0],range(2),range(2)):
  for m in [old,native]:
   m[0xa00000:0xa03000]=bytes([0xa5])*0x3000;m[0xf00000:0xf00100]=bytes([0xa5])*256
   struct.pack_into('<f',m,0xa00010,phase);struct.pack_into('<3I',m,0xa00100,0x8ca01000,0,len(times));struct.pack_into('<I',m,0xf0000c,0x8ca02000)
   for i,t in enumerate(times):struct.pack_into('<4I',m,0xa01000+i*16,t,0x3f800000+i,0x40000000+i,0x40400000+i)
  state=State();state.r[10]=0x8ca00100;state.r[12]=0x8ca00000;state.r[14]=0;state.r[15]=0x8cf00000;state.pc=0x8c079d66;state.pr=0x8c000100;fpu=Fpu();fpu.fpscr=0x40000|mode|(bank<<21);hit=[]
  def stop(address):
   def invoke(memory,r,s):hit.append(address);state.pr=0x8c000100;return s
   return invoke
  reference(old,state,{a:stop(a) for a in [0x8c079e10,0x8c079eb8]},fpu)
  sr=ctypes.c_uint32(0);fpscr=ctypes.c_uint32(0x40000|mode|(bank<<21));fpul=ctypes.c_uint32();out=(ctypes.c_uint32*4)()
  assert fn(view,ctypes.byref(sr),ctypes.byref(fpscr),ctypes.byref(fpul),0x8ca00000,0x8ca00100,0x8cf00000,out)==0
  assert list(out)==[state.r[5],state.r[4],struct.unpack_from('<I',old,0xf00004)[0],hit[0]]
  assert sr.value==state.sr and fpscr.value==fpu.fpscr and fpul.value==fpu.fpul
  assert old[0xa00000:0xa03000]==native[0xa00000:0xa03000] and old[0xf00000:0xf00100]==native[0xf00000:0xf00100];cases+=1
 outdir=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((outdir/'source/owner_model_channel_select.c').read_bytes()).hexdigest(),scope='First-channel keyframe selection and lower-key copy, native unsigned conversion and arithmetic. Single/multiple keys, unsigned timestamp boundaries, phase boundaries and wrap, rounding modes/banks. RAM, indices, key pointer, next branch, SR/FPSCR/FPUL. Interpolation and following record processing stop hooks; full worker incomplete.')
 (outdir/'analysis/owner_model_channel_select_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} keyframe selection comparisons passed')
if __name__=='__main__':main()
