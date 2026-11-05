"""Original startup/logo/EOR regression after actual native vertex init.
Derive only the existing Python caller prelude; no native body is replaced.
"""
import hashlib,inspect,json,sys
from pathlib import Path
import verify_native_vblank_startup as frozen
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    source=inspect.getsource(frozen.main);baseline=sha(frozen.__file__)
    old="                assert bind(service,'jp_vblank_arm_ninja_blank',I)()==0"
    new="""                lifecycle=C.CDLL(b['vertex_lifecycle_DLL'])
                bind(lifecycle,'njInitVertexBuffer',None,I,I,I,I,I)(-1,0,0x20000,0,0x8000)
                assert bind(lifecycle,'jp_vertex_lifecycle_status',I)()==0"""
    assert source.count(old)==1;source=source.replace(old,new)
    old="assert blank.value==0 and bind(service,'jp_vblank_chain_count',U)()>=1";assert source.count(old)==1;source=source.replace(old,"assert blank.value>=1 and bind(service,'jp_vblank_chain_count',U)()>=1")
    old='real_Ninja_blank_callback_armed=True';assert source.count(old)==1;source=source.replace(old,'real_Ninja_blank_callback_armed=True,actual_native_njInitVertexBuffer_called=True')
    frozen.__file__=__file__;exec(compile(source,__file__,'exec'),frozen.__dict__);frozen.main()
    argv=sys.argv[1:]
    if '--worker' in argv:raise AssertionError('native exit returned')
    out=Path(argv[argv.index('--output')+1]);path=out/'verification.json';v=json.loads(path.read_bytes());assert v['checks']==7714 and v['actual_GPU_Ninja_EOR_callbacks']==570
    for case in ['movie0','movie6','advertise1','cleanup','quit','pending_quit']:
        row=json.loads((out/case/'vblank_startup_prelude.json').read_bytes());assert row['actual_native_njInitVertexBuffer_called'] and row['wrapper_sha256']==sha(__file__)
    v.update(status='actual_native_vertex_initialization_original_boot_logo_GPU_EOR_exit_verified',actual_native_njInitVertexBuffer_calls=6,baseline_vblank_startup_verifier_sha256=baseline,derived_Python_caller_sha256=hashlib.sha256(source.encode()).hexdigest(),verifier_sha256=sha(__file__),native_function_bodies_replaced=False,full_system_boot_verified=False)
    path.write_text(json.dumps(v,indent=2)+'\n');print(v['status'],v['checks'],'checks',flush=True)
if __name__=='__main__':main()
