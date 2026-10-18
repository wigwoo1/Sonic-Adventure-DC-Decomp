"""Model-change setup versus retail instructions; following worker hooked."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ROOT/'work/sonic-adventure/build/owner_model_change_prepare.dll';fn=ctypes.CDLL(str(dll)).sa_owner_model_change_prepare
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32)]
 cases=0
 for current,previous,cf,pf,sr0 in itertools.product([0,1,65535],[0,1,65535],[0,1,4,5,0x8000,0xffff],[0,1,4,5,0x8000,0xffff],[0,0x700083f3]):
  for mem in (old,native):
   struct.pack_into('<2H',mem,0xa00004,current,previous);struct.pack_into('<I',mem,0xa00020,0x8c900000)
   struct.pack_into('<I',mem,0x900000+current*16,0x8ca01000);struct.pack_into('<I',mem,0x900000+previous*16,0x8ca01100)
   struct.pack_into('<I',mem,0xa01004,0x8ca02000);struct.pack_into('<I',mem,0xa01104,0x8ca02100)
   struct.pack_into('<H',mem,0xa02008,cf);struct.pack_into('<H',mem,0xa02108,pf)
  regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=0x8ca00000;regs.r[15]=0x8cf00000;regs.pc=0x8c079c20;regs.pr=0x8c000100;regs.sr=sr0
  initial=(ctypes.c_uint32*16)(*regs.r);status=ctypes.c_uint32(sr0)
  reference(old,regs,{0x8c079c76:lambda memory,r,s:s})
  fn(view,ctypes.byref(status),0x8ca00000,0x8cf00000,initial)
  assert list(initial)==list(regs.r),(current,previous,cf,pf)
  assert status.value==regs.sr
  assert native[0xeffffc:0xf00000]==old[0xeffffc:0xf00000]
  cases+=1
 out=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/owner_model_change_prepare.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),retail_span_sha256=hashlib.sha256(binary[0x69c20:0x69c76]).hexdigest(),scope='Setup only: indices 0/1/65535, same and different descriptors, metadata flag combinations, complete integer registers, SR and stack scratch. Following 8c079c76 worker hooked and unrecovered; not a complete original routine.')
 (out/'analysis/owner_model_change_prepare_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} model-change setup comparisons passed')
if __name__=='__main__':main()
