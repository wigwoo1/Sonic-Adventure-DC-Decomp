"""Real compiled sbInitSystem feeds original reset/boot/logo and native exit.
The Python caller supplies borrowed render prerequisites and controlled samples;
it does not create the heap/device/input/RTC/GDFS core services.
"""
import ctypes as C,hashlib,inspect,json,os,sys
from pathlib import Path
from types import SimpleNamespace
import verify_native_device_exit_startup as parent
import verify_native_exit_logo as leaf
from native_packet_test_support import bind
from verify_native_system import Options as SystemOptions,Info as SystemInfo,CORE
from verify_native_device_init import Options as DeviceOptions,Info as DeviceInfo
from verify_native_sdk_heap import Info as HeapInfo
from verify_native_gdfs import Info as GDFSInfo
from verify_native_rtc import Info as RTCInfo
from verify_native_ninja_global import Screen
from verify_native_input_backend import Source,Sample
I=C.c_int32;U=C.c_uint32;P=C.c_void_p
owner={}
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def row(out,name,n,**fields):(out/(name+'.json')).write_text(json.dumps(dict(checks=n,wrapper_sha256=sha(__file__),**fields),indent=2)+'\n')
def info(module,name,kind):
    v=kind();assert bind(module,name,I,P)(C.byref(v))==0;return v
def prepare(b,lib,present,out):
    sl=C.CDLL(b['System_DLL']);dl=C.CDLL(b['device_DLL']);owner.update(system=sl,frames=lib,fs=C.CDLL(b['GDFS_DLL']),rtc=C.CDLL(b['RTC_DLL']),heap=C.CDLL(b['SDK_heap_DLL']),target=None)
    @Source
    def controlled(context,samples):
        if owner['target'] is not None:return owner['target'](context,samples)
        C.memset(samples,0,4*C.sizeof(Sample));samples[0]=Sample(1,0,0,0,0,0,0,0);return 1
    owner['source']=controlled
    track=C.create_unicode_buffer(os.environ['JP_NATIVE_SYSTEM_TEST_TRACK']);owner['track']=track
    options=SystemOptions(C.sizeof(SystemOptions),0,DeviceOptions(640,480,0,0,5000,0,64<<20),C.addressof(track),2352,3,5000,0,C.cast(controlled,P).value,None,None,None)
    bind(lib,'njSetTextureMemorySize',None,U)(0x11c000);cable=bind(lib,'syCblCheckCable',I)();I.in_dll(lib,'cable_mode').value=cable;mode=1 if cable==0 else 0x34
    assert bind(sl,'jp_system_configure',I,P)(C.byref(options))==0
    bind(sl,'sbInitSystem',None,I,I,I)(mode,0,1)
    assert bind(sl,'jp_system_status',I)()==0
    device=info(dl,'jp_device_snapshot',DeviceInfo)
    assert (device.phase,device.resources,device.width,device.height,device.render_width,device.render_height)==(4,63,640,480,640,480)
    screen=Screen.in_dll(lib,'_nj_screen_');assert (screen.width,screen.height,screen.cx,screen.cy)==(640,480,320,240)
    assert I.in_dll(lib,'_nj_eov_flag_').value==1 and I.in_dll(lib,'_nj_fatal_err_callback_').value==0
    gpu=P(bind(dl,'jp_device_gpu',P)());window=P(bind(dl,'jp_device_window',P)());assert gpu and window
    row(out,'native_device_setup',6,status='actual_compiled_sbInitSystem_created_native_Ninja_device_before_original_boot',actual_native_entry='sbInitSystem',mode=mode,cable=cable,manual_GPU_window_presenter_or_core_initialization_used=False)
    state=info(sl,'jp_system_snapshot',SystemInfo);assert state.phase==3 and state.resources==CORE and state.mode==mode and state.window
    heap=info(owner['heap'],'jp_sdk_heap_snapshot',HeapInfo);assert heap.initialized and heap.usable==1<<20 and heap.live==0
    rtc=info(owner['rtc'],'jp_rtc_snapshot',RTCInfo);fs=info(owner['fs'],'jp_gdfs_snapshot',GDFSInfo);assert rtc.initialized and fs.dir_entries==1863
    assert bind(lib,'jp_input_status',I)()==bind(lib,'jp_input_bootstrap_status',I)()==0
    h=bind(owner['fs'],'gdFsOpen',P,C.c_char_p,P)(b'SET0001S.BIN',None);buf=(C.c_ubyte*2048)()
    assert h and bind(owner['fs'],'gdFsRead',I,P,I,P)(h,1,buf)==0 and hashlib.sha256(bytes(buf[:64])).hexdigest()=='16abab341fb7f370e27e4dadcf81766dd0dfd0ae64469477bb2cf6614938b2af'
    bind(owner['fs'],'gdFsClose',None,P)(h);assert info(owner['fs'],'jp_gdfs_snapshot',GDFSInfo).live_handles==0
    row(out,'native_system_setup',6,status='compiled_native_core_created_real_heap_device_input_RTC_GDFS',core_resources=CORE,actual_native_entry='sbInitSystem',native_functions_replaced=False)
    return dict(owner=dl,gpu=SimpleNamespace(handle=gpu,lib=C.CDLL(b['GPU_DLL'])),window=window,ready=True)
def set_source(source,lib):owner['target']=source;return bind(lib,'jp_input_poll',I)()
def make_binder(b,ex):
    sl=C.CDLL(b['System_DLL']);ordinary=bind(ex,'jp_exit_bind',I,P)
    def bind_final(config):
        if not config:return ordinary(config)
        if info(sl,'jp_system_snapshot',SystemInfo).phase!=3:return ordinary(config)
        from verify_native_exit_logo import ExitConfig
        cfg=C.cast(config,C.POINTER(ExitConfig)).contents;assert not cfg.gpu and not cfg.window
        return bind(sl,'jp_system_bind_exit',I,U)(cfg.resources)
    return bind_final
