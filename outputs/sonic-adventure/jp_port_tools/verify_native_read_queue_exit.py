"""Full original parent, real queued I/O, same System/Exit, actual failure/retry."""
import argparse,ctypes as C,datetime as D,faulthandler,hashlib,json,struct,subprocess,sys,threading,time
from pathlib import Path
from verify_native_audio import bind
from verify_native_system import Options as SystemOptions,Info as SystemInfo
from verify_native_device_init import Options as DeviceOptions
from verify_native_game_sound import Options as SoundOptions,Info as SoundInfo
from verify_native_aica import Options as AicaOptions
from verify_native_sound_clock import Options as ClockOptions
from verify_native_input_backend import Source,Sample
from verify_native_rtc import Reader,Info as RTCInfo
from verify_native_exit_logo import ExitInfo
from verify_native_history import Options as HistoryOptions,Info as HistoryInfo
from verify_native_read_queue import Info as QueueInfo,Slot,Disc
from verify_native_gdfs import Info as FSInfo
from verify_native_flash_manager import decode
from verify_native_vmu_fs import Time
I,U,Q,P,S=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p,C.c_size_t
CORE,SOUND,CARD,PRINT,HISTORY,READ_QUEUE=245762,524288,1048576,2097152,4194304,8388608
PARENT=CORE|SOUND|CARD|PRINT;OWNED=PARENT|HISTORY|READ_QUEUE;FINAL=OWNED|64|128|512|1024
CASES=('early','final','bound_data','local_restart','local_then_global','failure_early','failure_final','failure_local','missing_mask','late_claim_rejection',
       'callback','card_callback','foreign','claims','preexisting','outer_guard','start_failure','history_failure_early','history_failure_final','process_exit','process_failure')
class Claim(C.Structure):
    _fields_=[('next_token',Q),('token',Q)]+[(n,U) for n in ('terminal','closing','finished','reserved')]
