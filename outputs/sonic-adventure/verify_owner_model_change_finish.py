import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ROOT/'work/sonic-adventure/build/owner_model_change_finish.dll';fn=ctypes.CDLL(str(dll)).sa_owner_model_change_finish;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32)]+[ctypes.c_uint32]*4;fn.restype=ctypes.c_uint32
 cases=0
 for current,previous,cached,sr0 in itertools.product([2,3,0,0xffffffff],[2,3,0,0xffffffff],[2,3,0x12345678],[0,0x700083f3]):
  for memory in (old,native):
   struct.pack_into('<I',memory,0xa00028,0x8ca00100);struct.pack_into('<I',memory,0xa00104,0x8ca00200);memory[0xa00200:0xa00210]=bytes([0xa5])*16
   struct.pack_into('<2I',memory,0xf00014,previous,current)
  regs=State();regs.r[12]=0x8ca00000;regs.r[8]=cached;regs.r[15]=0x8cf00000;regs.pc=0x8c07a456;regs.pr=0x8c000100;regs.sr=sr0
  reference(old,regs,{0x8c07a484:lambda memory,r,s:s})
  status=ctypes.c_uint32(sr0);result=fn(view,ctypes.byref(status),0x8ca00000,current,previous,cached)
  assert result==regs.r[0] and status.value==regs.sr
  assert native[0xa00200:0xa00210]==old[0xa00200:0xa00210];cases+=1
 out=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/owner_model_change_finish.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),scope='Shared worker finalization only, header field writes and r0/SR; stack epilogue hooked, worker body unrecovered.')
 (out/'analysis/owner_model_change_finish_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} model-change finalization comparisons passed')
if __name__=='__main__':main()
