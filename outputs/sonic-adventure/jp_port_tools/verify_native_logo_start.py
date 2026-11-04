"""Execute actual logo_start, task/heap startup, cover and rendered logo.

Does not execute logo_step, boot_init, task stepping/destruction or gameplay.
The frame-wait component still has diagnostic out-of-scope service traces.
"""
import argparse
import ctypes as C
import hashlib
import json
import re
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime, List
from native_task_test_support import Task, Layout
from native_asset_test_support import Assets, Limits
from native_pvm_test_support import PVM, index_archive, image_oracle
from native_video_test_support import Video
from native_gpu_test_support import GPU, Config, Failure
from native_present_test_support import Present
U=C.c_uint32; I=C.c_int; P=C.c_void_p; S=C.c_size_t; Q=C.c_uint64


def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','inputs','asset-root','report']:
        ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve()
    assert not a.report.exists()
    b=json.loads((d/'build_verification.json').read_bytes())
    assert b['exit_code']==b['diagnostic_bytes']==0 and sha(b['DLL'])==b['DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(d/n)==h,n
    fb=json.loads((Path(b['frame_build'])/'build_verification.json').read_bytes())
    assert sha(Path(b['frame_build'])/'build_verification.json')==b['frame_build_sha256']
    for n in ['frames_DLL','GPU_DLL','DLL']:assert sha(fb[n])==fb[n+'_sha256']
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;v=Video(lib);assets=Assets(lib);pvm=PVM(lib)
    gpu=GPU(Path(b['GPU_DLL']));present=Present(Path(b['present_DLL']));startlib=C.CDLL(b['DLL'])
    checks=0;groups={};images=0;pixels=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None)
    reset=bind(lib,'jp_queue_reset_frame',I);queue_status=bind(lib,'jp_queue_status',I)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I)
    ri=bind(lib,'jp_render_state_initialize',I,U,S);rs=bind(lib,'jp_render_state_shutdown',None)
    contexts=bind(lib,'jp_ninja_contexts_initialize',I)
    fade=bind(lib,'title_fade_set',None,I);fadelevel=bind(lib,'jp_title_fade_level',I)
    drawfade=bind(lib,'title_fade_draw',None)
    start=bind(startlib,'logo_start',None);mode=bind(startlib,'jp_logo_mode',I)
    frames=bind(startlib,'jp_logo_frames',I);logo=bind(startlib,'jp_logo_asset_draw',None)
    listing=bind(startlib,'jp_logo_asset_list',C.POINTER(List));free=bind(startlib,'jp_logo_asset_release',I)
    cover_set=bind(startlib,'fade_set_cover',None,P);cover_get=bind(startlib,'fade_get_cover',None,P)
    pool=bind(startlib,'jp_startup_pool_info',U,P);alloc=bind(startlib,'task_alloc',P,I,P)
    stats=bind(startlib,'jp_heap_statistics',None,P,P)
    halloc=bind(startlib,'jp_heap_allocate',P,I);hfree=bind(startlib,'jp_heap_release',None,P)
    current=bind(startlib,'task_get_current_root_step',P)
    layout=Layout();bind(startlib,'jp_task_runtime_get_layout',None,P)(C.byref(layout))
    check(layout.pointer_bytes==8 and layout.task_bytes==C.sizeof(Task)==96 and layout.heap_header_bytes==32,'real native task and heap ABI')
    check(qi(8<<20)==0 and fi(8,128,32<<20)==0 and t.init(32,1024,32<<20)==0 and v.init(640,480,0xff000000,1)==0 and ri(128,1<<20)==0 and contexts()==0,'real shared native graphics owners')
    config=Config.make(640,480);config.logical_width=640;config.logical_height=480
    gl=gpu.lib;window=P();failure=Failure()
    wo=bind(gl,'jp_window_open',I,P,U,U,I,P,P);wc=bind(gl,'jp_window_close',None,P)
    copy=bind(gl,'jp_window_copy_job',I,P,Q,P);read=bind(gl,'jp_window_readback',I,P,P,S,P)
    device=C.addressof((C.c_byte*1).in_dll(lib,'kmiDeviceExtension'));present.device(device)
    check(wo(gpu.handle,640,480,1,C.byref(window),C.byref(failure))==0,'real startup diagnostic window')
    check(present.clock.init(None,None,0)==0 and present.wait_init(60000,1001,8,C.cast(present.window_idle,P),window)==0 and present.init(gpu.handle,window,device,C.byref(config),5000,0)==0,'real clock/vsync/presenter ready before original startup')
    limits=Limits(4<<20,16<<20,32<<20,64)
    check(assets.init(str(a.asset_root.resolve()),C.byref(limits))==0,'supplied-disc asset root')
    def poolstate():
        out=(U*4)();check(pool(out)==0,'real pool free-chain links valid');return list(out)
    def heapstate():
        total=I();largest=I();stats(C.byref(total),C.byref(largest));return total.value,largest.value
    def startup():
        before=assets.snapshot();fade(128);word=U(0x80402010);cover_set(C.byref(word));present.pacing(7,3)
        start();word.value=0xffffffff;cover_get(C.byref(word))
        check(assets.status()==t.status()==present.wait_status()==0,'all real void-startup dependency statuses checked')
        check(mode()==1 and frames()==0 and fadelevel()==0 and word.value==0,'original logo, fade and real cover reset')
        check([present.counter(i) for i in range(4)]==[1,1,1,0],'original frame_pacing_init reset actual shared pacing state')
        check(poolstate()==[4096,4096,0,0] and not current(),'original task_init initialized all 4096 actual records')
        check(heapstate()==(0x90000-(4096*C.sizeof(Task)+32),)*2,'actual task pool consumes expected real heap allocation')
        after=assets.snapshot()
        check(after.files_read==before.files_read+1 and after.decodes==before.decodes+1 and after.pvm_loads==before.pvm_loads+1 and pvm.loaded(listing())==t.live()==6,'original startup performs real file/PRS/PVM six-texture load')
    startup()
    words=[0,1,0xffffffff,0x80402010,0xff000000,0x00ffffff]
    for value in words:
        word=U(value);cover_set(C.byref(word));word.value=~value&0xffffffff
        cover_get(C.byref(word));check(word.value==value,'original cover copies complete color word')
        cover_get(None);cover_get(C.byref(word));check(word.value==value,'original cover null getter preserves owned state')
    zero=U();cover_set(C.byref(zero));groups['original_startup_and_real_cover']=checks
    first=None
    for i in range(4096):
        addr=alloc(7,None);check(bool(addr),'real task pool slot available')
        if first is None:first=addr
        record=Task.from_address(addr)
        # TASK_UNLINKED preserves next; child creation subsequently replaces it.
        expected_next=addr+C.sizeof(Task) if i<4095 else None
        check(addr==first+i*C.sizeof(Task) and record.next==expected_next and bool(record.step) and all(getattr(record,n) is None for n in ['prev','parent','child','draw','destructor','placement','work','motion','force','extra']),'original allocation returns distinct initialized task with exact unlinked next semantics')
    check(not alloc(7,None) and poolstate()==[4096,0,0,0],'real pool exhaustion rejects 4097th task')
    before=heapstate();request=before[1]-48;block=halloc(request)
    check(bool(block) and heapstate()==(0,0),'native allocator consumes unsafe short split remainder')
    C.memset(block,0x5a,request);hfree(block)
    check(heapstate()==before,'real heap coalesces after near-exhaustion and full payload writes')
    check(free()==1 and t.live()==0,'first real logo texture list released')
    startup()
    for level in list(range(7))+[8]:check(bool(alloc(level,None)),'actual per-level task allocation')
    check(poolstate()==[4096,4088,8,0],'all eight original task-level heads occupied')
    groups['task_pool_exhaustion_native_heap_split_and_restart']=checks-sum(groups.values())
    manifest=json.loads((a.inputs/'input_manifest.json').read_bytes())
    row=next(x for x in manifest['assets'] if x['disc_path']=='SONICADV/SEGALOGO.PRS')
    check(sha(row['PVM'])==row['PVM_sha256'] and sha(a.asset_root/'SEGALOGO.PRS')==row['compressed_sha256'],'original supplied asset hashes preserved')
    _,entries,rawtiles=index_archive(Path(row['PVM']).read_bytes());tiles=[image_oracle(x,256,256,True) for x in rawtiles]
    metadata=(d/'inputs/progress/demo/logo_texanims.inc').read_text()
    base=bytearray(640*480*4);coverage=bytearray(640*480)
    for line in metadata.splitlines():
        if '{' not in line:continue
        sx,sy,cx,cy,u1,v1,u2,v2,index,attr=map(int,re.findall(r'-?\d+',line))
        assert (sx,sy,u1,v1,u2,v2,attr)==(256,256,0,0,256,256,0)
        left=320-cx;top=-cy;tile=tiles[entries[index]['number']]
        for y in range(max(0,top),min(480,top+sy)):
            for x in range(max(0,left),min(640,left+sx)):
                at=y*640+x;offset=((y-top)*256+x-left)*4
                assert not coverage[at];coverage[at]=1;base[at*4:at*4+4]=tile[offset:offset+4]
    check(all(coverage),'independent original six-piece logo oracle covers image')
    observed=[]
    @C.CFUNCTYPE(None,P)
    def done(argument):
        try:
            state=present.snapshot();rgba=C.create_string_buffer(640*480*4);error=Failure()
            observed.append((copy(window,state.job,C.byref(error)),read(window,rgba,len(rgba),C.byref(error)),rgba.raw))
        except Exception as error:observed.append(str(error))
    check(present.callback(C.cast(done,P),None)==0,'EOR image observer registered')
    present.probe(0,0)
    for index,level in enumerate([0,128,255]):
        check(reset()==0,'real queue reset');fade(level);drawfade();logo()
        check(queue_status()==t.status()==0,'actual original fade/logo producers after real startup')
        present.frame();state=present.snapshot()
        check(present.status()==present.wait_status()==0 and state.submissions==state.completions==state.presentations==index+1,'actual original pacing, GPU completion and Present')
        check(len(observed)==index+1 and observed[-1][:2]==(0,0),'EOR reads actual startup-rendered frame')
        if level==0:expected=bytes([0,0,0,255])*(640*480)
        elif level==255:expected=base
        else:
            table=bytes(round(x*level/255) for x in range(256));expected=bytearray(base.translate(table));alpha=255-level
            expected[3::4]=bytes([round((alpha*alpha+255*level)/255)])*(640*480)
        check(len(observed[-1][2])==len(expected) and max(abs(x-y) for x,y in zip(observed[-1][2],expected))<=(1 if level==128 else 0),'independent whole-image startup logo/fade oracle')
        images+=1;pixels+=640*480
    groups['original_logo_draw_after_startup_GPU_images_and_Present']=checks-sum(groups.values())
    check(free()==1 and t.live()==0 and assets.shutdown()==0,'startup textures and supplied file owner drained')
    empty=d/'empty-asset-root';empty.mkdir(exist_ok=True);assert not any(empty.iterdir())
    check(assets.init(str(empty),C.byref(limits))==0,'valid empty root for missing-asset failure')
    empty_initial=assets.snapshot()
    start()
    check(assets.status()!=0 and pvm.loaded(listing())==0 and t.live()==0,'actual void logo_start missing-file failure is observable, no fake textures')
    check(mode()==1 and frames()==0,'original startup does not propagate load failure; host must gate entry on asset status')
    check(poolstate()==[4096,4096,0,0],'original task/heap restart remains real on load failure')
    failed_state=assets.snapshot()
    check(failed_state.owned_bytes==empty_initial.owned_bytes and failed_state.decoded_bytes==failed_state.pending_refs==failed_state.resident_refs==0,'missing file retains only initialization storage, no decoded buffer or texture references')
    assets.clear()
    check(assets.shutdown()==0,'failed asset owner drains without retained references')
    groups['original_missing_asset_failure_observed']=checks-sum(groups.values())
    check(present.shutdown()==0 and present.wait_shutdown()==0,'presentation owners drained')
    present.clock.shutdown();wc(window);gpu.close(gpu.handle);qs();rs();t.shutdown()
    check(fs()==0 and pvm.shutdown()==0,'all native graphics owners drained')
    groups['shutdown']=checks-sum(groups.values())
    result=dict(status='original_logo_start_real_heap_task_pool_cover_pacing_assets_GPU_and_Present_verified',
        checks=checks,groups=groups,actual_GPU_images=images,pixels_compared=pixels,actual_Presents=3,
        actual_task_slots_exhausted=4096,task_levels_initialized_and_allocated=8,
        native_short_split_fix_verified=True,original_logo_start_executed=True,
        original_cover_set_and_get_executed=True,missing_asset_failure_observed=True,
        host_must_check_void_startup_dependency_status=True,task_step_destroy_replay_verified=False,
        original_logo_step_or_boot_init_executed=False,full_native_gameplay_verified=False,
        build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),
        retail_images_published=False,playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))


if __name__=='__main__':main()
