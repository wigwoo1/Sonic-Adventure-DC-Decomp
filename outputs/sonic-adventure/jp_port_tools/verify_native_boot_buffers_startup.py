"""Existing reset/boot/logo/exit suite with real main-owned buffer setup."""
import ctypes as C,hashlib,json,sys
from pathlib import Path
import verify_native_display_reset_boot as parent
from native_packet_test_support import bind
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    baseline=sha(parent.__file__);argv=sys.argv[1:];out=Path(argv[argv.index('--output')+1]).resolve();worker='--worker' in argv
    if worker:
        previous=parent.frozen.bind
        def select(lib,name,result,*types):
            native=previous(lib,name,result,*types)
            if name!='jp_render_state_initialize':return native
            def initialize(*args):
                code=native(*args)
                if code:return code
                buffer=C.addressof((C.c_byte*0x40000).in_dll(lib,'texture_buffer'));shape=C.addressof((C.c_float*6144).in_dll(lib,'shape_buffer'))
                assert bind(lib,'jp_render_state_register_buffer',C.c_int,C.c_void_p,C.c_size_t,C.c_uint32)(buffer,0x40000,4096)==0
                bind(lib,'njInitTextureBuffer',None,C.c_void_p,C.c_uint32)(buffer,0x40000);bind(lib,'njInit3D',None,C.c_void_p,C.c_int)(buffer,4096);bind(lib,'njInitShape',None,C.c_void_p)(shape);bind(lib,'njSetTextureMemorySize',None,C.c_uint32)(0x11c000);bind(lib,'njSetBorderColor',None,C.c_uint32)(0)
                assert C.c_void_p.in_dll(lib,'_nj_texture_buffer_st').value==C.c_void_p.in_dll(lib,'_nj_vertex_buf_').value==buffer
                assert C.c_void_p.in_dll(lib,'_nj_shape_buf_').value==shape and C.c_uint32.in_dll(lib,'_nj_texture_buffer_max_size').value==0x40000
                assert C.c_int.in_dll(lib,'_nj_vertex_num_').value==4096 and bind(lib,'jp_render_state_capacity',C.c_uint32)()==2048
                assert bind(lib,'jp_render_state_status',C.c_int)()==0 and bind(lib,'jp_render_state_buffer_is_borrowed',C.c_int)()==1
                (out/'boot_buffer_setup.json').write_text(json.dumps(dict(status='actual_main_boot_buffer_setup_before_original_reset_and_boot',checks=5,native_functions_replaced=False,baseline_reset_boot_verifier_sha256=baseline,wrapper_sha256=sha(__file__)),indent=2)+'\n');return 0
            return initialize
        parent.frozen.bind=select
    parent.__file__=__file__;parent.main()
    if worker:raise AssertionError('original native process exit returned')
    path=out/'verification.json';result=json.loads(path.read_bytes());assert result['checks']==5959;extra=0
    for case in ['movie0','movie6','advertise1','cleanup','quit','pending_quit']:
        row=json.loads((out/case/'boot_buffer_setup.json').read_bytes());assert row['checks']==5 and row['baseline_reset_boot_verifier_sha256']==baseline and row['wrapper_sha256']==sha(__file__) and not row['native_functions_replaced'];extra+=row['checks']
    result.update(status='actual_main_buffer_setup_then_original_reset_boot_logo_and_exit_verified',checks=result['checks']+extra,prior_reset_boot_logo_checks=5959,actual_main_buffer_setup_checks=extra,original_main_texture_and_vertex_buffer_is_same=True,original_main_shape_buffer_bound=True,baseline_reset_boot_verifier_sha256=baseline,verifier_sha256=sha(__file__),whole_system_boot_executed=False)
    path.write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True)
if __name__=='__main__':main()
