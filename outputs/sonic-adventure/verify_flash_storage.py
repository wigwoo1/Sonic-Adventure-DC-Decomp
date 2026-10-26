"""Check actual read/program/erase bytes, partition metadata and host failures."""
import ctypes,hashlib,itertools,json,random
from pathlib import Path
import flash_storage_reference as ref
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
DLL=ROOT/'work/sonic-adventure/build/flash_storage.dll'
PARTS=((0,64),(128,64),(256,256),(1024,512),(2048,2048))

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 lib=ctypes.CDLL(str(DLL));ref.declare(lib);records=[];checks=0
 ram=bytearray([0x66])*0x1000000;wanted=bytearray(ram)
 view=(ctypes.c_uint8*len(ram)).from_buffer(ram)
 parts=(ref.Partition*5)(*(ref.Partition(a,n) for a,n in PARTS))
 for writable in (0,1):
  initial=bytes((i*17+31)&255 for i in range(4096));flash=bytearray(initial)
  data=(ctypes.c_uint8*len(flash)).from_buffer(flash);s=ref.State()
  assert lib.sa_flash_storage_init(ctypes.byref(s))==0
  assert lib.sa_flash_storage_configure(ctypes.byref(s),data,len(flash),parts,5,writable)==0
  oracle=ref.Reference(initial,PARTS,writable)
  def call(selector,first,second=0x8cd40000,count=0):
   nonlocal checks
   result=ref.U(0x12345678);expected=oracle.service(wanted,selector,first,second,count)
   rc=lib.sa_flash_storage_service(ctypes.byref(s),view,selector,first,second,count,ctypes.byref(result))
   assert rc==expected[0] and result.value==(0x12345678 if expected[1] is None else expected[1])
   assert ram==wanted and flash==oracle.data and ref.snapshot(s)==oracle.snapshot()
   checks+=5;records.append(dict(writable=writable,selector=selector,first=first,second=hex(second),count=count,host_status=rc,guest_result=hex(result.value)))
  for index,address in itertools.product((0,1,4,5,0xffffffff),(0x0cd40000,0x8cd40001,0xacd40000,0x8cfffffc,0x8d000000,0xccd40000)):
   call(0,index,address)
  for (offset,n),address in itertools.product(((0,0),(0,1),(31,65),(4095,1),(4096,0),(4096,1),(0xffffffff,1),(0,0x80000000)),(0x0cd40000,0x8cd40000,0xacd40000)):
   call(1,offset,address,n)
  # Erase actual bytes, then program data in several passes. A zero-to-one
  # request is rejected atomically; the final program succeeds after erase.
  call(3,256);ram[0x00d40000:0x00d40040]=wanted[0x00d40000:0x00d40040]=bytes([0xf0])*64
  call(2,256,count=64)
  ram[0x00d40000:0x00d40040]=wanted[0x00d40000:0x00d40040]=bytes([0x80])*64
  call(2,256,count=64)
  ram[0x00d40000:0x00d40040]=wanted[0x00d40000:0x00d40040]=bytes([0xff])*64
  before=bytes(flash);call(2,256,count=64);assert bytes(flash)==before;checks+=1
  call(3,256);call(2,256,count=64);call(1,256,count=256)
  for selector,first,address,n in ((2,4096,0x8d000000,0),(2,4096,0x8cd40000,1),(2,1,0x8cffffff,2),(1,1,0xcc000000,2),(3,257,0,0),(3,0xffffffff,0,0),(4,0,0,0),(0xffffffff,0,0,0)):
   call(selector,first,address,n)
  rng=random.Random(571+writable)
  for _ in range(100):
   selector=rng.randrange(4);first=rng.randrange(4200);n=rng.randrange(260)
   if selector==0:first=rng.randrange(7)
   if selector==3:first=rng.choice([a for a,_ in PARTS]+[17])
   call(selector,first,0x8cd40000,n)
  # Invalid host requests preserve all state and data, including output.
  for null in ('state','ram','result'):
   saved=(bytes(s),bytes(ram),bytes(flash));result=ref.U(0x12345678)
   rc=lib.sa_flash_storage_service(None if null=='state' else ctypes.byref(s),None if null=='ram' else view,0,0,0x8cd40000,0,None if null=='result' else ctypes.byref(result))
   assert rc==-2 and (bytes(s),bytes(ram),bytes(flash))==saved and result.value==0x12345678;checks+=3
  for invalid in ('configured','writable','count','overlap','range','ram_alias','result_alias'):
   original=bytes(s)
   if invalid=='configured':s.configured=2
   if invalid=='writable':s.writable=2
   if invalid=='count':s.partition_count=6
   if invalid=='overlap':s.partitions[1].start=0
   if invalid=='range':s.partitions[0].size=5000
   if invalid=='ram_alias':s.bytes=ctypes.cast(view,ctypes.POINTER(ctypes.c_uint8))
   before=(bytes(s),bytes(ram),bytes(flash));result=ref.U(0x12345678)
   pointer=ctypes.cast(view,ctypes.POINTER(ref.U)) if invalid=='result_alias' else ctypes.byref(result)
   rc=lib.sa_flash_storage_service(ctypes.byref(s),view,0,0,0x8cd40000,0,pointer)
   assert rc==-3 and (bytes(s),bytes(ram),bytes(flash))==before and result.value==0x12345678;checks+=3
   ctypes.memmove(ctypes.byref(s),original,len(original))
  original=bytes(s)
  for args,expected in (([None,4096,parts,5,1],-2),([data,0,parts,5,1],-3),([data,4096,parts,6,1],-3),([data,4096,None,1,1],-2),([data,4096,parts,5,2],-3)):
   assert lib.sa_flash_storage_configure(ctypes.byref(s),*args)==expected and bytes(s)==original;checks+=2
 files=[Path(__file__),P/'flash_storage_reference.py',P/'source/flash_storage.c',P/'source/flash_storage.h',P/'build_flash_storage.ps1']
 report=dict(continuous_service_comparisons=len(records),independent_checks=checks,cases=records,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Authored host storage only,zero original routine credit. Explicit synthetic4096-byte image/five disjoint partition extents. Whole16MiBRAM/flash/metadata/counters/results compare at each service. Actual read/program/erase,one-to-zero restriction,atomic incompatible-program rejection,readonly control,aliases,zero/range/negative sizes,unknown selectors and invalid host configuration tested. No default identity or game settings; physical failure timing/partial programming/protection and BIOS implementation remain unmodeled.',primary_abi_sources=['https://kos-docs.dreamcast.wiki/group__system__calls.html','https://mc.pp.se/dc/syscalls.html'])
 (P/'analysis/flash_storage_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Authored flash service comparisons',len(records),'checks',checks)
if __name__=='__main__':main()
