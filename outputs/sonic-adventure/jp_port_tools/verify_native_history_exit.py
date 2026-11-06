"""Actual full parent -> owned History -> ordinary/early/final global exit."""
import argparse,ctypes as C,datetime as D,faulthandler,hashlib,json,struct,subprocess,sys,threading
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
from verify_native_flash_manager import decode
from verify_native_vmu_fs import Time
I,U,Q,P,S=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p,C.c_size_t
CORE,SOUND,CARD,PRINT,HISTORY=245762,524288,1048576,2097152,4194304
PARENT=CORE|SOUND|CARD|PRINT;FINAL=PARENT|HISTORY|64|128|512|1024
CASES=('early','final','local_restart','denied_early','denied_final','denied_local',
       'missing_mask','late_claim_rejection','callback','constructor_callback','foreign',
       'claim_cancel','preexisting_owner','partial_start','corrected_identity','outer_guard',
       'pending_card','after_save_elapsed','process_exit','process_failure')
class Claim(C.Structure):
    _fields_=[('next_token',Q),('token',Q)]+[(n,U) for n in ('terminal','closing','finished','reserved')]
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
            for k in ('workspace','build','track'):cmd+=['--'+k,str(getattr(a,k).resolve())]
            cmd+=['--case',name,'--output',str(d/name)];run=subprocess.run(cmd,capture_output=True,timeout=180,creationflags=subprocess.CREATE_NO_WINDOW)
            (d/(name+'.log')).write_bytes(run.stdout+run.stderr);expected=70 if name=='process_failure' else 0
            assert run.returncode==expected,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-5000:])
            result=read(d/name/('progress.json' if name.startswith('process_') else 'result.json'));result['actual_exit_code']=run.returncode
            if name.startswith('process_'):
                path=Path(result['observations']['profile']);used,packets=decode(path.read_bytes());title=b''.join(packets[24+i] for i in range(4))
                if name=='process_failure':assert sha(path)==result['observations']['before_process_exit_profile_sha256']
                else:assert struct.unpack_from('<H',title,180)[0]==1 and sum(struct.unpack_from('<24H',title,126))==65
                result['independent_post_process_disk_check']=True
            cases[name]=result;print(name,result['checks'],'checks;',run.returncode,'exit',flush=True)
        out=dict(status='full_original_boot_System_owned_history_ordinary_early_final_exit_and_failure_recovery_verified',
            checks=sum(x['checks'] for x in cases.values()),cases=cases,build_verification_sha256=sha(a.build/'build_verification.json'),
            verifier_sha256=sha(__file__),same_existing_System_registration=True,
            full_game_init_or_gameplay_executed=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['checks'],'checks',flush=True);return
    fault=(d/'native_fault.txt').open('w');faulthandler.enable(file=fault,all_threads=True)
    f=C.CDLL(b['frames_DLL']);system=C.CDLL(b['System_DLL']);rtc=C.CDLL(b['RTC_DLL']);ex=C.CDLL(b['authored_exit_DLL']);boot=C.CDLL(b['main_boot_DLL']);hist=C.CDLL(b['history_DLL']);flash=C.CDLL(b['flash_DLL']);sound=C.CDLL(b['game_sound_DLL'])
    labels=[];obs={};keep=[];events=[];trigger=[False];clock_value=[int((D.datetime(2026,11,6,12)-D.datetime(1950,1,1)).total_seconds())]
    def check(ok,label):
        assert ok,(a.case,label);labels.append(label);(d/'progress.json').write_text(json.dumps(dict(case=a.case,checks=len(labels),assertions=labels),indent=2)+'\n')
    def snapshot(lib,name,kind):
        v=kind();check(bind(lib,name,I,P)(C.byref(v))==0,'actual '+name);return v
    def hi():return snapshot(hist,'jp_history_snapshot',HistoryInfo)
    def claim():return snapshot(hist,'jp_history_exit_snapshot',Claim)
    def si():return snapshot(system,'jp_system_snapshot',SystemInfo)
    clean=bind(ex,'jp_exit_cleanup',I);local=bind(boot,'jp_main_boot_shutdown',I,U)
    start_hist=bind(system,'jp_system_start_history',I,P);delegate=bind(system,'jp_system_bind_exit',I,U)
    @Source
    def source(ctx,out):C.memset(out,0,4*C.sizeof(Sample));out[0]=Sample(1,0,0,0,0,0,0,0);return 1
    @Reader
    def clock(ctx,out):
        if trigger[0]:
            old=ExitInfo();bind(ex,'jp_exit_snapshot',I,P)(C.byref(old));life=Claim();bind(hist,'jp_history_exit_snapshot',I,P)(C.byref(life))
            nested=(clean(),bind(system,'jp_system_shutdown',I)(),delegate(FINAL),bind(hist,'jp_history_shutdown',I)())
            new=ExitInfo();bind(ex,'jp_exit_snapshot',I,P)(C.byref(new));after=Claim();bind(hist,'jp_history_exit_snapshot',I,P)(C.byref(after))
            events.append((nested,old.cleanup_calls==new.cleanup_calls,bytes(life)==bytes(after)))
        out[0]=clock_value[0];return 0
    keep.extend([source,clock])
    for n,args in [('jp_queue_initialize',(8<<20,)),('jp_frames_initialize',(8,128,32<<20)),('jp_textures_initialize',(32,1024,32<<20)),('jp_render_state_initialize',(128,1<<20))]:
        check(bind(f,n,I,*([S] if len(args)==1 else [U,U,S] if len(args)==3 else [U,S]))(*args)==0,'borrowed real prerequisite '+n)
    buf=C.addressof((C.c_ubyte*0x40000).in_dll(f,'texture_buffer'));check(bind(f,'jp_render_state_register_buffer',I,P,S,U)(buf,0x40000,4096)==0,'original shared main buffer registered')
    vmu=d/'vmu';vmu.mkdir();check(bind(f,'jp_vmu_store_initialize',I,C.c_wchar_p)(str(vmu))==0,'borrowed genuine VMU profile')
    check(bind(f,'jp_vmu_store_create',I,U)(0)==0 and bind(f,'jp_vmu_store_open',I,U)(0)==0 and bind(f,'jp_lcd_initialize',I,U)(1)==0,'fresh real VMU image and LCD')
    track=C.create_unicode_buffer(str(a.track.resolve()));keep.append(track)
    options=SystemOptions(96,0,DeviceOptions(640,480,0,0,5000,0,64<<20),C.addressof(track),2352,3,5000,0,C.cast(source,P).value,None,C.cast(clock,P).value,None)
    sound_options=SoundOptions(56,0,2<<20,5000,AicaOptions(32,1,16384,40,5000,1,1,0),None);clock_options=ClockOptions(56,441,8192,5,None,None,0,None,None)
    def boot_up():
        check(bind(system,'jp_system_configure',I,P)(C.byref(options))==0 and bind(boot,'jp_main_boot_configure',I,P,P)(C.byref(sound_options),C.byref(clock_options))==0,'actual copied System sound and clock options')
        check(bind(boot,'jp_main_boot_initialize',I)()==0 and si().resources==PARENT,'complete original successful higher constructor and all actual providers')
    boot_up();profile=d/'profile';profile.mkdir();path=profile/'SONIC_FLASH.DAT';directory=C.create_unicode_buffer(str(profile));keep.append(directory)
    ho=HistoryOptions(32,1,C.addressof(directory),C.addressof(track),2352,0)
    if a.case=='partial_start':path.write_bytes(b'\xff'*131072);old_hash=sha(path)
    if a.case=='corrected_identity':
        missing=C.create_unicode_buffer(str(d/'absent_track.bin'));keep.append(missing);bad=HistoryOptions(32,1,C.addressof(directory),C.addressof(missing),2352,0)
        check(start_hist(C.byref(bad))==12 and si().resources==PARENT and not path.exists(),'pre-acquisition identity failure preserves core and acquires no foreign cleanup authority')
    if a.case in ('claim_cancel','preexisting_owner'):
        check(bind(hist,'jp_history_configure',I,P)(C.byref(ho))==0 and bind(hist,'jp_history_initialize',I)()==0,'actual independently initialized original history')
        before=bytes(hi());check(start_hist(C.byref(ho))==2 and bytes(hi())==before,'automatic startup rejects and preserves independent existing History owner')
        if a.case=='claim_cancel':
            token=Q();check(bind(hist,'jp_history_exit_claim',I,P)(C.byref(token))==0,'claim actual existing history provider')
            other=Q(0xabcdef);check(bind(hist,'jp_history_exit_claim',I,P)(C.byref(other))==2 and other.value==0xabcdef,'second claimant cannot replace authority or output')
            check(bind(hist,'jp_history_exit_cancel',I,Q)(token.value+1)==2 and bind(hist,'jp_history_exit_cancel',I,Q)(token.value)==0,'wrong cancellation rejected and untouched correct claim can cancel')
            check(bind(system,'jp_system_adopt_history',I)()==2,'explicit handoff requires actual constructor guard')
            check(bind(system,'jp_system_main_initialization_guard',I,U)(1)==0 and bind(system,'jp_system_adopt_history',I)()==0,'explicit guarded handoff adopts same actual instance')
            check(claim().token>token.value and bind(system,'jp_system_main_initialization_guard',I,U)(0)==0,'reclaimed authority never reuses earlier token')
        else:check(bind(hist,'jp_history_shutdown',I)()==0 and start_hist(C.byref(ho))==0,'foreign owner closes itself before System starts its own actual lifetime')
    else:
        if a.case=='outer_guard':check(bind(system,'jp_system_main_initialization_guard',I,U)(1)==0,'outer original-parent construction guard acquired')
        if a.case=='constructor_callback':bind(rtc,'syRtcSetServerMode',None,I)(1);trigger[0]=True
        code=start_hist(C.byref(ho));obs['history_start_result']=code
        if a.case=='partial_start':check(code==12 and si().resources==(PARENT|HISTORY),'failed actual UF constructor retains resources under same System registration')
        else:check(code==0,'System runs real history constructor and adopts exact lower owner')
        if trigger[0]:
            check(events and all(e==((4,4,4,4),True,True) for e in events),'real constructor RTC callbacks cannot commit early cleanup or final handoff')
            trigger[0]=False;events.clear()
        if a.case=='outer_guard':
            check(clean()==4 and not claim().terminal,'history attachment preserves outer constructor guard')
            check(bind(system,'jp_system_main_initialization_guard',I,U)(0)==0,'outer constructor alone releases its guard')
    token=claim().token;check(token and not claim().terminal and si().resources==(PARENT|HISTORY),'same System registration covers actual card history sound core and font')
    check(bind(hist,'jp_history_shutdown',I)()==2 and bind(hist,'jp_history_configure',I,P)(C.byref(ho))==2,'independent ordinary destruction and replacement are rejected while delegated')
    if a.case!='partial_start':
        payload=C.create_string_buffer(bytes(range(32)));keep.append(payload)
        check(bind(hist,'peer_history_play',I,U,U,U,P)(0,0,0,payload)==0 and bind(hist,'peer_history_play',I,U,U,U,P)(1,0,0,None)==0,'original UF can still record actual application work before close')
        clock_value[0]+=3900;bind(rtc,'syRtcExecServer',None)()
        if a.case=='after_save_elapsed':
            check(bind(hist,'jp_history_save',I)()==0 and not hi().dirty,'explicit original history save completes before final elapsed-time interval')
            clock_value[0]+=60;bind(rtc,'syRtcExecServer',None)()
    if a.case=='local_restart':
        check(local(5000)==0 and si().resources==0 and not claim().token and not claim().terminal,'ordinary full parent shutdown releases completed nonterminal history authority')
        check(bind(f,'jp_queue_reset_frame',I)()==0,'borrowed queue reset for next genuine constructor');boot_up()
        check(start_hist(C.byref(ho))==0 and claim().token>token,'full original parent and persisted UF profile restart with a new claim');token=claim().token
    if a.case=='foreign':
        before=bytes(claim());out=Claim();C.memset(C.byref(out),0x5a,C.sizeof(out));old_out=bytes(out);results=[]
        def other():results.extend((bind(hist,'jp_history_exit_finish',I,Q)(token),bind(hist,'jp_history_exit_cancel',I,Q)(token),bind(hist,'jp_history_exit_snapshot',I,P)(C.byref(out)),clean()))
        t=threading.Thread(target=other);t.start();t.join();check(results==[3]*4 and bytes(out)==old_out and bytes(claim())==before,'foreign cleanup cannot alter real authority or caller outputs')
    if a.case=='callback':
        bind(rtc,'syRtcSetServerMode',None,I)(1);trigger[0]=True
        check(bind(hist,'jp_history_save',I)()==0,'actual delegated history remains usable until close begins')
        trigger[0]=False;check(events and all(e==((4,4,4,4),True,True) for e in events),'real save callbacks reject cleanup before counters or terminal flags change')
    card_events=[]
    if a.case=='pending_card':
        def settle():
            for _ in range(10000):
                bind(f,'buExecServer',None)()
                if bind(f,'buGetLastError',I,I)(0)!=-1:return
            raise AssertionError('original pending card work did not settle')
        settle();date=Time(2026,11,6,12,0,0,5);name=C.create_string_buffer(b'HISTORY EXIT QUALIFICATION'.ljust(32,b'\0'),32);keep.extend([date,name])
        check(bind(f,'buFormatDisk',I,I,P,I,P,U)(0,name,0,C.byref(date),0)==0,'actual original card format queued');settle()
        complete=bind(f,'card_complete',I,I,I,I,U);Callback=C.CFUNCTYPE(I,I,I,I,U)
        @Callback
        def completed(drive,op,status,param):
            life=Claim();bind(hist,'jp_history_exit_snapshot',I,P)(C.byref(life))
            card_events.append((clean(),bind(hist,'jp_history_exit_finish',I,Q)(token),life.terminal,bind(hist,'peer_history_play',I,U,U,U,P)(0,0,0,None),status))
            return complete(drive,op,status,param)
        data=C.create_string_buffer(bytes((i*37+11)&255 for i in range(5120)));keep.extend([completed,data]);bind(f,'buSetCompleteCallback',None,Callback)(completed)
        check(bind(f,'buSaveFile',I,I,C.c_char_p,P,U,P,U)(0,b'HISTEXIT',data,10,C.byref(date),0)==0 and bind(f,'buGetLastError',I,I)(0)==-1,'actual ten-block card save remains pending when global exit starts')
    final=a.case in ('final','denied_final','missing_mask','late_claim_rejection','after_save_elapsed','process_exit','process_failure')
    if a.case=='missing_mask':
        before=bytes(claim());check(delegate(FINAL&~HISTORY)==11 and si().failed_resource==HISTORY and bytes(claim())==before,'missing History bit rejects binding before claims or terminal mutation')
    if a.case=='late_claim_rejection':
        before=bytes(claim());check(bind(f,'jp_queue_reserve',I,U,U)(0,4)==0 and bind(f,'njStartVertexNonTexOpaque',P)(),'real recording creates a later device claim conflict')
        check(delegate(FINAL)==11 and bytes(claim())==before and si().resources==(PARENT|HISTORY),'actual late rejection preserves bootstrap and borrowed history authority')
        check(bind(f,'jp_queue_discard_active',I)()==0 and bind(f,'jp_queue_reset_frame',I)()==0,'actual producer retires conflicting recording')
    if final:check(delegate(FINAL)==0,'final Exit adopts the exact existing history token with all core claims')
    blocker=None
    if a.case in ('denied_early','denied_final','denied_local','process_failure'):
        kernel=C.WinDLL('kernel32',use_last_error=True);old_hash=sha(path)
        blocker=bind(kernel,'CreateFileW',P,C.c_wchar_p,U,U,P,U,U,P)(str(path),0x80000000,1,None,3,0x80,None)
        check(blocker and blocker!=P(-1).value,'real OS sharing denial prevents atomic final profile replacement')
    if a.case.startswith('process_'):
        obs.update(profile=str(path),before_process_exit_profile_sha256=sha(path))
        (d/'progress.json').write_text(json.dumps(dict(case=a.case,checks=len(labels),assertions=labels,observations=obs,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__)),indent=2)+'\n')
        bind(ex,'syBtExit',None)();raise AssertionError('real process exit returned')
    if blocker:
        before=hi();code=local(5000) if a.case=='denied_local' else clean()
        check(code==(8 if a.case=='denied_local' else 6) and sha(path)==old_hash,'failed full-path cleanup reports actual error and preserves complete prior profile')
        after=hi();life=claim()
        check(after.resources==before.resources and after.work3==before.work3 and after.work4==before.work4 and life.token==token and life.closing and not life.finished,'failed global/local close retains actual UF work profile and authority')
        check(bool(life.terminal)==(a.case!='denied_local') and bind(hist,'jp_history_exit_cancel',I,Q)(token)==2,'partial close cannot cancel; terminal status is irreversible')
        check(snapshot(rtc,'jp_rtc_snapshot',RTCInfo).initialized and snapshot(sound,'jp_sound_snapshot',SoundInfo).resources,'dependent real RTC and timed sound remain owned after history failure')
        handle=bind(C.CDLL(b['GDFS_DLL']),'gdFsOpen',P,C.c_char_p,P)(b'masaru.drv',None);size=I()
        fs=C.CDLL(b['GDFS_DLL']);check(handle and bind(fs,'gdFsGetFileSize',I,P,P)(handle,C.byref(size)) and size.value>0,'actual GDFS remains usable until history cleanup succeeds');bind(fs,'gdFsClose',None,P)(handle)
        check(bind(kernel,'CloseHandle',I,P)(blocker)!=0 and bind(hist,'jp_history_recover',I)()==0,'corrected real OS condition permits recovery under retained authority')
    code=local(5000) if a.case=='denied_local' else clean()
    check(code==0,'actual full ordered cleanup completes after successful history persistence')
    if a.case=='denied_local':check(not claim().token and not claim().terminal and si().phase==1,'ordinary retry releases finished nonterminal claims for restart')
    else:
        life=claim();check(life.token==token and life.terminal and life.finished and si().phase==6,'same history token finishes terminally and System observes completed real exit')
        closed=hi().closes;check(clean()==0 and hi().closes==closed,'repeated terminal cleanup performs no duplicate history close')
        check(bind(hist,'jp_history_exit_cancel',I,Q)(token)==2 and bind(hist,'jp_history_configure',I,P)(C.byref(ho))==2,'terminal owner cannot cancel or restart a retired history provider')
    check(hi().resources==0 and not hi().work3 and not hi().work4,'actual original UF profile and refresh work retired')
    if a.case=='pending_card':
        check(card_events and all(e==(4,4,0,0,0) for e in card_events),'actual card completion can record history before history retires and cannot reenter shutdown')
        image=(vmu/'VMU_A1.VMU').read_bytes();fat=struct.unpack_from('<256H',image,254*512);found=False
        for block in range(253,240,-1):
            for offset in range(block*512,(block+1)*512,32):
                entry=image[offset:offset+32]
                if entry[0] and entry[4:16].rstrip(b'\0')==b'HISTEXIT':
                    at=struct.unpack_from('<H',entry,2)[0];n=struct.unpack_from('<H',entry,24)[0]
                    check(n==10,'actual VMU directory records all ten saved blocks');saved=bytearray();seen=set()
                    for _ in range(n):
                        check(at<200 and at not in seen,'independent VMU FAT chain remains bounded and acyclic')
                        seen.add(at);saved+=image[at*512:(at+1)*512];at=fat[at]
                    check(at==0xfffa and bytes(saved)==bytes((i*37+11)&255 for i in range(5120)),'complete VMU payload and FAT committed before history retirement');found=True
        check(found,'original card save is independently present in actual VMU image')
    if a.case=='partial_start':check(sha(path)==old_hash,'partial rejected original history profile remains byte-identical after terminal cleanup')
    else:
        _,packets=decode(path.read_bytes());title=b''.join(packets[24+i] for i in range(4))
        check(struct.unpack_from('<H',title,180)[0]==(2 if a.case=='pending_card' else 1) and struct.unpack_from('<H',title,174)[0]==1 and title[196:228]==bytes(range(32)),'independent persisted UF record includes all pre-close card/application work')
        check(sum(struct.unpack_from('<24H',title,126))==(66 if a.case=='after_save_elapsed' else 65),'final history flush preserves elapsed play time even after prior explicit save')
    check(bind(flash,'jp_flash_open',I,C.c_wchar_p,U)(str(profile),0)==0 and bind(flash,'jp_flash_close',I)()==0,'actual profile lock is released and can be independently reacquired')
    check(bind(f,'jp_vmu_store_status',I)()==0 and bind(f,'jp_lcd_shutdown',I)()==0 and bind(f,'jp_vmu_store_shutdown',I)()==0,'caller still owns borrowed VMU/LCD and explicitly closes them')
    if not final:
        check(bind(f,'jp_queue_reset_frame',I)()==0 and bind(f,'jp_frames_shutdown',I)()==0,'caller retires borrowed queue/frame owners after early cleanup')
        bind(f,'jp_render_state_shutdown',None)();bind(f,'jp_textures_shutdown',None)();bind(f,'jp_queue_shutdown',None)()
    faulthandler.disable();fault.close();out=dict(case=a.case,checks=len(labels),assertions=labels,observations=obs,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10)
    (d/'result.json').write_text(json.dumps(out,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
if __name__=='__main__':main()
