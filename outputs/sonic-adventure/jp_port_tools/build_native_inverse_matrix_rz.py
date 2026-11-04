"""Bind the preserved authored inverse to explicit toward-zero scalar services.

Reuses one existing active-state owner; does not change old sources or proofs.
The old shared math provider is reused for state/copy tests only: its other
arithmetic and the renderer still need a separate toward-zero migration.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for name in ['workspace','baseline','compiler','output']:ap.add_argument('--'+name,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();b=a.baseline.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    old=json.loads((b/'build_verification.json').read_text())
    assert old['exit_code']==old['diagnostic_bytes']==0 and sha(a.compiler)==old['compiler_sha256']
    for name,h in old['authored_source_sha256'].items():assert sha(r/name)==h,name
    existing={Path(n):h for n,h in old['existing_provider_objects_sha256'].items() if Path(n).name in ['math.obj','matrix_state.obj']}
    assert len(existing)==2
    for name in ['sh4_add_sub','sh4_multiply','sh4_multiply_extended','sh4_fmac','sh4_divide']:
        existing[b/(name+'.obj')]=old['production_objects_sha256'][name+'.obj']
    for path,h in existing.items():assert sha(path)==h,path
    d.mkdir(parents=True)
    flags=['--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror']
    mapping=['-Djp_scalar32_'+n+'=jp_rz_scalar32_'+n for n in ['add','subtract','multiply','divide']]
    source=p/'native/jp_ninja_inverse_matrix.c';scalar=p/'native/jp_ninja_scalar32_rz.c';probe=p/'native/jp_ninja_inverse_probe.c'
    commands=[]
    for name,path,extra in [('inverse_matrix_rz',source,mapping),('scalar32_rz',scalar,[]),('inverse_probe',probe,[])]:
        commands.append([str(a.compiler),*flags,*extra,'-c',str(path),'-o',str(d/(name+'.obj'))])
    probes=[]
    for row in old['commands']:
        cmd=row['command']
        if '-c' in cmd and any('jp_sh4zam_probe' in v for v in cmd):
            obj=Path(cmd[-1]);assert obj.exists();probes.append(obj)
    assert len(probes)==2
    dll=d/'jp_shared_matrix_inverse_rz.dll'
    commands.append([str(a.compiler),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld','-Wl,/export:shz_xmtrx_state_',
                     *map(str,existing),str(d/'inverse_matrix_rz.obj'),str(d/'scalar32_rz.obj'),str(d/'inverse_probe.obj'),*map(str,probes),'-o',str(dll)])
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        if run.returncode:print(log.read_text()[:5000]);break
    passed=len(results)==len(commands) and all(row['exit_code']==row['diagnostic_bytes']==0 for row in results)
    authored={n:sha(r/n) for n in old['authored_source_sha256']}
    for path in [scalar,p/'native/jp_ninja_scalar32_rz.h',Path(__file__)]:authored[path.resolve().relative_to(r).as_posix()]=sha(path)
    report=dict(status='toward_zero_inverse_runtime_linked' if passed else 'build_failed',exit_code=0 if passed else 1,
        diagnostic_bytes=sum(v['diagnostic_bytes'] for v in results),commands=results,compiler_sha256=sha(a.compiler),
        baseline_report_sha256=sha(b/'build_verification.json'),authored_source_sha256=authored,
        existing_provider_objects_sha256={str(o):h for o,h in existing.items()},diagnostic_probes_sha256={str(o):sha(o) for o in probes},
        production_objects_sha256={n+'.obj':sha(d/(n+'.obj')) for n in ['inverse_matrix_rz','scalar32_rz']} if passed else {},
        replaces_existing_production_objects=['inverse_matrix.obj','scalar32.obj'],inverse_scalar_mode='toward_zero_DN1_exceptions_disabled',
        DLL=str(dll),DLL_sha256=sha(dll) if passed else None,single_existing_matrix_state_owner=True,
        inverse_implementation_source_unchanged=True,original_nearest_sources_and_proofs_preserved=True,
        other_shared_math_toward_zero_qualified=False,renderer_toward_zero_qualified=False,full_game_mode_verified=False,
        game_source_linked=False,retail_assets_embedded=False,authored_asset_free_artifact_publishable=True,
        behavior_verified=False,overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],'diagnostic bytes',report['diagnostic_bytes']);raise SystemExit(0 if passed else 1)

if __name__=='__main__':main()
