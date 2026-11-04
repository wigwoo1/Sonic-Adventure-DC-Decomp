"""Build the authored Ninja matrix family with one RZ/DN1 state provider.
Renderer SH4ZAM arithmetic is not migrated by this component. Diagnostic
SH4ZAM units test shared storage only and stay outside production inputs.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();b=a.baseline.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    old=json.loads((b/'build_verification.json').read_text())
    assert old['exit_code']==old['diagnostic_bytes']==0 and old['inverse_scalar_mode']=='toward_zero_DN1_exceptions_disabled'
    assert sha(a.compiler)==old['compiler_sha256']
    for n,h in old['authored_source_sha256'].items():assert sha(r/n)==h,n
    objects={Path(n):h for n,h in old['existing_provider_objects_sha256'].items() if Path(n).name!='math.obj'}
    assert len(objects)==6 and sum(o.name=='matrix_state.obj' for o in objects)==1
    for n,h in old['production_objects_sha256'].items():objects[b/n]=h
    assert len(objects)==8
    for o,h in objects.items():assert sha(o)==h,o
    probes={Path(n):h for n,h in old['diagnostic_probes_sha256'].items()}
    probes[b/'inverse_probe.obj']=sha(b/'inverse_probe.obj')
    assert len(probes)==3
    for o,h in probes.items():assert sha(o)==h
    d.mkdir(parents=True)
    flags=['--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror']
    sources={'math_rz':p/'native/jp_ninja_math_shared_rz.c','multi_matrix_rz':p/'native/jp_ninja_multi_matrix_rz.c'}
    commands=[[str(a.compiler),*flags,'-c',str(source),'-o',str(d/(name+'.obj'))] for name,source in sources.items()]
    dll=d/'jp_ninja_matrix_rz.dll'
    commands.append([str(a.compiler),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld','-Wl,/export:shz_xmtrx_state_',
                     *map(str,objects),*[str(d/(n+'.obj')) for n in sources],*map(str,probes),'-o',str(dll)])
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        if run.returncode:print(log.read_text()[:5000]);break
    passed=len(results)==len(commands) and all(v['exit_code']==v['diagnostic_bytes']==0 for v in results)
    hashes=dict(old['authored_source_sha256'])
    for path in [*sources.values(),Path(__file__)]:hashes[path.resolve().relative_to(r).as_posix()]=sha(path)
    report=dict(status='Ninja_shared_matrix_family_RZ_runtime_linked' if passed else 'build_failed',commands=results,
        exit_code=0 if passed else 1,diagnostic_bytes=sum(v['diagnostic_bytes'] for v in results),compiler_sha256=sha(a.compiler),
        baseline_report_sha256=sha(b/'build_verification.json'),authored_source_sha256=hashes,
        existing_provider_objects_sha256={str(o):h for o,h in objects.items()},diagnostic_probes_sha256={str(o):h for o,h in probes.items()},
        production_objects_sha256={n+'.obj':sha(d/(n+'.obj')) for n in sources} if passed else {},
        replaces_existing_production_objects=['math.obj','multi_matrix.obj'],
        DLL=str(dll),DLL_sha256=sha(dll) if passed else None,inverse_scalar_mode=old['inverse_scalar_mode'],
        Ninja_matrix_numerical_mode='toward_zero_DN1_exceptions_disabled',single_active_matrix_owner=True,
        original_nearest_sources_and_proofs_preserved=True,renderer_SH4ZAM_arithmetic_migrated=False,
        game_source_linked=False,retail_assets_embedded=False,authored_asset_free_artifact_publishable=True,
        behavior_verified=False,overall_completion_percent=10,goal_paused=False,playable=False)
    for o,h in dict(objects,**{}).items():assert sha(o)==h
    for o,h in probes.items():assert sha(o)==h
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],'diagnostic bytes',report['diagnostic_bytes']);raise SystemExit(0 if passed else 1)

if __name__=='__main__':main()
