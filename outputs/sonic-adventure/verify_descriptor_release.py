"""Compare descriptor release with original instructions, including live callback mutations."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 dll_path=ROOT/'work/sonic-adventure/build/descriptor_release.dll'
 callback=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32))
 fn=ctypes.CDLL(str(dll_path)).sa_descriptor_release_8c6088bc
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_void_p,callback];fn.restype=ctypes.c_uint32
 cases=0
 for count,failures,mutation,initial_sr in itertools.product(range(9),range(16),['none','truncate','extend','redirect'],[0,0x700083f3]):
  for memory in (old,native):
   memory[0xa00000:0xa00400]=b'\xa5'*0x400
   struct.pack_into('<2I',memory,0xa00000,0x8ca00100,count)
   for i in range(12):
    struct.pack_into('<3I',memory,0xa00100+i*12,0x12340000+i,0x56780000+i,0x8cb00000+i)
    struct.pack_into('<3I',memory,0xa00200+i*12,0x77770000+i,0x88880000+i,0x8cc00000+i)
   struct.pack_into('<I',memory,0x88f7a8,0xdeadbeef)
  traces=[[],[]]
  def effect(memory,arg,sr,which):
   index=len(traces[which]);traces[which].append((arg,sr))
   if index==0:
    if mutation=='truncate':struct.pack_into('<I',memory,0xa00004,0)
    if mutation=='extend':struct.pack_into('<I',memory,0xa00004,10)
    if mutation=='redirect':struct.pack_into('<I',memory,0xa00000,0x8ca00200)
   value=[0,1,0x7fffffff,0x80000000,0xffffffff][index%5]
   if failures&(1<<(index%4)):value|=0x80000000
   return value,(sr^0x200)^((index&1)<<1)
  def original(memory,regs,sr):
   regs[0],sr=effect(memory,regs[4],sr,0);return sr
  @callback
  def recovered(ctx,arg,sr):
   value,sr[0]=effect(native,arg,sr[0],1);return value
  regs=State();regs.r[:]=[0x12340000+i for i in range(16)]
  regs.r[4]=0x8ca00000;regs.r[15]=0x8cf00000;regs.pc=0x8c6088bc;regs.pr=0x8c000100;regs.sr=initial_sr
  saved=list(regs.r[8:]);sr=ctypes.c_uint32(initial_sr)
  reference(old,regs,{0x8c64ddc0:original})
  result=fn(view,ctypes.byref(sr),0x8ca00000,None,recovered)
  assert result==regs.r[0] and sr.value==regs.sr,(count,failures,mutation)
  assert traces[0]==traces[1] and old[0xa00000:0xa00400]==native[0xa00000:0xa00400]
  assert old[0x88f7a4:0x88f7b0]==native[0x88f7a4:0x88f7b0]
  assert list(regs.r[8:])==saved and regs.pr==0x8c000100
  cases+=1
 out=ROOT/'outputs/sonic-adventure'
 report={'routine_start':'0x8c6088bc','routine_end_exclusive':'0x8c60890c','retail_span_sha256':hashlib.sha256(binary[0x5f88bc:0x5f890c]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/descriptor_release.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Counts zero through eight, failure patterns, signed result boundaries, live count/base mutation, return/SR, preserved registers and neighboring descriptor/index bytes; inner service 8c64ddc0 hooked.'}
 (out/'analysis/descriptor_release_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(f'{cases} descriptor release comparisons passed')
if __name__=='__main__':main()
