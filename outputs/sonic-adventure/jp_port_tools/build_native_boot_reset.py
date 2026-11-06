"""Link original boot/HUD/progress/event/stage resets into the shared frame owner."""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
from build_native_read_queue import PIN

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

FAMILIES={
    'src/object/common.c':('boot_finish','menu_get_mode','menu_get_save'),
    'src/hud/hud.c':('hud_reset','lives_reset','lives_count','play_time_reset','time_reset','time_start','time_stop','play_time_get'),
    'src/progress/progress.c':('progress_init','playable_set'),
    'src/event/play.c':('event_init',),
    'src/stage/stage.c':('stage_begin','stage_group'),
    'src/collision/world.c':('world_reset_gravity',),
}
DECLARATIONS={
    'src/object/common.c':'menu_state menu_saves npc_task',
    'src/stage/stage.c':'trial_count time_frames time_seconds time_minutes time_steps stage_current stage_mode stage_end_kind pause_choice pause_hidden return_point world_gravity world_gravity_ang_x world_gravity_ang_z player_record',
    'src/hud/hud.c':'time_running',
    'src/progress/progress.c':'dat_8c7a2d20 dat_8c7a2d30 dat_8c7a2b20 dat_8c7a2f30 story_current_script story_event_delay',
    'src/event/play.c':'event_module event_controlling module_loaded dat_8c751dfe event_current',
    'src/system/util.c':'messages_shown',
    'src/progress/movie.c':'movie_request',
}
SHARED='rings lives play_time character_current saves save_slot story_records stage_clears event_flags events_played'.split()
POINTERS=set('npc_task return_point player_record story_current_script event_module event_controlling'.split())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'));p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256']
    link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll'))
    kept={x:sha(x) for x in link if x.endswith('.obj')};symbols={}
    for obj in kept:
        for s in coff_symbols(Path(obj)):
            if s['section']>0:symbols.setdefault(s['name'],[]).append(obj)
    names=[n for names in FAMILIES.values() for n in names]
    fields=[n for names in DECLARATIONS.values() for n in names.split()]
    assert len(names)==17 and len(set(fields))==len(fields)
    assert not (set(names+fields+['fill_bytes'])&set(symbols)),set(names+fields+['fill_bytes'])&set(symbols)
    assert all(len(symbols.get(n,[]))==1 for n in SHARED+['get_character','ring_reset'])
    recorder=next(Path(x) for x in kept if Path(x).name=='recorder.obj')
    recorder_build=recorder.parent/'build_verification.json';rb=read(recorder_build)
    recorder_source=recorder.parent/'external_recorder.c'
    assert sha(recorder_source)==rb['generated_sources_sha256']['external_recorder.c']
    before=recorder_source.read_bytes();old_decl=b'static Sint16 stage_end_kind = STAGE_END_NONE;'
    assert before.count(old_decl)==1
    rebound=before.replace(old_decl,b'Sint16 stage_end_kind = STAGE_END_NONE;')
    del kept[str(recorder)]
    originals={}
    def source(n):
        if n not in originals:
            raw=(u/n).read_bytes();assert raw==subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n]);originals[n]=raw
        return originals[n]
    declarations=[];static=set()
    for src,names_in_source in DECLARATIONS.items():
        raw=source(src)
        for name in names_in_source.split():
            found=re.findall(rb'^(?:static )?[A-Za-z_][^;\r\n]*\b'+name.encode()+rb'(?:\[[^;\r\n]*\])?(?:\s*=\s*[^;\r\n]+)?;\r?\n',raw,re.M)
            assert len(found)==1,(name,found)
            declarations.append(b'extern Sint16 stage_end_kind;\n' if name=='stage_end_kind' else found[0])
            if found[0].startswith(b'static ') and name!='stage_end_kind':static.add(name)
    bodies={n:function(source(src),n) for src,names_in_source in FAMILIES.items() for n in names_in_source}
    old=b'for (byte = dat_8c7a2d20, i = sizeof(story_records); i != 0;'
    new=b'for (byte = dat_8c7a2d20, i = sizeof(dat_8c7a2d20); i != 0;'
    assert bodies['progress_init'].count(old)==1
    native=dict(bodies);native['progress_init']=native['progress_init'].replace(old,new)
    # The original clears all 512 bytes of dat_8c7a2d30 later in this same
    # function. Bound the preceding 16-byte array write instead of depending
    # on original contiguous BSS allocation across distinct native C arrays.
    assert b'for (byte = dat_8c7a2d30, i = sizeof(dat_8c7a2d30); i != 0;' in bodies['progress_init']
    prefix=b'''#include <ninja.h>
#include <sg_xpt.h>
#include <shinobi.h>
#include <string.h>
#include "hud/hud.h"
#include "hud/overlay.h"
#include "save/card.h"
#include "object/common.h"
#include "progress/progress.h"
#include "progress/playable.h"
#include "progress/movie.h"
#include "stage/stage.h"
#include "collision/world.h"
#include "event/play.h"
#include "system/math.h"
#include "system/util.h"
'''
    enum=re.search(rb'enum stage_mode \{.*?\};',source('src/stage/stage.c'),re.S).group()
    field_names=fields+SHARED
    diagnostic=b'\n/* Private diagnostic field views; never replace original callees. */\n__declspec(dllexport) void *jp_boot_reset_field(unsigned i)\n{switch(i){\n'
    for i,n in enumerate(field_names):diagnostic+=f'case {i}:return &{n};\n'.encode()
    diagnostic+=b'default:return NULL;}}\n__declspec(dllexport) unsigned jp_boot_reset_field_bytes(unsigned i)\n{switch(i){\n'
    for i,n in enumerate(field_names):diagnostic+=f'case {i}:return sizeof({n});\n'.encode()
    diagnostic+=b'default:return 0;}}\n_Static_assert(sizeof(struct menu_state)==18&&sizeof(NJS_VECTOR)==12,"original pointer-free scalar layouts");\n'
    d.mkdir()
    for path in base.iterdir():
        if path.is_file() and path.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(path,d/path.name)
        elif path.is_dir() and path.name in ('include','history_include','game_init_imports'):shutil.copytree(path,d/path.name)
    unit=d/'external_boot_reset.c';unit.write_bytes(prefix+enum+b'\n'+b'\n'.join(declarations)+b'\n'+b'\n'.join(native.values())+diagnostic)
    rebound_source=d/'external_recorder_shared_stage.c';rebound_source.write_bytes(rebound)
    source('src/system/math.src');source('src/system/math.h')
    compiler=b['commands'][0]['command'][0]
    common=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffreestanding','-fno-builtin','-fno-strict-aliasing','-fwrapv','-ffp-contract=off','-Wall','-Wextra','-Werror','-Wno-unused-parameter','-Wno-unknown-pragmas','-Wno-ignored-qualifiers','-include',str(p/'native/jp_host_types.h'),'-I'+str(p/'native'),*['-I'+str(u/n) for n in ('sdk/include','src','modules','events','.')]]
    commands=[common+['-c',str(unit),'-o',str(d/'boot_reset_original.obj')],common+['-c',str(p/'native/jp_native_fill_bytes.c'),'-o',str(d/'fill_bytes_native.obj')],common+['-c',str(rebound_source),'-o',str(d/'recorder_shared_stage.obj')],common+['-S','-emit-llvm',str(unit),'-o',str(d/'boot_reset_original.ll')]]
    assert link.count(str(recorder))==1;link[link.index(str(recorder))]=str(d/'recorder_shared_stage.obj')
    link[link.index('-o'):link.index('-o')]=[str(d/'boot_reset_original.obj'),str(d/'fill_bytes_native.obj')]+['-Wl,/export:'+n for n in names+['fill_bytes']]+[v for n in fields if n not in static for v in ('-Xlinker','/export:'+n+',DATA')]
    link[-1]=str(d/'jp_native_frames.dll');commands.append(link);results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'boot_reset_build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()[:6500]
    assert not re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp|fptosi|fptoui|sitofp|uitofp)\b',(d/'boot_reset_original.ll').read_text())
    for n,h in kept.items():assert sha(n)==h,n
    updates={}
    for k,v in b.items():
        if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):updates[k]=str(d/Path(v).name);updates[k+'_sha256']=sha(updates[k])
    authored=[p/'native/jp_native_fill_bytes.c',Path(__file__).resolve()]
    updates.update(status='original_boot_HUD_progress_event_and_stage_reset_dependencies_linked_unverified',
        boot_reset_baseline_sha256=sha(base/'build_verification.json'),boot_reset_original_source_sha256={n:hashlib.sha256(raw).hexdigest() for n,raw in originals.items()},
        boot_reset_original_body_sha256={n:hashlib.sha256(raw).hexdigest() for n,raw in bodies.items()},boot_reset_native_body_sha256={n:hashlib.sha256(raw).hexdigest() for n,raw in native.items()},
        boot_reset_fields=[dict(id=i,name=n,pointer=n in POINTERS,static=n in static,shared_prior=n in SHARED) for i,n in enumerate(field_names)],
        boot_reset_generated_source_sha256={'external_boot_reset.c':sha(unit),'external_recorder_shared_stage.c':sha(rebound_source)},boot_reset_kept_frame_objects_sha256=kept,
        boot_reset_prior_symbol_providers={n:symbols[n] for n in SHARED+['get_character','ring_reset']},
        boot_reset_current_symbol_providers={n:([str(d/'recorder_shared_stage.obj')] if n in ('character_current','get_character') else symbols[n]) for n in SHARED+['get_character','ring_reset']},
        boot_reset_recorder_rebinding=dict(original_object=str(recorder),original_object_sha256=sha(recorder),original_source=str(recorder_source),original_source_sha256=sha(recorder_source),original_build_sha256=sha(recorder_build),only_static_declaration_promoted_to_shared_linkage=True),
        boot_reset_adaptations=['Bound the first progress scratch-array clear to its actual 16-byte extent; the original later 512-byte clear preserves the complete post-call output.','Recover fill_bytes backward low-byte writes from original SH-4 assembly; zero, negative or null requests are safe no-ops outside the original positive-count contract.'],
        boot_reset_new_build_commands=len(results),commands=results+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),
        full_GAME_MODE_BOOT_or_gameplay_executed=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],'17 original functions and native fill_bytes',flush=True)

if __name__=='__main__':main()
