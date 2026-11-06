"""Exercise the actual motion owner through full game_init/System/Exit."""
import argparse,ctypes as C,faulthandler,hashlib,json,os,struct,subprocess,sys,threading
from pathlib import Path
from verify_native_audio import bind
from verify_native_motion_store import Info as MotionInfo,View
from verify_native_system import Info as SystemInfo
from verify_native_exit_logo import ExitInfo
from verify_native_history import Info as HistoryInfo
from verify_native_game_sound import Info as SoundInfo
from verify_native_read_queue import Info as QueueInfo
from verify_native_gdfs import Info as FSInfo
from verify_native_flash_manager import decode
I,U,Q,P,S=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p,C.c_size_t
MOTION=16777216;OWNED=3915778|4194304|8388608;FINAL=OWNED|64|128|512|1024
CASES=('restart','early','final','budget','adopt','outer_guard','foreign','late_claim','history_retry','history_global','card_callback','constructor','process_success','process_busy')
class Claim(C.Structure):
    _fields_=[('next_token',Q),('token',Q)]+[(n,U) for n in ('terminal','closing','finished','reserved')]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def exercise(c,case):
    d,b=c['d'],c['b'];f,system,boot,game,ex,sound,hist,queue,fs=(c[n] for n in ('f','system','boot','game','ex','sound','hist','queue','fs'))
    check,snapshot,si,info,shutdown,clean,keep=(c[n] for n in ('check','snapshot','si','info','shutdown','clean','keep'))
    motion=C.CDLL(b['motion_DLL']);start=bind(system,'jp_system_start_motions',I,S,U);adopt=bind(system,'jp_system_adopt_motions',I)
    guard=bind(system,'jp_system_main_initialization_guard',I,U);delegate=bind(system,'jp_system_bind_exit',I,U)
    choose=bind(motion,'jp_motions_select',I,I);acquire=bind(motion,'jp_motions_acquire',I,U,C.c_char_p,P);release=bind(motion,'jp_motions_release',I,Q)
    initialize_motion=bind(motion,'jp_motions_initialize',I,S,U);close_motion=bind(motion,'jp_motions_shutdown',I)
    claim=bind(motion,'jp_motion_exit_claim',I,P);cancel=bind(motion,'jp_motion_exit_cancel',I,Q);finish=bind(motion,'jp_motion_exit_finish',I,Q)
    can_close=bind(motion,'jp_motion_exit_can_close',I,Q);can_retire=bind(motion,'jp_motion_exit_can_retire',I,Q)
    local=bind(system,'jp_system_shutdown',I);boot_close=bind(boot,'jp_main_boot_shutdown',I,U)
    mi=lambda:snapshot(motion,'jp_motions_snapshot',MotionInfo)
    life=lambda:snapshot(motion,'jp_motion_exit_snapshot',Claim)
    ei=lambda:snapshot(ex,'jp_exit_snapshot',ExitInfo)
    check(C.sizeof(Claim)==32,'exact native capability ABI')
    if case=='constructor':check(c['events'] and all(e==(4,4,0,0) for e in c['events']),'actual input callbacks during original constructor cannot start or adopt motion data')
    if case=='budget':
        before=si();check(start(1,8)==12,'actual motion budget rejection returns parent cleanup error')
        current=si();check(current.resources==before.resources and current.failed_resource==MOTION and current.native_error==5 and not mi().allocated_bytes and not life().token,'failed allocation acquires no motion authority or resource bit')
        check(guard(1)==0 and guard(0)==0,'failed start releases only its own initialization guard')
    if case=='outer_guard':check(guard(1)==0,'caller acquires existing actual initialization guard')
    if case=='adopt':
        check(initialize_motion(8<<20,8)==0 and choose(0)==0,'independent genuine motion owner initialized')
        view=View();check(acquire(0,b'sonic_updraft_action',C.byref(view))==0,'independent owner has actual live data view')
        before=si();check(start(8<<20,8)==2 and adopt()==2 and si().resources==before.resources,'start and unguarded adoption preserve independent owner')
        raw=Q();check(claim(C.byref(raw))==0 and guard(1)==0,'independent exact authority and outer guard established')
        check(adopt()==2 and si().resources==before.resources and life().token==raw.value,'preclaimed owner rejects before parent resource mutation')
        check(cancel(raw.value)==0 and adopt()==0 and guard(0)==0,'same live owner adopted under genuine existing guard')
    else:
        check(start(8<<20,8)==0 and choose(0)==0,'System starts and adopts same actual motion data owner')
        view=View();check(acquire(0,b'sonic_updraft_action',C.byref(view))==0,'actual action view retained by consumer')
    token=life().token;obs=c['obs'];obs.update(motion_token=token)
    check(token and si().resources==OWNED|MOTION and mi().live_leases==1,'single System resource and exact motion capability track live view')
    old=Claim();C.memset(C.byref(old),0x5a,C.sizeof(old));sentinel=bytes(old);other=Q(0xabcdef)
    check(claim(C.byref(other))==2 and other.value==0xabcdef and cancel(token+1)==2 and can_close(token)==0 and can_retire(token)==7,'duplicate/stale capability rejection preserves outputs while handoff permits views')
    check(finish(token)==7 and not life().terminal and not life().closing and close_motion()==2 and initialize_motion(8<<20,8)==2,'busy delegated close leaves owner usable and cannot be bypassed independently')
    if case=='outer_guard':
        before=ei().cleanup_calls;check(clean()==4 and ei().cleanup_calls==before,'start preserves caller guard and cleanup cannot release it')
        check(guard(0)==0,'caller releases its own retained guard')
    if case=='foreign':
        results=[];before=bytes(mi())
        def worker():results.extend((start(8<<20,8),adopt(),local(),boot_close(5000),shutdown(5000),clean(),finish(token),cancel(token),bind(motion,'jp_motion_exit_snapshot',I,P)(C.byref(old))))
        t=threading.Thread(target=worker);t.start();t.join()
        check(results==[3]*9 and bytes(old)==sentinel and bytes(mi())==before,'foreign thread rejects every cleanup layer without output or owner mutation')
    final=case in ('final','late_claim','process_success')
    if final:
        prior=bytes(life());check(delegate(FINAL)==11 and si().failed_resource==MOTION and bytes(life())==prior,'missing explicit motion bit rejects before transferring authority')
        if case=='late_claim':
            check(bind(f,'jp_queue_reserve',I,U,U)(0,4)==0 and bind(f,'njStartVertexNonTexOpaque',P)(),'real unfinished renderer work prevents later device claim')
            check(delegate(FINAL|MOTION)==11 and bytes(life())==prior and si().resources==OWNED|MOTION,'late real final-claim failure rolls back without losing motion bootstrap')
            check(bind(f,'jp_queue_discard_active',I)()==0 and bind(f,'jp_queue_reset_frame',I)()==0,'caller discards actual conflicting recorder')
        check(delegate(FINAL|MOTION)==0 and si().phase==4 and not si().resources and life().token==token and not life().terminal,'final handoff transfers same capability while consumer view remains live')
    before=si();before_exit=ei().cleanup_calls;old_data=C.string_at(view.data,min(128,view.bytes));profile=c['path'];profile_hash=sha(profile)
    check(local()==12 and boot_close(5000)==8 and shutdown(5000)==11,'all ordinary parent cleanup layers reject actual live view')
    check(clean()==6 and ei().failed_resource==MOTION and ei().native_error==7 and ei().cleanup_calls==before_exit,'global cleanup rejects before cleanup counters or bootstrap mutation')
    after=si();check(after.resources==before.resources and after.phase==before.phase and mi().live_leases==1 and not life().terminal and not life().closing and C.string_at(view.data,min(128,view.bytes))==old_data and sha(profile)==profile_hash,'failed cleanup preserves view bytes card core profile and capability')
    check(snapshot(sound,'jp_sound_snapshot',SoundInfo).resources and snapshot(hist,'jp_history_snapshot',HistoryInfo).resources and snapshot(queue,'jp_read_queue_snapshot',QueueInfo).initialized and snapshot(fs,'jp_gdfs_snapshot',FSInfo).work,'all required real parents survive blocked motion retirement')
    if case=='process_busy':
        obs.update(profile=str(profile),before_process_exit_profile_sha256=profile_hash)
        (d/'progress.json').write_text(json.dumps(dict(case=case,checks=len(c['labels']),assertions=c['labels'],observations=obs),indent=2)+'\n')
        bind(ex,'syBtExit',None)();raise AssertionError('actual process exit returned')
    check(release(view.token)==0,'actual consumer explicitly releases its motion view')
    if case=='process_success':
        obs.update(profile=str(profile),before_process_exit_profile_sha256=profile_hash)
        (d/'progress.json').write_text(json.dumps(dict(case=case,checks=len(c['labels']),assertions=c['labels'],observations=obs),indent=2)+'\n')
        bind(ex,'syBtExit',None)();raise AssertionError('actual process exit returned')
    card_events=[]
    if case=='card_callback':
        from verify_native_vmu_fs import Time
        def settle():
            for _ in range(10000):
                bind(f,'buExecServer',None)()
                if bind(f,'buGetLastError',I,I)(0)!=-1:return
            raise AssertionError('actual card work did not settle')
        settle();date=Time(2026,11,6,12,0,0,5);label=C.create_string_buffer(b'MOTION EXIT QUALIFICATION'.ljust(32,b'\0'),32);keep.extend([date,label])
        check(bind(f,'buFormatDisk',I,I,P,I,P,U)(0,label,0,C.byref(date),0)==0,'actual card format queued');settle()
        original=bind(f,'card_complete',I,I,I,I,U);Callback=C.CFUNCTYPE(I,I,I,I,U)
        @Callback
        def completed(drive,op,status,param):
            current=MotionInfo();bind(motion,'jp_motions_snapshot',I,P)(C.byref(current));attempt=View()
            card_events.append((current.phase,current.allocated_bytes,acquire(0,b'sonic_updraft_action',C.byref(attempt)),local(),clean(),boot_close(5000),bind(hist,'peer_history_play',I,U,U,U,P)(0,0,0,None),status))
            return original(drive,op,status,param)
        data=C.create_string_buffer(bytes((i*37+11)&255 for i in range(5120)));keep.extend([completed,data]);bind(f,'buSetCompleteCallback',None,Callback)(completed)
        check(bind(f,'buSaveFile',I,I,C.c_char_p,P,U,P,U)(0,b'MOTIONEXIT',data,10,C.byref(date),0)==0 and bind(f,'buGetLastError',I,I)(0)==-1,'actual save callback pending at motion-owned MainBoot shutdown')
    if case.startswith('history_'):
        kernel=C.WinDLL('kernel32',use_last_error=True);blocker=bind(kernel,'CreateFileW',P,C.c_wchar_p,U,U,P,U,U,P)(str(profile),0x80000000,1,None,3,0x80,None)
        check(blocker and blocker!=P(-1).value,'real Windows sharing condition blocks later history commit')
        check(shutdown(5000)==11 and sha(profile)==profile_hash,'actual later History failure retains full parent responsibility and original profile')
        state=life();check(state.token==token and state.finished and state.closing and not state.terminal and mi().phase==2 and not mi().allocated_bytes,'motion closes exactly once but ordinary capability remains retained until all parents succeed')
        check(start(8<<20,8)==2 and initialize_motion(8<<20,8)==2 and snapshot(hist,'jp_history_snapshot',HistoryInfo).resources and snapshot(sound,'jp_sound_snapshot',SoundInfo).resources and snapshot(fs,'jp_gdfs_snapshot',FSInfo).work,'partial cleanup cannot restart motion while needed later parents survive')
        check(bind(kernel,'CloseHandle',I,P)(blocker)!=0 and bind(hist,'jp_history_recover',I)()==0,'actual external condition repaired and retained history recovered')
    terminal=case in ('early','final','late_claim','history_global')
    if terminal:check(clean()==0,'actual global cleanup retires motion first and complete remaining native graph')
    check(shutdown(5000)==0 and not info().parent_owned and not info().scope_held,'actual full game owner synchronizes completed parent cleanup')
    check(mi().phase==2 and not mi().allocated_bytes and not mi().live_leases,'real motion allocations and every consumer view retired')
    if terminal:
        state=life();check(state.token==token and state.terminal and state.finished and clean()==0 and cancel(token)==2 and initialize_motion(8<<20,8)==2,'global finalization is idempotent and terminal authority cannot be revoked or restarted')
    else:check(not life().token and not life().terminal,'complete ordinary cleanup cancels capability for genuine restart')
    if case=='restart':
        check(bind(f,'jp_queue_reset_frame',I)()==0,'borrowed frame queue reset for actual restart');c['configure']()
        check(c['initialize']()==0 and start(8<<20,8)==0 and choose(0)==0 and life().token>token,'same complete original initializer starts fresh motion lifetime')
        new=View();check(acquire(0,b'sonic_updraft_action',C.byref(new))==0 and new.token>view.token and release(view.token)==2,'stale consumer handle cannot release restarted actual data view')
        check(release(new.token)==0 and shutdown(5000)==0,'restarted genuine graph and motion owner close cleanly')
    if case=='card_callback':check(card_events and all(e==(2,0,2,4,4,4,0,0) for e in card_events),'motion retires before actual card callbacks; new views and nested parent cleanup reject while History remains usable')
    check(not snapshot(sound,'jp_sound_snapshot',SoundInfo).resources and not snapshot(hist,'jp_history_snapshot',HistoryInfo).resources and not snapshot(queue,'jp_read_queue_snapshot',QueueInfo).initialized and not snapshot(fs,'jp_gdfs_snapshot',FSInfo).work,'actual audio History queue and file-system resources fully retired')
    check(bind(f,'jp_lcd_shutdown',I)()==0 and bind(f,'jp_vmu_store_shutdown',I)()==0,'caller closes real borrowed LCD and VMU owners')
    if not final:
        check(bind(f,'jp_queue_reset_frame',I)()==0 and bind(f,'jp_frames_shutdown',I)()==0,'caller closes borrowed frame owner');bind(f,'jp_render_state_shutdown',None)();bind(f,'jp_textures_shutdown',None)();bind(f,'jp_queue_shutdown',None)()
    faulthandler.disable();c['fault'].close();out=dict(case=case,checks=len(c['labels']),assertions=c['labels'],observations=obs,build_verification_sha256=sha(c['a'].build/'build_verification.json'),verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10)
    (d/'result.json').write_text(json.dumps(out,indent=2)+'\n');print(case,len(c['labels']),'checks',flush=True)

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure';b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['motion_DLL'])==b['motion_DLL_sha256']
    if a.case:
        original=p/'jp_port_tools/verify_native_game_init.py';raw=original.read_bytes();marker=b"        if a.case=='normal':\n";assert raw.count(marker)==1
        prefix=raw.split(marker)[0]
        injection=b'''            from verify_native_motion_store import Info as MotionInfo
            probe=MotionInfo();motion=C.CDLL(b['motion_DLL'])
            bind(motion,'jp_motions_snapshot',I,P)(C.byref(probe))
            events.append((bind(system,'jp_system_start_motions',I,S,U)(8<<20,8),bind(system,'jp_system_adopt_motions',I)(),probe.allocated_bytes,probe.live_leases))
'''
        if a.case=='constructor':
            anchor=b"        if current.stage==3 and current.scope_held:\n";assert prefix.count(anchor)==1;prefix=prefix.replace(anchor,anchor+injection)
        caller=d/'private_actual_parent_caller.py';caller.write_bytes(prefix+("        from verify_native_motion_exit import exercise\n        exercise(locals(), "+repr(a.case)+")\nif __name__=='__main__':main()\n").encode())
        run=subprocess.run([sys.executable,'-B',str(caller),'--workspace',str(r),'--build',str(a.build.resolve()),'--track',str(a.track.resolve()),'--output',str(d/'run'),'--case','normal'],capture_output=True,env=dict(os.environ,PYTHONPATH=str(p/'jp_port_tools')),timeout=180,creationflags=subprocess.CREATE_NO_WINDOW)
        (d/'stdout.txt').write_bytes(run.stdout);(d/'stderr.txt').write_bytes(run.stderr)
        expected=70 if a.case=='process_busy' else 0;assert run.returncode==expected,(a.case,run.returncode,run.stderr.decode(errors='replace')[-6000:])
        v=read(d/'run'/('progress.json' if a.case.startswith('process_') else 'result.json'));v.update(actual_exit_code=run.returncode,derived_caller_sha256=sha(caller),original_parent_verifier_sha256=sha(original))
        if a.case.startswith('process_'):
            profile=Path(v['observations']['profile'])
            if a.case=='process_busy':assert sha(profile)==v['observations']['before_process_exit_profile_sha256']
            else:
                _,packets=decode(profile.read_bytes());title=b''.join(packets[24+i] for i in range(4));assert struct.unpack_from('<H',title,124)[0]>=1
            v['independent_process_profile_verified']=True
        (d/'result.json').write_text(json.dumps(v,indent=2)+'\n');print(a.case,v['checks'],'checks;',run.returncode,'exit',flush=True);return
    cases={}
    for name in CASES:
        cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
        for n in ('workspace','build','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
        cmd+=['--output',str(d/name),'--case',name];run=subprocess.run(cmd,capture_output=True,timeout=240,creationflags=subprocess.CREATE_NO_WINDOW)
        (d/(name+'.log')).write_bytes(run.stdout+run.stderr);assert run.returncode==0,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-6500:])
        cases[name]=read(d/name/'result.json');print(name,cases[name]['checks'],'checks',flush=True)
    out=dict(status='same_actual_motion_owner_System_MainBoot_bootstrap_final_Exit_and_consumer_retirement_verified',checks=sum(x['checks'] for x in cases.values()),cases=cases,
        build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),native_functions_replaced=False,actual_process_exit_codes=[0,70],
        automatic_task_or_renderer_view_acquisition_verified=False,full_set_character_animation_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['checks'],'checks',flush=True)
if __name__=='__main__':main()
