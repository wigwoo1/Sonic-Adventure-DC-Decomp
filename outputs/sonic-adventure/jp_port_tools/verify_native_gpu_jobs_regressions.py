"""Keep frozen pixel/direct suites; additionally consume every image as a job."""
import argparse,ctypes as C,hashlib,importlib,json,sys
from pathlib import Path
from native_packet_test_support import bind
from native_gpu_test_support import GPU,Config,Report,Failure
from native_gpu_jobs_test_support import Jobs
U=C.c_uint32;I=C.c_int;P=C.c_void_p;Q=C.c_uint64;S=C.c_size_t
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','output']:ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--suite',choices=['gpu','gpu_extended','stateful'],required=True);a=ap.parse_args()
    r=a.workspace.resolve();b=json.loads((a.build/'build_verification.json').read_text());d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not (d/(a.suite+'_jobs_verification.json')).exists()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    original=importlib.import_module('verify_native_direct_window');instances=[];checks=images=pixels=pending=valid_timings=0;overhead=None;timings=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    class TimedGPU(GPU):
        def __init__(self,path):
            super().__init__(path);self.jobs=Jobs(self);self.job_window=P();self.job_size=(64,64);gl=self.lib
            self.job_open=bind(gl,'jp_window_open',I,P,U,U,I,P,P);self.job_close=bind(gl,'jp_window_close',None,P);self.job_resize=bind(gl,'jp_window_resize',I,P,U,U,P)
            self.job_read=bind(gl,'jp_window_readback',I,P,P,S,P);e=Failure()
            check(self.job_open(self.handle,64,64,0,C.byref(self.job_window),C.byref(e))==0,'real GPU job comparison target')
            old_draw=self.draw;old_close=self.close
            def draw(handle,fid,config,out,n,rep,err):
                code=old_draw(handle,fid,config,out,n,rep,err)
                if not code:self.compare_job(fid,config,out,rep)
                return code
            def close(handle):
                check(self.jobs.live(self.handle)==0,'comparison jobs all retired');self.job_close(self.job_window);self.job_window=None;old_close(handle)
            if a.suite!='stateful':self.draw=draw
            self.close=close;instances.append(self)
        def compare_job(self,fid,config,out,rep):
            nonlocal checks,images,pixels,pending,overhead,valid_timings
            cfg=C.cast(config,C.POINTER(Config)).contents;expected=C.cast(rep,C.POINTER(Report)).contents
            id=Q();actual=Report();e=Failure();submit=self.jobs.submit if a.suite=='stateful' else self.jobs.configured
            before=self.jobs.now();code=submit(self.handle,fid,config,C.byref(id),C.byref(actual),C.byref(e));after=self.jobs.now()
            check(code==0,('actual measured job',code,e.hresult));t,p=self.jobs.wait(id.value);pending+=p
            check(before<=t.cpu_begin<=t.cpu_prepared<=t.cpu_submitted<=after,'CPU timestamps bracket preparation/submission')
            check((t.width,t.height)==(cfg.width,cfg.height),'measured target dimensions');valid_timings+=t.gpu_timing_valid
            timings.append({n:getattr(t,n) for n,_ in t._fields_})
            n=cfg.width*cfg.height*4;delta=actual.accounted_payload_bytes-expected.accounted_payload_bytes+n
            if overhead is None:overhead=delta
            check(delta==overhead and 0<delta<4096,'bounded job bookkeeping replaces staging payload')
            for name,_ in Report._fields_:
                if name!='accounted_payload_bytes':check(getattr(actual,name)==getattr(expected,name),('job rendering report',name))
            if self.job_size!=(cfg.width,cfg.height):
                check(self.job_resize(self.job_window,cfg.width,cfg.height,C.byref(e))==0,'resize diagnostic job window');self.job_size=(cfg.width,cfg.height)
            check(self.jobs.copy(self.job_window,id,C.byref(e))==0,'GPU-only completed image transfer')
            data=C.create_string_buffer(n);check(self.job_read(self.job_window,data,n,C.byref(e))==0,'diagnostic job readback')
            check(data.raw==C.string_at(out,n),'completed owned target equals frozen image byte for byte')
            check(self.jobs.release(self.handle,id,C.byref(e))==0 and self.jobs.live(self.handle)==0,'job query/target retirement')
            images+=1;pixels+=cfg.width*cfg.height
    original.GPU=TimedGPU
    if a.suite=='stateful':
        state=importlib.import_module('verify_native_video_window');old_bind=state.bind
        def bind_bound(lib,name,*args):
            f=old_bind(lib,name,*args)
            if name!='jp_gpu_draw_bound_frame':return f
            def call(handle,fid,config,out,n,rep,err):
                code=f(handle,fid,config,out,n,rep,err)
                if not code:instances[-1].compare_job(fid,config,out,rep)
                return code
            return call
        state.bind=bind_bound
    sys.argv=[original.__file__,'--workspace',str(r),'--build',str(a.build),'--output',str(d),'--suite',a.suite];original.main()
    check(images>0 and all(v.job_window is None for v in instances),'all job comparison owners cleaned')
    result=dict(status='real_GPU_query_jobs_'+a.suite+'_regression_verified',checks=checks,job_images_compared=images,job_backbuffer_pixels_compared=pixels,
        real_pending_observations=pending,valid_GPU_timestamp_results=valid_timings,job_bookkeeping_bytes=overhead,real_gpu_timings=timings,
        query_completion_before_GPU_copy_verified=True,immutable_frame_input_used=True,
        frozen_direct_wrapper_sha256=sha(Path(original.__file__)),frozen_direct_proof_sha256=sha(d/(a.suite+'_direct_verification.json')),
        verifier_sha256=sha(Path(__file__)),support_sha256=sha(Path(__file__).parent/'native_gpu_jobs_test_support.py'),build_report_sha256=sha(a.build/'build_verification.json'),
        Kamui_or_actual_game_frame_loop_implemented=False,physical_scanout_timing_verified=False,playable=False,overall_completion_percent=10)
    (d/(a.suite+'_jobs_verification.json')).write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k!='real_gpu_timings'},indent=2))
if __name__=='__main__':main()
