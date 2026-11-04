"""Run the frozen clock oracle against the exact new diagnostic DLL bytes."""
import argparse,ctypes as C,hashlib,json,shutil,sys
from pathlib import Path
from native_texture_test_support import Runtime
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    b=json.loads((a.build/'build_verification.json').read_text());assert sha(Path(b['DLL']))==b['DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    runtime=Runtime(Path(b['frames_DLL']));gpu=C.CDLL(b['GPU_DLL']);d.mkdir()
    shutil.copyfile(b['DLL'],d/'jp_native_clock.dll');assert sha(d/'jp_native_clock.dll')==b['DLL_sha256']
    alias=dict(b,dll_sha256=b['DLL_sha256']);(d/'build_verification.json').write_text(json.dumps(alias,indent=2)+'\n')
    import verify_native_clock as original
    report=d/'verification.json';sys.argv=[original.__file__,'--workspace',str(r),'--build',str(d),'--report',str(report)];original.main()
    q=json.loads(report.read_text());assert q['checks']==12873
    result=dict(status='preserved_clock_suite_passed_on_exact_frame_wait_DLL',checks=q['checks'],exact_DLL_bytes_verified=True,renamed_copy_is_diagnostic_only=True,
        new_build_report_sha256=sha(a.build/'build_verification.json'),original_verifier_sha256=sha(Path(original.__file__)),original_report_sha256=sha(report),wrapper_sha256=sha(Path(__file__)),playable=False,overall_completion_percent=10)
    (d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
