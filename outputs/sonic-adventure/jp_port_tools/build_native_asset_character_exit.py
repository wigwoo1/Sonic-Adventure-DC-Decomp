"""Delegate the actual shared asset/character providers through existing parents."""
import argparse,hashlib,json,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import owner_function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,base,d=(getattr(a,n).resolve() for n in ('workspace','baseline','output'));p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for key in ('frames_DLL','System_DLL','authored_exit_DLL','main_boot_DLL'):assert sha(b[key])==b[key+'_sha256']
    d.mkdir()
    for path in base.iterdir():
        if path.is_file() and path.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(path,d/path.name)
        elif path.is_dir() and path.name in ('include','history_include','game_init_imports','motion_view'):shutil.copytree(path,d/path.name)
    frame_link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'))
    old_objects={name:sha(name) for name in frame_link if name.endswith('.obj')};replaced={};recipes={};original_sources={};changes=[]
    def text(path):return Path(path).read_text(encoding='utf-8-sig').encode()
    def rep(raw,old,new,label,count=1):
        old,new=old.encode(),new.encode();assert raw.count(old)==count,(label,raw.count(old));changes.append(dict(reason=label,count=count,before_sha256=hashlib.sha256(old).hexdigest(),after_sha256=hashlib.sha256(new).hexdigest()));return raw.replace(old,new)
    def scoped(raw,name,old,new,label):
        body=owner_function(raw,name);return raw.replace(body,rep(body,old,new,label))
    for name in ('assets.obj','memory_pvm.obj'):
        obj=next(x for x in old_objects if Path(x).name==name);meta=Path(obj).parent/'build_verification.json';v=read(meta)
        row=next(x['command'] for x in v['commands'] if '-c' in x['command'] and Path(x['command'][-1]).name==name);source=Path(row[row.index('-c')+1]);assert sha(source) in json.dumps(v)
        recipes[name]=row[:row.index('-c')];replaced[name]=obj;original_sources[source.as_posix()]=sha(source)
        if name=='assets.obj':
            assert source==p/'native/jp_native_assets.c';raw=text(source)
            raw=rep(raw,'#include "jp_native_assets.h"','#include "jp_native_assets.h"\n#include "jp_native_asset_exit.h"\n#include "jp_native_character_exit.h"','same provider exposes explicit authority')
            raw=rep(raw,'NJS_TEXLIST pending;DWORD owner;int initialized,busy;','NJS_TEXLIST pending;DWORD owner;int initialized,busy;uint64_t exit_token;DWORD claim_thread;uint32_t exit_terminal,exit_closing,exit_finished,exit_call;','capability fields live in actual asset owner')
            raw=rep(raw,'static int asset_owner(void){return !assets.initialized?JP_ASSET_STATE:assets.owner!=GetCurrentThreadId()?JP_ASSET_THREAD:JP_ASSET_OK;}',
                'static DWORD asset_exit_thread(void);\nstatic void asset_clear_retaining_claim(void);\nstatic int asset_owner(void){DWORD thread=asset_exit_thread();return thread&&thread!=GetCurrentThreadId()?JP_ASSET_THREAD:!assets.initialized?JP_ASSET_STATE:((assets.exit_closing||assets.exit_terminal)&&!assets.exit_call)?JP_ASSET_STATE:JP_ASSET_OK;}','retain creator and reject consumers after claimed retirement starts')
            raw=scoped(raw,'jp_assets_initialize','    if(assets.initialized)',
                '    DWORD thread=asset_exit_thread();if(thread&&thread!=GetCurrentThreadId())return JP_ASSET_THREAD;\n    if(assets.exit_token||assets.exit_terminal||assets.exit_closing)return JP_ASSET_STATE;\n    if(assets.initialized)','independent initialization cannot steal delegated/terminal assets')
            raw=scoped(raw,'jp_assets_shutdown','    int rc=asset_ready();if(rc)return rc;',
                '    DWORD thread=asset_exit_thread();if(thread&&thread!=GetCurrentThreadId())return JP_ASSET_THREAD;\n    if((assets.exit_token||assets.exit_terminal||assets.exit_closing)&&!assets.exit_call)return JP_ASSET_STATE;\n    if(!assets.exit_call){struct jp_character_exit_info child;int e=jp_character_exit_snapshot(&child);if(e||child.active)return asset_fail(JP_ASSET_STATE);}\n    int rc=asset_ready();if(rc)return rc;','independent shutdown cannot invalidate claimed assets or a real live child')
            raw=scoped(raw,'jp_assets_shutdown','memset(&assets,0,sizeof assets);','asset_clear_retaining_claim();','actual freeing preserves exact completed capability for parent retry')
            raw+=text(p/'native/jp_native_asset_exit.inc');(d/'assets_with_exit.c').write_bytes(raw)
        else:
            raw=text(source)+b'\n__declspec(dllexport) int jp_asset_loader_activity(void){return source!=NULL||pvm.list!=NULL||pvm.busy;}\n';(d/'memory_asset_activity.c').write_bytes(raw)
    old_family=next(x for x in old_objects if Path(x).name=='external_character_texture_family.obj');replaced['character']=old_family
    source=p/'native/jp_native_character_textures.inc';original_sources[source.as_posix()]=sha(source);raw=text(source)
    raw=rep(raw,'int active,busy,partial_releasing;','int active,busy,partial_releasing;uint64_t exit_token;uint32_t exit_terminal,exit_closing,exit_finished,exit_call;','character capability in same original reference owner')
    raw=rep(raw,'static int character_result','static DWORD character_exit_thread(void);\nstatic int character_result','same retained creator declaration')
    raw=rep(raw,'return character_owner.thread&&character_owner.thread!=GetCurrentThreadId()?JP_CHARACTER_TEXTURE_THREAD:character_owner.busy?JP_CHARACTER_TEXTURE_REENTRANT:!character_owner.active?JP_CHARACTER_TEXTURE_STATE:0;',
        'DWORD thread=character_exit_thread();return thread&&thread!=GetCurrentThreadId()?JP_CHARACTER_TEXTURE_THREAD:character_owner.busy?JP_CHARACTER_TEXTURE_REENTRANT:((character_owner.exit_closing||character_owner.exit_terminal)&&!character_owner.exit_call)?JP_CHARACTER_TEXTURE_STATE:!character_owner.active?JP_CHARACTER_TEXTURE_STATE:0;','block late consumers after retirement')
    raw=rep(raw,'if(character_owner.thread&&character_owner.thread!=GetCurrentThreadId())return JP_CHARACTER_TEXTURE_THREAD;',
        'DWORD thread=character_exit_thread();if(thread&&thread!=GetCurrentThreadId())return JP_CHARACTER_TEXTURE_THREAD;','retained creator for initialize and snapshot',2)
    raw=scoped(raw,'jp_character_textures_initialize','if(character_owner.active)',
        'if(character_owner.exit_token||character_owner.exit_terminal||character_owner.exit_closing)return character_result(JP_CHARACTER_TEXTURE_STATE,0);if(character_owner.active)','no independent delegated character replacement')
    raw=scoped(raw,'jp_character_textures_shutdown','if(character_owner.info.phase!=1)',
        'if((character_owner.exit_token||character_owner.exit_terminal||character_owner.exit_closing)&&!character_owner.exit_call)return character_result(JP_CHARACTER_TEXTURE_STATE,0);if(character_owner.info.phase!=1)','claimed character shutdown requires actual authority')
    raw+=text(p/'native/jp_native_character_exit.inc');(d/'character_textures_with_exit.inc').write_bytes(raw)
    raw=text(base/'external_character_texture_family.c');raw=rep(raw,'#include "jp_native_character_textures.h"','#include "jp_native_character_textures.h"\n#include "jp_native_asset_exit.h"\n#include "jp_native_character_exit.h"','same family imports its own lifecycle types')
    raw=rep(raw,'#include "jp_native_character_textures.inc"','#include "character_textures_with_exit.inc"','existing original family uses derived same owner');(d/'character_family_with_exit.c').write_bytes(raw)
    imports='#define JP_ASSET_EXIT_API __declspec(dllimport)\n#define JP_CHARACTER_EXIT_API __declspec(dllimport)\n'
    raw=text(base/'system_with_motions.c');raw=rep(raw,'/* Exact input ABI,',imports+'#define JP_EXIT_ASSETS_API __declspec(dllimport)\n#include "jp_native_system_assets.h"\n#include "jp_native_exit_assets.h"\n__declspec(dllimport) int jp_assets_initialize(const wchar_t *,const struct jp_asset_limits *);\n__declspec(dllimport) int jp_character_textures_initialize(void);\n/* Exact input ABI,','same System imports real asset/character authorities')
    raw=rep(raw,'game_init_token,motion_token;','game_init_token,motion_token,asset_token,character_token;','existing System retains exact asset and character tokens')
    raw=rep(raw,'static int shutdown_owned(void)','static int system_assets_activity(int);\nstatic int system_assets_close(void);\nstatic int system_characters_close(void);\nstatic int system_assets_release(void);\nstatic int shutdown_owned(void)','same owner helper declarations')
    raw=rep(raw,'    CLOSE(JP_EXIT_MOTIONS,','    CLOSE(JP_EXIT_CHARACTER_TEXTURES,system_characters_close());\n    CLOSE(JP_EXIT_ASSETS,system_assets_close());\n    CLOSE(JP_EXIT_MOTIONS,','actual child then assets retire before motion and card callbacks')
    raw=rep(raw,'    system_owner.info.window=0;system_owner.info.phase=',
        '    if(system_owner.terminal&&system_owner.character_token){int e=jp_character_exit_finish(system_owner.character_token);if(e)return result(JP_SYSTEM_CLEANUP,JP_EXIT_CHARACTER_TEXTURES,e);}\n    if(system_owner.terminal&&system_owner.asset_token){int e=jp_assets_exit_finish(system_owner.asset_token);if(e)return result(JP_SYSTEM_CLEANUP,JP_EXIT_ASSETS,e);}\n    system_owner.info.window=0;system_owner.info.phase=','terminalize already ordinary-closed actual children on later global retry')
    for name,retire in (('jp_system_shutdown',1),('jp_system_bind_exit',0)):
        raw=scoped(raw,name,'    int motion_active=system_motion_activity(',f'    int assets_active=system_assets_activity({retire});if(assets_active)return assets_active;\n    int motion_active=system_motion_activity(','preflight before any parent cleanup or handoff: '+name)
    raw=scoped(raw,'jp_system_bind_exit','((system_owner.info.resources&~resources)&JP_EXIT_MOTIONS)?',
        '((system_owner.info.resources&~resources)&JP_EXIT_CHARACTER_TEXTURES)?JP_EXIT_CHARACTER_TEXTURES:((system_owner.info.resources&~resources)&JP_EXIT_ASSETS)?JP_EXIT_ASSETS:((system_owner.info.resources&~resources)&JP_EXIT_MOTIONS)?','accurate omitted child/asset reporting')
    raw=rep(raw,'if(!system_owner.terminal){rc=system_motion_release();','if(!system_owner.terminal){rc=system_assets_release();if(rc)return rc;rc=system_motion_release();','ordinary capabilities cancel only after complete parent teardown')
    raw=scoped(raw,'jp_system_motion_cleanup_activity','    return system_motion_activity(1);','    rc=system_assets_activity(1);return rc?rc:system_motion_activity(1);','existing lower MainBoot preflight covers all actual live child consumers')
    raw=scoped(raw,'jp_system_prepare_boot_shutdown','    if(!(system_owner.info.resources&JP_EXIT_MOTIONS)&&!system_owner.motion_token)',
        '    if(!(system_owner.info.resources&(JP_EXIT_MOTIONS|JP_EXIT_ASSETS|JP_EXIT_CHARACTER_TEXTURES))&&!system_owner.motion_token&&!system_owner.asset_token&&!system_owner.character_token)','asset/character-only parent also closes before original card path')
    raw=scoped(raw,'jp_system_prepare_boot_shutdown','    uint32_t owned=',
        '    uint32_t features=(system_owner.motion_token?JP_EXIT_MOTIONS:0)|(system_owner.asset_token?JP_EXIT_ASSETS:0)|(system_owner.character_token?JP_EXIT_CHARACTER_TEXTURES:0)|(system_owner.info.resources&(JP_EXIT_MOTIONS|JP_EXIT_ASSETS|JP_EXIT_CHARACTER_TEXTURES));\n    uint32_t owned=','report actual owned feature bits without inventing a motion resource')
    raw=scoped(raw,'jp_system_prepare_boot_shutdown','*retired=owned|JP_EXIT_MOTIONS;','*retired=owned|features;','lower parent clears only genuinely ordered ownership')
    raw+=text(p/'native/jp_native_system_assets.inc');(d/'system_with_assets.c').write_bytes(raw)
    raw=text(base/'exit_with_motions.c');raw=rep(raw,'#define JP_EXIT_BOOT_SUPPORTED',imports+'#include "jp_native_exit_assets.h"\n#define JP_EXIT_BOOT_SUPPORTED','Exit imports same actual providers without System dependency')
    raw=rep(raw,'(JP_EXIT_AUDIO_SUPPORTED|JP_EXIT_MAIN_LIFETIMES|JP_EXIT_HISTORY|JP_EXIT_READ_QUEUE|JP_EXIT_MOTIONS)',
        '(JP_EXIT_AUDIO_SUPPORTED|JP_EXIT_MAIN_LIFETIMES|JP_EXIT_HISTORY|JP_EXIT_READ_QUEUE|JP_EXIT_MOTIONS|JP_EXIT_ASSETS|JP_EXIT_CHARACTER_TEXTURES)','reuse existing ASSETS bit and add explicit character owner')
    raw=rep(raw,'history_token,read_queue_token,motion_token;','history_token,read_queue_token,motion_token,asset_token,character_token;','same bootstrap/final instances retain exact child capabilities',2)
    raw=rep(raw,'static int motion_retire_preflight(void);','static int asset_character_retire_preflight(void);\nstatic int motion_retire_preflight(void);','strict complete preflight declaration')
    raw=scoped(raw,'jp_exit_bind','    uint64_t motion_token=bootstrap.motion_token;',
        '    if((cfg->resources&JP_EXIT_CHARACTER_TEXTURES)&&(!bootstrap.character_token||!(cfg->resources&JP_EXIT_ASSETS)))return JP_EXIT_ARGUMENT;\n    if((bootstrap.resources&JP_EXIT_ASSETS)&&!bootstrap.asset_token)return JP_EXIT_ARGUMENT;\n    if((cfg->resources&JP_EXIT_ASSETS)&&!bootstrap.asset_token){struct jp_asset_exit_info asset;struct jp_character_exit_info child;\n        if(jp_assets_exit_snapshot(&asset)||asset.token||asset.terminal||jp_character_exit_snapshot(&child)||child.active)return JP_EXIT_ARGUMENT;}\n    uint64_t asset_token=bootstrap.asset_token,character_token=bootstrap.character_token;\n    uint64_t motion_token=bootstrap.motion_token;','final bind preserves independent/partial authority and requires the actual dependent asset bit')
    raw=rep(raw,'state.motion_token=motion_token;','state.motion_token=motion_token;state.asset_token=asset_token;state.character_token=character_token;','handoff only after all old final claims succeed')
    raw=scoped(raw,'boot_exit_activity','    uint32_t mask=state.bound?state.info.remaining:bootstrap.resources;',
        '    uint32_t mask=state.bound?state.info.remaining:bootstrap.resources;\n    uint64_t asset_token=state.bound?state.asset_token:bootstrap.asset_token,character_token=state.bound?state.character_token:bootstrap.character_token;\n    if(character_token||(mask&JP_EXIT_CHARACTER_TEXTURES)){int e=character_token?jp_character_exit_can_close(character_token):jp_character_exit_activity();if(e)return e==JP_CHARACTER_EXIT_THREAD?JP_EXIT_THREAD:e==JP_CHARACTER_EXIT_REENTRANT?JP_EXIT_REENTRANT:JP_EXIT_SERVICE;}\n    if(asset_token||(!state.bound&&(mask&JP_EXIT_ASSETS))){int e=asset_token?jp_assets_exit_can_close(asset_token):jp_assets_exit_activity();if(e)return e==JP_ASSET_EXIT_THREAD?JP_EXIT_THREAD:e==JP_ASSET_EXIT_REENTRANT?JP_EXIT_REENTRANT:JP_EXIT_SERVICE;}','identity/activity only permits live users and decoded views during handoff')
    raw=scoped(raw,'jp_exit_cleanup','    int motion_active=motion_retire_preflight();',
        '    int assets_active=asset_character_retire_preflight();if(assets_active)return assets_active;\n    int motion_active=motion_retire_preflight();','live child/decoded/loader users block before counters or bootstrap mutation')
    raw=rep(raw,'    CLOSE_INT(JP_EXIT_MOTIONS,',
        '    CLOSE_INT(JP_EXIT_CHARACTER_TEXTURES,jp_character_exit_finish(state.character_token));\n    if((state.info.remaining&JP_EXIT_ASSETS)&&state.asset_token){int rc=jp_assets_exit_finish(state.asset_token);if(rc)return failed(JP_EXIT_ASSETS,rc);complete(JP_EXIT_ASSETS);}\n    CLOSE_INT(JP_EXIT_MOTIONS,','claimed child/assets close before motion and card; legacy asset block unchanged')
    raw+=text(p/'native/jp_native_exit_assets.inc');(d/'exit_with_assets.c').write_bytes(raw)
    raw=text(base/'main_boot_with_motions.c');raw=rep(raw,'#include "jp_native_system_motion.h"','#include "jp_native_system_motion.h"\n#define JP_EXIT_API __declspec(dllimport)\n#include "jp_native_exit.h"\n#define JP_ASSET_EXIT_API __declspec(dllimport)\n#define JP_CHARACTER_EXIT_API __declspec(dllimport)\n#include "jp_native_system_assets.h"','lower MainBoot recognizes actual asset/character feature bits')
    raw=rep(raw,'if(retired&JP_EXIT_MOTIONS)boot.system_owned=0;','if(retired&(JP_EXIT_MOTIONS|JP_EXIT_ASSETS|JP_EXIT_CHARACTER_TEXTURES))boot.system_owned=0;','asset/character-only ordered System shutdown also clears actual parent flag');(d/'main_boot_with_assets.c').write_bytes(raw)
    compiler=b['commands'][0]['command'][0];host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
    family_row=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][-1]).name=='external_character_texture_family.obj');family_flags=family_row[:family_row.index('-c')]
    specs=[(d/'assets_with_exit.c','assets_exit_owner.obj',recipes['assets.obj']),(d/'memory_asset_activity.c','memory_asset_activity.obj',recipes['memory_pvm.obj']),(d/'character_family_with_exit.c','character_exit_owner.obj',family_flags)]
    commands=[flags+['-c',str(source),'-o',str(d/name)] for source,name,flags in specs]
    for old_name,new_name in [('assets.obj','assets_exit_owner.obj'),('memory_pvm.obj','memory_asset_activity.obj'),('character','character_exit_owner.obj')]:frame_link[frame_link.index(replaced[old_name])]=str(d/new_name)
    frame_link[-1]=str(d/'jp_native_frames.dll');commands.append(frame_link)
    for source,obj,key,old_obj in [('exit_with_assets.c','exit_assets_owner.obj','authored_exit_DLL','exit_motion_owner.obj'),('system_with_assets.c','system_assets_owner.obj','System_DLL','system_motion_owner.obj'),('main_boot_with_assets.c','main_boot_assets_owner.obj','main_boot_DLL','main_boot_motion_owner.obj')]:
        flags=host
        if key=='main_boot_DLL':
            row=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][x['command'].index('-c')+1]).name=='main_boot_with_motions.c');flags=row[:row.index('-c')]
        commands.append(flags+['-c',str(d/source),'-o',str(d/obj)])
        link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name==Path(b[key]).name));found=0
        for i,arg in enumerate(link):
            if arg.endswith('.obj') and Path(arg).name==old_obj:link[i]=str(d/obj);found+=1
            elif arg.endswith('.lib') and (d/Path(arg).name).exists():link[i]=str(d/Path(arg).name)
        assert found==1,(key,old_obj);link[-1]=str(d/Path(b[key]).name);commands.append(link)
    results=[]
    for i,cmd in enumerate(commands):
        executed=cmd;response={}
        if len(subprocess.list2cmdline(cmd))>=30000:
            content='\n'.join('"'+arg.replace('\\','\\\\').replace('"','\\"')+'"' for arg in cmd[1:])+'\n';assert shlex.split(content)==cmd[1:]
            path=d/f'asset_character_build_{i}.rsp';path.write_text(content);executed=[cmd[0],'@'+str(path)];response=dict(response_file=str(path),response_file_sha256=sha(path))
        run=subprocess.run(executed,capture_output=True);log=d/f'asset_character_build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,executed_command=executed,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,**response));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()[:6500]
    kept={name:h for name,h in old_objects.items() if name not in replaced.values()}
    for name,h in kept.items():assert sha(name)==h,name
    updates={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):updates[key]=str(d/Path(value).name);updates[key+'_sha256']=sha(updates[key])
    authored=[p/'native'/name for name in ('jp_native_asset_exit.h','jp_native_asset_exit.inc','jp_native_character_exit.h','jp_native_character_exit.inc','jp_native_system_assets.h','jp_native_system_assets.inc','jp_native_exit_assets.h','jp_native_exit_assets.inc')]+[Path(__file__).resolve()]
    generated=['assets_with_exit.c','memory_asset_activity.c','character_textures_with_exit.inc','character_family_with_exit.c','system_with_assets.c','exit_with_assets.c','main_boot_with_assets.c']
    updates.update(status='same_System_Exit_actual_asset_character_authority_decoded_and_logical_user_preflight_linked_unverified',asset_character_baseline_sha256=sha(base/'build_verification.json'),
        asset_character_original_source_sha256=original_sources,asset_character_generated_source_sha256={name:sha(d/name) for name in generated},asset_character_kept_frame_objects_sha256=kept,
        asset_character_replaced_objects_sha256={name:sha(path) for name,path in replaced.items()},asset_character_adaptations=changes,source_line_endings_normalized_in_private_derivations=True,
        asset_character_new_build_commands=len(results),commands=results+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{path.relative_to(r).as_posix():sha(path) for path in authored}),
        automatic_asset_character_ownership_verified=False,full_set_character_animation_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],len(results),'clean build commands',flush=True)
if __name__=='__main__':main()
