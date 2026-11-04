"""Direct target budgets, failure preservation and real retained input lifetimes."""
import argparse,ctypes as C,hashlib,json,struct,threading
from pathlib import Path
from native_packet_test_support import bind
from native_gpu_test_support import GPU,Config,Report,Failure
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
    assert sha(Path(b['DLL']))==b['DLL_sha256']
    runtime=Runtime(Path(b['frames_DLL']));lib=runtime.dll;video=Video(lib);gpu=GPU(Path(b['DLL']));gl=gpu.lib;checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q)
    reserve=bind(lib,'jp_queue_reserve',I,U,U);ctx=bind(lib,'jp_queue_set_context',I,U,P);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None)
    end=bind(lib,'njEndVertex',None,P);pin=bind(lib,'jp_frames_prepare_texture',I,U);start=bind(lib,'njStartVertexTexOpaque',P)
    mode=bind(lib,'kmSetPaletteMode',I,I);palette=bind(lib,'kmSetPaletteData',I,P)
    check(qi(1<<20)==0 and fi(8,8,1<<20)==0 and runtime.init(8,128,1<<20)==0 and video.init(64,64,0xff123456,1)==0,'real owners')
    check(mode(3)==0 and palette((U*1024)(*[0xff00ff00]*1024))==0,'captured palette')
    info=Info();s=info.texsurface;s.Type=0x10000;s.BitDepth=1;s.PixelFormat=6<<27;s.nWidth=s.nHeight=8;s.TextureSize=64;s.fSurfaceFlags=0x24
    check(runtime.copy(C.byref(info),C.create_string_buffer(bytes([7]*64)),64)==0,'actual encoded surface')
    names=(Name*1)();runtime.setname(C.byref(names[0]),None,1000,0);runtime.entry(field(names[0]),C.byref(info),1000,2,0xffffffff);runtime.param(runtime.resolve(names[0].texaddr));tcw=Record.from_address(runtime.resolve(names[0].texaddr)).texparambuffer
    listing=List(names,1);check(runtime.select(C.byref(listing))==1,'actual selected texture')
    check(reset()==0 and reserve(0,4)==0 and ctx(0,(U*4)(0x8000000a,0xe0800000,0x20800000,tcw))==0 and pin(tcw&0x1fffff)==0,'retained input batch')
    get();ptr=start();check(bool(ptr),'actual packet writer');vertices=[]
    for i,(x,y) in enumerate([(-64,-64),(192,-64),(-64,192)]):vertices += [0xf0000000 if i==2 else 0xe0000000,fw(x),fw(y),fw(1),fw(0.25),fw(0.25),0xffff0000,0]
    raw=struct.pack('<24I',*vertices);C.memmove(ptr,raw,len(raw));end(ptr+len(raw));put();fid=Q();check(submit(C.byref(fid))==0,'real retained frame')
    wo=bind(gl,'jp_window_open',I,P,U,U,I,P,P);wc=bind(gl,'jp_window_close',None,P);read=bind(gl,'jp_window_readback',I,P,P,S,P);size=bind(gl,'jp_window_size',I,P,P,P)
    render=bind(gl,'jp_window_render_frame',I,P,Q,P,P,P);draw=bind(gl,'jp_window_draw_direct_frame',I,P,Q,P,U,P,P);count=bind(gl,'jp_window_presented',Q,P)
    window=P();e=Failure();check(wo(gpu.handle,64,48,1,C.byref(window),C.byref(e))==0,'actual visible nonsquare direct target')
    config=Config.make();report=Report();check(render(window,fid,C.byref(config),C.byref(report),C.byref(e))==0,'actual bound direct render')
    check((report.width,report.height)==(64,48) and count(window)==0,'target size override without flip')
    def pixels():
        out=C.create_string_buffer(64*48*4);check(read(window,out,len(out),C.byref(e))==0,'diagnostic exact readback');return out.raw
    green=bytes([0,255,0,255])*(64*48);check(pixels()==green,'independent palette pixel oracle')
    budget=report.accounted_payload_bytes;config.budget=budget;check(render(window,fid,C.byref(config),C.byref(report),C.byref(e))==0,'exact direct budget succeeds')
    previous=bytes(report)
    # Captured-state culling deliberately overrides caller cull_small_area.
    # flat_color_vertex is an actual caller setting, so its invalid value fails.
    cases=[(fid.value,dict(budget=budget-1)),(fid.value,dict(textures=2)),(0,{}),(0xffffffffffffffff,{}),(fid.value,dict(flat_color_vertex=1))]
    for frame,fields in cases:
        c=Config.make()
        for key,value in fields.items():setattr(c,key,value)
        check(render(window,frame,C.byref(c),C.byref(report),C.byref(e))!=0,('invalid request rejected',frame,fields))
        check(bytes(report)==previous and pixels()==green and count(window)==0,'failed preflight preserves backbuffer/report/counter')
    check(draw(window,fid,C.byref(config),2,C.byref(report),C.byref(e))==1 and bytes(report)==previous and pixels()==green,'invalid interval preserves prior result')
    foreign=[]
    def other_thread():
        err=Failure();rep=Report();foreign.extend([render(window,fid,C.byref(config),C.byref(rep),C.byref(err)),draw(window,fid,C.byref(config),0,C.byref(rep),C.byref(err))])
    thread=threading.Thread(target=other_thread);thread.start();thread.join();check(foreign==[1,1] and pixels()==green,'foreign thread cannot submit or flip')
    config.budget=64<<20
    check(draw(window,fid,C.byref(config),1,C.byref(report),C.byref(e))==0 and count(window)==1,'bound direct render plus real Present')
    # Render again, then retire ALL source owners before any explicit GPU wait.
    check(render(window,fid,C.byref(config),C.byref(report),C.byref(e))==0,'submit before source retirement')
    check(release(fid)==0,'immediately release frame before readback');qs();runtime.shutdown();video.shutdown();check(fs()==0,'all CPU input owners gone before GPU wait')
    check(pixels()==green,'GPU inputs remain valid after frame/palette/texture shutdown')
    saved=bytes(report);check(render(window,fid,C.byref(config),C.byref(report),C.byref(e))!=0 and bytes(report)==saved and pixels()==green,'released frame rejected without changing backbuffer')
    gpu.close(gpu.handle);check(render(window,0,C.byref(config),C.byref(report),C.byref(e))==1 and bytes(report)==saved,'renderer deferred close rejects direct rendering');wc(window);gpu.handle=None
    result=dict(status='direct_target_failure_and_retirement_boundaries_verified',checks=checks,visible_direct_Presents=1,exact_budget_and_one_byte_short_verified=True,
        invalid_preflight_preserves_real_backbuffer=True,foreign_thread_submission_rejected=True,frame_palette_texture_shutdown_before_readback_verified=True,
        unsupported_device_loss_mid_submission_not_injected=True,verifier_sha256=sha(Path(__file__)),build_report_sha256=sha(a.build/'build_verification.json'),playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
