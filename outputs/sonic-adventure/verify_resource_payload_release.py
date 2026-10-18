"""Compare payload release dispatch with original instructions and nested call effects."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 cbtype=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32))
 dll_path=ROOT/'work/sonic-adventure/build/resource_payload_release.dll';fn=ctypes.CDLL(str(dll_path)).sa_resource_payload_release_8c653d68
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_void_p,cbtype];fn.restype=ctypes.c_uint32
 cases=0
 for tag,allocation,returned,initial_sr in itertools.product([0,1,2,3,4,0xffff,0x10003,0xffff0003,0xffffffff],[0,1,0x8ca00100,0xffffffff],[0,1,0xffffffff],[0,0x700083f3]):
  for memory in (old,native):
   memory[0xa00000:0xa00200]=b'\xa5'*0x200;struct.pack_into('<I',memory,0xa00000,tag);struct.pack_into('<I',memory,0xa0001c,allocation)
  events=[[],[]]
  def effect(memory,service,arg,sr,which):
   events[which].append((service,arg,sr));struct.pack_into('<I',memory,0xa00000,0x98765432);return sr^0x201
  def hook(service):
   def inner(memory,regs,sr):
    sr=effect(memory,service,regs[4],sr,0);regs[0]=returned;return sr
   return inner
  @cbtype
  def recovered(ctx,service,arg,sr):
   sr[0]=effect(native,service,arg,sr[0],1);return returned
  regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=0x8ca00000;regs.r[15]=0x8cf00000;regs.pc=0x8c653d68;regs.pr=0x8c000100;regs.sr=initial_sr
  saved=list(regs.r[8:]);sr=ctypes.c_uint32(initial_sr);reference(old,regs,{s:hook(s) for s in [0x8c663950,0x8c66385a]})
  assert fn(view,ctypes.byref(sr),0x8ca00000,None,recovered)==regs.r[0] and sr.value==regs.sr
  assert events[0]==events[1] and old[0xa00000:0xa00200]==native[0xa00000:0xa00200]
  assert list(regs.r[8:])==saved and regs.pr==0x8c000100
  cases+=1
 out=ROOT/'outputs/sonic-adventure';report={'routine_start':'0x8c653d68','routine_end_exclusive':'0x8c653d8c','retail_span_sha256':hashlib.sha256(binary[0x643d68:0x643d8c]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/resource_payload_release.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Tag equality and upper bits, allocation and callback-return boundaries, SR effects, callback RAM mutation, preserved registers; nested bodies 8c663950 and 8c66385a hooked.'}
 (out/'analysis/resource_payload_release_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} payload release comparisons passed')
if __name__=='__main__':main()
