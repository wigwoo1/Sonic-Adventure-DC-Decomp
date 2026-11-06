"""Verify the complete constructor after System adopts real card/font claims.

Controlled input samples are fixtures. All constructors, sound firmware, card
fibers, renderer, waits and native cleanup are genuine existing providers.
"""
import argparse,ctypes as C,faulthandler,hashlib,json,subprocess,sys,threading
from pathlib import Path
from verify_native_audio import bind,Info as AudioInfo
from verify_native_system import Options as SystemOptions,Info as SystemInfo
from verify_native_device_init import Options as DeviceOptions
from verify_native_game_sound import Options as SoundOptions,Info as SoundInfo
from verify_native_aica import Options as AicaOptions
from verify_native_sound_clock import Options as ClockOptions,Info as ClockInfo
from verify_native_input_backend import Source,Sample
from verify_native_card_lifecycle import Card
from verify_native_sdk_heap import Info as HeapInfo
from verify_native_gdfs import Info as GDFSInfo
from verify_native_vmu_lcd import Screen,StateInfo
from native_gpu_test_support import GPU,Config,Report,Failure
from native_texture_test_support import Record
from verify_native_gpu import png
I,U,Q,P,S,F=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p,C.c_size_t,C.c_float
class BootInfo(C.Structure):
    _fields_=[(n,Q) for n in ('starts','completions','shutdowns')]+[(n,U) for n in ('phase','status','native_error','creator_thread','sound_calls','card_owned')]
