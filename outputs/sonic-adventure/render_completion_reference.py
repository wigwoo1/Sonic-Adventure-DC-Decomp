"""Independent host job lifecycle oracle; no game-code hooks or pixel renderer."""
OFFSETS=(0x20,0x2c,0x30,0x48,0x4c,0x60,0x64,0x68,0x6c,0x74,0x78,0x7c,0x80,0x84,0x88,0x8c)
MAX64=(1<<64)-1
class Completion:
 def __init__(self,parameters=None):
  self.submitted=self.completed=self.cancelled=self.active_id=self.pending=0;self.enabled=1
  self.parameters=list(parameters or [0]*16);self.job=[0]*16
 def finish(self,token):
  if not self.enabled or token==0 or token!=self.active_id or self.completed==MAX64:return -6
  self.completed+=1;self.active_id=0;self.pending|=7;return 0
 def check(self,a,value,width):
  p=a&0x1fffffff
  if p in (0x5f8014,0x5f8008,0x5f6900) or p-0x5f8000 in OFFSETS:
   if width!=4:return -3
  if p==0x5f8014:
   if value not in (0,0xffffffff):return -2
   if value and self.enabled:
    if self.active_id or self.pending:return -4
    if self.submitted==MAX64:return -6
  if p==0x5f8008 and value&2 and self.active_id and self.cancelled==MAX64:return -6
  return 0
 def committed(self,a,value,width):
  p=a&0x1fffffff
  if p-0x5f8000 in OFFSETS:self.parameters[OFFSETS.index(p-0x5f8000)]=value
  if p==0x5f6900:self.pending&=~value
  if p==0x5f8008 and value&2 and self.active_id:self.active_id=0;self.cancelled+=1
  if p==0x5f8014 and value and self.enabled:self.submitted+=1;self.active_id=self.submitted;self.job=list(self.parameters)
 def snapshot(self):return tuple(getattr(self,n) for n in ('submitted','completed','cancelled','active_id','pending','enabled'))+(tuple(self.parameters),tuple(self.job))
 @classmethod
 def restore(cls,d):
  s=cls()
  for k,v in d.items():setattr(s,k,list(v) if k in ('parameters','job') else v)
  return s
