"""Verification-only exact dirty-page snapshots on single-threaded Windows.

Primary API contracts:
https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-getwritewatch

Owners must outlive borrowed native addresses. Exported buffer views keep the
owner alive. Explicit close rejects live buffer exports. No executable memory,
exception handlers, guest checkpoints or guest/helper result substitution.
"""
import ctypes,hashlib,mmap,os,threading,weakref

PAGE=mmap.PAGESIZE
if os.name=='nt':
 K=ctypes.WinDLL('kernel32',use_last_error=True)
 K.VirtualAlloc.argtypes=[ctypes.c_void_p,ctypes.c_size_t,ctypes.c_uint32,ctypes.c_uint32];K.VirtualAlloc.restype=ctypes.c_void_p
 K.VirtualFree.argtypes=[ctypes.c_void_p,ctypes.c_size_t,ctypes.c_uint32];K.VirtualFree.restype=ctypes.c_int
 K.GetWriteWatch.argtypes=[ctypes.c_uint32,ctypes.c_void_p,ctypes.c_size_t,ctypes.POINTER(ctypes.c_void_p),ctypes.POINTER(ctypes.c_size_t),ctypes.POINTER(ctypes.c_uint32)];K.GetWriteWatch.restype=ctypes.c_uint32

class WatchedMemory:
 def __init__(self,size):
  if os.name!='nt':raise OSError('Windows write-watch is unavailable')
  if type(size) is not int or size<=0 or size>1<<30:raise ValueError('Invalid write-watch allocation size')
  self.size=size;self.allocated=(size+PAGE-1)//PAGE*PAGE;self.address=0;self.exports=[];self.thread=threading.get_ident()
  self.address=K.VirtualAlloc(None,self.allocated,0x203000,4)
  if not self.address:raise ctypes.WinError(ctypes.get_last_error())
  self._addresses=(ctypes.c_void_p*(self.allocated//PAGE))()
 def _live(self):
  if not self.address:raise ValueError('Write-watch allocation is closed')
  if threading.get_ident()!=self.thread:raise RuntimeError('Write-watch regions require their owning verification thread')
 @property
 def view(self):
  self._live();array=(ctypes.c_uint8*self.size).from_address(self.address);array._write_watch_owner=self
  self.exports=[r for r in self.exports if r() is not None];self.exports.append(weakref.ref(array))
  return memoryview(array).cast('B')
 def pages(self,offset=0,size=None):
  self._live();size=self.size-offset if size is None else size
  if type(offset) is not int or type(size) is not int or offset<0 or size<=0 or offset>self.size-size:raise ValueError('Invalid watched window')
  count=ctypes.c_size_t(len(self._addresses));granularity=ctypes.c_uint32()
  rc=K.GetWriteWatch(1,self.address,self.allocated,self._addresses,ctypes.byref(count),ctypes.byref(granularity))
  if rc:raise OSError(rc,'GetWriteWatch failed: '+ctypes.FormatError(rc))
  if granularity.value!=PAGE or count.value>len(self._addresses):raise RuntimeError('Unexpected write-watch page geometry')
  result=set()
  for address in self._addresses[:count.value]:
   delta=address-self.address
   if delta<0 or delta>=self.allocated or delta%PAGE:raise RuntimeError('Invalid write-watch page address')
   start=max(delta,offset);end=min(delta+PAGE,offset+size)
   if start<end:result.update(range((start-offset)//PAGE,(end-1-offset)//PAGE+1))
  return sorted(result)
 def close(self):
  self._live()
  if any(r() is not None for r in self.exports):raise BufferError('Release exported views before closing write-watch memory')
  if not K.VirtualFree(self.address,0,0x8000):raise ctypes.WinError(ctypes.get_last_error())
  self.address=0
 def __del__(self):
  address=getattr(self,'address',0)
  if address and os.name=='nt':K.VirtualFree(address,0,0x8000)

class Window:
 def __init__(self,owner,offset=0,size=None):
  owner._live();size=owner.size-offset if size is None else size
  if type(offset) is not int or type(size) is not int or offset<0 or size<=0 or offset>owner.size-size:raise ValueError('Invalid window')
  self.owner=owner;self.offset=offset;self.size=size
 def pages(self):return self.owner.pages(self.offset,self.size)
 def page(self,index):
  self.owner._live()
  if type(index) is not int or index<0 or index*PAGE>=self.size:raise ValueError('Invalid logical page')
  return ctypes.string_at(self.owner.address+self.offset+index*PAGE,min(PAGE,self.size-index*PAGE))
 def read(self):self.owner._live();return ctypes.string_at(self.owner.address+self.offset,self.size)
 def digest(self):
  self.owner._live();array=(ctypes.c_uint8*self.size).from_address(self.owner.address+self.offset)
  return hashlib.sha256(array).digest()

class BoundaryJournal:
 """Reference deltas, native comparison of the union of written logical pages.

 Initial bytes agree. At each boundary every page changed by either execution
 is compared exactly. All other pages retain equality by induction. Final full
 bytes are independently checked. One window per allocation, no concurrent
 writes; the old decoder, boundaries and peripheral policies remain external.
 """
 def __init__(self,legacy_hashes=False):
  self.regions={};self.owners=set();self.errors=[];self.legacy_hashes=legacy_hashes
 def reference_region(self,name,window):
  if name in self.regions or id(window.owner) in self.owners:raise ValueError('Duplicate write-watch region')
  initial=window.read();window.pages();self.owners.add(id(window.owner))
  self.regions[name]=dict(reference=window,native=None,initial=initial,shadow=bytearray(initial),entries=[],cursor=0,captured_pages=0,compared_pages=0,compared_bytes=0)
 def native_region(self,name,window):
  r=self.regions[name]
  if r['native'] is not None or id(window.owner) in self.owners:raise ValueError('Duplicate native region')
  if window.size!=r['reference'].size or window.read()!=r['initial']:raise AssertionError('Initial memory differs: '+name)
  window.pages();r['native']=window;self.owners.add(id(window.owner))
 def reference_token(self,name):
  r=self.regions[name];w=r['reference'];delta=[(i,w.page(i)) for i in w.pages()];index=len(r['entries']);r['entries'].append(delta);r['captured_pages']+=len(delta)
  return (index,w.digest()) if self.legacy_hashes else index
 def native_token(self,name):
  r=self.regions[name];w=r['native'];index=r['cursor'];r['cursor']+=1
  if index>=len(r['entries']):
   self.errors.append((name,index,'unexpected native boundary'));return index
  touched=set(w.pages())
  for page,data in r['entries'][index]:
   start=page*PAGE;r['shadow'][start:start+len(data)]=data;touched.add(page)
  for page in sorted(touched):
   data=w.page(page);start=page*PAGE;r['compared_pages']+=1;r['compared_bytes']+=len(data)
   if data!=r['shadow'][start:start+len(data)]:self.errors.append((name,index,page))
  return (index,w.digest()) if self.legacy_hashes else index
 def check(self):
  if self.errors:raise AssertionError(('Exact boundary memory mismatch',self.errors[:8]))
 def finish(self):
  self.check()
  for name,r in self.regions.items():
   if r['cursor']!=len(r['entries']):raise AssertionError(('Missing native boundaries',name,r['cursor'],len(r['entries'])))
   if r['native'].read()!=r['reference'].read():raise AssertionError(('Final full memory mismatch',name))
  return dict(page_bytes=PAGE,legacy_hashes_also_compared=self.legacy_hashes,regions={name:dict(bytes=r['reference'].size,snapshots=len(r['entries']),captured_pages=r['captured_pages'],compared_pages=r['compared_pages'],exact_page_bytes_compared=r['compared_bytes'],initial_full_bytes_compared=len(r['initial']),final_full_bytes_compared=r['reference'].size) for name,r in self.regions.items()})
