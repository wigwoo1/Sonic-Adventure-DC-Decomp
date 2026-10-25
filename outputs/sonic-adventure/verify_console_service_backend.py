"""Independent full-byte oracles for explicit console info and flash reads."""
import ctypes,hashlib,json
from pathlib import Path
import console_service_reference as model
P=Path(__file__).resolve().parent;ROOT=P.parents[1];DLL=ROOT/'work/sonic-adventure/build/movie_console.dll'
def main():
 lib=ctypes.CDLL(str(DLL));model.declare(lib);checks=0;calls=0
 c=dict(console_info=True,console_flash=True,flash_patches=[[0,list(range(64))],[0x1a05e,[0x12,0x34]],[0x20000-64,list(range(63,-1,-1))]])
 s,ia,fa=model.configure(lib,c);info,flash=model.inputs(c);ram=bytearray([0xa5])*(16<<20);view=(ctypes.c_uint8*len(ram)).from_buffer(ram);ref=model.Reference(c)
 def run(channel,op,first=0,second=0x8cd00000,count=0):
  nonlocal checks,calls
  before=bytes(s);expected=bytearray(ram);result=ctypes.c_uint32(0x12345678);write=ctypes.c_uint32(0x87654321)
  rc=lib.sa_console_service(ctypes.byref(s),view,channel,op,first,second,count,ctypes.byref(result),ctypes.byref(write));wanted,value=ref.service(expected,channel,op,first,second,count)
  assert rc==wanted and ram==expected and len(ram)==16<<20;checks+=3
  assert model.snapshot(s)==ref.snapshot() and bytes(ia)==info and bytes(fa)==flash;checks+=3
  if rc:assert bytes(s)==before and result.value==0x12345678 and write.value==0x87654321;checks+=3
  elif value is None:assert result.value==0x12345678 and write.value==0;checks+=2
  else:assert result.value==value and write.value==1;checks+=2
  calls+=1
 run(0,3);run(0,0);assert ram[0x68:0x80]==info;checks+=1
 run(0,3);run(0,0);run(0,1);run(0,2);run(0,0xffffffff);run(2,0)
 for alias in (0,0x80000000,0xa0000000):
  for first,count in ((0,0),(0,1),(0,2),(3,24),(0x1a05e,2),(len(flash)-3,3),(0,len(flash))):run(1,1,first,alias|0x0cd00000,count)
 for first,dest,count in ((0,0x8cffffff,1),(0,0x8cffffff,2),(0,0x8d000000,0),(0,0x8d000000,1),(0,0xccd00000,1),(len(flash),0x8cd00000,0),(len(flash)+1,0x8cd00000,0),(0xfffffff0,0x8cd00000,64),(0,0x8cd00000,0xffffffff),(len(flash)-1,0x8cd00000,2)):run(1,1,first,dest,count)
 for op in (0,2,3,0xffffffff):run(1,op)
 # Binding only writes configured vectors, preserving other guest bytes.
 for config in ({},dict(console_info=True),dict(console_flash=True),dict(console_info=True,console_flash=True)):
  q,ib,fb=model.configure(lib,config);before=bytes(ram);expected=bytearray(ram)
  if config.get('console_info'):expected[0xb0:0xb4]=model.SYSINFO.to_bytes(4,'little')
  if config.get('console_flash'):expected[0xb8:0xbc]=model.FLASH.to_bytes(4,'little')
  assert lib.sa_console_bind_vectors(ctypes.byref(q),view)==(0 if config else -2) and ram==expected;checks+=2
  if not config:assert before==bytes(ram);checks+=1
 # Unconfigured, empty and rejected input spans remain distinct.
 for config,channel in (({},0),({},1),(dict(console_flash=True,flash_size=0),1)):
  q,ib,fb=model.configure(lib,config);result=ctypes.c_uint32(9);write=ctypes.c_uint32(10);before=bytes(ram)
  rc=lib.sa_console_service(ctypes.byref(q),view,channel,0 if channel==0 else 1,0,0x8cd00000,1,ctypes.byref(result),ctypes.byref(write))
  if config:assert (rc,result.value,write.value)==(0,0xffffffff,1)
  else:assert (rc,result.value,write.value)==(-2,9,10)
  assert bytes(ram)==before;checks+=2
 # Rejected setters and null output arguments do not mutate their state.
 before=bytes(s)
 for size in (0,1,23,25,0xffffffff):assert lib.sa_console_set_info(ctypes.byref(s),ia,size)==-3 and bytes(s)==before;checks+=2
 assert lib.sa_console_set_info(ctypes.byref(s),None,24)==-2 and bytes(s)==before;checks+=2
 assert lib.sa_console_set_flash(ctypes.byref(s),None,1)==-2 and bytes(s)==before;checks+=2
 for result_ptr,write_ptr in ((None,ctypes.byref(ctypes.c_uint32(7))),(ctypes.byref(ctypes.c_uint32(8)),None)):
  before_ram=bytes(ram);assert lib.sa_console_service(ctypes.byref(s),view,0,0,0,0,0,result_ptr,write_ptr)==-2 and bytes(s)==before and bytes(ram)==before_ram;checks+=3
 # Borrowed input spans cannot alias the destination RAM.
 for channel in (0,1):
  q=model.Console();assert lib.sa_console_service_init(ctypes.byref(q))==0
  setter=lib.sa_console_set_info if channel==0 else lib.sa_console_set_flash
  assert setter(ctypes.byref(q),ctypes.cast(ctypes.byref(view,0x100),ctypes.POINTER(ctypes.c_uint8)),24)==0
  before=bytes(q);before_ram=bytes(ram);result=ctypes.c_uint32(9);write=ctypes.c_uint32(10)
  assert lib.sa_console_service(ctypes.byref(q),view,channel,0 if channel==0 else 1,0,0x8cd00000,2,ctypes.byref(result),ctypes.byref(write))==-3
  assert bytes(q)==before and bytes(ram)==before_ram and (result.value,write.value)==(9,10);checks+=4
 # Reconfiguration requires a fresh init; a void init never overwrites result.
 assert lib.sa_console_set_info(ctypes.byref(s),ia,24)==0 and not s.initialized;checks+=2
 result=ctypes.c_uint32(9);write=ctypes.c_uint32(10);before=bytes(s)
 assert lib.sa_console_service(ctypes.byref(s),view,0,3,0,0,0,ctypes.byref(result),ctypes.byref(write))==-4 and bytes(s)==before and (result.value,write.value)==(9,10);checks+=3
 files=[Path(__file__),P/'console_service_reference.py',P/'source/native_console_service.c',P/'source/native_console_service.h']
 report=dict(api_cases=calls,independent_checks=checks,complete_ram_bytes_per_case=16<<20,inputs_unchanged=True,dll_sha256=hashlib.sha256(DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Declared host service policy and complete RAM/input/state checks. No console BIOS decompilation, physical flash behavior, persistent write/erase, icon/font or unsupported selector claim.')
 (P/'analysis/console_service_backend_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(calls,'modeled API cases;',checks,'independent checks')
if __name__=='__main__':main()
