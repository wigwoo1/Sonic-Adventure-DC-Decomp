"""Link actual sprite bodies and qualified providers into a private test DLL.
The replay and frame observer is diagnostic. Full production linking remains
incomplete; this DLL is not playable and must not be publicly distributed.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','sprites','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();s=a.sprites.resolve();base=a.baseline.resolve();u=a.upstream.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((base/'build_verification.json').read_text());sb=json.loads((s/'build_verification.json').read_text())
    assert b['exit_code']==b['diagnostic_bytes']==sb['exit_code']==sb['diagnostic_bytes']==0
    assert b['compiler_sha256']==sb['compiler_sha256']==sha(a.compiler)
    for report in [b,sb]:
        for n,h in report['authored_source_sha256'].items():assert sha(r/n)==h,n
    links=[v['command'] for v in b['commands'] if '-shared' in v['command']];assert len(links)==1
    link=list(links[0]);old_probe=[v for v in link if v.endswith('polygon_probe.obj')];assert len(old_probe)==1;link.remove(old_probe[0])
    objects={Path(v):sha(Path(v)) for v in link if v.endswith('.obj')}
    for n,h in b['existing_diagnostic_objects_sha256'].items():assert sha(Path(n))==h,n
    for n,h in sb['production_objects_sha256'].items():assert sha(s/n)==h;objects[s/n]=h
    # Reuse the actual native texture owner/SDK adapters, no selector stubs.
    audit=json.loads((r/'work/sonic-adventure/jp-queue-intensity-native-link-audit-v1/verification.json').read_text())
    texture_objects={Path(n):h for n,h in audit['linked_objects_sha256'].items() if Path(n).parent.name=='jp-native-texture-runtime-v4'}
    assert len(texture_objects)==10,len(texture_objects)
    for o,h in texture_objects.items():assert sha(o)==h;objects[o]=h
    d.mkdir(parents=True);probe=p/'jp_port_tools/sprite_diagnostic_probe.c'
    cmd=[str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-include',str(p/'native/jp_host_types.h'),'-I'+str(p/'native'),'-I'+str(u/'sdk/include'),'-I'+str(u),'-c',str(probe),'-o',str(d/'sprite_probe.obj')]
    link=[v for v in link if not v.endswith('.obj')];link[-1]=str(d/'jp_native_sprites.dll')
    pos=link.index('-o');link[pos:pos]=[*map(str,objects),str(d/'sprite_probe.obj')]
    exports=['njDrawSprite2D','njDrawSprite3D','njInitSprite','sprite_draw_3d','sprite_get_depth','sprite_set_depth','njSetTexture','njSetkmTextureNum',
        '_nj_screen_','_nj_scr_aspect_x_','_nj_scr_aspect_y_','_nj_2d_clip_','_nj_near_clip_','_nj_far_clip_','_nj_constant_material_','_nj_current_texture','_nj_current_texture_bank',
        'njSetTextureName','njEntryTexMemList','njFreeTexMemList','njTexMemList','njSetTextureParam','njSetPaletteBankLN','njReleaseTextureLN','njReleaseTextureNum','njReleaseTexture','njSearchTexList','njSearchTexList2','njSearchTexList3','njSetFlagAddrAllTexture','_nj_tex_count']
    link[1:1]=['-Wl,/export:'+n for n in exports]
    results=[]
    for i,command in enumerate([cmd,link]):
        run=subprocess.run(command,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=command,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    dll=d/'jp_native_sprites.dll';sources=dict(b['authored_source_sha256'],**sb['authored_source_sha256'])
    for source in [probe,Path(__file__)]:sources[source.resolve().relative_to(r).as_posix()]=sha(source)
    report=dict(status='complete_sprite_diagnostic_runtime_linked_behavior_unqualified',exit_code=0,diagnostic_bytes=0,commands=results,compiler_sha256=sha(a.compiler),
        DLL=str(dll),DLL_sha256=sha(dll),authored_source_sha256=sources,existing_provider_objects_sha256={str(o):h for o,h in objects.items()},diagnostic_probe_object_sha256=sha(d/'sprite_probe.obj'),
        baseline_report_sha256=sha(base/'build_verification.json'),sprite_object_report_sha256=sha(s/'build_verification.json'),actual_texture_providers_linked=True,
        diagnostic_replay_and_frame_observer=True,single_existing_queue_owner=True,single_existing_matrix_owner=True,single_existing_picture_owner=True,
        game_source_linked=True,artifact_publishable=False,original_sprite_behavior_verified=False,full_frame_executed=False,playable=False,overall_completion_percent=10,goal_paused=False)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'],'zero diagnostics; private actual-game-code DLL, no playable frame')

if __name__=='__main__':main()
