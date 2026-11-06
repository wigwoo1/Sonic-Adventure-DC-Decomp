"""Actual task scheduler/destruction around original animated native GPU draws.

The owning callbacks are a diagnostic fixture, not the original player callback
or a production automatic lease service. No original native routine is replaced.
"""
import argparse,hashlib,json,os,subprocess,sys
from pathlib import Path
import verify_native_action_frames as action_suite

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def derive():
    raw,previous=action_suite.derive();changes=[]
    def rep(old,new):
        nonlocal raw
        assert raw.count(old)==1,(old,raw.count(old));raw=raw.replace(old,new);changes.append(old)
    anchor='    entries=[];get=bind(motions,\'jp_motion_models_entry\',I,U,P)'
    setup='''    from native_task_test_support_v2 import Task
    task_events=[];task_errors=[];task_frame=[0.];task_pointer=[None];task_callback=C.CFUNCTYPE(None,P)
    @task_callback
    def task_draw(address):
        try:
            assert address==task_pointer[0]
            task_events.append(('draw',task_frame[0]));draw_action(action_view.data,task_frame[0])
        except BaseException as error:task_errors.append(repr(error))
    @task_callback
    def task_destructor(address):
        try:
            assert address==task_pointer[0]
            assert bind(f,'jp_character_textures_release',I,U)(0)==0
            assert bind(motions,'jp_motions_release',I,Q)(action_view.token)==0
            assert bind(motions,'jp_motions_release',I,Q)(view.token)==0
            task_events.append(('destroy',address))
        except BaseException as error:task_errors.append(repr(error))
    # game_init precedes stage/title entry, which initializes these actual
    # original providers. This fresh diagnostic has no existing task pool.
    bind(f,'heap_init',None)();bind(f,'task_init',None)()
    # TASK_WORK=2; TASK_LEVEL_FIRST=8 maps to actual level7. Creation's7
    # means TASK_UNLINKED, while task_draw_level accepts7 or8.
    task_pointer[0]=bind(f,'task_create',P,C.c_uint16,I,P)(2,8,None)
    check(bool(task_pointer[0]),'actual initialized original task pool allocates diagnostic animation owner')
    task_record=Task.from_address(task_pointer[0]);task_record.draw=C.cast(task_draw,P).value;task_record.destructor=C.cast(task_destructor,P).value
    def scheduled_draw(animation_frame):
        task_frame[0]=animation_frame;before=len(task_events)
        bind(f,'task_draw_level',None,I)(7)
        check(not task_errors and task_events[before:]==[('draw',animation_frame)],'actual original task_draw_level/present_draw calls genuine animation exactly once')
'''
    rep(anchor,setup+anchor)
    rep('    draw_action(action_view.data,sample_frames[0])','    scheduled_draw(sample_frames[0])')
    rep('        setmatrix(None,before_matrix);draw_action(action_view.data,animation_frame)','        setmatrix(None,before_matrix);scheduled_draw(animation_frame)')
    old="    check(bind(f,'jp_character_textures_release',I,U)(0)==0 and bind(motions,'jp_motions_release',I,Q)(action_view.token)==0 and bind(motions,'jp_motions_release',I,Q)(view.token)==0 and bind(f,'jp_character_switch_retire',I)()==0,'actual texture/model/game references retire after captured draw completes')"
    new="""    bind(f,'task_destroy',None,P)(task_pointer[0])
    check(not task_errors and len([v for v in task_events if v[0]=='draw'])==9 and len([v for v in task_events if v[0]=='destroy'])==1,'actual original task destruction invokes owning diagnostic destructor once after all GPU/frame retirement')
    check(not task_record.draw and not task_record.destructor,'actual original task destroy clears callbacks before pool slot reuse')
    check(bind(f,'jp_character_switch_retire',I)()==0,'game reference retires after task destructor releases actual model/action/texture consumers')"""
    rep(old,new)
    marker="    (d/'result.json').write_text(json.dumps(out,indent=2)+'\\n');"
    rep(marker,"    out.update(status='actual_original_task_scheduler_and_destructor_own_diagnostic_animation_views_through_real_GPU_frames_verified',task_callback_events=task_events,task_scheduler_original=True,owning_callbacks_are_diagnostic_not_original_player=True,production_automatic_task_render_lease_service_implemented=False,original_player_callback_or_gameplay_verified=False)\n"+marker)
    return raw,changes

def exercise(c,inputs,track):
    import verify_native_character_tree as tree
    raw,changes=derive();path=c['d']/'private_task_action_body.py';path.write_text(raw)
    namespace=dict(vars(tree),__file__=__file__);exec(compile(raw,str(path),'exec'),namespace);namespace['exercise'](c,inputs,track)
    v=read(c['d']/'result.json');v.update(derived_exercise_sha256=sha(path),original_action_frame_verifier_sha256=sha(action_suite.__file__),caller_adaptations=changes);(c['d']/'result.json').write_text(json.dumps(v,indent=2)+'\n')

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','inputs','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();b=read(a.build/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    original=p/'jp_port_tools/verify_native_game_init.py';raw=original.read_bytes();marker=b"        if a.case=='normal':\n";assert raw.count(marker)==1;prefix=raw.split(marker)[0]
    for old,new in [(b"('jp_textures_initialize',(32,1024,32<<20))",b"('jp_textures_initialize',(1024,4096,128<<20))"),(b"('jp_render_state_initialize',(128,1<<20))",b"('jp_render_state_initialize',(2048,16<<20))")]:assert prefix.count(old)==1;prefix=prefix.replace(old,new)
    caller=d/'private_parent_caller.py';caller.write_bytes(prefix+('        from verify_native_action_tasks import exercise\n        exercise(locals(), '+repr(str(a.inputs.resolve()))+', '+repr(str(a.track.resolve()))+")\nif __name__=='__main__':main()\n").encode())
    cmd=[sys.executable,'-B',str(caller),'--workspace',str(r),'--build',str(a.build.resolve()),'--track',str(a.track.resolve()),'--output',str(d/'run'),'--case','normal']
    run=subprocess.run(cmd,capture_output=True,timeout=240,env=dict(os.environ,PYTHONPATH=str(p/'jp_port_tools')),creationflags=subprocess.CREATE_NO_WINDOW);(d/'stdout.txt').write_bytes(run.stdout);(d/'stderr.txt').write_bytes(run.stderr);assert not run.returncode,(run.returncode,run.stderr.decode(errors='replace')[-6500:])
    v=read(d/'run/result.json');v.update(original_parent_verifier_sha256=sha(original),derived_caller_sha256=sha(caller));(d/'verification.json').write_text(json.dumps(v,indent=2)+'\n');print(v['status'],v['checks'],'checks',flush=True)
if __name__=='__main__':main()
