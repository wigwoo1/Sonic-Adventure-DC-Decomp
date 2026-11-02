"""Append native pixel job ownership and host image observations, retaining guards."""
from renderer_render_completion_reference_code import extend as previous

def extend(code):
 code=previous(code)
 marker="(queue_reference.render_completion.snapshot() if getattr(queue_reference,'render_completion',None) is not None else None)\n def stop("
 assert code.count(marker)==1
 return code.replace(marker,marker.replace('\n def stop(',",pixel_job_observation(getattr(queue_reference,'pixel_jobs',None)),(queue_reference.pixel_jobs.digests if getattr(queue_reference,'pixel_jobs',None) is not None else None)\n def stop("))
