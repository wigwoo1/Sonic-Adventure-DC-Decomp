"""Original reset/boot/logo through actual device creation and delegated exit.

Derive the existing Python caller only. Original game/SDK/native providers,
whole-image oracle and non-returning native exits remain real functions.
"""
import ctypes as C,hashlib,inspect,json,sys
from pathlib import Path
from types import SimpleNamespace
import verify_native_display_reset_boot as parent
import verify_native_exit_logo as leaf
from native_packet_test_support import bind
from verify_native_device_init import Info as DeviceInfo,Options
from verify_native_ninja_global import Screen
I=C.c_int32;U=C.c_uint32;P=C.c_void_p;Q=C.c_uint64;Callback=C.CFUNCTYPE(None,P)
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    argv=sys.argv[1:];out=Path(argv[argv.index('--output')+1]).resolve();worker='--worker' in argv;keep=[]
    original_getsource=inspect.getsource;original_leaf=leaf.main;real_present=leaf.Present;derived_hashes=[]
    def prepare(b,lib,present):
        dl=C.CDLL(b['device_DLL']);options=Options(640,480,0,0,5000,0,64<<20)
        bind(lib,'njSetTextureMemorySize',None,U)(0x11c000)
        cable=bind(lib,'syCblCheckCable',I)();I.in_dll(lib,'cable_mode').value=cable;mode=1 if cable==0 else 0x34
        assert bind(dl,'jp_device_configure',I,P)(C.byref(options))==0
        assert bind(dl,'jp_device_initialize',I,I,I,I)(mode,0,1)==0
        info=DeviceInfo();assert bind(dl,'jp_device_snapshot',I,P)(C.byref(info))==0
        gpu=P(bind(dl,'jp_device_gpu',P)());window=P(bind(dl,'jp_device_window',P)())
        assert gpu and window and (info.phase,info.resources,info.width,info.height,info.render_width,info.render_height)==(4,63,640,480,640,480)
        screen=Screen.in_dll(lib,'_nj_screen_');assert (screen.width,screen.height,screen.cx,screen.cy)==(640,480,320,240)
        assert I.in_dll(lib,'_nj_eov_flag_').value==1 and I.in_dll(lib,'_nj_fatal_err_callback_').value==0
        report=dict(status='actual_complete_Ninja_device_initialization_before_original_boot',checks=6,
            actual_native_entry='jp_device_initialize',original_complete_Ninja_init_and_global_executed=True,
            GPU_window_clock_video_wait_presenter_created_by_native_owner=True,manual_GPU_window_or_presenter_initialization_used=False,
            startup_logical_ticks=int(info.ticks),mode=mode,cable=cable,native_functions_replaced=False,wrapper_sha256=sha(__file__))
        (out/'native_device_setup.json').write_text(json.dumps(report,indent=2)+'\n')
        return dict(owner=dl,gpu=SimpleNamespace(handle=gpu,lib=C.CDLL(b['GPU_DLL'])),window=window,ready=True)
    def prelude(b,lib,present):
        checks=0
        def check(ok):
            nonlocal checks
            assert ok;checks+=1
        buffer=C.addressof((C.c_byte*0x40000).in_dll(lib,'texture_buffer'));shape=C.addressof((C.c_float*6144).in_dll(lib,'shape_buffer'))
        check(bind(lib,'jp_render_state_register_buffer',I,P,C.c_size_t,U)(buffer,0x40000,4096)==0)
        bind(present.lib,'frame_pacing_init',None,I,I)(2,2)
        bind(lib,'njInitTextureBuffer',None,P,U)(buffer,0x40000)
        life=C.CDLL(b['vertex_lifecycle_DLL']);bind(life,'njInitVertexBuffer',None,I,I,I,I,I)(-1,0,0x20000,0,0x8000)
        check(bind(life,'jp_vertex_lifecycle_status',I)()==0)
        bind(lib,'njInitPrint',None,P,U,U)(None,0,0);check(bind(lib,'jp_print_status',I)()==0)
        bind(lib,'text_style_init',None,P,U,U)(None,0,0);style=(U*6)();bind(lib,'jp_game_text_snapshot',None,P)(style)
        check(style[0]==0xffbfbfbf and style[1]==0x41800000 and style[5]!=0)
        bind(lib,'njInit3D',None,P,I)(buffer,4096);bind(lib,'njInitShape',None,P)(shape);bind(lib,'njSetBorderColor',None,U)(0)
        check(P.in_dll(lib,'_nj_texture_buffer_st').value==P.in_dll(lib,'_nj_vertex_buf_').value==buffer)
        check(P.in_dll(lib,'_nj_shape_buf_').value==shape and U.in_dll(lib,'_nj_texture_buffer_max_size').value==0x40000)
        check(I.in_dll(lib,'_nj_vertex_num_').value==4096 and bind(lib,'jp_render_state_capacity',U)()==2048)
        check(bind(lib,'jp_render_state_status',I)()==0 and bind(lib,'jp_render_state_buffer_is_borrowed',I)()==1)
        check(bind(present.lib,'jp_wait_status',I)()==bind(present.lib,'jp_vblank_chain_status',I)()==0)
        check(I.in_dll(present.lib,'_nj_blank_count_').value>=1 and bind(present.lib,'jp_vblank_chain_count',U)()>=1)
        (out/'native_device_main_prelude.json').write_text(json.dumps(dict(status='actual_main_buffers_vertex_font_and_style_after_native_device',checks=checks,native_functions_replaced=False,wrapper_sha256=sha(__file__)),indent=2)+'\n')
    def bound(owner,exit_library):
        assert bind(owner,'jp_device_shutdown',I)()==2
        assert bind(exit_library,'jp_exit_status',I)()==0
        (out/'native_device_exit_bound.json').write_text(json.dumps(dict(status='actual_device_claimed_by_one_terminal_exit_authority',checks=2,ordinary_device_close_rejected=True,expected_resource_mask=18422,wrapper_sha256=sha(__file__)),indent=2)+'\n')
    def exited(owner):
        info=DeviceInfo();assert bind(owner,'jp_device_snapshot',I,P)(C.byref(info))==0 and info.phase==info.resources==0
        assert bind(owner,'jp_device_shutdown',I)()==2
        (out/'native_device_exit_completed.json').write_text(json.dumps(dict(status='actual_delegated_device_owner_closed_after_dependent_cleanup',checks=2,phase=int(info.phase),resources=int(info.resources),wrapper_sha256=sha(__file__)),indent=2)+'\n')
    def present_factory(*args,**kwargs):
        value=real_present(*args,**kwargs);lib=value.lib;original_callback=value.callback;ninja=bind(lib,'njSetEORflag',None,P)
        def observe(address,context):
            observer=Callback(address.value if isinstance(address,P) else address);calls=0;checks=0;errors=[]
            @Callback
            def completed(argument):
                nonlocal calls,checks
                try:
                    ninja(argument);calls+=1
                    assert I.in_dll(lib,'_nj_render_flag_').value==1 and I.in_dll(lib,'_nj_eor_flag_').value==0;checks+=1
                    assert bind(lib,'jp_vblank_chain_status',I)()==0;checks+=1
                except BaseException as error:errors.append(str(error))
                observer(argument)
                (out/'native_device_gpu_eor.json').write_text(json.dumps(dict(status='real_GPU_completion_original_Ninja_EOR_and_passive_pixel_observer',checks=checks,actual_GPU_EOR_callbacks=calls,errors=errors,wrapper_sha256=sha(__file__)),indent=2)+'\n')
            keep.extend([completed,observer]);return original_callback(C.cast(completed,P),context)
        value.callback=observe;return value
    def caller_source(function):
        source=original_getsource(function)
        if function is not original_leaf:return source
        def patch(old,new):
            nonlocal source
            assert source.count(old)==1,(old,source.count(old));source=source.replace(old,new)
        patch("    gpu=GPU(Path(b['GPU_DLL']));gl=gpu.lib;assets=Assets(lib);pvm=PVM(lib)","    gl=C.CDLL(b['GPU_DLL']);assets=Assets(lib);pvm=PVM(lib)")
        patch("and Video(lib).init(640,480,0xff000000,1)==0 and ri(128,1<<20)==0 and bind(lib,'jp_ninja_contexts_initialize',I)()==0",'and ri(128,1<<20)==0')
        patch("    window=P();failure=Failure();check(bind(gl,'jp_window_open',I,P,U,U,I,P,P)(gpu.handle,640,480,0,C.byref(window),C.byref(failure))==0,'owned hidden DXGI window')", "    native_device=native_device_prepare(b,lib,present);gpu=native_device['gpu'];window=native_device['window']\n    check(native_device['ready'],'actual native Ninja-owned GPU and DXGI window')")
        patch("check(present.clock.init(None,None,0)==0 and present.wait_init(60000,1001,8,C.cast(present.window_idle,P),window)==0,'real QPC and logical-vsync/window wait')", "check(present.clock.status()==present.wait_status()==0,'real QPC and logical wait created by native Ninja owner')")
        patch("check(present.init(gpu.handle,window,device,C.byref(cfg),5000,0)==0,'real presenter')", "check(present.status()==0,'actual presenter initialized inside native Ninja owner')")
        marker="    limits=Limits(4<<20,16<<20,32<<20,64);check(assets.init(str(a.asset_root),C.byref(limits))==0,'real retail-input asset root')"
        patch(marker,marker+'\n    native_device_main_prelude(b,lib,present)')
        patch('config=ExitConfig(C.sizeof(ExitConfig),16383,gpu.handle,window)','config=ExitConfig(C.sizeof(ExitConfig),18422,None,None)')
        marker="check(bind_exit(C.byref(config))==0,'transfer native shutdown authority after full initialization')"
        patch(marker,marker+";native_device_exit_bound(native_device['owner'],ex)")
        patch('pvm.loaded(listing())==0 and runtime.live()==0',"pvm.loaded(listing())==0 and runtime.live()==1 and bind(lib,'jp_texture_surface_owned',I,P)(bind(lib,'jp_print_font_surface',P)())==1")
        patch("system_exit();s=state();check(s.remaining==0 and s.completed==16383", "system_exit();native_device_exit_completed(native_device['owner']);s=state();check(s.remaining==0 and s.completed==18422")
        derived_hashes.append(hashlib.sha256(source.encode()).hexdigest());return source
    leaf.native_device_prepare=prepare;leaf.native_device_main_prelude=prelude;leaf.native_device_exit_bound=bound;leaf.native_device_exit_completed=exited;leaf.Present=present_factory
    inspect.getsource=caller_source;baseline=sha(parent.__file__);parent.__file__=__file__
    try:parent.main()
    finally:inspect.getsource=original_getsource
    if worker:raise AssertionError('native process exit returned')
    path=out/'verification.json';v=json.loads(path.read_bytes());assert v['checks']==5959 and v['actual_Presents']==v['actual_GPU_images']==570
    extra=0
    for case in ('movie0','movie6','advertise1','cleanup','quit','pending_quit'):
        for name in ('native_device_setup','native_device_main_prelude','native_device_exit_bound'):
            row=json.loads((out/case/(name+'.json')).read_bytes());assert row['wrapper_sha256']==sha(__file__);extra+=row['checks']
        if case not in ('quit','pending_quit'):
            row=json.loads((out/case/'native_device_exit_completed.json').read_bytes());assert row['wrapper_sha256']==sha(__file__) and row['resources']==0;extra+=row['checks']
        if case in ('movie0','movie6','advertise1'):
            row=json.loads((out/case/'native_device_gpu_eor.json').read_bytes());assert row['actual_GPU_EOR_callbacks']==190 and row['checks']==380 and not row['errors'];extra+=row['checks']
    v.update(status='actual_native_Ninja_device_creation_main_buffers_original_reset_boot_logo_and_delegated_exit_verified',checks=v['checks']+extra,
        device_startup_and_exit_checks=extra,actual_complete_native_device_initializations=6,manual_GPU_window_presenter_initialization_used=False,
        actual_main_vertex_font_game_style_setup_calls=6,actual_native_device_exit_authority_claims=6,
        actual_original_Ninja_GPU_EOR_callbacks=570,source_selected_native_entries=['jp_device_initialize','boot_init'],
        caller_changes_only=True,native_game_or_SDK_functions_replaced=False,full_sbInitSystem_or_audio_or_gameplay_verified=False,
        baseline_reset_boot_verifier_sha256=baseline,derived_Python_caller_sha256=derived_hashes[0],verifier_sha256=sha(__file__))
    path.write_text(json.dumps(v,indent=2)+'\n');print(v['status'],v['checks'],'checks',flush=True)
if __name__=='__main__':main()
