"""Exercise recovered original placement/audio callbacks; drawing scope separate."""
import argparse,ctypes as C,hashlib,json
from pathlib import Path
from rna_test_runtime import Runtime
from native_task_test_support_v2 import Task,Work
from verify_native_audio import bind
I,U,Q,P=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256'];labels=[];rt=None
    def check(ok,label):assert ok,label;labels.append(label)
    try:
        rt=Runtime(b,a.track,check);lib=rt.lib;camera=P.in_dll(rt.frames,'camera_work');saved=camera.value
        viewer=Work();work=Work();task=Task();task.work=C.addressof(work);camera.value=C.addressof(viewer)
        zone=bind(lib,'sound_fx_zone_step',None,P);orbit=bind(lib,'sound_orbit_step',None,P);leave=bind(lib,'placement_try_leave',I,P)
        state=lambda:rt.peer('peer_game_sound',18)
        work.scl.x=20;work.scl.y=8;zone(C.byref(task));check(state()[3]==8,'inside original zone requests configured real effect level')
        bind(lib,'sound_request_fx',None,I)(-6);work.pos.x=20;zone(C.byref(task));check(C.c_int32(state()[3]).value==-6,'exact sphere boundary is excluded by original strict comparison')
        work.pos.x=1000;departed=leave(C.byref(task));address=task.step
        # Plain original function declarations can take an import thunk's
        # address. Resolve its real x64 indirect jump before checking identity.
        if address and C.string_at(address,2)==b'\xff\x25':
            displacement=C.c_int32.from_address(address+2).value;address=P.from_address(address+6+displacement).value
        check(departed==1 and address==C.cast(rt.frames.task_destroy,P).value,'original far placement schedules actual task_destroy callback through native import')
        task.step=None;I.in_dll(rt.frames,'frame_redrawing').value=1
        try:check(leave(C.byref(task))==0 and not task.step,'split picture prevents original deferred departure')
        finally:I.in_dll(rt.frames,'frame_redrawing').value=0
        work.pos.x=0;work.scl.x=10;work.scl.y=0;work.ang.x=17;work.ang.y=0x4000
        orbit(C.byref(task));check(work.ang.z==0x4000,'original orbit increments actual task angle')
        out=(C.c_int64*13)();check(bind(lib,'peer_effects_slot',I,U,P)(0,out)==0,'read same actual sound-slot table')
        check(out[2]==C.addressof(task) and out[2]>0xffffffff and out[4]==17 and out[1]==3,'orbit request preserves full native task identity and original lifetime')
        check(bind(lib,'placement_editing',I,P)(C.byref(task))==0,'original editor membership initially empty')
        bind(lib,'peer_sound_editor_slot',None,P)(C.byref(task));check(bind(lib,'placement_editing',I,P)(C.byref(task))==1,'original editor membership reads real recovered slot table')
        bind(lib,'peer_sound_editor_slot',None,P)(None);camera.value=saved;rt.close();rt=None
    finally:
        if rt:camera.value=saved;rt.close()
    result=dict(status='original_sound_placement_non_drawing_behavior_verified',checks=len(labels),assertions=labels,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),editor_render_branches_verified=False,composite_rotation_bitwise_SH4_verified=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(labels),'checks',flush=True)
if __name__=='__main__':main()
