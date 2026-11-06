"""Real original sound startup under compiled System/early/final exit owners."""
import argparse,ctypes as C,hashlib,json,subprocess,sys,threading
from pathlib import Path
from verify_native_audio import bind,Info as AudioInfo
from verify_native_game_sound import Options as SoundOptions,Info as SoundInfo
from verify_native_aica import Options as AicaOptions,Info as AicaInfo
from verify_native_system import Options as SystemOptions,Info as SystemInfo
from verify_native_device_init import Options as DeviceOptions,Info as DeviceInfo
from verify_native_sdk_heap import Info as HeapInfo
from verify_native_gdfs import Info as GDFSInfo
from verify_native_rtc import Info as RTCInfo
from verify_native_input_backend import Source,Sample
from verify_native_exit_logo import ExitInfo,ExitConfig
from verify_native_vblank_chain import Record
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p;S=C.c_size_t
CORE=245762;SOUND=524288;BOOTSTRAP=262144
class Claim(C.Structure):
    _fields_=[('next_token',Q),('token',Q)]+[(n,U) for n in ('terminal','closing','finished','reserved')]
CASES=('claim','restart','startup_failure','preexisting_sound','early','early_retry','ordinary_retry','partial_core_retry','final','final_retry',
       'missing_mask','late_claim_rejection','new_claim_rejection','direct_sound_final','ordinary_reentrant','final_reentrant','output_final','foreign','process_exit','process_exit_failure')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def values(v):return {n:getattr(v,n) for n,_ in v._fields_}
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['sound_exit_generated_sha256'].items():assert sha(a.build/n)==h,n
    for key in ('game_sound_DLL','System_DLL','authored_exit_DLL','present_DLL'):assert sha(b[key])==b[key+'_sha256']
    if not a.case:
        cases={}
        for case in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','build','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--output',str(d/case),'--case',case];run=subprocess.run(cmd,capture_output=True,timeout=90,creationflags=subprocess.CREATE_NO_WINDOW)
            (d/(case+'.log')).write_bytes(run.stdout+run.stderr)
            assert run.returncode==(70 if case=='process_exit_failure' else 0),(case,run.returncode,(run.stdout+run.stderr).decode(errors='replace'))
            cases[case]=read(d/case/'result.json');print(case,cases[case]['checks'],'checks',flush=True)
        result=dict(status='actual_original_game_sound_compiled_System_ordinary_restart_early_final_exit_claims_rollback_retry_and_callback_lifetime_verified',
            runtime_checks=sum(v['checks'] for v in cases.values()),fresh_process_cases=len(cases),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
            actual_process_exit_codes=[0,70],sound_resource_bit=SOUND,classic_resource_masks_preserved=True,
            sustained_audio_scheduler_full_game_queue_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    frames=C.CDLL(b['frames_DLL']);present=C.CDLL(b['present_DLL']);system=C.CDLL(b['System_DLL']);sound=C.CDLL(b['game_sound_DLL']);exit=C.CDLL(b['authored_exit_DLL'])
    core=C.CDLL(str(a.build/'jp_native_aica.dll'));g2=C.CDLL(str(a.build/'jp_native_g2_audio.dll'));fs=C.CDLL(b['GDFS_DLL']);rtc=C.CDLL(b['RTC_DLL']);heap=C.CDLL(b['SDK_heap_DLL']);device=C.CDLL(b['device_DLL'])
    start=bind(system,'jp_system_start_sound',I,P);sysclose=bind(system,'jp_system_shutdown',I);sysbind=bind(system,'jp_system_bind_exit',I,U)
    sndconfig=bind(sound,'jp_sound_configure',I,P);sndinit=bind(sound,'jp_sound_initialize',I);sndclose=bind(sound,'jp_sound_shutdown',I,U)
    claim=bind(sound,'jp_sound_exit_claim',I,P);cancel=bind(sound,'jp_sound_exit_cancel',I,Q);finish=bind(sound,'jp_sound_exit_finish',I,Q,U);delegclose=bind(sound,'jp_sound_exit_close',I,Q,U)
    clean=bind(exit,'jp_exit_cleanup',I);labels=[];keep=[];observations={};system_live=False;prereqs=False;delegated=False;token=Q()
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    def snap(module,name,kind):
        v=kind();check(bind(module,name,I,P)(C.byref(v))==0,'actual '+name);return v
    def si():return snap(sound,'jp_sound_snapshot',SoundInfo)
    def ci():return snap(sound,'jp_sound_exit_snapshot',Claim)
    def sy():return snap(system,'jp_system_snapshot',SystemInfo)
    def ei():return snap(exit,'jp_exit_snapshot',ExitInfo)
    def fi():return snap(fs,'jp_gdfs_snapshot',GDFSInfo)
    def options(name=None,output=0):
        buf=C.create_string_buffer(name) if name else None
        if buf:keep.append(buf)
        return SoundOptions(56,0,2<<20,5000,AicaOptions(32,output,16384,40,5000,1,1,0),C.addressof(buf) if buf else None)
    @Source
    def input_source(ctx,samples):C.memset(samples,0,4*C.sizeof(Sample));samples[0]=Sample(1,0,0,0,0,0,0,0);return 1
    path=C.create_unicode_buffer(str(a.track.resolve()));sysopts=SystemOptions(96,0,DeviceOptions(640,480,0,0,5000,0,64<<20),C.addressof(path),2352,3,5000,0,C.cast(input_source,P).value,None,None,None)
    def initialize_system():
        nonlocal system_live
        check(bind(system,'jp_system_configure',I,P)(C.byref(sysopts))==0 and bind(system,'jp_system_initialize',I,I,I,I)(1,0,1)==0,'real compiled System creates borrowed core services')
        system_live=True
    def core_still_alive():
        check(fi().phase==3 and snap(rtc,'jp_rtc_snapshot',RTCInfo).initialized and snap(heap,'jp_sdk_heap_snapshot',HeapInfo).initialized and snap(device,'jp_device_snapshot',DeviceInfo).resources==63 and bind(present,'jp_wait_status',I)()==0,
              'actual GDFS/RTC/SDK heap/device/wait remain live until sound retires')
    def core_closed():
        check(si().resources==0 and not snap(core,'jp_aica_snapshot',AicaInfo).resources and not snap(heap,'jp_sdk_heap_snapshot',HeapInfo).initialized and not snap(rtc,'jp_rtc_snapshot',RTCInfo).initialized and fi().work==0,
              'actual audio and borrowed System providers are retired')
    def raw_sound():check(sndconfig(C.byref(options()))==0 and sndinit()==0,'actual independent original game sound startup')
    def started(output=0):
        check(start(C.byref(options(output=output)))==0 and sy().resources==CORE|SOUND,'compiled System starts original game sound and owns dynamic sound resource')
        c=ci();check(c.token>0 and c.terminal==c.closing==c.finished==0 and si().resources==59,'exact uncommitted sound authority held by existing bootstrap')
        check(sndclose(5000)==2 and sndinit()==2 and sndconfig(C.byref(options()))==2 and bind(sound,'jp_sound_service',I)()==0,'ordinary sound lifetime blocked while real service remains usable')
        return c.token
    def hold_gate(held):check(bind(g2,'jp_g2_gate_set',I,I)(held)==0,'real G2 completion gate '+str(held))
    def callback_for_cleanup():
        CB=C.CFUNCTYPE(None,P);events=[];errors=[]
        @CB
        def callback(ctx):
            try:events.append([sysclose(),clean(),cancel(ci().token),sndclose(5000)])
            except BaseException as e:errors.append(repr(e))
        raw=C.create_string_buffer(96);keep.extend([raw,callback]);address=(C.addressof(raw)+31)&~31;h=P()
        check(bind(sound,'sdMemBlkCreate',I,P)(C.byref(h))==0 and bind(sound,'sdMemBlkSetPrm',I,P,P,I,P,P)(h,address,32,callback,None)==0 and bind(sound,'sdMemBlkTransfer',I,P,I)(h,0x1ff000)==0,'actual original asynchronous SDK transfer pending across cleanup')
        return events,errors
    def save():
        observations.update(sound=values(si()),claim=values(ci()),system=values(sy()))
        (d/'result.json').write_text(json.dumps(dict(case=a.case,checks=len(labels),assertions=labels,observations=observations),indent=2)+'\n')
    try:
        check(C.sizeof(Claim)==32 and si().phase==0 and ci().token==0,'real cold sound delegation ABI/state')
        sentinel=Q(99);check(claim(C.byref(sentinel))==2 and sentinel.value==99 and cancel(1)==2 and finish(1,5000)==2,'cold/stale claims cannot invent shutdown authority or touch output')
        for name,args in [('jp_queue_initialize',(8<<20,)),('jp_frames_initialize',(8,128,32<<20)),('jp_textures_initialize',(32,1024,32<<20)),('jp_render_state_initialize',(128,1<<20))]:
            check(bind(frames,name,I,*([S] if len(args)==1 else [U,U,S] if len(args)==3 else [U,S]))(*args)==0,'borrowed real render prerequisite '+name)
        prereqs=True;initialize_system()
        if a.case=='claim':
            raw_sound();check(claim(None)==1 and claim(C.byref(token))==0 and token.value>0,'actual live owner gives one native lifecycle token')
            stale=token.value;check(claim(C.byref(sentinel))==2 and sentinel.value==99 and cancel(stale+1)==2,'double/stale ownership cannot replace live authority')
            check(cancel(stale)==0 and claim(C.byref(token))==0 and token.value>stale and finish(stale,5000)==2,'cancelled token never recycles or retires a later session')
            check(delegclose(token.value,5000)==0 and ci().finished==1 and ci().terminal==0 and sndconfig(C.byref(options()))==2,'nonterminal delegated close keeps restart fenced while shared cleanup remains')
            check(cancel(token.value)==0 and sndconfig(C.byref(options()))==0 and sndinit()==0,'fully closed ordinary claim releases for actual restart')
            check(sndclose(5000)==0 and sysclose()==0,'caller closes independently owned audio before borrowed core');system_live=False
        elif a.case=='startup_failure':
            check(start(C.byref(options(b'absent.drv')))==13 and sy().resources==0 and sy().phase==7,'real sound input failure rolls back compiled System startup')
            core_closed();check(bind(exit,'jp_exit_status',I)()==2,'failed startup cancels exact early-exit registration after cleanup')
            initialize_system();started();check(sysclose()==0,'same process recovers actual full core and sound after startup failure');system_live=False
        elif a.case=='preexisting_sound':
            raw_sound();before=si();check(start(C.byref(options()))==2 and sy().resources==CORE and si().stage_address==before.stage_address and ci().token==0,'System rejects independently owned live sound without adoption/destruction')
            check(sndclose(5000)==0 and sysclose()==0,'actual independent sound then borrowed System close');system_live=False
        elif a.case in ('direct_sound_final','new_claim_rejection'):
            raw_sound()
            if a.case=='new_claim_rejection':
                check(bind(frames,'jp_queue_reserve',I,U,U)(0,4)==0 and bind(frames,'njStartVertexNonTexOpaque',P)(),'actual recording makes the last device claim reject')
                check(sysbind(CORE|SOUND)==11 and ci().token==0 and ci().next_token==1 and si().resources==59 and ei().remaining==BOOTSTRAP,'late failure cancels newly acquired sound claim and preserves independent live sound')
                check(bind(frames,'jp_queue_discard_active',I)()==0 and bind(frames,'jp_queue_reset_frame',I)()==0,'original producer releases actual conflicting recording')
            check(sysbind(CORE|SOUND)==0 and ci().token>0 and ei().remaining==CORE|SOUND,'explicit final mask claims independent live sound exactly once')
            delegated=True;check(clean()==0 and ci().terminal==ci().finished==1,'direct actual sound owner retires through final exit');core_closed();system_live=False
        else:
            first_token=started(1 if a.case=='output_final' else 0)
            if a.case=='foreign':
                output=Claim();C.memset(C.byref(output),0xa5,32);foreign=[]
                def outsider():foreign.extend([start(C.byref(options())),claim(C.byref(sentinel)),cancel(first_token),finish(first_token,5000),bind(sound,'jp_sound_exit_snapshot',I,P)(C.byref(output)),sysbind(CORE|SOUND),clean()])
                t=threading.Thread(target=outsider);t.start();t.join(5)
                check(not t.is_alive() and foreign==[3]*7 and bytes(output)==b'\xa5'*32 and ci().token==first_token,'foreign thread cannot alter System/global/sound authority or outputs')
                check(sysclose()==0,'creator performs actual ordinary shutdown after foreign attempts');system_live=False
            elif a.case=='restart':
                check(sysclose()==0 and ci().token==0 and sy().phase==1,'ordinary System cleanup releases closed claim and permits restart');core_closed()
                check(bind(system,'jp_system_initialize',I,I,I,I)(1,0,1)==0,'actual core restarts after nonterminal ordinary shutdown')
                check(started()>first_token and sysclose()==0,'fresh sound claim/token and actual driver boot after full restart');system_live=False
            elif a.case=='ordinary_retry':
                hold_gate(1);check(sysclose()==12 and sy().resources==CORE|SOUND and ci().closing==1 and ci().terminal==0 and ci().finished==0 and ei().remaining==BOOTSTRAP,
                    'ordinary actual sound cleanup failure retains nonterminal claim and complete borrowed core')
                core_still_alive();check(cancel(first_token)==2,'partly closed ordinary claim cannot be cancelled');hold_gate(0)
                check(sysclose()==0 and ci().token==0 and sy().phase==1 and bind(exit,'jp_exit_status',I)()==2,
                    'same ordinary owner retries before releasing fully closed claim and bootstrap')
                core_closed();system_live=False
            elif a.case=='partial_core_retry':
                record=Record();handle=fi().vblank
                check(bind(present,'jp_vblank_chain_describe',I,U,P)(handle,C.byref(record))==0,'capture actual still-owned GDFS handler')
                bind(present,'syChainDeleteHandler',None,U)(handle)
                check(sysclose()==12 and sy().resources==CORE and si().resources==0 and ci().finished==1 and ci().terminal==0,
                    'later real filesystem-close failure retains closed sound claim until shared core cleanup finishes')
                check(sndconfig(C.byref(options()))==2 and ei().remaining==BOOTSTRAP,'retired sound cannot restart during incomplete borrowed core cleanup')
                check(bind(present,'syChainAddHandler',U,C.c_int16,P,U,P)(0x900,P(record.callback),80,None)==handle,'restore exact real GDFS callback for retry')
                before=si().shutdowns;check(clean()==0 and ci().terminal==ci().finished==1 and si().shutdowns==before,
                    'early terminal retry commits previously closed sound without closing it twice')
                core_closed();system_live=False
            elif a.case in ('early','early_retry'):
                if a.case=='early_retry':
                    hold_gate(1);check(clean()==6 and ei().failed_resource==BOOTSTRAP and sy().resources==CORE|SOUND and si().resources==27 and ci().terminal==1 and ci().finished==0,'real early audio cleanup failure retains whole borrowed core and terminal sound token')
                    core_still_alive();check(cancel(first_token)==2 and sndconfig(C.byref(options()))==2,'failed terminal sound cleanup cannot be cancelled or restarted');hold_gate(0)
                check(clean()==0 and ei().remaining==0 and ei().completed==BOOTSTRAP and ci().terminal==ci().finished==1,'existing bootstrap callback performs terminal ordered audio/core shutdown')
                core_closed();check(clean()==0 and start(C.byref(options()))==2 and sndconfig(C.byref(options()))==2,'early exit retry is idempotent and terminal');system_live=False
            elif a.case=='missing_mask':
                check(sysbind(CORE)==11 and ci().token==first_token and sy().resources==CORE|SOUND and ei().remaining==BOOTSTRAP,'final mask omitting actual audio cannot steal bootstrap authority')
                check(clean()==0,'same preserved bootstrap safely closes real audio after rejected mask');core_closed();system_live=False
            elif a.case=='ordinary_reentrant':
                events,errors=callback_for_cleanup();check(sysclose()==0 and not errors and events==[[4,4,4,4]],'actual completion cannot start terminal global exit or retire owners during ordinary System close')
                check(bind(exit,'jp_exit_status',I)()==2 and ci().token==0,'guarded reentry leaves original bootstrap cancellable for restart');system_live=False
            else:
                if a.case=='late_claim_rejection':
                    check(bind(frames,'jp_queue_reserve',I,U,U)(0,4)==0 and bind(frames,'njStartVertexNonTexOpaque',P)(),
                        'actual unfinished vertex recording prevents device exit claim')
                    check(sysbind(CORE|SOUND)==11 and ci().token==first_token and ci().closing==0 and ei().remaining==BOOTSTRAP and si().resources==59,'late final-bind failure rolls core claims back without cancelling bootstrap sound')
                    check(bind(frames,'jp_queue_discard_active',I)()==0 and bind(frames,'jp_queue_reset_frame',I)()==0,'real producer discards unfinished recording before final claim retry')
                check(sysbind(CORE|SOUND)==0 and ei().remaining==CORE|SOUND and ci().token==first_token and sy().resources==0,'final bind adopts same exact sound authority with core owners')
                delegated=True
                if a.case=='output_final':
                    audio=C.CDLL(str(a.build/'jp_native_audio.dll'))
                    check(bind(sound,'jp_sound_pump',I,U)(4410)==0 and snap(audio,'jp_audio_snapshot',AudioInfo).submitted==4410,'actual original driver PCM reaches owned Windows worker before final shutdown')
                if a.case=='final_retry':
                    hold_gate(1);check(clean()==6 and ei().failed_resource==SOUND and ei().remaining==CORE|SOUND and ci().token==first_token and si().resources==27,'real final audio failure stops before every borrowed core owner')
                    core_still_alive();hold_gate(0)
                events,errors=callback_for_cleanup() if a.case=='final_reentrant' else ([],[])
                if a.case=='process_exit':
                    bind(exit,'sbExitSystem',None)();check(ei().remaining==0 and ci().terminal==ci().finished==1,'original ABI sbExitSystem closes actual audio and core before process exit');core_closed()
                    observations['exit_state']=values(ei());save();bind(exit,'syBtExit',None)();raise AssertionError('process exit returned')
                if a.case=='process_exit_failure':
                    hold_gate(1);bind(exit,'sbExitSystem',None)();check(ei().failed_resource==SOUND and ei().remaining==CORE|SOUND and si().resources==27,'original void exit preserves actual failed audio/core cleanup');core_still_alive()
                    observations['exit_state']=values(ei());save();bind(exit,'syBtExit',None)();raise AssertionError('failed process exit returned')
                check(clean()==0 and ei().remaining==0 and ei().completed==CORE|SOUND and ci().terminal==ci().finished==1,'final exit closes real sound before all core providers')
                if a.case=='final_reentrant':check(not errors and events==[[4,4,4,4]] and sy().status==0,'actual final completion cannot reenter global/System/sound cleanup or poison successful outer status')
                core_closed();check(clean()==0 and cancel(first_token)==2 and start(C.byref(options()))==2,'final exit is idempotent and cannot restart terminal sound');system_live=False
                if a.case=='output_final':
                    actual=snap(audio,'jp_audio_snapshot',AudioInfo);check(actual.state==4 and actual.worker_live==0,'actual Windows audio worker joined before core/provider retirement')
        save();print(a.case,len(labels),'checks',flush=True)
    finally:
        if system_live:
            if delegated:clean()
            else:sndclose(5000);sysclose()
        if prereqs:
            bind(frames,'jp_frames_shutdown',I)();bind(frames,'jp_render_state_shutdown',None)();bind(frames,'jp_textures_shutdown',None)();bind(frames,'jp_queue_shutdown',None)()
if __name__=='__main__':main()
