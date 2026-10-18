import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll')).sa_input_work_object_create_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for busy,capacity,bank,mask in itertools.product(range(9),[0,32],[0,0x200000],[0,15]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0x8d6938:0x8e9dd8]=bytes([0x5a])*0x134a0
  for i in range(8):struct.pack_into('<I',ram,0x8d6938+i*0x2694,1 if i<busy else 0)
  struct.pack_into('<I',ram,0x8e9de0,busy);native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
  f=Fpu();f.fpscr=bank;f.fpul=0xabcdef01;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=capacity;s.r[15]=0x8cf00000;s.sr=0x70000003|(mask<<4);s.pr=0x8c000100;s.pc=0x8c668b58
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32(0x33334444);mach=ctypes.c_uint32(0x11112222);gbr=ctypes.c_uint32(0x55556666);errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
  reference(ram,s,fpu=f,special=dict(mach=mach.value,macl=macl.value,gbr=gbr.value))
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call)==0
  nf.fpul=fpul.value
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert r[0]==(0x8c8d6938+busy*0x2694 if busy<8 else 0) and ((sr.value>>4)&15)==mask
  assert struct.unpack_from('<I',native,0x8e9de0)[0]==busy+(busy<8)
  if busy<8:
   obj=0x8d6938+busy*0x2694
   assert struct.unpack_from('<I',native,obj)[0]==1 and struct.unpack_from('<3I',native,obj+0x204)==(0,capacity,0)
   assert native[obj+0x210:obj+0x2690]==bytes(0x2480) and struct.unpack_from('<I',native,obj+0x10c)[0]==0x8c668bc4
  count+=1
 report=dict(cases_passed=count,instruction_count=75,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256=hashlib.sha256((out/'source/sa_input_work_object_create.c').read_bytes()).hexdigest(),scope='Native object creation, fill and context save against retail instructions; first free slot, eight-slot exhaustion, capacity/counter initialization, cleared ranges and interrupt-mask restoration checked. No injected services. Object pool initialization and complete game work lifecycle unfinished.')
 (out/'analysis/input_work_object_create_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'native work object comparisons passed')
if __name__=='__main__':main()

