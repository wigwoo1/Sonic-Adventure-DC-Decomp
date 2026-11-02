"""Retain all original comparisons; execute and compare actual native pixel jobs."""
import ctypes
import float_color_pixels_native as jobs
import pvr_pixels_native as pixels
from renderer_float_color_jobs_state import ReferenceJobs
from renderer_pixel_jobs_state import native_observation

def watched(parent):
 def build(*args):
  c=args[-1];assert not c.get('backend_completed_job',0),'scripted completion is forbidden in the pixel-work verifier'
  ref=parent(*args);seed=c.get('pixel_job_seed');ref.pixel_jobs=None if seed is None else ReferenceJobs(seed)
  if ref.pixel_jobs is None:return ref
  assert ref.render_completion is not None
  assert ref.pixel_jobs.execute(ref.render_completion,ref.vram)==0,'reference pixel work must succeed before a completion event'
  clock_class=next(k for k in type(ref).__mro__ if k.__name__=='RenderBelowClock');under_render=clock_class.__bases__[0]
  class PixelBelowRender(under_render):
   def __setitem__(self,a,value):
    result=super().__setitem__(a,value)
    self.pixel_jobs.committed(a,value,self.access_width,self.render_completion,self.packets)
    return result
  class PixelReference(type(ref),PixelBelowRender):pass
  ref.__class__=PixelReference
  return ref
 return build

def extend(code):
 def change(a,b):
  nonlocal code
  assert code.count(a)==1,(a,code.count(a));code=code.replace(a,b)
 change('def compare(m,s,f,c):', '''def compare(m,s,f,c):
 assert not c.get('backend_completed_job',0),'no scripted backend acknowledgements'
 pixel_seed=c.get('pixel_job_seed')
 old.native_pixel_job=job_model.restored(pixel_seed) if pixel_seed is not None else None
 old.native_pixel_digests=None
 if pixel_seed is not None:
  old.native_pixel_options=pixel_model.Options(*pixel_seed.options)
  old.native_pixel_scratch=(pixel_model.B*0x800000)()
  pixel_count=pixel_seed.options[0]*pixel_seed.options[1]
  # Stage a new host image for real work; retain the exact last image on a no-op.
  work=bool(old.native_pixel_job.ready and c['render_completion_seed']['enabled'])
  old.native_pixel_pixels=(pixel_model.U*pixel_count).from_buffer_copy(bytes.fromhex('181010ff')*pixel_count if work else pixel_seed.pixels)
  old.native_pixel_depth=(pixel_model.U*pixel_count)() if work else (pixel_model.U*pixel_count).from_buffer_copy(pixel_seed.depth)
''')
 marker=' restore_native(memory,pvr_state,sound_view,c)'
 change(marker,marker+'''
 old.native_pixel_vram=memory.vram
 old.native_pixel_frame=ctypes.byref(memory.frame)
''')
 marker='render_model.snapshot(old.native_render_completion) if old.native_render_completion is not None else None))'
 change(marker,'render_model.snapshot(old.native_render_completion) if old.native_render_completion is not None else None,job_observation(old.native_pixel_job) if old.native_pixel_job is not None else None,old.native_pixel_digests))')
 change(' journal.check()', ''' if old.native_pixel_job is not None:
  assert job_model.snapshot(old.native_pixel_job)==ref.pixel_jobs.snapshot(),'complete immutable pixel-job state'
  assert bytes(old.native_pixel_pixels)==ref.pixel_jobs.pixels,'complete colour image equality'
  assert bytes(old.native_pixel_depth)==ref.pixel_jobs.depth,'complete depth image equality'
 journal.check()''')
 change(' return dict(',''' pixel_out=job_model.Seed(bytes(old.native_pixel_job),bytes(old.native_pixel_pixels),bytes(old.native_pixel_depth),pixel_seed.options) if pixel_seed is not None else None
 return dict(pixel_job_out=pixel_out,pixel_job_summary=pixel_out.summary() if pixel_out is not None else None,''')
 return code
