"""Frozen complete queue/frame lifetime suite with required real video setup."""
import hashlib,json,sys
from pathlib import Path
import verify_native_frames as frozen
from native_video_test_support import Video
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    original=frozen.Runtime;calls=0
    def runtime(*args,**kwargs):
        nonlocal calls
        value=original(*args,**kwargs);assert Video(value.dll).init(64,64,0xff000000,1)==0;calls+=1;return value
    frozen.Runtime=runtime;frozen.main();assert calls==1
    argv=sys.argv[1:];path=Path(argv[argv.index('--report')+1]);result=json.loads(path.read_bytes());result.update(checks=result['checks']+calls,required_real_video_initialization_checks=calls,frozen_frames_verifier_sha256=sha(frozen.__file__),verifier_sha256=sha(__file__),native_functions_replaced=False)
    path.write_text(json.dumps(result,indent=2)+'\n');print('complete_frames_with_real_video',result['checks'],'checks',flush=True)
if __name__=='__main__':main()
