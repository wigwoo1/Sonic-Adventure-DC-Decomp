"""Run all original sprite contracts with retained owners enabled, including
real culling/discard paths. Keep the original instruction proof separate.
"""
import argparse,ctypes as C,hashlib,json,sys
from pathlib import Path
import verify_native_sprites as sprite_suite
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','numeric-dll','report']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();a.report=a.report.resolve();assert not a.report.exists()
    proofpath=a.report.with_name(a.report.stem+'_original_sprites.json');assert not proofpath.exists()
    b=json.loads((d/'build_verification.json').read_text());dll=Path(b['DLL']);assert sha(dll)==b['DLL_sha256'];lib=C.CDLL(str(dll))
    lib.jp_frames_initialize.argtypes=[C.c_uint32,C.c_uint32,C.c_size_t];lib.jp_frames_initialize.restype=C.c_int
    for n in ['jp_frames_pinned_bytes','jp_frames_orphaned_bytes','jp_frames_accounted_bytes']:getattr(lib,n).restype=C.c_size_t
    assert lib.jp_frames_initialize(4,64,32<<20)==0
    oldargv=sys.argv;sys.argv=[sprite_suite.__file__]
    for n in ['workspace','upstream','build','binary','numeric_dll']:sys.argv+=['--'+n.replace('_','-'),str(getattr(a,n))]
    sys.argv+=['--report',str(proofpath)]
    try:sprite_suite.main()
    finally:sys.argv=oldargv
    proof=json.loads(proofpath.read_text());assert proof['checks']==82489 and proof['original_draw_calls']==1536
    assert lib.jp_frames_status()==0 and lib.jp_frames_pinned_bytes()==lib.jp_frames_orphaned_bytes()==lib.jp_frames_live_count()==0
    assert lib.jp_frames_shutdown()==0 and lib.jp_frames_accounted_bytes()==0
    report=dict(status='complete_original_sprite_contracts_pass_with_frame_retention_enabled',original_sprite_checks=82489,original_sprite_draws=1536,
        all_eight_original_triangle_clipping_cases=proof['all_eight_triangle_clipping_table_indices_reached'],original_near_clip_masks=proof['original_near_clip_masks'],
        source_report=str(proofpath.relative_to(r)),source_report_sha256=sha(proofpath),frozen_sprite_verifier_sha256=sha(Path(sprite_suite.__file__)),verifier_sha256=sha(Path(__file__)),
        DLL_sha256=sha(dll),build_report_sha256=sha(d/'build_verification.json'),no_resource_or_recording_leaks_after_real_culled_and_drawn_paths=True,
        render_flip_implemented=False,full_frame_executed=False,rendered_pixels_verified=False,playable=False,overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],'82489 checks; 1536 original sprite draws; no retained-resource leaks',flush=True)
if __name__=='__main__':main()
