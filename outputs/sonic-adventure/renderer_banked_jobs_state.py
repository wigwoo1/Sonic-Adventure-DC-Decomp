"""Render the independent bank-linear game state through the texture address view."""
from renderer_pixel_jobs_state import ReferenceJobs as Previous
from pixel_banked_reference import texture_view
class ReferenceJobs(Previous):
 def execute(self,render,vram):
  if not self.ready or not render.enabled:return 0
  if self.options[4] and len(vram)!=0x800000:return -3
  return super().execute(render,texture_view(vram) if self.options[4] else None)
