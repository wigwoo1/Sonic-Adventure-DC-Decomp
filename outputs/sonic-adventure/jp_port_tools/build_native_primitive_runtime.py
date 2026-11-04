"""Link complete actual primitive bodies and one existing real runtime state.
The frame/replay observer remains diagnostic; game-containing DLL is private.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','primitives','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve();bdir=a.baseline.resolve();s=a.primitives.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((bdir/'build_verification.json').read_text());pb=json.loads((s/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==pb['exit_code']==pb['diagnostic_bytes']==0
    assert b['compiler_sha256']==pb['compiler_sha256']==sha(a.compiler)
    for x in [b,pb]:
        for n,h in x['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(p/'jp_port_tools/rewrite_sprite_float_operations.py')==pb['reused_float_rewriter_sha256']
    link=list(next(v['command'] for v in b['commands'] if '-shared' in v['command']));old=[v for v in link if v.endswith('sprite_probe.obj')];assert len(old)==1;link.remove(old[0])
    objects={Path(v):sha(Path(v)) for v in link if v.endswith('.obj')}
    for n,h in b['existing_provider_objects_sha256'].items():assert sha(Path(n))==h,n
    for n,h in pb['production_objects_sha256'].items():assert sha(s/n)==h;objects[s/n]=h
    d.mkdir(parents=True);probe=p/'jp_port_tools/primitive_diagnostic_probe.c'
    flags=['--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas',
        '-include',str(p/'native/jp_host_types.h'),'-I'+str(p/'native'),'-I'+str(p/'jp_port_tools'),'-I'+str(u/'sdk/include'),'-I'+str(u)]
    cmd=[str(a.compiler),*flags,'-c',str(probe),'-o',str(d/'primitive_probe.obj')]
    link=[v for v in link if not v.endswith('.obj')];link[-1]=str(d/'jp_native_primitives.dll');at=link.index('-o');link[at:at]=[*map(str,objects),str(d/'primitive_probe.obj')]
    exports=['njDrawPolygon','njDrawTexture','njDrawTextureMemList','njDrawPolygon3D','njFarClipping','njNearClipping','njtri_filled','njtri_filled_with_tex','njtri_wiredpoly','njtri_wiredpoly_with_tex','njCalcPoint','njSetkmTextureNumG','njSetkmTextureMemList']
    link[1:1]=['-Wl,/export:'+n for n in exports]
    results=[]
    for i,c in enumerate([cmd,link]):
        run=subprocess.run(c,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=c,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    authored=dict(b['authored_source_sha256'],**pb['authored_source_sha256'])
    for path in [probe,Path(__file__)]:authored[path.resolve().relative_to(r).as_posix()]=sha(path)
    dll=d/'jp_native_primitives.dll';report=dict(status='complete_native_primitive_diagnostic_runtime_linked_behavior_unqualified',exit_code=0,diagnostic_bytes=0,commands=results,
        authored_source_sha256=authored,compiler_sha256=sha(a.compiler),DLL=str(dll),DLL_sha256=sha(dll),existing_provider_objects_sha256={str(o):h for o,h in objects.items()},existing_diagnostic_objects_sha256={str(o):h for o,h in objects.items()},
        diagnostic_probe_object_sha256=sha(d/'primitive_probe.obj'),baseline_report_sha256=sha(bdir/'build_verification.json'),primitive_report_sha256=sha(s/'build_verification.json'),
        game_source_linked=True,artifact_publishable=False,diagnostic_frame_and_replay_observation=True,one_queue_and_matrix_and_texture_owner=True,
        original_primitive_packets_verified=False,full_frame_executed=False,playable=False,overall_completion_percent=10,goal_paused=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'],'zero diagnostics; all original algorithm bodies and real native owners linked')
if __name__=='__main__':main()
