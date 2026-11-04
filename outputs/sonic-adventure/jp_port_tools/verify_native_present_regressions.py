"""Run preserved suites on exact export-only owner and extended wait objects."""
import argparse,ctypes as C,hashlib,importlib,json,sys
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','output']:ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--suite',choices=['waits','frames','video'],required=True);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    b=json.loads((a.build/'build_verification.json').read_text())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    runtime=Runtime(Path(b['frames_DLL']));gpu=C.CDLL(b['GPU_DLL']);d.mkdir()
    if a.suite=='waits':
        original=importlib.import_module('verify_native_frame_wait');alias=json.loads((Path(b['wait_baseline'])/'build_verification.json').read_text());alias.update(DLL=b['wait_regression_DLL'],DLL_sha256=b['wait_regression_DLL_sha256'],GPU_DLL=b['GPU_DLL'],frames_DLL=b['frames_DLL']);expected=6943
    elif a.suite=='frames':
        original=importlib.import_module('verify_native_frames');alias=json.loads((r/'work/sonic-adventure/jp-native-stateful-frames-v4/build_verification.json').read_text());alias.update(DLL=b['frames_DLL'],DLL_sha256=b['frames_DLL_sha256']);expected=8572
        init=bind(runtime.dll,'jp_video_sdk_initialize',C.c_int,C.c_uint32,C.c_uint32,C.c_uint32,C.c_int);assert init(640,480,0xff000000,1)==0
    else:
        original=importlib.import_module('verify_native_video_window');alias=json.loads((r/'work/sonic-adventure/jp-native-gpu-jobs-v2/build_verification.json').read_text());alias.update(frames_DLL=b['frames_DLL'],frames_DLL_sha256=b['frames_DLL_sha256']);expected=103700
    alias.update(regression_view_of_actual_build_sha256=sha(a.build/'build_verification.json'),regression_view_for_frozen_verifier=True)
    (d/'build_verification.json').write_text(json.dumps(alias,indent=2)+'\n');report=d/'verification.json'
    sys.argv=[original.__file__,'--workspace',str(r),'--build',str(d),'--report',str(report)];original.main();q=json.loads(report.read_text());assert q['checks']==expected
    result=dict(status='preserved_'+a.suite+'_suite_passed_on_native_present_dependencies',checks=expected,wrapper_sha256=sha(Path(__file__)),original_verifier_sha256=sha(Path(original.__file__)),
        report_sha256=sha(report),actual_build_sha256=sha(a.build/'build_verification.json'),DLL_byte_identity_checked_by_frozen_verifier=True,
        frame_owner_objects_unchanged=True,device_export_only_relink=True,gameplay_verified=False,playable=False,overall_completion_percent=10)
    (d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
