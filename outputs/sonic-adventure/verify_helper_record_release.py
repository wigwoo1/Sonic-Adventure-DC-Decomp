"""Execute the original record cleanup and heap frees without service hooks."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll=ROOT/'work/sonic-adventure/build/helper_record_release.dll';fn=ctypes.CDLL(str(dll)).sa_helper_record_release_8c07a49c
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32];fn.restype=ctypes.c_uint32
 cases=0
 for mode,count,mask,sr0 in itertools.product([2,3],[0,1,2,4,8,128,255],range(8),[0,0x700083f3]):
  slots=mode;stride=16 if mode==2 else 24;blocks=count*slots
  for mem in (old,native):
   mem[0xa00000:0xa05000]=bytes(0x5000);mem[0x880000:0x888000]=bytes(0x8000)
   struct.pack_into('<I',mem,0xa00028,0x8ca00100);struct.pack_into('<I',mem,0xa00020,0x8ca00200)
   struct.pack_into('<H',mem,0xa00004,2);mem[0xa00224]=count
   struct.pack_into('<I',mem,0xa00104,0x8ca00300);struct.pack_into('<I',mem,0xa00300,0x8ca01000);struct.pack_into('<H',mem,0xa0030a,mode)
   for i in range(blocks+1):struct.pack_into('<4I',mem,0x880010+i*32,0,0,0x8c880010+(i+1)*32 if i<blocks else 0,0x8c880010+(i-1)*32 if i else 0)
   for row in range(count):
    for slot in range(slots):
     ptr=0x8c880020+(row*slots+slot)*32 if mask&(1<<slot) else 0
     struct.pack_into('<I',mem,0xa01000+row*stride+slot*4,ptr)
     struct.pack_into('<I',mem,0xa01000+row*stride+slot*4+mode*4,0xabcdef12)
   struct.pack_into('<I',mem,0x79f03c,0)
  state=State();state.r[:]=[0x12340000+i for i in range(16)];state.r[4]=0x8ca00000;state.r[15]=0x8cf00000;state.sr=sr0;state.pc=0x8c07a49c;state.pr=0x8c000100
  saved=list(state.r[8:]);reference(old,state);sr=ctypes.c_uint32(sr0)
  result=fn(view,ctypes.byref(sr),0x8ca00000)
  context=(mode,count,mask,sr0)
  assert result==state.r[0] and sr.value==state.sr,context
  for a,b in [(0xa00000,0xa05000),(0x880000,0x888000),(0x79f03c,0x79f040)]:assert old[a:b]==native[a:b],context
  assert list(state.r[8:])==saved and state.pr==0x8c000100
  cases+=1
 out=ROOT/'outputs/sonic-adventure'
 report=dict(routine_start='0x8c07a49c',routine_end_exclusive='0x8c07a55c',retail_span_sha256=hashlib.sha256(binary[0x6a49c:0x6a55c]).hexdigest(),source_sha256=hashlib.sha256((out/'source/helper_record_release.c').read_bytes()).hexdigest(),dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),cases_passed=cases,scope='Original record-release and heap-free bodies execute without hooks. Modes 2/3, all slot presence combinations, count 0/1/2/4/8/128/255, complete fixture RAM, free head, r0, SR, preserved registers. Invalid pointers and other mode layouts untested; hardware unverified.')
 (out/'analysis/helper_record_release_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} record release with native heap comparisons passed')
if __name__=='__main__':main()

