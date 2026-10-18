"""Compare complete resource release branches with original instructions; two nested bodies hooked."""
import ctypes,hashlib,itertools,json,struct
from verify_startup import ROOT,State,reference

def main():
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 old=bytearray(16*1024*1024);old[0x10000:0x10000+len(binary)]=binary
 native=bytearray(old);view=(ctypes.c_uint8*len(native)).from_buffer(native)
 cbtype=ctypes.CFUNCTYPE(ctypes.c_uint32,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint32))
 dll_path=ROOT/'work/sonic-adventure/build/resource_release.dll';fn=ctypes.CDLL(str(dll_path)).sa_resource_release_8c64ddc0
 fn.argtypes=[ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32,ctypes.c_void_p,cbtype];fn.restype=ctypes.c_uint32
 cases=0
 for flags,count,type_id,matches,mutation,initial_sr in itertools.product(range(8),[0,1,2,0x7fff,0x8000,0xffff],[0x400,0x500,0x600,0x700,0x800,0x900],range(4),['none','flags','identifier'],[0,0x700083f3]):
  for memory in (old,native):
   memory[0xa00000:0xa00400]=b'\xa5'*0x400
   struct.pack_into('<I',memory,0xa00000,0xffffffff if mutation=='identifier' else 0x1234)
   struct.pack_into('<I',memory,0xa00010,flags);struct.pack_into('<I',memory,0xa00018,type_id<<16)
   struct.pack_into('<H',memory,0xa00040,count)
   struct.pack_into('<3I',memory,0x673484,0x8ca00100,4,0x1234)
   struct.pack_into('<I',memory,0x88f7b0,0xabcdef12)
   for i in range(4):struct.pack_into('<I',memory,0xa00100+i*68,(0xffffffff if mutation=='identifier' else 0x1234) if i<matches else 0x5678)
  events=[[],[]]
  def effect(memory,service,arg,sr,which):
   events[which].append((service,arg,sr))
   if mutation=='flags' and service==0x8c653d68:struct.pack_into('<I',memory,0xa00010,flags^5)
   if mutation=='identifier' and service==0x8c653d68:struct.pack_into('<I',memory,0xa00000,0x1234)
   if service==0x8c64e720:struct.pack_into('<I',memory,0xa00008,0x87654321)
   return (service^arg)&0xffffffff,sr^0x201
  def hook(service):
   def inner(memory,regs,sr):
    regs[0],sr=effect(memory,service,regs[4],sr,0);return sr
   return inner
  @cbtype
  def recovered(ctx,service,arg,sr):
   result,sr[0]=effect(native,service,arg,sr[0],1);return result
  regs=State();regs.r[:]=[0x12340000+i for i in range(16)];regs.r[4]=0x8ca00000;regs.r[15]=0x8cf00000;regs.pc=0x8c64ddc0;regs.pr=0x8c000100;regs.sr=initial_sr
  saved=list(regs.r[8:]);sr=ctypes.c_uint32(initial_sr)
  reference(old,regs,{s:hook(s) for s in [0x8c653d68,0x8c64e720]})
  result=fn(view,ctypes.byref(sr),0x8ca00000,None,recovered)
  context=(flags,count,type_id,matches,mutation,initial_sr)
  assert result==regs.r[0] and sr.value==regs.sr and events[0]==events[1],(context,result,regs.r[0],sr.value,regs.sr,events)
  for start,end in [(0xa00000,0xa00400),(0x673480,0x673494),(0x88f7ac,0x88f7b8)]:assert old[start:end]==native[start:end],context
  assert list(regs.r[8:])==saved and regs.pr==0x8c000100
  cases+=1
 out=ROOT/'outputs/sonic-adventure';report={'routine_start':'0x8c64ddc0','routine_end_exclusive':'0x8c64deb8','retail_span_sha256':hashlib.sha256(binary[0x63ddc0:0x63deb8]).hexdigest(),'cases_passed':cases,'source_sha256':hashlib.sha256((out/'source/resource_release.c').read_bytes()).hexdigest(),'dll_sha256':hashlib.sha256(dll_path.read_bytes()).hexdigest(),'scope':'Flags 0..7, counter zero/one/overflow boundaries, six types, zero through three matching records, nested flag/identifier changes; callbacks, return, SR, preserved registers, whole resource/table/global bytes. Recovered type classifier executed; services 8c653d68 and 8c64e720 hooked.'}
 (out/'analysis/resource_release_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} resource release comparisons passed')
if __name__=='__main__':main()
