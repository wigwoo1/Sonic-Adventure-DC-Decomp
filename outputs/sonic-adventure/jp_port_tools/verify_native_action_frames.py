"""Derive a real full-parent caller;render sampled original motion to GPU/APNG."""
import argparse,ctypes as C,hashlib,inspect,json,os,subprocess,sys
from pathlib import Path
import verify_native_character_tree as tree_suite
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def derive():
    raw=inspect.getsource(tree_suite.exercise);changes=[]
    def rep(old,new,label):
        nonlocal raw
        assert raw.count(old)==1,(label,raw.count(old));raw=raw.replace(old,new);changes.append(label)
    anchor="    entries=[];get=bind(motions,'jp_motion_models_entry',I,U,P)"
    added='''    from verify_native_action_poses import Action,Motion
    from PIL import Image
    action_view=View();check(bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)(0,b'sonic_updraft_action',C.byref(action_view))==0,'actual original Sonic action view retained alongside canonical model/texture consumers')
    action=Action.from_address(action_view.data);motion=Motion.from_address(action.motion)
    check(action.object==view.data and motion.frames==33,'actual genuine action uses canonical Sonic tree and full33-frame original motion')
    draw_action=bind(f,'draw_action',None,P,F);sample_frames=(0.,4.,8.5,12.,16.,20.,24.,28.,32.);pose_digests=[];animation_images=[];pose_callback_errors=[]
    motion_entries=[];motion_get=bind(motions,'jp_motion_s_mot_entry',I,U,P)
    for i in range(bind(motions,'jp_motion_s_mot_count',U)()):
        e=Entry();assert motion_get(i,C.byref(e))==0;motion_entries.append(e)
    def motion_hash():
        h=hashlib.sha256()
        for e in motion_entries:h.update(C.string_at(e.data,e.host_bytes))
        return h.hexdigest()
    original_motion_hash=motion_hash()
'''
    rep(anchor,added+anchor,'retain real action and canonical data references')
    start=raw.index('    def measure(at,parent):');end=raw.index('    measure(view.data,identity);',start)
    measure='''    def measure(at,parent):
        observed=[];callback_pointer=P.in_dll(f,'_nj_motion_callback_');previous=callback_pointer.value
        callback_type=C.CFUNCTYPE(None,P)
        @callback_type
        def observer(pointer):
            try:
                n=Node.from_address(pointer)
                if n.model and not n.flags&8:
                    matrix=(F*16)();getmatrix(matrix);observed.append(bytes(matrix));model=Model.from_address(n.model)
                    for v in model.points[:model.point_count]:world.append(tuple(v.x*matrix[j]+v.y*matrix[4+j]+v.z*matrix[8+j]+matrix[12+j] for j in range(3)))
            except BaseException as error:pose_callback_errors.append(str(error))
        phase=C.c_int32.in_dll(f,'frame_step').value;later=C.c_int32.in_dll(f,'frame_later_draws').value
        # Camera fitting uses the actual original non-rasterizing frame policy.
        # Visible captures below restore the real inherited logical clock phase.
        bind(f,'jp_sprite_probe_frame',None,I,I)(1,0);callback_pointer.value=C.cast(observer,P).value
        for animation_frame in sample_frames:
            setmatrix(None,parent);observed.clear();draw_action(action_view.data,animation_frame)
            check(not pose_callback_errors and len(observed)==28,'actual native original action produces complete visible pose for camera fitting')
            pose_digests.append(hashlib.sha256(b''.join(observed)).hexdigest())
        callback_pointer.value=previous;bind(f,'jp_sprite_probe_frame',None,I,I)(phase,later)
        check(len(set(pose_digests))>=3,'genuine original keyframes produce multiple distinct matrix poses including fractional input')
'''
    raw=raw[:start]+measure+raw[end:];changes.append('measure all actual keyed poses for one fixed diagnostic camera')
    rep("    bind(f,'draw_object',None,P,P)(view.data,overrides)","    draw_action(action_view.data,sample_frames[0])",'execute original animation interpreter for actual captured draw')
    marker="    off=Config.make(512,512,textures=0);off.logical_width=off.logical_height=512;off_job=Q();off_report=Report();"
    loop='''    animation_images.append(dict(frame=sample_frames[0],path=str(path),sha256=sha(path),RGBA_sha256=hashlib.sha256(rgba).hexdigest(),packet_counts=counts))
    for animation_frame in sample_frames[1:]:
        check(bind(f,'jp_frame_release',I,Q)(frame.value)==0 and bind(f,'jp_queue_reset_frame',I)()==0,'previous actual frame retires before next motion draw')
        check(bind(f,'jp_model_reserve',I,P)(C.byref(bound))==0 and bind(f,'jp_frames_prepare_textures',I,P,U)(words,len(words))==0,'whole original model bounds and genuine texture surfaces reserved for next animated sample')
        setmatrix(None,before_matrix);draw_action(action_view.data,animation_frame)
        check(bind(f,'jp_render_state_check_guards',I)()==0 and bind(f,'jp_queue_status',I)()==0 and t.status()==0,'actual keyed motion draw preserves renderer guards and provider states')
        getmatrix(after_matrix);check(bytes(after_matrix)==bytes(before_matrix) and C.c_int32.in_dll(f,'_stack_size_').value==depth,'each original animated tree restores complete caller matrix and stack')
        frame=Q();check(bind(f,'jp_frames_submit',I,P)(C.byref(frame))==0,'actual animated packets captured into next owned frame');plan=P();failure=PacketFailure()
        check(decoder.frame(frame.value,64<<20,C.byref(plan),C.byref(failure))==0,'actual animated packet stream decodes');decoded=C.cast(plan,C.POINTER(Plan)).contents;counts=dict(batches=decoded.batch_count,vertices=decoded.vertex_count,triangles=decoded.triangle_count);check(counts['triangles']>100,'actual keyed pose emits substantial genuine model geometry');decoder.destroy(plan)
        job=Q();report=Report();check(jobs.submit(handle,frame.value,C.byref(cfg),C.byref(job),C.byref(report),C.byref(error))==0,'actual native GPU job renders next original animation sample');jobs.wait(job.value)
        check(jobs.copy(window,job.value,C.byref(error))==0 and bind(gl,'jp_window_readback',I,P,P,S,P)(window,output,len(output),C.byref(error))==0,'actual animated Win32 backbuffer captured before Present')
        rgba=output.raw;path=d/('sonic_action_'+str(animation_frame).replace('.','_')+'.png');png(path,rgba,512,512);animation_images.append(dict(frame=animation_frame,path=str(path),sha256=sha(path),RGBA_sha256=hashlib.sha256(rgba).hexdigest(),packet_counts=counts))
        check(bind(gl,'jp_window_present',I,P,U,P)(window,1,C.byref(error))==0 and jobs.release(handle,job.value,C.byref(error))==0,'actual animated image Presents and completed GPU result retires')
    check(len(animation_images)==9 and len({row['RGBA_sha256'] for row in animation_images})>=3,'nine actual original motion samples render distinct real GPU images')
    check(motion_hash()==original_motion_hash and data_hash()==original_model_hash,'all original motion and canonical model source records survive actual animation draws byte-for-byte')
'''
    rep(marker,loop+marker,'render nine actual native action frames with retained jobs and inputs')
    rep("and bind(motions,'jp_motions_release',I,Q)(view.token)==0 and bind(f,'jp_character_switch_retire',I)()==0", "and bind(motions,'jp_motions_release',I,Q)(action_view.token)==0 and bind(motions,'jp_motions_release',I,Q)(view.token)==0 and bind(f,'jp_character_switch_retire',I)()==0",'retire action/model/texture views only after real GPU/frame results')
    marker="    (d/'result.json').write_text(json.dumps(out,indent=2)+'\\n');"
    final='''    frames=[Image.open(row['path']).convert('RGBA') for row in animation_images];preview=d/'sonic_original_action_preview.png'
    frames[0].save(preview,save_all=True,append_images=frames[1:],duration=100,loop=0,disposal=0,blend=0)
    encoded=Image.open(preview);check(encoded.n_frames==len(frames),'lossless animated PNG contains all actual GPU captures')
    for i,image in enumerate(frames):encoded.seek(i);check(encoded.convert('RGBA').tobytes()==image.tobytes(),'animated PNG frame bytes equal actual native capture')
    out.update(status='genuine_original_Sonic_action_keyed_animation_full_parent_retained_GPU_and_actual_window_samples_verified',checks=len(c['labels']),assertions=c['labels'],rendered_original_action_frames=animation_images,pose_matrix_sha256=pose_digests,animated_preview=dict(path=str(preview),sha256=sha(preview),frames=len(frames),lossless_RGBA_frames_verified=True,preview_duration_ms_per_sample=100,preview_is_not_measured_game_FPS=True),actual_window_presents=10,actual_original_keyed_animation_rendered=True,original_gameplay_task_FSM_or_main_loop_verified=False,camera_fitting_uses_explicit_original_frame_skip_policy=True,actual_visible_captures_use_restored_inherited_clock_phase=True,all_original_model_and_motion_records_preserved=True,automatic_task_render_view_acquisition=False,animation_main_loop_or_gameplay_verified=False)
'''
    rep(marker,final+marker,'encode and verify a lossless preview from actual captured frames')
    return raw,changes
