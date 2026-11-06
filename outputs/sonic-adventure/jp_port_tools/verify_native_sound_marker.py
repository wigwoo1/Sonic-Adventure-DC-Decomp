"""Capture the real original sound-editor marker through native frame/GPU owners."""
import argparse,ctypes as C,hashlib,json,faulthandler
from pathlib import Path
from rna_test_runtime import Runtime
from native_task_test_support_v2 import Task,Work
from native_video_test_support import Video
from native_gpu_test_support import GPU,Config
from verify_native_audio import bind
from verify_native_gpu import png
I,U,Q,P,S,F=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p,C.c_size_t,C.c_float
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    fault=(d/'native_fault.txt').open('w');faulthandler.enable(file=fault,all_threads=True)
    b=json.loads((a.build/'build_verification.json').read_bytes());labels=[];rt=None;gpu=None
    for n,h in b['authored_source_sha256'].items():assert hashlib.sha256((r/n).read_bytes()).hexdigest()==h,n
    for n in ('game_sound_DLL','frames_DLL','GPU_DLL'):assert hashlib.sha256(Path(b[n]).read_bytes()).hexdigest()==b[n+'_sha256'],n
    def check(ok,label):assert ok,label;labels.append(label);print(label,flush=True)
    try:
        rt=Runtime(b,a.track,check);frame=rt.frames;lib=rt.lib
        video=Video(frame);check(video.init(640,480,0xff182030,1)==0,'real SDK video state')
        print('initializing real matrix stack',flush=True)
        stack=C.create_string_buffer(4096);bind(frame,'njInitMatrix',None,P,I,I)(stack,32,0)
        identity=(F*16)(*[1 if i%5==0 else 0 for i in range(16)])
        (F*5).in_dll(frame,'_nj_screen_')[:]=[640,640,480,320,240]
        (F*4).in_dll(frame,'_nj_2d_clip_')[:]=[0,0,639,479]
        for n in ('_nj_scr_aspect_y_','port_draw_aspect','port_view_aspect'):F.in_dll(frame,n).value=1
        F.in_dll(frame,'_nj_near_clip_').value=1;F.in_dll(frame,'_nj_far_clip_').value=2000
        U.in_dll(frame,'_nj_control_3d_flag_').value=0;U.in_dll(frame,'_nj_constant_attr_and_').value=0xffffffff;U.in_dll(frame,'_nj_constant_attr_or_').value=0
        print('initializing real model state',flush=True)
        bind(frame,'draw_model_init',None)();bind(frame,'draw_set_light',None,F,F,F)(0,0,1)
        print('opening actual GPU',flush=True)
        gpu=GPU(b['GPU_DLL']);viewer=Work();camera=P.in_dll(frame,'camera_work');saved=camera.value;camera.value=C.addressof(viewer)
        work=Work();work.pos.z=100;work.scl.x=10;work.scl.y=8;work.ang.x=17
        task=Task();task.work=C.addressof(work);results=[]
        for name,callback,selected in [('zone_selected','sound_fx_zone_step',True),('zone_visible','sound_fx_zone_step',False),('orbit_selected','sound_orbit_step',True),('orbit_visible','sound_orbit_step',False)]:
            check(bind(frame,'jp_queue_reset_frame',I)()==0,'reset actual command storage')
            bind(frame,'njSetMatrix',None,P,P)(None,identity)
            matrix=bind(frame,'jp_ninja_current_matrix_storage',P)();before=C.string_at(matrix,64);depth=I.in_dll(frame,'_stack_size_').value
            bind(lib,'peer_sound_editor_slot',None,P)(C.byref(task) if selected else None);C.c_int16.in_dll(lib,'edit_mode').value=0 if selected else 1
            print('calling '+name,flush=True)
            bind(frame,'draw_models_begin',None)()
            bind(lib,callback,None,P)(C.byref(task))
            check(bind(lib,'peer_sound_marker_status',I)()==0,name+': bounded original marker reservation')
            bind(frame,'draw_models_end',None)()
            check(bind(frame,'jp_queue_status',I)()==0,name+': queue remained within bounds')
            check(bind(frame,'jp_frames_status',I)()==0,name+': frame owner remained valid')
            check(C.string_at(matrix,64)==before and I.in_dll(frame,'_stack_size_').value==depth,name+': original callback restored matrix and stack depth')
            check(bind(frame,'jp_render_state_check_guards',I)()==0,name+': vertex scratch guards intact')
            if not selected:check((F*4).in_dll(frame,'_nj_constant_material_')[0]==0,name+': original visibility branch restored material alpha')
            total=sum(bind(frame,'jp_queue_committed_bytes',S,U)(lane) for lane in range(5));check(total>0,name+': original marker generated actual packets')
            fid=Q();check(bind(frame,'jp_frames_submit',I,P)(C.byref(fid))==0,'retain real frame')
            cfg=Config.make(640,480,0);cfg.logical_width=640;cfg.logical_height=480;cfg.clear_rgba[:]=[0.094,0.125,0.188,1]
            rc,image,report,error=gpu.image(fid.value,cfg)
            check(rc==0 and report.triangles>0,(name,'real GPU consumed marker',rc,error.code))
            check(len(set(image[i:i+4] for i in range(0,len(image),4)))>1,name+': marker produces visible pixels')
            png(d/(name+'.png'),640,480,image);results.append(dict(case=name,packet_bytes=total,triangles=report.triangles,image_sha256=hashlib.sha256(image).hexdigest()))
            check(bind(frame,'jp_frame_release',I,Q)(fid.value)==0,'release captured frame')
        # Insufficient real vertex capacity must skip the marker safely. These
        # are explicit rejection fixtures, not replacement rendering behavior.
        check(bind(frame,'jp_render_state_initialize',I,U,S)(64,1<<20)==0,'install smaller real vertex capacity for rejection fixture')
        for callback in ('sound_fx_zone_step','sound_orbit_step'):
            for selected in (True,False):
                check(bind(frame,'jp_queue_reset_frame',I)()==0,'reset rejection fixture')
                bind(frame,'njSetMatrix',None,P,P)(None,identity);before=C.string_at(matrix,64);depth=I.in_dll(frame,'_stack_size_').value
                bind(lib,'peer_sound_editor_slot',None,P)(C.byref(task) if selected else None);C.c_int16.in_dll(lib,'edit_mode').value=0 if selected else 1
                bind(lib,callback,None,P)(C.byref(task))
                check(bind(lib,'peer_sound_marker_status',I)()!=0,'actual preflight refuses undersized vertex scratch')
                check(sum(bind(frame,'jp_queue_committed_bytes',S,U)(lane) for lane in range(5))==0,'rejected marker wrote no packets')
                check(C.string_at(matrix,64)==before and I.in_dll(frame,'_stack_size_').value==depth,'rejected callback balanced original matrix state')
                check(bind(frame,'jp_render_state_check_guards',I)()==0,'rejected marker preserved scratch guards')
                if not selected:check((F*4).in_dll(frame,'_nj_constant_material_')[0]==0,'rejected visibility branch restored material alpha')
        camera.value=saved;C.c_int16.in_dll(lib,'edit_mode').value=0;bind(lib,'peer_sound_editor_slot',None,P)(None)
        gpu.close(gpu.handle);gpu=None;video.shutdown();rt.close();rt=None
    finally:
        if gpu:gpu.close(gpu.handle)
        if rt:camera.value=saved;rt.close()
    faulthandler.disable();fault.close()
    result=dict(status='original_sound_marker_editor_branches_rendered_through_actual_native_GPU',checks=len(labels),assertions=labels,cases=results,undersized_vertex_scratch_rejected=True,build_verification_sha256=hashlib.sha256((a.build/'build_verification.json').read_bytes()).hexdigest(),verifier_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),gameplay_or_final_stage_lighting_verified=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(labels),'checks')
if __name__=='__main__':main()
