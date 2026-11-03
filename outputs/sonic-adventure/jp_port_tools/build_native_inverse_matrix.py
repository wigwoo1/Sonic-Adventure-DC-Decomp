"""Build authored inverse/determinant/scalar code with existing shared state."""
import argparse,hashlib,json,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,b,d=[getattr(a,n).resolve() for n in ['workspace','upstream','baseline','output']]
    p=r/'outputs/sonic-adventure'
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    pin_path=p/'jp_port_tools/jp_inverse_matrix_external_inputs.json';pin=json.loads(pin_path.read_text())
    for n,h in pin['files_sha256'].items():assert sha(u/n)==h,n
    for n,h in pin['reused_authored_scalar_cores_sha256'].items():assert sha(r/n)==h,n
    bb=json.loads((b/'build_verification.json').read_text());assert bb['exit_code']==bb['diagnostic_bytes']==0
    assert sha(a.compiler)==bb['compiler_sha256']
    for n,h in bb['authored_source_sha256'].items():assert sha(r/n)==h,n
    objects={Path(n).resolve():h for n,h in bb['existing_provider_objects_sha256'].items()}
    for n,h in bb['production_objects_sha256'].items():objects[b/n]=h
    for o,h in objects.items():assert sha(o)==h
    probe_commands=[v['command'] for v in bb['commands'] if '-c' in v['command'] and any('jp_sh4zam_probe' in x for x in v['command'])]
    assert len(probe_commands)==2
    z=Path(probe_commands[0][probe_commands[0].index('-isystem')+1]).parent
    dep=json.loads((p/'jp_port_tools/jp_renderer_external_inputs.json').read_text())
    for n,h in dep['sh4zam_files_sha256'].items():assert sha(z/n)==h,n
    d.mkdir(parents=True)
    production=[('inverse_matrix',p/'native/jp_ninja_inverse_matrix.c'),('scalar32',p/'native/jp_ninja_scalar32.c')]
    production +=[(n,p/'source'/(n+'.c')) for n in ['sh4_add_sub','sh4_multiply','sh4_multiply_extended','sh4_fmac','sh4_divide']]
    units=production+[('inverse_probe',p/'native/jp_ninja_inverse_probe.c')]
    flags=['--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror']
    commands=[[str(a.compiler),*flags,'-c',str(source),'-o',str(d/(name+'.obj'))] for name,source in units]
    for i,cmd in enumerate(probe_commands):
        cmd=list(cmd);cmd[-1]=str(d/f'cross_state_{i}.obj');commands.append(cmd)
    dll=d/'jp_shared_matrix_inverse.dll'
    commands.append([str(a.compiler),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld','-Wl,/export:shz_xmtrx_state_',
                     *map(str,objects),*[str(d/(n+'.obj')) for n,_ in units],
                     str(d/'cross_state_0.obj'),str(d/'cross_state_1.obj'),'-o',str(dll)])
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        if run.returncode:print(log.read_text()[:6000]);break
    passed=len(results)==len(commands) and all(v['exit_code']==0 for v in results)
    authored=[source for _,source in units]+[p/'native'/n for n in ['jp_ninja_scalar32.h','jp_ninja_inverse_matrix.h']]+[pin_path,Path(__file__).resolve()]
    for n,h in pin['files_sha256'].items():assert sha(u/n)==h
    for n,h in dep['sh4zam_files_sha256'].items():assert sha(z/n)==h
    hashes={n:sha(r/n) for n in dict(bb['authored_source_sha256'],**pin['reused_authored_scalar_cores_sha256'])}
    hashes.update({s.relative_to(r).as_posix():sha(s) for s in authored})
    report=dict(status='native_inverse_and_determinant_runtime_linked' if passed else 'build_failed',commands=results,
                exit_code=0 if passed else 1,diagnostic_bytes=sum(v['diagnostic_bytes'] for v in results),compiler_sha256=sha(a.compiler),
                baseline_report_sha256=sha(b/'build_verification.json'),external_input_manifest_sha256=sha(pin_path),
                upstream_commit=pin['upstream_commit'],source_reference_sha256=pin['files_sha256'],authored_source_sha256=hashes,
                existing_provider_objects_sha256={str(o):h for o,h in objects.items()},
                production_objects_sha256={n+'.obj':sha(d/(n+'.obj')) for n,_ in production} if passed else {},
                expected_resolved_services=['njInvertMatrix'],additional_complete_SDK_entry_points=['njDetMatrix'],
                DLL=str(dll),DLL_sha256=sha(dll) if passed else None,game_source_linked=False,retail_assets_embedded=False,
                authored_asset_free_artifact_publishable=True,game_containing_artifact_publishable=False,
                single_existing_matrix_state_owner=True,behavior_verified=False,overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],'diagnostic bytes',report['diagnostic_bytes']);raise SystemExit(0 if passed else 1)
if __name__=='__main__':main()
