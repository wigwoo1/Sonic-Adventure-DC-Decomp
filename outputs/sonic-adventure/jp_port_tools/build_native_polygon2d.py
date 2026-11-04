"""Build a complete authored polygon producer plus a private diagnostic DLL.

Only the three new production objects enter the real frame dependency audit.
Diagnostic replay/wide stand-ins never enter that audit. The diagnostic DLL
reuses real SDK initialization objects, so it stays private with game bodies.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline-audit','compiler','output']:
        ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    pin_path=p/'jp_port_tools/jp_polygon2d_external_inputs.json';pin=json.loads(pin_path.read_text())
    for n,h in pin['files_sha256'].items():assert sha(u/n)==h,n
    prior=json.loads(a.baseline_audit.read_text());objects={Path(n):h for n,h in prior['linked_objects_sha256'].items()}
    for o,h in objects.items():assert sha(o)==h,o
    camera_path=r/'work/sonic-adventure/jp-native-camera-v1/build_verification.json';cam=json.loads(camera_path.read_text())
    assert cam['exit_code']==cam['diagnostic_bytes']==0 and sha(a.compiler)==cam['compiler_sha256']
    for n,h in cam['authored_source_sha256'].items():assert sha(r/n)==h,n
    test_objects={Path(n):h for n,h in cam['existing_provider_objects_sha256'].items()}
    test_objects.update({camera_path.parent/n:h for n,h in cam['production_objects_sha256'].items()})
    test_objects.update({Path(n):h for n,h in cam['diagnostic_probes_sha256'].items()})
    required=['queue.obj','jp_native_render_state.obj','external_control.obj','external_screen.obj']
    for name in required:
        found=[o for o in objects if o.name==name];assert len(found)==1
        test_objects[found[0]]=objects[found[0]]
    for o,h in test_objects.items():assert sha(o)==h
    d.mkdir(parents=True)
    sources={'polygon2d':p/'native/jp_native_polygon2d_rz.c','picture_aspect':p/'native/jp_native_picture_aspect_rz.c','color_mode':p/'native/jp_native_color_mode.c','polygon_probe':p/'jp_port_tools/polygon2d_diagnostic_probe.c'}
    common=[str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-include',str(p/'native/jp_host_types.h'),'-I',str(p/'native'),'-I',str(u/'sdk/include'),'-I',str(u)]
    commands=[common+['-c',str(s),'-o',str(d/(n+'.obj'))] for n,s in sources.items()]
    # Compile ABI/layout assertions against pinned external declarations.
    abi=d/'abi_check.c'
    abi.write_text('#include "jp_ninja_polygon2d.h"\n#define njDrawPolygon2D sdk_njDrawPolygon2D\n#include <ninja.h>\n#undef njDrawPolygon2D\n#include "gcc/src/system/port.h"\n#include <stddef.h>\n'
        '_Static_assert(sizeof(jp_point2col)==sizeof(NJS_POINT2COL),"polygon layout");\n'
        '_Static_assert(sizeof(jp_color2)==sizeof(NJS_COLOR),"color layout");\n'
        '_Static_assert(sizeof(jp_point2)==sizeof(NJS_POINT2),"point layout");\n'+''.join(
        f'_Static_assert(offsetof(jp_point2col,{n})==offsetof(NJS_POINT2COL,{n}),"{n} offset");\n' for n in ['p','col','tex','num'])+
        '_Static_assert((int)JP_PRESENT_POLYGON_2D==(int)PRESENT_POLYGON_2D,"replay kind");\n'
        '_Static_assert((int)JP_PRESENT_SPRITE_2D==(int)PRESENT_SPRITE_2D,"sprite replay kind");\n')
    commands.append(common+['-c',str(abi),'-o',str(d/'abi_check.obj')])
    dll=d/'jp_native_polygon2d.dll'
    exports=['njSetkmColorMode','njSetkmModelAttr','_nj_control_3d_flag_','_nj_ctx_color_mode_','port_wide_anchor','port_wide_cover','njGetkmPointer','njSetkmPointer','njEndVertex','njStartVertexNonTexOpaque','njStartVertexNonTexTrans','njStartVertexTexOpaque','njStartVertexTexTrans','_builtin_prefetch','shz_xmtrx_state_']
    commands.append([str(a.compiler),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',*['-Wl,/export:'+n for n in exports],*map(str,test_objects),*[str(d/(n+'.obj')) for n in sources],'-o',str(dll)])
    librarian=a.compiler.parent/'llvm-lib.exe';component=d/'jp_polygon2d_component.lib'
    commands.append([str(librarian),'/nologo','/out:'+str(component),*[str(d/(n+'.obj')) for n in ['polygon2d','picture_aspect','color_mode']]])
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        if run.returncode:print(log.read_text()[:6000]);break
    passed=len(results)==len(commands) and all(v['exit_code']==v['diagnostic_bytes']==0 for v in results)
    authored=[*sources.values(),p/'native/jp_ninja_polygon2d.h',pin_path,Path(__file__)]
    report=dict(status='complete_native_polygon2d_producer_built' if passed else 'build_failed',exit_code=0 if passed else 1,diagnostic_bytes=sum(v['diagnostic_bytes'] for v in results),commands=results,
        authored_source_sha256={s.resolve().relative_to(r).as_posix():sha(s) for s in authored},compiler_sha256=sha(a.compiler),external_manifest_sha256=sha(pin_path),external_files_sha256=pin['files_sha256'],
        baseline_audit_sha256=sha(a.baseline_audit),existing_diagnostic_objects_sha256={str(o):h for o,h in test_objects.items()},
        production_objects_sha256={n+'.obj':sha(d/(n+'.obj')) for n in ['polygon2d','picture_aspect','color_mode']} if passed else {},
        expected_resolved_services=['njDrawPolygon2D','port_draw_aspect','port_pillarbox','port_sprite_record'],diagnostic_probe_object_sha256=sha(d/'polygon_probe.obj') if passed else None,
        DLL=str(dll),DLL_sha256=sha(dll) if passed else None,diagnostic_DLL_contains_external_SDK_bodies=True,diagnostic_DLL_publishable=False,
        component_library=str(component),component_library_sha256=sha(component) if passed else None,component_library_contains_only_authored_production_objects=True,librarian_sha256=sha(librarian),
        diagnostic_replay_and_wide_owner_standins=True,production_uses_real_replay_and_render_state=True,behavior_verified=False,
        numerical_policy='RZ_DN1_exceptions_disabled_value_contract',full_frame_executed=False,playable=False,overall_completion_percent=10,goal_paused=False)
    for n,h in pin['files_sha256'].items():assert sha(u/n)==h,n
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],'diagnostic bytes',report['diagnostic_bytes']);raise SystemExit(0 if passed else 1)

if __name__=='__main__':main()
