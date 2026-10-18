"""Compare descriptor reset; division helper explicitly hooked on both sides."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 cbtype=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32))
 dll_path=ROOT/'work/sonic-adventure/build/descriptor_reset.dll';fn=ctypes.CDLL(str(dll_path)).sa_descriptor_reset_8c64e720
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_void_p,cbtype];fn.restype=ctypes.c_uint32
 cases=0
 for index,count,empty,global_index,initial_sr in itertools.product(range(8),[0,1,2,4,8,0x80000000,0xffffffff],range(9),[0,1,7,0xffffffff],[0,0x700083f3]):
  address=0x8ca00000+index*68
  for memory in (old,native):
   memory[0xa00000:0xa00400]=b'\xa5'*0x400
   for i in range(8):struct.pack_into('<I',memory,0xa00000+i*68,0xffffffff if i>=8-empty else i+100)
   struct.pack_into('<I',memory,0x673484,0x8ca00000);struct.pack_into('<I',memory,0x88f6a0,count);struct.pack_into('<I',memory,0x673d18,global_index)
  events=[[],[]]
  def original(memory,regs,sr):
   events[0].append((regs[1],regs[0],sr));regs[0]=regs[1]//regs[0];regs[1]=regs[0];return sr^0x301
  @cbtype
  def recovered(ctx,numerator,denominator,sr):
   events[1].append((numerator,denominator,sr[0]));sr[0]^=0x301;return numerator//denominator
  regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=address;regs.r[15]=0x8cf00000;regs.pc=0x8c64e720;regs.pr=0x8c000100;regs.sr=initial_sr
  saved=list(regs.r[8:]);sr=ctypes.c_uint32(initial_sr);reference(old,regs,{0x8c10cb10:original})
  context=(index,count,empty,global_index,initial_sr)
  assert fn(view,ctypes.byref(sr),address,None,recovered)==regs.r[0] and sr.value==regs.sr,context
  assert events[0]==events[1] and old[0xa00000:0xa00400]==native[0xa00000:0xa00400],context
  for a in [0x673484,0x88f6a0,0x673d18]:assert old[a:a+4]==native[a:a+4],context
  assert list(regs.r[8:])==saved and regs.pr==0x8c000100
  cases+=1
 out=ROOT/'outputs/sonic-adventure';report={'routine_start':'0x8c64e720','routine_end_exclusive':'0x8c64e7a2','retail_span_sha256':hashlib.sha256(binary[0x63e720:0x63e7a2]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/descriptor_reset.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Eight descriptor positions, signed count boundaries, trailing unused records, global index boundaries; preserved fields, return, SR, registers, division arguments. Division 8c10cb10 hooked; its export proves r5 untouched but its arithmetic body remains unrecovered.'}
 (out/'analysis/descriptor_reset_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} descriptor reset comparisons passed')
if __name__=='__main__':main()
