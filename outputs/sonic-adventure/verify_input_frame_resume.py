import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P
def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_frame_context_native.dll')).sa_input_frame_resume_native
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,P,P,ctypes.c_void_p,Call];count=0
 for entry,bank,tick,jobs in itertools.product([0x8c668e92,0x8c668edc,0x8c668ef2],[0,0x200000],[0,0xffffffff],[0,1]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
  struct.pack_into('<2I',ram,0xa00204,tick,jobs);struct.pack_into('<2I',ram,0x8e9dd8,0x8ca00000,0x8cab0000)
  ram[0xa00210:0xa00334]=bytes(0x124);stack=[bank,0x8c000100]+[0x22340000+i for i in range(9,15)];struct.pack_into('<8I',ram,0xefffe0,*stack)
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);f=Fpu();f.fpscr=0x40001;nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[9]=0x8c8e9dd8;s.r[10]=0x8c1108e0;s.r[11]=3;s.r[12]=0;s.r[13]=0x8ca00000;s.r[14]=0x8ca00210;s.r[15]=0x8cefffe0;s.sr=0x70000063;s.pr=entry;s.pc=entry
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();mach=ctypes.c_uint32();gbr=ctypes.c_uint32();errors=[]
  @Call
  def call(ctx,t,m,f,sr,r,pr):errors.append(hex(t));return -9
  reference(ram,s,fpu=f,stop_pc=0x8c000100)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(mach),ctypes.byref(gbr),None,call)==0
  assert not errors and native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  assert r[15]==0x8cf00000 and pr.value==0x8c000100 and nf.fpscr==bank
  assert struct.unpack_from('<I',native,0xa00204)[0]==(tick+1)&0xffffffff and struct.unpack_from('<I',native,0x8e9dd8)[0]==0x8cab0000
  count+=1
 report=dict(cases_passed=count,source_sha256={n:hashlib.sha256((out/'source'/(n+'.c')).read_bytes()).hexdigest() for n in ['sa_input_frame_manager','input_frame_context_native']},scope='Three manager continuation entry points compared with retail instructions; empty/one idle record, counter wrap, restored stack/FPSCR/context and caller return. No injected calls. Full suspend/restore/resume sequence and general continuations still unfinished.')
 (out/'analysis/input_frame_resume_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'frame continuation comparisons passed')
if __name__=='__main__':main()
