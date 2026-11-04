"""Run frozen ownership/window/presenter suites on the new exact DLL bytes."""
import argparse,ctypes as C,hashlib,importlib,json,subprocess,sys
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','output']:ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--suite',choices=['textures','frames','video','present'],required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists()
    b=json.loads((a.build/'build_verification.json').read_text())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ['DLL','frames_DLL','GPU_DLL']:assert sha(Path(b[n]))==b[n+'_sha256']
    d.mkdir();runtime=None
    if a.suite=='textures':
        # The frozen standalone texture suite needs its old selection observer,
        # which the full frame DLL doesn't export. Relink IDENTICAL objects,
        # changing only exports and output name; no test implementations added.
        command=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b['frames_DLL']))
        for obj in command:
            if obj.endswith('.obj'):
                expected_hash=b['production_objects_sha256'].get(Path(obj).name,b['kept_frame_owner_objects_sha256'].get(str(Path(obj).resolve())))
                assert expected_hash==sha(Path(obj)),obj
        command.insert(1,'-Wl,/export:jp_texture_renderer_select');command[-1]=str(d/'jp_native_textures.dll')
        run=subprocess.run(command,capture_output=True);assert run.returncode==0 and not run.stdout and not run.stderr,run.stderr.decode()[:3000]
        (d/'export_only_relink.json').write_text(json.dumps(dict(command=command,exit_code=0,identical_owner_objects=True,additional_export_only='jp_texture_renderer_select'),indent=2)+'\n')
        original=importlib.import_module('verify_native_texture_runtime');alias=dict(b,status='native_texture_runtime_linked_behavior_unverified',dll_sha256=sha(d/'jp_native_textures.dll'))
        expected=None
    else:
        runtime=Runtime(Path(b['frames_DLL']))
        if a.suite=='frames':
            original=importlib.import_module('verify_native_frames');alias=json.loads((r/'work/sonic-adventure/jp-native-stateful-frames-v4/build_verification.json').read_text());alias.update(DLL=b['frames_DLL'],DLL_sha256=b['frames_DLL_sha256'],authored_source_sha256=b['authored_source_sha256']);expected=8572
            assert bind(runtime.dll,'jp_video_sdk_initialize',C.c_int,C.c_uint32,C.c_uint32,C.c_uint32,C.c_int)(640,480,0xff000000,1)==0
        elif a.suite=='video':
            original=importlib.import_module('verify_native_video_window');alias=json.loads((r/'work/sonic-adventure/jp-native-gpu-jobs-v2/build_verification.json').read_text());alias.update(frames_DLL=b['frames_DLL'],frames_DLL_sha256=b['frames_DLL_sha256']);expected=103700
        else:
            original=importlib.import_module('verify_native_present');alias=dict(b);expected=None
    alias.update(regression_view_of_actual_build_sha256=sha(a.build/'build_verification.json'),regression_view_for_frozen_verifier=True)
    (d/'build_verification.json').write_text(json.dumps(alias,indent=2)+'\n');report=d/'verification.json'
    sys.argv=[original.__file__,'--workspace',str(r),'--build',str(d),'--report',str(report)];original.main();q=json.loads(report.read_text())
    if expected is not None:assert q['checks']==expected
    if a.suite=='present':assert q['visible_Kamui_Presents']==8 and q['actual_GPU_images_compared']==4
    result=dict(status='preserved_'+a.suite+'_suite_passed_on_bitmap_upload_dependencies',checks=q['checks'],wrapper_sha256=sha(Path(__file__)),original_verifier_sha256=sha(Path(original.__file__)),
        report_sha256=sha(report),actual_build_sha256=sha(a.build/'build_verification.json'),DLL_byte_identity_checked_by_frozen_verifier=True,
        retained_texture_owner_extended_without_replacing_original_record_contracts=True,gameplay_verified=False,playable=False,overall_completion_percent=10)
    result.update(texture_suite_relink_exports_only=a.suite=='textures',other_suites_use_exact_runtime_DLL_bytes=a.suite!='textures',
        frozen_present_report_bitmap_trace_description_superseded=a.suite=='present',bitmap_trace_providers_removed=True)
    (d/'qualification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
