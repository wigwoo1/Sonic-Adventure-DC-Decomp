"""Frozen whole-model contracts using the original main's borrowed scratch.
Only the Python caller setup changes. No native function body is replaced.
"""
import ctypes as C,hashlib,inspect,json,sys
from pathlib import Path
import verify_native_model_rz as frozen
from native_packet_test_support import bind
from native_video_test_support import Video
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    argv=sys.argv[1:];build=Path(argv[argv.index('--build')+1]).resolve();output=Path(argv[argv.index('--report')+1]).resolve();b=json.loads((build/'build_verification.json').read_bytes())
    lib=C.CDLL(b['DLL']);assert b['DLL']==b['frames_DLL'];assert Video(lib).init(640,480,0xff000000,1)==0
    assert bind(lib,'jp_frames_initialize',C.c_int,C.c_uint32,C.c_uint32,C.c_size_t)(16,128,32<<20)==0
    buffer=C.addressof((C.c_byte*0x40000).in_dll(lib,'texture_buffer'));C.memset(buffer,0x6d,0x40000)
    def initialize(actual):
        assert bind(actual,'jp_render_state_register_buffer',C.c_int,C.c_void_p,C.c_size_t,C.c_uint32)(buffer,0x40000,4096)==0
        bind(actual,'njInitTextureBuffer',None,C.c_void_p,C.c_uint32)(buffer,0x40000)
        bind(actual,'njInit3D',None,C.c_void_p,C.c_int)(buffer,4096)
        assert bind(actual,'jp_render_state_status',C.c_int)()==0 and C.c_void_p.in_dll(actual,'_nj_vertex_buf_').value==buffer
        assert C.c_int.in_dll(actual,'_nj_vertex_num_').value==4096 and bind(actual,'jp_render_state_capacity',C.c_uint32)()==2048
        assert bind(actual,'jp_render_state_buffer_is_borrowed',C.c_int)()==1
    original_hash=sha(frozen.__file__);source=inspect.getsource(frozen.main)
    anchor="    check(init(16<<20)==0 and stateinit(2048,1<<20)==0,'owned native stores initialized')";assert source.count(anchor)==1
    source=source.replace(anchor,anchor+'\n    boot_bind_main_scratch(lib)');frozen.boot_bind_main_scratch=initialize
    exec(compile(source,frozen.__file__,'exec'),frozen.__dict__);frozen.main()
    result=json.loads(output.read_bytes());assert result['verifier_sha256']==original_hash and not result['original_dependency_hooks_used']
    assert bind(lib,'jp_render_state_buffer_is_borrowed',C.c_int)()==1 and bind(lib,'jp_render_state_scratch',C.c_void_p)()==buffer
    assert C.string_at(buffer+2048*16,0x40000-2048*16)==bytes([0x6d])*(0x40000-2048*16)
    result.update(status='whole_original_model_contracts_on_actual_main_borrowed_buffer_verified',frozen_model_checks=result['checks'],checks=result['checks']+6,caller_setup_and_retained_extent_checks=6,frozen_verifier_sha256=original_hash,derived_Python_harness_sha256=hashlib.sha256(source.encode()).hexdigest(),verifier_sha256=sha(__file__),actual_main_buffer_bytes=0x40000,original_SDK_vertex_count=4096,retained_GCC_model_lighting_limit=2048,borrowed_buffer_used_by_actual_model_producer=True,trailing_main_buffer_bytes_preserved=True,native_functions_replaced=False)
    output.write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True)
if __name__=='__main__':main()
