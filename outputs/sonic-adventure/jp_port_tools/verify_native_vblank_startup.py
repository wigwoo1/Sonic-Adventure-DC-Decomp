"""Existing real startup/logo suite with Ninja blank and GPU EOR callbacks.
The Python caller composes the genuine Ninja callback with the existing passive
pixel observer. Native game/SDK/renderer functions remain actual providers.
"""
import ctypes as C,hashlib,json,sys
from pathlib import Path
import verify_native_boot_buffers_startup as parent
from native_packet_test_support import bind
P=C.c_void_p;I=C.c_int;U=C.c_uint32;Callback=C.CFUNCTYPE(None,P)
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    baseline=sha(parent.__file__);argv=sys.argv[1:];out=Path(argv[argv.index('--output')+1]).resolve();build=Path(argv[argv.index('--build')+1]).resolve();b=json.loads((build/'build_verification.json').read_bytes());worker='--worker' in argv
    if worker:
        frozen=parent.parent.frozen;real_present=frozen.Present;keep=[]
        def present_factory(*args,**kwargs):
            value=real_present(*args,**kwargs);lib=value.lib;native_init=value.init;native_callback=value.callback;eor=bind(lib,'njSetEORflag',None,P);ninja_count=I.in_dll(lib,'_nj_blank_count_');render=I.in_dll(lib,'_nj_render_flag_');eor_flag=I.in_dll(lib,'_nj_eor_flag_');count=bind(lib,'jp_vblank_chain_count',U);status=bind(lib,'jp_vblank_chain_status',I)
            def initialize(*values):
                code=native_init(*values)
                if not code:assert native_callback(C.cast(eor,P),None)==0
                return code
            value.init=initialize
            def observe(address,context):
                observer=Callback(address.value if isinstance(address,P) else address);frames=0;checks=0;first=None;errors=[]
                @Callback
                def completed(argument):
                    nonlocal frames,checks,first
                    try:
                        eor(argument);frames+=1
                        assert render.value==1 and eor_flag.value==0;checks+=1
                        assert status()==0;checks+=1
                        if first is None:first=ninja_count.value
                        else:
                            assert count()==1 and ninja_count.value==first+1;checks+=1
                    except BaseException as error:errors.append(str(error))
                    observer(argument)
                    (out/'gpu_ninja_eor.json').write_text(json.dumps(dict(status='real_GPU_completion_called_original_Ninja_EOR_and_retired_original_blank',actual_GPU_EOR_callbacks=frames,checks=checks,first_EOR_blank_counter=first,latest_blank_counter=ninja_count.value,active_chain_handlers=count(),errors=errors,original_Ninja_function_replaced=False,passive_original_pixel_observer_retained=True),indent=2)+'\n')
                keep.extend([completed,observer]);return native_callback(C.cast(completed,P),context)
            value.callback=observe;return value
        frozen.Present=present_factory;previous=frozen.bind
        def select(lib,name,result,*types):
            native=previous(lib,name,result,*types)
            if name!='boot_init':return native
            def boot():
                service=C.CDLL(b['present_DLL']);blank=I.in_dll(service,'_nj_blank_count_');render=I.in_dll(service,'_nj_render_flag_');eor_flag=I.in_dll(service,'_nj_eor_flag_');blank.value=render.value=0;eor_flag.value=1
                assert bind(service,'jp_vblank_arm_ninja_blank',I)()==0
                assert bind(service,'jp_wait_status',I)()==bind(service,'jp_vblank_chain_status',I)()==0
                assert blank.value==0 and bind(service,'jp_vblank_chain_count',U)()>=1
                (out/'vblank_startup_prelude.json').write_text(json.dumps(dict(checks=3,real_Ninja_blank_callback_armed=True,native_functions_replaced=False,wrapper_sha256=sha(__file__)),indent=2)+'\n');native()
            return boot
        frozen.bind=select
    parent.__file__=__file__;parent.main()
    if worker:raise AssertionError('native process exit returned')
    path=out/'verification.json';result=json.loads(path.read_bytes());assert result['checks']==5989;extra=0;eor_checks=0;frames=0
    for case in ['movie0','movie6','advertise1','cleanup','quit','pending_quit']:
        row=json.loads((out/case/'vblank_startup_prelude.json').read_bytes());assert row['checks']==3 and row['wrapper_sha256']==sha(__file__) and not row['native_functions_replaced'];extra+=row['checks']
        if case in ['movie0','movie6','advertise1']:
            row=json.loads((out/case/'gpu_ninja_eor.json').read_bytes());assert row['actual_GPU_EOR_callbacks']==190 and row['checks']==569 and not row['errors'] and row['active_chain_handlers']==1 and row['latest_blank_counter']==row['first_EOR_blank_counter']+1;eor_checks+=row['checks'];frames+=row['actual_GPU_EOR_callbacks']
    assert extra==18 and eor_checks==1707 and frames==570
    result.update(status='real_GPU_EOR_original_Ninja_blank_and_existing_complete_startup_logo_exit_verified',checks=result['checks']+extra+eor_checks,prior_boot_buffer_startup_checks=5989,native_vblank_arm_checks=extra,actual_GPU_Ninja_EOR_checks=eor_checks,actual_GPU_Ninja_EOR_callbacks=frames,real_GPU_completion_retires_original_Ninja_blank=True,original_pixel_observers_retained=True,baseline_boot_buffers_startup_verifier_sha256=baseline,verifier_sha256=sha(__file__),physical_IRQs_or_whole_system_boot_verified=False)
    path.write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True)
if __name__=='__main__':main()
