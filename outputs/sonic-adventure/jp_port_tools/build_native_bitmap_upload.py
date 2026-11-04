"""Compile original repaired bitmap queue with real native memory uploads.

Replace ONE retained texture owner, retain every other frame-owner object, and
remove bitmap trace definitions from the isolated original-frame diagnostic.
External game/SDK function bodies and linked DLLs remain private.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
from build_native_texture_runtime import function

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','present','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';w=r/'work/sonic-adventure';u=a.upstream.resolve();d=a.output.resolve()
    assert d.is_relative_to(w) and not d.is_relative_to(w/'build') and not d.exists()
    b=json.loads((a.present/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    external=['src/system/util.c','src/system/bitmap.h','sdk/ninja/njTexture.c','sdk/ninja/njLoadTextureLN.c','sdk/ninja/njTexLoad.c','sdk/ninja/kmtex_kamui_lib.c','sdk/include/ninja.h','sdk/include/kamui.h']
    # Bind to the reviewed upstream pin, then preserve exact input hashes.
    git='C:/Program Files/Git/cmd/git.exe'
    assert subprocess.check_output([git,'-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output([git,'-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    inputs={n:sha(u/n) for n in external};d.mkdir();(d/'frames').mkdir()
    common=[str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-fno-strict-aliasing','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-Wno-unused-parameter',
        '-include',str(p/'native/jp_host_types.h'),'-I'+str(p/'native'),'-I'+str(u/'sdk/include'),'-I'+str(u/'sdk'),'-I'+str(u/'src')]
    commands=[];authored=['jp_native_texture_upload_owner.c','jp_native_texture_upload.c','jp_native_bitmap_upload.c','jp_native_texture_upload.h']
    for src,name in [(authored[0],'textures_upload.obj'),(authored[1],'memory_upload.obj'),(authored[2],'bitmap_source.obj')]:commands.append(common+['-c',str(p/'native'/src),'-o',str(d/name)])
    util=(u/external[0]).read_bytes().replace(b'\r\n',b'\n');queue=b'#include "jp_native_texture_upload.h"\n#include "system/bitmap.h"\n#define QUEUE_SIZE 8\n#define BITMAP_TEXFMT (NJD_TEXFMT_RECTANGLE | NJD_TEXFMT_ARGB_1555)\n#define BITMAP_TEXATTR (NJD_TEXATTR_TYPE_MEMORY | NJD_TEXATTR_GLOBALINDEX)\nstatic struct bitmap *texture_queue[QUEUE_SIZE];\nint texture_queue_count;\n'
    # Keep the complete original functions, selecting only its GCC repair.
    clear=function(util,'bitmap_clear_queue');add=function(util,'bitmap_queue_add');flush=function(util,'bitmap_flush_queue')
    assert flush.count(b'#ifdef GCC')==1 and flush.count(b'#endif')==1
    flush=flush.replace(b'#ifdef GCC',b'#if 1')
    flush=flush.replace(b'NJS_TEXINFO info;',b'NJS_TEXINFO info = {0};')
    marker=b'            if (bitmap->texlist.textures != NULL) {';assert flush.count(marker)==1
    flush=flush.replace(marker,b'            if (jp_bitmap_source_begin(bitmap)) { jp_bitmap_retry_from(i); return; }\n'+marker)
    release=b'                njReleaseTexture(&bitmap->texlist);';assert flush.count(release)==1
    flush=flush.replace(release,b'                if (njReleaseTexture(&bitmap->texlist) < 0) { jp_memory_texture_error(); jp_memory_unbind_source(); jp_bitmap_retry_from(i); return; }\n                bitmap->texlist.textures = NULL;')
    load=b'            njLoadTexture(&bitmap->texlist);';assert flush.count(load)==1
    flush=flush.replace(load,b'            if (njLoadTexture(&bitmap->texlist) < 0) { bitmap->texlist.textures = NULL; jp_memory_unbind_source(); jp_bitmap_retry_from(i); return; }\n            jp_memory_unbind_source();')
    # This error path consumes only the successfully uploaded prefix. Remaining
    # callers retain the same bitmap pointers; no lost failed/remaining work.
    queue+=b'static void jp_bitmap_retry_from(int first)\n{\n    int pending=texture_queue_count-first;\n    for(int i=0;i<pending;i++)texture_queue[i]=texture_queue[first+i];\n    texture_queue_count=pending;\n}\n'+clear+b'\n'+add+b'\n'+flush
    queue_path=d/'external_bitmap_queue.c';queue_path.write_bytes(queue)
    raw=(u/'sdk/ninja/njTexture.c').read_bytes();load=function(raw,'njLoadTexture')
    load=load.replace(b'{',b'{\n    if (!texlist || !texlist->textures) return -1;',1)
    dispatch=d/'external_memory_list.c';dispatch.write_bytes(b'#include "jp_native_texture_upload.h"\nextern Uint32 _nj_user_globalIndex;\n'+function(raw,'njInitTextureErrStat')+b'\n'+function(raw,'njInitTextureGlobalIndex')+b'\n'+load)
    for src,name in [(queue_path,'bitmap_queue.obj'),(dispatch,'memory_list.obj')]:commands.append(common+['-c',str(src),'-o',str(d/name)])
    link=list(b['commands'][0]['command']);assert '-shared' in link
    retained=[v for v in link if v.endswith('.obj') and Path(v).name=='textures_retained.obj'];assert len(retained)==1
    retained=retained[0];old_owner=sha(Path(retained));link[link.index(retained)]=str(d/'textures_upload.obj')
    kept={str(Path(v).resolve()):sha(Path(v)) for v in link if v.endswith('.obj') and v!=str(d/'textures_upload.obj')}
    # Existing callers and the diagnostic DLL all import this one shared owner.
    exports=['njSetTextureInfo','njLoadTexture','njLoadTextureLN','njLoadMemoryTexture','njInitTextureErrStat','njInitTextureGlobalIndex','bitmap_flush_queue','bitmap_clear_queue','bitmap_queue_add']
    at=link.index('-o');link[at:at]=[str(d/n) for n in ['memory_upload.obj','bitmap_source.obj','bitmap_queue.obj','memory_list.obj']]+['-Wl,/export:'+n for n in exports]+['-Xlinker','/export:texture_queue_count,DATA']
    link[link.index('-o')+1]=str(d/'frames/jp_native_frames.dll');commands.append(link)
    frame=(a.present/'external_frame_native.c').read_bytes();present=(a.present/'external_present_native.c').read_bytes();diag=(a.present/'external_present_diagnostic.c').read_bytes()
    for name,raw in [('frame',frame),('present',present),('diagnostic',diag)]:
        raw=b'#include "jp_native_texture_upload.h"\n'+raw
        for routine,ending in ([('frame_wait',b'return;')] if name=='frame' else [('present_hold',b'return TRUE;')] if name=='present' else [('frame_wait',b'return;'),('present_hold',b'return TRUE;')]):
            body=function(raw,routine);old=b'bitmap_flush_queue();';assert body.count(old)==1
            replacement=body.replace(old,old+b'\n        if (jp_memory_status()) '+ending)
            if routine=='frame_wait':
                marker=b'if (jp_wait_status()) return;';assert replacement.count(marker)==1
                replacement=replacement.replace(marker,b'if (jp_wait_status() || jp_memory_status()) return;')
            raw=raw.replace(body,replacement)
        if name=='diagnostic':
            for definition in [b'void bitmap_flush_queue(void){event(9);}',b'void bitmap_clear_queue(void){event(15);}']:
                assert raw.count(definition)==1;raw=raw.replace(definition,b'')
        target=d/('external_'+name+'_native.c');target.write_bytes(raw)
        old_common=list(next(row['command'] for row in b['commands'] if row['command'][-1].endswith('frame_pacing.obj')));old_common=old_common[:old_common.index('-c')]
        commands.append(old_common+['-c',str(target),'-o',str(d/({'frame':'frame_pacing.obj','present':'game_present.obj','diagnostic':'diagnostic.obj'}[name]))])
    diagnostic_link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1].endswith('jp_native_present.dll')))
    changes={str(a.present/'diagnostic.obj'):str(d/'diagnostic.obj'),str(a.present/'frames/jp_native_frames.lib'):str(d/'frames/jp_native_frames.lib')}
    for old,new in changes.items():assert diagnostic_link.count(old)==1,(old,diagnostic_link);diagnostic_link[diagnostic_link.index(old)]=new
    diagnostic_link[-1]=str(d/'jp_native_present.dll');commands.append(diagnostic_link)
    results=[]
    for i,command in enumerate(commands):
        run=subprocess.run(command,cwd=r,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=command,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    for n,h in kept.items():assert sha(Path(n))==h,n
    for n,h in inputs.items():assert sha(u/n)==h,n
    sources=dict(b['authored_source_sha256'])
    for f in [*[p/'native'/n for n in authored],Path(__file__).resolve()]:sources[f.relative_to(r).as_posix()]=sha(f)
    result=dict(status='real_bitmap_queue_and_bounded_memory_loader_compiled_behavior_unverified',exit_code=0,diagnostic_bytes=0,commands=results,compiler_sha256=sha(a.compiler),
        authored_source_sha256=sources,external_source_sha256=inputs,upstream_commit='25a3d3154dbe657325e97dbe8c5291086f6af5c0',
        kept_frame_owner_objects_sha256=kept,replaced_retained_texture_object_sha256=old_owner,
        generated_source_sha256={x.name:sha(x) for x in d.glob('*.c')},production_objects_sha256={n:sha(d/n) for n in ['textures_upload.obj','memory_upload.obj','bitmap_source.obj','bitmap_queue.obj','memory_list.obj','frame_pacing.obj','game_present.obj']},
        frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),
        GPU_DLL=b['GPU_DLL'],GPU_DLL_sha256=b['GPU_DLL_sha256'],present_baseline=str(a.present.resolve()),present_baseline_report_sha256=sha(a.present/'build_verification.json'),
        original_bitmap_queue_GCC_pointer_repair_retained=True,original_memory_loader_body_replaced_by_bounded_native_service=True,original_list_loader_and_record_release_retained=True,
        single_shared_texture_queue_frame_video_device_owner=True,bitmap_trace_providers_removed=True,other_diagnostic_game_services_remain_traces=True,
        game_or_SDK_bodies_public=False,full_game_loop_executed=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
