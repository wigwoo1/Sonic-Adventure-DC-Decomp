"""Native render-start snapshots, generation reuse and completion/failure guards."""
import ctypes,hashlib,json,struct
from pathlib import Path
from types import SimpleNamespace
P=Path(__file__).resolve().parent
parent=P/'verify_ta_stream_ownership.py';source=parent.read_text().replace('import ta_stream_native as n','import ta_job_stream_native as n')
fixture={'__name__':'retained_page_fixture','__file__':str(parent)};exec(compile(source,str(parent),'exec'),fixture)
import stream_pixel_jobs_native as j
import render_completion_native as r
import pvr_pixels_native as p
import pvr_pixels_stream_reference as oracle
def bits(value):return int.from_bytes(struct.pack('<f',value),'little')
def main():
 target=P/'analysis/stream_pixel_jobs_verification.json';assert not target.exists();checks=0
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 f=fixture['Fixture'](custom=True);state=j.State()
 try:
  rows=[(0x80000000,7<<29,0,0,0,0,0,0)]
  for triangle in range(100):
   for i,(x,y) in enumerate(((1,1),(1,14),(14,1))):rows.append((0xf0000000 if i==2 else 0xe0000000,bits(x),bits(y),bits(1),0,0,0xff33aa99,0))
  rows.append((0,1,2,3,4,5,6,7))
  for row in rows:require(f.sq(row)==0,'accept declared synthetic source packet')
  require(f.count()==302 and f.pages()==5,'complete source spans five original-sized pages')
  params=[0]*16;params[7]=params[8]=15<<16;render=r.initialized(params,lib=j.LIB)
  binding=j.Binding();capture=j.Bus();parent_bus=j.Bus.from_buffer_copy(bytes(f.bus))
  require(j.LIB.sa_stream_pixel_job_bind(ctypes.byref(binding),ctypes.byref(state),ctypes.byref(render),f.handle,ctypes.byref(parent_bus),ctypes.byref(capture))==0,'bind owned snapshot below render lifecycle')
  outer=r.Bus();rb=r.Binding();require(j.LIB.sa_render_completion_bind(ctypes.byref(rb),ctypes.byref(render),ctypes.byref(capture),ctypes.byref(outer))==0,'original render-start contract remains outermost')
  baseline=bytes(state),bytes(render),f.state(),len(f.owned)
  f.reject=-8
  require(outer.write(outer.context,0xa05f8014,0xffffffff,4)==-8 and baseline==(bytes(state),bytes(render),f.state(),len(f.owned)),'parent failure releases candidate and preserves old job')
  f.reject=0;f.budget=0
  require(outer.write(outer.context,0xa05f8014,0xffffffff,4)==-4 and baseline==(bytes(state),bytes(render),f.state(),len(f.owned)),'snapshot-owner allocation failure cannot submit')
  f.budget=2
  require(outer.write(outer.context,0xa05f8014,0xffffffff,4)==-4 and baseline==(bytes(state),bytes(render),f.state(),len(f.owned)),'later page allocation failure releases all temporary ownership')
  f.budget=None
  require(outer.write(outer.context,0xa05f8014,0xffffffff,4)==0 and state.ready==1 and state.job_id==render.active_id==1 and state.count==302 and render.pending==0,'real strobe commits complete immutable snapshot before core submission')
  saved=j.packets(state)
  require(outer.write(outer.context,0xa05f8014,0xffffffff,4)==-4 and j.packets(state)==saved,'busy submission preserves the in-flight job')
  for row in (rows[0],*rows[1:4],rows[-1]):require(f.sq(row)==0,'accept newer active-generation packets')
  require(j.packets(state)==saved and f.count()==307,'later capture cannot alias immutable job pages')
  require(outer.write(outer.context,0xa05f8144,0x80000000,4)==0 and not f.count() and f.count(1)==307 and f.pages(1)==5 and state.cursor==0 and j.packets(state)==saved,'guest init retires current pages while submitted ownership remains')
  require(f.state()[1][2]==5,'only packets accepted after submission are discarded by list init')
  pixels=(p.U*256)(*([0xff101018]*256));depth=(p.U*256)();options=p.Options(16,16,16,16,0,0,0,128)
  before=bytes(pixels),bytes(depth),bytes(render);render.active_id=9
  wrong=bytes(render)
  require(j.LIB.sa_stream_pixel_job_execute(ctypes.byref(state),ctypes.byref(render),None,0,pixels,depth,256,ctypes.byref(options))==-6 and bytes(render)==wrong and (bytes(pixels),bytes(depth))==before[:2],'wrong job identifier never generates completion')
  render.active_id=1;bad=p.Options(16,16,8,16,0,0,0,128)
  require(j.LIB.sa_stream_pixel_job_execute(ctypes.byref(state),ctypes.byref(render),None,0,pixels,depth,256,ctypes.byref(bad))==-2 and (bytes(pixels),bytes(depth),bytes(render))==before,'unsupported viewport preserves queued job and image')
  f.budget=0
  require(j.LIB.sa_stream_pixel_job_execute(ctypes.byref(state),ctypes.byref(render),None,0,pixels,depth,256,ctypes.byref(options))==-4 and (bytes(pixels),bytes(depth),bytes(render))==before,'materialization allocation failure never acknowledges a job')
  f.budget=None
  o=SimpleNamespace(**dict(zip(('width','height','input_width','input_height','textures','mip_level','cull_area','alpha_ref'),(16,16,16,16,0,0,0,128))))
  rc,expected,z,stats=oracle.draw([w for row in rows for w in row],None,o,[0xff101018]*256,[0]*256);require(rc==0,'independent rational oracle accepts complete cross-page strips')
  require(j.LIB.sa_stream_banked_pixel_job_execute(ctypes.byref(state),ctypes.byref(render),None,0,None,0,pixels,depth,256,ctypes.byref(options))==0,'Texture Off executes without VRAM or conversion workspace')
  require(list(pixels)==expected and list(depth)==z and p.snapshot(state.stats)==stats and stats[0]==100,'full colour/depth/statistics agree beyond page boundaries')
  require(state.processed==state.last_job==render.completed==1 and not state.ready and render.pending==7,'all completion flags follow successful full native output')
  repeated=bytes(state),bytes(render),bytes(pixels),bytes(depth)
  require(j.LIB.sa_stream_pixel_job_execute(ctypes.byref(state),ctypes.byref(render),None,0,pixels,depth,256,ctypes.byref(options))==0 and repeated==(bytes(state),bytes(render),bytes(pixels),bytes(depth)),'polling cannot process or complete a job twice')
  require(outer.write(outer.context,0xa05f6900,7,4)==0 and render.pending==0,'guest acknowledgement retains lifecycle ownership')
  require(f.sq((0x80000001,0,0,0,0,0,0,0))==0,'retain accepted but raster-unsupported header')
  require(outer.write(outer.context,0xa05f8014,0xffffffff,4)==0 and state.job_id==2 and state.count==1,'next generation starts a distinct native-owned job')
  before=bytes(pixels),bytes(depth),bytes(render)
  require(j.LIB.sa_stream_pixel_job_execute(ctypes.byref(state),ctypes.byref(render),None,0,pixels,depth,256,ctypes.byref(options))==-2 and before==(bytes(pixels),bytes(depth),bytes(render)) and state.ready==1,'unsupported input yields neither partial pixels nor fake completion')
  require(outer.write(outer.context,0xa05f8008,2,4)==0 and not state.ready and not render.active_id and render.cancelled==1 and render.completed==1,'reset cancels pending ownership without completion')
 finally:
  j.LIB.sa_stream_pixel_job_dispose(ctypes.byref(state));f.close()
 files=[Path(__file__),parent,P/'source/stream_pixel_jobs.c',P/'source/stream_pixel_jobs.h',P/'source/ta_stream_jobs_store.c',P/'source/pvr_pixels_stream.c',P/'stream_pixel_jobs_native.py',P/'pvr_pixels_stream_reference.py',P/'ta_job_stream_native.py',P/'build_model_ta_jobs.ps1',j.DLL]
 report=dict(status='verified_native_paged_job_submission_retirement_raster_failures_and_cancellation',checks=checks,synthetic_source_packets=302,source_pages=5,synthetic_triangles=100,independent_full_colour_depth_stats_match=True,allocation_failures_release_temporary_pages=True,all_tracked_allocations_released=True,unsupported_jobs_complete=False,Texture_Off_needs_no_VRAM=True,game_callee_results_injected=False,original_game_frame_claimed=False,overall_completion_percent=10,goal_paused=False,sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('STREAM PIXEL JOB OWNERSHIP VERIFIED',checks,'checks',flush=True)
if __name__=='__main__':main()