CASES=('complete','void_complete','restart','missing_driver','failure_retry','clock_failure','late_render_failure','late_texture_failure','no_store','foreign','reentrant','fatal_driver')
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    if not a.case:
        cases={}
        for name in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','build','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--case',name,'--output',str(d/name)];run=subprocess.run(cmd,capture_output=True,timeout=120,creationflags=subprocess.CREATE_NO_WINDOW)
            (d/(name+'.log')).write_bytes(run.stdout+run.stderr);expected=72 if name=='fatal_driver' else 0
            assert run.returncode==expected,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-6000:])
            cases[name]=read(d/name/('progress.json' if expected else 'result.json'));cases[name]['actual_exit_code']=run.returncode
            print(name,cases[name]['checks'],'checks;',run.returncode,'exit',flush=True)
        result=dict(status='complete_original_parent_with_System_card_font_lifetime_delegation_scoped_verification',checks=sum(v['checks'] for v in cases.values()),cases=cases,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),borrowed_host_render_store_and_LCD_owners=True,constructor_delegates_card_and_font_to_System=True,terminal_global_exit_verified_by_this_suite=False,adapted_from_verifier_sha256='0ab3dbccb5d1835612c52679021fb68515b67a609a9c5fde88561dfe9efb5157',gameplay_or_full_game_init_verified=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True);return
    fault=(d/'native_fault.txt').open('w');faulthandler.enable(file=fault,all_threads=True)
    frame=C.CDLL(b['frames_DLL']);system=C.CDLL(b['System_DLL']);sound=C.CDLL(b['game_sound_DLL']);present=C.CDLL(b['present_DLL']);boot=C.CDLL(b['main_boot_DLL']);device=C.CDLL(b['device_DLL'])
    labels=[];observations={};keep=[];reentry=[]
    def check(ok,label):
        assert ok,(a.case,label);labels.append(label)
        (d/'progress.json').write_text(json.dumps(dict(case=a.case,checks=len(labels),assertions=labels),indent=2)+'\n')
    def snap(lib,name,kind):
        out=kind();check(bind(lib,name,I,P)(C.byref(out))==0,'actual '+name);return out
    configure=bind(boot,'jp_main_boot_configure',I,P,P);initialize=bind(boot,'jp_main_boot_initialize',I);shutdown=bind(boot,'jp_main_boot_shutdown',I,U)
    @Source
    def source(ctx,samples):
        if a.case=='reentrant':reentry.append((initialize(),shutdown(5000),configure(None,None)))
        C.memset(samples,0,4*C.sizeof(Sample));samples[0]=Sample(1,0,0,0,0,0,0,0);return 1
    keep.append(source)
    for name,args in [('jp_queue_initialize',(8<<20,)),('jp_frames_initialize',(8,128,32<<20)),('jp_textures_initialize',(32,1024,32<<20)),('jp_render_state_initialize',(128,1<<20))]:
        check(bind(frame,name,I,*([S] if len(args)==1 else [U,U,S] if len(args)==3 else [U,S]))(*args)==0,'borrowed real render prerequisite '+name)
    texture_baseline=(bind(frame,'jp_textures_allocated_bytes',S)(),bind(frame,'jp_textures_live_surfaces',U)())
    if a.case=='late_texture_failure':
        check(128<<10<2048*C.sizeof(Record),'fixture budget is below even the original record array cost')
        check(bind(frame,'jp_texture_sdk_limits',I,U,S)(1024,128<<10)==0,'constrain actual SDK record replacement below full original table cost')
    texture=C.addressof((C.c_ubyte*0x40000).in_dll(frame,'texture_buffer'))
    if a.case!='late_render_failure':check(bind(frame,'jp_render_state_register_buffer',I,P,S,U)(texture,0x40000,4096)==0,'register original aligned main scratch without replacing njInit3D')
    media=d/'media';media.mkdir();store=False
    if a.case!='no_store':
        check(bind(frame,'jp_vmu_store_initialize',I,C.c_wchar_p)(str(media))==0,'borrowed actual VMU store');store=True
        check(bind(frame,'jp_vmu_store_create',I,U)(0)==0 and bind(frame,'jp_vmu_store_open',I,U)(0)==0,'fresh isolated actual VMU image and lock')
        check(bind(frame,'jp_lcd_initialize',I,U)(1)==0,'borrowed actual VMU LCD endpoint')
    track=C.create_unicode_buffer(str(a.track.resolve()));keep.append(track)
    options=SystemOptions(96,0,DeviceOptions(640,480,0,0,5000,0,64<<20),C.addressof(track),2352,3,5000,0,C.cast(source,P).value,None,None,None)
    check(bind(system,'jp_system_configure',I,P)(C.byref(options))==0,'copied actual System host configuration')
    driver=C.create_string_buffer(b'MISSING.DRV' if a.case in ('missing_driver','failure_retry','fatal_driver') else b'masaru.drv');keep.append(driver)
    so=SoundOptions(56,0,2<<20,5000,AicaOptions(32,1,16384,40,5000,1,1,0),C.addressof(driver));clock=ClockOptions(56,441,8192,5,None,None,1 if a.case=='clock_failure' else 0,None,None)
    check(configure(None,None)==1,'null configuration rejected')
    check(configure(C.byref(so),C.byref(clock))==0,'copied bridge sound/clock configuration')
    # Overwriting caller memory proves that the real startup uses copied data.
    C.memset(driver,ord('Z'),len(driver)-1);so.aica.flags=0;clock.quantum_frames=0
    if a.case=='foreign':
        result=[];thread=threading.Thread(target=lambda:result.extend((initialize(),shutdown(5000),configure(C.byref(so),C.byref(clock)))));thread.start();thread.join();check(result==[3,3,3],'foreign creator-thread calls rejected before changing real owners')
    I.in_dll(frame,'dat_8c78b4d8').value=29;I.in_dll(frame,'dat_8c78b4dc').value=-41
    before=P.in_dll(frame,'_nj_texture_buffer_st').value
    if a.case=='fatal_driver':
        bind(boot,'system_boot_init',None)();raise AssertionError('failed production void parent returned')
    if a.case=='void_complete':bind(boot,'system_boot_init',None)();rc=0
    else:rc=initialize()
    observations['initialize_result']=rc;print('actual full original parent result',rc,flush=True)
    if a.case=='no_store':
        check(rc==7 and snap(system,'jp_system_snapshot',SystemInfo).resources==0,'missing store rejected before any core constructor')
        state=snap(boot,'jp_main_boot_snapshot',BootInfo);check(state.starts==state.sound_calls==0 and I.in_dll(frame,'dat_8c78b4d8').value==29 and I.in_dll(frame,'dat_8c78b4dc').value==-41,'preconstructor rejection preserves main sentinels and never enters original body')
    elif a.case in ('missing_driver','failure_retry','clock_failure'):
        check(rc==5,'actual owned sound/clock failure stops the adapted C parent')
        check(snap(system,'jp_system_snapshot',SystemInfo).resources==0 and snap(sound,'jp_sound_snapshot',SoundInfo).resources==0,'failed original parent rolled back actual core and sound')
        check(P.in_dll(frame,'_nj_texture_buffer_st').value==before,'no later original texture initializer executed after sound failure')
        check(snap(boot,'jp_main_boot_snapshot',BootInfo).completions==0,'failed parent is never reported complete')
    elif a.case in ('late_render_failure','late_texture_failure'):
        check(rc==6,'actual late renderer or texture-table rejection is propagated')
        state=snap(boot,'jp_main_boot_snapshot',BootInfo);check(state.phase==4 and not state.card_owned and state.completions==0,'late failed parent delegates acquired card cleanup to System without claiming completion')
        check(snap(system,'jp_system_snapshot',SystemInfo).resources==245762+524288+1048576+2097152,'late failure retains actual sound/core until explicit cleanup')
    else:
        check(rc==0,'complete original system_boot_init returned successfully')
        state=snap(boot,'jp_main_boot_snapshot',BootInfo);check(state.phase==3 and state.completions==1 and state.sound_calls==1 and not state.card_owned,'one original full parent and one actual owned sound call')
        check(initialize()==2 and configure(None,None)==2,'READY parent rejects repeated initialize and configure')
        si=snap(system,'jp_system_snapshot',SystemInfo);check(si.phase==3 and si.resources==245762+524288+1048576+2097152,'same System owns actual core sound card and font')
        si=snap(sound,'jp_sound_snapshot',SoundInfo);check(si.phase==3 and si.status==0 and si.dispatch_token and si.resources,'actual original sound initialized with live frame dispatch')
        ci=snap(sound,'jp_sound_clock_snapshot',ClockInfo);observations['clock_after_parent']={n:getattr(ci,n) for n,_ in ci._fields_};check(ci.phase==1 and ci.quantum_frames==441 and ci.service_calls>0 and ci.render_calls>0,'copied production clock services real firmware during original vertex wait')
        for name in ('jp_vertex_lifecycle_status','jp_print_status','jp_render_state_status','jp_render_state_check_guards','jp_queue_status','jp_textures_status'):
            module=C.CDLL(b['vertex_lifecycle_DLL']) if name=='jp_vertex_lifecycle_status' else frame
            check(bind(module,name,I)()==0,'actual post-parent '+name)
        check(U.in_dll(frame,'_rnext').value==0xdead0cab,'original display reset initializes real random seed')
        check(I.in_dll(frame,'dat_8c78b4d8').value==0 and I.in_dll(frame,'dat_8c78b4dc').value==0,'original parent clears actual shared main state')
        check(P.in_dll(frame,'_nj_texture_buffer_st').value==texture and P.in_dll(frame,'_nj_vertex_buf_').value==texture and I.in_dll(frame,'_nj_vertex_num_').value==4096,'original texture and vertex setters use the actual registered shared buffer')
        shape=C.addressof((F*6144).in_dll(frame,'shape_buffer'));check(P.in_dll(frame,'_nj_shape_buf_').value==shape,'original shape setter points to shared original storage')
        stack=C.addressof((C.c_ubyte*8192).in_dll(frame,'matrix_stack'));check(P.in_dll(frame,'_nj_base_matrix_ptr_').value==stack,'original display reset initializes genuine main matrix stack')
        get=bind(frame,'njGetPeripheral',P,I);check(list((P*4).in_dll(frame,'per_info'))==[get(port) for port in (0,6,12,18)],'original parent binds all four actual peripheral pointers')
        card=Card();check(bind(frame,'jp_card_state_read',I,U,P)(0,C.byref(card))==0 and card.initialized==1 and bind(frame,'jp_card_selected_work_matches',I)()==1,'original card_init created actual asynchronous backend with original work')
        vmu=StateInfo();check(bind(frame,'jp_vmu_state_read',I,P)(C.byref(vmu))==0 and vmu.skip==vmu.updates==vmu.request==0,'original VMU initializer reset actual state')
        screen=Screen.from_address(bind(frame,'jp_vmu_screen',P,U)(0));check(screen.port==0 and screen.held==screen.pressed==0,'original VMU screen probe and input reset')
        if a.case=='reentrant':check(reentry and set(reentry)=={(4,4,4)},'actual input callbacks cannot reenter initialize shutdown or configure')
        check(bind(frame,'jp_textures_live_surfaces',U)()>texture_baseline[1],'original print constructor owns an actual font surface')
        # Draw real original font through the GPU already created by System.
        check(bind(frame,'jp_queue_reset_frame',I)()==0,'reset actual queue for post-bootstrap diagnostic capture')
        bind(frame,'njPrintC',None,U,C.c_char_p)(0x00020002,b'FULL ORIGINAL BOOTSTRAP TEST')
        check(bind(frame,'jp_print_status',I)()==0,'actual original font draw after full initializer')
        fid=Q();check(bind(frame,'jp_frames_submit',I,P)(C.byref(fid))==0,'capture genuine post-bootstrap frame')
        gpu=GPU.__new__(GPU);gpu.lib=C.CDLL(b['GPU_DLL']);gpu.handle=bind(device,'jp_device_gpu',P)();gpu.draw=bind(gpu.lib,'jp_gpu_draw_frame',I,P,Q,C.POINTER(Config),P,S,C.POINTER(Report),C.POINTER(Failure))
        cfg=Config.make(640,480,1);cfg.logical_width=640;cfg.logical_height=480;cfg.clear_rgba[:]=[0,0,0,1]
        image_rc,pixels,report,error=gpu.image(fid.value,cfg);check(image_rc==0 and report.triangles>0 and report.texture_uploads>0,('same actual System GPU drew original font',image_rc,error.code))
        png(d/'full-original-bootstrap.png',640,480,pixels);observations['capture_RGBA_sha256']=hashlib.sha256(pixels).hexdigest();observations['capture_triangles']=report.triangles
        check(bind(frame,'jp_frame_release',I,Q)(fid.value)==0,'release actual diagnostic frame')
    texture_before_cleanup=(bind(frame,'jp_textures_allocated_bytes',S)(),bind(frame,'jp_textures_live_surfaces',U)())
    if a.case=='late_texture_failure':
        check(shutdown(5000)==8,'sticky texture error retains font cleanup authority and reports real failure')
        check(bind(frame,'jp_textures_live_surfaces',U)()==texture_before_cleanup[1]==1,'failed cleanup does not pretend the actual font surface retired')
        check(configure(None,None)==2,'retained font obligation prevents reconfiguration')
        check(snap(system,'jp_system_snapshot',SystemInfo).resources==2097152+32768,'System retains the failed font and its dependent SDK heap until cleanup can retry')
        check(bind(frame,'jp_texture_sdk_limits',I,U,S)(1024,32<<20)==0,'caller corrects actual SDK replacement budget')
        bind(frame,'jp_textures_clear_error',None)()
    check(shutdown(5000)==0,'real parent cleanup drains original card work then actual sound/System owners')
    texture_after_cleanup=(bind(frame,'jp_textures_allocated_bytes',S)(),bind(frame,'jp_textures_live_surfaces',U)())
    if texture_before_cleanup[1]:check(texture_after_cleanup==(texture_before_cleanup[0]-32768,0),'actual font payload retires while resized original table metadata remains owned')
    else:check(texture_after_cleanup==texture_baseline,'pre-font failure leaves actual texture ownership unchanged')
    if a.case in ('restart','failure_retry'):
        check(bind(frame,'jp_queue_reset_frame',I)()==0,'empty actual command storage before same-process restart')
        check(bind(system,'jp_system_configure',I,P)(C.byref(options))==0,'reconfigure the same real System owner after cleanup')
        again=SoundOptions(56,0,2<<20,5000,AicaOptions(32,1,16384,40,5000,1,1,0),None);clock_again=ClockOptions(56,441,8192,5,None,None,0,None,None)
        check(configure(C.byref(again),C.byref(clock_again))==0 and initialize()==0,'corrected configuration runs complete original parent using restarted real owners')
        state=snap(boot,'jp_main_boot_snapshot',BootInfo);check(state.starts==state.sound_calls==2 and state.completions==(1 if a.case=='failure_retry' else 2),'completed-call counts distinguish failed attempt from full successful initialization')
        check(shutdown(5000)==0,'second original card/sound/core lifetime drains successfully')
    check(snap(system,'jp_system_snapshot',SystemInfo).resources==0 and snap(sound,'jp_sound_snapshot',SoundInfo).resources==0,'no core or sound resources remain')
    card=Card();check(bind(frame,'jp_card_state_read',I,U,P)(0,C.byref(card))==0 and not card.initialized,'real buExit retired native card backend')
    check(bind(frame,'jp_textures_live_surfaces',U)()==0,'no actual font surface survives into borrowed texture shutdown')
    if store:
        check(bind(frame,'jp_lcd_shutdown',I)()==0 and bind(frame,'jp_vmu_store_shutdown',I)()==0,'caller releases borrowed LCD/store after original filesystem fibers retire')
        # The sidecar file intentionally persists; the exclusive OS HANDLE is
        # the lock. Reopening the image proves that ownership was released.
        check(bind(frame,'jp_vmu_store_initialize',I,C.c_wchar_p)(str(media))==0 and bind(frame,'jp_vmu_store_open',I,U)(0)==0,'actual VMU image can be exclusively reopened after cleanup')
        check(bind(frame,'jp_vmu_store_shutdown',I)()==0,'release independently reopened image owner')
    check(bind(frame,'jp_frames_shutdown',I)()==0,'caller releases borrowed captured frames')
    bind(frame,'jp_render_state_shutdown',None)();bind(frame,'jp_textures_shutdown',None)();bind(frame,'jp_queue_shutdown',None)()
    faulthandler.disable();fault.close();result=dict(case=a.case,checks=len(labels),assertions=labels,observations=observations,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10)
    (d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
if __name__=='__main__':main()
