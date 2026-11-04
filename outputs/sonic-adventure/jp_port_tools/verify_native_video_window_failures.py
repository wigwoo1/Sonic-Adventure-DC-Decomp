"""Atomic state-capture retry and real Win32 close/thread/buffer boundaries."""
import argparse,ctypes as C,hashlib,json,os,threading
from pathlib import Path
from native_packet_test_support import bind
from native_gpu_test_support import GPU,Failure
from native_video_test_support import Video,Snapshot
U=C.c_uint32;I=C.c_int;S=C.c_size_t;P=C.c_void_p;Q=C.c_uint64
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();assert not a.report.exists();b=json.loads((d/'build_verification.json').read_text())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    lib=C.CDLL(b['frames_DLL']);video=Video(lib);gpu=GPU(Path(b['DLL']));gl=gpu.lib;checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I)
    submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q);clear_frame=bind(lib,'jp_frames_clear_error',None);live=bind(lib,'jp_frames_live_count',U)
    check(qi(1<<20)==0 and fi(8,8,1<<20)==0 and video.init(64,64,0xff123456,1)==0,'failure owners initialize')
    first=Q();check(submit(C.byref(first))==0,'first immutable empty frame');old=video.frame_snapshot(first)
    check(video.write(0xdead,1)==2,'unsupported MMIO explicitly rejected');out=Snapshot();C.memset(C.byref(out),0x5a,C.sizeof(out));saved=bytes(out)
    check(video.capture(C.byref(out))==2 and bytes(out)==saved,'state read failure preserves output')
    retry=Q(0xfeedface);check(submit(C.byref(retry))==2 and retry.value==0xfeedface and live()==1,'failed state capture preserves frame ID/live frames')
    video.clear();clear_frame();check(bytes(video.frame_snapshot(first))==bytes(old),'old frame unaffected by live state error')
    check(submit(C.byref(retry))==0 and live()==2 and bytes(video.frame_snapshot(retry))==bytes(old),'state capture retry succeeds with same version')
    check(release(retry)==0,'retry frame release');clear_frame()
    check(video.frame(retry,C.byref(out))==7 and bytes(out)==saved,'stale state ID preserves output');clear_frame()
    # Failed device callback cannot publish partially filled captured state.
    callback_type=C.CFUNCTYPE(I,P,C.POINTER(Snapshot));callbacks=[]
    @callback_type
    def reject(context,s):s.contents.palette[0]=0xabcdef;callbacks.append(1);return 1
    hook=bind(lib,'jp_video_set_device_source',I,P,P);check(hook(C.cast(reject,P),None)==0,'failing native source attached')
    q=Q(123);check(submit(C.byref(q))==2 and q.value==123 and len(callbacks)==1,'failed device source aborts capture')
    clear_frame();check(video.init(64,64,0xff123456,1)==0,'new source recovers');check(bytes(video.frame_snapshot(first))==bytes(old),'old frame survives live source replacement')
    # Several future state versions coexist with the first retained snapshot.
    pending=[]
    for i in range(6):
        check(video.write(0xb0,i+1)==0 and video.write(0x1000,i+100)==0,'new register state version');q=Q();check(submit(C.byref(q))==0,'distinct state submit');pending.append((q.value,video.frame_snapshot(q)))
    video.shutdown()
    for fid,snap in reversed(pending):check(bytes(video.frame_snapshot(fid))==bytes(snap) and release(fid)==0,'out-of-order independent state retirement')
    check(release(first)==0,'oldest frame retires last');qs();check(fs()==0,'frame owner cleanup')
    wo=bind(gl,'jp_window_open',I,P,U,U,I,P,P);wc=bind(gl,'jp_window_close',None,P);pump=bind(gl,'jp_window_pump',I,P);upload=bind(gl,'jp_window_upload',I,P,P,S,P)
    readback=bind(gl,'jp_window_readback',I,P,P,S,P);resize=bind(gl,'jp_window_resize',I,P,U,U,P);size=bind(gl,'jp_window_size',I,P,P,P)
    window=P();e=Failure();check(wo(gpu.handle,128,96,0,C.byref(window),C.byref(e))==0,'hidden native window')
    pixels=bytes(range(256))*(128*96*4//256);src=C.create_string_buffer(pixels);dst=C.create_string_buffer(len(pixels));check(upload(window,src,len(pixels),C.byref(e))==0,'initial exact backbuffer upload')
    check(upload(window,src,len(pixels)-1,C.byref(e))==1 and readback(window,dst,len(pixels),C.byref(e))==0 and dst.raw==pixels,'short upload preserves real backbuffer')
    C.memset(dst,0x5a,len(dst));check(readback(window,dst,len(pixels)-1,C.byref(e))==1 and dst.raw==b'Z'*len(dst),'short readback preserves CPU output')
    check(resize(window,0,96,C.byref(e))==1,'invalid resize rejected');w=U();h=U();check(size(window,C.byref(w),C.byref(h))==0 and (w.value,h.value)==(128,96),'failed resize preserves dimensions')
    foreign=[]
    def other_thread():
        error=Failure();foreign.extend([pump(window),upload(window,src,len(pixels),C.byref(error))])
    thread=threading.Thread(target=other_thread);thread.start();thread.join();check(foreign==[0,1],'wrong-thread window calls rejected')
    user=C.WinDLL('user32',use_last_error=True);find=bind(user,'FindWindowW',P,C.c_wchar_p,C.c_wchar_p);pid=bind(user,'GetWindowThreadProcessId',U,P,P);send=bind(user,'SendMessageW',C.c_ssize_t,P,U,C.c_size_t,C.c_ssize_t)
    hwnd=find('SA_Native_Renderer_Research_Window',None);owner=U();check(bool(hwnd) and pid(hwnd,C.byref(owner)) and owner.value==os.getpid(),'identified this process native window')
    send(hwnd,0x10,0,0);check(pump(window)==0 and upload(window,src,len(pixels),C.byref(e))==1,'actual WM_CLOSE blocks later uploads')
    gpu.close(gpu.handle);wc(window)
    result=dict(status='state_capture_and_window_failure_boundaries_verified',checks=checks,atomic_failed_state_capture_retry=True,seven_retained_state_generations=True,
        actual_WM_CLOSE_verified=True,foreign_thread_calls_rejected=True,short_buffer_and_resize_failures_preserve_outputs=True,
        build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
