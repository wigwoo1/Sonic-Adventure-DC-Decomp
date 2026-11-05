"""Unchanged logo_start importing the complete genuine shared task/heap owner."""
import argparse,hashlib,json,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','probe','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r=a.workspace;u=a.upstream;p=r/'outputs/sonic-adventure';d=a.output
    b=json.loads((a.build/'build_verification.json').read_bytes());pb=json.loads((a.probe/'build_verification.json').read_bytes())
    assert b['task_runtime_in_frame_owner'] and b['complete_task_source_linked'] and b['one_real_clock_owner']
    assert sha(a.compiler)==b['compiler_sha256'] and pb['asset_build_sha256']==sha(a.build/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    raw=(u/'src/progress/demo.c').read_bytes();assert hashlib.sha256(raw).hexdigest()==pb['external_game_source_sha256']
    previous=a.probe/'external_original_logo_draw.c';assert sha(previous)==pb['private_generated_source_sha256']
    prefix=previous.read_bytes().split(b'__declspec(dllexport) int jp_logo_asset_load')[0]
    d.mkdir();(d/'inputs/progress/demo').mkdir(parents=True)
    for n,record in pb['private_generated_metadata'].items():
        f=a.probe/'inputs/progress/demo'/n;assert sha(f)==record['sha256'];(d/'inputs/progress/demo'/n).write_bytes(f.read_bytes())
    enum=raw[raw.index(b'enum logo_mode {'):raw.index(b'\n};',raw.index(b'enum logo_mode {'))+3]
    start=function(raw,'logo_start');game=d/'external_original_logo_start.c'
    game.write_bytes(prefix+b'\n#include "system/task.h"\n#include "system/frame.h"\n#include "progress/title.h"\n#include "draw/fade.h"\n'+enum+b'\nstatic enum logo_mode logo_mode;\nstatic int logo_frames;\n'+start+b'''
__declspec(dllexport) void jp_logo_asset_draw(void){logo_draw();}
__declspec(dllexport) NJS_TEXLIST *jp_logo_asset_list(void){return &logo_texlist;}
__declspec(dllexport) int jp_logo_asset_release(void){return njReleaseTexture(&logo_texlist);}
__declspec(dllexport) int jp_logo_mode(void){return logo_mode;}
__declspec(dllexport) int jp_logo_frames(void){return logo_frames;}
''')
    fade=(u/'src/draw/fade.c').read_bytes();cover=d/'external_original_cover.c'
    cover.write_bytes(b'#include <ninja.h>\nstatic NJS_COLOR fade_cover;\n'+function(fade,'fade_set_cover')+function(fade,'fade_get_cover'))
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')]+['-I'+str(d/'inputs')]
    commands=[common+['-c',str(src),'-o',str(d/(n+'.obj'))] for n,src in [('logo',game),('cover',cover)]]
    forwarders=['jp_startup_pool_info','task_alloc','task_get_current_root_step','jp_task_runtime_get_layout','jp_heap_statistics','jp_heap_allocate','jp_heap_release','jp_heap_allocate_zeroed','jp_heap_reset']
    dll=d/'jp_native_logo_start.dll'
    commands.append([str(a.compiler),'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'logo.obj'),str(d/'cover.obj'),str(a.build/'frames/jp_native_frames.lib'),str(Path(b['DLL']).with_suffix('.lib')),
        *['-Wl,/export:'+n for n in ['logo_start','fade_set_cover','fade_get_cover']],
        *['-Wl,/export:'+n+'=jp_native_frames.'+n for n in forwarders],'-o',str(dll)])
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:5000]
    report=dict(status='original_logo_start_linked_to_complete_real_shared_task_heap_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,compiler_sha256=sha(a.compiler),DLL=str(dll),DLL_sha256=sha(dll),frames_DLL=b['frames_DLL'],GPU_DLL=b['GPU_DLL'],present_DLL=b['DLL'],frame_build=str(a.build),frame_build_sha256=sha(a.build/'build_verification.json'),metadata_probe_build_sha256=sha(a.probe/'build_verification.json'),original_logo_start_body_sha256=hashlib.sha256(start).hexdigest(),original_logo_start_body_unchanged=True,original_cover_bodies_unchanged=True,complete_task_heap_imported=True,duplicate_task_or_heap_storage=False,task_diagnostic_forwarders=forwarders,generated_sources_sha256={x.name:sha(x) for x in [game,cover]},authored_source_sha256={str(Path(__file__).resolve().relative_to(r)):sha(Path(__file__))},artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(report['status'])


if __name__=='__main__':main()
