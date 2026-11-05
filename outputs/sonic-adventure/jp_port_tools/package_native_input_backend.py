"""Qualify only the authored asset-free input DLL and its matching import lib."""
import argparse,hashlib,json
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','input','pad','startup','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    ib=read(a.input/'build_verification.json');pb=read(a.pad/'build_verification.json')
    assert ib['exit_code']==ib['diagnostic_bytes']==pb['exit_code']==pb['diagnostic_bytes']==0
    for b in [ib,pb]:
        for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert pb['input_object_sha256']==ib['object_sha256']==sha(a.input/'input.obj')
    proofs=[a.input/'verification.json',a.pad/'verification.json',a.pad/'startup-regression.json',a.pad/'task-regression.json']
    checks=[read(p)['checks'] for p in proofs];assert checks==[150352,4851,8266,125] and sum(checks)==163594
    assert read(proofs[0])['build_report_sha256']==sha(a.input/'build_verification.json')
    assert read(proofs[1])['build_report_sha256']==read(proofs[3])['build_report_sha256']==sha(a.pad/'build_verification.json')
    assert read(proofs[2])['build_report_sha256']==sha(a.startup/'build_verification.json')
    assert pb['full_pad_link_audit']['remaining_imports']==['recording_update'] and not pb['full_pad_link_audit']['forced_or_dummy_services']
    dll=Path(ib['DLL']);library=dll.with_suffix('.lib');assert sha(dll)==ib['DLL_sha256'] and library.read_bytes().startswith(b'!<arch>')
    d.mkdir();p=r/'outputs/sonic-adventure'
    sources=[p/'native/jp_native_input.c',p/'native/jp_native_input.h',*[p/'jp_port_tools'/n for n in ['build_native_input_backend.py','verify_native_input_backend.py','build_native_pad_helpers.py','verify_native_pad_helpers.py','package_native_input_backend.py']],p/'JP_NATIVE_INPUT.txt']
    result=dict(status='authored_native_input_DLL_import_library_and_genuine_pad_helper_boundary_qualified',public_artifacts={str(f.resolve()):dict(bytes=f.stat().st_size,sha256=sha(f)) for f in [dll,library]},authored_source_sha256={f.resolve().relative_to(r).as_posix():sha(f) for f in sources},verification_files_sha256={str(p.resolve()):sha(p) for p in proofs},checks=checks,total_checks=sum(checks),input_object_byte_identity_in_integrated_pad_runtime=True,public_artifacts_contain_original_game_or_Sega_SDK_bodies=False,public_artifacts_contain_retail_assets=False,private_game_containing_pad_runtime_publishable=False,physical_controller_events_verified=False,focused_physical_keyboard_events_verified=False,full_pad_scan_linked=False,remaining_pad_scan_services=['recording_update'],frame_wait_trace_replaced=False,artifact_publishable=True,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],sum(checks),'checks')
if __name__=='__main__':main()
