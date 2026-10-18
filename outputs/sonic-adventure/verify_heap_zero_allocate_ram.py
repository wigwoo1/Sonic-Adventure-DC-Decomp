import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(b)]=b;native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/heap_zero_allocate_ram.dll')).sa_heap_zero_allocate_ram_8c098b0c;fn.restype=ctypes.c_uint32;fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_uint32];cases=0
 for count,size,capacity,sr0 in itertools.product([0,1,2,111,0x40000000,0x80000000,0xffffffff],[0,1,3,4,5,24,0xfffffffd,0xffffffff],[0,32,128,4096],[0,0x700083f3]):
  byte_count=(((size+3)&0xfffffffc)*count)&0xffffffff
  if byte_count>capacity and ((byte_count+31)&0xfffffff0)<=capacity:continue # Original overflow would zero beyond supplied heap.
  for m in [old,native]:
   m[0xa00000:0xa02000]=bytes([0xa5])*8192;head=0x8ca00000;end=head+capacity
   struct.pack_into('<4I',m,0xa00000,end if capacity else 0,0,end if capacity else 0,0)
   if capacity:struct.pack_into('<4I',m,0xa00000+capacity,0,head,0,head)
   struct.pack_into('<I',m,0x79f03c,head)
  state=State();state.pc=0x8c098b0c;state.pr=0x8c000100;state.r[15]=0x8cf00000;state.r[14]=0x12345678;state.r[4]=count;state.r[5]=size;state.sr=sr0;reference(old,state)
  sr=ctypes.c_uint32(sr0);result=fn(view,ctypes.byref(sr),count,size)
  case=(count,size,capacity,sr0)
  assert result==state.r[0] and sr.value==state.sr and state.r[14]==0x12345678 and state.r[15]==0x8cf00000,case
  assert native[0xa00000:0xa02000]==old[0xa00000:0xa02000] and native[0x79f03c:0x79f040]==old[0x79f03c:0x79f040],case;cases+=1
 out=ROOT/'outputs/sonic-adventure';(out/'analysis/heap_zero_allocate_ram_verification.json').write_text(json.dumps(dict(cases_passed=cases,source_sha256=hashlib.sha256((out/'source/heap_zero_allocate_ram.c').read_bytes()).hexdigest(),scope='Zero allocation through original return without hooks; element rounding then wrapping multiplication, allocation failures/exact fits/splits and zero writes, r0/SR/game heap compared. Original callee preservation checked; MACL and nested scratch outside C API. Overflow requests whose wrapped allocation fits but zeroing exceeds supplied heap excluded.'),indent=2)+'\n');print(f'{cases} zero allocator comparisons passed')
if __name__=='__main__':main()

