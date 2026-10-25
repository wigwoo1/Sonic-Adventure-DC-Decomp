"""Independent declared host console/flash policy; not a Dreamcast BIOS model."""
import ctypes
U=ctypes.c_uint32
class Console(ctypes.Structure):
 _fields_=[('info',ctypes.POINTER(ctypes.c_uint8)),('flash',ctypes.POINTER(ctypes.c_uint8))]+[(n,U) for n in ('info_size','flash_size','info_configured','flash_configured','initialized')]+[(n,ctypes.c_uint64) for n in ('info_calls','flash_calls','flash_bytes')]
SYSINFO,FLASH,ADDRESS=0x8c00f110,0x8c00f120,0x8c000068
def inputs(c):
 info=bytearray((i*7+3)&255 for i in range(24));info[12]=c.get('region_byte',ord('1'))&255
 if 'info_bytes' in c:info=bytearray(c['info_bytes'])
 flash=bytearray([0xff])*c.get('flash_size',0x20000)
 for at,values in c.get('flash_patches',[]):
  assert 0<=at<=len(flash) and len(values)<=len(flash)-at
  flash[at:at+len(values)]=bytes(values)
 return bytes(info),bytes(flash)
def snapshot(s):return tuple(getattr(s,n) for n in ('info_size','flash_size','info_configured','flash_configured','initialized','info_calls','flash_calls','flash_bytes'))
class Reference:
 def __init__(self,c):
  self.info,self.flash=inputs(c);self.info_configured=int(c.get('console_info',False));self.flash_configured=int(c.get('console_flash',False));self.initialized=0;self.info_calls=self.flash_calls=self.flash_bytes=0
  self.info_size=len(self.info) if self.info_configured else 0;self.flash_size=len(self.flash) if self.flash_configured else 0
 def snapshot(self):return snapshot(self)
 def service(self,m,channel,selector,first,second,count):
  if channel==0:
   if selector not in (0,3):return -8,None
   if not self.info_configured:return -2,None
   if len(self.info)!=24:return -3,None
   if selector==3 and not self.initialized:return -4,None
   if selector==0:m[0x68:0x80]=self.info;self.initialized=1;result=None
   else:result=ADDRESS
   self.info_calls=(self.info_calls+1)&0xffffffffffffffff;return 0,result
  if channel!=1 or selector!=1:return -8,None
  if not self.flash_configured:return -2,None
  if count>0x7fffffff or first>len(self.flash) or count>len(self.flash)-first:
   self.flash_calls=(self.flash_calls+1)&0xffffffffffffffff;return 0,0xffffffff
  physical=second&0x1fffffff;start=physical-0xc000000
  if second&0xe0000000 not in (0,0x80000000,0xa0000000) or not 0<=start<=0x1000000 or count>0x1000000-start:return -3,None
  m[start:start+count]=self.flash[first:first+count];self.flash_calls=(self.flash_calls+1)&0xffffffffffffffff;self.flash_bytes=(self.flash_bytes+count)&0xffffffffffffffff;return 0,count

def configure(lib,c):
 info,flash=inputs(c);ia=(ctypes.c_uint8*len(info))(*info);fa=(ctypes.c_uint8*len(flash))(*flash);s=Console()
 assert lib.sa_console_service_init(ctypes.byref(s))==0
 if c.get('console_info'):assert lib.sa_console_set_info(ctypes.byref(s),ia,len(info))==0
 if c.get('console_flash'):assert lib.sa_console_set_flash(ctypes.byref(s),fa,len(flash))==0
 return s,ia,fa
def declare(lib):
 lib.sa_console_service_size.restype=U;assert lib.sa_console_service_size()==ctypes.sizeof(Console)
 lib.sa_console_service_init.argtypes=[ctypes.POINTER(Console)];lib.sa_console_service_init.restype=ctypes.c_int
 for name in ('sa_console_set_info','sa_console_set_flash'):
  getattr(lib,name).argtypes=[ctypes.POINTER(Console),ctypes.POINTER(ctypes.c_uint8),U];getattr(lib,name).restype=ctypes.c_int
 lib.sa_console_bind_vectors.argtypes=[ctypes.POINTER(Console),ctypes.POINTER(ctypes.c_uint8)];lib.sa_console_bind_vectors.restype=ctypes.c_int
 lib.sa_console_service.argtypes=[ctypes.POINTER(Console),ctypes.POINTER(ctypes.c_uint8),U,U,U,U,U,ctypes.POINTER(U),ctypes.POINTER(U)];lib.sa_console_service.restype=ctypes.c_int
