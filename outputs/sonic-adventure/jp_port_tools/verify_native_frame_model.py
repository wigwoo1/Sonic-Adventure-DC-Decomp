"""Run the frozen original-model comparison with frame retention ENABLED.
The original proof remains its own immutable file; this report binds that
proof to capture/release of the actual final model's owned command stream.
"""
import argparse,ctypes as C,hashlib,json,sys
from pathlib import Path
import verify_native_model_rz as original_model_suite
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','numeric-dll','report']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();a.report=a.report.resolve();assert not a.report.exists()
    source_report=a.report.with_name(a.report.stem+'_original_model.json');assert not source_report.exists()
    b=json.loads((d/'build_verification.json').read_text());dll=Path(b['DLL']);assert sha(dll)==b['DLL_sha256']
    lib=C.CDLL(str(dll));I=C.c_int32;U=C.c_uint32;S=C.c_size_t;P=C.c_void_p;Q=C.c_uint64
    def bind(n,result,*args):
        f=getattr(lib,n);f.restype=result;f.argtypes=list(args);return f
    init=bind('jp_frames_initialize',I,U,U,S);status=bind('jp_frames_status',I);shutdown=bind('jp_frames_shutdown',I);submit=bind('jp_frames_submit',I,C.POINTER(Q))
    data=bind('jp_queue_data',P,U);used=bind('jp_queue_committed_bytes',S,U);get=bind('jp_frame_list',P,Q,U,C.POINTER(S));surface=bind('jp_frame_surface',P,Q,U,C.POINTER(S));count=bind('jp_frame_surface_count',U,Q)
    release=bind('jp_frame_release',I,Q);pin=bind('jp_frames_pinned_bytes',S);orphan=bind('jp_frames_orphaned_bytes',S);ts=bind('jp_textures_shutdown',None)
    assert init(4,64,32<<20)==0
    oldargv=sys.argv;sys.argv=[original_model_suite.__file__]
    for n in ['workspace','upstream','build','binary','numeric_dll']:sys.argv+=['--'+n.replace('_','-'),str(getattr(a,n))]
    sys.argv+=['--report',str(source_report)]
    try:original_model_suite.main()
    finally:sys.argv=oldargv
    proof=json.loads(source_report.read_text());assert proof['checks']==200669 and proof['original_entry_calls']==3485 and status()==0
    before=[C.string_at(data(i),used(i)) if used(i) else b'' for i in range(5)]
    assert any(before) and pin()==128
    # The final suite case is a complete real model draw. Destroy its original
    # owner BEFORE submission, then consume the retained command snapshot.
    ts();assert orphan()==128
    fid=Q();assert submit(C.byref(fid))==0 and count(fid.value)==1
    checks=0
    for i,raw in enumerate(before):
        size=S();ptr=get(fid.value,i,C.byref(size));assert size.value==len(raw) and (C.string_at(ptr,size.value) if ptr else b'')==raw;checks+=1
    # Its first real textured header carries the native surface ID in TCW.
    word=int.from_bytes(before[0][12:16],'little')&0x1fffff;size=S();ptr=surface(fid.value,word,C.byref(size))
    assert size.value==128 and C.string_at(ptr,128)==bytes(128);checks+=1
    assert release(fid.value)==0 and pin()==orphan()==0 and shutdown()==0;checks+=1
    report=dict(status='complete_original_model_contracts_pass_with_frame_retention_enabled',checks=checks,original_model_checks=proof['checks'],original_model_entry_calls=proof['original_entry_calls'],
        original_model_report=str(source_report.relative_to(r)),original_model_report_sha256=sha(source_report),frozen_model_verifier_sha256=sha(Path(original_model_suite.__file__)),
        final_real_model_captured_after_texture_shutdown=True,DLL_sha256=sha(dll),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),
        render_flip_implemented=False,full_frame_executed=False,rendered_pixels_verified=False,playable=False,overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],proof['checks'],'original-model checks plus',checks,'capture checks',flush=True)
if __name__=='__main__':main()
