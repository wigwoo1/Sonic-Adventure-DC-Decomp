import ctypes,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
lib=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/transfer_status_getters.dll'))
abi=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
cases=0
for name,entry,offset in [('first',0x8c10d428,0xec),('second',0x8c10d444,0xdc)]:
 fn=getattr(lib,'sa_transfer_status_'+name+'_get');fn.argtypes=abi
 for index,value,bank in itertools.product(range(4),[0,0x12345678,0xffffffff],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<I',ram,0x5fbb14,0x8cb09000)
  struct.pack_into('<I',ram,0xb09000+offset+index*4,value)
  ram[0xb10000:0xb1000c]=bytes([0x99])*12
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(t);return -9
  s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=index;s.r[5]=0x8cb10004;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=entry
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f)
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0 and not errors
  assert ram==native and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert native[0xb10000:0xb1000c]==bytes([0x99])*4+struct.pack('<I',value)+bytes([0x99])*4
  assert r[0]==0 and r[15]==0x8cf00000
  cases+=1
report={'cases_passed':cases,'instructions_recovered':20,'scope':'Both transfer context counter getters: full original RAM/CPU/SR/PR/FPU equality, indices0..3, zero/pattern/all-one counters and both FPU banks. Independent output word and adjacent guards; no modeled calls. Invalid indices or null context not checked.'}
(out/'analysis/transfer_status_getters_verification.json').write_text(json.dumps(report,indent=2)+'\n')
print(cases,'transfer status getter comparisons passed')
