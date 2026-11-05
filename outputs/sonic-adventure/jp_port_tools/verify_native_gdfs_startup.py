"""Actual GDFS alongside the qualified original boot/logo and delegated exit.
Only the Python platform caller is extended; native/original bodies stay real.
"""
import ctypes as C,hashlib,inspect,json,os,sys
from pathlib import Path
import verify_native_peripheral_startup as parent
import verify_native_rtc_startup as rtc_parent
import verify_native_exit_logo as leaf
from native_packet_test_support import bind
from verify_native_gdfs import Info
I=C.c_int32;U=C.c_uint32;P=C.c_void_p
owner={}
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def save(out,name,checks,**kw):(out/(name+'.json')).write_text(json.dumps(dict(checks=checks,wrapper_sha256=sha(__file__),**kw),indent=2)+'\n')
def fsinfo():
    v=Info();assert bind(owner['lib'],'jp_gdfs_snapshot',I,P)(C.byref(v))==0;return v
def prepare(b,lib,out):
    fs=C.CDLL(b['GDFS_DLL']);owner['lib']=fs;track=Path(os.environ['JP_GDFS_STARTUP_TEST_TRACK']).resolve()
    assert bind(fs,'jp_gdfs_configure',I,C.c_wchar_p,U,U,U,U)(str(track),45150,2352,3,5000)==0
    assert bind(fs,'jp_gdfs_initialize',I,U,U)(8,1920)==0
    v=fsinfo();assert v.phase==3 and v.dir_entries==248 and v.vblank and v.handle_bytes==120
    assert bind(fs,'gdFsChangeDir',I,C.c_char_p)(b'SONICADV')==0 and fsinfo().dir_entries==1863
    h=bind(fs,'gdFsOpen',P,C.c_char_p,P)(b'SET0001S.BIN',None);assert h
    data=(C.c_ubyte*2048)();assert bind(fs,'gdFsRead',I,P,I,P)(h,1,data)==0
    assert hashlib.sha256(bytes(data[:64])).hexdigest()=='16abab341fb7f370e27e4dadcf81766dd0dfd0ae64469477bb2cf6614938b2af'
    bind(fs,'gdFsClose',None,P)(h);assert fsinfo().live_handles==0
    save(out,'gdfs_setup',7,status='actual_original_GDFS_native_media_and_real_game_file_read_before_original_boot',root_entries=248,game_entries=1863,known_file_sha256=hashlib.sha256(bytes(data[:64])).hexdigest(),native_functions_replaced=False)
def bound(ex,out):
    fs=owner['lib'];assert bind(fs,'jp_gdfs_shutdown',I)()==2 and bind(fs,'gdFsReinit',I)()==-13 and bind(ex,'jp_exit_status',I)()==0
    h=bind(fs,'gdFsOpen',P,C.c_char_p,P)(b'ADV00.PRS',None);assert h and bind(fs,'gdFsReqGdRd',I,P,I)(h,64)==64 and fsinfo().pending==1
    owner['h']=h;save(out,'gdfs_exit_bound',2,status='actual_GDFS_one_terminal_exit_owner_with_real_pending_stream',expected_exit_mask=247798,pending_original_request=True)
def completed(out):
    v=fsinfo();assert v.phase==1 and not(v.work or v.dir or v.vblank or v.pending)
    assert bind(owner['lib'],'jp_gdfs_shutdown',I)()==2
    assert v.callbacks==0
    save(out,'gdfs_completed',3,status='actual_GDFS_read_drained_and_handler_retired_before_device_wait_or_heap',actual_GDFS_vblanks=v.vblanks,actual_GDFS_read_bytes=v.read_bytes,late_user_callbacks=v.callbacks)
def destructor_state(out):
    v=fsinfo();assert v.phase==1 and not(v.work or v.dir or v.pending)
    assert bind(owner['lib'],'jp_gdfs_shutdown',I)()==2
    assert v.callbacks==0
    save(out,'gdfs_task_destructor',3,status='actual_original_task_destructor_observes_already_closed_GDFS',late_user_callbacks=v.callbacks)
    return dict(gdfs_phase=v.phase,gdfs_work=v.work,gdfs_pending=v.pending)
