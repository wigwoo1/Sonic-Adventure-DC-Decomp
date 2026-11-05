"""Frozen rendered startup/task suites with a real neutral input provider.
The fixture supplies controlled samples through the actual shared provider;
it does not replace SDK functions or original game/frame behavior.
"""
import argparse,ctypes as C,hashlib,importlib,json,sys
from pathlib import Path
from native_packet_test_support import bind
from verify_native_input_backend import Source,Sample,Info
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','startup','inputs','asset-root','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    ap.add_argument('--suite',choices=['startup','task'],required=True);a=ap.parse_args()
    r=a.workspace.resolve();d=a.build.resolve();out=a.output.resolve();assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();out.mkdir()
    b=json.loads((d/'build_verification.json').read_bytes());assert b['frame_wait_input_trace_replaced'] and sha(b['frames_DLL'])==b['frames_DLL_sha256']
    lib=C.CDLL(b['frames_DLL']);init=bind(lib,'jp_input_initialize',C.c_int,C.c_void_p,C.c_uint32,C.c_void_p,C.c_void_p)
    @Source
    def neutral(context,samples):C.memset(samples,0,C.sizeof(Sample)*4);return 1
    assert init(None,0,C.cast(neutral,C.c_void_p),None)==0
    original=importlib.import_module('verify_native_logo_start' if a.suite=='startup' else 'verify_native_task_presentation')
    report=out/'original_suite.json';args=[original.__file__,'--workspace',str(r),'--build',str(a.startup.resolve() if a.suite=='startup' else d),'--inputs',str(a.inputs.resolve()),'--asset-root',str(a.asset_root.resolve()),'--report',str(report)]
    if a.suite=='task':args+=['--startup',str(a.startup.resolve())]
    sys.argv=args;original.main();q=json.loads(report.read_bytes())
    snapshot=Info();assert bind(lib,'jp_input_snapshot',C.c_int,C.c_void_p)(C.byref(snapshot))==0
    expected=8266 if a.suite=='startup' else 125;assert q['checks']==expected and snapshot.polls>0 and snapshot.status==0
    assert bind(lib,'jp_input_shutdown',C.c_int)()==0
    result=dict(status='rendered_'+a.suite+'_regression_passed_with_real_shared_frame_input',checks=expected,actual_frame_input_polls=snapshot.polls,input_status_after_rendering=snapshot.status,actual_GPU_images=q['actual_GPU_images'],pixels_compared=q['pixels_compared'],actual_Presents=q['actual_Presents'],actual_build_sha256=sha(d/'build_verification.json'),startup_build_sha256=sha(a.startup/'build_verification.json'),original_suite_report_sha256=sha(report),original_verifier_sha256=sha(Path(original.__file__)),wrapper_sha256=sha(Path(__file__)),input_fixture_supplies_samples_only=True,original_SDK_or_game_functions_replaced=False,frozen_reports_input_trace_descriptions_superseded=True,frame_wait_input_trace_replaced=True,remaining_audio_loader_overlay_traces=True,physical_input_events_verified=False,original_logo_step_executed=False,playable=False,overall_completion_percent=10)
    (out/'qualification.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result),flush=True)
if __name__=='__main__':main()
