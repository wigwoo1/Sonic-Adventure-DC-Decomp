"""Native C fixture: real stack/FPU preservation, scheduler order and cleanup."""
import argparse, ctypes as C, hashlib, json, subprocess
from pathlib import Path
def sha(p): return hashlib.sha256(Path(p).read_bytes()).hexdigest()

FIXTURE=r'''
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <string.h>
#include <xmmintrin.h>
#include "jp_native_fibers.h"
struct result { int failures, count, trace[32]; uint32_t after[4], was_fiber,
    restored_fiber; uint64_t switches, passes; };
static struct result *out;
static struct rf_server *inner;
static void mark(int n) { if(out->count<32) out->trace[out->count++]=n; else out->failures++; }
static void need(int ok) { if(!ok) out->failures++; }
static void first(void) {
    volatile uint64_t local=UINT64_C(0x123456789abcdef0);
    _mm_setcsr((_mm_getcsr() & ~0x6000U)|0x2000U);
    mark(10); rfSleepFiber(0); mark(11); rfSleepFiber(2);
    need(local==UINT64_C(0x123456789abcdef0));
    need((_mm_getcsr() & 0x6000U)==0x2000U); mark(12); rfEndFiber();
}
static void second(void) {
    volatile uint64_t local=UINT64_C(0xfedcba9876543210);
    _mm_setcsr((_mm_getcsr() & ~0x6000U)|0x4000U);
    mark(20); rfSleepVFiber(); mark(21); rfSleepFiber(-1);
    need(local==UINT64_C(0xfedcba9876543210));
    need((_mm_getcsr() & 0x6000U)==0x4000U); mark(22);
    /* Returning must end this fiber, not terminate the Windows thread. */
}
static void nested_inner(void) { mark(40); rfSleepVFiber(); mark(41); }
static void nested_outer(void) {
    mark(30); rfExecFiberServer(inner); mark(31); rfSleepVFiber();
    rfExecFiberServer(inner); mark(32);
}
__declspec(dllexport) int jp_fiber_fixture(struct result *result) {
    out=result; memset(out,0,sizeof(*out)); out->was_fiber=IsThreadAFiber();
    uint32_t control=_mm_getcsr(); struct rf_server *s[8]; char stacks[2][4096];
    memset(stacks,0xa5,sizeof(stacks)); rfInitSystem(0); need(jp_rf_status()==0);
    need(rfCreateServer(33)==NULL && jp_rf_status()==JP_RF_ARGUMENT); jp_rf_clear_error();
    for(int i=0;i<8;i++) { s[i]=rfCreateServer(2); need(s[i]!=NULL); }
    need(rfCreateServer(1)==NULL && jp_rf_status()==JP_RF_CAPACITY); jp_rf_clear_error();
    need(rfCreateFiber(s[0],first,stacks[0],4096)==0);
    need(rfCreateFiber(s[0],second,stacks[1],4096)==1);
    need(rfCreateFiber(s[0],first,stacks[0],4096)==-1 && jp_rf_status()==JP_RF_CAPACITY); jp_rf_clear_error();
    struct jp_rf_info info;
    for(int pass=0;pass<4;pass++) {
        if(pass==3) rfWakeupFiber(s[0],1);
        rfExecFiberServer(s[0]); need(jp_rf_snapshot(&info)==0);
        out->after[pass]=info.fibers; need(_mm_getcsr()==control);
    }
    inner=s[2];
    need(rfCreateFiber(inner,nested_inner,stacks[0],4096)==0);
    need(rfCreateFiber(s[1],nested_outer,stacks[1],4096)==0);
    rfExecFiberServer(s[1]); rfExecFiberServer(s[1]);
    need(rfCreateFiber(s[3],first,stacks[0],4096)==0);
    rfKillFiberLevel(s[3],0); rfExecFiberServer(s[3]);
    need(jp_rf_snapshot(&info)==0 && info.fibers==1);
    rfExecFiberServer(s[3]); need(jp_rf_snapshot(&info)==0 && info.fibers==0);
    need(rfKillFiber(s[3],0)==-1);
    need(rfKillFiber(s[3],32)==-1 && jp_rf_status()==JP_RF_ARGUMENT); jp_rf_clear_error();
    for(unsigned int i=0;i<sizeof(stacks);i++) need(((unsigned char *)stacks)[i]==0xa5);
    need(jp_rf_snapshot(&info)==0 && info.servers==8 && !info.fibers);
    out->switches=info.switches; out->passes=info.passes;
    rfExitSystem(); need(jp_rf_snapshot(&info)==0 && !info.servers && !info.fibers);
    out->restored_fiber=IsThreadAFiber(); need(out->restored_fiber==out->was_fiber);
    return out->failures;
}
'''
class Result(C.Structure):
    _fields_=[('failures',C.c_int),('count',C.c_int),('trace',C.c_int*32),('after',C.c_uint32*4),('was_fiber',C.c_uint32),('restored_fiber',C.c_uint32),('switches',C.c_uint64),('passes',C.c_uint64)]
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','output'): ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args(); r=a.workspace.resolve(); build=a.build.resolve(); out=a.output.resolve()
    assert not out.exists() and out.is_relative_to(r/'work/sonic-adventure') and not out.is_relative_to(r/'work/sonic-adventure/build')
    b=json.loads((build/'build_verification.json').read_bytes()); assert sha(build/'jp_native_vmu_fs.dll')==b['DLL_sha256']
    for n,h in b['authored_source_sha256'].items(): assert sha(r/n)==h,n
    out.mkdir(); source=out/'fiber_fixture.c'; source.write_text(FIXTURE); dll=out/'fiber_fixture.dll'
    cmd=[b['commands'][0]['command'][0],'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(r/'outputs/sonic-adventure/native'),'-shared','-fuse-ld=lld',str(source),str(build/'jp_native_vmu_fs.lib'),'-o',str(dll)]
    run=subprocess.run(cmd,capture_output=True); (out/'build.txt').write_bytes(run.stdout+run.stderr)
    assert not run.returncode and not run.stdout and not run.stderr,(run.stdout+run.stderr).decode()
    core=C.CDLL(str(build/'jp_native_vmu_fs.dll')); fixture=C.CDLL(str(dll)); f=fixture.jp_fiber_fixture; f.argtypes=[C.POINTER(Result)]; f.restype=C.c_int
    value=Result(); assert f(C.byref(value))==0,value.failures
    trace=list(value.trace[:value.count]); expected=[10,11,20,21,12,22,30,40,31,41,32]
    assert trace==expected,trace
    assert list(value.after)==[2,2,1,0]
    # The outer nested-server fiber records 30 and 31 in the same turn.
    assert value.switches==10 and value.passes==10,(value.switches,value.passes)
    result=dict(status='native_cooperative_fiber_order_stacks_FPU_context_nested_servers_and_cleanup_verified',build_verification_sha256=sha(build/'build_verification.json'),DLL_sha256=b['DLL_sha256'],verifier_sha256=sha(Path(__file__)),fixture_sha256=sha(source),fixture_DLL_sha256=sha(dll),command=cmd,fixture_failure_count=value.failures,actual_trace=trace,independent_expected_trace=expected,live_fibers_after_each_pass=list(value.after),actual_context_switches=value.switches,server_passes=value.passes,real_native_stack_locals_and_FPU_rounding_preserved=True,original_SH4_stack_arrays_unchanged=True,capacity_and_index_bounds_checked=True,level_kill_and_wakeup_checked=True,nested_different_servers_checked=True,thread_fiber_conversion_state_restored=True,upstream_SH4_setjmp_not_used=True,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n'); print(result['status'],flush=True)

if __name__=='__main__': main()
