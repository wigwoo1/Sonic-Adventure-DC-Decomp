"""Original full chain suite plus the retired/recycled Ninja-slot regression."""
import ctypes as C,hashlib,json,shutil,sys
from pathlib import Path
import verify_native_vblank_chain as frozen
from native_packet_test_support import bind
from native_clock_test_support import Clock,Counter
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    argv=sys.argv[1:];build=Path(argv[argv.index('--build')+1]).resolve();out=Path(argv[argv.index('--output')+1]).resolve();frozen.main();path=out/'verification.json';result=json.loads(path.read_bytes());assert result['checks']==79096 and result['verifier_sha256']==sha(frozen.__file__);shutil.copyfile(path,out/'frozen-chain-verification.json')
    b=json.loads((build/'build_verification.json').read_bytes());clock=Clock(Path(b['present_DLL']));lib=clock.lib;counter=Counter();checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    Idle=C.CFUNCTYPE(C.c_int,C.c_void_p,C.c_uint32);Callback=C.CFUNCTYPE(None,C.c_void_p)
    @Idle
    def idle(context,ms):counter.tick+=(ms*781250+999)//1000;return 1
    @Callback
    def unrelated(context):pass
    check(counter.start(clock,781250)==0,'same native clock reinitialized');check(bind(lib,'jp_wait_initialize',C.c_int,C.c_uint32,C.c_uint32,C.c_uint32,C.c_void_p,C.c_void_p)(60,1,7,idle,None)==0,'one native wait owner reinitialized')
    arm=bind(lib,'jp_vblank_arm_ninja_blank',C.c_int);wait=bind(lib,'jp_vblank_wait_ninja_blank',C.c_int);count=bind(lib,'jp_vblank_chain_count',C.c_uint32)
    check(arm()==0,'genuine blank armed');bind(lib,'njSetEORflag',None,C.c_void_p)(None);check(wait()==0 and count()==0,'genuine blank retires on next native tick')
    handle=bind(lib,'syChainAddHandler',C.c_uint32,C.c_int16,C.c_void_p,C.c_uint32,C.c_void_p)(0x900,unrelated,1,None);check(handle==1,'unrelated callback reuses retired numeric slot');check(arm()==0 and count()==2,'recycled slot does not block new genuine blank');record=frozen.Record();check(wait()==0 and count()==1 and bind(lib,'jp_vblank_chain_describe',C.c_int,C.c_uint32,C.c_void_p)(handle,C.byref(record))==0 and record.callback==C.cast(unrelated,C.c_void_p).value,'new genuine blank retires while original unrelated callback survives')
    assert checks==7;assert bind(lib,'jp_wait_shutdown',C.c_int)()==0;clock.shutdown()
    result.update(status='original_chain_contracts_and_retired_Ninja_slot_reuse_verified',checks=result['checks']+checks,frozen_chain_checks=79096,retired_slot_reuse_checks=checks,retired_Ninja_slot_reuse_fixed=True,reused_numeric_handle=handle,unrelated_callback_preserved=True,frozen_verifier_sha256=sha(frozen.__file__),verifier_sha256=sha(__file__))
    path.write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True)
if __name__=='__main__':main()
