"""Full original logo loops and native lifecycle/termination in child processes.
Retail inputs/images stay private. Every rendered frame has an independent
whole-image oracle. Only the input SAMPLE source is controlled; game, SDK,
renderer, platform cleanup and process-exit functions remain actual providers.
"""
import argparse,ctypes as C,hashlib,json,re,subprocess,sys,threading
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,List
from native_gpu_test_support import GPU,Config,Failure
from native_video_test_support import Video
from native_present_test_support import Present
from native_asset_test_support import Assets,Limits
from native_pvm_test_support import PVM,index_archive,image_oracle
from native_task_test_support_v2 import Task
from verify_native_input_backend import Sample,Source
P=C.c_void_p;I=C.c_int;U=C.c_uint32;S=C.c_size_t;Q=C.c_uint64;Callback=C.CFUNCTYPE(None,P)
class ExitConfig(C.Structure):_fields_=[('bytes',U),('resources',U),('gpu',P),('window',P)]
class ExitInfo(C.Structure):_fields_=[('cleanup_calls',Q)]+[(n,U) for n in ['remaining','completed','status','failed_resource','native_error']]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','inputs','asset-root','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    ap.add_argument('--worker',choices=['movie0','movie6','advertise1','cleanup','quit','pending_quit','partial','unbound'])
    ap.add_argument('--pixel-dll',type=Path);a=ap.parse_args()
    for n in ['workspace','build','inputs','asset_root','output']:setattr(a,n,getattr(a,n).resolve())
    r,d,out=a.workspace,a.build,a.output;b=read(d/'build_verification.json')
    assert sha(b['DLL'])==b['DLL_sha256'] and sha(b['authored_exit_DLL'])==b['authored_exit_DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(d/n)==h,n
    if not a.worker:
        assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();out.mkdir()
        helper=out/'pixel_difference.c';helper.write_text('#include <stddef.h>\n__declspec(dllexport) unsigned pixel_difference(const unsigned char *a,const unsigned char *b,size_t n){unsigned maximum=0;for(size_t i=0;i<n;i++){unsigned d=a[i]>b[i]?a[i]-b[i]:b[i]-a[i];if(d>maximum)maximum=d;}return maximum;}\n')
        compiler=b['commands'][1]['command'][0];pixel=out/'pixel_difference.dll'
        cmd=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-shared','-fuse-ld=lld',str(helper),'-o',str(pixel)]
        run=subprocess.run(cmd,capture_output=True);assert run.returncode==0 and not run.stdout and not run.stderr,run.stderr.decode()
        cases={}
        for case in ['movie0','movie6','advertise1','cleanup','quit','pending_quit','partial','unbound']:
            child=out/case;child.mkdir();args=[sys.executable,'-B',str(Path(__file__).resolve()),'--workspace',str(r),'--build',str(d),'--inputs',str(a.inputs),'--asset-root',str(a.asset_root),'--output',str(child),'--worker',case,'--pixel-dll',str(pixel)]
            print('Running actual native exit/logo case:',case,flush=True)
            run=subprocess.run(args,capture_output=True,creationflags=subprocess.CREATE_NO_WINDOW)
            (child/'stdout.txt').write_bytes(run.stdout);(child/'stderr.txt').write_bytes(run.stderr)
            expected=66 if case=='unbound' else 0
            assert run.returncode==expected and not run.stderr,(case,run.returncode,run.stderr.decode()[-5000:])
            result=read(child/'result.json');result['process_exit_code']=run.returncode
            if case in ['quit','pending_quit']:
                destructor=read(child/'destructor.json');assert destructor['reentrant_cleanup']==4 and destructor['input_status']==4 and destructor['wait_status']==2;result['actual_original_task_destructor_during_exit']=True
            cases[case]=result;print(case,'verified; checks',result['checks'],flush=True)
        report=dict(status='complete_original_logo_loops_real_native_exit_cleanup_and_process_termination_verified',checks=sum(q['checks'] for q in cases.values()),cases=cases,complete_original_logo_step_calls=573,actual_GPU_images=570,pixels_compared=570*640*480,actual_Presents=570,original_logo_step_body_unchanged=True,original_game_or_SDK_functions_replaced_by_test=False,controlled_sample_fixture=True,actual_Windows_process_exits_verified=True,full_native_boot_init_or_split_frame_controller_executed=False,physical_input_events_verified=False,remaining_audio_loader_overlay_traces=True,original_binary_or_assets_published=False,pixel_helper_source_sha256=sha(helper),pixel_helper_DLL_sha256=sha(pixel),pixel_helper_compile_command=cmd,build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),playable=False,overall_completion_percent=10)
        (out/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='cases'}),flush=True);return
    checks=images=0;max_error=0;callback_errors=[];steps=destructions=0;buttons=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(a.worker,checks,label)
    runtime=Runtime(Path(b['frames_DLL']));lib=runtime.dll
    C.CDLL(b['GPU_DLL']);present=Present(Path(b['present_DLL']));ex=C.CDLL(b['authored_exit_DLL']);logo=C.CDLL(b['DLL'])
    bind_exit=bind(ex,'jp_exit_bind',I,P);clean=bind(ex,'jp_exit_cleanup',I);status=bind(ex,'jp_exit_status',I);snapshot=bind(ex,'jp_exit_snapshot',I,P)
    system_exit=bind(ex,'sbExitSystem',None);boot_exit=bind(ex,'syBtExit',None)
    def state():
        s=ExitInfo();check(snapshot(C.byref(s))==0,'exit snapshot');return s
    def save_result(**kw):(out/'result.json').write_text(json.dumps(dict(checks=checks,**kw),indent=2)+'\n')
    check(status()==2 and clean()==2,'unbound exit fails explicitly');check(bind_exit(None)==1,'null exit config')
    if a.worker=='unbound':save_result(unbound_cleanup_rejected=True);boot_exit();raise AssertionError('syBtExit returned')
    if a.worker=='partial':
        source=C.create_string_buffer(b'owned by caller');check(bind(lib,'jp_memory_bind_source',I,P,S)(source,len(source))==0,'actual outstanding borrowed loader view')
        cfg=ExitConfig(C.sizeof(ExitConfig),256,None,None);check(bind_exit(C.byref(cfg))==0,'partial-bootstrap shutdown authority')
        check(bind_exit(C.byref(cfg))==5,'no rebinding initialized lifecycle');foreign=[]
        thread=threading.Thread(target=lambda:foreign.append(clean()));thread.start();thread.join();check(foreign==[3],'foreign cleanup rejected')
        check(clean()==6,'actual native shutdown failure retained');s=state();check(s.remaining==256 and s.completed==0 and s.failed_resource==256 and s.native_error==2,'failed borrowed view remains owned')
        check(bind(lib,'jp_memory_unbind_source',I)()==0 and clean()==0,'explicit owner recovery and genuine retry');s=state();check(s.remaining==0 and s.completed==256,'only selected owner completed')
        check(clean()==0 and state().completed==256,'idempotent cleanup does not close again');save_result(actual_service_failure_recovery=True,completed=256);boot_exit();raise AssertionError('syBtExit returned')
    gpu=GPU(Path(b['GPU_DLL']));gl=gpu.lib;assets=Assets(lib);pvm=PVM(lib)
    qi=bind(lib,'jp_queue_initialize',I,S);fi=bind(lib,'jp_frames_initialize',I,U,U,S);ri=bind(lib,'jp_render_state_initialize',I,U,S)
    check(qi(8<<20)==0 and fi(8,128,32<<20)==0 and runtime.init(32,1024,32<<20)==0 and Video(lib).init(640,480,0xff000000,1)==0 and ri(128,1<<20)==0 and bind(lib,'jp_ninja_contexts_initialize',I)()==0,'actual native render owners')
    window=P();failure=Failure();check(bind(gl,'jp_window_open',I,P,U,U,I,P,P)(gpu.handle,640,480,0,C.byref(window),C.byref(failure))==0,'owned hidden DXGI window')
    hwnd=bind(gl,'jp_window_native_handle',P,P)(window);check(bool(hwnd),'actual renderer-owned HWND')
    @Source
    def source(context,samples):
        C.memset(samples,0,4*C.sizeof(Sample));samples[0]=Sample(1,buttons,0,0,0,0,0,0);return 1
    check(bind(lib,'jp_input_initialize',I,P,U,P,P)(hwnd,0,C.cast(source,P),None)==0 and bind(lib,'jp_input_poll',I)()==0,'actual native sample provider primed')
    check(bind(lib,'jp_recording_initialize',I,C.c_wchar_p)(str(out))==0,'actual recording owner')
    check(present.clock.init(None,None,0)==0 and present.wait_init(60000,1001,8,C.cast(present.window_idle,P),window)==0,'real QPC and logical-vsync/window wait')
    cfg=Config.make(640,480);cfg.logical_width=640;cfg.logical_height=480
    device=C.addressof((C.c_byte*1).in_dll(lib,'kmiDeviceExtension'));present.device(device)
    check(present.init(gpu.handle,window,device,C.byref(cfg),5000,0)==0,'real presenter')
    limits=Limits(4<<20,16<<20,32<<20,64);check(assets.init(str(a.asset_root),C.byref(limits))==0,'real retail-input asset root')
    start=bind(logo,'logo_start',None);step=bind(logo,'logo_step',I);mode=bind(logo,'jp_logo_mode',I);frames=bind(logo,'jp_logo_frames',I)
    listing=bind(logo,'jp_logo_asset_list',C.POINTER(List));free_logo=bind(logo,'jp_logo_asset_release',I);level=bind(lib,'jp_title_fade_level',I)
    start();check(assets.status()==runtime.status()==0 and pvm.loaded(listing())==6,'unchanged startup loaded actual six-tile logo')
    config=ExitConfig(C.sizeof(ExitConfig),16383,gpu.handle,window);check(bind_exit(C.byref(config))==0,'transfer native shutdown authority after full initialization')
    @Callback
    def task_step(address):
        nonlocal steps
        steps+=1
    @Callback
    def destroy(address):
        nonlocal destructions
        try:
            destructions+=1;reentrant=clean()
            data=dict(reentrant_cleanup=reentrant,input_status=bind(lib,'jp_input_status',I)(),wait_status=present.wait_status(),destructions=destructions)
            (out/'destructor.json').write_text(json.dumps(data)+'\n')
            check(reentrant==4,'genuine task destructor cannot reenter cleanup')
        except BaseException as error:callback_errors.append(str(error))
    task=bind(lib,'task_create',P,U,I,P)(2,0,C.cast(task_step,P));check(bool(task),'genuine task in original logo loop');Task.from_address(task).destructor=C.cast(destroy,P).value
    present.probe(0,0)
    if a.worker in ['quit','pending_quit']:
        if a.worker=='pending_quit':
            check(step()==0 and present.render()==0,'original logo submits actual GPU work before quit');s=present.snapshot();check(bool(s.job) and bool(s.frame),'actual presenter retains frame/job shutdown ownership')
        buttons=4|2|1024|512|8;check(bind(lib,'jp_input_poll',I)()==0,'prime actual quit combination')
        save_result(original_quit_path_entered=True,pending_gpu_ownership=a.worker=='pending_quit')
        step();raise AssertionError('original logo quit returned instead of native process exit')
    if a.worker=='cleanup':
        check(step()==0 and present.render()==0,'real submitted logo frame before native shutdown');check(bool(present.snapshot().job),'real outstanding owned GPU job')
        data=P();n=S();check(assets.read(b'SEGALOGO.PRS',C.byref(data),C.byref(n))==0 and n.value>0,'actual retained decoded asset view')
        check(bind(lib,'jp_memory_bind_source',I,P,S)(data,n)==0,'borrow decoded asset data into genuine loader')
    else:
        manifest=read(a.inputs/'input_manifest.json');row=next(x for x in manifest['assets'] if x['disc_path']=='SONICADV/SEGALOGO.PRS')
        check(sha(row['PVM'])==row['PVM_sha256'] and sha(a.asset_root/'SEGALOGO.PRS')==row['compressed_sha256'],'retail input/proof hashes preserved')
        _,entries,rawtiles=index_archive(Path(row['PVM']).read_bytes());tiles=[image_oracle(x,256,256,True) for x in rawtiles]
        base=bytearray(640*480*4);coverage=bytearray(640*480)
        for line in (d/'inputs/progress/demo/logo_texanims.inc').read_text().splitlines():
            if '{' not in line:continue
            sx,sy,cx,cy,u1,v1,u2,v2,index,attr=map(int,re.findall(r'-?\d+',line));check((sx,sy,u1,v1,u2,v2,attr)==(256,256,0,0,256,256,0),'actual logo tile metadata')
            left=320-cx;top=-cy;tile=tiles[entries[index]['number']]
            for y in range(max(0,top),min(480,top+sy)):
                for x in range(max(0,left),min(640,left+sx)):
                    at=y*640+x;off=((y-top)*256+x-left)*4;assert not coverage[at];coverage[at]=1;base[at*4:at*4+4]=tile[off:off+4]
        check(all(coverage),'independent six-piece image oracle');base=bytes(base)
        difference=bind(C.CDLL(str(a.pixel_dll.resolve())),'pixel_difference',U,P,P,S)
        output=C.create_string_buffer(640*480*4);copy=bind(gl,'jp_window_copy_job',I,P,Q,P);readback=bind(gl,'jp_window_readback',I,P,P,S,P)
        expected_level=0;expected_mode=1;show=0;seen_image=0
        @Callback
        def done(context):
            nonlocal seen_image,max_error
            try:
                s=present.snapshot();error=Failure();check(copy(window,s.job,C.byref(error))==0 and readback(window,output,len(output),C.byref(error))==0,'actual logo-loop GPU image')
                v=level()
                if v<=0:want=bytes([0,0,0,255])*(640*480)
                elif v>=255:want=base
                else:
                    table=bytes(round(x*v/255) for x in range(256));want=bytearray(base.translate(table));alpha=255-v;want[3::4]=bytes([round((alpha*alpha+255*v)/255)])*(640*480);want=bytes(want)
                error=difference(output,C.c_char_p(want),len(want));max_error=max(max_error,error);check(error<=(0 if v<=0 or v>=255 else 1),'independent whole-image original-logo fade oracle');seen_image+=1
            except BaseException as error:callback_errors.append(str(error))
        check(present.callback(C.cast(done,P),None)==0,'passive completed-frame image observer')
        demo=I.in_dll(lib,'demo_next');demo.value=0 if a.worker=='movie0' else 6 if a.worker=='movie6' else 1
        for call in range(1,192):
            want_return=0
            if expected_mode==1:
                expected_level=255 if expected_level>255 else expected_level+8
                if show>150:expected_mode=2
                show+=1
            elif expected_mode==2:
                if expected_level< -32:expected_level=0;expected_mode=15
                else:expected_level-=8
            elif expected_mode==15:expected_mode=0;want_return=3 if demo.value in [0,6] else 1
            result=step();check(result==want_return and mode()==expected_mode and frames()==show and level()==expected_level,'complete original logo state, fade schedule and transition')
            check(steps==call and not callback_errors,'original task stepping occurs in every logo_step')
            if result:check(call==191 and pvm.loaded(listing())==0 and runtime.live()==0,'original final transition releases logo textures');break
            present.frame();s=present.snapshot();images+=1
            check(not callback_errors and present.status()==present.wait_status()==bind(lib,'jp_input_status',I)()==0,'actual native frame/poll/presentation completed')
            check(s.presentations==images and s.submissions==s.completions==images and seen_image==images,'each original logo draw actually submitted, read and Presented')
        check(images==190 and steps==191 and not callback_errors,'entire unchanged logo loop completed')
    # Check actual cleanup before the final non-returning platform exit.
    system_exit();s=state();check(s.remaining==0 and s.completed==16383 and status()==0,'all selected native owners actually shut down')
    check(destructions==1 and not callback_errors,'genuine task destruction exactly once')
    check(runtime.live()==runtime.allocated()==bind(lib,'jp_frames_live_count',U)()==bind(lib,'jp_frames_accounted_bytes',S)()==bind(lib,'jp_render_state_allocated_bytes',S)()==0,'owned native payloads/frame/render storage retired')
    check(bind(lib,'jp_input_status',I)()==4 and present.wait_status()==2 and present.clock.status()==4 and assets.status()==2 and bind(lib,'jp_recording_status',I)()==2,'actual input/wait/clock/file/recording owners closed')
    user=C.WinDLL('user32');user.IsWindow.argtypes=[P];user.IsWindow.restype=I;check(not user.IsWindow(hwnd),'native HWND actually destroyed')
    total=I();largest=I();bind(lib,'jp_heap_statistics',None,P,P)(C.byref(total),C.byref(largest));check((total.value,largest.value)==(0x90000,0x90000),'genuine task arena reset after destructors')
    before=s.completed;check(clean()==0 and state().completed==before and destructions==1,'repeat cleanup closes no owner twice')
    save_result(complete_original_logo_calls=191 if images else 1,actual_GPU_images=images,actual_Presents=images,max_pixel_error=max_error,all_resources_completed=s.completed,actual_task_destructors=destructions,retained_asset_view_cleanup=a.worker=='cleanup')
    boot_exit();raise AssertionError('native boot-menu process exit returned')
if __name__=='__main__':main()
