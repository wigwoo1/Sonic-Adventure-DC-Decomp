"""Real timestamp/event ownership, four retained targets and lifecycle limits."""
import argparse,ctypes as C,hashlib,json,random,struct,threading
from pathlib import Path
from native_packet_test_support import bind
from native_gpu_test_support import GPU,Config,Report,Failure
from native_gpu_jobs_test_support import Jobs,Timing
from native_video_test_support import Video
from native_texture_test_support import Runtime,Info,Name,List,Record,field
U=C.c_uint32;I=C.c_int;P=C.c_void_p;Q=C.c_uint64;S=C.c_size_t
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def fw(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();b=json.loads((a.build/'build_verification.json').read_text());assert not a.report.exists()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert b['exit_code']==b['diagnostic_bytes']==0 and sha(Path(b['DLL']))==b['DLL_sha256']
    runtime=Runtime(Path(b['frames_DLL']));lib=runtime.dll;video=Video(lib)
    gpu=GPU(Path(b['DLL']));jobs=Jobs(gpu);gl=gpu.lib;checks=pending=timed=images=pixels=presents=0;timings=[];rng=random.Random(0x4a4f4253)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    maximum=(1<<64)-1;limit=maximum//781251
    cases=[(0,0,1),(0,1,1),(1,0,1),(0,maximum,1),(0,maximum,maximum),(0,maximum,limit),(0,maximum,limit+1),(0,100,0)]
    cases += [(rng.getrandbits(64),rng.getrandbits(64),rng.choice([1,781250,10000000,1000000000,limit])) for _ in range(2048)]
    for begin,end,hz in cases:
        out=Q(0xfeedface);rc=jobs.convert(begin,end,hz,C.byref(out));valid=end>=begin and 0<hz<=limit and (end-begin)*781250//hz<=maximum
        check((rc==0)==valid,'independent integer timing conversion validity')
        check(out.value==((end-begin)*781250//hz if valid else 0xfeedface),'timing conversion or unchanged failure output')
    check(jobs.convert(0,1,1,None)==1,'null timing conversion output')
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q)
    reserve=bind(lib,'jp_queue_reserve',I,U,U);ctx=bind(lib,'jp_queue_set_context',I,U,P);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None)
    end=bind(lib,'njEndVertex',None,P);pin=bind(lib,'jp_frames_prepare_texture',I,U);start=bind(lib,'njStartVertexTexOpaque',P)
    mode=bind(lib,'kmSetPaletteMode',I,I);palette=bind(lib,'kmSetPaletteData',I,P);clip=bind(lib,'kmSetPixelClipping',I,I,I,I,I)
    check(qi(2<<20)==0 and fi(8,8,4<<20)==0 and runtime.init(8,128,1<<20)==0 and video.init(64,64,0xff123456,1)==0,'actual frame/video/texture owners')
    check(mode(3)==0 and palette((U*1024)(*[0xff00ff00]*1024))==0,'actual palette')
    info=Info();s=info.texsurface;s.Type=0x10000;s.BitDepth=1;s.PixelFormat=6<<27;s.nWidth=s.nHeight=8;s.TextureSize=64;s.fSurfaceFlags=0x24
    check(runtime.copy(C.byref(info),C.create_string_buffer(bytes([7]*64)),64)==0,'actual encoded PAL surface')
    names=(Name*1)();runtime.setname(C.byref(names[0]),None,1000,0);runtime.entry(field(names[0]),C.byref(info),1000,2,0xffffffff);runtime.param(runtime.resolve(names[0].texaddr));tcw=Record.from_address(runtime.resolve(names[0].texaddr)).texparambuffer
    listing=List(names,1);check(runtime.select(C.byref(listing))==1,'actual texture selected')
    def record(n=1,color=0xff00ff00,left=False):
        check(video.write(0x101c,color)==0 and clip(0,0,31 if left else 63,63)==0,'new captured state')
        check(reset()==0 and reserve(0,1+n*3)==0 and ctx(0,(U*4)(0x8000000a,0xe0800000,0x20800000,tcw))==0 and pin(tcw&0x1fffff)==0,'actual multi-strip batch')
        get();ptr=start();check(bool(ptr),'actual packet writer');vertices=[]
        for _ in range(n):
            for i,(x,y) in enumerate([(-64,-64),(192,-64),(-64,192)]):vertices += [0xf0000000 if i==2 else 0xe0000000,fw(x),fw(y),fw(1),fw(.25),fw(.25),0xffff0000,0]
        raw=struct.pack('<'+'I'*len(vertices),*vertices);C.memmove(ptr,raw,len(raw));end(ptr+len(raw));put();fid=Q();check(submit(C.byref(fid))==0,'retained frame submission');return fid.value
    wo=bind(gl,'jp_window_open',I,P,U,U,I,P,P);wc=bind(gl,'jp_window_close',None,P);resize=bind(gl,'jp_window_resize',I,P,U,U,P);read=bind(gl,'jp_window_readback',I,P,P,S,P);count=bind(gl,'jp_window_presented',Q,P)
    window=P();e=Failure();check(wo(gpu.handle,64,64,1,C.byref(window),C.byref(e))==0,'real visible job presentation window')
    def launch(fid,config):
        before=jobs.now();id=Q(123);rep=Report();rc=jobs.submit(gpu.handle,fid,C.byref(config),C.byref(id),C.byref(rep),C.byref(e));after=jobs.now()
        check(rc==0,('real GPU submission',rc,e.hresult));return id.value,rep,before,after
    def wait(id):
        nonlocal pending,timed
        t,p=jobs.wait(id);pending+=p;timed+=t.gpu_timing_valid;timings.append({n:getattr(t,n) for n,_ in Timing._fields_});old=bytes(t);again=Timing()
        check(jobs.poll(gpu.handle,id,C.byref(again),C.byref(e))==0 and bytes(again)==old,'immutable completed timing');return t
    # Exact real per-job cost, with a frame left valid through both submissions.
    probe=record();config=Config.make();jid,rep,lo,hi=launch(probe,config);t=wait(jid);check(lo<=t.cpu_begin<=t.cpu_submitted<=hi,'CPU submission timestamps bracket actual call')
    check(jobs.release(gpu.handle,jid,C.byref(e))==0,'probe job retirement');config.budget=rep.accounted_payload_bytes-1;out=Q(456);saved=bytes(rep)
    check(jobs.submit(gpu.handle,probe,C.byref(config),C.byref(out),C.byref(rep),C.byref(e))==7 and out.value==456 and bytes(rep)==saved and jobs.live(gpu.handle)==0,'one byte short preserves ID/report/live slots')
    config.budget+=1;jid,rep,lo,hi=launch(probe,config);wait(jid);check(jobs.release(gpu.handle,jid,C.byref(e))==0 and release(probe)==0,'exact budget and input cleanup')
    for frame,c in [(0,Config.make()),(0,Config.make(0,64))]:
        check(jobs.submit(gpu.handle,frame,C.byref(c),C.byref(out),C.byref(rep),C.byref(e))!=0 and out.value==456 and bytes(rep)==saved,'invalid job preserves caller output')
    # Invalid frame IDs set the preserved frame owner's explicit sticky error.
    # Recover that owner after the negative test before asking it to pin again.
    bind(lib,'jp_frames_clear_error',None)();check(bind(lib,'jp_frames_status',I)()==0,'explicit recovery after invalid frame probe')
    # Four distinct targets/state generations coexist even when older jobs finish.
    live=[]
    for n,(color,left,off,size,overdraw) in enumerate([(0xff00ff00,False,False,2048,128),(0xffffff00,True,False,64,1),(0xff00ffff,False,True,64,1),(0xff00ffff,False,False,64,1)]):
        fid=record(overdraw,color,left);c=Config.make(size,size,textures=0 if off else 1);id,rr,lo,hi=launch(fid,c);live.append((id,size,color,left,off,rr))
        if n==0:
            sentinel=Timing();C.memset(C.byref(sentinel),0x5a,C.sizeof(sentinel));old=bytes(sentinel);rc=jobs.poll(gpu.handle,id,C.byref(sentinel),C.byref(e))
            check(rc in [0,100],'real heavy job ready or pending')
            if rc==100:check(bytes(sentinel)==old,'pending query preserves timing output');pending+=1
        check(release(fid)==0,'CPU retained frame retires immediately after submit')
    check(jobs.live(gpu.handle)==4 and len({v[0] for v in live})==4,'four independent live job IDs')
    c=Config.make();out=Q(456);saved=bytes(rep)
    check(jobs.submit(gpu.handle,0,C.byref(c),C.byref(out),C.byref(rep),C.byref(e))==102 and out.value==456 and bytes(rep)==saved,'fifth outstanding job rejected without losing older targets')
    runtime.shutdown();video.shutdown();qs();check(fs()==0,'all CPU source owners gone while targets remain owned')
    foreign=[]
    def other_thread():
        t=Timing();err=Failure();foreign.extend([jobs.poll(gpu.handle,live[0][0],C.byref(t),C.byref(err)),jobs.release(gpu.handle,live[0][0],C.byref(err))])
    thread=threading.Thread(target=other_thread);thread.start();thread.join();check(foreign==[1,1] and jobs.live(gpu.handle)==4,'foreign thread cannot poll or retire jobs')
    # A second real device cannot access the first device's tickets.
    other=GPU(Path(b['DLL']));other_jobs=Jobs(other);t=Timing();C.memset(C.byref(t),0x5a,C.sizeof(t));saved_t=bytes(t)
    check(other_jobs.poll(other.handle,live[0][0],C.byref(t),C.byref(e))==101 and bytes(t)==saved_t,'foreign-owner ID rejected');other.close(other.handle)
    for id,size,color,left,off,rr in [live[1],live[3],live[0],live[2]]:
        t=wait(id);check((t.width,t.height)==(size,size),'retained dimensions')
        check(resize(window,size,size,C.byref(e))==0 and jobs.copy(window,id,C.byref(e))==0,'GPU-only completed target copy')
        out_pixels=C.create_string_buffer(size*size*4);check(read(window,out_pixels,len(out_pixels),C.byref(e))==0,'diagnostic backbuffer readback')
        pixel=bytes([255,0,0,255]) if off else bytes([(color>>shift)&255 for shift in [16,8,0,24]])
        row=(pixel*(size//2)+bytes([0x12,0x34,0x56,255])*(size//2)) if left else pixel*size
        check(out_pixels.raw==row*size,'independent complete palette/clip/Off pixel oracle');images+=1;pixels+=size*size
        if off:check(rr.texture_uploads==rr.decoded_texels==rr.shader_texture_bindings==rr.shader_sampler_bindings==0,'async Texture Off does not sample')
        check(jobs.present(window,id,n%2,C.byref(e))==0,'completed job real Present');presents+=1
        check(count(window)==presents,'real job Present counter')
    old_count=count(window);check(jobs.present(window,live[0][0],2,C.byref(e))==1 and count(window)==old_count,'invalid interval does not flip')
    check(resize(window,128,96,C.byref(e))==0 and jobs.copy(window,live[0][0],C.byref(e))==5 and count(window)==old_count,'size mismatch explicitly rejected')
    for id,*_ in [live[3],live[0],live[2]]:
        check(jobs.release(gpu.handle,id,C.byref(e))==0,'out-of-order ready target retirement')
        check(jobs.poll(gpu.handle,id,C.byref(t),C.byref(e))==101,'stale job rejected')
    # Window and renderer close while the final owned job is retained.
    gpu.close(gpu.handle);wc(window);last=live[1][0];check(jobs.live(gpu.handle)==1,'job keeps closed renderer device alive');wait(last)
    check(jobs.release(gpu.handle,last,C.byref(e))==0,'final query release finishes deferred device close');gpu.handle=None
    check(timed==7,'seven real GPU query results reliable on this device')
    result=dict(status='bounded_real_GPU_jobs_and_completion_timing_verified',checks=checks,completed_job_observations=timed,distinct_jobs_submitted=6,real_pending_observations=pending,
        independent_backbuffer_images=images,backbuffer_pixels_compared=pixels,visible_job_Presents=presents,real_gpu_timings=timings,
        four_live_targets_and_fifth_rejection_verified=True,out_of_order_retirement_verified=True,CPU_source_owners_retired_before_target_consumption=True,
        foreign_thread_and_owner_rejected=True,exact_budget_verified=True,deferred_GPU_window_close_drained=True,
        physical_scanout_or_Kamui_scheduling_verified=False,disjoint_or_device_removal_not_injected=True,
        verifier_sha256=sha(Path(__file__)),support_sha256=sha(Path(__file__).parent/'native_gpu_jobs_test_support.py'),build_report_sha256=sha(a.build/'build_verification.json'),playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k!='real_gpu_timings'},indent=2))
if __name__=='__main__':main()
