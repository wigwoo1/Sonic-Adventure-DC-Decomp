"""Actual GDFS token authority and ordered multi-service cleanup, no service stubs."""
import argparse,ctypes as C,hashlib,json,subprocess,sys,threading,time
from pathlib import Path
from native_packet_test_support import bind
from verify_native_gdfs import Info as GDFSInfo,Callback,Idle
from verify_native_rtc import Info as RTCInfo
from verify_native_vblank_chain import Record
from verify_native_exit_logo import ExitConfig,ExitInfo
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p
GDFS=131072;RTC=65536;HEAP=32768;WAIT=8;DEVICE=16384
CASES=('tokens','gdfs_only','with_wait_heap_RTC','gdfs_cold','gdfs_claimed','device_cold','retry_chain','retry_wait','callback_retry','foreign','legacy_mask','immediate_exit','failure_exit')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--worker',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    if not a.worker:
        assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();cases={}
        for name in CASES:
            child=d/name;child.mkdir();cmd=[sys.executable,'-B',str(Path(__file__).resolve()),'--workspace',str(r),'--build',str(a.build.resolve()),'--track',str(a.track.resolve()),'--output',str(child),'--worker',name]
            run=subprocess.run(cmd,capture_output=True,timeout=90,creationflags=subprocess.CREATE_NO_WINDOW);(child/'stdout.txt').write_bytes(run.stdout);(child/'stderr.txt').write_bytes(run.stderr)
            assert run.returncode==(70 if name=='failure_exit' else 0) and not run.stderr,(name,run.returncode,run.stderr.decode()[-7000:])
            cases[name]=dict(json.loads((child/'result.json').read_bytes()),observed_process_exit_code=run.returncode);print(name,'verified',flush=True)
        result=dict(status='actual_GDFS_terminal_authority_rollback_pending_IO_and_ordered_native_exit_verified',checks=sum(x['checks'] for x in cases.values()),cases=cases,
            native_services_or_original_bodies_replaced=False,actual_process_exit_codes=[0,70],build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
            full_system_bootstrap_audio_or_gameplay_verified=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True);return
    C.CDLL(b['frames_DLL']);C.CDLL(b['GPU_DLL']);chain=C.CDLL(b['present_DLL']);rtc=C.CDLL(b['RTC_DLL']);heap=C.CDLL(b['SDK_heap_DLL']);fs=C.CDLL(b['GDFS_DLL']);ex=C.CDLL(b['authored_exit_DLL'])
    checks=[];retained=[]
    def check(ok,label):assert ok,label;checks.append(label)
    def save(**kw):(d/'result.json').write_text(json.dumps(dict(checks=len(checks),check_labels=checks,**kw),indent=2)+'\n')
    configure=bind(fs,'jp_gdfs_configure',I,C.c_wchar_p,U,U,U,U);init=bind(fs,'jp_gdfs_initialize',I,U,U);close=bind(fs,'jp_gdfs_shutdown',I);snapshot=bind(fs,'jp_gdfs_snapshot',I,P)
    claim=bind(fs,'jp_gdfs_exit_claim',I,P);cancel=bind(fs,'jp_gdfs_exit_cancel',I,Q);finish=bind(fs,'jp_gdfs_exit_finish',I,Q);reinit=bind(fs,'gdFsReinit',I)
    change=bind(fs,'gdFsChangeDir',I,C.c_char_p);open_=bind(fs,'gdFsOpen',P,C.c_char_p,P);hc=bind(fs,'gdFsClose',None,P);req=bind(fs,'gdFsReqRd32',I,P,I,P);read=bind(fs,'gdFsRead',I,P,I,P);server=bind(fs,'gdFsExecServer',None)
    eb=bind(ex,'jp_exit_bind',I,P);clean=bind(ex,'jp_exit_cleanup',I);es=bind(ex,'jp_exit_snapshot',I,P);exitstatus=bind(ex,'jp_exit_status',I)
    create=bind(heap,'jp_sdk_heap_create',I,U);heapclose=bind(heap,'jp_sdk_heap_shutdown',I);rtclose=bind(rtc,'jp_rtc_shutdown',I);wc=bind(chain,'jp_wait_shutdown',I);ws=bind(chain,'jp_wait_status',I);count=bind(chain,'jp_vblank_chain_count',U)
    def info():v=GDFSInfo();assert snapshot(C.byref(v))==0;return v
    def ri():v=RTCInfo();assert bind(rtc,'jp_rtc_snapshot',I,P)(C.byref(v))==0;return v
    def state():v=ExitInfo();assert es(C.byref(v))==0;return v
    def cfg(mask):return ExitConfig(C.sizeof(ExitConfig),mask,None,None)
    @Idle
    def idle(context,ms):
        if ms:time.sleep(ms/1000)
        return 1
    check(bind(chain,'jp_clock_initialize',I,P,P,Q)(None,None,0)==0 and bind(chain,'jp_wait_initialize',I,U,U,U,P,P)(60,1,8,idle,None)==0,'actual shared PC QPC/vblank owner')
    if a.worker=='gdfs_cold':
        check(create(4096)==0 and bind(rtc,'syRtcInit',I)()==0,'actual heap and RTC before cold GDFS rejection');c=cfg(HEAP|RTC|GDFS)
        check(eb(C.byref(c))==10 and exitstatus()==2,'cold filesystem rejects binding without accepting exit state')
        check(rtclose()==0 and heapclose()==0 and wc()==0,'all earlier real claims rolled back after GDFS rejection');save();return
    check(configure(str(a.track.resolve()),45150,2352,1,5000)==0 and init(8,1920)==0 and count()==1,'real original GDFS initialized with one handler and one-sector I/O chunks')
    check(change(b'SONICADV')==0 and info().dir_entries==1863,'actual full game directory loaded')
    def file():
        h=open_(b'ADV00.PRS',None);check(bool(h),'actual game file handle opened');return h
    def buffer(n):
        raw=(C.c_ubyte*(n+64))();C.memset(raw,0xa5,len(raw));at=(C.addressof(raw)+31)&~31;retained.append(raw);return at
    def pending():
        h=file();at=buffer(64*2048);check(req(h,64,at)==64 and info().pending==1,'actual chunked native file request remains owned before cleanup');return h,at
    if a.worker=='tokens':
        token=Q(0x1234);check(claim(None)==1 and claim(P(info().work))==1 and claim(C.byref(token))==0 and token.value,'real filesystem claims only a separate full-width output')
        old=token.value;other=Q(0x5678)
        check(claim(C.byref(other))==2 and other.value==0x5678 and cancel(old|(1<<32))==2 and finish(0)==2,'duplicate and wrong full-width claims preserve actual authority')
        check(close()==2 and init(8,1920)==2 and configure(str(a.track.resolve()),45150,2352,1,5000)==2 and reinit()==-13,'ordinary lifecycle cannot steal delegated filesystem')
        h=file();at=buffer(2048);check(read(h,1,at)==0,'actual file I/O remains usable under uncommitted exit claim');hc(h)
        check(cancel(old)==0 and finish(old)==2 and close()==0 and init(8,1920)==0,'uncommitted token cancels and ordinary owner can restart')
        check(claim(C.byref(token))==0 and token.value!=old,'cancelled filesystem token never recycled');old=token.value
        check(finish(old)==0 and finish(old)==0 and cancel(old)==2 and close()==2 and init(8,1920)==2 and count()==0 and not info().work,'committed cleanup closes once and remains terminal')
        foreign=[];thread=threading.Thread(target=lambda:foreign.extend([finish(old),cancel(old),close()]));thread.start();thread.join();check(foreign==[3,3,3],'creator thread restriction persists after final cleanup')
        check(wc()==0,'caller-owned shared wait survives filesystem-only close');save();return
    if a.worker in ('gdfs_claimed','device_cold'):
        check(create(4096)==0 and bind(rtc,'syRtcInit',I)()==0 and count()==2,'real earlier heap/RTC owners');token=Q()
        if a.worker=='gdfs_claimed':check(claim(C.byref(token))==0,'independent existing filesystem claim')
        c=cfg(GDFS|RTC|HEAP|(DEVICE if a.worker=='device_cold' else 0))
        check(eb(C.byref(c))==(7 if a.worker=='device_cold' else 10) and exitstatus()==2 and info().phase==3,'later claim rejects without retiring current real owners')
        if a.worker=='gdfs_claimed':check(cancel(token.value)==0,'prior independent claim remains untouched')
        check(close()==0 and rtclose()==0 and heapclose()==0 and count()==0 and wc()==0,'GDFS/RTC/heap rollback leaves ordinary lifecycle authority intact');save();return
    if a.worker=='legacy_mask':
        check(bind(rtc,'syRtcInit',I)()==0,'actual RTC for old resource mask');c=cfg(RTC)
        check(eb(C.byref(c))==0 and clean()==0 and state().completed==RTC and info().phase==3 and count()==1,'legacy RTC-only mask never implicitly claims or closes GDFS')
        h=file();at=buffer(2048);check(read(h,1,at)==0,'unselected original filesystem still performs real I/O');hc(h)
        check(close()==0 and wc()==0,'caller closes unselected filesystem before its shared wait');save();return
    with_others=a.worker in ('with_wait_heap_RTC','retry_chain','retry_wait','callback_retry','immediate_exit','failure_exit')
    if with_others:check(create(4096)==0 and bind(rtc,'syRtcInit',I)()==0 and count()==2,'actual SDK heap and second RTC handler')
    c=cfg(GDFS|(RTC|HEAP|WAIT if with_others else 0));check(eb(C.byref(c))==0 and close()==2 and reinit()==-13,'native general exit takes one filesystem shutdown claim')
    h,at=pending();user_callbacks=[]
    @Callback
    def observed(arg):user_callbacks.append(threading.get_ident())
    bind(fs,'gdFsEntryRdEndFunc',None,P,P,P)(h,observed,None)
    if a.worker=='foreign':
        foreign=[];thread=threading.Thread(target=lambda:foreign.extend([clean(),close()]));thread.start();thread.join()
        check(foreign==[3,3] and info().pending==1 and count()==1,'foreign cleanup cannot cancel main-thread native I/O')
    if a.worker in ('retry_chain','failure_exit'):
        record=Record();handle=info().vblank;check(bind(chain,'jp_vblank_chain_describe',I,U,P)(handle,C.byref(record))==0,'capture actual filesystem callback identity')
        bind(chain,'syChainDeleteHandler',None,U)(handle)
        check(clean()==6 and state().failed_resource==GDFS and state().native_error==8 and state().remaining==c.resources,'missing real filesystem chain rejects before retiring any later owner')
        check(ws()==0 and ri().initialized and not user_callbacks,'RTC/wait still live and cancelled read dispatches no late completion')
        if a.worker=='failure_exit':save(actual_native_exit=True,failed_resource=GDFS);bind(ex,'syBtExit',None)();raise AssertionError('native exit returned')
        restored=bind(chain,'syChainAddHandler',U,C.c_int16,P,U,P)(0x900,P(record.callback),80,None);check(restored==handle,'restore original still-owned filesystem handler')
    if a.worker=='retry_wait':
        observed_cleanup=[]
        @Callback
        def inside_dispatch(arg):observed_cleanup.append(clean())
        hook=bind(chain,'syChainAddHandler',U,C.c_int16,P,U,P)(0x900,inside_dispatch,1,None);check(bool(hook),'actual shared chain callback for real busy-wait shutdown rejection')
        time.sleep(.025);processed=U();check(bind(chain,'jp_wait_pump',I,P)(C.byref(processed))==0 and observed_cleanup and observed_cleanup[0]==6,'cleanup entered from actual running wait service')
        check(state().failed_resource==WAIT and state().native_error==4 and state().completed==(GDFS|RTC) and state().remaining==(WAIT|HEAP),'successful filesystem/RTC cleanup retained while busy wait remains owned')
        bind(chain,'syChainDeleteHandler',None,U)(hook)
    if a.worker=='callback_retry':
        # Cancel the long request, then make a read callback call actual cleanup.
        check(bind(fs,'gdFsStopRd',I,P)(h)==0,'real pending request cancelled before callback-reentrancy case');hc(h);h=file();at=buffer(2048);observed_cleanup=[]
        @Callback
        def read_completed(arg):observed_cleanup.append(clean())
        bind(fs,'gdFsEntryRdEndFunc',None,P,P,P)(h,read_completed,None);check(req(h,1,at)==1,'actual callback read accepted after delegated claim')
        deadline=time.monotonic()+5
        while not observed_cleanup:
            server()
            if time.monotonic()>deadline:raise AssertionError('actual callback deadline')
        check(observed_cleanup==[6] and state().failed_resource==GDFS and state().native_error==9 and state().remaining==c.resources and ws()==0 and ri().initialized,'callback reentrancy rejects cleanup without losing or retiring owners')
    if a.worker=='immediate_exit':save(actual_native_exit=True,pending_real_GDFS_read=True);bind(ex,'syBtExit',None)();raise AssertionError('native exit returned')
    frozen_before=C.string_at(at,64*2048 if a.worker!='callback_retry' else 2048)
    check(clean()==0 and state().remaining==0 and state().completed==c.resources and not info().work and not info().dir,'actual ordered cleanup drains I/O, retires handlers and releases owner arenas')
    check(clean()==0 and close()==2,'terminal delegated filesystem cannot be closed twice')
    frozen=C.string_at(at,len(frozen_before));time.sleep(.005);check(C.string_at(at,len(frozen_before))==frozen and not user_callbacks,'detached caller buffer remains stable and receives no late completion callback')
    if with_others:check(ws()==2 and not ri().initialized and heapclose()==3,'actual RTC/wait retired before final SDK heap')
    else:check(ws()==0 and count()==0 and wc()==0,'unselected real wait remains owned by caller')
    save(completed_resources=int(c.resources),pending_read_cancelled=True)
if __name__=='__main__':main()
