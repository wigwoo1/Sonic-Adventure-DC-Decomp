"""Genuine boot_init, complete reset/default/CRC family and one state owner.
Original game source, objects and resulting diagnostic DLLs stay private.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','logo','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,u,base,logo,c,d=a.workspace,a.upstream,a.baseline,a.logo,a.compiler,a.output;p=r/'outputs/sonic-adventure'
    b=read(base/'build_verification.json');lb=read(logo/'build_verification.json')
    assert lb['frame_build_sha256']==sha(base/'build_verification.json') and b['frame_wait_input_trace_replaced'] and c.exists()
    assert sha(c)==b['compiler_sha256'] and lb['original_logo_step_body_unchanged']
    for report in [b,lb]:
        for n,h in report['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in lb['generated_sources_sha256'].items():assert sha(logo/n)==h,n
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    d.mkdir();(d/'frames').mkdir();(d/'imports/progress').mkdir(parents=True);(d/'imports/save').mkdir(parents=True)
    raw={n:(u/n).read_bytes() for n in ['src/save/save.c','src/save/minigame.c','src/save/card.c','src/stage/stage.c','src/hud/hud.c','src/progress/demo.c']}
    def line(file,pattern):
        found=re.findall(pattern,raw[file],re.M);assert len(found)==1,(file,pattern);return found[0]
    state=b'\n'.join([line('src/save/save.c',rb'^struct save saves\[SAVE_COUNT\];'),line('src/save/card.c',rb'^int save_slot = 0;'),line('src/stage/stage.c',rb'^Sint16 rings;'),line('src/progress/demo.c',rb'^Bool demo_playing = FALSE;')])
    save_names=['reset_shooting_scores','reset_mole_scores','reset_trial_times','save_checksum','save_reset_all','save_reset','save_check_all']
    mini_names=['board_reset_icecap_scores','board_reset_sunaboard_scores','cart_reset_times']
    prefix=raw['src/save/minigame.c'].split(b'static struct save_cart_times *cart_find_times',1)[0]
    constants=b'\n'.join(line('src/save/save.c',rb'^#define '+n+rb'[^\r\n]*') for n in [b'LEAST_TIMES',b'CRC_POLYNOMIAL'])
    prefix+=b'\n#include <stddef.h>\n#include <string.h>\n#include "hud/hud.h"\n#include "progress/demo.h"\n'+constants+b'\n'+state+b'\n'
    bodies=[function(raw['src/save/minigame.c'],n) for n in mini_names]+[function(raw['src/hud/hud.c'],'ring_reset')]+[function(raw['src/save/save.c'],n) for n in save_names]
    game=d/'external_boot_save_state.c';game.write_bytes(prefix+b'\n'.join(bodies)+b'''
_Static_assert(sizeof(struct save)==0x4a0,"Japanese serialized save bytes");
_Static_assert(sizeof(struct save_time)==3 && sizeof(struct story_record)==12,"Japanese scalar record layout");
_Static_assert(offsetof(struct save,event_flags)==0x260 && offsetof(struct save,story_records)==0x2e8 && offsetof(struct save,stage_clears)==0x348,"Japanese save section offsets");
__declspec(dllexport) unsigned jp_save_bytes(void){return sizeof(struct save);}
__declspec(dllexport) unsigned jp_save_count(void){return SAVE_COUNT;}
__declspec(dllexport) unsigned jp_save_checksum(const void *data){return save_checksum((struct save *)data);}
''')
    # Providers use the original declarations. Consumers explicitly import the
    # single shared data fields rather than defining another save/demo owner.
    for name,needle in [('progress/demo.h',b'extern Bool demo_playing;'),('save/card.h',b'extern int save_slot;')]:
        data=(u/'src'/name).read_bytes();assert data.count(needle)==1;data=data.replace(needle,b'__declspec(dllimport) '+needle)
        if name=='progress/demo.h':
            needle=b'extern int demo_event;';assert data.count(needle)==1;data=data.replace(needle,b'__declspec(dllimport) '+needle)
        (d/'imports'/name).write_bytes(data)
    data=(logo/'include/progress/recorder.h').read_bytes();(d/'imports/progress/recorder.h').write_bytes(data)
    diag_source=base/'present_real_input.c';assert sha(diag_source)==b['generated_sources_sha256'][diag_source.name]
    diag=diag_source.read_bytes();assert diag.count(b'Bool draw_off,demo_playing;')==1;diag=diag.replace(b'Bool draw_off,demo_playing;',b'Bool draw_off; /* demo_playing belongs to the real shared game owner. */')
    presenter=d/'present_imported_demo_state.c';presenter.write_bytes(diag)
    simple=[str(c),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffreestanding','-fno-builtin','-fwrapv','-Wall','-Wextra','-Werror','-Wno-unused-parameter','-Wno-unknown-pragmas','-Wno-ignored-qualifiers','-include',str(p/'native/jp_host_types.h'),'-I'+str(p/'native'),'-I'+str(u/'sdk/include'),'-I'+str(u/'src'),'-I'+str(u/'modules')]
    pc=list(b['commands'][0]['command']);pc=pc[:pc.index('-c')];pc.insert(1,'-I'+str(d/'imports'))
    commands=[simple+['-c',str(game),'-o',str(d/'boot_save_state.obj')],pc+['-c',str(presenter),'-o',str(d/'present_imported_demo_state.obj')]]
    fl=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['frames_DLL']))
    kept={str(Path(n).resolve()):sha(n) for n in fl if n.endswith('.obj')}
    assert not any(x['section']>0 and x['name'] in ['saves','save_slot','rings','demo_playing'] for n in kept for x in coff_symbols(Path(n)))
    extra=[str(d/'boot_save_state.obj')]+['-Wl,/export:'+n for n in ['save_reset_all','save_reset','save_check_all','ring_reset',*mini_names]]+[v for n in ['saves','save_slot','rings','demo_playing'] for v in ['-Xlinker','/export:'+n+',DATA']]
    at=fl.index('-o');fl[at:at]=extra;fl[-1]=str(d/'frames/jp_native_frames.dll');commands.append(fl)
    pl=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['DLL']))
    old=str(base/'frame_real_input.obj');assert pl.count(old)==1;pl[pl.index(old)]=str(d/'present_imported_demo_state.obj')
    old=str(Path(b['frames_DLL']).with_suffix('.lib'));assert pl.count(old)==1;pl[pl.index(old)]=str(d/'frames/jp_native_frames.lib');pl[-1]=str(d/'jp_native_present.dll');commands.append(pl)
    el=list(next(x['command'] for x in lb['commands'] if '-shared' in x['command'] and x['command'][-1]==lb['authored_exit_DLL']))
    for old,new in [(Path(b['frames_DLL']).with_suffix('.lib'),d/'frames/jp_native_frames.lib'),(Path(b['DLL']).with_suffix('.lib'),d/'jp_native_present.lib')]:
        assert el.count(str(old))==1;el[el.index(str(old))]=str(new)
    el[-1]=str(d/'jp_native_exit.dll');commands.append(el)
    (d/'inputs/progress/demo').mkdir(parents=True)
    for f in (logo/'inputs/progress/demo').glob('*'):shutil.copyfile(f,d/'inputs/progress/demo'/f.name)
    original=logo/'external_original_complete_logo.c';assert sha(original)==lb['generated_sources_sha256'][original.name]
    boot=function(raw['src/progress/demo.c'],'boot_init');full=d/'external_original_boot_logo.c';full.write_bytes(original.read_bytes()+b'\n#include "save/card.h"\n#include "save/save.h"\n'+boot)
    lc=list(next(x['command'] for x in lb['commands'] if '-c' in x['command'] and x['command'][-1].endswith('logo_complete.obj')));lc.insert(1,'-I'+str(d/'imports'));lc.insert(1,'-I'+str(d/'inputs'));lc[-3]=str(full);lc[-1]=str(d/'boot_logo.obj');commands.append(lc)
    ll=list(lb['commands'][-1]['command']);ll[ll.index(str(logo/'logo_complete.obj'))]=str(d/'boot_logo.obj')
    for old,new in [(Path(b['frames_DLL']).with_suffix('.lib'),d/'frames/jp_native_frames.lib'),(Path(b['DLL']).with_suffix('.lib'),d/'jp_native_present.lib'),(Path(lb['authored_exit_DLL']).with_suffix('.lib'),d/'jp_native_exit.lib')]:
        assert ll.count(str(old))==1;ll[ll.index(str(old))]=str(new)
    ll.insert(ll.index('-o'),'-Wl,/export:boot_init');ll[-1]=str(d/'jp_native_logo_start.dll');commands.append(ll)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6500]
    for n,h in kept.items():assert sha(n)==h,n
    frame=dict(b,status='original_save_reset_CRC_defaults_and_shared_boot_demo_state_linked_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),new_original_save_functions=save_names+mini_names+['ring_reset'],new_state_fields=['saves','save_slot','rings','demo_playing'],serialized_save_bytes=0x4a0,original_source_sha256={n:sha(u/n) for n in raw},kept_frame_objects_sha256=kept,previous_frame_build_sha256=sha(base/'build_verification.json'),single_demo_playing_owner_in_frame_DLL=True,game_containing_DLL_publishable=False,playable=False,overall_completion_percent=10)
    frame['authored_source_sha256']=dict(b['authored_source_sha256']);frame['authored_source_sha256'][Path(__file__).resolve().relative_to(r).as_posix()]=sha(Path(__file__))
    frame['generated_sources_sha256']={f.name:sha(f) for f in [game,presenter]}
    fd=d/'frame_view';fd.mkdir()
    for f in [game,presenter]:shutil.copyfile(f,fd/f.name);assert sha(fd/f.name)==sha(f)
    frame['generated_source_view_copies_match_actual_compiled_sources']=True
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(lb,status='unchanged_original_boot_init_and_genuine_reset_dependencies_linked_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),frames_DLL=frame['frames_DLL'],frames_DLL_sha256=frame['frames_DLL_sha256'],present_DLL=frame['DLL'],authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'),original_boot_init_body_sha256=hashlib.sha256(boot).hexdigest(),original_boot_init_body_unchanged=True,original_boot_init_executed=False,game_containing_DLL_publishable=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    result['authored_source_sha256']=dict(lb['authored_source_sha256'],**frame['authored_source_sha256'])
    result['generated_sources_sha256']={f.name:sha(f) for f in [game,presenter,full]}
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])
if __name__=='__main__':main()
