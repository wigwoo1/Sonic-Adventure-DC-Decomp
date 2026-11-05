"""Real task hooks/recording, shared clock and native GPU lifetime integration.

Controls presentation phases through private probe accessors. Does not execute
the complete original split-frame controller, logo_step or full game loop.
"""
import argparse,ctypes as C,hashlib,json,re
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
from native_packet_test_support import bind
from native_texture_test_support import Runtime,List
from native_task_test_support_v2 import Task,Work
from native_asset_test_support import Assets,Limits
from native_pvm_test_support import PVM,index_archive,image_oracle
from native_video_test_support import Video
from native_gpu_test_support import GPU,Config,Failure
from native_present_test_support import Present
from native_clock_test_support import Clock,Counter
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
Callback=C.CFUNCTYPE(None,P);RecordObserver=C.CFUNCTYPE(None,I,P,I,I,C.c_float,U);UntimedObserver=C.CFUNCTYPE(None,I)
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','startup','inputs','asset-root','report']:
        ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();assert not a.report.exists()
    b=json.loads((d/'build_verification.json').read_bytes());sb=json.loads((a.startup/'build_verification.json').read_bytes())
    assert b['exit_code']==b['diagnostic_bytes']==0 and sb['frame_build_sha256']==sha(d/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(d/n)==h,n
    for n in ['frames_DLL','DLL','GPU_DLL']:assert sha(b[n])==b[n+'_sha256']
    assert sha(sb['DLL'])==sb['DLL_sha256']
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;gpu=GPU(Path(b['GPU_DLL']));present=Present(Path(b['DLL']));logo_lib=C.CDLL(sb['DLL'])
    checks=0;groups={};images=0;pixels=0;callback_errors=[];callbacks=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    # Audit actual linked definitions, rather than trusting inherited metadata.
    names=['frame_interval','frame_countdown','frame_step_count','frame_step','frame_split','frame_redrawing','frame_later_draws','camera_work','player_tasks','player_records','heap_arena','task_init','task_step_all','task_draw_all','task_destroy','present_record','present_untimed','present_run','present_draw','present_forget','_nj_calc_vertex_','_nj_calc_polygon_','_nj_draw_polygon_','_nj_vsync_count_','_nj_wait_vsync_in_','_nj_wait_vsync_out_','jp_clock_initialize','jp_clock_read_tstr','jp_clock_write_tstr']
    providers={n:[] for n in names};by_dll={}
    for dllkey in ['frames_DLL','DLL']:
        command=next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b[dllkey])
        by_dll[dllkey]=[x for x in command if x.endswith('.obj')]
        for obj in by_dll[dllkey]:
            for s in coff_symbols(Path(obj)):
                if s['name'] in providers and s['section']>0:providers[s['name']].append(obj)
    for n,objects in providers.items():check(len(objects)==1 and objects[0] in by_dll['frames_DLL'],('single real shared definition',n,objects))
    check(Path(providers['present_record'][0]).name==Path(providers['present_untimed'][0]).name=='presentation.obj','genuine selected functions provide recording and timing, not fixture observers')
    check(Path(providers['camera_work'][0]).name==Path(providers['player_tasks'][0]).name=='game_state.obj','original camera/player declarations own state')
    for n in b['clock_export_forwarders']:
        check(C.cast(getattr(lib,n),P).value==C.cast(getattr(present.lib,n),P).value,('presenter clock export forwards to exact same real function',n))
    for n in sb['task_diagnostic_forwarders']:
        check(C.cast(getattr(lib,n),P).value==C.cast(getattr(logo_lib,n),P).value,('startup forwards to same full task/heap implementation',n))
    for index,n in enumerate(['_nj_calc_vertex_','_nj_calc_polygon_','_nj_draw_polygon_','_nj_vsync_count_']):
        value=I.in_dll(lib,n);value.value=100+index
        check(present.counter(4+index)==100+index,'frame wait observes actual shared Ninja accounting state')
    clocks=Clock(Path(b['frames_DLL']));counter=Counter();check(counter.start(clocks,1000000)==0,'real clock core initialized through shared owner')
    counter.tick=250;check(present.clock.wall()==clocks.wall()==195,'clock observations from both DLL paths use one domain')
    set_observers=bind(lib,'jp_present_set_observers',None,P,P);untimed=bind(lib,'present_untimed',None,I)
    untimed_events=[]
    @UntimedObserver
    def timing(begin):untimed_events.append(begin)
    set_observers(None,C.cast(timing,P));I.in_dll(lib,'frame_split').value=1;I.in_dll(lib,'frame_step').value=1
    clocks.write(0xa2);untimed(1);check(clocks.tstr()==present.clock.tstr()==0xa0,'actual untimed pauses the same real timer bit')
    untimed(1);untimed(0);check(clocks.tstr()==0xa0,'nested actual untimed keeps outer pause')
    untimed(0);check(clocks.tstr()==0xa2 and untimed_events==[1,1,0,0],'actual untimed restores original timer bits and optional observer is passive')
    set_observers(None,None);I.in_dll(lib,'frame_split').value=0;I.in_dll(lib,'frame_step').value=0;clocks.shutdown()
    groups['single_task_presentation_game_state_and_clock_ownership']=checks
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);qstatus=bind(lib,'jp_queue_status',I)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);orphaned=bind(lib,'jp_frames_orphaned_bytes',S)
    ri=bind(lib,'jp_render_state_initialize',I,U,S);rs=bind(lib,'jp_render_state_shutdown',None)
    v=Video(lib);assets=Assets(lib);pvm=PVM(lib)
    check(qi(8<<20)==0 and fi(8,128,32<<20)==0 and t.init(32,1024,32<<20)==0 and v.init(640,480,0xff000000,1)==0 and ri(128,1<<20)==0 and bind(lib,'jp_ninja_contexts_initialize',I)()==0,'actual shared graphics owners')
    config=Config.make(640,480);config.logical_width=640;config.logical_height=480
    gl=gpu.lib;window=P();failure=Failure();device=C.addressof((C.c_byte*1).in_dll(lib,'kmiDeviceExtension'));present.device(device)
    wo=bind(gl,'jp_window_open',I,P,U,U,I,P,P);wc=bind(gl,'jp_window_close',None,P)
    copy=bind(gl,'jp_window_copy_job',I,P,Q,P);read=bind(gl,'jp_window_readback',I,P,P,S,P)
    check(wo(gpu.handle,640,480,1,C.byref(window),C.byref(failure))==0 and present.clock.init(None,None,0)==0 and present.wait_init(60000,1001,8,C.cast(present.window_idle,P),window)==0 and present.init(gpu.handle,window,device,C.byref(config),5000,0)==0,'real QPC/vsync/GPU presenter for actual task images')
    limits=Limits(4<<20,16<<20,32<<20,64);check(assets.init(str(a.asset_root.resolve()),C.byref(limits))==0,'supplied-disc asset owner')
    start=bind(logo_lib,'logo_start',None);logo=bind(logo_lib,'jp_logo_asset_draw',None);listing=bind(logo_lib,'jp_logo_asset_list',C.POINTER(List));free=bind(logo_lib,'jp_logo_asset_release',I)
    start();check(assets.status()==t.status()==present.wait_status()==0 and pvm.loaded(listing())==t.live()==6,'unchanged original logo_start uses full real shared task/heap and asset owner')
    pool=bind(lib,'jp_startup_pool_info',U,P);pool_out=(U*4)();check(pool(pool_out)==0 and list(pool_out)==[4096,4096,0,0],'actual original startup task pool')
    manifest=json.loads((a.inputs/'input_manifest.json').read_bytes());row=next(x for x in manifest['assets'] if x['disc_path']=='SONICADV/SEGALOGO.PRS')
    check(sha(row['PVM'])==row['PVM_sha256'] and sha(a.asset_root/'SEGALOGO.PRS')==row['compressed_sha256'],'original supplied logo asset hashes')
    _,entries,rawtiles=index_archive(Path(row['PVM']).read_bytes());tiles=[image_oracle(x,256,256,True) for x in rawtiles]
    base=bytearray(640*480*4);coverage=bytearray(640*480)
    for line in (a.startup/'inputs/progress/demo/logo_texanims.inc').read_text().splitlines():
        if '{' not in line:continue
        sx,sy,cx,cy,u1,v1,u2,v2,index,attr=map(int,re.findall(r'-?\d+',line));assert (sx,sy,u1,v1,u2,v2,attr)==(256,256,0,0,256,256,0)
        left=320-cx;top=-cy;tile=tiles[entries[index]['number']]
        for y in range(max(0,top),min(480,top+sy)):
            for x in range(max(0,left),min(640,left+sx)):
                at=y*640+x;offset=((y-top)*256+x-left)*4;assert not coverage[at];coverage[at]=1;base[at*4:at*4+4]=tile[offset:offset+4]
    check(all(coverage),'independent six-piece logo oracle')
    levels=bind(lib,'title_fade_set',None,I);drawfade=bind(lib,'title_fade_draw',None)
    phase=bind(lib,'jp_lifecycle_reset',None,I,I);current=bind(lib,'jp_lifecycle_current',P);anchor=bind(lib,'jp_lifecycle_anchor',C.c_float);wide=bind(lib,'jp_lifecycle_wide',None,C.c_float,I)
    flats=bind(lib,'jp_present_flat_count',U,I);flags=bind(lib,'jp_lifecycle_flags',U,P)
    create=bind(lib,'task_create',P,C.c_uint16,I,P);child_create=bind(lib,'task_create_child',P,C.c_uint16,P,P)
    step_children=bind(lib,'task_step_children',None,P);step_all=bind(lib,'task_step_all',None);draw_all=bind(lib,'task_draw_all',None);destroy=bind(lib,'task_destroy',None,P)
    root_step=bind(lib,'task_get_current_root_step',P);install=bind(lib,'jp_present_install_sprite',None)
    callback_events=[];draw_calls=0;destructor_events=[]
    def guarded(fn):
        @Callback
        def invoke(address):
            try:fn(address)
            except BaseException as error:callback_errors.append(type(error).__name__+': '+str(error))
        callbacks.append(invoke);return invoke
    def work(address):return Work.from_address(Task.from_address(address).work)
    def child_body(address):
        check(current()==address and anchor()==123.,'real child presentation context inherits outer anchor');work(address).timer+=1;callback_events.append(('child',address))
    child_callback=guarded(child_body)
    def root_body(address):
        check(current()==address and root_step()==C.cast(root_callback,P).value,'real root scheduling/presentation ownership')
        work(address).timer+=1;wide(123.,1);step_children(address)
        check(current()==address and anchor()==123.,'real nested hooks restore root context')
        callback_events.append(('root',address));drawfade();logo()
    root_callback=guarded(root_body)
    root=create(2,0,C.cast(root_callback,P));check(bool(root),'full original task_create with actual work allocation')
    child=child_create(2,C.cast(child_callback,P),root);check(bool(child),'full original child creation and linkage')
    root_record=Task.from_address(root)
    def draw_body(address):
        nonlocal draw_calls
        check(current()==address,'genuine replay callback has actual task owner');work(address).timer=777;draw_calls+=1;logo()
    draw_callback=guarded(draw_body);root_record.draw=C.cast(draw_callback,P).value
    def destruct(address):
        destructor_events.append(('root',address));check(free()==1,'real task destructor releases actual logo texture ownership')
    destructor=guarded(destruct);root_record.destructor=C.cast(destructor,P).value
    install();present.probe(0,0);observed=[]
    @Callback
    def done(argument):
        try:
            state=present.snapshot();rgba=C.create_string_buffer(640*480*4);error=Failure()
            observed.append((copy(window,state.job,C.byref(error)),read(window,rgba,len(rgba),C.byref(error)),rgba.raw))
        except Exception as error:callback_errors.append(str(error))
    callbacks.append(done);check(present.callback(C.cast(done,P),None)==0,'real EOR observer')
    def expected(level):
        if level<=0:return bytes([0,0,0,255])*(640*480)
        if level>=255:return base
        table=bytes(round(x*level/255) for x in range(256));out=bytearray(base.translate(table));alpha=255-level;out[3::4]=bytes([round((alpha*alpha+255*level)/255)])*(640*480);return out
    def present_image(level):
        nonlocal images,pixels
        present.frame();state=present.snapshot()
        check(not callback_errors and present.status()==present.wait_status()==0 and state.submissions==state.completions==state.presentations==images+1,('actual task frame completes GPU job and Present',callback_errors))
        check(len(observed)==images+1 and observed[-1][:2]==(0,0),'EOR reads actual task-produced frame')
        want=expected(level);check(max(abs(x-y) for x,y in zip(observed[-1][2],want))<=(1 if level==128 else 0),'independent whole-image task/fade/logo oracle')
        images+=1;pixels+=640*480
    observer_records=[]
    @RecordObserver
    def observe(kind,what,count,entry,priority,attributes):observer_records.append((kind,current()))
    callbacks.append(observe)
    for index,level in enumerate([0,128,255]):
        phase(1,0);install();check(reset()==0,'real task frame begins with clean queue');levels(level)
        set_observers(C.cast(observe,P) if index==1 else None,None);before=len(observer_records);step_all()
        check(not callback_errors and work(root).timer==work(child).timer==index+1,('actual root/child simulation runs once',callback_errors))
        check(current() is None and anchor()==0. and not root_step(),'actual root/child context and current-root state restored to WIDE_MIDDLE offset zero')
        check(flats(0)==(6 if level==255 else 7),'genuine present_record keeps actual task-produced logo/fade records')
        check(len(observer_records)-before==(7 if index==1 else 0),'optional observer can be disabled without disabling real recording')
        present_image(level)
    groups['actual_task_steps_original_recording_and_GPU_logo_images']=checks-sum(groups.values())
    # The genuine redraw path restores the actual work record after drawing.
    set_observers(None,None);phase(0,1);install();check(reset()==0,'actual redraw frame queue');timer=work(root).timer;draw_all()
    check(not callback_errors and draw_calls==1 and work(root).timer==timer and current() is None,'real task_draw_all/present_draw restores work after actual GPU-producing callback')
    check(flags(root)&8,'genuine record hook marks task callback 2D draws')
    bind(lib,'jp_lifecycle_redraw',None,I)(0);present_image(255)
    # Delete before submission; native queued textures must outlive task/list.
    phase(1,0);install();check(reset()==0,'delete-before-submit frame queue');levels(255);step_all()
    check(not callback_errors and flats(0)==6 and orphaned()==0,'real task queued six logo draws and original retention records')
    destroy(root)
    check(not callback_errors and len(destructor_events)==1 and t.live()==0 and orphaned()==6*256*256*2,'real destructor releases owners while native queue retains original texture payloads')
    owner_kind=bind(lib,'jp_lifecycle_owner_kind',I,I,I)
    check(flats(0)==6 and all(owner_kind(2,i)==3 for i in range(6)),'real present_forget retags all six records as resource-released, preserving original list semantics')
    check(pool(pool_out)==0 and list(pool_out)==[4096,4096,0,0],'full original destruction returns root and child to actual shared pool')
    present_image(255);check(orphaned()==0,'GPU completion retires all payloads retained past task destruction')
    groups['actual_redraw_record_restore_and_destroy_before_GPU_submission']=checks-sum(groups.values())
    check(present.shutdown()==0 and present.wait_shutdown()==0 and assets.shutdown()==0,'actual presentation and asset owners drained');present.clock.shutdown();wc(window);gpu.close(gpu.handle)
    qs();rs();t.shutdown();check(fs()==0 and pvm.shutdown()==0,'all shared native frame/texture owners drained')
    groups['shutdown']=checks-sum(groups.values())
    result=dict(status='complete_original_task_hooks_genuine_recording_shared_clock_GPU_redraw_and_task_texture_lifetime_verified',checks=checks,groups=groups,actual_GPU_images=images,pixels_compared=pixels,actual_Presents=images,actual_original_root_steps=4,actual_original_child_steps=4,actual_original_redraw_callbacks=draw_calls,actual_destructors=len(destructor_events),single_real_symbol_providers=providers,clock_export_addresses_identical=True,startup_task_heap_forwarder_addresses_identical=True,original_presentation_recording_executed=True,optional_observers_do_not_replace_real_recording=True,genuine_task_step_draw_destroy_integrated=True,original_logo_start_executed=True,original_logo_step_or_boot_init_executed=False,presentation_phases_controlled_by_private_probe=True,complete_original_split_frame_controller_executed=False,out_of_scope_audio_input_loader_traces_remain=True,build_report_sha256=sha(d/'build_verification.json'),startup_build_sha256=sha(a.startup/'build_verification.json'),verifier_sha256=sha(Path(__file__)),playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k!='single_real_symbol_providers'},indent=2))


if __name__=='__main__':main()
