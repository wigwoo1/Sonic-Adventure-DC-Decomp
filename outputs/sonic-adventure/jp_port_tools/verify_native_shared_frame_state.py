"""Prove original pacing and renderer controls address one counter owner."""
import argparse,ctypes as C,hashlib,json
from pathlib import Path
from build_native_texture_runtime import function
from native_packet_test_support import bind
from native_texture_test_support import Runtime,List
from native_present_test_support import Present
from native_clock_test_support import Counter
from verify_native_frame_wait import Snapshot
from verify_native_model_preflight import Vec,Material,Mesh,Model,Span,Bound
from native_video_test_support import Video
U=C.c_uint32;I=C.c_int;P=C.c_void_p;Idle=C.CFUNCTYPE(I,P,U)
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();assert not a.report.exists()
    b=json.loads((d/'build_verification.json').read_bytes());assert b['exit_code']==b['diagnostic_bytes']==0
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ['frames_DLL','DLL','GPU_DLL']:assert sha(b[n])==b[n+'_sha256']
    t=Runtime(Path(b['frames_DLL']));gpu_library=C.CDLL(b['GPU_DLL'])
    present=Present(Path(b['DLL']));counters=[I.in_dll(t.dll,n) for n in b['shared_frame_counters']]
    check_count=0;groups={}
    def check(ok,label):
        nonlocal check_count
        check_count+=1;assert ok,(check_count,label)
    check(len({C.addressof(x) for x in counters})==4,'four distinct scalars in single real frame-state owner')
    check(b['counter_definitions_by_object']=={'frame_state.obj':sorted(b['shared_frame_counters']),'frame_probe.obj':[],'diagnostic.obj':[],'frame_pacing.obj':[]},'no counter storage in either diagnostic object or original pacing object')
    for index,cell in enumerate(counters):
        for value in [-2147483648,-65536,-1,0,1,2,7,65535,2147483647]:
            cell.value=value
            # Original counter accessor uses countdown, interval, steps, step.
            mapped=[1,0,2,3][index]
            check(present.counter(mapped)==value,('presenter reads exact exported frame-state bits',index,value))
    renderer=bind(t.dll,'jp_sprite_probe_frame',None,I,I)
    for step in [-2,-1,0,1,2,3,2147483647]:
        renderer(step,0);check(counters[3].value==present.counter(3)==step,'renderer test control writes same original pacing step')
    set_countdown=bind(present.lib,'jp_wait_probe_countdown',None,I)
    for value in [-17,0,1,1024]:set_countdown(value);check(counters[1].value==value,'wait control writes same original exported countdown')
    groups['cross_DLL_native_data_identity']=check_count
    source=json.loads((d/'build_verification.json').read_bytes())['private_input_sources_sha256']
    original=next(r/n for n in source if n.endswith('external_frame_native.c'))
    hashes={}
    for name in ['frame_pacing_init','frame_set_interval','frame_wait','frame_wait_step','frame_vsync']:
        before=function(original.read_bytes(),name);after=function((d/'external_frame_native.c').read_bytes(),name)
        check(before==after,'original frame routine body unchanged: '+name);hashes[name]=hashlib.sha256(after).hexdigest()
    counter=Counter();check(counter.start(present.clock,781250)==0,'deterministic source for real clock provider')
    @Idle
    def idle(context,milliseconds):
        counter.tick+=(milliseconds*781250+999)//1000 if milliseconds else 1
        return 1
    check(present.wait_init(60,1,8,C.cast(idle,P),None)==0,'actual native wait owner initialized')
    present.probe(0,0)
    set_interval=bind(present.lib,'frame_set_interval',None,I)
    for interval,steps in [(1,1),(2,2),(4,3),(7,1)]:
        for cell in counters:cell.value=99
        present.pacing(interval,steps)
        check([x.value for x in counters]==[interval,interval,steps,0],'actual original pacing writes all shared counter fields')
        check([present.counter(i) for i in range(4)]==[interval,interval,steps,0] and present.wait_status()==0,'presenter observes real pacing initialization')
        set_interval(interval+1)
        check([x.value for x in counters]==[interval+1,interval+1,steps,0],'actual original set_interval updates shared storage only')
    present.pacing(3,2);snap=bind(present.lib,'jp_wait_snapshot',I,P);pump=bind(present.lib,'jp_wait_pump',I,P)
    delivered_total=0
    for expected in [2,1,0,-1]:
        state=Snapshot();check(snap(C.byref(state))==0,'real rational wait deadline')
        counter.tick=state.next_deadline;delivered=U()
        check(pump(C.byref(delivered))==0 and delivered.value==1,'one actual registered original vsync callback')
        delivered_total+=delivered.value
        check(counters[1].value==present.counter(0)==expected,'actual original vsync mutates shared renderer/pacing countdown')
    check(delivered_total==4 and counters[3].value==0 and counters[2].value==2,'vsync preserves original separate step semantics')
    groups['unchanged_original_pacing_set_interval_and_real_vsync']=check_count-sum(groups.values())
    # Exercise the actual model parent's FRAME_DRAWS gate against pacing writes.
    lib=t.dll;F=C.c_float;S=C.c_size_t
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I)
    ri=bind(lib,'jp_render_state_initialize',I,U,S);rs=bind(lib,'jp_render_state_shutdown',None)
    used=bind(lib,'jp_queue_committed_bytes',S,U);qstatus=bind(lib,'jp_queue_status',I)
    measure=bind(lib,'jp_model_preflight',I,P,S,S,S,S,P,P);reserve=bind(lib,'jp_model_reserve',I,P)
    draw=bind(lib,'draw_model',None,P);calls=bind(lib,'jp_polygon_probe_calls',U)
    check(qi(8<<20)==0 and fi(4,32,8<<20)==0 and ri(128,1<<20)==0 and Video(lib).init(640,480,0xff000000,1)==0,'actual model-policy owners')
    texture_list=bind(lib,'jp_sprite_probe_prepare_texture',P)()
    check(bool(texture_list) and t.select(C.cast(texture_list,C.POINTER(List)))==1,'actual synthetic owned texture fixture selected')
    check(bind(lib,'jp_ninja_contexts_initialize',I)()==0,'original Ninja contexts for actual parent')
    identity=(F*16)(*[1. if i%5==0 else 0. for i in range(16)])
    bind(lib,'njSetMatrix',None,P,P)(None,identity);bind(lib,'draw_model_init',None)();bind(lib,'draw_set_light',None,F,F,F)(0,0,1)
    I.in_dll(lib,'event_skipping').value=0
    pts=(Vec*8)(*(Vec((i%3-1)*.25,(i%2-.5)*.25,.5 if i%4==0 else 2.) for i in range(8)))
    normals=(Vec*8)(*(Vec(0,0,1) for i in range(8)));meshes=(Mesh*3)();materials=(Material*3)();spans=(Span*3)();arrays=[]
    for j in range(3):
        indices=(C.c_int16*3)(j,j+1,j+2);uvs=(C.c_int16*6)(0,0,255,0,128,255);arrays.extend([indices,uvs])
        meshes[j]=Mesh(j,1,indices,None,None,None,C.cast(uvs,P));materials[j]=Material(0xffffffff,0,1.,0,0x200000|(0x400000 if j==2 else 0)|(0x100000 if j==1 else 0));spans[j]=Span(3,3)
    model=Model(pts,normals,8,meshes,materials,3,3,Vec(0,0,2),3.);bound=Bound()
    check(measure(C.byref(model),8,8,3,3,spans,C.byref(bound))==0,'actual parent model preflight')
    policies=[(0,0,True),(1,0,False),(1,1,True),(2,1,False)]
    for step,later,expected in policies:
        check(reset()==0 and reserve(C.byref(bound))==0,'actual model queue reservation')
        renderer(0,later);counters[3].value=step;before=calls();draw(C.byref(model))
        check(present.counter(3)==step and bool(sum(used(i) for i in range(5)))==expected and calls()==before+int(expected),'complete original model parent uses same actual pacing step and later-draw gate')
        check(qstatus()==t.status()==0,'model gate preserves real native owner status')
    present.pacing(2,2);check(reset()==0 and reserve(C.byref(bound))==0,'real pacing restores draw-ready step zero')
    before=calls();draw(C.byref(model));check(counters[3].value==0 and calls()==before+1 and sum(used(i) for i in range(5))>0,'actual original pacing restores model parent drawing')
    qs();rs();t.shutdown();check(fs()==0,'actual model-policy owners drained')
    groups['complete_model_parent_observes_original_pacing_state']=check_count-sum(groups.values())
    check(present.wait_shutdown()==0,'actual wait owner drained');present.clock.shutdown()
    groups['shutdown']=check_count-sum(groups.values())
    result=dict(status='one_original_frame_counter_owner_cross_DLL_renderer_pacing_and_actual_vsync_verified',checks=check_count,groups=groups,actual_original_vsync_callbacks=delivered_total,complete_original_model_parent_policy_calls=5,original_frame_function_body_sha256=hashes,one_frame_counter_owner=True,renderer_and_presenter_read_same_step=True,original_pacing_bodies_unchanged=True,actual_task_execution_or_real_replay_integrated=False,diagnostic_out_of_scope_services_remain=True,build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))


if __name__=='__main__':main()
