"""Explicit other-client queue activity; no firmware/game return substitution."""
import ctypes,types
from disc_mount_platform_reference import DiscReference as PreviousDiscReference

class DiscReference(PreviousDiscReference):
 def __init__(self,root,c):
  super().__init__(root,c);self.actions=[];self.pumps=0
  if c.get('empty_media'):self.sectors=0
  for i in range(c.get('owned',0)):assert self.submit(self.base,0,None,0)==i+1
  # Setup must not consume the scheduled runtime cancellation input.
  for i in range(c.get('owned',0)):assert PreviousDiscReference.service(self,bytearray(),2,0,0)==(0,0)
 def service(self,m,op,a,b):
  if op==0 and a==24:
   for i in range(self.c.get('fill_before_command',0)):
    h=self.submit(self.base,0,None,0);assert h;self.actions.append(('submit',h))
  if op==2:
   self.pumps+=1
   if self.pumps==self.c.get('cancel_pump',-1):
    job=next(j for j in self.jobs if j[1]==1);handle=job[0]
    assert super().service(m,8,handle,0)==(0,0);self.actions.append(('cancel',handle))
  return super().service(m,op,a,b)

def native_scenario(v,c):
 """Call real exported queue APIs after recording the service-entry snapshot."""
 effects=types.SimpleNamespace(**v.effects.__dict__);lib=effects.LIB;queue=v.platform.Queue;u=ctypes.c_uint32
 lib.sa_disc_queue_submit.argtypes=[ctypes.POINTER(queue),u,u,ctypes.POINTER(ctypes.c_uint8),ctypes.c_uint64,ctypes.POINTER(u)];lib.sa_disc_queue_submit.restype=ctypes.c_int
 lib.sa_disc_queue_step.argtypes=[ctypes.POINTER(queue)];lib.sa_disc_queue_step.restype=ctypes.c_int
 lib.sa_disc_queue_cancel.argtypes=[ctypes.POINTER(queue),u];lib.sa_disc_queue_cancel.restype=ctypes.c_int
 actions=[];errors=[]
 def invoke(*args):
  args=list(args);q=ctypes.cast(args[12],ctypes.POINTER(queue));pumps=[0];observe=args[7]
  for i in range(c.get('owned',0)):
   handle=u();assert lib.sa_disc_queue_submit(q,q.contents.track_lba,0,None,0,ctypes.byref(handle))==0 and handle.value==i+1
  for i in range(c.get('owned',0)):assert lib.sa_disc_queue_step(q)==0
  @effects.OBSERVE
  def callback(ctx,target,mem,fp,sr,r,pr):
   try:
    observe(ctx,target,mem,fp,sr,r,pr)
    if target!=args[13]:return
    if r[7]==0 and r[4]==24:
     for i in range(c.get('fill_before_command',0)):
      handle=u();assert lib.sa_disc_queue_submit(q,q.contents.track_lba,0,None,0,ctypes.byref(handle))==0;actions.append(('submit',handle.value))
    if r[7]==2:
     pumps[0]+=1
     if pumps[0]==c.get('cancel_pump',-1):
      job=next(j for j in q.contents.jobs if j.state==1);handle=job.handle
      assert lib.sa_disc_queue_cancel(q,handle)==0;actions.append(('cancel',handle))
   except BaseException as exc:errors.append(repr(exc))
  args[7]=callback;result=v.effects.fn(*args);assert not errors,errors;return result
 effects.fn=invoke
 return effects,actions
