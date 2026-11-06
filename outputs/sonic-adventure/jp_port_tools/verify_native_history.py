"""Original UF/game history, real core providers and independent persisted records."""
import argparse,ctypes as C,datetime as D,faulthandler,hashlib,json,struct,subprocess,sys,threading
from pathlib import Path
from verify_native_audio import bind
from verify_native_system import Options as SystemOptions
from verify_native_device_init import Options as DeviceOptions
from verify_native_input_backend import Source,Sample
from verify_native_rtc import Reader,Info as RTCInfo
from verify_native_flash_manager import decode,crc,Info as FlashInfo,FMInfo
I,U,Q,P,S=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p,C.c_size_t
CASES=('normal','restart','denied_save','denied_shutdown','rtc_failure','reentrant','foreign','existing_corrupt','bad_identity','safe_refresh','denied_refresh','read_flush','denied_read_flush','wall_clock')
class Options(C.Structure):
    _fields_=[('bytes',U),('create',U),('directory',P),('track',P),('sector',U),('reserved',U)]
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('starts','saves','closes','flash_token','work3','work4')]+[(n,U) for n in ('phase','status','native_error','creator_thread','resources','dirty')]
class Lease(C.Structure):
    _fields_=[('next_token',Q),('token',Q)]+[(n,U) for n in ('active','created','transaction','reserved')]
