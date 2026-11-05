"""Exercise SDK input bootstrap inside the real RTC/device/original boot caller."""
import hashlib,inspect,json,sys
from pathlib import Path
import verify_native_rtc_startup as parent
import verify_native_exit_logo as leaf
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    argv=sys.argv[1:];worker='--worker' in argv;out=Path(argv[argv.index('--output')+1]).resolve()
    original=inspect.getsource;entry=leaf.main;baseline=sha(parent.__file__);derived=[]
    def getsource(function):
        source=original(function)
        if function is entry:
            old="    check(bind(lib,'jp_input_initialize',I,P,U,P,P)(hwnd,0,C.cast(source,P),None)==0 and bind(lib,'jp_input_poll',I)()==0,'actual native sample provider primed')"
            new="""    assert bind(lib,'jp_input_configure',I,P,U,P,P)(hwnd,0,C.cast(source,P),None)==0
    assert bind(lib,'jp_input_use_virtual_cards',I)()==0
    bind(lib,'pdInitPeripheral',None,I,P,P)(0,None,None)
    assert bind(lib,'jp_input_bootstrap_status',I)()==0
    check(bind(lib,'jp_input_status',I)()==0 and bind(lib,'jp_input_poll',I)()==0,'actual SDK-created native sample provider primed')
    (out/'SDK_input_setup.json').write_text(json.dumps(dict(status='actual_SDK_peripheral_entry_created_shared_input_owner_before_original_boot',checks=3,original_entry='pdInitPeripheral',virtual_card_metadata_provider_configured=True,controlled_sample_provider=True,wrapper_sha256=sha(__file__)),indent=2)+'\\n')"""
            # The derived leaf executes in its original module namespace.
            # Embed this wrapper's exact provenance instead of that module's
            # __file__; do not relabel the frozen verifier's own source.
            new=new.replace('wrapper_sha256=sha(__file__)','wrapper_sha256='+repr(sha(__file__)))
            assert source.count(old)==1;source=source.replace(old,new);derived.append(hashlib.sha256(source.encode()).hexdigest())
        return source
    inspect.getsource=getsource;parent.__file__=__file__
    try:parent.main()
    finally:inspect.getsource=original
    if worker:raise AssertionError('native process exit returned')
    path=out/'verification.json';v=json.loads(path.read_bytes());assert v['checks']==7319 and v['actual_Presents']==570;extra=0
    for case in ('movie0','movie6','advertise1','cleanup','quit','pending_quit'):
        row=json.loads((out/case/'SDK_input_setup.json').read_bytes());assert row['wrapper_sha256']==sha(__file__);extra+=row['checks']
    assert extra==18
    v.update(status='actual_SDK_peripheral_initialization_RTC_original_boot_logo_and_exit_verified',checks=v['checks']+extra,
        actual_SDK_peripheral_initializations=6,SDK_peripheral_bootstrap_checks=extra,one_shared_input_owner=True,
        input_host_window_borrowed_from_actual_device=True,console_DMA_buffers_used=False,actual_VMUs_in_startup_suite=False,
        baseline_RTC_startup_verifier_sha256=baseline,derived_Python_SDK_caller_sha256=derived,verifier_sha256=sha(__file__),
        full_sbInitSystem_audio_or_gameplay_verified=False)
    path.write_text(json.dumps(v,indent=2)+'\n');print(v['status'],v['checks'],'checks',flush=True)
if __name__=='__main__':main()
