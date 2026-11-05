"""Actual SDK heap before Ninja, original boot/logo, consumer before final exit.
Only the established Python caller/prelude and its test-task callback change.
"""
import ctypes as C,hashlib,inspect,json,sys
from pathlib import Path
import verify_native_device_exit_startup as runner
from verify_native_sdk_heap import Info as SDKHeapInfo
from native_task_test_support_v2 import Task as SDKTask
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    argv=sys.argv[1:];worker='--worker' in argv;out=Path(argv[argv.index('--output')+1]).resolve()
    baseline=sha(runner.__file__);source=inspect.getsource(runner.main)
    def patch(old,new,count=1):
        nonlocal source
        assert source.count(old)==count,(old,source.count(old));source=source.replace(old,new)
    patch("    original_getsource=inspect.getsource;original_leaf=leaf.main;real_present=leaf.Present;derived_hashes=[]", "    original_getsource=inspect.getsource;original_leaf=leaf.main;real_present=leaf.Present;derived_hashes=[]\n    heap_owner=None;heap_fixture=None")
    patch("        dl=C.CDLL(b['device_DLL']);options=Options(640,480,0,0,5000,0,64<<20)", """        nonlocal heap_owner,heap_fixture
        heap_owner=C.CDLL(b['SDK_heap_DLL']);heap_fixture=C.CDLL(b['fixture_DLL'])
        assert bind(heap_owner,'jp_sdk_heap_create',I,U)((1<<20)+32)==0
        hi=SDKHeapInfo();assert bind(heap_owner,'jp_sdk_heap_snapshot',I,P)(C.byref(hi))==0 and hi.owned and hi.usable==1<<20 and hi.live==0
        assert bind(heap_fixture,'jp_sdk_fixture_prepare',I,U)(256)==0
        assert bind(heap_owner,'jp_sdk_heap_snapshot',I,P)(C.byref(hi))==0 and hi.live==1 and hi.requested==hi.capacity==256
        (out/'sdk_heap_setup.json').write_text(json.dumps(dict(status='actual_bounded_SDK_heap_and_native_C_consumer_before_Ninja',checks=4,usable=hi.usable,live=hi.live,requested=hi.requested,wrapper_sha256=sha(__file__)),indent=2)+'\\n')
        dl=C.CDLL(b['device_DLL']);options=Options(640,480,0,0,5000,0,64<<20)""")
    patch("        assert bind(exit_library,'jp_exit_status',I)()==0", """        assert bind(exit_library,'jp_exit_status',I)()==0
        assert bind(heap_owner,'jp_sdk_heap_shutdown',I)()==3
        hi=SDKHeapInfo();assert bind(heap_owner,'jp_sdk_heap_snapshot',I,P)(C.byref(hi))==0 and hi.initialized and hi.live==1
        (out/'sdk_heap_exit_bound.json').write_text(json.dumps(dict(status='SDK_heap_has_one_terminal_exit_owner',checks=2,live=hi.live,wrapper_sha256=sha(__file__)),indent=2)+'\\n')""")
    patch("    def present_factory(*args,**kwargs):", """    def attach_heap_destructor(task,observer):
        @Callback
        def observed(address):
            errors=[]
            try:
                values=(U*4)();bind(heap_fixture,'jp_sdk_fixture_snapshot',None,P)(values)
                assert tuple(values)==(1,0,0,256)
                hi=SDKHeapInfo();assert bind(heap_owner,'jp_sdk_heap_snapshot',I,P)(C.byref(hi))==0 and hi.initialized and hi.live==hi.requested==hi.capacity==0
                total=U();largest=U();assert bind(heap_owner,'jp_sdk_heap_statistics',I,P,P)(C.byref(total),C.byref(largest))==0 and total.value==largest.value==(1<<20)-32
            except BaseException as error:errors.append(str(error))
            observer(address)
            (out/'sdk_heap_task_destructor.json').write_text(json.dumps(dict(status='original_task_destruction_called_C_SDK_consumer_before_arena_close',checks=3,errors=errors,wrapper_sha256=sha(__file__)),indent=2)+'\\n')
        keep.extend([observed,observer])
        bind(heap_fixture,'jp_sdk_fixture_observe',None,P)(C.cast(observed,P))
        SDKTask.from_address(task).destructor=C.cast(bind(heap_fixture,'jp_sdk_fixture_destructor',None,P),P).value
    def present_factory(*args,**kwargs):""")
    patch("        assert bind(owner,'jp_device_shutdown',I)()==2\n        (out/'native_device_exit_completed.json')", """        assert bind(owner,'jp_device_shutdown',I)()==2
        hi=SDKHeapInfo();assert bind(heap_owner,'jp_sdk_heap_snapshot',I,P)(C.byref(hi))==0 and hi.initialized==hi.live==0
        assert bind(heap_owner,'jp_sdk_heap_shutdown',I)()==3
        values=(U*4)();bind(heap_fixture,'jp_sdk_fixture_snapshot',None,P)(values);assert tuple(values)==(1,0,0,256)
        (out/'sdk_heap_completed.json').write_text(json.dumps(dict(status='SDK_heap_closed_only_after_original_task_C_consumer',checks=3,initialized=hi.initialized,wrapper_sha256=sha(__file__)),indent=2)+'\\n')
        (out/'native_device_exit_completed.json')""")
    patch("        derived_hashes.append(hashlib.sha256(source.encode()).hexdigest());return source", """        patch('Task.from_address(task).destructor=C.cast(destroy,P).value','native_sdk_task_destructor(task,destroy)')
        derived_hashes.append(hashlib.sha256(source.encode()).hexdigest());return source""")
    patch("    leaf.native_device_prepare=prepare;", "    leaf.native_sdk_task_destructor=attach_heap_destructor\n    leaf.native_device_prepare=prepare;")
    patch('18422','51190',3)
    runner.SDKHeapInfo=SDKHeapInfo;runner.SDKTask=SDKTask;runner.__file__=__file__
    exec(compile(source,__file__,'exec'),runner.__dict__);runner.main()
    if worker:raise AssertionError('native process exit returned')
    path=out/'verification.json';v=json.loads(path.read_bytes());assert v['checks']==7215 and v['actual_Presents']==570;extra=0
    for case in ('movie0','movie6','advertise1','cleanup','quit','pending_quit'):
        for name in ('sdk_heap_setup','sdk_heap_exit_bound','sdk_heap_task_destructor'):
            row=json.loads((out/case/(name+'.json')).read_bytes());assert row['wrapper_sha256']==sha(__file__) and not row.get('errors');extra+=row['checks']
        if case not in ('quit','pending_quit'):
            row=json.loads((out/case/'sdk_heap_completed.json').read_bytes());assert row['wrapper_sha256']==sha(__file__) and row['initialized']==0;extra+=row['checks']
    assert extra==66
    v.update(status='actual_SDK_heap_before_Ninja_original_boot_logo_task_C_consumer_and_final_exit_verified',checks=v['checks']+extra,
        SDK_heap_setup_consumer_and_final_close_checks=extra,actual_full_one_MiB_SDK_heap_creations=6,
        actual_original_task_C_destructor_calls_freeing_SDK_memory=6,SDK_heap_closes_after_consumers=True,
        SDK_heap_and_device_exit_mask=51190,native_CRT_allocators_replaced=False,
        original_game_or_SDK_function_bodies_replaced=False,baseline_device_exit_verifier_sha256=baseline,
        derived_Python_caller_sha256=hashlib.sha256(source.encode()).hexdigest(),verifier_sha256=sha(__file__),
        whole_sbInitSystem_GDFS_audio_or_gameplay_verified=False)
    path.write_text(json.dumps(v,indent=2)+'\n');print(v['status'],v['checks'],'checks',flush=True)
if __name__=='__main__':main()
