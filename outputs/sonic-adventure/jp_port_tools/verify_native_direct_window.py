"""Run frozen pixel oracles, additionally comparing actual direct backbuffers.

The explicit readback in this verifier is diagnostic only. The production direct
render functions have no CPU image transfer. Retain original verifier reports.
"""
import argparse,ctypes as C,hashlib,importlib,json,sys
from pathlib import Path
from native_packet_test_support import bind
from native_gpu_test_support import GPU,Config,Report,Failure
U=C.c_uint32;I=C.c_int;P=C.c_void_p;Q=C.c_uint64;S=C.c_size_t

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','output']:ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--suite',choices=['gpu','gpu_extended','stateful'],required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_text())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert b['exit_code']==b['diagnostic_bytes']==0 and sha(Path(b['DLL']))==b['DLL_sha256']
    d.mkdir(parents=True,exist_ok=True);manifest=d/'build_verification.json'
    if manifest.exists():assert json.loads(manifest.read_text())==b
    else:manifest.write_text(json.dumps(b,indent=2)+'\n')
    modules={'gpu':'verify_native_gpu','gpu_extended':'verify_native_gpu_extended','stateful':'verify_native_video_window'}
    original=importlib.import_module(modules[a.suite]);report=d/({'gpu':'verification.json','gpu_extended':'extended_verification.json','stateful':'stateful_verification.json'}[a.suite]);assert not report.exists()
    checks=images=compared_bytes=direct_presented=0;sizes=set();instances=[];texture_off_images=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    class DirectGPU(GPU):
        def __init__(self,path):
            super().__init__(path);gl=self.lib;self.window=P();self.size=(64,64)
            self.wo=bind(gl,'jp_window_open',I,P,U,U,I,P,P);self.wc=bind(gl,'jp_window_close',None,P)
            self.resize=bind(gl,'jp_window_resize',I,P,U,U,P);self.pump=bind(gl,'jp_window_pump',I,P)
            self.read=bind(gl,'jp_window_readback',I,P,P,S,P);self.present=bind(gl,'jp_window_present',I,P,U,P)
            self.count=bind(gl,'jp_window_presented',Q,P)
            self.direct=bind(gl,'jp_window_render_frame' if a.suite=='stateful' else 'jp_window_render_configured_frame',I,P,Q,P,P,P)
            check(self.wo(self.handle,64,64,int(a.suite=='stateful'),C.byref(self.window),C.byref(self.failure))==0,'real direct-render window')
            check(self.pump(self.window)==1,'direct window message pump');original_draw=self.draw;original_close=self.close
            def draw(handle,fid,config,out,n,rep,err):
                code=original_draw(handle,fid,config,out,n,rep,err)
                if not code:self.compare(fid,config,out,rep)
                return code
            def close(handle):
                self.wc(self.window);self.window=None;original_close(handle)
            if a.suite!='stateful':self.draw=draw
            self.close=close;instances.append(self)
        def compare(self,fid,config,out,rep):
            nonlocal images,compared_bytes,direct_presented,texture_off_images
            cfg=C.cast(config,C.POINTER(Config)).contents;expected=C.cast(rep,C.POINTER(Report)).contents
            width,height=cfg.width,cfg.height;n=width*height*4
            if self.size!=(width,height):
                check(self.resize(self.window,width,height,C.byref(self.failure))==0,'resize direct backbuffer');self.size=(width,height)
            before=self.count(self.window);actual=Report()
            code=self.direct(self.window,fid,config,C.byref(actual),C.byref(self.failure));check(code==0,('direct frame render',code,self.failure.batch,self.failure.hresult))
            check(self.count(self.window)==before,'render does not implicitly Present')
            for name,_ in Report._fields_:
                want=getattr(expected,name)
                if name=='accounted_payload_bytes':want-=n
                check(getattr(actual,name)==want,('same rendering report, one staging image removed',name,getattr(actual,name),want))
            data=C.create_string_buffer(n);check(self.read(self.window,data,n,C.byref(self.failure))==0,'diagnostic backbuffer readback')
            check(data.raw==C.string_at(out,n),'direct target equals legacy image byte for byte')
            if not cfg.textures:
                check(actual.texture_uploads==actual.decoded_texels==actual.shader_texture_bindings==actual.shader_sampler_bindings==0,'direct Texture Off has zero sampling resources');texture_off_images+=1
            if a.suite=='stateful':
                check(self.present(self.window,0,C.byref(self.failure))==0,'direct image real Present');direct_presented+=1
                check(self.count(self.window)==before+1,'direct Present counted')
            images+=1;compared_bytes+=n;sizes.add((width,height))
    original.GPU=DirectGPU
    if a.suite=='stateful':
        original_bind=original.bind
        def bound_bind(lib,name,*args):
            function=original_bind(lib,name,*args)
            if name!='jp_gpu_draw_bound_frame':return function
            def call(handle,fid,config,out,n,rep,err):
                code=function(handle,fid,config,out,n,rep,err)
                if not code:instances[-1].compare(fid,config,out,rep)
                return code
            return call
        original.bind=bound_bind
    lib=C.CDLL(b['frames_DLL']);init=bind(lib,'jp_video_sdk_initialize',I,U,U,U,I);check(init(640,480,0xff000000,1)==0,'initialize actual state provider')
    sys.argv=[original.__file__,'--workspace',str(r),'--build',str(d),'--report',str(report)];original.main()
    q=json.loads(report.read_text());expected={'gpu':491158,'gpu_extended':121451,'stateful':103700}[a.suite];check(q['checks']==expected,'frozen suite count')
    check(images>0 and all(v.window is None for v in instances),'all compared frames and windows cleaned')
    result=dict(status='direct_backbuffer_'+a.suite+'_verified',checks=checks,frozen_suite_checks=expected,direct_images_compared=images,
        backbuffer_bytes_compared=compared_bytes,backbuffer_pixels_compared=compared_bytes//4,backbuffer_sizes=sorted(sizes),direct_visible_Presents=direct_presented,
        texture_Off_images=texture_off_images,direct_report_removes_exact_staging_payload=True,real_backbuffers_compared=True,
        original_verifier_sha256=sha(Path(original.__file__)),original_report_sha256=sha(report),verifier_sha256=sha(Path(__file__)),
        build_report_sha256=sha(a.build/'build_verification.json'),gameplay_verified=False,Kamui_scheduling_implemented=False,playable=False,overall_completion_percent=10)
    target=d/(a.suite+'_direct_verification.json');assert not target.exists();target.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
