"""Whole display-reset/image suite after actual original boot buffer setup."""
import ctypes as C,hashlib,json,sys
from pathlib import Path
import verify_native_display_reset as frozen
from native_packet_test_support import bind
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    argv=sys.argv[1:];build=Path(argv[argv.index('--build')+1]).resolve();out=Path(argv[argv.index('--output')+1]).resolve();b=json.loads((build/'build_verification.json').read_bytes());original_bind=frozen.bind;calls=0
    def select(lib,name,result,*types):
        native=original_bind(lib,name,result,*types)
        if name!='jp_render_state_initialize':return native
        def initialize(*args):
            nonlocal calls
            code=native(*args)
            if code:return code
            buffer=C.addressof((C.c_byte*0x40000).in_dll(lib,'texture_buffer'));shape=C.addressof((C.c_float*6144).in_dll(lib,'shape_buffer'));C.memset(buffer,0x4d,0x40000)
            assert bind(lib,'jp_render_state_register_buffer',C.c_int,C.c_void_p,C.c_size_t,C.c_uint32)(buffer,0x40000,4096)==0
            bind(lib,'njInitTextureBuffer',None,C.c_void_p,C.c_uint32)(buffer,0x40000);bind(lib,'njInit3D',None,C.c_void_p,C.c_int)(buffer,4096);bind(lib,'njInitShape',None,C.c_void_p)(shape)
            assert C.c_void_p.in_dll(lib,'_nj_texture_buffer_st').value==C.c_void_p.in_dll(lib,'_nj_vertex_buf_').value==buffer
            assert C.c_void_p.in_dll(lib,'_nj_shape_buf_').value==shape and C.c_uint32.in_dll(lib,'_nj_texture_buffer_max_size').value==0x40000
            assert C.c_int.in_dll(lib,'_nj_vertex_num_').value==4096 and bind(lib,'jp_render_state_capacity',C.c_uint32)()==2048
            assert bind(lib,'jp_render_state_status',C.c_int)()==0 and bind(lib,'jp_render_state_buffer_is_borrowed',C.c_int)()==1
            calls+=1;return 0
        return initialize
    frozen.bind=select;frozen.main();path=out/'verification.json';result=json.loads(path.read_bytes());assert calls==1 and result['checks']==1841658 and result['verifier_sha256']==sha(frozen.__file__)
    lib=C.CDLL(b['frames_DLL']);buffer=C.addressof((C.c_byte*0x40000).in_dll(lib,'texture_buffer'));assert C.string_at(buffer+2048*16,0x40000-2048*16)==bytes([0x4d])*(0x40000-2048*16)
    result.update(status='whole_display_reset_and_actual_GPU_pixels_on_original_boot_borrowed_buffers_verified',checks=result['checks']+6,frozen_display_checks=result['checks'],boot_buffer_setup_and_extent_checks=6,actual_original_boot_buffer_setup_calls=calls,original_main_texture_and_vertex_buffer_is_same=True,original_main_shape_buffer_bound=True,trailing_main_buffer_bytes_preserved=True,frozen_verifier_sha256=sha(frozen.__file__),verifier_sha256=sha(__file__),native_functions_replaced=False)
    path.write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True)
if __name__=='__main__':main()
