"""Retain the current instruction observer and append render-job state."""
from renderer_frame_clock_reference_code import extend as clock_extend
def extend(code):
 code=clock_extend(code)
 marker="(queue_reference.frame_clock.snapshot() if getattr(queue_reference,'frame_clock',None) is not None else None)\n def stop("
 assert code.count(marker)==1
 return code.replace(marker,marker.replace('\n def stop(',",(queue_reference.render_completion.snapshot() if getattr(queue_reference,'render_completion',None) is not None else None)\n def stop("))
