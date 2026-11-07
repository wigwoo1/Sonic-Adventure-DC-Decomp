"""Render genuine two-motion blends through the real parent/task/GPU owners.

The callbacks and fixed camera are diagnostic, not the original player or game.
The nine preview samples demonstrate blending; their timing is not game FPS.
"""
import hashlib,inspect,json
from pathlib import Path
import verify_native_action_tasks as task_suite
import verify_native_action_frames as frame_suite
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def derive():
 raw,changes=task_suite.derive()
 def rep(old,new):
  nonlocal raw
  assert raw.count(old)==1,(old,raw.count(old));raw=raw.replace(old,new)
 old="    draw_action=bind(f,'draw_action',None,P,F);sample_frames="
 new="""    from verify_native_action_link_poses import MotionLink,ActionLink
    second_view=View();check(bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)(7,b'sonic_full_speed_action',C.byref(second_view))==0,'second genuine action in resident bank7 retained through actual task/frame/GPU lifetime')
    second_action=Action.from_address(second_view.data)
    check(second_action.object==action.object and Motion.from_address(second_action.motion).frames==33,'both genuine motions use the same canonical Sonic tree')
    motion_link=MotionLink((P*2)(action.motion,second_action.motion),(F*2)(8.5,16.))
    action_link=ActionLink(action.object,C.addressof(motion_link));actual_draw_link=bind(f,'draw_action_link',None,P,F)
    def draw_action(unused,animation_frame):
        actual_draw_link(C.byref(action_link),animation_frame/32.)
    sample_frames="""
 rep(old,new)
 rep("            assert bind(motions,'jp_motions_release',I,Q)(action_view.token)==0", "            assert bind(motions,'jp_motions_release',I,Q)(second_view.token)==0\n            assert bind(motions,'jp_motions_release',I,Q)(action_view.token)==0")
 marker="    (d/'result.json').write_text(json.dumps(out,indent=2)+'\\n');"
 final="""    for row in animation_images:row.update(blend_ratio=row['frame']/32.,motion_frames=[8.5,16.])
    out.update(status='genuine_two_motion_Sonic_blends_actual_parent_task_GPU_and_texture_Off_captures_verified',original_blending_interpreter='draw_action_link',motion_slots=[dict(bank=0,action='sonic_updraft_action',frame=8.5),dict(bank=7,action='sonic_full_speed_action',frame=16.)],rendered_blend_samples=animation_images,blend_ratio_formula='diagnostic sample frame / 32',both_genuine_action_views_retained_until_task_destruction=True,shape_scaled_other_character_paths_verified=False,original_player_callback_or_gameplay_verified=False)
"""
 # The derived Python source contains a literal escaped newline at this site.
 marker=marker.replace('\\\\n','\\n');rep(marker,final+marker)
 return raw,changes
def exercise(c,inputs,track):
 import verify_native_character_tree as tree
 from PIL import Image
 raw,changes=derive();path=c['d']/'private_actual_action_link_frame_body.py';path.write_text(raw);namespace=dict(vars(tree),__file__=__file__);exec(compile(raw,str(path),'exec'),namespace);namespace['exercise'](c,inputs,track)
 v=read(c['d']/'result.json')
 # The established opaque-window PNG writer preserves readback RGB and omits
 # the non-display alpha channel. Do not equate its PNG RGBA hash to raw GPU
 # RGBA bytes; qualify the displayed RGB and lossless preview independently.
 for row in v['rendered_blend_samples']:row['displayed_RGB_sha256']=hashlib.sha256(Image.open(row['path']).convert('RGB').tobytes()).hexdigest()
 v.update(derived_exercise_sha256=sha(path),original_task_frame_verifier_sha256=sha(task_suite.__file__),original_action_frame_verifier_sha256=sha(frame_suite.__file__),opaque_window_RGB_preserved=True,readback_alpha_not_stored_in_opaque_window_PNG=True);(c['d']/'result.json').write_text(json.dumps(v,indent=2)+'\n')
def main():
 raw=inspect.getsource(frame_suite.main);old='from verify_native_action_frames import exercise';assert raw.count(old)==1;raw=raw.replace(old,'from verify_native_action_link_frames import exercise')
 marker="    original=p/'jp_port_tools/verify_native_game_init.py'";assert raw.count(marker)==1;raw=raw.replace(marker,"    for key,value in b.items():\n        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):assert sha(value)==b[key+'_sha256']\n"+marker)
 namespace=dict(vars(frame_suite),__file__=__file__);exec(compile(raw,__file__,'exec'),namespace);namespace['main']()
if __name__=='__main__':main()
