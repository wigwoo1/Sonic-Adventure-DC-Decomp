import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll')).sa_input_work_pool_initialize_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for fill,bank in itertools.product([0,0x5a,255],[0,0x200000]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0x8d6938:0x8e9dd8]=bytes([fill])*0x134a0;struct.pack_into('<3I',ram,0x8e9dd8,0xabcdef01,0xabcdef02,0xabcdef03)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);f=Fpu();f.fpscr=bank;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c668b38
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();mach=ctypes.c_uint32();gbr=ctypes.c_uint32();errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call)==0
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert native[0x8d6938:0x8e9dd8]==bytes(0x134a0) and struct.unpack_from('<3I',native,0x8e9dd8)==(0,0,0)
  count+=1
 report=dict(cases_passed=count,instruction_count=16,binary_sha256=hashlib.sha256(binary).hexdigest(),source_sha256={n:hashlib.sha256((out/'source'/(n+'.c')).read_bytes()).hexdigest() for n in ['sa_input_work_pool_initialize','sa_memory_fill','input_frame_context_native']},scope='Full retail CPU/RAM/FPU comparisons; entire 79008-byte pool and three scheduler globals cleared from three initial patterns, both FPU banks. No injected services. Subsequent object/work creation lifecycle remains to be connected in one test.')
 (out/'analysis/input_work_pool_initialize_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'native work pool comparisons passed')
if __name__=='__main__':main()
