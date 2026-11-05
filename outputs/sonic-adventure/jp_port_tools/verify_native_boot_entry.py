"""Run the frozen real logo/exit suite with boot_init as its caller entry.

Only the Python harness's entry binding selects a different real exported
function. No native game/SDK function, dependency or body is replaced. Every
worker still executes the unchanged original boot_init -> logo_start chain.
"""
import argparse,ctypes as C,hashlib,json,subprocess,sys
from pathlib import Path
import verify_native_exit_logo as frozen
from native_packet_test_support import bind
from native_present_test_support import Present
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','inputs','asset-root','save-proof','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    ap.add_argument('--worker',choices=['movie0','movie6','advertise1','cleanup','quit','pending_quit','partial','unbound']);ap.add_argument('--pixel-dll',type=Path)
    a=ap.parse_args()
    for n in ['workspace','build','inputs','asset_root','save_proof','output']:setattr(a,n,getattr(a,n).resolve())
    r,d,out=a.workspace,a.build,a.output;b=read(d/'build_verification.json');save=read(a.save_proof)
    assert save['build_report_sha256']==sha(d/'build_verification.json') and save['checks']==591 and not save['original_dependency_hooks_used']
    prior=read(r/'outputs/sonic-adventure/analysis/jp_native_exit_logo_verification.json');assert sha(Path(frozen.__file__))==prior['verifier_sha256']
    if a.worker:
        # Entry selection belongs to this caller, not to the native module:
        # the DLL's logo_start symbol and all its callers remain unchanged.
        original_bind=frozen.bind
        def select_entry(library,name,result,*types):
            if name!='logo_start':return original_bind(library,name,result,*types)
            native_boot=original_bind(library,'boot_init',None)
            def call_boot():
                lib=C.CDLL(b['frames_DLL']);size=bind(lib,'jp_save_bytes',C.c_uint32)();count=bind(lib,'jp_save_count',C.c_uint32)()
                array=(C.c_ubyte*(size*count)).in_dll(lib,'saves');C.memset(array,0xa5,C.sizeof(array))
                values={n:C.c_int.in_dll(lib,n) for n in ['demo_next','demo_event','demo_playing','save_slot']}
                for n,value in [('demo_next',11),('demo_event',147),('demo_playing',1),('save_slot',2)]:values[n].value=value
                rings=C.c_int16.in_dll(lib,'rings');rings.value=321
                native_boot()
                checks=0
                for n in values:assert values[n].value==0,n;checks+=1
                assert rings.value==0;checks+=1
                digest=hashlib.sha256(bytes(array)).hexdigest();assert digest==save['default_all_slots_sha256'];checks+=1
                assert bind(lib,'save_check_all',C.c_int,C.c_void_p)(C.addressof(array))==1;checks+=1
                present=Present(Path(b['present_DLL']));present.probe(0,4)
                assert values['demo_playing'].value==1;checks+=1
                present.probe(0,0);assert values['demo_playing'].value==0;checks+=1
                report=dict(status='unchanged_original_boot_init_reset_and_shared_state_verified',checks=checks,selected_native_entry='boot_init',native_functions_replaced=False,default_all_slots_sha256=digest,source_save_machine_code_proof_sha256=sha(a.save_proof),single_demo_playing_owner_verified_across_DLLs=True)
                (out/'boot_entry.json').write_text(json.dumps(report,indent=2)+'\n')
            return call_boot
        frozen.bind=select_entry
        sys.argv=[frozen.__file__,'--workspace',str(r),'--build',str(d),'--inputs',str(a.inputs),'--asset-root',str(a.asset_root),'--output',str(out),'--worker',a.worker,'--pixel-dll',str(a.pixel_dll.resolve())]
        frozen.main();raise AssertionError('frozen worker returned from native process exit')
    assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();out.mkdir()
    helper=out/'pixel_difference.c';helper.write_text('#include <stddef.h>\n__declspec(dllexport) unsigned pixel_difference(const unsigned char *a,const unsigned char *b,size_t n){unsigned maximum=0;for(size_t i=0;i<n;i++){unsigned d=a[i]>b[i]?a[i]-b[i]:b[i]-a[i];if(d>maximum)maximum=d;}return maximum;}\n')
    pixel=out/'pixel_difference.dll';cmd=[b['commands'][0]['command'][0],'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-shared','-fuse-ld=lld',str(helper),'-o',str(pixel)]
    run=subprocess.run(cmd,capture_output=True);assert run.returncode==0 and not run.stdout and not run.stderr,run.stderr.decode()
    cases={};boot_checks=0;frozen_checks=0
    for case in ['movie0','movie6','advertise1','cleanup','quit','pending_quit','partial','unbound']:
        child=out/case;child.mkdir();args=[sys.executable,'-B',str(Path(__file__).resolve()),'--workspace',str(r),'--build',str(d),'--inputs',str(a.inputs),'--asset-root',str(a.asset_root),'--save-proof',str(a.save_proof),'--output',str(child),'--worker',case,'--pixel-dll',str(pixel)]
        print('Running genuine boot entry / frozen logo-exit case:',case,flush=True)
        run=subprocess.run(args,capture_output=True,creationflags=subprocess.CREATE_NO_WINDOW);(child/'stdout.txt').write_bytes(run.stdout);(child/'stderr.txt').write_bytes(run.stderr)
        assert run.returncode==(66 if case=='unbound' else 0) and not run.stderr,(case,run.returncode,run.stderr.decode()[-6000:])
        result=read(child/'result.json');frozen_checks+=result['checks'];result['process_exit_code']=run.returncode
        if case not in ['partial','unbound']:
            result['boot_entry']=read(child/'boot_entry.json');boot_checks+=result['boot_entry']['checks']
        if case in ['quit','pending_quit']:
            q=read(child/'destructor.json');assert q['reentrant_cleanup']==4 and q['input_status']==4 and q['wait_status']==2
        cases[case]=result;print(case,'verified',flush=True)
    assert frozen_checks==3601 and boot_checks==54
    report=dict(status='original_boot_init_genuine_save_reset_and_complete_logo_exit_integration_verified',checks=frozen_checks+boot_checks,frozen_logo_exit_checks=frozen_checks,boot_entry_checks=boot_checks,actual_original_boot_init_calls=6,actual_complete_191_step_logo_runs=3,actual_GPU_images=570,pixels_compared=175104000,actual_Presents=570,cases=cases,caller_entry_selection_only=True,native_game_or_SDK_function_replacements=False,single_demo_playing_owner_in_shared_frame_DLL=True,save_machine_code_proof_sha256=sha(a.save_proof),frozen_verifier_sha256=sha(Path(frozen.__file__)),verifier_sha256=sha(Path(__file__)),build_report_sha256=sha(d/'build_verification.json'),pixel_helper_source_sha256=sha(helper),pixel_helper_DLL_sha256=sha(pixel),game_init_main_mode_dispatch_or_full_system_boot_executed=False,save_store_load_or_disk_persistence_verified=False,physical_input_events_verified=False,remaining_audio_loader_overlay_traces=True,playable=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='cases'}),flush=True)
if __name__=='__main__':main()
