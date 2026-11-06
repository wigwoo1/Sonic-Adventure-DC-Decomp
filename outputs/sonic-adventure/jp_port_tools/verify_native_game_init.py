"""Run complete original game_init with actual native frames, audio and storage."""
import argparse,ctypes as C,datetime as D,faulthandler,hashlib,json,struct,subprocess,sys,threading,time
from pathlib import Path
from verify_native_audio import bind
from verify_native_system import Options as SystemOptions,Info as SystemInfo
from verify_native_device_init import Options as DeviceOptions
from verify_native_game_sound import Options as SoundOptions,Info as SoundInfo
from verify_native_aica import Options as AicaOptions
from verify_native_sound_clock import Options as ClockOptions
from verify_native_input_backend import Source,Sample
from verify_native_rtc import Reader
from verify_native_history import Options as HistoryOptions,Info as HistoryInfo
from verify_native_read_queue import Info as QueueInfo,Disc
from verify_native_read_queue_exit import Chain
from verify_native_gdfs import Info as FSInfo
from verify_native_adxt import Info as AdxtInfo
from native_present_test_support import Info as PresentInfo
from verify_native_exit_logo import ExitInfo
from verify_native_flash_manager import decode
I,U,Q,P,S=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p,C.c_size_t
PARENT=3915778;OWNED=PARENT|4194304|8388608;FINAL=OWNED|64|128|512|1024
CASES=('normal','cold_precondition','copied_options','restart','callback','foreign','data','real_error','shutdown_denial','history_failure','history_identity','sound_failure','audio_failure','frame_close','early','final','audio_player','process_success','process_failure')
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('starts','completions','shutdowns','preinit_calls','parent_calls','frame_calls','frame_completed','audio_calls','queue_calls','history_calls','loader_error_calls','logical_ticks_before','logical_ticks_after','stack_base','stack_end')]
    _fields_ += [(n,U) for n in ('phase','status','native_error','creator_thread','parent_owned','scope_held','stage')]+[(n,I) for n in ('game_mode','loader_error_flag','last_loader_error')]+[(n,U) for n in ('audio_stages','preinit_mode','callback_bound','system_phase')]
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
            (d/(name+'.log')).write_bytes(run.stdout+run.stderr);expected=73 if name=='process_failure' else 0
            assert run.returncode==expected,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-6000:])
            v=read(d/name/('progress.json' if name.startswith('process_') else 'result.json'));v['actual_exit_code']=run.returncode
            if name.startswith('process_'):
                path=Path(v['observations']['profile'])
                if name=='process_failure':assert sha(path)==v['observations']['before_process_exit_profile_sha256']
                else:
                    _,packets=decode(path.read_bytes());title=b''.join(packets[24+i] for i in range(4));assert len(title)==240 and struct.unpack_from('<H',title,124)[0]>=1
                v['independent_post_process_profile_verified']=True
            cases[name]=v;print(name,v['checks'],'checks;',run.returncode,'exit',flush=True)
        out=dict(status='complete_original_game_init_native_parent_frames_owned_audio_queue_history_and_cleanup_verified',checks=sum(v['checks'] for v in cases.values()),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),full_game_init_executed=True,game_main_loop_or_gameplay_executed=False,
            active_native_interrupt_stack_or_movie_playback_verified=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['checks'],'checks',flush=True);return
    fault=(d/'native_fault.txt').open('w');faulthandler.enable(file=fault,all_threads=True);labels=[];obs={};keep=[];events=[];closed=[False];gated=[False]
    f=C.CDLL(b['frames_DLL']);system=C.CDLL(b['System_DLL']);boot=C.CDLL(b['main_boot_DLL']);game=C.CDLL(b['game_init_DLL']);ex=C.CDLL(b['authored_exit_DLL']);sound=C.CDLL(b['game_sound_DLL']);queue=C.CDLL(b['read_queue_DLL']);hist=C.CDLL(b['history_DLL']);fs=C.CDLL(b['GDFS_DLL']);present=C.CDLL(b['present_DLL'])
    g2=C.CDLL(str(Path(b['game_sound_DLL']).parent/'jp_native_g2_audio.dll'));gate_set=bind(g2,'jp_g2_gate_set',I,I)
    def check(ok,label):
        assert ok,(a.case,label);labels.append(label);(d/'progress.json').write_text(json.dumps(dict(case=a.case,checks=len(labels),assertions=labels),indent=2)+'\n')
    check(C.sizeof(Info)==176 and C.sizeof(PresentInfo)==104,'native game and presentation snapshot ABI widths')
    def snapshot(lib,name,kind):
        v=kind();check(bind(lib,name,I,P)(C.byref(v))==0,'actual '+name);return v
    def info():return snapshot(game,'jp_game_init_snapshot',Info)
    def si():return snapshot(system,'jp_system_snapshot',SystemInfo)
    initialize=bind(game,'jp_game_initialize',I);shutdown=bind(game,'jp_game_init_shutdown',I,U);clean=bind(ex,'jp_exit_cleanup',I)
    stamp=int((D.datetime(2026,11,6,12)-D.datetime(1950,1,1)).total_seconds())
    @Reader
    def rtc(ctx,out):out[0]=stamp;return 0
    @Source
    def source(ctx,out):
        C.memset(out,0,4*C.sizeof(Sample));out[0]=Sample(1,0,0,0,0,0,0,0)
        current=Info();bind(game,'jp_game_init_snapshot',I,P)(C.byref(current))
        if current.stage==3 and current.scope_held:
            if a.case=='callback':
                before=ExitInfo();bind(ex,'jp_exit_snapshot',I,P)(C.byref(before));state=SystemInfo();bind(system,'jp_system_snapshot',I,P)(C.byref(state))
                values=(initialize(),shutdown(5000),bind(system,'jp_system_shutdown',I)(),bind(boot,'jp_main_boot_shutdown',I,U)(5000),
                    bind(system,'jp_system_bind_exit',I,U)(FINAL),clean(),bind(system,'jp_system_main_initialization_guard',I,U)(0),bind(system,'jp_system_start_read_queue',I)())
                after=ExitInfo();bind(ex,'jp_exit_snapshot',I,P)(C.byref(after));new=SystemInfo();bind(system,'jp_system_snapshot',I,P)(C.byref(new))
                token=Q(0xabcdef)
                scope_safe=(bind(system,'jp_system_game_init_claim',I,P)(C.byref(token))==2 and token.value==0xabcdef and
                    bind(system,'jp_system_game_init_release',I,Q)(0xabcdef)==2 and bind(system,'jp_system_game_init_queue',I,Q)(0xabcdef)==2)
                events.append((values,before.cleanup_calls==after.cleanup_calls,bytes(state)==bytes(new),scope_safe))
            if a.case=='frame_close' and not closed[0]:
                state=SystemInfo();bind(system,'jp_system_snapshot',I,P)(C.byref(state));kernel=C.WinDLL('user32',use_last_error=True)
                assert bind(kernel,'PostMessageW',I,P,U,P,P)(state.window,0x10,None,None)!=0;closed[0]=True
            if a.case=='audio_failure' and current.frame_calls==2 and not gated[0]:
                gate_set(15);gated[0]=True
        return 1
    keep.extend([source,rtc])
    for name,args in [('jp_queue_initialize',(8<<20,)),('jp_frames_initialize',(8,128,32<<20)),('jp_textures_initialize',(32,1024,32<<20)),('jp_render_state_initialize',(128,1<<20))]:
        types=[S] if len(args)==1 else [U,U,S] if len(args)==3 else [U,S];check(bind(f,name,I,*types)(*args)==0,'actual borrowed prerequisite '+name)
    buffer=C.addressof((C.c_ubyte*0x40000).in_dll(f,'texture_buffer'));check(bind(f,'jp_render_state_register_buffer',I,P,S,U)(buffer,0x40000,4096)==0,'original shared main buffer registered')
    vmu=d/'vmu';vmu.mkdir();check(bind(f,'jp_vmu_store_initialize',I,C.c_wchar_p)(str(vmu))==0 and bind(f,'jp_vmu_store_create',I,U)(0)==0 and bind(f,'jp_vmu_store_open',I,U)(0)==0 and bind(f,'jp_lcd_initialize',I,U)(1)==0,'actual borrowed VMU media/LCD owner')
    runtime_track=a.track.resolve()
    if a.case=='real_error':
        import msvcrt
        disc=Disc(a.track);files=disc.files();bad_name,(bad_fad,bad_bytes,_)=next((n,v) for n,v in sorted(files.items()) if not v[2] and 64*1024<=v[1]<=512*1024 and len(n)<=31)
        driver_fad,driver_bytes,_=files['MASARU.DRV'];disc.sectors(driver_fad,(driver_bytes+2047)//2048);bad_capacity=(bad_bytes+2047)&~2047;disc.sectors(bad_fad,bad_capacity//2048)
        runtime_track=d/'private_bad_sector_track.bin'
        with runtime_track.open('w+b') as fixture:
            kernel=C.WinDLL('kernel32',use_last_error=True);done=U()
            check(bind(kernel,'DeviceIoControl',I,P,U,P,U,P,U,P,P)(msvcrt.get_osfhandle(fixture.fileno()),0x900c4,None,0,None,0,C.byref(done),None)!=0,'actual sparse raw-track fixture enabled')
            fixture.truncate(a.track.stat().st_size)
            for sector in sorted(disc.reads|set(range(45150,45214))):
                disc.f.seek((sector-45150)*2352);data=disc.f.read(2352)
                if sector==bad_fad+3:data=b'\x01'+data[1:]
                fixture.seek((sector-45150)*2352);fixture.write(data)
        disc.f.close()
    track=C.create_unicode_buffer(str(runtime_track));keep.append(track);profile=d/'profile';profile.mkdir();directory=C.create_unicode_buffer(str(profile));keep.append(directory);path=profile/'SONIC_FLASH.DAT'
    history_track=track
    if a.case=='history_identity':history_track=C.create_unicode_buffer(str(d/'missing_track.bin'));keep.append(history_track)
    if a.case in ('history_failure','process_failure'):path.write_bytes(b'\xff'*131072);old_hash=sha(path)
    options=SystemOptions(96,0,DeviceOptions(640,480,0,0,5000,0,64<<20),C.addressof(track),2352,3,5000,0,C.cast(source,P).value,None,C.cast(rtc,P).value,None)
    driver=C.create_string_buffer(b'ABSENT.DRV');keep.append(driver)
    so=SoundOptions(56,0,2<<20,5000,AicaOptions(32,1,16384,40,5000,1,1,0),C.addressof(driver) if a.case=='sound_failure' else None);co=ClockOptions(56,441,8192,5,None,None,0,None,None)
    ho=HistoryOptions(32,1,C.addressof(directory),C.addressof(history_track),2352,0)
    def configure():
        check(bind(system,'jp_system_configure',I,P)(C.byref(options))==0 and bind(boot,'jp_main_boot_configure',I,P,P)(C.byref(so),C.byref(co))==0 and bind(game,'jp_game_init_configure',I,P)(C.byref(ho))==0,'actual copied System/MainBoot and higher constructor options')
    if a.case=='cold_precondition':
        check(bind(game,'jp_game_init_configure',I,P)(C.byref(ho))==0 and initialize()==2 and not info().starts and not info().parent_owned and not info().preinit_mode and not info().stack_end and not path.exists(),'missing configured real parents rejected before original preinitialization or resource acquisition')
    configure();C.c_int32.in_dll(game,'game_mode').value=99;C.c_int32.in_dll(game,'loader_error_flag').value=1
    stack=(C.c_uint32*2048).in_dll(game,'mwsfdg_intStack');C.memset(stack,0x5a,C.sizeof(stack));before_stack=bytes(stack)
    if a.case=='copied_options':
        directory.value='Z:\\WRONG';track.value='Z:\\ABSENT';check(True,'caller path buffers changed after copy')
    if a.case.startswith('process_'):
        obs.update(profile=str(path),before_process_exit_profile_sha256=sha(path) if path.exists() else None);(d/'progress.json').write_text(json.dumps(dict(case=a.case,checks=len(labels),assertions=labels,observations=obs,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__)),indent=2)+'\n')
        bind(game,'game_init',None)();check(a.case=='process_success' and info().phase==3,'original game_init ABI only returns after successful construction')
        (d/'progress.json').write_text(json.dumps(dict(case=a.case,checks=len(labels),assertions=labels,observations=obs,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__)),indent=2)+'\n')
        bind(ex,'syBtExit',None)();raise AssertionError('process exit returned')
    code=initialize();state=info();obs.update(initialize_result=code,initial_snapshot={n:getattr(state,n) for n,_ in Info._fields_})
    (d/'initial_observation.json').write_text(json.dumps(obs,indent=2)+'\n')
    failure_codes={'history_failure':9,'history_identity':9,'sound_failure':5,'audio_failure':7,'frame_close':6}
    if a.case in failure_codes:
        check(code==failure_codes[a.case] and state.phase==4 and not state.completions,'actual constructor failure is returned before successful completion')
        if a.case!='sound_failure':check(state.scope_held and clean()==4 and bind(system,'jp_system_shutdown',I)()==4,'failed higher construction retains actual scope until explicit owner cleanup')
        check(C.c_int32.in_dll(game,'loader_error_flag').value==1 and not state.callback_bound,'failure does not fall through to clear shared loader flag or bind final callback')
        if a.case in ('history_failure','history_identity'):check(state.frame_completed==2 and state.audio_stages==15 and snapshot(queue,'jp_read_queue_snapshot',QueueInfo).initialized,'late failure retains genuinely initialized frames audio and queued-file owner')
        if a.case=='history_failure':check(sha(path)==old_hash,'unknown existing profile is never reformatted')
        if a.case=='history_identity':check(not path.exists(),'failed real disc identity acquires no profile')
        if a.case=='audio_failure':check(gated[0] and gate_set(0)==0,'caller releases actual G2 gate before cleanup')
    else:
        check(code==0 and state.phase==3 and state.completions==1 and not state.scope_held and si().resources==OWNED,'complete original initializer owns actual full native dependency graph')
        check(state.preinit_calls==state.parent_calls==state.audio_calls==state.queue_calls==state.history_calls==1 and state.frame_calls==state.frame_completed==2,'complete original successful initialization call order executed exactly once')
        check(state.preinit_mode==0x80000000 and state.stack_end==state.stack_base+8192 and state.stack_base>0xffffffff and bytes(stack)==before_stack,'actual original Sofdec preinit selects retained full-width stack end without touching its bytes')
        check(state.game_mode==0 and state.loader_error_flag==0 and state.callback_bound and state.audio_stages==15,'same original main globals and real owned ADXT chain initialized')
        presentation=snapshot(present,'jp_present_snapshot',PresentInfo);obs['presentations']=presentation.presentations
        check(state.logical_ticks_after>state.logical_ticks_before and presentation.presentations==2 and not presentation.status,'two genuine native presentations completed on the actual inherited logical clock phase')
        if a.case=='normal':
            note=bind(game,'loader_note_error',None,P,I);flag=C.c_int32.in_dll(game,'loader_error_flag');note(None,-19)
            check(flag.value==0 and info().last_loader_error==-19,'original shared loader callback ignores ordinary read errors')
            note(None,-23);check(flag.value==1,'original shared loader flag records tray-open error code');flag.value=0
            note(None,-33);check(flag.value==1,'original shared loader flag records disc-change error code')
        if a.case=='callback':check(events and all(e==((4,)*8,True,True,True) for e in events),'actual frame/input callbacks cannot retire card/core or mutate constructor ownership')
        if a.case=='foreign':
            out=Info();C.memset(C.byref(out),0x5a,C.sizeof(out));old=bytes(out);values=[];before=bytes(info())
            def other():values.extend((initialize(),shutdown(5000),bind(game,'jp_game_init_configure',I,P)(C.byref(ho)),bind(game,'jp_game_init_snapshot',I,P)(C.byref(out))))
            t=threading.Thread(target=other);t.start();t.join();check(values==[3]*4 and bytes(out)==old and bytes(info())==before,'foreign caller cannot mutate actual constructor or output')
        if a.case=='audio_player':
            player=P();check(bind(sound,'jp_adxt_create',I,U,U,P)(1,0,C.byref(player))==0 and player.value and snapshot(sound,'jp_adxt_snapshot',AdxtInfo).live_players==1,'actual ADXT/RNA/joint player constructs after complete game_init')
            check(bind(sound,'jp_adxt_destroy',I,P)(player)==0 and not snapshot(sound,'jp_adxt_snapshot',AdxtInfo).live_players,'actual player retires before shared audio parents')
        if a.case in ('data','real_error','shutdown_denial'):
            disc=Disc(a.track);files=disc.files();name,(fad,count,_)=next((n,v) for n,v in sorted(files.items()) if not v[2] and 64*1024<=v[1]<=512*1024 and len(n)<=31)
            capacity=(count+2047)&~2047;expected_bytes=disc.sectors(fad,capacity//2048);disc.f.close();raw=(C.c_ubyte*(capacity+96))();C.memset(raw,0xa5,C.sizeof(raw));at=(C.addressof(raw)+63)&~31;keep.append(raw)
            check(bind(queue,'jp_read_queue_add',I,C.c_char_p,P,S)(name.encode(),at,capacity)==0,'complete game initializer leaves actual checked file queue usable')
            serve=bind(queue,'jp_read_queue_serve',I);check(serve()==0,'real bounded queued read starts after original game_init')
            if a.case in ('data','real_error'):
                deadline=time.monotonic()+15;actual=QueueInfo();code=0
                while True:
                    assert bind(queue,'jp_read_queue_snapshot',I,P)(C.byref(actual))==0
                    if not actual.queued or code:break
                    code=serve();bind(present,'frame_wait',None)();assert time.monotonic()<deadline
                if a.case=='real_error':check(code==6 and info().loader_error_calls>0 and info().last_loader_error==-19 and info().loader_error_flag==0,'real malformed-sector GDFS callback executes original loader error semantics')
                else:
                    check(code==0 and C.string_at(at,capacity)==expected_bytes,'complete initializer feeds real file bytes matching independent raw-sector oracle')
                    data=C.create_string_buffer(bytes(range(32)));keep.append(data);check(bind(hist,'peer_history_play',I,U,U,U,P)(0,0,0,data)==0,'complete initializer leaves actual UF history producer usable')
            else:
                file_state=snapshot(fs,'jp_gdfs_snapshot',FSInfo);record=Chain();check(file_state.pending and bind(present,'jp_vblank_chain_describe',I,U,P)(file_state.vblank,C.byref(record))==0,'actual pending read and original GDFS handler identified')
                bind(present,'syChainDeleteHandler',None,U)(file_state.vblank)
                check(shutdown(5000)==11 and info().parent_owned and info().audio_stages==15 and snapshot(queue,'jp_read_queue_snapshot',QueueInfo).initialized,'real failed game teardown retains queue and all necessary audio/storage parents')
                temporary=[];Callback=C.CFUNCTYPE(None,P)
                @Callback
                def noop(ctx):pass
                keep.append(noop)
                for index in range(1,file_state.vblank):
                    if bind(present,'jp_vblank_chain_describe',I,U,P)(index,C.byref(Chain())):
                        h=bind(present,'syChainAddHandler',U,C.c_int16,P,U,P)(0x900,noop,200,None);assert h==index;temporary.append(h)
                check(bind(present,'syChainAddHandler',U,C.c_int16,P,U,P)(0x900,record.callback,record.priority&0x7fffffff,record.argument)==file_state.vblank,'same actual GDFS handler restored for genuine game teardown retry')
                for h in temporary:bind(present,'syChainDeleteHandler',None,U)(h)
            offset=at-C.addressof(raw);check(bytes(raw[:offset])==b'\xa5'*offset and bytes(raw[offset+capacity:])==b'\xa5'*(len(raw)-offset-capacity),'actual destination guards preserved after initialization and native I/O')
        if a.case in ('early','final'):
            if a.case=='final':check(bind(system,'jp_system_bind_exit',I,U)(FINAL)==0,'final native Exit adopts initialized original game graph')
            check(clean()==0,'actual global cleanup retires complete initialized graph')
    rc=shutdown(5000);obs['shutdown_result']=rc
    check(rc==0 and not info().parent_owned and not info().scope_held,'explicit owner cleanup releases real constructor dependencies and scope')
    check(not snapshot(sound,'jp_sound_snapshot',SoundInfo).resources and not snapshot(hist,'jp_history_snapshot',HistoryInfo).resources and not snapshot(queue,'jp_read_queue_snapshot',QueueInfo).initialized,'actual sound history queue ownership fully retired')
    check(not info().audio_stages and not snapshot(fs,'jp_gdfs_snapshot',FSInfo).work,'owned audio submodules and actual file-system arenas retired')
    if a.case=='data':
        _,packets=decode(path.read_bytes());title=b''.join(packets[24+i] for i in range(4));check(struct.unpack_from('<H',title,180)[0]==1 and title[196:228]==bytes(range(32)),'original UF data from complete game path persisted before shutdown')
    if a.case=='restart':
        check(bind(f,'jp_queue_reset_frame',I)()==0,'borrowed render queue reset for genuine restart');configure();check(initialize()==0 and info().completions==2 and info().frame_completed==4,'same complete original initializer restarts without duplicate ADXT records')
        check(shutdown(5000)==0,'restarted full game initializer retires cleanly')
    check(bind(f,'jp_lcd_shutdown',I)()==0 and bind(f,'jp_vmu_store_shutdown',I)()==0,'caller closes its actual borrowed LCD/VMU owners')
    if a.case!='final':
        check(bind(f,'jp_queue_reset_frame',I)()==0 and bind(f,'jp_frames_shutdown',I)()==0,'caller retires borrowed frame owner');bind(f,'jp_render_state_shutdown',None)();bind(f,'jp_textures_shutdown',None)();bind(f,'jp_queue_shutdown',None)()
    faulthandler.disable();fault.close();out=dict(case=a.case,checks=len(labels),assertions=labels,observations=obs,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),full_game_init_executed=True,playable=False,overall_completion_percent=10)
    (d/'result.json').write_text(json.dumps(out,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
if __name__=='__main__':main()
