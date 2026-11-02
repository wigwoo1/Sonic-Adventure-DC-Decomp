"""Append collector lifecycle observations without changing instruction semantics."""
from renderer_pixel_jobs_reference_code import extend as previous
def extend(code):
 code=previous(code)
 marker="(queue_reference.pixel_jobs.digests if getattr(queue_reference,'pixel_jobs',None) is not None else None)\n def stop("
 assert code.count(marker)==1
 return code.replace(marker,marker.replace('\n def stop(',",queue_reference.ta_lists.observation()\n def stop("))
