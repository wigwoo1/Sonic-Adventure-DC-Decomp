"""Actual RTC ownership tokens, multi-owner rollback and ordered native exit."""
import argparse,ctypes as C,hashlib,json,subprocess,sys,threading
from pathlib import Path
from native_packet_test_support import bind
from verify_native_rtc import Info,Record,Idle
from verify_native_exit_logo import ExitConfig,ExitInfo
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p
CASES=('tokens','rtc_only','with_wait_heap','rtc_cold','rtc_claimed','device_cold','retry','foreign','immediate_exit','failure_exit')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--worker',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    if not a.worker:
        assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();cases={}
        for name in CASES:
            child=d/name;child.mkdir();run=subprocess.run([sys.executable,'-B',str(Path(__file__).resolve()),'--workspace',str(r),'--build',str(a.build.resolve()),'--output',str(child),'--worker',name],capture_output=True,creationflags=subprocess.CREATE_NO_WINDOW)
            (child/'stdout.txt').write_bytes(run.stdout);(child/'stderr.txt').write_bytes(run.stderr)
            assert run.returncode==(70 if name=='failure_exit' else 0) and not run.stderr,(name,run.returncode,run.stderr.decode()[-6000:])
            cases[name]=dict(json.loads((child/'result.json').read_bytes()),observed_process_exit_code=run.returncode);print(name,'verified',flush=True)
        result=dict(status='actual_RTC_terminal_authority_rollback_before_wait_shutdown_and_native_exits_verified',checks=sum(x['checks'] for x in cases.values()),cases=cases,
            native_services_or_original_bodies_replaced=False,actual_process_exit_codes=[0,70],build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True);return
    C.CDLL(b['frames_DLL']);C.CDLL(b['GPU_DLL']);chain=C.CDLL(b['present_DLL']);rtc=C.CDLL(b['RTC_DLL']);heap=C.CDLL(b['SDK_heap_DLL']);ex=C.CDLL(b['authored_exit_DLL'])
    checks=[]
    def check(ok,label):assert ok,label;checks.append(label)
    def save(**kw):(d/'result.json').write_text(json.dumps(dict(checks=len(checks),check_labels=checks,**kw),indent=2)+'\n')
    init=bind(rtc,'syRtcInit',I);close=bind(rtc,'jp_rtc_shutdown',I);config=bind(rtc,'jp_rtc_configure',I,P,P);snapshot=bind(rtc,'jp_rtc_snapshot',I,P)
    claim=bind(rtc,'jp_rtc_exit_claim',I,P);cancel=bind(rtc,'jp_rtc_exit_cancel',I,Q);finish=bind(rtc,'jp_rtc_exit_finish',I,Q)
    eb=bind(ex,'jp_exit_bind',I,P);clean=bind(ex,'jp_exit_cleanup',I);es=bind(ex,'jp_exit_snapshot',I,P);exitstatus=bind(ex,'jp_exit_status',I)
    create=bind(heap,'jp_sdk_heap_create',I,U);hc=bind(heap,'jp_sdk_heap_shutdown',I);wc=bind(chain,'jp_wait_shutdown',I);ws=bind(chain,'jp_wait_status',I);count=bind(chain,'jp_vblank_chain_count',U)
    def info():v=Info();assert snapshot(C.byref(v))==0;return v
    def state():v=ExitInfo();assert es(C.byref(v))==0;return v
    def cfg(mask):return ExitConfig(C.sizeof(ExitConfig),mask,None,None)
    @Idle
    def idle(context,ms):return 1
    def start():
        check(bind(chain,'jp_clock_initialize',I,P,P,Q)(None,None,0)==0 and bind(chain,'jp_wait_initialize',I,U,U,U,P,P)(60,1,8,C.cast(idle,P),None)==0,'actual PC clock and original-chain wait initialized')
        check(config(None,None)==0 and init()==0 and count()==1,'actual PC RTC has one live native callback')
    if a.worker=='rtc_cold':
        check(create(4096)==0,'actual native SDK heap');c=cfg(32768|65536)
        check(eb(C.byref(c))==9 and exitstatus()==2 and hc()==0,'missing RTC rejects binding and cancels the already claimed real heap');save();return
    start()
    if a.worker=='tokens':
        token=Q(0x1234);check(claim(None)==1 and claim(C.byref(token))==0 and token.value,'real initialized RTC issues a full-width terminal claim');old=token.value;other=Q(0x5678)
        check(claim(C.byref(other))==10 and other.value==0x5678 and cancel(old|(1<<32))==10 and finish(0)==10,'duplicate/wrong full-width tokens cannot steal ownership')
        check(close()==10 and config(None,None)==10 and init()==-1 and info().initialized,'ordinary lifecycle cannot close/reinitialize delegated RTC')
        check(cancel(old)==0 and finish(old)==10 and close()==0 and init()==0,'uncommitted token cancels without stranding actual callback ownership')
        check(claim(C.byref(token))==0 and token.value!=old,'cancelled RTC token is never recycled');old=token.value
        check(finish(old)==0 and finish(old)==0 and cancel(old)==10 and close()==10 and init()==-1 and not info().initialized and count()==0,'same committed token closes once and remains terminal')
        foreign=[];thread=threading.Thread(target=lambda:foreign.extend([finish(old),cancel(old),close()]));thread.start();thread.join();check(foreign==[3,3,3],'creator restriction persists after final close');check(wc()==0,'separately owned wait remains closeable');save();return
    if a.worker in ('rtc_claimed','device_cold'):
        check(create(4096)==0,'actual SDK arena before attempted combined claims');token=Q()
        if a.worker=='rtc_claimed':check(claim(C.byref(token))==0,'existing independent RTC exit claim')
        c=cfg(65536|32768|(16384 if a.worker=='device_cold' else 0));check(eb(C.byref(c))==(7 if a.worker=='device_cold' else 9) and exitstatus()==2 and info().initialized,'later real owner rejects combined binding without accepting exit state')
        check(hc()==0,'failed combined bind cancels real heap claim')
        if a.worker=='rtc_claimed':check(cancel(token.value)==0,'pre-existing RTC claim remains with its actual owner')
        check(close()==0 and wc()==0,'rolled-back RTC and wait close normally');save();return
    c=cfg(65536|(32768|8 if a.worker=='with_wait_heap' else 8 if a.worker in ('retry','immediate_exit','failure_exit') else 0))
    if a.worker=='with_wait_heap':check(create(4096)==0,'real heap included in ordered native exit')
    check(eb(C.byref(c))==0 and close()==10 and info().initialized,'general exit claims real RTC and blocks independent close')
    if a.worker=='foreign':
        foreign=[];thread=threading.Thread(target=lambda:foreign.extend([clean(),close()]));thread.start();thread.join();check(foreign==[3,3] and info().initialized and count()==1,'foreign cleanup cannot retire game-thread clock')
    if a.worker in ('retry','failure_exit'):
        record=Record();h=info().chain;check(bind(chain,'jp_vblank_chain_describe',I,U,P)(h,C.byref(record))==0,'capture actual native callback identity')
        bind(chain,'syChainDeleteHandler',None,U)(h)
        check(clean()==6 and state().failed_resource==65536 and state().native_error==5 and state().remaining==c.resources and ws()==0,'missing actual handler fails RTC close before touching remaining wait')
        if a.worker=='failure_exit':save(actual_native_exit=True,failed_resource=65536);bind(ex,'syBtExit',None)();raise AssertionError('native exit returned')
        restored=bind(chain,'syChainAddHandler',U,C.c_int16,P,U,P)(0x900,P(record.callback),92,None)
        check(restored==h,'recover actual native callback at original still-owned handle')
    if a.worker=='immediate_exit':save(actual_native_exit=True);bind(ex,'syBtExit',None)();raise AssertionError('native exit returned')
    check(clean()==0 and state().remaining==0 and state().completed==c.resources and not info().initialized,'native general exit really retires RTC before selected wait/heap owners')
    check(clean()==0 and close()==10,'completed RTC authority cannot close twice')
    if c.resources&8:check(ws()==2,'selected actual wait is closed only after RTC')
    else:check(ws()==0 and count()==0 and wc()==0,'unselected actual wait survives RTC-only cleanup')
    if c.resources&32768:check(hc()==3,'final SDK heap remains terminal under its actual exit authority')
    save(completed_resources=int(c.resources))
if __name__=='__main__':main()
