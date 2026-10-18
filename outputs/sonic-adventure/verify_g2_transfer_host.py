import ctypes,itertools,json
from verify_startup import ROOT
Read=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint8),ctypes.c_uint32)
Write=ctypes.CFUNCTYPE(ctypes.c_int,ctypes.c_void_p,ctypes.c_uint32,ctypes.POINTER(ctypes.c_uint8),ctypes.c_uint32)
class Job(ctypes.Structure):_fields_=[(n,ctypes.c_uint32) for n in ['channel','g2_address','root_address','length','direction']]
class Bus(ctypes.Structure):_fields_=[('user',ctypes.c_void_p),('read',Read),('write',Write)]
def main():
 fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/g2_transfer_host.dll')).sa_g2_transfer_execute
 fn.argtypes=[ctypes.POINTER(Job),ctypes.POINTER(Bus),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)]
 count=0
 for channel,direction,length,flag,failure in itertools.product(range(4),[0,1],[0,32,96],[0,0x80000000],['none','read','write']):
  regions={0x00800000:bytearray((i*7+3)&255 for i in range(256)),0x0ca00000:bytearray((i*11+5)&255 for i in range(256))}
  before={a:bytes(v) for a,v in regions.items()};errors=[]
  @Read
  def read(ctx,a,p,n):
   base=0x00800000 if a<0x01000000 else 0x0ca00000;offset=a-base
   if failure=='read' and offset>=32:return -7
   if base not in regions or offset<0 or offset+n>256:errors.append(a);return -9
   for i,v in enumerate(regions[base][offset:offset+n]):p[i]=v
   return 0
  @Write
  def write(ctx,a,p,n):
   base=0x00800000 if a<0x01000000 else 0x0ca00000;offset=a-base
   if failure=='write' and offset>=32:return -8
   if base not in regions or offset<0 or offset+n>256:errors.append(a);return -9
   regions[base][offset:offset+n]=bytes(p[:n]);return 0
  bus=Bus(None,read,write);job=Job(channel,0x00800000,0x0ca00000,flag|length,direction);completed=ctypes.c_uint32();event=ctypes.c_uint32()
  result=fn(ctypes.byref(job),ctypes.byref(bus),ctypes.byref(completed),ctypes.byref(event));failed=failure!='none' and length>32
  transferred=32 if failed else length
  assert result==((-7 if failure=='read' else -8) if failed else 0) and not errors
  assert completed.value==transferred and event.value==(0 if failed else 0xa60+channel*32)
  source=0x00800000 if direction else 0x0ca00000;destination=0x0ca00000 if direction else 0x00800000
  assert regions[source]==before[source]
  assert regions[destination]==before[source][:transferred]+before[destination][transferred:]
  count+=1
 (ROOT/'outputs/sonic-adventure/analysis/g2_transfer_host_verification.json').write_text(json.dumps(dict(cases_passed=count,scope='Authored host byte-transfer service, not recovered code or retail equivalence. Independent source/destination bytes, source preservation, completion event code and partial-failure progress checks in both directions across four channels/lengths/high length bit/read-write failures. Valid caller mapped regions. No hardware timing/cache/IRQ injection or register-trigger integration yet.'),indent=2)+'\n')
 print(count,'host G2 byte transfer cases passed')
if __name__=='__main__':main()