def bound(out):
    state=info(owner['system'],'jp_system_snapshot',SystemInfo);assert state.phase==4 and not state.resources
    assert bind(owner['fs'],'jp_gdfs_shutdown',I)()==2
    h=bind(owner['fs'],'gdFsOpen',P,C.c_char_p,P)(b'ADV00.PRS',None);assert h and bind(owner['fs'],'gdFsReqGdRd',I,P,I)(h,64)==64 and info(owner['fs'],'jp_gdfs_snapshot',GDFSInfo).pending
    owner['stream']=h;row(out,'native_system_bound',3,status='compiled_core_transferred_to_full_exit_with_real_pending_GDFS_stream',complete_exit_mask=247798)
def completed(out):
    state=info(owner['system'],'jp_system_snapshot',SystemInfo);assert state.phase==6 and not state.resources and not state.window
    assert not info(owner['heap'],'jp_sdk_heap_snapshot',HeapInfo).initialized and not info(owner['rtc'],'jp_rtc_snapshot',RTCInfo).initialized and not info(owner['fs'],'jp_gdfs_snapshot',GDFSInfo).work
    assert bind(owner['system'],'jp_system_initialize',I,I,I,I)(1,0,1)==2
    row(out,'native_system_completed',3,status='all_compiled_core_owners_retired_by_final_native_exit')
def main():
    if '--track' in sys.argv:
        at=sys.argv.index('--track');os.environ['JP_NATIVE_SYSTEM_TEST_TRACK']=str(Path(sys.argv[at+1]).resolve());del sys.argv[at:at+2]
    assert os.environ.get('JP_NATIVE_SYSTEM_TEST_TRACK'),'Pass --track with the local Japanese track03.bin'
    worker='--worker' in sys.argv;out=Path(sys.argv[sys.argv.index('--output')+1]).resolve();original=inspect.getsource;entry=leaf.main;old_main=parent.main;old_file=parent.__file__;base_hash=sha(old_file)
    source=original(old_main);start=source.index('    def prepare(b,lib,present):');end=source.index('    def prelude(b,lib,present):',start)
    source=source[:start]+'    def prepare(b,lib,present):\n        return native_system_prepare(b,lib,present,out)\n'+source[end:]
    assert source.count('18422')==3;source=source.replace('18422','247798')
    marker='    def bound(owner,exit_library):\n';assert source.count(marker)==1;source=source.replace(marker,marker+'        native_system_bound(out)\n')
    marker='    def exited(owner):\n';assert source.count(marker)==1;source=source.replace(marker,marker+'        native_system_completed(out)\n')
    derived=[]
    def getsource(fn):
        value=original(fn)
        if fn is entry:
            old="    check(bind(lib,'jp_input_initialize',I,P,U,P,P)(hwnd,0,C.cast(source,P),None)==0 and bind(lib,'jp_input_poll',I)()==0,'actual native sample provider primed')"
            assert value.count(old)==1;value=value.replace(old,"    check(native_system_set_source(source,lib)==0,'actual compiled-core sample provider primed')")
            old="bind_exit=bind(ex,'jp_exit_bind',I,P)";assert value.count(old)==1;value=value.replace(old,'bind_exit=native_system_make_binder(b,ex)');derived.append(hashlib.sha256(value.encode()).hexdigest())
        return value
    parent.native_system_prepare=prepare;parent.native_system_bound=bound;parent.native_system_completed=completed
    leaf.native_system_set_source=set_source;leaf.native_system_make_binder=make_binder
    parent.__file__=__file__;exec(compile(source,__file__,'exec'),parent.__dict__);inspect.getsource=getsource
    try:parent.main()
    finally:parent.main=old_main;parent.__file__=old_file;inspect.getsource=original
    if worker:raise AssertionError('native process exit returned')
    path=out/'verification.json';v=json.loads(path.read_bytes());assert v['checks']==7215 and v['actual_Presents']==570;extra=0
    for case in ('movie0','movie6','advertise1','cleanup','quit','pending_quit'):
        for name in ('native_system_setup','native_system_bound'):
            data=json.loads((out/case/(name+'.json')).read_bytes());assert data['wrapper_sha256']==sha(__file__);extra+=data['checks']
        if case not in ('quit','pending_quit'):
            data=json.loads((out/case/'native_system_completed.json').read_bytes());assert data['wrapper_sha256']==sha(__file__);extra+=data['checks']
    assert extra==66
    v.update(status='compiled_native_sbInitSystem_original_reset_boot_logo_and_ordered_final_exit_verified',checks=v['checks']+extra,native_core_setup_and_final_handoff_checks=extra,
        actual_compiled_sbInitSystem_calls=6,core_services_initialized_by_Python=False,borrowed_render_prerequisites_created_by_Python=True,controlled_input_provider=True,
        actual_pending_GDFS_streams_cancelled=6,actual_full_native_core_resource_mask=CORE,actual_full_exit_mask=247798,
        original_native_or_SDK_function_bodies_replaced=False,baseline_device_startup_verifier_sha256=base_hash,derived_Python_caller_sha256=hashlib.sha256(source.encode()).hexdigest(),derived_leaf_callers_sha256=derived,
        verifier_sha256=sha(__file__),full_native_main_system_boot_init_audio_or_gameplay_verified=False)
    path.write_text(json.dumps(v,indent=2)+'\n');print(v['status'],v['checks'],'checks',flush=True)
if __name__=='__main__':main()
