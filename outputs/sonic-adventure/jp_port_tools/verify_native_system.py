"""Actual compiled sbInitSystem ownership, errors, early exit and final handoff."""
import argparse,ctypes as C,hashlib,json,subprocess,sys,threading
from pathlib import Path
from native_packet_test_support import bind
from verify_native_device_init import Options as DeviceOptions,Info as DeviceInfo
from verify_native_sdk_heap import Info as HeapInfo
from verify_native_gdfs import Info as GDFSInfo
from verify_native_rtc import Info as RTCInfo
from verify_native_vblank_chain import Record
from verify_native_exit_logo import ExitConfig,ExitInfo
from verify_native_input_backend import Source,Sample
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p;S=C.c_size_t
CORE=245762;BOOTSTRAP=262144;Reader=C.CFUNCTYPE(I,P,C.POINTER(U))
class Options(C.Structure):
    _fields_=[('bytes',U),('input_flags',U),('device',DeviceOptions),('track',P)]+[(n,U) for n in ('raw_sector_bytes','chunk_sectors','timeout_ms','reserved')]+[(n,P) for n in ('input_source','input_context','rtc_reader','rtc_context')]
class Info(C.Structure):
    _fields_=[(n,U) for n in ('phase','status','resources','failed_resource','native_error')]+[(n,I) for n in ('mode','frame','count')]+[(n,U) for n in ('attempts','reserved')]+[(n,Q) for n in ('initializations','cleanups','window')]
