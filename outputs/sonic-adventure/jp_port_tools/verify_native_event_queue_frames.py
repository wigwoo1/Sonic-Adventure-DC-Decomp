"""Real event/weld task rendering; owning callback/camera remain diagnostic."""
import hashlib
import inspect
import json
from pathlib import Path
import verify_native_action_tasks as task_suite
import verify_native_action_frames as frame_suite

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def derive():
    raw,changes=task_suite.derive()
    def rep(old,new):
        nonlocal raw
        assert raw.count(old)==1,(old,raw.count(old))
        raw=raw.replace(old,new)
    rep("(0,b'sonic_updraft_action'","(7,b'sonic_full_speed_action'")
    rep("    draw_action=bind(f,'draw_action',None,P,F);sample_frames=",
        "    def draw_action(unused,unused_frame):owner.draw()\n    sample_frames=")
    rep("    def extent(pointer,element):",
        "    from native_event_frame_owner import Owner\n"
        "    owner=Owner(f,motions,b,entries,task_record,action_view,r/'work/sonic-adventure/jp-ninja-math-reference/1ST_READ.BIN',check)\n"
        "    def extent(pointer,element):")
    rep("            assert bind(f,'jp_character_textures_release',I,U)(0)==0",
        "            owner.close()\n            assert bind(f,'jp_character_textures_release',I,U)(0)==0")
    # Fitting is a separate queue run: reset through real event_free and
    # allocate fresh entries before visible captures, not dangling pointers.
    rep("    measure(view.data,identity);",
        "    measure(view.data,identity);owner.reset(C.addressof(listing.contents));visible_sample_start=len(owner.samples);")
    assert raw.count("data_hash()==original_model_hash")==2
    raw=raw.replace("data_hash()==original_model_hash","owner.immutable_hash()==owner.immutable_baseline")
    rep("all2793 canonical model records remain byte-for-byte intact",
        "all immutable canonical bytes survive genuine event welding")
    rep("all original motion and canonical model source records survive actual animation draws byte-for-byte",
        "motion source and immutable canonical bytes survive actual event welding")
    marker="    (d/'result.json').write_text(json.dumps(out,indent=2)+'\\n');"
    extra="""    for index,row in enumerate(animation_images):
        row['diagnostic_sample_label']=row.pop('frame')
        row.update(event_draw_step=index+1,draw_mode='blend' if index<8 else 'normal',blend_ratio=(index+1)/9. if index<8 else None,current_motion_frame=0.)
    out.update(status='original_event_queue_and_welding_actual_task_GPU_textures_and_Off_frames_verified',event_samples=owner.samples[visible_sample_start:],camera_fit_event_samples=owner.samples[:visible_sample_start],original_event_motions_draw_connected_to_real_rendering=True,original_player_weld_connected_to_real_rendering=True,original_action='sonic_full_speed_action',previous_action='sonic_stand_action',event_blend_steps=8,canonical_records=len(entries),all_canonical_bytes_restored_before_Motion_shutdown=True,all_original_model_and_motion_records_preserved=False,production_player_or_actor_callback_verified=False,original_gameplay_task_FSM_or_main_loop_verified=False,diagnostic_sample_labels_are_not_event_frames=True)
"""
    rep(marker,extra+marker)
    return raw,changes

def exercise(c,inputs,track):
    import verify_native_character_tree as tree
    raw,changes=derive()
    path=c['d']/'private_event_queue_frame_body.py'
    path.write_text(raw)
    namespace=dict(vars(tree),__file__=__file__)
    exec(compile(raw,str(path),'exec'),namespace)
    namespace['exercise'](c,inputs,track)
    v=read(c['d']/'result.json')
    v.update(derived_exercise_sha256=sha(path),
             event_owner_sha256=sha(Path(__file__).with_name('native_event_frame_owner.py')),
             task_verifier_sha256=sha(task_suite.__file__))
    (c['d']/'result.json').write_text(json.dumps(v,indent=2)+'\n')

def main():
    raw=inspect.getsource(frame_suite.main)
    old='from verify_native_action_frames import exercise'
    assert raw.count(old)==1
    raw=raw.replace(old,'from verify_native_event_queue_frames import exercise')
    namespace=dict(vars(frame_suite),__file__=__file__)
    exec(compile(raw,__file__,'exec'),namespace)
    namespace['main']()

if __name__=='__main__':main()
