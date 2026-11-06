"""Bind original set_character and genuine Chao reset to qualified providers."""
import argparse,hashlib,json,re,shlex,shutil,subprocess
from pathlib import Path
from build_native_read_queue import PIN,owner_function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'));p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    frame=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'))
    motion=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name==Path(b['motion_DLL']).name))
    kept={x:sha(x) for x in frame if x.endswith('.obj')};symbols={}
    for obj in kept:
        for s in coff_symbols(Path(obj)):
            if s['section']>0:symbols.setdefault(s['name'],[]).append(obj)
    assert all(len(symbols.get(n,[]))==1 for n in ('character_current','lives','lives_reset','lives_count','saves','save_slot'))
    original={}
    for n in ('src/stage/stage.c','src/chao/chao.c','src/system/loader.c'):
        raw=(u/n).read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n]);original[n]=raw
    chao=original['src/chao/chao.c'];functions=('chao_clear_carried','chao_set_carried','chao_get_carried','chao_set_carried_to','chao_get_carried_to','chao_get_records','chao_get_file_data','chao_clear_records','chao_reset')
    fields=('records_cleared','carried_to','carried','chao_file_data','chao_records','icon_count','icon_row_request','animal_icons','dat_8c6c4618','dat_8c6c4619')
    assert not set(functions+fields+('set_character',))&set(symbols)
    decl=[]
    for n in fields:
        matches=re.findall(rb'^static [^;\r\n]*\b'+n.encode()+rb'(?:\[[^;\r\n]*\])?;\r?\n',chao,re.M);assert len(matches)==1,(n,matches);decl.append(matches[0])
    bodies={n:owner_function(chao,n) for n in functions};stage=owner_function(original['src/stage/stage.c'],'set_character')
    tail=stage[stage.index(b'    index = character_current;',stage.index(b'    texture_load_character')):stage.rfind(b'}')]
    assert tail.startswith(b'    index = character_current;') and tail.rstrip().endswith(b'chao_reset();')
    chao_prefix=b'''#include <string.h>
#include "chao/chao.h"
#define ANIMAL_ICONS 10
'''+re.search(rb'struct animal_icon \{.*?\};',chao,re.S).group()+b'\n'
    diagnostic=b'\n__declspec(dllexport) void *jp_character_chao_field(unsigned i){switch(i){\n'
    for i,n in enumerate(fields):diagnostic+=f'case {i}:return &{n};\n'.encode()
    diagnostic+=b'default:return NULL;}}\n__declspec(dllexport) size_t jp_character_chao_field_bytes(unsigned i){switch(i){\n'
    for i,n in enumerate(fields):diagnostic+=f'case {i}:return sizeof({n});\n'.encode()
    diagnostic+=b'default:return 0;}}\n'
    d.mkdir()
    for path in base.iterdir():
        if path.is_file() and path.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(path,d/path.name)
        elif path.is_dir() and path.name in ('include','history_include','game_init_imports','motion_view'):shutil.copytree(path,d/path.name)
    (d/'external_character_chao.c').write_bytes(chao_prefix+b'\n'.join(decl)+b'\n'+b'\n'.join(bodies.values())+diagnostic)
    raw=(base/'character_textures_with_exit.inc').read_text().encode();changes=[]
    def rep(old,new,label,count=1):
        nonlocal raw
        old,new=old.encode(),new.encode();assert raw.count(old)==count,(label,raw.count(old));raw=raw.replace(old,new);changes.append(label)
    def scope(name,old,new,label):
        nonlocal raw
        body=owner_function(raw,name);before,after=old.encode(),new.encode();assert body.count(before)==1,(label,body.count(before));raw=raw.replace(body,body.replace(before,after));changes.append(label)
    rep('/* Included after genuine data, tables and guarded original function bodies. */','#include "jp_native_character_switch.h"\n/* Same owner also retains its game texture reference and switch barrier. */','declare same-owner switch metadata')
    rep('int active,busy,partial_releasing;','int active,busy,partial_releasing;int switch_held,switch_call;struct jp_character_switch_info switching;','embed game reference and switch state')
    rep('character_owner={.info.partial_character=-1};','character_owner={.info.partial_character=-1,.switching={.requested=-1,.owned_character=-1}};','empty game reference has no character')
    rep('character_owner.busy?JP_CHARACTER_TEXTURE_REENTRANT:','(character_owner.busy||(character_owner.switch_held&&!character_owner.switch_call))?JP_CHARACTER_TEXTURE_REENTRANT:','reject unrelated calls while original sequence runs')
    rep('character_owner.thread=GetCurrentThreadId();character_owner.active=1;','memset(&character_owner.switching,0,sizeof character_owner.switching);character_owner.switching.requested=character_owner.switching.owned_character=-1;character_owner.switch_held=character_owner.switch_call=0;\n    character_owner.thread=GetCurrentThreadId();character_owner.active=1;','fresh lifetime discards abandoned operation metadata')
    scope('jp_character_textures_release','if(character_owner.info.phase!=1)','if(!character_owner.switch_call&&character_owner.switching.owned_character==(int32_t)character&&character_users[character]==1)return character_result(JP_CHARACTER_TEXTURE_STATE,0);\n    if(character_owner.info.phase!=1)','public release cannot steal game reference')
    scope('jp_character_textures_release_all','if(character_owner.info.phase!=1)','if(character_owner.switching.owned_character>=0)return character_result(JP_CHARACTER_TEXTURE_STATE,0);if(character_owner.info.phase!=1)','public release_all cannot steal game reference')
    scope('jp_character_textures_recover','character_owner.info.partial_character=-1;character_owner.info.phase=1;','if(character_owner.switching.owned_character>=0&&!character_users[character_owner.switching.owned_character])character_owner.switching.owned_character=-1;\n    character_owner.info.partial_character=-1;character_owner.info.phase=1;','actual failed release recovery reconciles game reference')
    rep('character_owner.busy?JP_CHARACTER_EXIT_REENTRANT:0;','(character_owner.busy||character_owner.switch_held)?JP_CHARACTER_EXIT_REENTRANT:0;','parent preflight sees original sequence barrier')
    (d/'character_textures_with_switch.inc').write_bytes(raw)
    family=(base/'character_family_with_exit.c').read_text().encode();anchor=b'#include "character_textures_with_exit.inc"';assert family.count(anchor)==1
    prefix=b'''#include "save/save.h"
#include "save/card.h"
#include "stage/stage.h"
#include "hud/hud.h"
#define JP_MOTION_API __declspec(dllimport)
#include "jp_native_motion_store.h"
__declspec(dllimport) int jp_motions_can_select(int32_t);
void chao_reset(void);
static int switch_release(int);
static int switch_motion(int);
static int switch_load(int);
#define set_character jp_original_set_character
#define texture_release_character(c) do {if(switch_release(c))return;} while(0)
#define loader_read_character_motions(c) do {if(switch_motion(c))return;} while(0)
#define texture_load_character(c) do {if(switch_load(c))return;} while(0)
'''
    suffix=b'''\n#undef set_character
#undef texture_release_character
#undef loader_read_character_motions
#undef texture_load_character
static void jp_original_character_finish(void){int index;
'''+tail+b'}\n#include "jp_native_character_switch.inc"\n'
    family=family.replace(anchor,b'#include "character_textures_with_switch.inc"')+b'\n'+prefix+stage+suffix;(d/'character_family_with_switch.c').write_bytes(family)
    raw=(base/'motion_store_with_exit.c').read_bytes()+(p/'native/jp_native_motion_select.inc').read_bytes();(d/'motion_store_with_select.c').write_bytes(raw)
    compiler=b['commands'][0]['command'][0]
    char_row=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][-1]).name=='character_exit_owner.obj');sdk=char_row[:char_row.index('-c')]+['-I'+str(u/'src')]
    motion_row=next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][-1]).name=='motion_exit_owner.obj');host=motion_row[:motion_row.index('-c')]
    commands=[host+['-c',str(d/'motion_store_with_select.c'),'-o',str(d/'motion_select_owner.obj')]]
    old_motion=next(x for x in motion if x.endswith('.obj') and Path(x).name=='motion_exit_owner.obj');motion[motion.index(old_motion)]=str(d/'motion_select_owner.obj');motion[-1]=str(d/Path(b['motion_DLL']).name);commands.append(motion)
    commands += [sdk+['-c',str(d/name),'-o',str(d/obj)] for name,obj in [('external_character_chao.c','character_chao_original.obj'),('character_family_with_switch.c','character_switch_owner.obj')]]
    old_character=next(x for x in frame if x.endswith('.obj') and Path(x).name=='character_exit_owner.obj');frame[frame.index(old_character)]=str(d/'character_switch_owner.obj');del kept[old_character]
    extra=[str(d/'character_chao_original.obj'),str(d/Path(b['motion_DLL']).with_suffix('.lib').name)]
    exports=['jp_character_chao_field','jp_character_chao_field_bytes','chao_get_records','chao_get_file_data','chao_get_carried','chao_get_carried_to','chao_set_carried','chao_set_carried_to']
    frame[frame.index('-o'):frame.index('-o')]=extra+['-Wl,/export:'+n for n in exports];frame[-1]=str(d/'jp_native_frames.dll');commands.append(frame);results=[]
    for i,cmd in enumerate(commands):
        executed=cmd;response={}
        if len(subprocess.list2cmdline(cmd))>=30000:
            content='\n'.join('"'+arg.replace('\\','\\\\').replace('"','\\"')+'"' for arg in cmd[1:])+'\n';assert shlex.split(content)==cmd[1:]
            path=d/f'character_switch_build_{i}.rsp';path.write_text(content);executed=[cmd[0],'@'+str(path)];response=dict(response_file=str(path),response_file_sha256=sha(path))
        run=subprocess.run(executed,capture_output=True);log=d/f'character_switch_build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,executed_command=executed,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,**response));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()[:7000]
    for n,h in kept.items():assert sha(n)==h,n
    updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
    authored=[p/'native'/n for n in ('jp_native_character_switch.h','jp_native_character_switch.inc','jp_native_motion_select.inc')]+[Path(__file__).resolve()]
    generated=['external_character_chao.c','character_textures_with_switch.inc','character_family_with_switch.c','motion_store_with_select.c']
    updates.update(status='original_set_character_genuine_Chao_reset_shared_native_owners_and_checked_partial_continuation_linked_unverified',character_switch_baseline_sha256=sha(base/'build_verification.json'),character_switch_original_source_sha256={n:hashlib.sha256(raw).hexdigest() for n,raw in original.items()},character_switch_original_body_sha256={n:hashlib.sha256(body).hexdigest() for n,body in dict(bodies,set_character=stage).items()},character_switch_original_tail_sha256=hashlib.sha256(tail).hexdigest(),character_switch_generated_source_sha256={n:sha(d/n) for n in generated},character_switch_kept_frame_objects_sha256=kept,character_switch_shared_symbol_providers={n:symbols[n] for n in ('character_current','lives','lives_reset','lives_count','saves','save_slot')},character_switch_Chao_fields=list(fields),character_switch_adaptations=changes,character_switch_new_build_commands=len(results),commands=results+b['commands'],authored_source_sha256=dict(b['authored_source_sha256'],**{n.relative_to(r).as_posix():sha(n) for n in authored}),original_set_character_or_Chao_reset_runtime_verified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],len(results),'clean commands',flush=True)
if __name__=='__main__':main()
