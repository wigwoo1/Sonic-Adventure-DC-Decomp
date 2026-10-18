import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;view=(ctypes.c_uint8*len(ram)).from_buffer(ram)
 dll=ROOT/'work/sonic-adventure/build/counter_get.dll';fn=ctypes.CDLL(str(dll)).sa_counter_get;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 @Call
 def call(*args):raise AssertionError('Unexpected service')
 values=[0,1,127,128,255,256,32767,32768,65534,65535];count=0
 for stage,act,mode in itertools.product(range(65536),[0],[0,0x200000,0x380003,0xffffffff]):
  struct.pack_into('<H',ram,0x752be4,stage);struct.pack_into('<H',ram,0x7493bc,act);f=Fpu();f.fpscr=mode;f.fpul=0x12345678;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x12340000+i for i in range(16)];s.pc=0x8c08a2bc;s.pr=0x8c000100;s.sr=0x700083f3;r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  signed=lambda x:x if x<32768 else x-65536
  assert r[0]==signed(stage)&0xffffffff
  nf.fpul=fpul.value
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f);count+=1
 report=dict(cases_passed=count,instruction_count=3,source_sha256=hashlib.sha256((out/'source/sa_counter_get.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),binary_sha256=hashlib.sha256(binary).hexdigest(),scope='Whole signed16 counter getter, read in return delay slot; all65536 bit patterns across four FPU mode configurations. Independent signed-extension formula plus full register/SR/PR/FPU comparison against retail instructions; no external services.')
 (out/'analysis/counter_get_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'counter getter checks passed')
if __name__=='__main__':main()
