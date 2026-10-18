"""Shared-tail structured C versus its original instructions, no service hooks."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference
from verify_fpu_cleanup_transfer import Fpu

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ROOT/'work/sonic-adventure/build/owner_helper_tail.dll';fn=ctypes.CDLL(str(dll)).sa_owner_helper_tail
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8)]+[ctypes.POINTER(ctypes.c_uint32)]*3+[ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32)];fn.restype=ctypes.c_int
 cases=0
 for state,index,count,mode,bank,sr0 in itertools.product(list(range(16))+[32768,65535],[0,1,65535],[0,1,7,0xffffff,0x1000001,0x1000003,0x7fffffff,0x80000000,0xffffffff],range(2),range(2),[0,0x700083f3]):
  for memory in (old,native):
   memory[0xa00000:0xa03000]=bytes([0xa5])*0x3000
   struct.pack_into('<H',memory,0xa00000,state);struct.pack_into('<H',memory,0xa00006,index)
   struct.pack_into('<2I',memory,0xa00010,0xdeadbeef,0x7fc00001);struct.pack_into('<I',memory,0xa00020,0x8c900000)
   struct.pack_into('<I',memory,0x900000+index*16,0x8ca01000);struct.pack_into('<I',memory,0xa01004,0x8ca02000);struct.pack_into('<I',memory,0xa02004,count)
  regs=State();regs.r[4]=0x8ca00000;regs.r[15]=0x8cf00000;regs.pc=0x8c079b92;regs.pr=0x8c000100;regs.sr=sr0
  fpu=Fpu();fpu.fpscr=(bank<<21)|0x40000|mode;fpu.fpul=0xabcdef12
  reference(old,regs,fpu=fpu)
  sr=ctypes.c_uint32(sr0);fpscr=ctypes.c_uint32((bank<<21)|0x40000|mode);fpul=ctypes.c_uint32(0xabcdef12);r0=ctypes.c_uint32()
  assert fn(view,ctypes.byref(sr),ctypes.byref(fpscr),ctypes.byref(fpul),0x8ca00000,ctypes.byref(r0))==0
  assert old[0xa00000:0xa03000]==native[0xa00000:0xa03000],(state,count,mode)
  assert r0.value==regs.r[0] and sr.value==regs.sr and fpscr.value==fpu.fpscr and fpul.value==fpu.fpul,(state,count,mode)
  cases+=1
 out=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/owner_helper_tail.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),retail_span_sha256=hashlib.sha256(binary[0x69b92:0x69bfa]).hexdigest(),scope='Shared-tail structured C: all states 0..15 plus signed-boundary defaults; current-table indices, uint32 count conversion, phase copy/clear, r0/SR/FPSCR/FPUL and RAM versus original instructions. Native arithmetic and conversion execute, no service hooks. Volatile floating registers not exposed; parent helper incomplete and hardware unverified.')
 (out/'analysis/owner_helper_tail_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} shared helper tail comparisons passed')
if __name__=='__main__':main()
