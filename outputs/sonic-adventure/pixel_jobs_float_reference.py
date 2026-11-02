"""Independent snapshot ownership and rational host rendering before completion."""
import struct
import pvr_pixels_float_reference as p
from pixel_jobs_native import State,Seed,image_digests
class Jobs:
 def __init__(self,seed):
  s=State.from_buffer_copy(seed.state)
  self.job_id=s.job_id;self.processed=s.processed;self.last_job=s.last_job;self.cursor=s.cursor;self.count=s.count;self.ready=s.ready;self.words=list(s.words);self.stats=tuple(getattr(s.stats,n) for n,_ in s.stats._fields_)
  self.pixels=seed.pixels;self.depth=seed.depth;self.options=seed.options;self.digests=image_digests(self.pixels,self.depth)
 def snapshot(self):return self.job_id,self.processed,self.last_job,self.cursor,self.count,self.ready,tuple(self.words),self.stats
 def execute(self,render,vram):
  if not self.ready or not render.enabled:return 0
  if self.job_id!=render.active_id or not self.job_id or self.count>64:return -6
  from types import SimpleNamespace
  names=('width','height','input_width','input_height','textures','mip_level','cull_area','alpha_ref');o=SimpleNamespace(**dict(zip(names,self.options)))
  if render.job[7]&65535 or render.job[8]&65535 or o.input_width!=((render.job[7]>>16)&1023)+1 or o.input_height!=((render.job[8]>>16)&1023)+1:return -2
  bg=[0xff101018]*(o.width*o.height);depth=[0]*len(bg)
  rc,pixels,z,stats=p.draw(self.words[:self.count*8],vram,o,bg,depth) if self.count else (0,bg,depth,(0,)*5)
  if rc:return rc
  assert render.finish(self.job_id)==0
  self.processed+=1;self.last_job=self.job_id;self.ready=0;self.stats=stats
  self.pixels=struct.pack('<'+'I'*len(pixels),*pixels);self.depth=struct.pack('<'+'I'*len(z),*z);self.digests=image_digests(self.pixels,self.depth);return 0
 def committed(self,a,value,width,render,packets):
  physical=a&0x1fffffff
  if physical==0x5f8014 and value==0xffffffff and render.enabled:
   assert width==4 and not self.ready and self.cursor<=len(packets)<=64
   selected=packets[self.cursor:];self.words=[word for _,_,words in selected for word in words]
   self.count=len(selected);self.words += [0]*(512-len(self.words));self.cursor=len(packets);self.ready=1;self.job_id=render.submitted+1
  if physical==0x5f8008 and width==4 and value&2:self.ready=0
