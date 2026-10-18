import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 call=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,*([ctypes.c_uint32]*5),ctypes.POINTER(ctypes.c_uint32));fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/owner_helper_native_model.dll')).sa_owner_helper_with_native_model;fn.restype=ctypes.c_int;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8)]+[ctypes.POINTER(ctypes.c_uint32)]*3+[ctypes.c_uint32]*2+[ctypes.c_void_p,call,ctypes.POINTER(ctypes.c_uint32)];cases=0
 for count,cm,pm,presence,success,phase,rm in itertools.product([0,1,3,8],[2,3],[2,3],range(4),[0,48,96,256,4096],[0.0,2.0,10.0,11.0,12.0,16.0],range(2)):
  for m in [old,native]:
   m[0xa00000:0xa20000]=bytes([0xa5])*0x20000;m[0xef0000:0xf01000]=bytes([0xa5])*0x11000
   struct.pack_into('<8H',m,0xa00000,8,0,1,1,0,2,0,0);struct.pack_into('<I',m,0xa01000,0x8ca03000);struct.pack_into('<I',m,0xa00020,0x8ca01000);struct.pack_into('<I',m,0xa00028,0x8ca02000);struct.pack_into('<f',m,0xa00010,phase)
   struct.pack_into('<I',m,0xa01010,0x8ca03000);m[0xa01014]=count;struct.pack_into('<I',m,0xa01020,0x8ca03100);m[0xa01024]=count
   struct.pack_into('<2I',m,0xa03000,0x8ca06000,0x8ca04000);struct.pack_into('<I',m,0xa03104,0x8ca04100)
   struct.pack_into('<2IH',m,0xa04000,0x8ca07000,12,4 if cm==3 else 0);struct.pack_into('<2IH',m,0xa04100,0x8ca08000,12,4 if pm==3 else 0)
   struct.pack_into('<I',m,0xa02004,0x8ca02100);struct.pack_into('<I',m,0xa02100,0x8ca05000)
   for i in range(max(1,count)):
    struct.pack_into('<2I',m,0xa0602c+i*64,0x8ca06000+(i+1)*64 if i+1<count else 0,0)
    struct.pack_into('<3f3i3f',m,0xa06008+i*64,8,16,32,-100,32767,65535,1,2,4)
    for base,mode,present in [(0xa07000,cm,presence&1),(0xa08000,pm,presence&2)]:
     addr=base+i*mode*8
     if mode==2:struct.pack_into('<4I',m,addr,0x8ca09000 if present else 0,0x8ca09100 if present else 0,3,3)
     else:struct.pack_into('<6I',m,addr,0x8ca09000 if present else 0,0x8ca09100 if present else 0,0x8ca09200 if present else 0,3,3,3)
   for i in range(3):
    struct.pack_into('<I3f',m,0xa09000+i*16,i*4,i*8-4,16-i*4,i+1);struct.pack_into('<4I',m,0xa09100+i*16,i*4,100+i*100,32760+i*8,65530+i*8);struct.pack_into('<I3f',m,0xa09200+i*16,i*4,1+i,2+i,4+i)
   head=0x8ca10000;end=head+success
   struct.pack_into('<4I',m,0xa10000,end,0,end,0)
   if success:struct.pack_into('<4I',m,0xa10000+success,0,head,0,head)
   else:struct.pack_into('<4I',m,0xa10000,0,0,0,0)
   struct.pack_into('<I',m,0x79f03c,head)
   struct.pack_into('<I',m,0xa03100,0x8ca06000);struct.pack_into('<2f',m,0xa01018,0.5,0.25);struct.pack_into('<2f',m,0xa01028,0.5,0.25)
  state=State();state.r[4]=0x8ca00000;state.r[15]=0x8cf00000;state.pc=0x8c079476;state.pr=0x8c000100;state.sr=0x700083f3;events=[];state.r[8:15]=[0x12340000+i for i in range(7)];fpu=Fpu();fpu.banks[0][15]=0x3f800000;fpu.fpscr=0x40000|rm
  reference(old,state,{},fpu)
  @call
  def fallback(*args):raise AssertionError('Unexpected fallback')
  sr=ctypes.c_uint32(0x700083f3);fpscr=ctypes.c_uint32(0x40000|rm);fpul=ctypes.c_uint32();r0=ctypes.c_uint32();result=fn(view,ctypes.byref(sr),ctypes.byref(fpscr),ctypes.byref(fpul),0x8ca00000,0x8cf00000,None,fallback,ctypes.byref(r0))
  case=(count,cm,pm,presence,success,phase,rm)
  assert list(state.r[8:15])==[0x12340000+i for i in range(7)] and state.r[15]==0x8cf00000 and state.pr==0x8c000100 and fpu.banks[0][15]==0x3f800000,case
  assert result==0 and sr.value==state.sr and r0.value==state.r[0] and fpscr.value==fpu.fpscr and fpul.value==fpu.fpul,case
  assert native[0x79f03c:0x79f040]==old[0x79f03c:0x79f040],case
  assert native[0xa00000:0xa20000]==old[0xa00000:0xa20000],case
  # Nested integer helper stack scratch is verified separately; compare worker frame.
  # Parent C interface does not expose nested stack scratch.
  cases+=1
 outdir=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((outdir/'source/owner_helper_native_model.c').read_bytes()).hexdigest(),scope='Parent state-8 candidate-descriptor model-transition integration without original routine hooks. Counts 0/1/3/8, independent model formats, all source presence combinations, heap capacities 0/48/96/256/4096, below-threshold and transition phases and rounding modes. Game/heap RAM, r0/SR/FPSCR/FPUL compared; original parent callee-save restoration checked. Nested stack scratch and volatile register outputs outside the C interface. Other parent selectors not covered by this verifier.')
 (outdir/'analysis/owner_helper_native_state8_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} native state-8 model-change comparisons passed')
if __name__=='__main__':main()

