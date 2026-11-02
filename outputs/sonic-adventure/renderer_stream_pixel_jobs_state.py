"""Independent complete-list ownership and rational raster oracle for paged jobs."""
import hashlib,struct
from types import SimpleNamespace
import pvr_pixels_stream_reference as pixels
from pixel_banked_reference import texture_view
from stream_pixel_jobs_native import State,image_digests
class ReferenceJobs:
 def __init__(self,seed):
  state=State.from_buffer_copy(seed.state)
  assert not state.snapshot and len(seed.packets)==state.count
  for name in ('job_id','processed','last_job','cursor','count','ready'):setattr(self,name,getattr(state,name))
  self.words=[w for _,_,row in seed.packets for w in row];self.stats=tuple(getattr(state.stats,n) for n,_ in state.stats._fields_)
  self.pixels=seed.pixels;self.depth=seed.depth;self.options=seed.options;self.digests=image_digests(self.pixels,self.depth);self.word_digest=self.digest()
 def digest(self):return hashlib.sha256(struct.pack('<'+'I'*len(self.words),*self.words)).digest()
 def snapshot(self):return self.job_id,self.processed,self.last_job,self.cursor,self.count,self.ready,tuple(self.words),self.stats
 def observation(self):return self.job_id,self.processed,self.last_job,self.cursor,self.count,self.ready,self.word_digest,self.stats
 def execute(self,render,vram):
  if not self.ready or not render.enabled:return 0
  if self.job_id!=render.active_id or not self.job_id or self.count>0xffffffff//8:return -6
  o=SimpleNamespace(**dict(zip(('width','height','input_width','input_height','textures','mip_level','cull_area','alpha_ref'),self.options)))
  if render.job[7]&65535 or render.job[8]&65535 or o.input_width!=((render.job[7]>>16)&1023)+1 or o.input_height!=((render.job[8]>>16)&1023)+1:return -2
  if o.textures and len(vram)!=0x800000:return -3
  bg=[0xff101018]*(o.width*o.height);z=[0]*len(bg)
  rc,image,depth,stats=pixels.draw(self.words,texture_view(vram) if o.textures else None,o,bg,z) if self.count else (0,bg,z,(0,)*5)
  if rc:return rc
  assert render.finish(self.job_id)==0;self.processed+=1;self.last_job=self.job_id;self.ready=0;self.stats=stats
  self.pixels=struct.pack('<'+'I'*len(image),*image);self.depth=struct.pack('<'+'I'*len(depth),*depth);self.digests=image_digests(self.pixels,self.depth);return 0
 def committed(self,address,value,width,render,packets):
  physical=address&0x1fffffff
  if physical==0x5f8014 and value==0xffffffff and render.enabled:
   assert width==4 and not self.ready and self.cursor<=len(packets)<=0xffffffff
   selected=packets[self.cursor:];self.words=[word for _,_,row in selected for word in row]
   self.count=len(selected);self.cursor=len(packets);self.ready=1;self.job_id=render.submitted+1;self.word_digest=self.digest()
  if physical==0x5f8008 and width==4 and value&2:self.ready=0
