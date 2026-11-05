"""Existing real startup suite with full original global initialization entry.
Only the Python caller selects the newly linked real provider; no native body
or callback is replaced. Fresh setup occurs before wait/vsync callbacks start.
"""
import ctypes as C,hashlib,json,struct,sys
from pathlib import Path
import verify_native_game_text_startup as parent
import verify_native_exit_logo as leaf
from native_packet_test_support import bind
from verify_native_ninja_global import Screen,Control,LightQueue
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    argv=sys.argv[1:];out=Path(argv[argv.index('--output')+1]).resolve();worker='--worker' in argv
    if worker:
        original=leaf.bind
        def select(lib,name,result,*types):
            if name!='jp_ninja_contexts_initialize':return original(lib,name,result,*types)
            real=original(lib,'jp_ninja_global_initialize',result,*types)
            def initialize():
                code=real();s=Screen.in_dll(lib,'_nj_screen_');material=bytes((C.c_ubyte*52).in_dll(lib,'_nj_light_material_'));src=(C.c_uint32*16).in_dll(lib,'_nj_cnk_easy_multilight_buffer_');dst=(C.c_uint32*32).in_dll(lib,'_nj_cnk_easy_multilight_buffer_dst_')
                assert code==0 and bind(lib,'jp_ninja_global_status',C.c_int)()==0
                assert (s.distance,s.width,s.height,s.cx,s.cy)==(500,640,480,320,240)
                assert material==struct.pack('<13f',1,0,0,0,1,.5,.5,.5,1,.5,.5,.5,6)
                assert src[2]==0xbf800000 and dst[0]==3
                assert Control.in_dll(lib,'_nj_light_control_').count==LightQueue.in_dll(lib,'_nj_light_queue_').count==0
                (out/'ninja_global_setup.json').write_text(json.dumps(dict(status='actual_complete_original_Ninja_global_init_before_vertex_font_and_boot',checks=5,actual_native_entry='jp_ninja_global_initialize',original_global_and_light_bodies_executed=True,old_partial_context_helper_used=False,native_function_bodies_replaced=False,setup_precedes_native_wait_callback_registration=True,verifier_sha256=sha(__file__)),indent=2)+'\n')
                return code
            return initialize
        leaf.bind=select
    baseline=sha(parent.__file__);parent.__file__=__file__;parent.main()
    if worker:raise AssertionError('native process exit returned')
    report=out/'verification.json';v=json.loads(report.read_bytes());assert v['checks']==7726 and v['actual_GPU_images']==v['actual_Presents']==570
    extra=0
    for case in ('movie0','movie6','advertise1','cleanup','quit','pending_quit'):
        q=json.loads((out/case/'ninja_global_setup.json').read_bytes());assert q['checks']==5 and q['verifier_sha256']==sha(__file__) and not q['native_function_bodies_replaced'];extra+=q['checks']
    v.update(status='complete_original_Ninja_global_then_actual_vertex_font_boot_logo_and_exit_verified',checks=v['checks']+extra,actual_complete_original_global_calls=6,actual_original_lighting_init_calls=6,actual_global_setup_checks=extra,prior_vertex_font_game_style_checks=7726,baseline_startup_verifier_sha256=baseline,verifier_sha256=sha(__file__),native_functions_replaced=False,full_njInitSystem_or_sbInitSystem_executed=False)
    report.write_text(json.dumps(v,indent=2)+'\n');print(v['status'],v['checks'],'checks',flush=True)
if __name__=='__main__':main()
