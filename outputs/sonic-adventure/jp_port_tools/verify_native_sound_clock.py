"""Actual elapsed-time firmware/PCM and native wait/System lifetime tests.

Clock injection tests arithmetic; original GDFS/SDK/ARM/AICA/Windows PCM stay
real. Separate actual wall-clock waits prove production cadence integration.
"""
import argparse,ctypes as C,hashlib,json,struct,subprocess,sys,threading,time
from pathlib import Path
from verify_native_audio import bind,fnv,Info as AudioInfo
from verify_native_game_sound import Options as SoundOptions,Info as SoundInfo
from verify_native_aica import Options as AicaOptions,Info as AicaInfo
from verify_native_system import Options as SystemOptions,Info as SystemInfo
from verify_native_device_init import Options as DeviceOptions
from verify_native_input_backend import Source,Sample
from verify_native_exit_logo import ExitInfo
from verify_native_gdfs import Info as GDFSInfo
from verify_native_vblank_chain import Record
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p;S=C.c_size_t
ClockSource=C.CFUNCTYPE(I,P,C.POINTER(Q));Callback=C.CFUNCTYPE(I,P)
class Options(C.Structure):
    _fields_=[(n,U) for n in ('bytes','quantum_frames','max_frames_per_tick','period_ms')]+[(n,P) for n in ('source','context')]+[('frequency',Q),('activity',P),('activity_context',P)]
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('anchor','last_tick','frequency','fraction','due_frames','rendered_frames','backlog','service_calls','render_calls','queue_full','wait_token','initial_core_frames','cancelled_frames')]+[(n,U) for n in ('phase','status','native_error','creator_thread','quantum_frames','max_frames_per_tick','period_ms','in_tick')]
class WaitInfo(C.Structure):
    _fields_=[(n,Q) for n in ('token','calls','failures','next_token')]+[(n,U) for n in ('bound','in_callback','creator_thread','period_ms','status')]+[('native_result',I)]
