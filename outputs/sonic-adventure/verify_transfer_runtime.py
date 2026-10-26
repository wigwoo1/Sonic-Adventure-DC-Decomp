"""Persistent native transfer device versus an independent byte/state oracle."""
import ctypes,hashlib,itertools,json
from pathlib import Path
import transfer_runtime_reference as model
from verify_sh4_exception_state import Bus,Read,Write,Cache,U
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
class Binding(ctypes.Structure):_fields_=[('state',ctypes.POINTER(model.State)),('parent',Bus),('ram',ctypes.POINTER(ctypes.c_uint8))]

class Scenario:
 def __init__(self,lib,enabled=1):
  self.lib=lib;self.n=model.State();assert lib.sa_transfer_runtime_init(ctypes.byref(self.n),enabled)==0;self.r=model.Device(enabled=enabled)
  self.ram=[bytearray(0x1000000),bytearray(0x1000000)];payload=bytes((i*41+(i>>8)+3)&255 for i in range(4096))
  self.sound=[bytearray([0xa5])*0x200000,bytearray([0xa5])*0x200000]
  for m in self.ram:m[0x900000:0x901000]=payload
  for m in self.sound:m[0x100:0x1100]=payload
  self.words=[{},{}];self.events=[[],[]];self.failure=None
  self.rd=Read(lambda ctx,a,n,out:self.read(0,a,n,out));self.wr=Write(lambda ctx,a,value,n:self.write(0,a,value,n));self.cache=Cache(lambda *args:0)
  self.parent=Bus(None,self.rd,self.wr,self.cache);self.bus=Bus();self.binding=Binding();self.view=(ctypes.c_uint8*len(self.ram[0])).from_buffer(self.ram[0])
  assert lib.sa_transfer_runtime_bind(ctypes.byref(self.binding),ctypes.byref(self.n),self.view,ctypes.byref(self.parent),ctypes.byref(self.bus))==0
 def value(self,i,a):
  if 0x800000<=a<0xa00000:return int.from_bytes(self.sound[i][a-0x800000:a-0x800000+4],'little')
  return self.words[i].get(a&0x1fffffff,0)
 def read(self,i,a,n,out=None):
  status=-3 if n!=4 or a&3 else -9 if self.failure==('r',a) else 0;value=0 if status else self.value(i,a)
  self.events[i].append(('r',a,n,value,status))
  if out is not None:out[0]=value;return status
  return status,value
 def write(self,i,a,value,n=4):
  status=-3 if n!=4 or a&3 else -9 if self.failure==('w',a) else 0;self.events[i].append(('w',a,n,value,status))
  if status:return status
  if 0x800000<=a<0xa00000:self.sound[i][a-0x800000:a-0x800000+4]=value.to_bytes(4,'little')
  elif a&0x1fffffff==0x5f6900:self.words[i][a&0x1fffffff]=self.words[i].get(a&0x1fffffff,0)&~value
  else:self.words[i][a&0x1fffffff]=value
  return 0
 def guest_write(self,a,value,n=4):
  actual=self.bus.write(self.bus.context,a,value,n);wanted=self.write(1,a,value,n)
  if not wanted:wanted=self.r.written(a,value,n)
  assert actual==wanted;self.check();return actual
 def start(self,ch,direction=0,length=64,end=1,trigger=0):
  for index,value in enumerate((0x800100,0xc900000,length|(end<<31),direction,trigger,1,1)):
   self.guest_write(0xa05f7800+ch*32+index*4,value)
 def tick(self):
  actual=self.lib.sa_transfer_runtime_tick(ctypes.byref(self.binding));wanted=self.r.tick(self.ram[1],lambda a:self.read(1,a,4),lambda a,v:self.write(1,a,v))
  assert actual==wanted;self.check();return actual
 def ready(self):
  ready=U(99);actual=self.lib.sa_transfer_runtime_irq_ready(ctypes.byref(self.binding),ctypes.byref(ready));wanted=0
  if self.r.enabled and self.r.pending:
   status,mask=self.read(1,0xa05f6920,4);wanted=bool(mask&self.r.pending)
  else:status=0
  assert actual==status and ready.value==wanted;self.check();return wanted
 def check(self):
  assert model.snapshot(self.n)==self.r.snapshot() and self.events[0]==self.events[1]
  assert self.words[0]==self.words[1] and self.ram[0]==self.ram[1] and self.sound[0]==self.sound[1]

