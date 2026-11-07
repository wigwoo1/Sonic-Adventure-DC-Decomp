"""Real full-speed Sonic animation and original toe control through task/GPU."""
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
 rep("(0,b'sonic_updraft_action'","(7,b'sonic_full_speed_action'")
 anchor='    from native_task_test_support_v2 import Task'
 setup='''    import struct
    toe_layout=(U*7)();check(bind(f,'jp_sonic_toes_layout',I,P)(toe_layout)==0,'actual player ABI for original toe control')
    toe_player=C.create_string_buffer(toe_layout[0]);struct.pack_into('<H',toe_player,toe_layout[1],13)
    check(bind(f,'jp_sonic_toes_bind',I)()==0,'native toe owner acquires twelve actual model/node/normal views')
    toe_samples=[]
    def toe_control(animation_frame):
        struct.pack_into('<f',toe_player,toe_layout[2],animation_frame)
        check(bind(f,'jp_sonic_toes_step',I,P)(toe_player)==0,'original Sonic toe control updates genuine canonical shapes')
        toe_samples.append(dict(frame=animation_frame,weights=[bytes(toe_player[o:o+4]).hex() for o in toe_layout[3:5]]))
'''
 rep(anchor,setup+anchor)
 rep("task_events.append(('draw',task_frame[0]));draw_action(action_view.data,task_frame[0])","task_events.append(('draw',task_frame[0]));toe_control(task_frame[0]);draw_action(action_view.data,task_frame[0])")
 rep("            assert bind(f,'jp_character_textures_release',I,U)(0)==0", "            struct.pack_into('<H',toe_player,toe_layout[1],0)\n            assert bind(f,'jp_sonic_toes_step',I,P)(toe_player)==0 and bind(f,'jp_sonic_toes_close',I)()==0\n            assert bind(f,'jp_character_textures_release',I,U)(0)==0")
 # Ignore exactly the writable toe destinations; verify every other byte and
 # all pointers/descriptors. Independent full-function VM proof checks values.
 anchor='    def data_hash():'
 masks='''    canonical={e.name.decode():e for e in entries};toe_mutable=[]
    for side in ('plus','minus'):
        prefix='sonic_toe_'+side+'_z';e=canonical[prefix];toe_mutable.append((e.data+Node.model.offset,e.data+Node.model.offset+C.sizeof(P)))
        m=Model.from_address(canonical[prefix+'_blend'].data)
        for pointer in (C.cast(m.points,P).value,C.cast(m.normals,P).value):toe_mutable.append((pointer,pointer+m.point_count*12))
        e=canonical[prefix+'_face_normals'];toe_mutable.append((e.data,e.data+e.host_bytes))
'''
 rep(anchor,masks+anchor)
 rep('        for e in entries:out.update(C.string_at(e.data,e.host_bytes))','''        for e in entries:
            chunk=bytearray(C.string_at(e.data,e.host_bytes))
            for lo,hi in toe_mutable:
                start=max(lo,e.data)-e.data;end=min(hi,e.data+e.host_bytes)-e.data
                if start<end:chunk[start:end]=b'\\x00'*(end-start)
            out.update(chunk)''')
 anchor='    # Load a real palette using the same actual original read queue/GDFS owner.'
 extra='''    for side in ('plus','minus'):
        for suffix in ('blend','shape_a','shape_b'):
            address=canonical['sonic_toe_'+side+'_z_'+suffix].data;model=Model.from_address(address);spans=(Span*model.mesh_count)()
            for i,mesh in enumerate(model.meshes[:model.mesh_count]):spans[i]=Span(extent(C.cast(mesh.indices,P).value,2),extent(mesh.uvs,4))
            models.append((address,model,spans))
'''
 rep(anchor,extra+anchor)
 rep('setmatrix(None,parent);observed.clear();draw_action(action_view.data,animation_frame)','setmatrix(None,parent);observed.clear();toe_control(animation_frame);draw_action(action_view.data,animation_frame)')
 raw=raw.replace('all2793 canonical model records remain byte-for-byte intact','all immutable canonical bytes and toe mutation guards remain intact').replace('all original motion and canonical model source records survive actual animation draws byte-for-byte','all motion bytes and immutable canonical bytes survive genuine toe mutation')
 marker="    (d/'result.json').write_text(json.dumps(out,indent=2)+'\\n');"
 rep(marker,"    out.update(status='genuine_full_speed_Sonic_original_toe_control_actual_task_GPU_and_texture_Off_frames_verified',native_original_toe_control_connected_to_real_rendering=True,original_action='sonic_full_speed_action',canonical_records=len(entries),original_models=28,visible_models=28,preflighted_model_variants=len(models),toe_control_samples=toe_samples,all_immutable_model_data_and_mutation_guards_preserved=True,whole_model_data_unchanged=False,all_original_model_and_motion_records_preserved=False,preflight_includes_all_six_genuine_alternate_toe_models=True,original_player_callback_or_gameplay_verified=False)\n"+marker)
 return raw,changes
def exercise(c,inputs,track):
 import verify_native_character_tree as tree
 raw,changes=derive();path=c['d']/'private_actual_toe_frame_body.py';path.write_text(raw);namespace=dict(vars(tree),__file__=__file__);exec(compile(raw,str(path),'exec'),namespace);namespace['exercise'](c,inputs,track)
 v=read(c['d']/'result.json');v.update(derived_exercise_sha256=sha(path),original_task_frame_verifier_sha256=sha(task_suite.__file__));(c['d']/'result.json').write_text(json.dumps(v,indent=2)+'\n')
def main():
 raw=inspect.getsource(frame_suite.main);old='from verify_native_action_frames import exercise';assert raw.count(old)==1;raw=raw.replace(old,'from verify_native_sonic_toes_frames import exercise');namespace=dict(vars(frame_suite),__file__=__file__);exec(compile(raw,__file__,'exec'),namespace);namespace['main']()
if __name__=='__main__':main()
