"""Genuine presentation recording/lifecycle and complete task in one native owner.

Reuse the real clock and matrix/texture owners; move the existing clock objects
into the shared DLL and forward the presenter's existing clock exports there.
Original game bodies, generated sources and diagnostic DLLs stay private.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())


def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','presentation','task','task-runtime','compiler','output']:
        ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r=a.workspace;u=a.upstream;p=r/'outputs/sonic-adventure';w=r/'work/sonic-adventure';d=a.output
    assert d.is_relative_to(w) and not d.is_relative_to(w/'build') and not d.exists()
    b=read(a.baseline/'build_verification.json');pb=read(a.presentation/'build_verification.json');tb=read(a.task/'build_verification.json');rb=read(a.task_runtime/'build_verification.json')
    for old in [b,pb,rb]:
        for n,h in old['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(a.compiler)==b['compiler_sha256'] and sha(a.presentation/'external_present_native.c')==pb['generated_source_sha256']
    assert sha(a.task/'task_present.obj')==tb['object_sha256'] and sha(a.task/'external_task_presentation_native.c')==tb['generated_source_sha256']
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    d.mkdir();(d/'frames').mkdir();overlay=d/'diagnostics';overlay.mkdir();inc=d/'include';inc.mkdir()
    # One real clock storage owner; consumers explicitly import the original API.
    clock_header=(p/'native/jp_native_clock.h').read_bytes();assert clock_header.count(b'__declspec(dllexport)')==1
    clock_import=inc/'clock_import.h';clock_import.write_bytes(clock_header.replace(b'__declspec(dllexport)',b'__declspec(dllimport)')+b'\n#undef JP_CLOCK_API\n#define JP_CLOCK_API __declspec(dllexport)\n')
    imports=['frame_split','frame_redrawing','frame_later_draws','_nj_calc_vertex_','_nj_calc_polygon_','_nj_draw_polygon_','_nj_vsync_count_','_nj_wait_vsync_in_','_nj_wait_vsync_out_']
    import_header=inc/'game_state_import.h';import_header.write_bytes(b'#include <ninja.h>\n#include "gcc/src/system/port.h"\n#include "jp_native_frame_state.h"\n')
    ninja_header=(u/'sdk/include/ninja.h').read_bytes()
    port_header=(u/'gcc/src/system/port.h').read_bytes()
    for n in imports:
        header=port_header if n.startswith('frame') else ninja_header
        matches=re.findall(rb'^extern (?:int|Int|Uint32) '+n.encode()+rb';$',header,re.M);assert len(matches)==1,n
        header=header.replace(matches[0],b'__declspec(dllimport) '+matches[0])
        if n.startswith('frame'):port_header=header
        else:ninja_header=header
    (inc/'ninja.h').write_bytes(ninja_header)
    for path in [inc/'system/port.h',inc/'gcc/src/system/port.h']:
        path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(port_header)
    fh=(u/'src/system/frame.h').read_bytes()
    for n in b['shared_frame_counters']:
        old=('extern int '+n+';').encode();assert fh.count(old)==1;fh=fh.replace(old,b'JP_FRAME_STATE_API '+old)
    (inc/'system').mkdir(exist_ok=True);(inc/'system/frame.h').write_bytes(b'#include "jp_native_frame_state.h"\n'+fh)
    # Qualified full task source, with read-only pool diagnostics already used.
    task_raw=(a.task/'external_task_presentation_native.c').read_bytes()
    startup_task=w/'jp-native-shared-frame-state-startup-v1/external_task_startup.c'
    diagnostics=startup_task.read_bytes().split(b'__declspec(dllexport) unsigned jp_startup_pool_info',1)[1]
    task_source=d/'external_task.c';task_source.write_bytes(task_raw+b'\n__declspec(dllexport) unsigned jp_startup_pool_info'+diagnostics)
    raw=(a.presentation/'external_present_native.c').read_bytes();prefix=raw[:raw.index(function(raw,'state_get'))]
    names=['state_get','timer_stop','marks_slot','marks_find','marks_add','view_index','keep_draw','flat_words','keep_flat','keep_step','present_record','present_sprite','present_untimed','released','draws_forget','flats_forget','shadows_forget','present_forget','present_wide','present_run','records_keep','records_put','present_draw','replays']
    declarations=[]
    for pattern in [rb'^static struct task deleted_task;\r?\n',rb'^static struct task released_task;\r?\n',rb'^struct records \{.*?^\};\r?\n']:
        matches=re.findall(pattern,raw,re.M|re.S);assert len(matches)==1;declarations.append(matches[0])
    pieces={n:function(raw,n) for n in names};original_bodies={n:hashlib.sha256(x).hexdigest() for n,x in pieces.items()}
    for n,notify in [('present_record',b'    jp_present_observe_record(kind,what,count,a,pri,attr);\n'),('present_untimed',b'    jp_present_observe_untimed(begin);\n')]:
        pieces[n]=pieces[n].replace(b'{\n',b'{\n'+notify,1)
    # Existing fixture controls actual private state, but defines no game globals.
    fixture=(p/'native/jp_presentation_lifecycle_fixture.c').read_bytes()
    for definition in [b'int frame_step;',b'struct task_work *camera_work;',b'struct task *player_tasks[PLAYER_COUNT];',b'struct player *player_records[PLAYER_COUNT];',b'Float port_wide_anchor;',b'Int port_wide_cover;']:
        assert fixture.count(definition)==1;fixture=fixture.replace(definition,b'')
    selected=d/'external_real_presentation.c'
    selected.write_bytes(b'#include "jp_native_present_observer.h"\n'+prefix+b''.join(declarations)+b''.join(pieces.values())+b'\n'+fixture+b'''
__declspec(dllexport) void jp_present_install_sprite(void){port_sprite_record=present_sprite;}
__declspec(dllexport) unsigned jp_present_kept_count(int later){return later?later_draws.count:first_draws.count;}
__declspec(dllexport) unsigned jp_present_flat_count(int later){return later?later_flats.count:first_flats.count;}
''')
    state=d/'external_game_state.c'
    state_declarations=[]
    for path,line in [('src/camera/camera.c',b'struct task_work *camera_work = NULL;'),('src/player/player.c',b'struct task *player_tasks[PLAYER_COUNT];'),('src/player/player.c',b'struct player *player_records[PLAYER_COUNT];'),('sdk/ninja/njGlobal.c',b'Uint32 _nj_vsync_count_;'),('sdk/ninja/njGlobal.c',b'Uint32 _nj_wait_vsync_in_;'),('sdk/ninja/njGlobal.c',b'Uint32 _nj_wait_vsync_out_;')]:
        assert (u/path).read_bytes().count(line)==1;(state_declarations.append(line))
    state.write_bytes(b'#include "camera/camera.h"\n#include "player/player.h"\n'+b'\n'.join(state_declarations)+b'\n')
    # Retain diagnostic observers as optional tracing, never service providers.
    for n in ['frame_diagnostic_probe.c','primitive_diagnostic_probe.c','sprite_diagnostic_probe.c']:
        body=(p/'jp_port_tools'/n).read_bytes()
        if n=='sprite_diagnostic_probe.c':
            body=body.replace(b'int frame_step,frame_later_draws;',b'extern int frame_step,frame_later_draws;')
            body=body.replace(b'void present_untimed(',b'void jp_diagnostic_present_untimed(')
            marker=b'{frame_step=step;frame_later_draws=later;}';assert body.count(marker)==1
            body=body.replace(marker,b'{jp_present_set_observers(jp_diagnostic_record_adapter,jp_diagnostic_present_untimed);frame_step=step;frame_later_draws=later;}')
            at=body.index(b'__declspec(dllexport) void jp_sprite_probe_frame')
            body=body[:at]+b'void jp_diagnostic_present_record(enum present_kind,const void *,Int,Int,Float,Uint32);\nstatic void jp_diagnostic_record_adapter(int k,const void *p,int n,int a,float z,unsigned f){jp_diagnostic_present_record((enum present_kind)k,p,n,a,z,f);}\n'+body[at:]
            body=b'#include "jp_native_present_observer.h"\n'+body
        if n=='primitive_diagnostic_probe.c':body=body.replace(b'void present_record(',b'void jp_diagnostic_present_record(')
        (overlay/n).write_bytes(body)
    # Reuse exact original compilation recipes and all unrelated owner objects.
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')]+['-I'+str(u/'modules'),'-I'+str(u/'events')]
    commands=[common+['-c',str(state),'-o',str(d/'game_state.obj')]]
    taskcmd=list(tb['command']);taskcmd[taskcmd.index('-c')+1]=str(task_source);taskcmd[-1]=str(d/'task.obj');commands.append(taskcmd)
    presentcmd=list(pb['command']);presentcmd[presentcmd.index('-c')+1]=str(selected);presentcmd[-1]=str(d/'presentation.obj')
    presentcmd[presentcmd.index('-c'):presentcmd.index('-c')]=['-Wno-unused-variable'];commands.append(presentcmd)
    probe_cmd=list(next(x['command'] for x in b['commands'] if x['command'][-1].endswith('frame_probe.obj')));probe_cmd[probe_cmd.index('-c')+1]=str(overlay/'frame_diagnostic_probe.c');probe_cmd[-1]=str(d/'frame_probe.obj');commands.append(probe_cmd)
    commands.append(common+['-c',str(p/'native/jp_native_present_observer.c'),'-o',str(d/'observer.obj')])
    frame_link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['frames_DLL']))
    presenter_link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['DLL']))
    clock_objects=[x for x in presenter_link if Path(x).name in ['clock.obj','sdk_clock.obj']];assert len(clock_objects)==2
    presenter_link=[x for x in presenter_link if x not in clock_objects]
    clock_exports=['jp_clock_initialize','jp_clock_shutdown','jp_clock_status','jp_clock_wall_counts','jp_clock_timer1_ticks','jp_clock_read_tstr','jp_clock_write_tstr','jp_clock_read_tcnt1','jp_clock_reset_timer1','jp_clock_frequency','syTmrGetCount']
    for n in clock_exports:presenter_link=[x for x in presenter_link if x!='-Wl,/export:'+n]
    # Recompile every remaining presenter object to use imported clock/state.
    present_base=read(w/'jp-native-bitmap-upload-v7/build_verification.json')
    recipes={Path(x['command'][-1]).resolve():x['command'] for source in [present_base,b] for x in source['commands'] if '-c' in x['command']}
    # Earlier host service recipes live in their original qualified reports.
    for folder in ['jp-native-present-v6','jp-native-frame-wait-v3']:
        for row in read(w/folder/'build_verification.json')['commands']:
            if '-c' in row['command']:recipes[Path(row['command'][-1]).resolve()]=row['command']
    rebuilt={}
    for old in [x for x in presenter_link if x.endswith('.obj')]:
        key=Path(old).resolve();cmd=list(recipes[key]);src=Path(cmd[cmd.index('-c')+1]);body=src.read_bytes()
        if Path(old).name=='diagnostic.obj':
            body=(a.baseline/'external_diagnostic_native.c').read_bytes()
            for line in [b'int frame_split;',b'Int _nj_calc_vertex_,_nj_calc_polygon_,_nj_draw_polygon_;',b'Uint32 _nj_vsync_count_,_nj_wait_vsync_in_,_nj_wait_vsync_out_;']:
                assert body.count(line)==1;body=body.replace(line,b'')
            body=b'#include "game_state_import.h"\n'+body
        target=d/('present_'+Path(old).stem+'.c');target.write_bytes(body)
        cmd[cmd.index('-c')+1]=str(target);cmd[-1]=str(d/Path(old).name);cmd[1:1]=['-include',str(clock_import),'-I'+str(inc)]
        commands.append(cmd);presenter_link[presenter_link.index(old)]=cmd[-1];rebuilt[old]=cmd[-1]
    old_probe=next(x for x in frame_link if x.endswith('frame_probe.obj'));frame_link[frame_link.index(old_probe)]=str(d/'frame_probe.obj')
    additions=[str(d/n) for n in ['game_state.obj','task.obj','presentation.obj','observer.obj']]+clock_objects+[str(a.task_runtime/n) for n in ['bridge.obj','lifecycle.obj']]+[str(a.presentation/'replay_lifetime.obj')]
    exports=['heap_init','task_init','task_create','task_alloc','task_destroy','task_kill','task_step_all','task_draw_all','task_destroy_all','task_step_level','task_draw_level','task_step_children','task_draw_children','task_create_child','task_create_children','task_destroy_children','task_get_current_root_step','present_run','present_draw','present_forget','present_record','present_untimed','sprite_get_depth','sprite_set_depth']
    data=['camera_work','player_tasks','player_records','frame_redrawing','frame_split','frame_later_draws','_nj_vsync_count_','_nj_wait_vsync_in_','_nj_wait_vsync_out_','_nj_calc_vertex_','_nj_calc_polygon_','_nj_draw_polygon_']
    at=frame_link.index('-o');frame_link[at:at]=additions+['-Wl,/export:'+n for n in exports+['syTmrGetCount']]+[x for n in data for x in ['-Xlinker','/export:'+n+',DATA']]
    frame_link[-1]=str(d/'frames/jp_native_frames.dll');commands.append(frame_link)
    old_lib=next(x for x in presenter_link if x.endswith('frames\\jp_native_frames.lib') or x.endswith('frames/jp_native_frames.lib'));presenter_link[presenter_link.index(old_lib)]=str(d/'frames/jp_native_frames.lib')
    presenter_link[presenter_link.index('-o'):presenter_link.index('-o')]=['-Wl,/export:'+n+'=jp_native_frames.'+n for n in clock_exports]
    presenter_link[-1]=str(d/'jp_native_present.dll');commands.append(presenter_link)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)))
        assert run.returncode==0 and not log.stat().st_size,log.read_text()[:7500]
    result=dict(b,status='real_complete_task_and_presentation_recording_shared_clock_native_runtime_linked_behavior_unverified',commands=results,frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),task_runtime_in_frame_owner=True,complete_task_source_linked=True,selected_original_presentation_functions=names,original_presentation_function_sha256=original_bodies,optional_observer_notifications=['present_record','present_untimed'],fixture_defines_game_state=False,one_real_clock_owner=True,clock_owner_objects_sha256={x:sha(x) for x in clock_objects},clock_export_forwarders=clock_exports,diagnostic_record_and_untimed_service_definitions_removed=True,actual_task_gameplay_or_complete_frame_loop_verified=False,playable=False,artifact_publishable=False,overall_completion_percent=10)
    result['authored_source_sha256']=dict(b['authored_source_sha256'])
    for src in [Path(__file__).resolve(),p/'native/jp_native_present_observer.c',p/'native/jp_native_present_observer.h']:result['authored_source_sha256'][src.relative_to(r).as_posix()]=sha(src)
    result['generated_sources_sha256']={str(x.relative_to(d)):sha(x) for x in d.rglob('*') if x.is_file() and x.suffix in ['.c','.h']}
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'])


if __name__=='__main__':main()