def main():
 dll=ROOT/'work/sonic-adventure/build/movie_transfer_runtime.dll';lib=ctypes.CDLL(str(dll))
 lib.sa_transfer_runtime_init.argtypes=[ctypes.POINTER(model.State),U];lib.sa_transfer_runtime_bind.argtypes=[ctypes.POINTER(Binding),ctypes.POINTER(model.State),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(Bus),ctypes.POINTER(Bus)]
 lib.sa_transfer_runtime_tick.argtypes=[ctypes.POINTER(Binding)];lib.sa_transfer_runtime_irq_ready.argtypes=[ctypes.POINTER(Binding),ctypes.POINTER(U)]
 records=[]
 def save(name,s):s.check();records.append(dict(name=name,state=s.r.snapshot(),transactions=len(s.events[0])))
 for ch,direction,end,length in itertools.product(range(4),(0,1),(0,1),(0,32,96)):
  s=Scenario(lib);s.start(ch,direction,length,end);before=s.ram[0][0x900000:0x900000+length] if not direction else s.sound[0][0x100:0x100+length]
  assert s.tick()==0 and s.n.transfers==1 and s.n.pending==1<<(15+ch) and s.n.bytes==length
  result=s.sound[0][0x100:0x100+length] if not direction else s.ram[0][0x900000:0x900000+length];assert result==before
  assert s.tick()==0 and s.n.transfers==1
  assert not s.ready();s.guest_write(0xa05f6920,1<<(15+ch));assert s.ready()
  s.guest_write(0xa05f6900,1<<(15+ch));assert not s.ready() and not s.n.pending
  s.start(ch,direction,length,end);assert s.tick()==0 and s.n.transfers==2;save('copy-repeat-%d-%d-%d-%d'%(ch,direction,end,length),s)
 for kind in ('disabled','cancel-enable','cancel-start','hardware-trigger','nested-irq','unaligned-length','invalid-root','write-failure-retry','read-failure-retry','completion-store-failure-retry','two-channels'):
  s=Scenario(lib,kind!='disabled');s.start(0,1 if kind=='read-failure-retry' else 0,33 if kind=='unaligned-length' else 64,trigger=2 if kind=='hardware-trigger' else 0)
  if kind=='cancel-enable':s.guest_write(0xa05f7814,0)
  if kind=='cancel-start':s.guest_write(0xa05f7818,0)
  if kind=='nested-irq':s.n.in_irq=s.r.in_irq=1
  if kind=='invalid-root':s.n.jobs[0].root_address=s.r.jobs[0][2]=0xcffffe0
  if kind=='write-failure-retry':s.failure=('w',0x800124)
  if kind=='read-failure-retry':s.failure=('r',0x800124)
  if kind=='completion-store-failure-retry':s.failure=('w',0xa05f7818)
  if kind=='two-channels':s.start(3)
  status=s.tick()
  if 'failure-retry' in kind:
   assert status==-9 and not s.n.pending;s.failure=None;assert s.tick()==0 and s.n.pending==1<<15 and s.n.transfers==1
  elif kind in ('unaligned-length','invalid-root'):assert status==-2 and not s.n.pending
  elif kind=='two-channels':assert status==0 and s.n.transfers==2 and s.n.pending==(1<<15|1<<18)
  else:assert status==0 and s.n.transfers==0 and not s.n.pending
  save(kind,s)
 files=[Path(__file__),P/'transfer_runtime_reference.py',P/'source/transfer_runtime.c',P/'source/transfer_runtime.h',P/'source/g2_transfer_host.c',P/'source/g2_transfer_host.h']
 report=dict(cases_passed=len(records),cases=records,dll_sha256=hashlib.sha256(dll.read_bytes()).hexdigest(),newly_registered_entries=0,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Authored persistent untimed platform only; no recovered-game credit. Four channels, both directions, reset/preserve enable, zero/single/multiple blocks, persistent repeat, cancellation, disabled/HW-trigger/nested guards, masked/acknowledged pending status, actual whole memory bytes and every bus transaction compare independently. Partial read/write/completion-store failures retain real effects and retry without emitting completion early. No physical timing/security-window/suspend/pin mechanism is claimed.')
 (P/'analysis/transfer_runtime_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Persistent platform cases',len(records))
if __name__=='__main__':main()