class Identity(C.Structure):_fields_=[('no',I),('all',I),('product',C.c_char*16)]
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
        for n in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for k in ('workspace','build','track'):cmd+=['--'+k,str(getattr(a,k).resolve())]
            cmd+=['--case',n,'--output',str(d/n)];run=subprocess.run(cmd,capture_output=True,timeout=180,creationflags=subprocess.CREATE_NO_WINDOW)
            (d/(n+'.log')).write_bytes(run.stdout+run.stderr);assert not run.returncode,(n,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-5000:])
            cases[n]=read(d/n/'result.json');print(n,cases[n]['checks'],'checks',flush=True)
        out=dict(status='original_UF_and_game_history_native_transaction_profile_owned_lifecycle_verified',
            checks=sum(x['checks'] for x in cases.values()),cases=cases,build_verification_sha256=sha(a.build/'build_verification.json'),
            verifier_sha256=sha(__file__),full_game_init_or_global_history_exit_handoff_verified=False,
            original_FM_UF_RTC_peripheral_producers_replaced_by_test=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['checks'],'checks',flush=True);return
    fault=(d/'native_fault.txt').open('w');faulthandler.enable(file=fault,all_threads=True)
    frame=C.CDLL(b['frames_DLL']);system=C.CDLL(b['System_DLL']);rtc=C.CDLL(b['RTC_DLL']);flash=C.CDLL(b['flash_DLL']);hist=C.CDLL(b['history_DLL']);labels=[];obs={};keep=[]
    def check(ok,label):
        assert ok,(a.case,label);labels.append(label);(d/'progress.json').write_text(json.dumps(dict(case=a.case,checks=len(labels),assertions=labels),indent=2)+'\n')
    def snapshot(lib,name,kind):
        out=kind();check(bind(lib,name,I,P)(C.byref(out))==0,'actual '+name);return out
    def info():return snapshot(hist,'jp_history_snapshot',Info)
    def lease():return snapshot(flash,'jp_flash_history_snapshot',Lease)
    init=bind(hist,'jp_history_initialize',I);save=bind(hist,'jp_history_save',I);close=bind(hist,'jp_history_shutdown',I)
    configure=bind(hist,'jp_history_configure',I,P);recover=bind(hist,'jp_history_recover',I)
    read_hist=bind(hist,'jp_history_read',I,P,P);play=bind(hist,'peer_history_play',I,U,U,U,P)
    before=D.datetime(2026,11,6,12,0);epoch=D.datetime(1950,1,1);count=int((before-epoch).total_seconds());clock_value=[count];bad_clock=[False];reenter=[False];events=[]
    @Source
    def input_sample(ctx,out):C.memset(out,0,4*C.sizeof(Sample));out[0]=Sample(1,0,0,0,0,0,0,0);return 1
    @Reader
    def clock(ctx,out):
        if reenter[0]:
            ls=Lease();bind(flash,'jp_flash_history_snapshot',I,P)(C.byref(ls))
            old=FMInfo();bind(flash,'jp_flash_fm_snapshot',I,P)(C.byref(old))
            nested=(save(),close(),bind(flash,'jp_flash_close',I)(),bind(flash,'fmMngFormat',I,I)(0))
            bind(flash,'fmMngFinish',None)();new=FMInfo();bind(flash,'jp_flash_fm_snapshot',I,P)(C.byref(new))
            events.append((nested,ls.active,ls.transaction,bytes(old)==bytes(new)))
        if bad_clock[0]:return 1
        out[0]=clock_value[0];return 0
    keep.extend([input_sample,clock])
    for name,args in [('jp_queue_initialize',(8<<20,)),('jp_frames_initialize',(8,128,32<<20)),('jp_textures_initialize',(32,1024,32<<20)),('jp_render_state_initialize',(128,1<<20))]:
        types=[S] if len(args)==1 else [U,U,S] if len(args)==3 else [U,S]
        check(bind(frame,name,I,*types)(*args)==0,'borrowed real render prerequisite '+name)
    vmu=d/'vmu';vmu.mkdir()
    check(bind(frame,'jp_vmu_store_initialize',I,C.c_wchar_p)(str(vmu))==0,'real borrowed VMU owner')
    check(bind(frame,'jp_vmu_store_create',I,U)(0)==0 and bind(frame,'jp_vmu_store_open',I,U)(0)==0 and bind(frame,'jp_lcd_initialize',I,U)(1)==0,'actual VMU storage LCD and timer attachment')
    check(bind(frame,'jp_input_use_virtual_cards',I)()==0,'input queries genuine virtual-card metadata')
    profile=d/'profile';profile.mkdir();path=profile/'SONIC_FLASH.DAT'
    if a.case=='existing_corrupt':path.write_bytes(b'\xff'*131072);original_hash=sha(path)
    track=C.create_unicode_buffer(str(a.track.resolve()));directory=C.create_unicode_buffer(str(profile));keep.extend([track,directory])
    config=Options(32,1,C.addressof(directory),C.addressof(track),2352,0)
    check(C.sizeof(Options)==32 and C.sizeof(Info)==72 and configure(C.byref(config))==0,'native history ABI and copied configuration')
    check(init()==6 and info().resources==0 and not path.exists() if a.case!='existing_corrupt' else init()==6,'missing real RTC prevents premature history startup')
    options=SystemOptions(96,0,DeviceOptions(640,480,0,0,5000,0,64<<20),C.addressof(track),2352,3,5000,0,C.cast(input_sample,P).value,None,None if a.case=='wall_clock' else C.cast(clock,P).value,None)
    check(bind(system,'jp_system_configure',I,P)(C.byref(options))==0 and bind(system,'jp_system_initialize',I,I,I,I)(0,0,1)==0,'actual System core owns real RTC input GDFS wait and device')
    if a.case=='bad_identity':
        bad=d/'bad_track.bin';bad.write_bytes(b'\0'*2352);bad_name=C.create_unicode_buffer(str(bad));keep.append(bad_name)
        wrong=Options(32,1,C.addressof(directory),C.addressof(bad_name),2352,0)
        check(configure(C.byref(wrong))==0 and init()==8 and not path.exists(),'invalid actual disc header fails before profile creation')
        check(configure(C.byref(config))==0,'caller corrects copied disc path')
    rc=init();obs['initialize_result']=rc
    if a.case=='existing_corrupt':
        check(rc==10 and sha(path)==original_hash,'existing unformatted profile never autoformats or reports successful history')
        check(close()==0 and info().resources==0,'failed history initialization releases actual cache lease profile and work')
    else:
        check(rc==0,('complete original history initialization',rc,dict((n,getattr(info(),n)) for n,_ in Info._fields_)))
        state=info();token=state.flash_token
        check(state.phase==3 and state.resources==3 and state.work3>0xffffffff and state.work4>0xffffffff,'ready history owns actual high-address refresh work and profile')
        current=lease();check(current.token==token and not current.active and not current.transaction,'flash authority is idle between individual original FM calls')
        identifier=Identity();check(bind(hist,'syBtGetBootSystemID',I,P)(C.byref(identifier))==1 and identifier.no==identifier.all==1,'original boot identity parser reads actual native disc-header copy')
        with a.track.open('rb') as stream:stream.seek(16);header=stream.read(256)
        check(bytes(identifier.product)[:10]==header[64:74],'actual Japanese product identifier is preserved')
        used,packets=decode(path.read_bytes());title=b''.join(packets[24+i] for i in range(4))
        check(len(title)==240 and title[2:12]==header[64:74] and crc(title[2:112])==struct.unpack_from('<H',title,112)[0],'independent original title-record extent product and CRC')
        flags=packets[0];parity=sum((int(v).bit_count()%2==0)<<(7-i) for i,v in enumerate(flags[:8]))
        check(flags[:4]==bytes([1,1,0,1]) and flags[59]==parity,'original shared flags and independent parity are persisted')
        if a.case=='wall_clock':count=snapshot(rtc,'jp_rtc_snapshot',RTCInfo).count
        check(abs(struct.unpack_from('<I',title,108)[0]-count)<=2 and struct.unpack_from('<H',title,124)[0]==1,'fresh native profile records real clock and initial launch count')
        per=bind(frame,'pdGetPeripheral',P,U)(0);controller=U.from_address(per).value
        expansion=bind(frame,'pdGetPeripheralInfo',P,U)(1);exp_type=U.from_address(expansion).value
        check(struct.unpack_from('<I',packets[20])[0]==controller and struct.unpack_from('<I',packets[22])[0]==exp_type,'original UF records actual controller and VMU device identities')
        check(title[114:120]==bytes.fromhex('7fffff7fffff'),'original peripheral masks preserve connected and absent sockets')
        wide=(C.c_ubyte*92)();C.memset(wide,0xa5,92);out=C.addressof(wide)+16
        check(out>0xffffffff and bind(hist,'peer_history_user_state',I,P)(out)==0 and bytes(wide)[16:76]==flags and bytes(wide)[:16]==bytes(wide)[76:]==b'\xa5'*16,'widened real ufGetUserState copies exactly sixty bytes at native address')
        old=bytes(snapshot(flash,'jp_flash_fm_snapshot',FMInfo))
        check(bind(flash,'jp_flash_close',I)()==7 and bind(flash,'fmMngFormat',I,I)(0)==-11,'independent legacy profile destruction is rejected')
        bind(flash,'fmMngFinish',None)();check(bytes(snapshot(flash,'jp_flash_fm_snapshot',FMInfo))==old,'legacy finish cannot detach original live history work')
        counts=(C.c_ubyte*30).in_dll(hist,'history_counts')
        for character in range(6):
            check(bind(hist,'jp_history_count_play',I,I)(character)==0 and bind(hist,'jp_history_count_clear',I,I)(character)==0,'actual game character play and clear counters')
            check(bytes(counts)[character*5:character*5+5]==b'\0\1\0\1\0','original big-endian five-byte character record')
        counts[0]=counts[1]=255;check(bind(hist,'jp_history_count_play',I,I)(0)==0 and counts[0]==counts[1]==0,'original sixteen-bit count wrap remains explicit')
        prior=bytes(counts);check(bind(hist,'jp_history_count_play',I,I)(-1)==0 and bind(hist,'jp_history_count_clear',I,I)(6)==0 and bytes(counts)==prior,'invalid character leaves original count table unchanged')
        payload=C.create_string_buffer(bytes(range(32)));keep.append(payload)
        check(play(0,4,5,payload)==0 and play(1,6,7,None)==0,'actual original UF records a save load and application payload')
        clock_value[0]+=3900
        bind(rtc,'syRtcExecServer',None)()
        check(bind(rtc,'jp_rtc_status',I)()==0,'actual RTC server accepts advanced clock input before history consumes it')
        if a.case in ('reentrant','rtc_failure'):bind(rtc,'syRtcSetServerMode',None,I)(1)
        if a.case=='foreign':
            previous=bytes(info());results=[];out=Info();C.memset(C.byref(out),0xa5,C.sizeof(out));before_out=bytes(out)
            t=threading.Thread(target=lambda:results.extend((save(),close(),bind(hist,'jp_history_count_play',I,I)(0),bind(hist,'jp_history_snapshot',I,P)(C.byref(out)))))
            t.start();t.join();check(results==[3]*4 and bytes(out)==before_out and bytes(info())==previous,'foreign calls preserve actual history ownership counters and output')
        if a.case=='reentrant':reenter[0]=True
        blocker=None
        if a.case in ('denied_save','denied_shutdown','denied_read_flush'):
            old_hash=sha(path);kernel=C.WinDLL('kernel32',use_last_error=True)
            blocker=bind(kernel,'CreateFileW',P,C.c_wchar_p,U,U,P,U,U,P)(str(path),0x80000000,1,None,3,0x80,None)
            check(blocker and blocker!=P(-1).value,'actual Windows sharing denial blocks transaction commit')
            before=info();read_times=(C.c_uint16*24)();read_starts=(U*3)();C.memset(read_times,0xa5,48);C.memset(read_starts,0x5a,12)
            code=close() if a.case=='denied_shutdown' else read_hist(read_times,read_starts) if a.case=='denied_read_flush' else save()
            after=info();check(code==5 and sha(path)==old_hash,'real failed complete history transaction preserves entire previous image')
            check(after.resources==before.resources and after.flash_token==before.flash_token and after.work3==before.work3 and after.work4==before.work4 and after.dirty,'failed save retains actual UF profile token and native work for retry')
            check(not lease().transaction and bind(flash,'jp_flash_close',I)()==7,'aborted staging retains exact history authority')
            if a.case=='denied_read_flush':check(bytes(read_times)==b'\xa5'*48 and bytes(read_starts)==b'Z'*12,'failed pre-read checkpoint preserves both caller output arrays')
            check(bind(kernel,'CloseHandle',I,P)(blocker)!=0,'caller releases actual denial handle')
            check(recover()==0,'actual recovery reloads restored flash cache while preserving live UF record')
        if a.case=='rtc_failure':
            old_hash=sha(path);bad_clock[0]=True
            check(save()==6 and sha(path)==old_hash and info().dirty,'real clock failure aborts transaction and retains unsaved history')
            bad_clock[0]=False;check(recover()==0,'corrected real clock provider permits owned recovery')
        if a.case in ('read_flush','denied_read_flush'):
            read_times=(C.c_uint16*24)();read_starts=(U*3)()
            check(read_hist(read_times,read_starts)==0 and sum(read_times)==65,'original read checkpoints live history before reloading original UF state')
        else:check((close() if a.case=='denied_shutdown' else save())==0,'genuine original history write commits successfully')
        if reenter[0]:
            check(events and all(row==((4,4,7,-11),0,1,True) for row in events),'real RTC callbacks cannot reenter history or steal flash authority')
            reenter[0]=False
        used,packets=decode(path.read_bytes());title=b''.join(packets[24+i] for i in range(4))
        check(struct.unpack_from('<H',title,180)[0]==1 and struct.unpack_from('<H',title,174)[0]==1 and title[196:228]==bytes(range(32)),'independent persisted record retains original save/load counts and full application payload')
        times=struct.unpack_from('<24H',title,126)
        if a.case=='wall_clock':check(sum(times)<=1,'actual Windows local-clock path records plausible elapsed play time')
        else:check(times[12:14]==(60,5) and sum(times)==65,'original history accounts elapsed minutes across the actual RTC hour boundary')
        if a.case in ('safe_refresh','denied_refresh'):
            original_packets=packets;before_hash=sha(path);refresh=bind(hist,'jp_history_refresh',I)
            blocker=None
            if a.case=='denied_refresh':
                kernel=C.WinDLL('kernel32',use_last_error=True)
                blocker=bind(kernel,'CreateFileW',P,C.c_wchar_p,U,U,P,U,U,P)(str(path),0x80000000,1,None,3,0x80,None)
                check(blocker and blocker!=P(-1).value,'real handle denies whole safe-refresh transaction commit')
                check(refresh()==5 and sha(path)==before_hash and info().resources==3,'failed original safe refresh rolls back complete media and retains all work')
                check(bind(kernel,'CloseHandle',I,P)(blocker)!=0 and recover()==0,'corrected safe-refresh commit can recover actual original cache')
            check(refresh()==0 and bind(hist,'peer_history_library_error',I)()==13,'complete original safe-refresh succeeds despite its documented SDK errno quirk')
            _,after_packets=decode(path.read_bytes())
            check(all(after_packets[n]==v for n,v in original_packets.items()),'independent decoder verifies every retained history record after complete spare/history refresh')
            _,spare=decode(path.read_bytes(),4)
            check(spare[1020]==b'SEGA_FH01_OK\0'.ljust(60,b'\xff'),'actual original spare-partition status records finished refresh')
        if a.case!='denied_shutdown':
            out_times=(C.c_uint16*24)();out_starts=(U*3)()
            check(read_hist(out_times,out_starts)==0 and list(out_times)==list(times) and out_starts[0]==count,'actual guarded original game history read returns persisted values')
            check(close()==0 and info().resources==0,'history closes before borrowed System RTC/input owners')
        if a.case=='restart':
            check(configure(C.byref(config))==0 and init()==0 and info().flash_token>token,'same process reopens persisted original history with nonrecycled authority')
            check(close()==0,'restarted history releases actual profile and work')
        check(bind(flash,'jp_flash_open',I,C.c_wchar_p,U)(str(profile),0)==0 and bind(flash,'jp_flash_close',I)()==0,'real native profile lock can be independently reacquired')
    check(bind(system,'jp_system_shutdown',I)()==0,'borrowed real System core remains usable for its own cleanup')
    check(bind(frame,'jp_lcd_shutdown',I)()==0 and bind(frame,'jp_vmu_store_shutdown',I)()==0,'caller closes actual borrowed VMU LCD and media')
    check(bind(frame,'jp_frames_shutdown',I)()==0,'caller closes actual captured-frame owner')
    bind(frame,'jp_render_state_shutdown',None)();bind(frame,'jp_textures_shutdown',None)();bind(frame,'jp_queue_shutdown',None)()
    faulthandler.disable();fault.close();out=dict(case=a.case,checks=len(labels),assertions=labels,observations=obs,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10)
    (d/'result.json').write_text(json.dumps(out,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
if __name__=='__main__':main()
