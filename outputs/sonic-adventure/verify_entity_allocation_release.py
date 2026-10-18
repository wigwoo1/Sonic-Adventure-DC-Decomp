"""Compare entity allocation release with original SH-4 instructions."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll_path=ROOT/'work/sonic-adventure/build/entity_allocation_release.dll'
 cbtype=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32))
 fn=ctypes.CDLL(str(dll_path)).sa_entity_allocation_release_8c07d6c0
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_void_p,cbtype];fn.restype=ctypes.c_uint32
 cases=0
 for allocation,returned,mutation,initial_sr in itertools.product([0,1,0x8ca03000,0xffffffff],[0,0x78,0xffffffff],[False,True],[0,1,0x700083f2,0x700083f3]):
  for memory in (old,native):
   memory[0xa00000:0xa00200]=b'\xa5'*0x200
   struct.pack_into('<I',memory,0xa00078,allocation)
  traces=[[],[]]
  def effect(memory,arg,sr,which):
   traces[which].append((arg,sr))
   if mutation:struct.pack_into('<I',memory,0xa00078,0x12345678)
   return sr^0x201
  def original(memory,regs,sr):
   sr=effect(memory,regs[4],sr,0);regs[0]=returned;return sr
  @cbtype
  def recovered(ctx,arg,sr):
   sr[0]=effect(native,arg,sr[0],1);return returned
  regs=State();regs.r[:]=[0x12340000+i for i in range(16)]
  regs.r[4]=0x8ca00000;regs.r[15]=0x8cf00000;regs.pc=0x8c07d6c0;regs.pr=0x8c000100;regs.sr=initial_sr
  saved=list(regs.r[8:]);sr=ctypes.c_uint32(initial_sr)
  reference(old,regs,{0x8c098b42:original})
  assert fn(view,ctypes.byref(sr),0x8ca00000,None,recovered)==regs.r[0]==0x78
  assert sr.value==regs.sr and traces[0]==traces[1]
  assert old[0xa00000:0xa00200]==native[0xa00000:0xa00200]
  assert list(regs.r[8:])==saved and regs.pr==0x8c000100
  cases+=1
 out=ROOT/'outputs/sonic-adventure'
 report={'routine_start':'0x8c07d6c0','routine_end_exclusive':'0x8c07d6e0','retail_span_sha256':hashlib.sha256(binary[0x6d6c0:0x6d6e0]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/entity_allocation_release.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Null/non-null and boundary allocation values, nested return/SR effects and slot mutation, whole entity/neighbor bytes and preserved registers; heap release service hooked.'}
 (out/'analysis/entity_allocation_release_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(f'{cases} entity allocation release comparisons passed')
if __name__=='__main__':main()
