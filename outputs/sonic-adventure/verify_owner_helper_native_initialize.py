import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 call=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,*([ctypes.c_uint32]*5),ctypes.POINTER(ctypes.c_uint32));fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/owner_helper_native_model.dll')).sa_owner_helper_with_native_model;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8)]+[ctypes.POINTER(ctypes.c_uint32)]*3+[ctypes.c_uint32]*2+[ctypes.c_void_p,call,ctypes.POINTER(ctypes.c_uint32)];cases=0
 for index,previous,capacity,sr0,rm,bank in itertools.product([0,1,7],[0,1,7],[64,80,512,2752,4096],[0,0x700083f3],range(2),range(2)):
  for m in [old,native]:
   m[0xa00000:0xa20000]=bytes([0xa5])*0x20000
   struct.pack_into('<8H',m,0xa00000,0,0,index,previous,0,0,13,0xffff);struct.pack_into('<I',m,0xa00020,0x8ca01000)
   struct.pack_into('<I',m,0xa01000+index*16,0x8ca02000);struct.pack_into('<I',m,0xa02000,0x8ca06000)
   head=0x8ca10000;end=head+capacity;struct.pack_into('<4I',m,0xa10000,end,0,end,0);struct.pack_into('<4I',m,0xa10000+capacity,0,head,0,head);struct.pack_into('<I',m,0x79f03c,head)
  state=State();state.r[4]=0x8ca00000;state.r[15]=0x8cf00000;state.pc=0x8c079476;state.pr=0x8c000100;state.sr=sr0;state.r[8:15]=[0x12340000+i for i in range(7)]
  fpu=Fpu();fpu.fpscr=0x40000|rm|(bank<<21);fpu.banks[bank][14]=0x87654321;fpu.banks[bank][15]=0x3f800000;reference(old,state,{},fpu)
  calls=[]
  @call
  def fallback(*args):calls.append(args[1]);return 0
  sr=ctypes.c_uint32(sr0);fpscr=ctypes.c_uint32(0x40000|rm|(bank<<21));fpul=ctypes.c_uint32();r0=ctypes.c_uint32();result=fn(view,ctypes.byref(sr),ctypes.byref(fpscr),ctypes.byref(fpul),0x8ca00000,0x8cf00000,None,fallback,ctypes.byref(r0))
  case=(index,previous,capacity,sr0,rm,bank)
  assert not calls and result==0 and r0.value==state.r[0] and sr.value==state.sr and fpscr.value==fpu.fpscr and fpul.value==fpu.fpul,case
  assert list(state.r[8:15])==[0x12340000+i for i in range(7)] and state.r[15]==0x8cf00000 and fpu.banks[bank][14]==0x87654321 and fpu.banks[bank][15]==0x3f800000,case
  assert native[0xa00000:0xa20000]==old[0xa00000:0xa20000] and native[0x79f03c:0x79f040]==old[0x79f03c:0x79f040],case;cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/owner_helper_native_initialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/owner_helper_native_model.c').read_bytes()).hexdigest(),scope='Parent state 0 through return, native allocator and zero-fill allocation, no original routine hooks. Current/previous indices, heap exact fits and payload failure, SR, both FR banks and rounding modes; entire game/heap fixture RAM and r0/SR/FPSCR/FPUL compared. Original callee preservation checked. Header/holder allocation failure corrupts original null address and is outside bounded-RAM fixture. Nested stack scratch outside C interface.'),indent=2)+'\n');print(f'{cases} native state-0 initialization comparisons passed')
if __name__=='__main__':main()
