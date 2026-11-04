"""Apply frozen compatibility suites to the exact new PVM owner DLL.
PVM-specific behavior is separately qualified by verify_native_pvm_upload.py.
"""
import argparse,hashlib,importlib,json,sys
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','output']:ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--suite',choices=['textures','frames','video','present','bitmap'],required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    b=json.loads((a.build/'build_verification.json').read_text())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    if a.suite=='bitmap':
        d.mkdir();(d/'build_verification.json').write_text(json.dumps(b,indent=2)+'\n');original=importlib.import_module('verify_native_bitmap_upload');report=d/'verification.json'
        sys.argv=[original.__file__,'--workspace',str(r),'--build',str(d),'--report',str(report)];original.main();count=json.loads(report.read_text())['checks'];assert count==722
    else:
        original=importlib.import_module('verify_native_bitmap_regressions')
        sys.argv=[original.__file__,'--workspace',str(r),'--build',str(a.build),'--suite',a.suite,'--output',str(d)];original.main()
        report=d/'verification.json';count=json.loads(report.read_text())['checks']
    result=dict(status='preserved_'+a.suite+'_suite_passed_on_PVM_native_dependencies',checks=count,actual_build_sha256=sha(a.build/'build_verification.json'),report_sha256=sha(report),
        wrapper_sha256=sha(Path(__file__)),original_verifier_sha256=sha(Path(original.__file__)),
        new_PVM_specific_behavior_qualified_separately=True,frame_owner_DLL=str(b['frames_DLL']),frame_owner_DLL_sha256=b['frames_DLL_sha256'],
        texture_suite_uses_identical_objects_with_observer_export_only=a.suite=='textures',other_suites_use_exact_runtime_DLL_bytes=a.suite!='textures',
        full_game_loop_executed=False,playable=False,overall_completion_percent=10)
    (d/'pvm_qualification.json').write_text(json.dumps(result,indent=2)+'\n');print('PVM prerequisite compatibility:',a.suite,count,'checks')
if __name__=='__main__':main()