def main():
    args=sys.argv
    if '--track' in args:
        at=args.index('--track');os.environ['JP_GDFS_STARTUP_TEST_TRACK']=str(Path(args[at+1]).resolve());del args[at:at+2]
    assert os.environ.get('JP_GDFS_STARTUP_TEST_TRACK'),'Pass --track with the local Japanese track03.bin'
    worker='--worker' in args;out=Path(args[args.index('--output')+1]).resolve()
    original=inspect.getsource;entry=leaf.main;old_rtc=rtc_parent.main;baseline=sha(parent.__file__);derived=[]
    rtc_source=original(old_rtc);assert rtc_source.count('116726')==3
    rtc_source=rtc_source.replace('116726','247798');exec(compile(rtc_source,rtc_parent.__file__,'exec'),rtc_parent.__dict__)
    def getsource(fn):
        source=original(fn)
        if fn is entry:
            def patch(old,new):
                nonlocal source
                assert source.count(old)==1,(old,source.count(old));source=source.replace(old,new)
            marker="    limits=Limits(4<<20,16<<20,32<<20,64);check(assets.init(str(a.asset_root),C.byref(limits))==0,'real retail-input asset root')"
            patch(marker,marker+'\n    native_gdfs_prepare(b,lib,out)')
            marker="check(bind_exit(C.byref(config))==0,'transfer native shutdown authority after full initialization')"
            patch(marker,marker+';native_gdfs_bound(ex,out)')
            patch("    user=C.WinDLL('user32');", "    native_gdfs_completed(out)\n    user=C.WinDLL('user32');")
            marker="            (out/'destructor.json').write_text"
            patch(marker,"            data.update(native_gdfs_destructor_state(out))\n"+marker)
            derived.append(hashlib.sha256(source.encode()).hexdigest())
        return source
    leaf.native_gdfs_prepare=prepare;leaf.native_gdfs_bound=bound;leaf.native_gdfs_completed=completed;leaf.native_gdfs_destructor_state=destructor_state
    inspect.getsource=getsource;parent.__file__=__file__
    try:parent.main()
    finally:inspect.getsource=original;rtc_parent.main=old_rtc
    if worker:raise AssertionError('native process exit returned')
    path=out/'verification.json';v=json.loads(path.read_bytes());assert v['checks']==7337 and v['actual_Presents']==570;extra=0;stats={}
    for case in ('movie0','movie6','advertise1','cleanup','quit','pending_quit'):
        for name in ('gdfs_setup','gdfs_exit_bound','gdfs_task_destructor'):
            row=json.loads((out/case/(name+'.json')).read_bytes());assert row['wrapper_sha256']==sha(__file__);extra+=row['checks']
        if case not in ('quit','pending_quit'):
            row=json.loads((out/case/'gdfs_completed.json').read_bytes());assert row['wrapper_sha256']==sha(__file__) and row['late_user_callbacks']==0;extra+=row['checks'];stats[case]=row
        else:
            row=json.loads((out/case/'destructor.json').read_bytes());assert row['gdfs_phase']==1 and row['gdfs_work']==row['gdfs_pending']==0
    assert extra==84
    v.update(status='actual_original_GDFS_device_RTC_input_boot_logo_and_ordered_final_exit_verified',checks=v['checks']+extra,GDFS_setup_and_terminal_exit_checks=extra,
        actual_original_GDFS_initializations=6,actual_original_GDFS_known_file_reads=6,actual_pending_GDFS_streams_cancelled=6,GDFS_closes_before_RTC_device_wait_and_heap=True,
        GDFS_case_stats=stats,actual_combined_exit_mask=247798,caller_changes_only=True,native_game_or_SDK_function_bodies_replaced=False,
        baseline_peripheral_startup_verifier_sha256=baseline,derived_RTC_caller_sha256=hashlib.sha256(rtc_source.encode()).hexdigest(),derived_original_leaf_callers_sha256=derived,verifier_sha256=sha(__file__),
        full_native_sbInitSystem_audio_or_gameplay_verified=False)
    path.write_text(json.dumps(v,indent=2)+'\n');print(v['status'],v['checks'],'checks',flush=True)
if __name__=='__main__':main()
