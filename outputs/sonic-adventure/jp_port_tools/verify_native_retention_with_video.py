"""Initialize the actual video owner, then execute the unchanged frame suite."""
import argparse,ctypes as C,hashlib,json,sys
from pathlib import Path
from native_video_test_support import Video
import verify_native_frames as frozen
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();b=json.loads((a.build/'build_verification.json').read_bytes());assert b['DLL']==b['frames_DLL'] and sha(b['DLL'])==b['DLL_sha256']==b['frames_DLL_sha256'];assert not a.report.exists()
    lib=C.CDLL(b['DLL']);assert Video(lib).init(640,480,0xff000000,1)==0
    sys.argv=[frozen.__file__,'--workspace',str(a.workspace),'--build',str(a.build),'--report',str(a.report)];frozen.main()
    result=json.loads(a.report.read_bytes());assert result['checks']==8572 and result['verifier_sha256']==sha(frozen.__file__)
    proof=a.report.with_name(a.report.stem+'_video_binding.json');assert not proof.exists();proof.write_text(json.dumps(dict(status='actual_video_owner_initialized_for_frozen_frame_suite',frozen_report_sha256=sha(a.report),frozen_verifier_sha256=sha(frozen.__file__),wrapper_sha256=sha(__file__),build_report_sha256=sha(a.build/'build_verification.json'),native_function_bodies_replaced=False,full_gameplay_verified=False),indent=2)+'\n')
if __name__=='__main__':main()