class Chain(C.Structure):
    _fields_=[('priority',U),('next',U),('callback',Q),('argument',Q)]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    if not a.case:
        cases={}
        for name in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','build','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--case',name,'--output',str(d/name)];run=subprocess.run(cmd,capture_output=True,timeout=180,creationflags=subprocess.CREATE_NO_WINDOW)
            (d/(name+'.log')).write_bytes(run.stdout+run.stderr);expected=70 if name=='process_failure' else 0
            assert run.returncode==expected,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-5000:])
            v=read(d/name/('progress.json' if name.startswith('process_') else 'result.json'));v['actual_exit_code']=run.returncode
            if name.startswith('process_'):
                profile=Path(v['observations']['profile'])
                if name=='process_failure':assert sha(profile)==v['observations']['before_process_exit_profile_sha256']
                else:
                    _,packets=decode(profile.read_bytes());title=b''.join(packets[24+i] for i in range(4))
                    assert struct.unpack_from('<H',title,180)[0]==1 and sum(struct.unpack_from('<24H',title,126))==65
                v['independent_post_process_profile_check']=True
            cases[name]=v;print(name,v['checks'],'checks;',run.returncode,'exit',flush=True)
        out=dict(status='real_queued_reads_same_System_ordinary_early_final_exit_and_failure_retry_verified',checks=sum(v['checks'] for v in cases.values()),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),same_existing_System_registration=True,
            actual_OS_cancel_timeout_forced=False,full_game_init_executed=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['checks'],'checks',flush=True);return
    fault=(d/'native_fault.txt').open('w');faulthandler.enable(file=fault,all_threads=True);labels=[];obs={};keep=[]
    f=C.CDLL(b['frames_DLL']);system=C.CDLL(b['System_DLL']);rtc=C.CDLL(b['RTC_DLL']);ex=C.CDLL(b['authored_exit_DLL']);boot=C.CDLL(b['main_boot_DLL']);hist=C.CDLL(b['history_DLL']);queue=C.CDLL(b['read_queue_DLL']);fs=C.CDLL(b['GDFS_DLL']);sound=C.CDLL(b['game_sound_DLL']);clock=C.CDLL(b['present_DLL'])
    def check(ok,label):
        assert ok,(a.case,label);labels.append(label);(d/'progress.json').write_text(json.dumps(dict(case=a.case,checks=len(labels),assertions=labels),indent=2)+'\n')
    def snapshot(lib,name,kind):
        v=kind();check(bind(lib,name,I,P)(C.byref(v))==0,'actual '+name);return v
    def qi():return snapshot(queue,'jp_read_queue_snapshot',QueueInfo)
    def life():return snapshot(queue,'jp_read_queue_exit_snapshot',Claim)
    def si():return snapshot(system,'jp_system_snapshot',SystemInfo)
    def fi():return snapshot(fs,'jp_gdfs_snapshot',FSInfo)
    clean=bind(ex,'jp_exit_cleanup',I);local=bind(boot,'jp_main_boot_shutdown',I,U);start=bind(system,'jp_system_start_read_queue',I);delegate=bind(system,'jp_system_bind_exit',I,U)
    add=bind(queue,'jp_read_queue_add',I,C.c_char_p,P,S);serve=bind(queue,'jp_read_queue_serve',I);finish=bind(queue,'jp_read_queue_exit_finish',I,Q);cancel_claim=bind(queue,'jp_read_queue_exit_cancel',I,Q)
    clock_value=[int((D.datetime(2026,11,6,12)-D.datetime(1950,1,1)).total_seconds())]
    @Source
    def input_source(ctx,out):C.memset(out,0,4*C.sizeof(Sample));out[0]=Sample(1,0,0,0,0,0,0,0);return 1
    @Reader
    def reader(ctx,out):out[0]=clock_value[0];return 0
    keep.extend([input_source,reader])
    for name,args in [('jp_queue_initialize',(8<<20,)),('jp_frames_initialize',(8,128,32<<20)),('jp_textures_initialize',(32,1024,32<<20)),('jp_render_state_initialize',(128,1<<20))]:
        types=[S] if len(args)==1 else [U,U,S] if len(args)==3 else [U,S];check(bind(f,name,I,*types)(*args)==0,'actual borrowed render prerequisite '+name)
    buf=C.addressof((C.c_ubyte*0x40000).in_dll(f,'texture_buffer'));check(bind(f,'jp_render_state_register_buffer',I,P,S,U)(buf,0x40000,4096)==0,'original shared main buffer registered')
    vmu=d/'vmu';vmu.mkdir();check(bind(f,'jp_vmu_store_initialize',I,C.c_wchar_p)(str(vmu))==0 and bind(f,'jp_vmu_store_create',I,U)(0)==0 and bind(f,'jp_vmu_store_open',I,U)(0)==0 and bind(f,'jp_lcd_initialize',I,U)(1)==0,'actual borrowed VMU image and LCD')
    track=C.create_unicode_buffer(str(a.track.resolve()));keep.append(track)
    options=SystemOptions(96,0,DeviceOptions(640,480,0,0,5000,0,64<<20),C.addressof(track),2352,3,5000,0,C.cast(input_source,P).value,None,C.cast(reader,P).value,None)
    so=SoundOptions(56,0,2<<20,5000,AicaOptions(32,1,16384,40,5000,1,1,0),None);co=ClockOptions(56,441,8192,5,None,None,0,None,None)
    def boot_up():
        check(bind(system,'jp_system_configure',I,P)(C.byref(options))==0 and bind(boot,'jp_main_boot_configure',I,P,P)(C.byref(so),C.byref(co))==0,'actual copied complete parent options')
        check(bind(boot,'jp_main_boot_initialize',I)()==0 and si().resources==PARENT,'complete original higher bootstrap and real card/font/sound/core providers')
    boot_up()
    if a.case=='start_failure':
        foreign_lease=Q();check(bind(fs,'jp_gdfs_read_queue_claim',I,P)(C.byref(foreign_lease))==0,'independent actual GDFS client already owns lease')
        check(start()==12 and si().resources==PARENT and not qi().initialized and not life().token,'rejected real constructor acquires no queue cleanup authority')
        check(bind(fs,'jp_gdfs_read_queue_release',I,Q)(foreign_lease.value)==0,'independent client releases its own actual lease')
    if a.case in ('claims','preexisting'):
        check(bind(queue,'jp_read_queue_initialize',I)()==0,'independent real queue initialized before explicit handoff');old=bytes(qi())
        check(start()==2 and bytes(qi())==old,'automatic startup rejects and preserves preexisting queue owner')
        if a.case=='claims':
            token=Q();check(bind(queue,'jp_read_queue_exit_claim',I,P)(C.byref(token))==0,'claim actual original queue')
            out=Q(0xabcdef);check(bind(queue,'jp_read_queue_exit_claim',I,P)(C.byref(out))==2 and out.value==0xabcdef and cancel_claim(token.value+1)==2,'duplicate/wrong claims preserve existing authority and caller output')
            check(bind(system,'jp_system_adopt_read_queue',I)()==2 and bind(system,'jp_system_main_initialization_guard',I,U)(1)==0,'explicit handoff requires actual parent guard')
            before=bytes(si());old_claim=bytes(life());check(bind(system,'jp_system_adopt_read_queue',I)()==2 and bytes(si())==before and bytes(life())==old_claim,'already claimed explicit handoff rejects before System mutation')
            check(cancel_claim(token.value)==0 and bind(system,'jp_system_adopt_read_queue',I)()==0 and life().token>token.value,'guarded handoff adopts same original queue with a fresh claim')
            check(bind(system,'jp_system_main_initialization_guard',I,U)(0)==0,'caller releases its own explicit parent guard')
        else:check(bind(queue,'jp_read_queue_shutdown',I)()==0 and start()==0,'independent queue closes itself before System starts its own lifetime')
    else:
        if a.case=='outer_guard':check(bind(system,'jp_system_main_initialization_guard',I,U)(1)==0,'actual outer constructor guard acquired')
        check(start()==0,'same System starts and adopts actual original queue')
    profile=d/'profile';profile.mkdir();directory=C.create_unicode_buffer(str(profile));keep.append(directory);path=profile/'SONIC_FLASH.DAT';ho=HistoryOptions(32,1,C.addressof(directory),C.addressof(track),2352,0)
    check(bind(system,'jp_system_start_history',I,P)(C.byref(ho))==0 and si().resources==OWNED,'same existing registration covers queue then genuine original history')
    if a.case=='outer_guard':
        check(clean()==4 and not life().terminal,'new queue/history adoption preserves outer construction guard');check(bind(system,'jp_system_main_initialization_guard',I,U)(0)==0,'outer caller alone releases its guard')
    token=life().token;check(token and not life().terminal,'actual queue token remains nonterminal until close')
    check(bind(queue,'jp_read_queue_shutdown',I)()==2 and bind(queue,'jp_read_queue_initialize',I)()==2,'independent destruction/replacement cannot steal delegated queue')
    payload=C.create_string_buffer(bytes(range(32)));keep.append(payload)
    check(bind(hist,'peer_history_play',I,U,U,U,P)(0,0,0,payload)==0 and bind(hist,'peer_history_play',I,U,U,U,P)(1,0,0,None)==0,'actual original UF records application work before close')
    clock_value[0]+=3900;bind(rtc,'syRtcExecServer',None)()
    disc=Disc(a.track);files=disc.files();name,(fad,n,_) = next((name,data) for name,data in sorted(files.items()) if not data[2] and 64*1024<=data[1]<=512*1024 and len(name)<=31)
    capacity=(n+2047)&~2047;oracle=disc.sectors(fad,capacity//2048);disc.f.close()
    def buffer():
        raw=(C.c_ubyte*(capacity+96))();C.memset(raw,0xa5,C.sizeof(raw));at=(C.addressof(raw)+63)&~31;keep.append(raw);return raw,at
    raw,at=buffer();check(add(name.encode(),at,capacity)==0,'actual bounded destination queued under System authority')
    if a.case=='callback':
        h=bind(fs,'gdFsOpen',P,C.c_char_p,P)(name.encode(),None);other,op=buffer();events=[];Callback=C.CFUNCTYPE(None,P)
        @Callback
        def completed(ctx):
            old=ExitInfo();bind(ex,'jp_exit_snapshot',I,P)(C.byref(old));before=Claim();bind(queue,'jp_read_queue_exit_snapshot',I,P)(C.byref(before))
            values=(clean(),bind(system,'jp_system_shutdown',I)(),delegate(FINAL),finish(token))
            new=ExitInfo();bind(ex,'jp_exit_snapshot',I,P)(C.byref(new));after=Claim();bind(queue,'jp_read_queue_exit_snapshot',I,P)(C.byref(after))
            events.append((values,old.cleanup_calls==new.cleanup_calls,bytes(before)==bytes(after)))
        keep.append(completed);bind(fs,'gdFsEntryRdEndFunc',None,P,Callback,P)(h,completed,None)
        check(h and bind(fs,'gdFsReqRd32',I,P,I,P)(h,capacity//2048,op)==capacity//2048,'actual independent GDFS read dispatches real completion callback')
        deadline=time.monotonic()+10
        while not events:bind(fs,'gdFsExecServer',None)();assert time.monotonic()<deadline
        check(events==[((4,4,4,4),True,True)],'actual GDFS callback rejects whole cleanup/handoff before terminal flags or counters change');bind(fs,'gdFsClose',None,P)(h)
    def pending():
        check(serve()==0 and qi().current==1 and qi().active_handle and fi().pending,'actual original queued read is pending at shutdown')
        copied=bind(fs,'gdFsGetNumRd',I,P)(qi().active_handle)
        check(0<=copied<=capacity and C.string_at(at,copied)==oracle[:copied],'actual transferred prefix matches independent raw sectors')
    pending()
    if a.case=='local_restart':
        check(local(5000)==0 and not qi().initialized and not life().token and not life().terminal and si().resources==0,'ordinary full parent shutdown releases queue claim and actual GDFS lease')
        old_buffer=(at,C.string_at(at,capacity));check(bind(f,'jp_queue_reset_frame',I)()==0,'borrowed render queue reset for original parent restart');boot_up()
        check(start()==0 and life().token>token and bind(system,'jp_system_start_history',I,P)(C.byref(ho))==0,'same original parent queue and history restart with fresh authority');token=life().token
        raw,at=buffer();check(add(name.encode(),at,capacity)==0,'new lifetime borrows a new real destination');pending();obs['old_buffer_sha256']=hashlib.sha256(old_buffer[1]).hexdigest()
    if a.case=='foreign':
        before=bytes(life());out=Claim();C.memset(C.byref(out),0x5a,C.sizeof(out));old=bytes(out);results=[]
        def other_thread():results.extend((finish(token),cancel_claim(token),bind(queue,'jp_read_queue_exit_snapshot',I,P)(C.byref(out)),clean(),bind(system,'jp_system_shutdown',I)()))
        t=threading.Thread(target=other_thread);t.start();t.join();check(results==[3]*5 and bytes(out)==old and bytes(life())==before,'foreign cleanup preserves actual authority and caller output')
    if a.case=='missing_mask':
        before=bytes(life());check(delegate(FINAL&~READ_QUEUE)==11 and si().failed_resource==READ_QUEUE and bytes(life())==before,'missing queue mask rejects before claims or terminal mutation')
    if a.case=='late_claim_rejection':
        before=bytes(life());check(bind(f,'jp_queue_reserve',I,U,U)(0,4)==0 and bind(f,'njStartVertexNonTexOpaque',P)(),'actual unfinished recording conflicts with later device claim')
        check(delegate(FINAL)==11 and bytes(life())==before and si().resources==OWNED,'late real rejection preserves bootstrap and exact borrowed queue token')
        check(bind(f,'jp_queue_discard_active',I)()==0 and bind(f,'jp_queue_reset_frame',I)()==0,'actual recording owner retires conflicting work')
    card_events=[]
    if a.case=='card_callback':
        def settle():
            for _ in range(10000):
                bind(f,'buExecServer',None)()
                if bind(f,'buGetLastError',I,I)(0)!=-1:return
            raise AssertionError('actual card work did not settle')
        settle();date=Time(2026,11,6,12,0,0,5);label=C.create_string_buffer(b'QUEUE EXIT QUALIFICATION'.ljust(32,b'\0'),32);keep.extend([date,label])
        check(bind(f,'buFormatDisk',I,I,P,I,P,U)(0,label,0,C.byref(date),0)==0,'actual card format queued');settle()
        original=bind(f,'card_complete',I,I,I,I,U);Callback=C.CFUNCTYPE(I,I,I,I,U);extra,ep=buffer()
        @Callback
        def completed(drive,op,status,param):
            current=Claim();bind(queue,'jp_read_queue_exit_snapshot',I,P)(C.byref(current))
            card_events.append((clean(),finish(token),current.terminal,add(name.encode(),ep,capacity),bind(hist,'peer_history_play',I,U,U,U,P)(0,0,0,None),status))
            return original(drive,op,status,param)
        data=C.create_string_buffer(bytes((i*37+11)&255 for i in range(5120)));keep.extend([completed,data]);bind(f,'buSetCompleteCallback',None,Callback)(completed)
        check(bind(f,'buSaveFile',I,I,C.c_char_p,P,U,P,U)(0,b'QUEUEEXIT',data,10,C.byref(date),0)==0 and bind(f,'buGetLastError',I,I)(0)==-1,'actual card save remains pending before queue closes')
    final=a.case in ('final','bound_data','failure_final','missing_mask','late_claim_rejection','history_failure_final','process_exit','process_failure')
    if final:check(delegate(FINAL)==0,'final Exit borrows exact queue token after every core claim succeeds')
    if a.case=='bound_data':
        deadline=time.monotonic()+15;current=QueueInfo()
        while True:
            assert bind(queue,'jp_read_queue_snapshot',I,P)(C.byref(current))==0
            if not current.queued:break
            assert serve()==0 and time.monotonic()<deadline
        check(C.string_at(at,capacity)==oracle and not life().terminal,'actual original reads remain usable after final Exit binding')
        raw,at=buffer();check(add(name.encode(),at,capacity)==0,'bound queue can accept a new real caller destination');pending()
    chain_record=Chain();chain_handle=0;blocker=None
    failing_queue=a.case in ('failure_early','failure_final','failure_local','process_failure')
    if failing_queue:
        chain_handle=fi().vblank;check(bind(clock,'jp_vblank_chain_describe',I,U,P)(chain_handle,C.byref(chain_record))==0,'actual original GDFS frame handler identified')
        bind(clock,'syChainDeleteHandler',None,U)(chain_handle)
        check(bind(clock,'jp_vblank_chain_describe',I,U,P)(chain_handle,C.byref(Chain()))!=0,'real handler removal prevents native checked file close')
    if a.case.startswith('history_failure') or a.case=='local_then_global':
        kernel=C.WinDLL('kernel32',use_last_error=True);blocker=bind(kernel,'CreateFileW',P,C.c_wchar_p,U,U,P,U,U,P)(str(path),0x80000000,1,None,3,0x80,None)
        check(blocker and blocker!=P(-1).value,'real OS sharing denial prevents later history commit')
    if a.case.startswith('process_'):
        obs.update(profile=str(path),before_process_exit_profile_sha256=sha(path));(d/'progress.json').write_text(json.dumps(dict(case=a.case,checks=len(labels),assertions=labels,observations=obs,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__)),indent=2)+'\n')
        bind(ex,'syBtExit',None)();raise AssertionError('actual process exit returned')
    if failing_queue:
        before=qi();code=local(5000) if a.case=='failure_local' else clean();after=qi();authority=life()
        check(code==(8 if a.case=='failure_local' else 6) and after.initialized and after.lease==before.lease and after.active_handle==before.active_handle and after.queued==before.queued,'real failed full-path queue close retains slots destination handle and GDFS client lease')
        check(authority.token==token and authority.closing and not authority.finished and bool(authority.terminal)==(a.case!='failure_local') and cancel_claim(token)==2,'failed close retains irreversible exact authority until retry')
        check(add(name.encode(),at,capacity)==2 and bind(queue,'jp_read_queue_cancel',I)()==2,'application cannot mutate retained buffers once close has started')
        check(snapshot(rtc,'jp_rtc_snapshot',RTCInfo).initialized and snapshot(sound,'jp_sound_snapshot',SoundInfo).resources and snapshot(hist,'jp_history_snapshot',HistoryInfo).resources and fi().work,'dependent real history sound RTC and filesystem remain alive after queue failure')
        # Original syChainAddHandler chooses the lowest free ID. Fill only
        # lower free slots without dispatch, then restore the exact original
        # callback/argument/priority at its still-owned expected ID.
        temporary=[];Callback=C.CFUNCTYPE(None,P)
        @Callback
        def idle(ctx):pass
        keep.append(idle)
        for number in range(1,chain_handle):
            if bind(clock,'jp_vblank_chain_describe',I,U,P)(number,C.byref(Chain())):
                handle=bind(clock,'syChainAddHandler',U,C.c_int16,P,U,P)(0x900,idle,200,None);check(handle==number,'original allocator fills only lower free handler slots');temporary.append(handle)
        restored=bind(clock,'syChainAddHandler',U,C.c_int16,P,U,P)(0x900,chain_record.callback,chain_record.priority&0x7fffffff,chain_record.argument)
        check(restored==chain_handle,'same actual original GDFS handler restored at its expected native ID')
        for handle in temporary:bind(clock,'syChainDeleteHandler',None,U)(handle)
    if blocker:
        old_hash=sha(path);code=local(5000) if a.case=='local_then_global' else clean();authority=life()
        check(code==(8 if a.case=='local_then_global' else 6) and sha(path)==old_hash and bool(authority.terminal)!=(a.case=='local_then_global') and authority.finished and not qi().initialized and not qi().lease,'later real history failure preserves profile after queue has already closed exactly once')
        check(bind(kernel,'CloseHandle',I,P)(blocker)!=0 and bind(hist,'jp_history_recover',I)()==0,'corrected OS condition recovers actual retained history')
    code=local(5000) if a.case=='failure_local' else clean();check(code==0,'actual complete ordered cleanup succeeds after any recovery')
    if a.case=='failure_local':check(not life().token and not life().terminal and si().phase==1,'ordinary retry releases queue claim for a later native restart')
    else:
        current=life();check(current.token==token and current.terminal and current.finished and si().phase==6,'same queue claim finishes terminally through real global exit')
        count=qi().cancels;check(clean()==0 and qi().cancels==count and cancel_claim(token)==2,'repeated terminal cleanup neither re-closes queue nor revokes terminal authority')
    check(not qi().initialized and not qi().lease and not qi().queued and not qi().active_handle,'actual queue slots file handle and GDFS client lease fully retired')
    before=at-C.addressof(raw);check(bytes(raw[:before])==b'\xa5'*before and bytes(raw[before+capacity:])==b'\xa5'*(len(raw)-before-capacity),'real caller destination canaries survive full shutdown')
    stopped=C.string_at(at,capacity)
    for _ in range(8):bind(fs,'gdFsExecServer',None)()
    check(C.string_at(at,capacity)==stopped,'retired actual reader cannot write to released caller destination')
    if a.case=='local_restart':check(C.string_at(old_buffer[0],capacity)==old_buffer[1],'new lifetime cannot resume an orphaned old destination')
    if a.case=='card_callback':check(card_events and all(e==(4,4,0,0,0,0) for e in card_events),'actual card completion can queue work and record history before queue retirement while nested finish rejects')
    _,packets=decode(path.read_bytes());title=b''.join(packets[24+i] for i in range(4))
    check(struct.unpack_from('<H',title,180)[0]==(2 if a.case=='card_callback' else 1) and struct.unpack_from('<H',title,174)[0]==1 and sum(struct.unpack_from('<24H',title,126))==65,'independent actual UF profile contains all work after queue closes and before RTC retires')
    check(bind(f,'jp_vmu_store_status',I)()==0 and bind(f,'jp_lcd_shutdown',I)()==0 and bind(f,'jp_vmu_store_shutdown',I)()==0,'caller still owns and closes borrowed VMU/LCD')
    if not final:
        check(bind(f,'jp_queue_reset_frame',I)()==0 and bind(f,'jp_frames_shutdown',I)()==0,'caller retires borrowed frame owner after early/local cleanup');bind(f,'jp_render_state_shutdown',None)();bind(f,'jp_textures_shutdown',None)();bind(f,'jp_queue_shutdown',None)()
    faulthandler.disable();fault.close();out=dict(case=a.case,checks=len(labels),assertions=labels,observations=obs,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10)
    (d/'result.json').write_text(json.dumps(out,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
if __name__=='__main__':main()