def exercise(c,inputs,track):
    raw,changes=derive();path=c['d']/'private_action_caller_body.py';path.write_text(raw)
    namespace=dict(vars(tree_suite),__file__=__file__);exec(compile(raw,str(path),'exec'),namespace);namespace['exercise'](c,inputs,track)
    result=read(c['d']/'result.json');result.update(original_tree_verifier_sha256=sha(tree_suite.__file__),derived_exercise_sha256=sha(path),caller_adaptations=changes);(c['d']/'result.json').write_text(json.dumps(result,indent=2)+'\n')
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','inputs','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();b=read(a.build/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    original=p/'jp_port_tools/verify_native_game_init.py';raw=original.read_bytes();marker=b"        if a.case=='normal':\n";assert raw.count(marker)==1;prefix=raw.split(marker)[0]
    for old,new in [(b"('jp_textures_initialize',(32,1024,32<<20))",b"('jp_textures_initialize',(1024,4096,128<<20))"),(b"('jp_render_state_initialize',(128,1<<20))",b"('jp_render_state_initialize',(2048,16<<20))")]:assert prefix.count(old)==1;prefix=prefix.replace(old,new)
    caller=d/'private_actual_parent_caller.py';caller.write_bytes(prefix+('        from verify_native_action_frames import exercise\n        exercise(locals(), '+repr(str(a.inputs.resolve()))+', '+repr(str(a.track.resolve()))+")\nif __name__=='__main__':main()\n").encode());cmd=[sys.executable,'-B',str(caller),'--workspace',str(r),'--build',str(a.build.resolve()),'--track',str(a.track.resolve()),'--output',str(d/'run'),'--case','normal']
    run=subprocess.run(cmd,capture_output=True,timeout=240,env=dict(os.environ,PYTHONPATH=str(p/'jp_port_tools')),creationflags=subprocess.CREATE_NO_WINDOW);(d/'stdout.txt').write_bytes(run.stdout);(d/'stderr.txt').write_bytes(run.stderr);assert not run.returncode,(run.returncode,run.stderr.decode(errors='replace')[-7000:])
    out=read(d/'run/result.json');out.update(original_parent_verifier_sha256=sha(original),derived_caller_sha256=sha(caller));(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['checks'],'checks',flush=True)
if __name__=='__main__':main()
