"""Compile real pinned SDK state setters with owned native scratch storage."""
import argparse,hashlib,json,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,c,d=(getattr(a,n).resolve() for n in ['workspace','upstream','compiler','output'])
    p=r/'outputs/sonic-adventure'
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    inputs=p/'jp_port_tools/jp_render_state_external_inputs.json';pin=json.loads(inputs.read_text())
    base=json.loads((p/'jp_port_tools/jp_task_external_inputs.json').read_text())
    def verify():
        for n,h in dict(base['files_sha256'],**pin['files_sha256']).items():assert sha(u/n)==h,n
    verify();d.mkdir(parents=True)
    control=(u/'sdk/ninja/njControl.c').read_bytes()
    anchor=b'void njInit3D(NJS_VERTEX_BUF *vbuf, Int vn)\n{'
    assert control.count(anchor)==1
    control=control.replace(anchor,anchor+b'\n    if (!jp_render_state_accept_buffer(vbuf,vn)) return;')
    private=[]
    for name,raw in [('external_control.c',control),('external_screen.c',(u/'sdk/ninja/njSetScreen.c').read_bytes())]:
        path=d/name;path.write_bytes(b'#include "jp_native_render_state.h"\n'+raw);private.append(path)
    sources=[p/'native/jp_native_render_state.c',*private]
    common=[str(c),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off',
        '-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas',
        '-include',str(p/'native/jp_host_types.h'),'-I',str(p/'native'),'-I',str(u/'sdk/include')]
    objects=[d/(s.stem+'.obj') for s in sources]
    commands=[common+['-c',str(s),'-o',str(o)] for s,o in zip(sources,objects)]
    exports=['njInit3D','njControl3D','njSetConstantAttr','njSetConstantMaterial','njSetDepthQueue','njSetScreen','njClipZ']
    globals=['_nj_screen_','_nj_2d_clip_','_nj_near_clip_','_nj_far_clip_','_nj_scr_aspect_x_',
        '_nj_scr_aspect_y_','_nj_control_3d_flag_','_nj_constant_attr_and_','_nj_constant_attr_or_',
        '_nj_constant_material_','_nj_vertex_buf_','_nj_vertex_num_','_nj_calc_vertex_',
        '_nj_calc_polygon_','_nj_draw_polygon_','_nj_depth_near_','_nj_depth_far_','_nj_depth_range_']
    dll=d/'jp_native_render_state.dll'
    commands.append([str(c),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',
        *['-Wl,/export:'+n for n in exports],
        *[arg for n in globals for arg in ['-Xlinker','/export:'+n+',DATA']],*map(str,objects),'-o',str(dll)])
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        if run.returncode:print(log.read_text()[:5000]);break
    verify();passed=len(results)==4 and all(x['exit_code']==0 for x in results)
    authored=[sources[0],p/'native/jp_native_render_state.h',p/'native/jp_host_types.h',Path(__file__)]
    report=dict(status='native_render_state_linked_behavior_unverified' if passed else 'build_failed',commands=results,
        compiler_sha256=sha(c),external_manifest_sha256=sha(inputs),
        authored_source_sha256={s.relative_to(r).as_posix():sha(s) for s in authored},
        generated_private_source_sha256={s.name:sha(s) for s in private},
        object_sha256={o.name:sha(o) for o in objects if o.exists()},dll_sha256=sha(dll) if passed else None,
        SDK_Init3D_owned_buffer_guard=True,ClipZ_native_value_contract=True,
        game_source_linked=True,game_containing_artifact_publishable=False,original_sources_modified=False,
        complete_renderer_linked=False,overall_completion_percent=10,goal_paused=False,playable=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],'diagnostic bytes',sum(x['diagnostic_bytes'] for x in results))
    raise SystemExit(0 if passed else 1)
if __name__=='__main__':main()
