import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu
from verify_tikal_controller import Call,P

def main():
 out=ROOT/'outputs/sonic-adventure';binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary;view=(ctypes.c_uint8*len(ram)).from_buffer(ram)
 dll=ROOT/'work/sonic-adventure/build/stage_mode_query.dll';fn=ctypes.CDLL(str(dll)).sa_stage_mode_query;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Fpu),P,P,P,P,P,ctypes.c_void_p,Call]
 @Call
 def call(*args):raise AssertionError('Unexpected service')
 values=[0,1,127,128,255,256,32767,32768,65534,65535];count=0
 for stage,act,mode in itertools.product(range(65536),[0],[0,0x200000,0x380003,0xffffffff]):
  struct.pack_into('<H',ram,0x7493ba,stage);struct.pack_into('<H',ram,0x7493bc,act);f=Fpu();f.fpscr=mode;f.fpul=0x12345678;nf=Fpu.from_buffer_copy(f)
  s=State();s.r[:]=[0x12340000+i for i in range(16)];s.pc=0x8c04c960;s.pr=0x8c000100;s.sr=0x700083f3;r=(ctypes.c_uint32*16)(*s.r);sr=ctypes.c_uint32(s.sr);pr=ctypes.c_uint32(s.pr);fpul=ctypes.c_uint32(f.fpul);macl=ctypes.c_uint32()
  reference(ram,s,fpu=f)
  assert fn(view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
  signed=lambda x:x if x<32768 else x-65536
  assert r[0]==(1 if 26<=signed(stage)<=34 or signed(stage)>38 else 2)
  nf.fpul=fpul.value
  assert list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f);count+=1
 report=dict(cases_passed=count,instruction_count=23,source_sha256=hashlib.sha256((out/'source/sa_stage_mode_query.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),binary_sha256=hashlib.sha256(binary).hexdigest(),scope='Whole stage mode query, signed16 stage and conditional/delay-slot branch behavior. All65536 patterns across four FPU modes; independent range formula plus full regs/SR/PR/FPU comparison against retail instructions. No external services; parent connection pending.')
 (out/'analysis/stage_mode_query_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(count,'stage mode query checks passed')
if __name__=='__main__':main()
