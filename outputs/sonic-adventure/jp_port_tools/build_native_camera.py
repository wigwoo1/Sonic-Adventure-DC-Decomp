"""Add authored camera setup/translation and exact SDK-value math to real state."""
import argparse,hashlib,json,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();b=a.baseline.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    pin_path=p/'jp_port_tools/jp_camera_external_inputs.json';pin=json.loads(pin_path.read_text())
    for n,h in pin['files_sha256'].items():assert sha(a.upstream/n)==h,n
    for n,h in pin['reused_authored_sources_sha256'].items():assert sha(r/n)==h,n
    old=json.loads((b/'build_verification.json').read_text());assert old['exit_code']==old['diagnostic_bytes']==0
    assert old['Ninja_matrix_numerical_mode']=='toward_zero_DN1_exceptions_disabled' and sha(a.compiler)==old['compiler_sha256']
    for n,h in old['authored_source_sha256'].items():assert sha(r/n)==h,n
    objects={Path(n):h for n,h in old['existing_provider_objects_sha256'].items()}
    objects.update({b/n:h for n,h in old['production_objects_sha256'].items()});assert len(objects)==13
    probes={Path(n):h for n,h in old['diagnostic_probes_sha256'].items()}
    for o,h in objects.items():assert sha(o)==h
    for o,h in probes.items():assert sha(o)==h
    d.mkdir(parents=True)
    sources={'sqrt_core':p/'source/sh4_sqrt.c','camera_math':p/'native/jp_ninja_camera_math_rz.c','set_view':p/'native/jp_ninja_set_view_rz.c'}
    flags=['--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror']
    commands=[[str(a.compiler),*flags,'-c',str(source),'-o',str(d/(n+'.obj'))] for n,source in sources.items()]
    dll=d/'jp_ninja_camera.dll'
    commands.append([str(a.compiler),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld','-Wl,/export:shz_xmtrx_state_',
                     *map(str,objects),*[str(d/(n+'.obj')) for n in sources],*map(str,probes),'-o',str(dll)])
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        if run.returncode:print(log.read_text()[:5000]);break
    passed=len(results)==len(commands) and all(v['exit_code']==v['diagnostic_bytes']==0 for v in results)
    hashes=dict(old['authored_source_sha256'])
    for path in [*sources.values(),p/'native/jp_ninja_camera_math.h',p/'native/jp_ninja_camera.h',pin_path,Path(__file__)]:hashes[path.resolve().relative_to(r).as_posix()]=sha(path)
    report=dict(status='actual_native_camera_setup_and_translation_runtime_linked' if passed else 'build_failed',commands=results,
        exit_code=0 if passed else 1,diagnostic_bytes=sum(v['diagnostic_bytes'] for v in results),compiler_sha256=sha(a.compiler),
        baseline_report_sha256=sha(b/'build_verification.json'),external_input_manifest_sha256=sha(pin_path),source_reference_sha256=pin['files_sha256'],
        authored_source_sha256=hashes,existing_provider_objects_sha256={str(o):h for o,h in objects.items()},diagnostic_probes_sha256={str(o):h for o,h in probes.items()},
        production_objects_sha256={n+'.obj':sha(d/(n+'.obj')) for n in sources} if passed else {},expected_resolved_services=['njSetView'],
        complete_new_SDK_entry_points=['njSetView','njTranslate'],DLL=str(dll),DLL_sha256=sha(dll) if passed else None,
        inverse_scalar_mode=old['inverse_scalar_mode'],Ninja_matrix_numerical_mode=old['Ninja_matrix_numerical_mode'],
        SDK_errno_native_adapter='jp_sdk_math_errno',SDK_math_error_values=dict(EDOM=1101,ERANGE=1100),
        single_active_matrix_owner=True,single_actual_current_view_pointer=True,renderer_SH4ZAM_arithmetic_migrated=False,
        game_source_linked=False,retail_assets_embedded=False,authored_asset_free_artifact_publishable=True,
        behavior_verified=False,overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],'diagnostic bytes',report['diagnostic_bytes']);raise SystemExit(0 if passed else 1)

if __name__=='__main__':main()
