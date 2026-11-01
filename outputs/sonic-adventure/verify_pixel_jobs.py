"""Capture real ABI packets immutably and finish only after successful pixels."""
import ctypes,hashlib,json,struct
from pathlib import Path
import pixel_jobs_native as j
import pvr_pixels_native as p
import render_completion_native as r
import write_watch_renderer_render_completion as old
P=Path(__file__).resolve().parent;R=P.parents[1]
def b(v):return int.from_bytes(struct.pack('<f',v),'little')
def main():
 target=P/'analysis/pixel_jobs_verification.json';assert not target.exists();checks=0
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 memory=old.namespace['RtcMemory']();frame=memory.frame;frame.packet_capacity=64
 words=[0x82800028,7<<29,(1<<29)|(2<<22)|(1<<20),0xffffffff,*([b(1)]*4)]
 for i,(x,y) in enumerate(((1,1),(1,7),(7,1),(7,7))):words += [0xf0000000 if i==3 else 0xe0000000,b(x),b(y),b(1),b(0),b(0),b(1),0]
 frame.packet_count=5
 for i in range(5):frame.packets[i].words[:]=words[8*i:8*i+8]
 state=j.State();j.LIB.sa_pixel_job_init(ctypes.byref(state));params=[0]*16;params[7]=params[8]=7<<16
 render=r.initialized(params,lib=j.LIB);fail=[0];calls=[]
 @r.READ
 def read(ctx,a,n,out):out[0]=0;return 0
 @r.WRITE
 def write(ctx,a,value,n):calls.append((a,value,n));return fail[0]
 @r.CACHE
 def cache(ctx,a,op):return -9
 parent=r.Bus(None,read,write,cache);capture=r.Bus();binding=j.Binding()
 require(j.LIB.sa_pixel_job_bind(ctypes.byref(binding),ctypes.byref(state),ctypes.byref(render),ctypes.byref(frame),ctypes.byref(parent),ctypes.byref(capture))==0,'capture ABI binding')
 outer=r.Bus();rb=r.Binding();require(j.LIB.sa_render_completion_bind(ctypes.byref(rb),ctypes.byref(render),ctypes.byref(capture),ctypes.byref(outer))==0,'render ownership outside capture')
 fail[0]=-8;saved=bytes(state),bytes(render)
 require(outer.write(outer.context,0xa05f8014,0xffffffff,4)==-8 and saved==(bytes(state),bytes(render)),'parent failure cannot submit or capture a job')
 fail[0]=0;require(outer.write(outer.context,0xa05f8014,0xffffffff,4)==0 and state.ready==1 and state.job_id==render.active_id==1 and state.count==5 and render.pending==0,'real start captures accepted packet interval before core submission')
 frame.packets[1].words[1]=b(999)
 require(list(state.words[:40])==words,'captured command input does not alias reused TA storage')
 pixels=(p.U*64)();depth=(p.U*64)();options=p.Options(8,8,8,8,0,0,0,128)
 require(j.LIB.sa_pixel_job_execute(ctypes.byref(state),ctypes.byref(render),None,0,pixels,depth,64,ctypes.byref(options))==0,'execute actual native pixel work')
 require(sum(x!=0 for x in pixels)==36 and state.stats.pixels==36 and state.stats.texture_samples==0,'completion follows actual vertex-coloured output')
 require(state.processed==state.last_job==render.completed==1 and not state.ready and render.pending==7,'render stages are signalled after native output exists')
 saved=bytes(state),bytes(render),bytes(pixels),bytes(depth)
 require(j.LIB.sa_pixel_job_execute(ctypes.byref(state),ctypes.byref(render),None,0,pixels,depth,64,ctypes.byref(options))==0 and saved==(bytes(state),bytes(render),bytes(pixels),bytes(depth)),'repeated poll cannot render or finish a job twice')
 require(outer.write(outer.context,0xa05f6900,7,4)==0 and render.pending==0,'guest IRQ acknowledgement preserves backend ownership')
 # A new, unsupported packet snapshot must remain unacknowledged.
 frame.packet_count=6;frame.packets[5].words[:]=[words[0]|1,*words[1:8]]
 require(outer.write(outer.context,0xa05f8014,0xffffffff,4)==0 and state.count==1 and state.job_id==2,'next job owns only newly accepted packets')
 saved=bytes(pixels),bytes(depth),bytes(render)
 require(j.LIB.sa_pixel_job_execute(ctypes.byref(state),ctypes.byref(render),None,0,pixels,depth,64,ctypes.byref(options))==-2 and saved==(bytes(pixels),bytes(depth),bytes(render)) and state.ready==1,'unsupported geometry never generates fake completion or partial pixels')
 require(outer.write(outer.context,0xa05f8008,2,4)==0 and not state.ready and not render.active_id,'engine reset cancels queued work')
 files=[Path(__file__),P/'source/pixel_jobs.c',P/'source/pixel_jobs.h',P/'pixel_jobs_native.py',P/'pixel_jobs_reference.py',P/'build_pixel_jobs.ps1',P/'source/pvr_pixels.c',P/'source/pvr_pixels.h',P/'source/render_completion.c',P/'source/render_completion.h',j.DLL]
 report=dict(status='immutable_native_pixel_jobs_and_completion_after_work_verified',checks=checks,frame_ABI_from_existing_verified_renderer=True,unsupported_jobs_complete=False,texture_Off_vertex_pixels=36,physical_GPU_accuracy_qualified=False,overall_completion_percent=10,new_original_routine_credit=0,sha256={str(f):hashlib.sha256(f.read_bytes()).hexdigest() for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('VERIFIED pixel jobs',checks,'checks',flush=True)
if __name__=='__main__':main()
