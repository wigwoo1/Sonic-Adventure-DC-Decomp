"""Real PC RTC around the qualified SDK-heap/device/original boot composition.
Only Python caller/prelude changes; native and original function bodies remain.
"""
import ctypes as C,hashlib,inspect,json,sys
from pathlib import Path
import verify_native_sdk_heap_startup as parent
from verify_native_rtc import Info as RTCInfo,Date as RTCDate
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    argv=sys.argv[1:];worker='--worker' in argv;out=Path(argv[argv.index('--output')+1]).resolve();baseline=sha(parent.__file__)
    source=inspect.getsource(parent.main)
    old="    patch('18422','51190',3)";assert source.count(old)==1
    source=source.replace(old,"""    patch('18422','116726',3)
    patch('heap_owner=None;heap_fixture=None','heap_owner=None;heap_fixture=None;rtc_owner=None')
    patch('nonlocal heap_owner,heap_fixture','nonlocal heap_owner,heap_fixture,rtc_owner')
    patch('        screen=Screen.in_dll(lib,', '''        rtc_owner=C.CDLL(b['RTC_DLL'])
        assert bind(rtc_owner,'jp_rtc_configure',I,P,P)(None,None)==0 and bind(rtc_owner,'syRtcInit',I)()==0
        ri=RTCInfo();date=RTCDate();assert bind(rtc_owner,'jp_rtc_snapshot',I,P)(C.byref(ri))==0 and ri.initialized and ri.chain and ri.count
        assert bind(rtc_owner,'syRtcGetDate',I,P)(C.byref(date))==0 and 2026<=date.year<=2086
        (out/'rtc_setup.json').write_text(json.dumps(dict(status='actual_PC_RTC_and_native_chain_after_device_before_original_boot',checks=3,year=date.year,wrapper_sha256=sha(__file__)),indent=2)+'\\\\n')
        screen=Screen.in_dll(lib,''')
    patch("        (out/'sdk_heap_exit_bound.json')", '''        assert bind(rtc_owner,'jp_rtc_shutdown',I)()==10
        ri=RTCInfo();assert bind(rtc_owner,'jp_rtc_snapshot',I,P)(C.byref(ri))==0 and ri.initialized
        (out/'rtc_exit_bound.json').write_text(json.dumps(dict(status='RTC_has_one_terminal_exit_owner',checks=2,wrapper_sha256=sha(__file__)),indent=2)+'\\\\n')
        (out/'sdk_heap_exit_bound.json')''')
    patch("        (out/'sdk_heap_completed.json')", '''        ri=RTCInfo();assert bind(rtc_owner,'jp_rtc_snapshot',I,P)(C.byref(ri))==0 and not ri.initialized and not ri.chain
        assert bind(rtc_owner,'jp_rtc_shutdown',I)()==10
        (out/'rtc_completed.json').write_text(json.dumps(dict(status='RTC_closed_before_device_wait_and_heap_retirement',checks=2,actual_RTC_ticks=ri.ticks,wrapper_sha256=sha(__file__)),indent=2)+'\\\\n')
        (out/'sdk_heap_completed.json')''')
    runner.RTCInfo=RTCInfo;runner.RTCDate=RTCDate""")
    source=source.replace('SDK_heap_and_device_exit_mask=51190','SDK_heap_and_device_exit_mask=116726')
    parent.RTCInfo=RTCInfo;parent.RTCDate=RTCDate;parent.__file__=__file__
    exec(compile(source,__file__,'exec'),parent.__dict__);parent.main()
    if worker:raise AssertionError('native process exit returned')
    path=out/'verification.json';v=json.loads(path.read_bytes());assert v['checks']==7281 and v['actual_Presents']==570;extra=0;ticks={}
    for case in ('movie0','movie6','advertise1','cleanup','quit','pending_quit'):
        for name in ('rtc_setup','rtc_exit_bound'):
            row=json.loads((out/case/(name+'.json')).read_bytes());assert row['wrapper_sha256']==sha(__file__);extra+=row['checks']
        if case not in ('quit','pending_quit'):
            row=json.loads((out/case/'rtc_completed.json').read_bytes());assert row['wrapper_sha256']==sha(__file__);extra+=row['checks'];ticks[case]=row['actual_RTC_ticks']
    assert extra==38
    v.update(status='actual_PC_RTC_original_boot_logo_and_ordered_final_exit_verified',checks=v['checks']+extra,
        actual_PC_RTC_creations=6,RTC_setup_and_terminal_exit_checks=extra,actual_RTC_logical_ticks=ticks,
        RTC_closes_before_wait_and_device=True,SDK_heap_and_device_RTC_exit_mask=116726,
        baseline_SDK_heap_startup_verifier_sha256=baseline,derived_Python_caller_sha256=hashlib.sha256(source.encode()).hexdigest(),verifier_sha256=sha(__file__),
        whole_sbInitSystem_GDFS_audio_or_gameplay_verified=False)
    path.write_text(json.dumps(v,indent=2)+'\n');print(v['status'],v['checks'],'checks',flush=True)
if __name__=='__main__':main()
