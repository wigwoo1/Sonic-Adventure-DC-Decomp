"""Independent rational schedule, original frame/hold traces and live Win32 wait."""
import argparse,ctypes as C,hashlib,json,os,random,threading,time
from pathlib import Path
from native_packet_test_support import bind
from native_clock_test_support import Clock,Counter
from native_texture_test_support import Runtime
from native_gpu_test_support import GPU,Failure
U=C.c_uint32;I=C.c_int;Q=C.c_uint64;P=C.c_void_p
Idle=C.CFUNCTYPE(I,P,U);Callback=C.CFUNCTYPE(None)
class Snapshot(C.Structure):
    _fields_=[(n,Q) for n in ['ticks','callbacks','idle_calls','last_wall','next_deadline']]+[(n,U) for n in ['rate_numerator','rate_denominator','max_catchup','status']]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();b=json.loads((d/'build_verification.json').read_text());assert not a.report.exists()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert b['exit_code']==b['diagnostic_bytes']==0 and sha(Path(b['DLL']))==b['DLL_sha256']
    runtime=Runtime(Path(b['frames_DLL']));gpu=GPU(Path(b['GPU_DLL']));clock=Clock(Path(b['DLL']));lib=clock.lib
    init=bind(lib,'jp_wait_initialize',I,U,U,U,P,P);shutdown=bind(lib,'jp_wait_shutdown',I);status=bind(lib,'jp_wait_status',I)
    snapshot=bind(lib,'jp_wait_snapshot',I,P);setter=bind(lib,'jp_wait_set_vsync',I,P);pump=bind(lib,'jp_wait_pump',I,P)
    wait=bind(lib,'jp_wait_until_le',I,P,I);equal=bind(lib,'jp_wait_while_equal',I,P,I);window_idle=bind(lib,'jp_wait_window_idle',I,P,U)
    pacing=bind(lib,'frame_pacing_init',None,I,I);frame_wait=bind(lib,'frame_wait',None);step=bind(lib,'frame_wait_step',None,I);hold=bind(lib,'present_hold',I)
    probe=bind(lib,'jp_wait_probe_reset',None,U,U);count=bind(lib,'jp_wait_probe_counter',I,U);setcount=bind(lib,'jp_wait_probe_countdown',None,I)
    events=bind(lib,'jp_wait_probe_events',U);event=bind(lib,'jp_wait_probe_event',U,U);clear=bind(lib,'jp_wait_probe_clear_events',None)
    checks=callbacks=0;groups={};rng=random.Random(0x57414954);counter=Counter();keep=[];idle_mode=[1];idle_timeouts=[];callback_threads=[];simple=I(1000000)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    @Idle
    def idle(ctx,ms):
        idle_timeouts.append(ms)
        if idle_mode[0]<=0:return idle_mode[0]
        if ms:counter.tick+=(ms*781250+999)//1000
        return 1
    @Callback
    def decrement():
        nonlocal callbacks
        callbacks+=1;simple.value-=1;callback_threads.append(threading.get_ident())
    keep.extend([idle,decrement]);main_thread=threading.get_ident()
    def snap():
        s=Snapshot();check(snapshot(C.byref(s))==0,'snapshot owner');return s
    def configure(num=60000,den=1001,maximum=7,cb=True):
        check(shutdown()==0,'previous wait owner shutdown');counter.tick=0;counter.ok=True;idle_mode[0]=1
        check(counter.start(clock,781250)==0 and init(num,den,maximum,C.cast(idle,P),None)==0,'rational clock and native wait initialize')
        if cb:check(setter(C.cast(decrement,P))==0,'real callback installation')
    check(status()==2 and wait(C.byref(simple),0)==2,'uninitialized wait fails explicitly')
    for num,den in [(60000,1001),(60,1),(240,1),(1,1),(1001,60000)]:
        configure(num,den);base_callbacks=callbacks
        for case in range(256):
            counter.tick+=rng.randrange(40001);expected=counter.tick*num//(781250*den)
            while True:
                delivered=U(0xfeed);check(pump(C.byref(delivered))==0 and delivered.value<=7,'bounded callback pump')
                s=snap();check(s.ticks==callbacks-base_callbacks and s.callbacks==s.ticks,'actual callback count')
                check(s.next_deadline==((s.ticks+1)*781250*den+num-1)//num,'independent exact rational next deadline')
                if s.next_deadline>counter.tick:break
            check(s.ticks==expected and s.last_wall==counter.tick,'no early, duplicate or dropped callbacks')
        check(callbacks-base_callbacks==expected,'rate-specific cumulative oracle')
    check(all(t==main_thread for t in callback_threads),'callbacks execute on calling game thread')
    groups['rational_phase_catchup_no_drift_and_thread_serialization']=checks
    configure();before=snap();counter.tick=781250*3600;processed=U();check(pump(C.byref(processed))==0 and processed.value==7 and snap().next_deadline<counter.tick,'long backlog retained, per-pump work bounded')
    check(setter(C.cast(decrement,P))==0 and snap().next_deadline==counter.tick+(781250*1001+59999)//60000,'pacing reset anchors fresh phase without retroactive callbacks')
    saved=bytes(snap());check(init(0,1,7,C.cast(idle,P),None)==1 and bytes(snap())==saved,'invalid init preserves working schedule')
    configure();simple.value=3;clock.write(0);begin_timer=clock.timer()
    check(wait(C.byref(simple),1)==0 and simple.value==1 and snap().ticks==2,'greater-than wait releases after two actual ticks')
    check(clock.timer()==begin_timer and clock.wall()>0,'paused timer 1 does not stop vertical wall ticks')
    simple.value=2;check(equal(C.byref(simple),2)==0 and simple.value==1 and snap().ticks==3,'held-picture equality wait releases at next tick')
    previous=bytes(snap());check(wait(C.byref(simple),1)==0 and bytes(snap())==previous,'satisfied predicate does no extra work')
    check(idle_timeouts and max(idle_timeouts)<=50,'native idle requests bounded to 50 ms')
    configure(cb=False);simple.value=1;check(wait(C.byref(simple),0)==8 and status()==8,'missing callback cannot spin forever')
    configure();idle_mode[0]=0;simple.value=1;check(wait(C.byref(simple),0)==5 and status()==5,'host close terminates wait')
    configure();idle_mode[0]=-1;check(wait(C.byref(simple),0)==7 and status()==7,'host failure terminates wait')
    configure();counter.ok=False;processed=U(77);check(pump(C.byref(processed))==3 and status()==3 and processed.value==0,'clock read failure explicit')
    configure();foreign=[]
    def other_thread():
        out=U(77);foreign.extend([pump(C.byref(out)),out.value,shutdown()])
    th=threading.Thread(target=other_thread);th.start();th.join();check(foreign==[1,77,1] and status()==0,'foreign thread cannot mutate owner or output')
    reentered=[]
    @Callback
    def recursive():
        out=U(55);reentered.extend([pump(C.byref(out)),out.value,shutdown()])
    keep.append(recursive);check(setter(C.cast(recursive,P))==0,'recursive-test callback');counter.tick+=14000
    check(pump(C.byref(processed))==0 and reentered==[4,55,4] and status()==0,'reentrant calls rejected without corrupting active callback')
    groups['predicates_rebase_timer_independence_and_fault_boundaries']=checks-sum(groups.values())
    def trace():return [event(i) for i in range(events())]
    def original_setup(interval=2,steps=2,stage=0,flags=0):
        configure(cb=False);probe(stage,flags);pacing(interval,steps);check(trace()==[15],'original pacing clears bitmap queue')
        check([count(i) for i in range(4)]==[interval,interval,steps,0],'original frame globals');clear()
    original_setup();frame_wait();check(trace()==[1,2,4,3,1,2,7,1000,9,10,11,12,13],'complete original frame wait callback/render/pad order')
    check([count(i) for i in range(8)]==[2,2,2,0,0,0,0,1],'original next frame and render counters')
    original_setup();step(1);check(trace()==[1,2,12,13,14] and count(0)==1,'original step wait and sound queue order')
    original_setup(flags=1);frame_wait();check(trace()==[1,2,4,3,1,2,7,1000,8,12,13] and count(7)==0 and count(4)==11,'draw-off drops queue without rendering')
    original_setup(flags=8);frame_wait();check(trace()==[1,2,4,3,1,2,7,1000,9,1,2,8,12,13] and count(0)==1,'real held picture waits once then resets queue')
    original_setup();setcount(-3);frame_wait();check(trace()==[7,1003,9,10,11,12,13] and count(0)==2,'original late-frame marker and no gratuitous wait')
    original_setup();counter.tick=(5*781250*1001+59999)//60000;frame_wait()
    check(trace()==[1,2,4,3,1,2,1,2,1,2,1,2,7,1003,9,10,11,12,13] and count(0)==2,'actual overdue ticks synchronize before original late-count snapshot')
    for stage,flags,fog in [(2<<8,0,[5,6]),(2<<8,4,[]),(2<<8,20,[5,6]),(6<<8,0,[5,6]),((7<<8)|1,0,[5,6]),((33<<8)|2,0,[6,5])]:
        original_setup(1,1,stage,flags);counter.tick=14000;check(pump(C.byref(processed))==0 and trace()==[4]+fog+[3,1,2],'actual stage fog/sound/backup/loader ordering')
    original_setup(2,2,0,2);counter.tick=14000;check(pump(C.byref(processed))==0 and trace()==[4,3,1,2],'split-frame callback serves sound on each tick')
    original_setup(0,1);counter.tick=14000;check(pump(C.byref(processed))==0 and trace()==[4,3,1,2] and count(0)==-1,'interval-zero GCC callback behavior')
    original_setup();idle_mode[0]=0;frame_wait();check(status()==5 and trace()==[] and count(0)==2,'closed host aborts original frame before rendering')
    # Hold fails after the outer two ticks; the new guard must stop reset/pads.
    original_setup(flags=8)
    @Idle
    def close_hold(ctx,ms):
        if events() and event(events()-1)==9:return 0
        return idle(ctx,ms)
    keep.append(close_hold);check(init(60000,1001,7,C.cast(close_hold,P),None)==0,'close during hold host');pacing(2,2);clear();frame_wait()
    check(status()==5 and trace()==[1,2,4,3,1,2,7,1000,9],('hold failure prevents later queue reset/render/pad work',status(),trace()))
    groups['complete_original_frame_and_hold_function_contracts']=checks-sum(groups.values())
    # Real QPC, real native window and real OS message wait. No retail inputs.
    check(shutdown()==0 and clock.init(None,None,0)==0,'live QPC clock')
    gl=gpu.lib;wo=bind(gl,'jp_window_open',I,P,U,U,I,P,P);wc=bind(gl,'jp_window_close',None,P);window=P();error=Failure()
    check(wo(gpu.handle,96,64,1,C.byref(window),C.byref(error))==0,'visible hardware window')
    check(init(60000,1001,8,C.cast(window_idle,P),window)==0,'real Windows message-wait adapter');probe(0,0);pacing(2,2);clear();clock.write(0)
    started=clock.wall();step(1);ended=clock.wall();live=snap()
    check(status()==0 and live.callbacks>=1 and count(0)<=1 and ended-started>=13034,'actual QPC-paced original step wait')
    check(clock.timer()==0 and trace()[-3:]==[12,13,14],'live wait preserves paused timer 1 and original post-wait work')
    user=C.WinDLL('user32');post=bind(user,'PostQuitMessage',None,I);peek=bind(user,'PeekMessageW',I,P,P,U,U,U)
    class MSG(C.Structure):_fields_=[('hwnd',P),('message',U),('wParam',C.c_size_t),('lParam',C.c_ssize_t),('time',U),('x',I),('y',I),('private',U)]
    post(77);check(window_idle(window,0)==0,'WM_QUIT stops native wait');message=MSG();check(peek(C.byref(message),None,0x12,0x12,1) and message.wParam==77,'quit message preserved for application')
    find=bind(user,'FindWindowW',P,C.c_wchar_p,C.c_wchar_p);pid=bind(user,'GetWindowThreadProcessId',U,P,P);send=bind(user,'SendMessageW',C.c_ssize_t,P,U,C.c_size_t,C.c_ssize_t)
    hwnd=find('SA_Native_Renderer_Research_Window',None);owner=U();check(hwnd and pid(hwnd,C.byref(owner)) and owner.value==os.getpid(),'this process window identified')
    send(hwnd,0x10,0,0);clear();frame_wait();check(status()==5 and trace()==[],'actual WM_CLOSE aborts next original frame wait')
    wc(window);gpu.close(gpu.handle);check(shutdown()==0,'all native wait/window owners clean')
    groups['live_QPC_Win32_original_wait_and_close']=checks-sum(groups.values())
    result=dict(status='native_game_thread_waits_and_original_frame_contracts_verified',checks=checks,groups=groups,rational_rates=[[60000,1001],[60,1],[240,1],[1,1],[1001,60000]],
        complete_original_frame_source_executed=True,complete_adapted_present_hold_executed=True,diagnostic_services_are_trace_fixtures=True,
        original_GCC_behavior_selected_without_SH4_hardware_headers=True,all_three_busy_waits_adapted=True,stage_fog_sound_backup_loader_order_verified=True,
        rational_deadline_oracle_verified=True,callback_thread_serialization_verified=True,paused_timer1_does_not_pause_ticks=True,
        real_QPC_Win32_wait_verified=True,WM_QUIT_preserved=True,actual_WM_CLOSE_abort_verified=True,terminal_wait_failure_aborts_frame_work=True,
        native_game_render_flip_or_full_loop_executed=False,physical_scanout_vsync_verified=False,playable=False,overall_completion_percent=10,
        build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)))
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
