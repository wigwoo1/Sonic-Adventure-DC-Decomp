"""Run preserved ownership/GPU suites on the new real state/window providers.
Metadata aliases point at verified new binaries; original proofs stay untouched.
"""
import argparse,ctypes as C,hashlib,json,subprocess,sys
from pathlib import Path
from native_packet_test_support import bind
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','frames','window','output']:ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--suite',choices=['frames','gpu','gpu_extended'],required=True);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure');d.mkdir(parents=True,exist_ok=True)
    fb=json.loads((a.frames/'build_verification.json').read_text());wb=json.loads((a.window/'build_verification.json').read_text())
    for b in [fb,wb]:
        assert b['exit_code']==b['diagnostic_bytes']==0
        for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
        assert sha(Path(b['DLL']))==b['DLL_sha256']
    if a.suite=='frames':
        import verify_native_frames as original;b=fb;report=d/'frame_ownership.json'
    else:
        if a.suite=='gpu':import verify_native_gpu as original;report=d/'verification.json'
        else:import verify_native_gpu_extended as original;report=d/'extended_verification.json'
        b=wb
    assert not report.exists()
    manifest=d/'build_verification.json'
    if manifest.exists():assert json.loads(manifest.read_text())==b
    else:manifest.write_text(json.dumps(b,indent=2)+'\n')
    lib=C.CDLL(str(a.frames.resolve()/'jp_native_frames.dll'));init=bind(lib,'jp_video_sdk_initialize',C.c_int,C.c_uint32,C.c_uint32,C.c_uint32,C.c_int)
    assert init(640,480,0xff000000,1)==0
    sys.argv=[original.__file__,'--workspace',str(r),'--build',str(d),'--report',str(report)]
    original.main();q=json.loads(report.read_text());expected={'frames':8572,'gpu':491158,'gpu_extended':121451}[a.suite];assert q['checks']==expected
    evidence=dict(status='preserved_'+a.suite+'_suite_passed_on_stateful_providers',checks=expected,original_verifier_sha256=sha(Path(original.__file__)),
        original_report_sha256=sha(report),actual_new_build_report_sha256=sha((a.frames if a.suite=='frames' else a.window)/'build_verification.json'),
        alias_manifest_is_exact_new_build_report=True,wrapper_sha256=sha(Path(__file__)),game_source_or_original_verifier_modified=False,playable=False,overall_completion_percent=10)
    target=d/(a.suite+'_qualification.json');assert not target.exists();target.write_text(json.dumps(evidence,indent=2)+'\n')
if __name__=='__main__':main()