CASES=('sdk_entry','restart','bad_mode','missing_track','bad_RTC','bad_input','bad_directory','preexisting_heap','foreign','reentrant','early_cleanup','early_retry','final_bind','immediate_exit','failure_exit','sdk_failure')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','bad-directory-track','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    ap.add_argument('--worker',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    if not a.worker:
        assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();cases={}
        for name in CASES:
            child=d/name;child.mkdir();args=[sys.executable,'-B',str(Path(__file__).resolve()),'--workspace',str(r),'--build',str(a.build.resolve()),'--track',str(a.track.resolve()),'--bad-directory-track',str(a.bad_directory_track.resolve()),'--output',str(child),'--worker',name]
            run=subprocess.run(args,capture_output=True,timeout=90,creationflags=subprocess.CREATE_NO_WINDOW);(child/'stdout.txt').write_bytes(run.stdout);(child/'stderr.txt').write_bytes(run.stderr)
            expected=70 if name=='failure_exit' else 73 if name=='sdk_failure' else 0
            assert run.returncode==expected and not run.stderr,(name,run.returncode,run.stderr.decode()[-6000:])
            cases[name]=dict(json.loads((child/'result.json').read_bytes()),observed_process_exit_code=run.returncode);print(name,'verified',flush=True)
        result=dict(status='compiled_native_sbInitSystem_real_core_ownership_rollback_early_exit_and_final_handoff_verified',checks=sum(x['checks'] for x in cases.values()),cases=cases,
            native_services_or_original_bodies_replaced=False,actual_process_exit_codes=[0,70,73],build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
            native_main_system_boot_init_audio_or_gameplay_verified=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True);return
    frames=C.CDLL(b['frames_DLL']);gpu=C.CDLL(b['GPU_DLL']);chain=C.CDLL(b['present_DLL']);dev=C.CDLL(b['device_DLL']);heap=C.CDLL(b['SDK_heap_DLL']);rtc=C.CDLL(b['RTC_DLL']);fs=C.CDLL(b['GDFS_DLL']);ex=C.CDLL(b['authored_exit_DLL']);lib=C.CDLL(b['System_DLL'])
    checks=[];keep=[]
    def check(ok,label):assert ok,label;checks.append(label)
    def save(**kw):(d/'result.json').write_text(json.dumps(dict(checks=len(checks),check_labels=checks,**kw),indent=2)+'\n')
    config=bind(lib,'jp_system_configure',I,P);init=bind(lib,'jp_system_initialize',I,I,I,I);close=bind(lib,'jp_system_shutdown',I);delegate=bind(lib,'jp_system_bind_exit',I,U)
    def snapshot(module,name,kind):v=kind();assert bind(module,name,I,P)(C.byref(v))==0;return v
    def info():return snapshot(lib,'jp_system_snapshot',Info)
    def hi():return snapshot(heap,'jp_sdk_heap_snapshot',HeapInfo)
    def di():return snapshot(dev,'jp_device_snapshot',DeviceInfo)
    def ri():return snapshot(rtc,'jp_rtc_snapshot',RTCInfo)
    def fi():return snapshot(fs,'jp_gdfs_snapshot',GDFSInfo)
    def ei():return snapshot(ex,'jp_exit_snapshot',ExitInfo)
    clean=bind(ex,'jp_exit_cleanup',I);es=bind(ex,'jp_exit_status',I);wc=bind(chain,'jp_wait_status',I)
    qi=bind(frames,'jp_queue_initialize',I,S);frinit=bind(frames,'jp_frames_initialize',I,U,U,S);texinit=bind(frames,'jp_textures_initialize',I,U,U,S);renderinit=bind(frames,'jp_render_state_initialize',I,U,S)
    check(C.sizeof(Options)==96 and C.sizeof(Info)==64 and close()==0 and info().phase==0,'native system ABI and harmless cold shutdown')
    check(qi(8<<20)==0 and frinit(8,128,32<<20)==0 and texinit(32,1024,32<<20)==0 and renderinit(128,1<<20)==0,'actual empty render prerequisite owners, borrowed by native core')
    track=a.bad_directory_track if a.worker=='bad_directory' else d/'absent.bin' if a.worker in ('missing_track','sdk_failure') else a.track
    path=C.create_unicode_buffer(str(track.resolve()));keep.append(path)
    opts=Options(C.sizeof(Options),3,DeviceOptions(640,480,0,0,5000,0,64<<20),C.addressof(path),2352,3,5000,0,None,None,None,None)
    @Reader
    def bad_rtc(ctx,out):return 1
    @Source
    def bad_input(ctx,samples):return 0
    @Source
    def reentrant(ctx,samples):
        C.memset(samples,0,4*C.sizeof(Sample));samples[0]=Sample(1,0,0,0,0,0,0,0)
        if info().phase==2:reentries.extend([close(),init(1,0,1),config(C.byref(opts))])
        return 1
    reentries=[]
    if a.worker=='bad_RTC':opts.rtc_reader=C.cast(bad_rtc,P).value
    if a.worker=='bad_input':opts.input_source=C.cast(bad_input,P).value;opts.input_flags=0
    if a.worker=='reentrant':opts.input_source=C.cast(reentrant,P).value;opts.input_flags=0
    check(config(None)==1 and config(C.byref(opts))==0 and info().phase==1,'validated native settings copied before original-ABI init')
    def release_prerequisites():
        check(bind(frames,'jp_frames_shutdown',I)()==0,'caller releases borrowed frame owner')
        bind(frames,'jp_render_state_shutdown',None)();bind(frames,'jp_textures_shutdown',None)();bind(frames,'jp_queue_shutdown',None)()
    if a.worker=='preexisting_heap':
        check(bind(heap,'jp_sdk_heap_create',I,U)(4096)==0,'independent existing real SDK heap')
        check(init(1,0,1)==2 and hi().initialized and info().resources==0 and di().phase==0,'preflight rejects without stealing/closing existing heap')
        check(close()==0 and hi().initialized and bind(heap,'jp_sdk_heap_shutdown',I)()==0,'ordinary system cleanup does not retire another owner');release_prerequisites();save();return
    if a.worker=='sdk_failure':save(actual_native_sbInitSystem_failure_exit=True);bind(lib,'sbInitSystem',None,I,I,I)(1,0,1);raise AssertionError('failed SDK entry returned')
    if a.worker=='sdk_entry':
        path[0]='Z';opts.device.width=0
        bind(lib,'sbInitSystem',None,I,I,I)(1,0,1);rc=bind(lib,'jp_system_status',I)()
    else:rc=init(0x7fffffff if a.worker=='bad_mode' else 1,0,1)
    expected={'bad_mode':6,'missing_track':9,'bad_RTC':8,'bad_input':7,'bad_directory':10}
    if a.worker in expected:
        check(rc==expected[a.worker] and info().phase==7 and info().resources==0,'real native service/directory failure rolls back all acquired core resources')
        check(not hi().initialized and not di().resources and not ri().initialized and fi().phase<2 and bind(frames,'jp_input_status',I)()==4 and es()==2,'failed init leaves real services cold and no false global cleanup owner')
        if a.worker=='missing_track':check(info().attempts==8,'original eight bounded filesystem init attempts retained')
        if a.worker=='bad_directory':check(info().attempts==1 and info().native_error==C.c_uint32(-8).value,'directory error checked after one successful actual volume startup')
        check(close()==0 and info().phase==1,'failed factory remains retryable after explicit cleanup');release_prerequisites();save(observed_error=rc);return
    check(rc==0 and info().phase==3 and info().resources==CORE and info().attempts==1 and info().initializations==1,'compiled core starts exact original limits and owns all actual services')
    check(hi().initialized and hi().usable==1<<20 and di().phase==4 and di().resources==63 and ri().initialized and fi().dir_entries==1863,'actual SDK heap, native device/window, RTC and game directory ready')
    check(bind(frames,'jp_input_status',I)()==0 and bind(frames,'jp_input_bootstrap_status',I)()==0 and wc()==0 and es()==0 and ei().remaining==BOOTSTRAP,'configured SDK input, shared wait and real early-exit cleanup ready')
    user=C.WinDLL('user32');user.IsWindow.argtypes=[P];user.IsWindow.restype=I;hwnd=info().window
    borrowed=bind(dev,'jp_device_window',P)();native=bind(gpu,'jp_window_native_handle',P,P)(borrowed)
    check(borrowed>0xffffffff and hwnd==native and user.IsWindow(hwnd),'full-width native window object and unchanged real opaque HWND')
    h=bind(fs,'gdFsOpen',P,C.c_char_p,P)(b'SET0001S.BIN',None);data=(C.c_ubyte*2048)()
    check(h and bind(fs,'gdFsRead',I,P,I,P)(h,1,data)==0 and hashlib.sha256(bytes(data[:64])).hexdigest()=='16abab341fb7f370e27e4dadcf81766dd0dfd0ae64469477bb2cf6614938b2af','native coordinator enables an actual original GDFS game-file read')
    bind(fs,'gdFsClose',None,P)(h)
    if a.worker=='reentrant':check(reentries and all(x==4 for x in reentries),'native initialization rejects recursive lifecycle calls from actual input provider')
    if a.worker=='foreign':
        foreign=[];sentinel=Info();C.memset(C.byref(sentinel),0xa5,C.sizeof(sentinel));before=bytes(sentinel)
        def other():foreign.extend([config(C.byref(opts)),init(1,0,1),close(),delegate(CORE),bind(lib,'jp_system_snapshot',I,P)(C.byref(sentinel))])
        t=threading.Thread(target=other);t.start();t.join();check(foreign==[3]*5 and bytes(sentinel)==before and user.IsWindow(hwnd),'foreign thread cannot replace/read/retire the native core')
    if a.worker=='immediate_exit':save(actual_native_bootstrap_exit=True);bind(ex,'syBtExit',None)();raise AssertionError('native exit returned')
    if a.worker in ('early_retry','failure_exit'):
        record=Record();handle=fi().vblank;check(bind(chain,'jp_vblank_chain_describe',I,U,P)(handle,C.byref(record))==0,'capture real filesystem handler for actual cleanup failure')
        bind(chain,'syChainDeleteHandler',None,U)(handle)
        if a.worker=='failure_exit':save(actual_native_bootstrap_failure_exit=True);bind(ex,'syBtExit',None)();raise AssertionError('failed native exit returned')
        check(clean()==6 and ei().failed_resource==BOOTSTRAP and info().failed_resource==131072 and info().resources==CORE and wc()==0 and ri().initialized,'early cleanup failure retains native core and its actual shared wait/RTC')
        check(bind(chain,'syChainAddHandler',U,C.c_int16,P,U,P)(0x900,P(record.callback),80,None)==handle,'restore exact still-owned original GDFS handler')
    if a.worker=='final_bind':
        bad=ExitConfig(C.sizeof(ExitConfig),65536,None,None);check(bind(ex,'jp_exit_bind',I,P)(C.byref(bad))==1 and ei().remaining==BOOTSTRAP,'partial final mask cannot discard core cleanup authority')
        check(delegate(CORE)==0 and info().phase==4 and info().resources==0 and ei().remaining==CORE,'native factory hands every owned core service to terminal exit')
        check(bind(fs,'jp_gdfs_shutdown',I)()==2 and bind(heap,'jp_sdk_heap_shutdown',I)()==3,'independent close cannot steal delegated native owners')
        check(clean()==0 and info().phase==6 and ei().completed==CORE,'final native authority closes actual core services once')
    elif a.worker in ('early_cleanup','early_retry'):
        check(clean()==0 and info().phase==6 and ei().completed==BOOTSTRAP and ei().remaining==0,'early sbExitSystem cleanup really closes native-owned core')
        check(clean()==0 and delegate(CORE)==2,'early exit remains terminal and repeat cleanup harmless')
    else:
        check(close()==0 and info().resources==0 and info().phase==1 and es()==2,'ordinary shutdown closes core and cancels only its temporary exit registration')
        if a.worker=='restart':check(init(1,0,1)==0 and info().initializations==2 and close()==0,'ordinary uncommitted native factory restarts on the same creator thread')
    check(not hi().initialized and not di().resources and not ri().initialized and not user.IsWindow(hwnd),'actual heap/device/RTC resources and HWND retired')
    release_prerequisites();save(compiled_native_core=True)
if __name__=='__main__':main()
