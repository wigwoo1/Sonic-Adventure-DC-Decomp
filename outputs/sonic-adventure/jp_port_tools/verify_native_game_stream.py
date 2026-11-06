"""Actual original game tasks, requests and concurrent retail music/voice playback."""
import argparse,ctypes as C,hashlib,json,struct,subprocess,sys,threading,time,wave
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
from rna_test_runtime import Runtime,Joints
from adx_media_fixtures import Disc
from adx_step_oracle import StereoADX
from verify_native_adxt import MonoADX,Info as AdxtInfo
from verify_native_audio import bind,Info as AudioInfo
from verify_native_adxstm import Info as StmInfo
from verify_native_g2_audio import Info as G2Info
from verify_native_gdfs import Info as FSInfo
I,U,Q,P=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p
CASES=('lifecycle','requests','pool_failure','mix_cpu','mix_dma','interlude','global_pending','timed')
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('initializations','shutdowns','steps','errors','task','pending_task','music','voice')]+[(n,U) for n in ('active','stopping','ready','status','children','archive_owned')]+[(n,I) for n in ('music_request','voice_request','music_current','music_saved','music_state','voice_state','native_error','reserved')]
class Task(C.Structure):
    _fields_=[(n,P) for n in ('next','prev','parent','child','step','draw','destructor','placement','work','motion','force','extra')]
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())
def signed(n):return C.c_int32(n).value

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);ap.add_argument('--reference',type=Path);a=ap.parse_args()
    r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,key in [('external_game_stream.c','game_stream_generated_source_sha256'),('sound_owner_stream.c','game_stream_owner_generated_source_sha256'),('external_adx_fs.c','game_stream_adxf_generated_source_sha256')]:assert sha(a.build/n)==b[key],n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256'];d.mkdir()
    if not a.case:
        cases={}
        for name in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','build','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--case',name,'--output',str(d/name),'--reference',str(d/'mix_cpu')]
            run=subprocess.run(cmd,capture_output=True,timeout=180,creationflags=subprocess.CREATE_NO_WINDOW);(d/(name+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-6500:])
            cases[name]=read(d/name/'result.json');print(name,cases[name]['checks'],'checks',flush=True)
        result=dict(status='original_game_stream_task_requests_concurrent_retail_playback_and_native_lifetimes_verified',runtime_checks=sum(x['checks'] for x in cases.values()),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),retail_data_published=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    labels=[];obs={};rt=None;groups=[];messages=[];pool_ready=False
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    try:
        rt=Runtime(b,a.track,check,timed=a.case in ('global_pending','timed'));lib=rt.lib;frames=rt.frames;fs=C.CDLL(b['GDFS_DLL'])
        check(C.sizeof(Info)==120 and C.sizeof(Task)==96,'actual native stream info/task ABI')
        check(bind(lib,'jp_game_stream_initialize',I)()==2,'missing active ADXT parent rejected')
        check(bind(lib,'jp_adxstm_initialize',I)()==0 and bind(lib,'jp_adxf_initialize',I)()==0 and bind(lib,'jp_adxt_initialize',I)()==0,'actual lower file/archive/player owners ready')
        check(bind(fs,'gdFsChangeDir',I,C.c_char_p)(b'/SONICADV')==0,'real retail audio directory')
        ERROR=C.CFUNCTYPE(None,P,P)
        @ERROR
        def error(ctx,msg):messages.append((ctx,C.string_at(msg).decode(errors='replace')))
        rt.keep.append(error);bind(lib,'ADXERR_EntryErrFunc',None,P,P)(error,0x123456789);bind(lib,'mwRnaErrEntryFunc',None,P,P)(error,0xabcdef012)
        def info():
            v=Info();check(bind(lib,'jp_game_stream_snapshot',I,P)(C.byref(v))==0,'actual original stream tasks/handles/requests snapshot');return v
        def adxt():
            v=AdxtInfo();check(bind(lib,'jp_adxt_snapshot',I,P)(C.byref(v))==0,'actual shared ADXT records/owned work');return v
        def pool():
            v=(U*4)();check(bind(frames,'jp_startup_pool_info',U,P)(v)==0,'actual original task pool free-list invariants');return list(v)
        def taskpass():bind(frames,'task_step_all',None)()
        def initialize():
            check(bind(lib,'jp_game_stream_initialize',I)()==0,'schedule the real original stream task without resetting any heap')
            check(info().active and info().pending_task and not info().ready,'original task remains pending until its normal scheduler pass')
            taskpass();v=info()
            if not v.ready:(d/'initialization_diagnostic.json').write_text(json.dumps(dict(state={n:getattr(v,n) for n,_ in Info._fields_},messages=messages,pool=pool(),adxt={n:getattr(adxt(),n) for n,_ in AdxtInfo._fields_}),indent=2)+'\n')
            check(v.ready and not v.pending_task and v.music and v.voice and v.archive_owned,'actual original initializer owns both players and voice table')
            check(adxt().owned_bytes==239944,'native work retains original music and voice sizes')
        def request(kind,n):check(bind(lib,'jp_game_stream_request_'+kind,I,I)(n)==0,'actual original '+kind+' request accepted')
        def action(kind,value=0):
            result=I(-99);check(bind(lib,'jp_game_stream_action',I,U,I,P)(kind,value,C.byref(result))==0,'actual original stream control '+str(kind));return result.value
        def state(h):
            out=(Q*33)();check(bind(lib,'peer_adxt_state',I,P,P)(h,out)==0,'actual original ADXT subplayer record');return list(out)
        def finish():
            check(bind(lib,'jp_game_stream_shutdown',I)()==0,'actual original task retires native work/table before its audio parents')
            v=info();check(not any((v.active,v.task,v.pending_task,v.music,v.voice,v.archive_owned)),'no stream task/player/archive ownership remains')
        if a.case=='lifecycle':
            check(bind(lib,'jp_game_stream_initialize',I)()==9 and not info().active,'missing real game task pool fails without constructing audio players')
        bind(frames,'jp_heap_reset',None)();bind(frames,'task_init',None)();pool_ready=True
        check(pool()==[4096,4096,0,0],'real original heap/task pool initialized by caller fixture only')
        C.c_int32.in_dll(frames,'event_skipping').value=0
        if a.case=='pool_failure':
            tasks=[bind(frames,'task_create',P,C.c_uint16,I,P)(0,0,None) for _ in range(4096)]
            check(all(tasks) and pool()[1]==0,'actual complete original task pool exhausted')
            check(bind(lib,'jp_game_stream_initialize',I)()==9 and not adxt().live_players and not info().archive_owned,'pool exhaustion does not allocate players or voice work')
            bind(frames,'task_destroy',None,P)(tasks.pop());initialize()
            request('music',46);before=info();check(bind(lib,'jp_game_stream_request_interlude',I,I)(0)==9,'actual child task allocation failure returns cleanly')
            check(info().music_request==before.music_request and not info().children,'failed interlude preserves previous pending music without a NULL work write')
            finish();check(pool()[1]==1,'stream destructor returns its actual exhausted-pool slot')
            for h in tasks:bind(frames,'task_destroy',None,P)(h)
            groups=[Joints(rt,1) for _ in range(3)];renderers=[rt.create(g.handles) for g in groups];check(all(renderers),'three unrelated real sound ports before failed game initialization')
            check(bind(lib,'jp_game_stream_initialize',I)()==0,'real task scheduled for actual port exhaustion');taskpass();v=info()
            check(not v.active and not v.task and not v.music and not v.voice and not v.archive_owned and not adxt().owned_work,'failed second player creation rolls back music, task, partition and all owned buffers')
            check(rt.info().live_renderers==3,'failed game construction preserves unrelated real renderers')
            for h,g in zip(renderers,groups):bind(lib,'mwRnaDestroy',None,P)(h);g.retired();g.close()
            groups.clear();messages.clear();initialize();finish()
            obs.update(actual_task_pool_capacity=4096,actual_interlude_child_failure_rollback=True,actual_second_player_port_failure_transactional_rollback=True)
        elif a.case=='lifecycle':
            calls=[];STEP=C.CFUNCTYPE(None,P)
            @STEP
            def unrelated(h):calls.append(h)
            rt.keep.append(unrelated);carry=bind(frames,'task_create',P,C.c_uint16,I,P)(0,0,unrelated)
            initialize();check(calls==[carry] and pool()[1]==4094,'normal task pass preserves and steps unrelated original task')
            v=info();check(v.task>0xffffffff and Task.from_address(v.task).step and Task.from_address(v.task).destructor,'actual scheduled native task owns original callbacks')
            flag=C.c_int32.from_address(bind(lib,'peer_game_stream_error_address',P)());flag.value=1
            check(bind(lib,'peer_adx_error',I)()==1,'controlled flag fixture reaches the actual decoder module data')
            taskpass();check(flag.value==0 and bind(lib,'peer_adx_error',I)()==0,'actual original stream task clears the same real decoder error flag')
            check(bind(lib,'jp_adxt_shutdown',I)()==2 and bind(lib,'jp_adxf_partition_unload',I,U)(0)==2,'active stream protects its parent player owner and reserved voice table')
            check(bind(lib,'jp_adxf_partition_load',I,U,C.c_char_p,U)(0,b'EVENT_ADX.AFS',2088)==2,'native callers cannot replace live game voice table')
            results=[];t=threading.Thread(target=lambda:results.append(bind(lib,'jp_game_stream_shutdown',I)()));t.start();t.join(5)
            check(not t.is_alive() and results==[3] and info().ready,'foreign teardown rejects without changing real task/players')
            finish();taskpass();check(calls==[carry,carry,carry] and pool()[1]==4095,'stream cleanup leaves unrelated task runnable')
            bind(lib,'ADXERR_CallErrFunc',None,C.c_char_p)(b'after stream teardown')
            check(messages==[(0x123456789,'after stream teardown')],'original stream destructor restores preexisting error callback');messages.clear()
            bind(frames,'task_destroy',None,P)(carry)
            check(bind(lib,'jp_game_stream_initialize',I)()==0,'new pending task can be scheduled');bind(frames,'task_destroy_all',None)()
            check(not info().pending_task and not info().task,'external task destruction clears pending-init pointer before slot reuse');finish()
            initialize();finish();check(pool()==[4096,4096,0,0],'real original pool is fully reusable after stream restarts')
        elif a.case=='requests':
            initialize();v=info();music,voice=v.music,v.voice
            names=[]
            for index in range(256):
                text=C.create_string_buffer(32);ret=bind(lib,'peer_game_stream_music_file',I,U,P,U)(index,text,32)
                if ret:break
                names.append(text.value.decode())
            check(len(names)==110 and names[46]=='FISHMISS.ADX','actual complete original music filename table and tested music ID')
            for kind,value in [('music',110),('music',-2),('voice',2088),('voice',-2),('interlude',110)]:
                before=info();check(bind(lib,'jp_game_stream_request_'+kind,I,I)(value)==1 and info().music_request==before.music_request and info().voice_request==before.voice_request,'invalid native request preserves original pending state')
            request('music',0);request('music',46);request('voice',1);request('voice',0)
            check(info().music_request==46 and info().voice_request==0,'actual pending request replacement uses latest music and voice')
            taskpass();check(info().music_request==info().voice_request==-1 and state(music)[1]==state(voice)[1]==1,'original task starts both real sources then consumes pending requests')
            action(2);check(all(signed(rt.state(state(h)[6])[30])==-960 for h in (music,voice)),'original mute applies to both real renderers')
            action(3);check(signed(rt.state(state(music)[6])[30])==-10 and signed(rt.state(state(voice)[6])[30])==0,'original unmute restores distinct music and voice volumes')
            action(8,46);request('music',0);check(action(5)==1 and info().music_request==46 and info().music_saved==46,'actual saved music restores its own original selection')
            check(action(4)==1 and info().music_request==46,'actual music restart queues the last request')
            action(0);check(info().music_request==46 and info().music_current==-1 and action(4)==0,'original stop forgets restart choice and preserves an already pending request')
            action(6);action(7);check(info().music_request==info().voice_request==-1,'original request resets clear pending values')
            request('music',46);request('voice',1);C.c_int32.in_dll(frames,'event_skipping').value=1;taskpass();C.c_int32.in_dll(frames,'event_skipping').value=0
            check(info().music_request==info().voice_request==-1,'actual shared event-skip state suppresses queued starts behind curtain')
            check(not info().errors,'valid original requests do not emit decoder/file errors');finish()
        elif a.case=='global_pending':
            initialize();request('music',46);request('voice',0);taskpass();rt.service()
            stm=StmInfo();check(bind(lib,'jp_adxstm_snapshot',I,P)(C.byref(stm))==0 and stm.pending_reads>0,'actual original task accepted pending retail file read')
            before=adxt();check(bind(rt.g2,'jp_g2_gate_set',I,I)(15)==0,'hold actual SDK completion gate')
            check(bind(lib,'jp_game_stream_shutdown',I)()==7 and info().task and adxt().owned_bytes==before.owned_bytes,'blocked game shutdown retains actual task and native buffers')
            bind(frames,'task_destroy_all',None)();check(not info().task and info().music and info().voice and adxt().owned_bytes==before.owned_bytes,'external task destructor retains unfinished audio after gated cleanup rejection')
            bind(frames,'jp_heap_reset',None)();pool_ready=False
            check(bind(rt.g2,'jp_g2_gate_set',I,I)(0)==0,'release real gate');rt.close()
            check(not info().active and not info().music and not info().voice and not adxt().owned_bytes,'actual System cleanup retires retained players after game task heap reset')
            obs.update(actual_pending_read_and_failed_external_task_cleanup_retried_by_System=True)
        else:
            initialize();request('music',46);request('voice',0);taskpass();v=info();handles=[v.music,v.voice]
            check(bind(lib,'sdMemBlkSetTransferMode',I,I)(int(a.case=='mix_dma'))==0,'selected actual SDK CPU/DMA mode')
            disc=Disc(a.track.resolve())
            try:
                music_data=disc.read('FISHMISS.ADX',0,disc.files['FISHMISS.ADX'][1]);afs,_=disc.files['EVENT_ADX.AFS'];offset,size=struct.unpack('<II',disc.read('EVENT_ADX.AFS',8,8));voice_data=disc.sectors(afs+offset//2048,(size+2047)//2048)[:size]
            finally:disc.close()
            data=[music_data,voice_data];totals=[214913,37975];payloads=[x[struct.unpack_from('>H',x,2)[0]+4:] for x in data]
            oracles=[StereoADX((7334,-3283)),MonoADX((6569,-2634))];decoded=[0,0];used=[0,0];expected=[[bytearray(),bytearray()],[bytearray()]]
            output=bytearray();overlap=0;task_frames=0;accum=0;deadline=time.monotonic()+90;interlude_phase=0
            if a.case=='interlude':
                request('music',101);taskpass();request('interlude',46);check(info().children==1 and info().music_current==101 and info().music_request==46,'actual interlude child preserves base music and queues temporary track');taskpass();interlude_phase=1
            before_dma=G2Info();check(bind(rt.g2,'jp_g2_snapshot',I,P)(C.byref(before_dma))==0,'actual transfer counters before game stream run')
            for iteration in range(12000):
                if a.case=='timed':
                    hwnd=bind(rt.device,'jp_device_window',P)();check(bind(rt.present,'jp_wait_window_idle',I,P,U)(hwnd,5)==1,'existing real-time wait progresses music/voice through one sound worker');taskpass()
                else:rt.service()
                v=info();states=[state(h) for h in handles]
                if all(bind(lib,'mwRnaGetPlaySw',I,P)(s[6])==1 for s in states):
                    overlap+=1;check(all(bind(lib,'ADXT_GetStat',I,P)(h) in (3,4) for h in handles),'both original game streams play concurrently in same native sound system')
                for index,s in enumerate(states):
                    if a.case in ('interlude','timed'):continue
                    now=bind(lib,'ADXSJD_GetDecNumSmpl',I,P)(s[4]);at=bind(lib,'ADXSJD_GetDecDtLen',I,P)(s[4])
                    check(decoded[index]<=now<=totals[index] and used[index]<=at<=len(payloads[index]),'actual game file decoder progress stays in unmodified retail bounds')
                    if now>decoded[index]:
                        values=oracles[index].decode_step(payloads[index][used[index]:at]);count=now-decoded[index]
                        for ch,samples in enumerate(values):
                            samples=samples[:count*2];expected[index][ch]+=samples;pos=decoded[index]%8192*2;size=min(len(samples),16384-pos);base=s[14]+ch*16448
                            check(C.string_at(base+pos,size)+C.string_at(base,len(samples)-size)==samples,'every game-task decoded music/voice sample matches independent recurrence')
                    decoded[index],used[index]=now,at
                if a.case=='interlude':
                    if v.music_request==101 and v.children and states[0][1]==5:
                        taskpass();taskpass();check(info().children==0 and info().music_request==-1 and state(handles[0])[1]==1,'actual child resumes saved base music and retires through real task scheduler');interlude_phase=2;break
                elif all(s[1]==5 for s in states):break
                if a.case!='timed':
                    output+=rt.render(128);accum+=128
                    if accum>=735:taskpass();task_frames+=1;accum-=735
                    time.sleep(.0001)
                check(time.monotonic()<deadline,'complete actual game task playback makes bounded progress')
            else:raise AssertionError('original game music/voice task did not reach expected completion')
            check(overlap>0 and not v.errors and not messages,('real simultaneous game streams produce no original errors',v.errors,messages))
            if a.case=='interlude':check(interlude_phase==2,'natural full interlude completion observed')
            else:
                for h,total,rate in zip(handles,totals,(44100,22050)):
                    count=I();scale=I();bind(lib,'ADXT_GetTime',None,P,P,P)(h,C.byref(count),C.byref(scale));check(count.value==total and scale.value==rate,'each complete original game player retains exact final sample time')
            after_dma=G2Info();check(bind(rt.g2,'jp_g2_snapshot',I,P)(C.byref(after_dma))==0,'actual transfer counters after game playback');dma_count=after_dma.dma_started-before_dma.dma_started
            if a.case in ('mix_cpu','mix_dma'):
                check(decoded==totals and any(output),'both complete retail streams reach PLAYEND with nonzero mixed original ARM output')
                check(dma_count>0 if a.case=='mix_dma' else dma_count==0,'actual game playback honours selected SDK CPU/DMA path')
                ram=rt.memory(0,2<<20);(d/'private_output.s16le').write_bytes(output);(d/'private_final_RAM.bin').write_bytes(ram)
                if a.case=='mix_dma':check(output==(a.reference/'private_output.s16le').read_bytes() and ram==(a.reference/'private_final_RAM.bin').read_bytes(),'all mixed firmware PCM and full2MiB sound RAM agree across CPU/DMA game-task playback')
                wav=d/'native-JP-game-music-and-voice.wav'
                with wave.open(str(wav),'wb') as f:f.setnchannels(2);f.setsampwidth(2);f.setframerate(44100);f.writeframes(output)
                obs.update(private_WAV=str(wav),PCM_sha256=hashlib.sha256(output).hexdigest(),RAM_sha256=hashlib.sha256(ram).hexdigest(),decoded_samples=decoded,independent_PCM_sha256=[[hashlib.sha256(x).hexdigest() for x in channels] for channels in expected],firmware_frames=len(output)//4)
            if a.case=='timed':
                au=AudioInfo();check(bind(rt.audio,'jp_audio_snapshot',I,P)(C.byref(au))==0 and au.worker_live and au.session_muted,'actual simultaneous game playback uses existing unique muted output worker')
            obs.update(simultaneous_play_observations=overlap,original_task_steps=info().steps,logical_game_passes=task_frames,real_DMA_transfers=dma_count,natural_interlude_resume=a.case=='interlude',automatic_elapsed_time=a.case=='timed')
            finish()
        check(not messages,('remaining original callbacks accounted for',messages))
        if rt.live:
            if info().active:finish()
            rt.close()
        if pool_ready:bind(frames,'task_destroy_all',None)();check(pool()==[4096,4096,0,0],'caller-owned original task pool is completely returned');bind(frames,'jp_heap_reset',None)();pool_ready=False
    finally:
        if rt and rt.live:
            bind(rt.g2,'jp_g2_gate_set',I,I)(0);bind(rt.lib,'jp_game_stream_shutdown',I)();rt.close()
        if rt and pool_ready:bind(rt.frames,'task_destroy_all',None)();bind(rt.frames,'jp_heap_reset',None)()
    result=dict(case=a.case,checks=len(labels),assertions=labels,observations=obs)
    (d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)

if __name__=='__main__':main()
