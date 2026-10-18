import ctypes,hashlib,itertools,json
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/input_backend_finalize_native.dll')).sa_input_packet_row_copy;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call];count=0
 @Call
 def call(*args):return -9
 for alignment,length,delta in itertools.product(range(4),[4,5,7,8,16,64,256],[-260,-4,-1,0,1,4,260]):
  ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;ram[0xa00000:0xa01000]=bytes(range(256))*16;src=0xa00400+alignment;dst=src+delta;wanted=bytearray(ram)
  for i in range(length//4):wanted[dst+4*i:dst+4*i+4]=wanted[src+4*i:src+4*i+4]
  native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native);f=Fpu();nf=Fpu.from_buffer_copy(f);s=State();s.r[:]=[0x22340000+i for i in range(16)];s.r[4]=0x8c000000+dst;s.r[5]=0x8c000000+src;s.r[6]=length;s.r[15]=0x8cf00000;s.sr=0x70000063;s.pr=0x8c000100;s.pc=0x8c6607fc
  r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32();macl=ctypes.c_uint32();reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  assert native==ram==wanted and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
  count+=1
 report=dict(cases_passed=count,instruction_count=8,source_sha256=hashlib.sha256((out/'source/sa_input_packet_row_copy.c').read_bytes()).hexdigest(),scope='Retail full-state and independent forward-word-copy comparison across alignment, overlap and lengths >=4 including nonmultiples of four. No injected services. Lengths below four cause retail count underflow and are outside tested valid input scope.')
 (out/'analysis/input_packet_row_copy_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'row copy comparisons passed')
if __name__=='__main__':main()
