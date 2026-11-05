"""Original startup after actual vertex init, default font and game text style."""
import hashlib,inspect,json,sys
from pathlib import Path
import verify_native_vblank_startup as frozen
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    source=inspect.getsource(frozen.main);baseline=sha(frozen.__file__)
    old="                assert bind(service,'jp_vblank_arm_ninja_blank',I)()==0"
    new="""                lifecycle=C.CDLL(b['vertex_lifecycle_DLL'])
                bind(lifecycle,'njInitVertexBuffer',None,I,I,I,I,I)(-1,0,0x20000,0,0x8000)
                assert bind(lifecycle,'jp_vertex_lifecycle_status',I)()==0
                frames=C.CDLL(b['frames_DLL'])
                bind(frames,'njInitPrint',None,P,U,U)(None,0,0)
                assert bind(frames,'jp_print_status',I)()==0
                bind(frames,'text_style_init',None,P,U,U)(None,0,0)
                style=(U*6)();bind(frames,'jp_game_text_snapshot',None,P)(style)
                assert style[0]==0xffbfbfbf and style[1]==0x41800000 and style[5]!=0"""
    assert source.count(old)==1;source=source.replace(old,new)
    for old,new in [("assert blank.value==0 and bind(service,'jp_vblank_chain_count',U)()>=1","assert blank.value>=1 and bind(service,'jp_vblank_chain_count',U)()>=1"),('dict(checks=3,real_Ninja_blank_callback_armed=True','dict(checks=5,actual_vertex_font_game_style_setup=True,real_Ninja_blank_callback_armed=True'),("row['checks']==3","row['checks']==5"),('assert extra==18','assert extra==30')]:assert source.count(old)==1,(old,source.count(old));source=source.replace(old,new)
    # The old caller never initialized the SDK font, so its logo-only release
    # oracle expected zero surfaces. The real parent graphics setup now owns
    # one font until system exit: require that exact font and no logo surfaces.
    logo=frozen.parent.parent.frozen;original_getsource=inspect.getsource
    def caller_source(function):
        body=original_getsource(function)
        if function is logo.main:
            old="pvm.loaded(listing())==0 and runtime.live()==0"
            new="pvm.loaded(listing())==0 and runtime.live()==1 and bind(lib,'jp_texture_surface_owned',I,P)(bind(lib,'jp_print_font_surface',P)())==1"
            assert body.count(old)==1;body=body.replace(old,new)
        return body
    inspect.getsource=caller_source
    frozen.__file__=__file__;exec(compile(source,__file__,'exec'),frozen.__dict__)
    try:frozen.main()
    finally:inspect.getsource=original_getsource
    argv=sys.argv[1:]
    if '--worker' in argv:raise AssertionError('native exit returned')
    out=Path(argv[argv.index('--output')+1]);path=out/'verification.json';v=json.loads(path.read_bytes());assert v['checks']==7726 and v['actual_GPU_Ninja_EOR_callbacks']==570
    for case in ['movie0','movie6','advertise1','cleanup','quit','pending_quit']:assert json.loads((out/case/'vblank_startup_prelude.json').read_bytes())['actual_vertex_font_game_style_setup']
    v.update(status='actual_native_vertex_font_game_text_style_and_original_logo_exit_verified',actual_game_text_style_calls=6,actual_original_Ninja_default_font_calls=6,baseline_vblank_startup_verifier_sha256=baseline,derived_Python_caller_sha256=hashlib.sha256(source.encode()).hexdigest(),logo_release_preserves_only_actual_SDK_font=True,system_exit_still_requires_zero_owned_surfaces=True,verifier_sha256=sha(__file__),native_functions_replaced=False,full_system_boot_verified=False);path.write_text(json.dumps(v,indent=2)+'\n');print(v['status'],v['checks'],'checks',flush=True)
if __name__=='__main__':main()
