"""Compile bounded native file/PRS services and the adapted original game wrapper.
Original SDK/game bodies stay external/private. Vendored MIT prskit is unchanged.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';u=a.upstream.resolve();base=a.baseline.resolve();d=a.output.resolve()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=json.loads((base/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0 and b['compiler_sha256']==sha(a.compiler)
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    vendor=p/'third_party/prskit';vendor_files={}
    for name,original in [('prskit.c','prskit.c'),('prskit.h','prskit.h'),('LICENSE.txt','LICENSE')]:
        assert sha(vendor/name)==sha(u/'tools/prskit'/original);vendor_files[(vendor/name).relative_to(r).as_posix()]=sha(vendor/name)
    raw=(u/'src/system/texture.c').read_bytes();assert hashlib.sha256(raw).hexdigest()=='f08e3747079814517763a5fa3bfd0f3f51f314c85fe31e57cbff516f40fb7397'
    wrapper=function(raw,'texture_load_pvm').replace(b'\r\n',b'\n');original_wrapper_sha256=hashlib.sha256(wrapper).hexdigest()
    for old in [b'    NJS_TEXNAME *textures;\n',b'    int count;\n',b'    int i;\n']:assert wrapper.count(old)==1;wrapper=wrapper.replace(old,b'')
    wrapper=wrapper.replace(b'NJS_TEXLIST loaded;',b'NJS_TEXLIST loaded = {0};')
    call=b'    njSetPvmTextureList(&loaded, pvm_textures, names, PVM_TEXTURES);';assert wrapper.count(call)==1
    wrapper=wrapper.replace(call,b'    if (jp_assets_prepare_game(texlist, pvm_textures, PVM_TEXTURES)) return;\n'+call)
    call=b'    load_prs(name, &loaded);';assert wrapper.count(call)==1;wrapper=wrapper.replace(call,b'    if (load_prs(name, &loaded) < 0) return;')
    begin=wrapper.index(b'    count = texlist->nbTexture;');end=wrapper.rindex(b'\n}')
    wrapper=wrapper[:begin]+b'    (void)jp_assets_commit_game(&loaded, texlist);'+wrapper[end:]
    generated=b'''#include "jp_native_assets.h"
#define PVM_TEXTURES 512
static NJS_TEXNAME pvm_textures[PVM_TEXTURES];
static Sint32 load_prs(char *name, NJS_TEXLIST *texlist)
{
    char path[256];
    if (jp_assets_make_prs_path(name, path, sizeof path)) return -1;
    return jp_assets_load_prs(path, texlist, PVM_TEXTURES, PVM_NAME_SIZE);
}
'''+wrapper+b'''
Sint32 texture_load_prs_file(char *name, NJS_TEXLIST *texlist)
{return jp_assets_load_game_file(name,texlist);}
'''
    d.mkdir();(d/'frames').mkdir();game=d/'external_game_texture_load.c';game.write_bytes(generated)
    flags=list(b['commands'][0]['command']);flags=flags[:flags.index('-c')];assert flags[0]==str(a.compiler);flags+=['-I'+str(vendor)]
    sources=[(p/'native/jp_native_texture_move_owner.c','textures_move.obj'),(p/'native/jp_native_assets.c','assets.obj'),(vendor/'prskit.c','prskit.obj'),(game,'game_texture.obj')]
    commands=[flags+['-c',str(path),'-o',str(d/name)] for path,name in sources]
    link=list(next(row['command'] for row in b['commands'] if '-shared' in row['command'] and row['command'][-1]==b['frames_DLL']))
    old=[v for v in link if v.endswith('.obj') and Path(v).name=='textures_upload.obj'];assert len(old)==1
    replaced={old[0]:sha(Path(old[0]))};link[link.index(old[0])]=str(d/'textures_move.obj')
    kept={str(Path(v).resolve()):sha(Path(v)) for v in link if v.endswith('.obj') and v!=str(d/'textures_move.obj')}
    at=link.index('-o');link[at:at]=[str(d/n) for n in ['assets.obj','prskit.obj','game_texture.obj']]+['-Wl,/export:'+n for n in ['texture_load_pvm','texture_load_prs_file','prskit_decompress']]
    link[link.index('-o')+1]=str(d/'frames/jp_native_frames.dll');commands.append(link);results=[]
    for i,command in enumerate(commands):
        run=subprocess.run(command,cwd=r,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=command,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6500]
    for n,h in kept.items():assert sha(Path(n))==h,n
    authored=dict(b['authored_source_sha256'])
    for path in [p/'native'/n for n in ['jp_native_assets.h','jp_native_assets.c','jp_native_texture_move_owner.c']]+[Path(__file__).resolve()]:authored[path.relative_to(r).as_posix()]=sha(path)
    result=dict(status='native_asset_file_PRS_original_texture_wrapper_compiled_behavior_unverified',exit_code=0,diagnostic_bytes=0,commands=results,compiler_sha256=sha(a.compiler),authored_source_sha256=authored,vendored_MIT_source_sha256=vendor_files,
        external_source_sha256=dict(b['external_source_sha256'],**{'src/system/texture.c':sha(u/'src/system/texture.c')}),upstream_commit=b['upstream_commit'],
        generated_source_sha256={game.name:sha(game)},production_objects_sha256={n:sha(d/n) for n in ['textures_move.obj','assets.obj','prskit.obj','game_texture.obj']},kept_frame_owner_objects_sha256=kept,replaced_texture_owner_objects_sha256=replaced,
        original_game_wrapper_sha256=original_wrapper_sha256,adapted_game_wrapper_sha256=hashlib.sha256(wrapper).hexdigest(),original_512_entry_scratch_architecture_retained=True,plain_handle_copy_replaced_by_checked_binding_move=True,
        frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),DLL=b['DLL'],DLL_sha256=b['DLL_sha256'],GPU_DLL=b['GPU_DLL'],GPU_DLL_sha256=b['GPU_DLL_sha256'],
        baseline=str(base),baseline_report_sha256=sha(base/'build_verification.json'),single_native_record_memory_frame_queue_video_device_owner=True,
        private_game_or_SDK_bodies_publishable=False,MIT_PRS_codec_publishable_with_notice=True,retail_assets_published=False,original_source_and_proofs_modified=False,full_game_loop_executed=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
