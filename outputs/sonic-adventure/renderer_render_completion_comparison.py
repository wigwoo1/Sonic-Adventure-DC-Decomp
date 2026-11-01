"""Full old comparisons plus explicit backend completion and immutable job state."""
from render_completion_reference import Completion
from renderer_frame_pacing_trace import FIELDS,attach as attach_pacing
render_fields=tuple(sorted(set(FIELDS)|{0x8c8a2f34,0x8c8a2f5c,0x8c8a2f60,0x8c8a2d78}|set(range(0x8c8a3098,0x8c8a30d0,4))|set(range(0x8c8a3238,0x8c8a32c0,4))))
def watched(parent):
 def build(*args):
  ref=parent(*args);c=args[-1];seed=c.get('render_completion_seed')
  ref.render_completion=None if seed is None else Completion.restore(seed)
  if ref.render_completion is not None and c.get('backend_completed_job'):
   assert ref.render_completion.finish(c['backend_completed_job'])==0,'explicit backend acknowledgement must match a live job'
  clock_class=type(ref);under_clock=clock_class.__bases__[0]
  class RenderBelowClock(under_clock):
   def __getitem__(self,a):
    value=super().__getitem__(a)
    return value|self.render_completion.pending if self.render_completion is not None and a&0x1fffffff==0x5f6900 else value
   def __setitem__(self,a,value):
    model=self.render_completion
    if model is not None:assert model.check(a,value,self.access_width)==0,('render lifecycle write',hex(a),hex(value))
    result=super().__setitem__(a,value)
    if model is not None:model.committed(a,value,self.access_width)
    return result
  class CompleteReference(clock_class,RenderBelowClock):pass
  ref.__class__=CompleteReference
  return ref
 return build
def extend(code):
 def change(a,b):
  nonlocal code
  assert code.count(a)==1,(a,code.count(a));code=code.replace(a,b)
 change('def compare(m,s,f,c):','def compare(m,s,f,c):\n pacing_stores.clear()\n old.native_render_completion=render_model.restore(c[\'render_completion_seed\'],lib=effects.LIB) if c.get(\'render_completion_seed\') is not None else None\n old.native_backend_completed_job=c.get(\'backend_completed_job\',0)')
 marker="frame_model.snapshot(old.native_frame_clock) if old.native_frame_clock is not None else None))"
 change(marker,"frame_model.snapshot(old.native_frame_clock) if old.native_frame_clock is not None else None,render_model.snapshot(old.native_render_completion) if old.native_render_completion is not None else None))")
 marker="  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8]),sr=sr[0]))"
 change(marker,marker+'''
  if target in (0x8c641f00,0x8c6416ec,0x8c651bc0,0x8c652210,0x8c642440,0x8c651f1e,0x8c651570,0x8c6516a0,0x8c65194a) or target==vb.value+0x600:
   boundaries[-1]['pacing_input']={hex(a):int.from_bytes(ctypes.string_at(ctypes.addressof(mem.contents)+(a&0xffffff),4),'little') for a in pacing_fields}
''')
 change(' journal.check()'," if old.native_render_completion is not None:assert render_model.snapshot(old.native_render_completion)==ref.render_completion.snapshot(),'complete render-job ownership and pending events'\n journal.check()")
 change(' return dict('," return dict(render_completion_out=render_model.serialize(old.native_render_completion) if old.native_render_completion is not None else None,pacing_stores=list(pacing_stores),")
 return code