CASES=('reference','wait_binding','rate60','rate120','rate240','fraction','backpressure_recovery','backpressure','source_failure','source_failure_pending','backwards','overflow','foreign','reentrant','final_reentrant','start_failure','real_wait','restart','cleanup_retry')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def fields(v):return {n:getattr(v,n) for n,_ in v._fields_}
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--reference',type=Path);ap.add_argument('--case',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['sound_clock_generated_sha256'].items():assert sha(a.build/n)==h,n
    if not a.case:
        cases={}
        for case in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','build','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--output',str(d/case),'--case',case,'--reference',str(d/'reference')]
            run=subprocess.run(cmd,capture_output=True,timeout=100,creationflags=subprocess.CREATE_NO_WINDOW);(d/(case+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(case,run.returncode,(run.stdout+run.stderr).decode(errors='replace'))
            cases[case]=read(d/case/'result.json');print(case,cases[case]['checks'],'checks',flush=True)
        result=dict(status='actual_elapsed_time_firmware_PCM_native_wait_timing_debt_output_backpressure_and_System_lifetime_verified',
            runtime_checks=sum(v['checks'] for v in cases.values()),fresh_process_cases=len(cases),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
            actual_original_driver_bank_MIDI_and_Windows_output=True,clock_sources_controlled_only_in_arithmetic_cases=True,
            actual_native_wall_waits_and_game_thread_callbacks=True,full_gameplay_or_hardware_cycle_fidelity_verified=False,
            playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    present=C.CDLL(b['present_DLL']);frames=C.CDLL(b['frames_DLL']);system=C.CDLL(b['System_DLL']);sound=C.CDLL(b['game_sound_DLL']);exit=C.CDLL(b['authored_exit_DLL']);fs=C.CDLL(b['GDFS_DLL']);device=C.CDLL(b['device_DLL'])
    core=C.CDLL(str(a.build/'jp_native_aica.dll'));audio=C.CDLL(str(a.build/'jp_native_audio.dll'));g2=C.CDLL(str(a.build/'jp_native_g2_audio.dll'))
    start=bind(sound,'jp_sound_clock_start',I,P);tick=bind(sound,'jp_sound_clock_tick',I);stop=bind(sound,'jp_sound_clock_stop',I,U)
    sysstart=bind(system,'jp_system_start_sound_clock',I,P,P);sysclose=bind(system,'jp_system_shutdown',I);clean=bind(exit,'jp_exit_cleanup',I)
    labels=[];keep=[];observations={};system_live=False;prereqs=False;now=Q(1<<50);failure=[False];events=[];callback_errors=[];read_completions=[]
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    def snap(module,name,kind):v=kind();check(bind(module,name,I,P)(C.byref(v))==0,'actual '+name);return v
    def info():return snap(sound,'jp_sound_clock_snapshot',Info)
    def si():return snap(sound,'jp_sound_snapshot',SoundInfo)
    def sy():return snap(system,'jp_system_snapshot',SystemInfo)
    def wi():return snap(present,'jp_wait_service_snapshot',WaitInfo)
    def ci():return snap(core,'jp_aica_snapshot',AicaInfo)
    @ClockSource
    def controlled(ctx,out):
        try:
            if a.case in ('reentrant','final_reentrant'):
                events.append([sysclose(),clean(),tick(),stop(1),info().in_tick])
            if failure[0]:return 0
            out[0]=C.cast(ctx,C.POINTER(Q))[0];return 1
        except BaseException as error:callback_errors.append(repr(error));return 0
    keep.append(controlled)
    def clock_options(default=False,hz=781250,quantum=441,budget=8192):
        return Options(56,quantum,budget,5,None if default else C.cast(controlled,P).value,None if default else C.addressof(now),0 if default else hz,None,None)
    def sound_options(output=1,capacity=65536):return SoundOptions(56,0,2<<20,5000,AicaOptions(32,output,capacity,40,5000,1,1,0),None)
    def raw_start(output=1,capacity=65536):check(bind(system,'jp_system_start_sound',I,P)(C.byref(sound_options(output,capacity)))==0,'compiled System starts real original sound before explicit timing')
    @Source
    def input_source(ctx,samples):C.memset(samples,0,4*C.sizeof(Sample));samples[0]=Sample(1,0,0,0,0,0,0,0);return 1
    path=C.create_unicode_buffer(str(a.track.resolve()));sysopts=SystemOptions(96,0,DeviceOptions(640,480,0,0,5000,0,64<<20),C.addressof(path),2352,3,5000,0,C.cast(input_source,P).value,None,None,None)
    def initialize_system():
        nonlocal system_live
        check(bind(system,'jp_system_configure',I,P)(C.byref(sysopts))==0 and bind(system,'jp_system_initialize',I,I,I,I)(1,0,1)==0,'actual compiled System creates GDFS/window/wait/heap');system_live=True
    def sdk_state(name,count):out=(Q*count)();check(bind(sound,name,I,P)(out)==0,'passive actual '+name);return list(out)
    def pcm(frames):
        out=bytearray()
        while frames:
            n=min(frames,4093);buf=C.create_string_buffer(n*4);check(bind(sound,'jp_sound_render',I,U,P,Q)(n,buf,len(buf))==0,'explicit independent actual firmware reference render');out+=buf.raw;frames-=n
        return bytes(out)
    def flush(timed=False):
        for turn in range(100):
            check(bind(sound,'jp_sound_service',I)()==0,'real original SDK services commands before reference anchor')
            if timed:check(bind(present,'jp_wait_window_idle',I,P,U)(bind(device,'jp_device_window',P)(),5)==1,'real timed wait advances live original firmware commands')
            else:pcm(441)
            queue=sdk_state('peer_commands',8);sdk=sdk_state('peer_observe',20);status=(C.c_uint8*1)()
            check(bind(core,'jp_aica_read',I,U,P,U)(sdk[14],status,1)==0,'actual firmware command status')
            if not any(queue[:3]) and not sdk[7] and not sdk[8] and status[0]!=1:return
        raise AssertionError('real firmware failed to accept commands')
    def note_prelude(timed=False):
        flush(timed);handle=bind(fs,'gdFsOpen',P,C.c_char_p,P)(b'emeraldcoast.mlt',None);size=I()
        check(handle and bind(fs,'gdFsGetFileSize',I,P,P)(handle,C.byref(size)) and size.value==1125984,'original GDFS opens actual stage sound bank')
        sectors=(size.value+2047)//2048;data=C.create_string_buffer(sectors*2048+32);pointer=(C.addressof(data)+31)&~31;keep.append(data)
        check(bind(fs,'gdFsRead',I,P,I,P)(handle,sectors,pointer)==0 and hashlib.sha256(C.string_at(pointer,size.value)).hexdigest()=='362a8b1077e403bf864acbbf756a854e9299f25f9111a5ad26a2add634c25792','real stage bank bytes match independent retail hash')
        bind(fs,'gdFsClose',None,P)(handle);h=P()
        check(bind(sound,'sdMemBlkCreate',I,P)(C.byref(h))==0 and bind(sound,'sdMemBlkSetPrm',I,P,P,I,P,P)(h,pointer,size.value,0xffffffff,None)==0 and bind(sound,'jp_sd_download_multi',I,P)(h)==0 and bind(sound,'sdMemBlkDestroy',I,P)(h)==0,'original SDK/G2 downloads actual bank')
        port=sdk_state('peer_game_sound',18)[6]
        for status,x,y in [(0xb0,121,0),(0xb0,32,0),(0xc0,0,0),(0xb0,10,64),(0xe0,0,64),(0xb0,1,127),(0xb0,7,106),(0x90,60,127)]:
            data=(C.c_uint8*4)();args=(C.c_int(x),) if status&0xf0 in (0xc0,0xa0) else (C.c_int(x),C.c_int(y))
            check(bind(sound,'sdMidiSetMes',I,P,P,I)(port,data,status,*args)==0 and bind(sound,'sdMidiSendMes',I,P,P,C.c_int8)(port,data,0)==0,'original game-owned MIDI port produces actual note request')
        flush(timed);observations['anchor_core_frames']=ci().frames
    try:
        check(C.sizeof(Options)==56 and C.sizeof(Info)==136 and C.sizeof(WaitInfo)==56 and info().phase==0,'native timing and callback ABI/cold state')
        for name,args in [('jp_queue_initialize',(8<<20,)),('jp_frames_initialize',(8,128,32<<20)),('jp_textures_initialize',(32,1024,32<<20)),('jp_render_state_initialize',(128,1<<20))]:
            check(bind(frames,name,I,*([S] if len(args)==1 else [U,U,S] if len(args)==3 else [U,S]))(*args)==0,'actual borrowed render prerequisite '+name)
        prereqs=True;initialize_system()
        if a.case=='wait_binding':
            token=Q(99);calls=[]
            @Callback
            def callback(ctx):
                calls.append([ctx,wi().in_callback,bind(present,'jp_wait_service_unbind',I,Q)(token.value),bind(present,'jp_wait_service_run',I)()]);return 0
            keep.append(callback);wb=bind(present,'jp_wait_service_bind',I,P,P,U,P);unbind=bind(present,'jp_wait_service_unbind',I,Q)
            check(wb(None,None,5,C.byref(token))==1 and token.value==99 and wb(callback,C.addressof(now),5,C.byref(token))==0,'real native wait accepts one owned high callback context')
            check(sysstart(C.byref(sound_options()),C.byref(clock_options()))==2 and si().resources==0 and sy().phase==3,'timed System refuses pre-existing wait callback before acquiring audio or closing core')
            check(bind(present,'jp_wait_service_run',I)()==0 and calls==[[C.addressof(now),1,4,4]],'actual optional callback cannot recursively run or retire itself')
            check(bind(present,'jp_wait_service_limit',U,U)(50)==5 and unbind(token.value+1)==2 and unbind(token.value)==0,'same callback period caps native wait and exact token retires context')
        elif a.case=='reference':
            raw_start(0);note_prelude();output=pcm(44100);check(any(output) and max(abs(x[0]) for x in struct.iter_unpack('<h',output))>100,'actual original note reference is nonzero')
            memory=C.create_string_buffer(2<<20);check(bind(core,'jp_aica_read',I,U,P,U)(0,memory,len(memory))==0,'independent complete sound RAM reference')
            floats=b''.join(struct.pack('<f',x[0]/32768.0) for x in struct.iter_unpack('<h',output))
            observations.update(PCM_sha256=hashlib.sha256(output).hexdigest(),float_FNV=fnv(floats),full_RAM_sha256=hashlib.sha256(memory.raw).hexdigest())
            (d/'private_reference.s16le').write_bytes(output);(d/'private_reference_RAM.bin').write_bytes(memory.raw)
        elif a.case.startswith('rate') or a.case in ('fraction','backpressure_recovery'):
            recovery=a.case=='backpressure_recovery';raw_start(capacity=4096 if recovery else 65536);note_prelude();hz=1000003 if a.case=='fraction' else 781250;rate=1 if recovery else 137 if a.case=='fraction' else int(a.case[4:]);opts=clock_options(hz=hz)
            check(start(C.byref(opts))==0 and info().initial_core_frames==observations['anchor_core_frames'],'start fresh native-width time anchor after actual driver/note prelude')
            anchor=now.value
            for n in range(1,rate+1):
                now.value=anchor+n*hz//rate;check(tick()==0,'actual scheduled firmware/PCM service')
                v=info();elapsed=now.value-anchor
                check(v.due_frames==elapsed*44100//hz and v.fraction==elapsed*44100%hz and v.rendered_frames+v.backlog==v.due_frames,'independent rational elapsed-time target preserves fractional samples and all debt')
            if recovery:
                for turn in range(1000):
                    before=info().rendered_frames
                    if before==44100:break
                    time.sleep(0.005);check(tick()==0,'actual worker frees output capacity for retained debt recovery');v=info()
                    check(v.rendered_frames<=before+8192 and v.backlog==44100-v.rendered_frames and ci().frames==v.initial_core_frames+v.rendered_frames,'bounded catch-up neither skips nor double-renders any firmware frame')
                else:raise AssertionError('actual output queue did not recover retained debt')
                check(info().queue_full>0,'real finite Windows queue exercised backpressure before recovery')
            v=info();check(v.rendered_frames==44100 and v.backlog==0 and stop(0)==0,'same exact second at independent caller cadence renders44100 frames')
            check(bind(core,'jp_aica_drain',I,U)(5000)==0,'actual Windows stream physically drains submitted note frames')
            expected=read(a.reference/'result.json')['observations'];actual=snap(audio,'jp_audio_snapshot',AudioInfo)
            check(actual.submitted==actual.consumed==44100 and actual.source_hash==expected['float_FNV'] and actual.session_muted==1,'entire actual delivered float PCM equals independent original-driver reference across cadence')
            memory=C.create_string_buffer(2<<20);check(bind(core,'jp_aica_read',I,U,P,U)(0,memory,len(memory))==0 and memory.raw==(a.reference/'private_reference_RAM.bin').read_bytes(),'all2MiB actual firmware RAM equals independent explicit-progression reference')
            observations.update(clock=fields(v),actual_PCM_FNV=actual.source_hash)
        elif a.case=='start_failure':
            failure[0]=True;check(sysstart(C.byref(sound_options()),C.byref(clock_options()))==13 and sy().resources==0 and si().resources==0 and wi().bound==0,'real failing clock provider rolls back owned sound and core startup')
            check(snap(audio,'jp_audio_snapshot',AudioInfo).state==4,'failed timed startup joins the actual Windows audio worker');system_live=False
        else:
            opts=clock_options(default=a.case=='real_wait',hz=1 if a.case=='overflow' else 781250,budget=8192)
            if a.case=='overflow':now.value=0
            check(sysstart(C.byref(sound_options(capacity=4096 if a.case=='backpressure' else 65536)),C.byref(opts))==0,'compiled System starts actual timed original sound and guarded native wait callback')
            clock_begin_perf=time.perf_counter()
            first=info();check(first.initial_core_frames==88200 and first.rendered_frames==0 and wi().bound==1,'original two-driver warmup is excluded from new elapsed-time debt')
            guard=C.create_string_buffer(b'guard');check(bind(sound,'jp_sound_render',I,U,P,Q)(1,guard,len(guard))==2 and bind(sound,'jp_sound_pump',I,U)(1)==2 and guard.raw.startswith(b'guard'),'manual firmware progress cannot double-advance active timed owner')
            if a.case=='backpressure':
                now.value+=781250;check(tick()==0,'bounded real firmware work under long elapsed debt');before=info()
                for turn in range(20):
                    check(tick()==0,'actual output queue backpressure remains a nonfatal tick');v=info()
                    if v.queue_full:break
                else:raise AssertionError('real output queue did not reach capacity')
                check(v.backlog>0 and v.rendered_frames<=v.due_frames and ci().frames==v.initial_core_frames+v.rendered_frames and stop(0)==11 and info().phase==1,'bounded work/actual queue-full retains every unrendered frame and refuses silent debt loss')
                check(stop(1)==0 and info().cancelled_frames==v.backlog and info().backlog==0,'explicit cancellation accounts for pending frames without advancing firmware')
            elif a.case in ('source_failure','source_failure_pending','backwards','overflow'):
                before=ci().frames
                if a.case=='source_failure_pending':
                    handle=bind(fs,'gdFsOpen',P,C.c_char_p,P)(b'emeraldcoast.mlt',None);data=C.create_string_buffer(256*2048+32);keep.append(data);pointer=(C.addressof(data)+31)&~31
                    End=C.CFUNCTYPE(None,P)
                    @End
                    def completed(ctx):read_completions.append(1)
                    keep.append(completed);check(handle and bind(fs,'gdFsReqRd32',I,P,I,P)(handle,256,pointer)==256,'real original GDFS read pending before audio clock fault')
                    bind(fs,'gdFsEntryRdEndFunc',None,P,P,P)(handle,completed,None);bind(fs,'gdFsExecServer',None)()
                    check(snap(fs,'jp_gdfs_snapshot',GDFSInfo).pending==1,'actual native asynchronous read remains owned at fault')
                if a.case.startswith('source_failure'):failure[0]=True;expected=5
                elif a.case=='backwards':now.value-=1;expected=6
                else:now.value=(1<<64)-1;expected=7
                processed=U(99)
                check(bind(present,'jp_wait_pump',I,P)(C.byref(processed))==7 and processed.value==0 and wi().status==5 and wi().native_result==expected and info().status==expected and ci().frames==before,
                    'actual native wait reports terminal callback source/backwards/overflow failure without invented firmware progress')
                check(stop(1)==0 and wi().bound==0,'failed timing context retires safely and releases borrowed provider')
                handle=snap(fs,'jp_gdfs_snapshot',GDFSInfo).vblank;record=Record()
                check(bind(present,'jp_vblank_chain_describe',I,U,P)(handle,C.byref(record))==0 and record.callback,
                    'terminal wait permits genuine owned callback identification for safe teardown')
                check(bind(present,'syChainAddHandler',U,C.c_int16,P,U,P)(0x900,C.cast(bind(present,'njBlank',None,P),P),88,None)==0 and bind(present,'jp_wait_status',I)()==7,
                    'terminal wait remains failed and cannot register new callbacks')
                sentinel=Record();C.memset(C.byref(sentinel),0xa5,C.sizeof(sentinel));foreign=[]
                def other():
                    foreign.append(bind(present,'jp_vblank_chain_describe',I,U,P)(handle,C.byref(sentinel)));bind(present,'syChainDeleteHandler',None,U)(handle)
                thread=threading.Thread(target=other);thread.start();thread.join(5)
                check(not thread.is_alive() and foreign==[3] and bytes(sentinel)==b'\xa5'*C.sizeof(sentinel),'foreign terminal-wait inspection/deletion cannot steal live GDFS callback')
            elif a.case=='foreign':
                output=Info();C.memset(C.byref(output),0xa5,136);results=[]
                def other():results.extend([tick(),stop(1),start(C.byref(opts)),sysclose(),bind(sound,'jp_sound_clock_snapshot',I,P)(C.byref(output)),bind(present,'jp_wait_service_run',I)()])
                thread=threading.Thread(target=other);thread.start();thread.join(5)
                check(not thread.is_alive() and results==[3]*6 and bytes(output)==b'\xa5'*136,'foreign access cannot advance/retire sound clock or alter outputs')
            elif a.case in ('reentrant','final_reentrant'):
                if a.case=='final_reentrant':check(bind(system,'jp_system_bind_exit',I,U)(245762|524288)==0,'real timed sound authority transfers to final exit')
                now.value+=78125;check(bind(present,'jp_wait_service_run',I)()==0 and not callback_errors and events and all(x==[4,4,4,4,1] for x in events),'actual clock callbacks cannot mutate System/global/sound lifetime before or after final binding')
                check(clean()==0,'actual guarded clock callback/context retires before original sound/core providers');system_live=False
            elif a.case=='cleanup_retry':
                now.value+=78125;check(tick()==0 and bind(g2,'jp_g2_gate_set',I,I)(1)==0,'actual timed sound progresses before real transport cleanup failure')
                check(sysclose()==12 and wi().bound==0 and info().phase==2 and si().resources==27 and sy().resources==245762|524288,'timed callback retires first while failed transport keeps real audio/core resources')
                bind(g2,'jp_g2_gate_set',I,I)(0);check(sysclose()==0,'same real System retries retained sound cleanup');system_live=False
            elif a.case=='restart':
                now.value+=1000;check(stop(0)==11 and info().backlog==1000*44100//781250,'stopping samples unobserved elapsed time rather than silently dropping debt')
                check(stop(1)==0 and sysclose()==0,'explicit cancelled timing and ordinary System close');initialize_system()
                check(sysstart(C.byref(sound_options()),C.byref(clock_options()))==0 and info().wait_token>first.wait_token and info().rendered_frames==0,'actual fresh driver/PCM/timing session restarts with nonrecycled wait token')
            elif a.case=='real_wait':
                note_prelude(True);window=bind(device,'jp_device_window',P)();idle=bind(present,'jp_wait_window_idle',I,P,U)
                for rate in (60,120,240):
                    for n in range(12):time.sleep(1/rate);check(idle(window,0)==1,'actual native window idle services real sound at varied caller cadence')
                bind(present,'jp_wait_probe_reset',None,U,U)(0,0);bind(present,'frame_pacing_init',None,I,I)(1,1)
                for n in range(10):bind(present,'frame_set_interval',None,I)(1);bind(present,'frame_wait_step',None,I)(0)
                v=info();actual=snap(audio,'jp_audio_snapshot',AudioInfo);elapsed=(v.last_tick-v.anchor)/781250
                check(bind(present,'jp_wait_status',I)()==0 and v.due_frames==(v.last_tick-v.anchor)*44100//781250 and v.rendered_frames+v.backlog==v.due_frames and ci().frames==v.initial_core_frames+v.rendered_frames,
                    'real wall-clock and original frame waits preserve exact elapsed sample target independent of presentation cadence')
                check(0.4<elapsed<5 and abs(elapsed-(time.perf_counter()-clock_begin_perf))<0.1 and actual.submitted==v.rendered_frames and wi().calls>46,'actual QPC-paced firmware/Windows output runs between original native waits')
                check(stop(1)==0 and bind(core,'jp_aica_drain',I,U)(5000)==0,'actual live QPC note frames drain after explicit timing stop')
                delivered=snap(audio,'jp_audio_snapshot',AudioInfo)
                check(delivered.consumed==delivered.submitted and delivered.submitted>1000 and delivered.source_hash!=fnv(bytes(delivered.submitted*8)),
                    'actual real-time Windows delivery contains original nonzero bank/note audio')
                observations.update(real_elapsed_seconds=elapsed,clock=fields(v),wait=fields(wi()))
        check(not callback_errors,'native callback errors are not concealed')
        if system_live:
            closed=sysclose();check(closed==0,'compiled System retires actual timed callback before G2/SDK/PCM/core/GDFS/wait: '+str((closed,fields(sy()),fields(si()))));system_live=False
        if a.case=='source_failure_pending':check(not read_completions and snap(fs,'jp_gdfs_snapshot',GDFSInfo).work==0,'fatal audio/wait cleanup cancels actual pending read without late completion or retained file storage')
        check(wi().bound==0,'no periodic callback can reach retired sound context');observations['final_clock']=fields(info())
        result=dict(case=a.case,checks=len(labels),assertions=labels,observations=observations);(d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
    finally:
        if system_live:sysclose()
        if prereqs:
            bind(frames,'jp_frames_shutdown',I)();bind(frames,'jp_render_state_shutdown',None)();bind(frames,'jp_textures_shutdown',None)();bind(frames,'jp_queue_shutdown',None)()
if __name__=='__main__':main()
