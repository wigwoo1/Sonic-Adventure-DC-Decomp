"""Independent guest list-init transition; no host-only queue reset is exposed."""
import hashlib,struct
from ta_list_lifecycle_native import COUNTERS,FIELDS
class Lifecycle:
 def __init__(self,seed=None,enabled=True):
  for name in COUNTERS+FIELDS:setattr(self,name,seed[name] if seed is not None else 0)
  self.enabled=int(bool(enabled)) if seed is None else seed['enabled']
  self.last=[] if seed is None else [(a,q,tuple(w)) for a,q,w in seed['last']]
  self.update_digest()
 def update_digest(self):
  raw=b''.join(struct.pack('<10I',a,q,*w) for a,q,w in self.last);self.word_digest=hashlib.sha256(raw+bytes(max(0,2560-len(raw)))).digest()
 def check(self,a,value,width,packets,capacity,events,job):
  if not self.enabled:return 0
  address=a&0x1fffffff
  if address in (0x5f8008,0x5f8124,0x5f8128,0x5f8144) and width!=4:return -3
  if address==0x5f8008 and value&1 and self.soft_resets==2**64-1:return -6
  if address==0x5f8144 and value&(2**31):
   count=len(packets);cursor=job.cursor if job is not None else 0
   if count>0xffffffff or cursor>count or events.cursor>count:return -6
   if self.inits==2**64-1 or self.retired+count>=2**64 or self.discarded+count-cursor>=2**64:return -6
  return 0
 def committed(self,a,value,width,packets,events,job,owner=None):
  if not self.enabled:return
  address=a&0x1fffffff
  if address==0x5f8144 and value&(2**31):
   count=len(packets);cursor=job.cursor if job is not None else 0
   assert not events.enabled or events.cursor==count
   self.last=list(packets);self.last_count=count;self.inits+=1;self.retired+=count;self.discarded+=count-cursor;self.update_digest()
   packets.clear();events.cursor=events.list=events.active=0
   if job is not None:job.cursor=0
   if owner is not None:owner.ta_packet=None
  if address==0x5f8124:self.opb_base=value
  if address==0x5f8128:self.isp_base=value
  if address==0x5f8008 and value&1:self.soft_resets+=1
 def serialize(self):return dict(**{n:getattr(self,n) for n in COUNTERS+FIELDS},last=[[a,q,list(w)] for a,q,w in self.last])
 def snapshot(self):return tuple(getattr(self,n) for n in COUNTERS+FIELDS),tuple(self.last+[(0,0,(0,)*8)]*(64-len(self.last)))
 def observation(self):return tuple(getattr(self,n) for n in COUNTERS+FIELDS),self.word_digest
