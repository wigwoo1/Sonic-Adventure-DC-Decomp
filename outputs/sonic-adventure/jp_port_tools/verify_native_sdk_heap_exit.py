"""Actual SDK heap/exit delegation, rollback and original task destruction."""
import argparse,ctypes as C,hashlib,json,subprocess,sys,threading
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime
from native_task_test_support_v2 import Task
from verify_native_sdk_heap import Info
from verify_native_exit_logo import ExitConfig,ExitInfo
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p;S=C.c_size_t
CASES=('tokens','heap_only','rollback','heap_cold','heap_claimed','device_claimed','task_consumer','task_leak','foreign','immediate_exit','failure_exit')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--worker',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for k in ('frames','GPU','present','vertex_lifecycle','device','authored_exit','SDK_heap','fixture'):assert sha(b[k+'_DLL'])==b[k+'_DLL_sha256']
    if not a.worker:
        assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build');d.mkdir();cases={}
        for name in CASES:
            child=d/name;child.mkdir();args=[sys.executable,'-B',str(Path(__file__).resolve()),'--workspace',str(r),'--build',str(a.build.resolve()),'--output',str(child),'--worker',name]
            run=subprocess.run(args,capture_output=True,creationflags=subprocess.CREATE_NO_WINDOW);(child/'stdout.txt').write_bytes(run.stdout);(child/'stderr.txt').write_bytes(run.stderr)
            assert run.returncode==(70 if name=='failure_exit' else 0) and not run.stderr,(name,run.returncode,run.stderr.decode()[-6000:])
            row=json.loads((child/'result.json').read_bytes());row['observed_process_exit_code']=run.returncode;cases[name]=row;print(name,'verified',flush=True)
        result=dict(status='real_SDK_heap_terminal_exit_authority_atomic_bind_rollback_and_original_task_consumer_verified',checks=sum(x['checks'] for x in cases.values()),cases=cases,
            actual_SDK_arenas_and_allocation_used=True,original_task_destroy_all_invokes_native_C_destructor=True,
            failed_later_device_claim_rolls_back_heap_claim=True,live_pointer_retry_and_destructor_once_verified=True,
            full_width_token_thread_and_terminal_owner_guards=True,actual_native_success_and_failure_exit_codes=[0,70],
            original_game_or_SDK_function_bodies_replaced=False,build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),whole_system_boot_or_gameplay_verified=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True);return
    checks=[]
    def check(ok,label):assert ok,label;checks.append(label)
    def save(**kw):(d/'result.json').write_text(json.dumps(dict(checks=len(checks),check_labels=checks,**kw),indent=2)+'\n')
    t=Runtime(Path(b['frames_DLL']));fl=t.dll;C.CDLL(b['GPU_DLL']);C.CDLL(b['present_DLL']);C.CDLL(b['vertex_lifecycle_DLL']);dl=C.CDLL(b['device_DLL']);hl=C.CDLL(b['SDK_heap_DLL']);fixture=C.CDLL(b['fixture_DLL']);ex=C.CDLL(b['authored_exit_DLL'])
    create=bind(hl,'jp_sdk_heap_create',I,U);close=bind(hl,'jp_sdk_heap_shutdown',I);allocate=bind(hl,'jp_sdk_heap_allocate',P,S);free=bind(hl,'jp_sdk_heap_free',I,P);snapshot=bind(hl,'jp_sdk_heap_snapshot',I,P)
    claim=bind(hl,'jp_sdk_heap_exit_claim',I,P);cancel=bind(hl,'jp_sdk_heap_exit_cancel',I,Q);finish=bind(hl,'jp_sdk_heap_exit_finish',I,Q)
    exit_bind=bind(ex,'jp_exit_bind',I,P);clean=bind(ex,'jp_exit_cleanup',I);exit_status=bind(ex,'jp_exit_status',I);exit_snapshot=bind(ex,'jp_exit_snapshot',I,P);boot_exit=bind(ex,'syBtExit',None)
    def heap():v=Info();assert snapshot(C.byref(v))==0;return v
    def state():v=ExitInfo();assert exit_snapshot(C.byref(v))==0;return v
    def cfg(mask):return ExitConfig(C.sizeof(ExitConfig),mask,None,None)
    def device_init():
        check(bind(fl,'jp_queue_initialize',I,S)(8<<20)==0 and bind(fl,'jp_frames_initialize',I,U,U,S)(8,128,32<<20)==0 and t.init(32,1024,32<<20)==0 and bind(fl,'jp_render_state_initialize',I,U,S)(128,1<<20)==0,'actual native graphics prerequisites')
        check(bind(dl,'jp_device_initialize',I,I,I,I)(1,0,1)==0,'actual native graphics device')
    if a.worker=='tokens':
        out=Q(0xaabbccdd);check(claim(C.byref(out))==2 and out.value==0xaabbccdd,'cold heap cannot issue an exit token')
        check(create(4096)==0,'actual guarded SDK heap');info=heap();check(claim(P(info.base))==1 and claim(None)==1,'claim output cannot alias arena/header or be NULL')
        p=allocate(64);check(p and claim(C.byref(out))==0 and out.value,'real terminal authority claim with a live consumer');token=out.value;other=Q(0x11223344)
        check(claim(C.byref(other))==3 and other.value==0x11223344 and cancel(token|(1<<32))==3 and finish(0)==3,'duplicate/full-width wrong tokens preserve ownership/output')
        check(close()==3 and create(4096)==3,'ordinary lifecycle cannot replace a delegated heap')
        check(cancel(token)==0 and finish(token)==3 and close()==4,'uncommitted claim cancels and normal close again reports actual live owner')
        check(free(p)==0 and close()==0 and create(4096)==0,'cancelled claim permits real ordinary teardown/reinitialization')
        check(claim(C.byref(out))==0 and out.value!=token,'cancelled token is never recycled');token=out.value;p=allocate(32)
        check(finish(token)==4 and cancel(token)==3 and heap().initialized and heap().live==1,'first final-close attempt commits authority and retains live pointer for retry')
        check(free(p)==0 and finish(token)==0 and finish(token)==0 and heap().initialized==0 and close()==3 and create(4096)==3,'same token completes once and remains terminal after actual release')
        foreign=[];thread=threading.Thread(target=lambda:foreign.extend([finish(token),cancel(token),close()]));thread.start();thread.join();check(foreign==[10,10,10],'creator-thread restriction persists after delegated heap release');save(token_cancel_retry_terminal_guards=True);return
    if a.worker in ('heap_cold','heap_claimed','device_claimed'):device_init()
    if a.worker!='heap_cold':check(create((1<<20)+32)==0 and heap().usable==1<<20,'real full one-MiB SDK usable heap with safe alignment headroom')
    if a.worker=='heap_cold':
        c=cfg(49152);check(exit_bind(C.byref(c))==8 and exit_status()==2,'missing heap rejects combined bind before claiming device')
        check(bind(dl,'jp_device_shutdown',I)()==0,'failed heap precondition leaves native device independently closable');save(no_partial_device_claim=True);return
    if a.worker=='heap_claimed':
        token=Q();check(claim(C.byref(token))==0,'external heap authority exists before bind');c=cfg(49152)
        check(exit_bind(C.byref(c))==8 and exit_status()==2 and bind(dl,'jp_device_shutdown',I)()==0,'already claimed heap cannot partially capture another device')
        check(finish(token.value)==0,'original external heap authority remains valid');save(existing_heap_claim_preserved=True);return
    if a.worker=='device_claimed':
        token=Q();check(bind(dl,'jp_device_exit_claim',I,P)(C.byref(token))==0,'external device authority exists before bind');c=cfg(49152)
        check(exit_bind(C.byref(c))==7 and exit_status()==2 and close()==0,'later rejected device claim cancels uncommitted heap claim')
        check(bind(dl,'jp_device_exit_finish',I,Q)(token.value)==0,'original device token remains valid after rolled-back heap bind');save(actual_heap_claim_rollback=True);return
    if a.worker=='rollback':
        c=cfg(49152);check(exit_bind(C.byref(c))==7 and exit_status()==2 and close()==0,'missing device rolls back actual heap claim without binding exit')
        check(create((1<<20)+32)==0,'heap remains reusable after rejected combined bind');device_init()
        check(exit_bind(C.byref(c))==0 and clean()==0 and heap().initialized==0 and state().completed==49152,'same exit can subsequently claim and close both genuine owners');save(rolled_back_then_successful_combined_bind=True);return
    prepare=bind(fixture,'jp_sdk_fixture_prepare',I,U);observe=bind(fixture,'jp_sdk_fixture_observe',None,P);destructor=bind(fixture,'jp_sdk_fixture_destructor',None,P);fixture_snapshot=bind(fixture,'jp_sdk_fixture_snapshot',None,P)
    def consumer():v=(U*4)();fixture_snapshot(v);return tuple(v)
    task_mode=a.worker in ('task_consumer','task_leak')
    if task_mode:
        bind(fl,'heap_init',None)();bind(fl,'task_init',None)();check(prepare(256)==0 and heap().live==1,'native C consumer holds an actual SDK allocation')
        task=bind(fl,'task_create',P,U,I,P)(2,0,None);check(bool(task),'original game task created');Task.from_address(task).destructor=C.cast(destructor,P).value;observe(None)
    extra=allocate(64) if a.worker in ('heap_only','task_leak','failure_exit') else None
    if extra:check(heap().live>=1,'actual outside consumer remains live')
    c=cfg(32768|(16 if task_mode else 0));check(exit_bind(C.byref(c))==0 and exit_status()==0,'one SDK heap exit authority accepted')
    check(exit_bind(C.byref(c))==5 and close()==3,'rebinding and independent arena close reject after claim')
    if a.worker=='foreign':
        foreign=[];thread=threading.Thread(target=lambda:foreign.extend([clean(),close()]));thread.start();thread.join();check(foreign==[3,10] and heap().initialized,'foreign cleanup cannot close the heap')
    if a.worker=='immediate_exit':save(actual_syBtExit_entered=True);boot_exit();raise AssertionError('native exit returned')
    if extra:
        check(clean()==6 and state().remaining==32768 and state().failed_resource==32768 and state().native_error==4,'real live SDK allocation blocks only final heap close')
        check(heap().initialized and heap().live==1,'failed final close preserves arena and remaining allocation')
        if task_mode:check(consumer()==(1,0,0,256) and state().completed==16,'original task destructor freed its allocation exactly once before heap retirement')
        if a.worker=='failure_exit':save(actual_syBtExit_entered=True,expected_failure_exit_code=70);boot_exit();raise AssertionError('failed native exit returned')
        check(free(extra)==0,'outside consumer retires for genuine final-close retry')
    check(clean()==0 and state().remaining==0 and state().completed==c.resources and heap().initialized==0,'actual exit closes heap after all selected consumers')
    if task_mode:check(consumer()==(1,0,0,256),'actual original task destruction invokes native C SDK destructor exactly once')
    check(clean()==0 and close()==3 and state().completed==c.resources,'completed delegated heap is terminal and cannot close twice')
    save(actual_heap_exit=True,completed_resources=int(c.resources),actual_native_task_destructor=task_mode)
if __name__=='__main__':main()
