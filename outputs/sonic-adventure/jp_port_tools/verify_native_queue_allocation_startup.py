"""Run actual startup/logo/EOR regression using preallocated real host queues.
This supplies allocation minima as a native caller, not njInitVertexBuffer.
"""
import ctypes as C, hashlib, json, sys
from pathlib import Path
import verify_native_vblank_startup as parent
from native_packet_test_support import bind
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    argv=sys.argv[1:];out=Path(argv[argv.index('--output')+1]).resolve();worker='--worker' in argv
    if worker:
        frozen=parent.parent.parent.frozen;previous=frozen.bind
        def select(lib,name,result,*types):
            native=previous(lib,name,result,*types)
            if name!='jp_queue_initialize':return native
            def initialize(*args):
                code=native(*args)
                if code:return code
                # Main requests opaque bypass and tp+pt=0x28000 words.
                # Native bypass remains a real growable opaque host list.
                minimum=(C.c_size_t*5)(416,0xa0000,0,0,0)
                assert bind(lib,'jp_queue_preallocate',C.c_int,C.POINTER(C.c_size_t))(minimum)==0
                capacity=bind(lib,'jp_queue_capacity_bytes',C.c_size_t,C.c_uint32)
                assert all(capacity(i)==minimum[i] for i in range(5))
                assert bind(lib,'jp_queue_allocated_bytes',C.c_size_t)()==sum(minimum)+128
                (out/'queue_allocation_setup.json').write_text(json.dumps(dict(checks=3,minimum_bytes=list(minimum),real_existing_queue_owner=True,njInitVertexBuffer_called=False,wrapper_sha256=sha(__file__)),indent=2)+'\n')
                return code
            return initialize
        frozen.bind=select
    parent.__file__=__file__;parent.main()
    if worker:raise AssertionError('native process exit returned')
    path=out/'verification.json';result=json.loads(path.read_bytes());assert result['checks']==7714
    for case in ['movie0','movie6','advertise1','cleanup','quit','pending_quit']:
        row=json.loads((out/case/'queue_allocation_setup.json').read_bytes());assert row['checks']==3 and row['wrapper_sha256']==sha(__file__) and row['real_existing_queue_owner'] and not row['njInitVertexBuffer_called']
    result.update(status='preallocated_real_queues_original_boot_logo_GPU_EOR_exit_verified',checks=7732,prior_vblank_startup_checks=7714,actual_queue_allocation_setup_checks=18,verifier_sha256=sha(__file__),queue_allocation_via_real_native_caller=True,njInitVertexBuffer_integrated=False,whole_system_boot_verified=False)
    path.write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True)
if __name__=='__main__':main()
