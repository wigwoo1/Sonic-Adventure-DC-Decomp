"""Qualify actual muted WASAPI output, PCM ordering and owned worker lifetimes.

No audio capture, endpoint-volume changes, retail data, simulated render client,
or substituted COM/audio functions. Packet hashing observes what was handed to
the real render client; this is not an audible-speaker or game-sound test.
"""
import argparse
import ctypes as C
import hashlib
import json
import struct
import subprocess
import sys
import threading
import time
from pathlib import Path

U=C.c_uint32;Q=C.c_uint64;I=C.c_int32;P=C.c_void_p;F=C.c_float
OFFSET=14695981039346656037;PRIME=1099511628211;MASK=(1<<64)-1
CASES=('pcm_48000','pcm_44100_half','pcm_zero_gain','overflow','stalled_worker','opening_close','concurrent_open')

class Config(C.Structure):
    _fields_=[(n,U) for n in ('bytes','rate','capacity_frames','latency_ms','timeout_ms','muted')]+[('gain',F),('reserved',U)]

class Info(C.Structure):
    _fields_=[(n,Q) for n in ('submitted','consumed','silence','discarded','events','clock_position','clock_frequency','source_hash','clipped_samples','client_frames','last_user_frame')]+[(n,U) for n in ('state','status','os_error','rate','capacity_frames','queued_frames','device_buffer_frames','channels','worker_live','creator_thread','worker_thread','session_muted')]+[('stream_latency_hns',Q)]

class Guard(C.Structure):
    _fields_=[('before',Q),('info',Info),('after',Q)]

def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def bind(lib,name,result,*args):
    f=getattr(lib,name);f.restype=result;f.argtypes=args;return f
def fnv(raw,value=OFFSET):
    for byte in raw:value=((value^byte)*PRIME)&MASK
    return value
def fields(value):return {name:getattr(value,name) for name,_ in value._fields_}
def sample_clock(info):return info.clock_position*info.rate//info.clock_frequency

def main():
    ap=argparse.ArgumentParser()
    for name in ('workspace','build','output'):ap.add_argument('--'+name,type=Path,required=True)
    ap.add_argument('--case',choices=CASES)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['DLL'])==b['DLL_sha256'] and sha(a.build/'audio.obj')==b['object_sha256']
    if not a.case:
        results={}
        for case in CASES:
            run=subprocess.run([sys.executable,'-B',str(Path(__file__).resolve()),'--workspace',str(r),'--build',str(a.build.resolve()),'--output',str(d/case),'--case',case],capture_output=True,timeout=40)
            (d/(case+'.log')).write_bytes(run.stdout+run.stderr)
            assert run.returncode==0,(case,run.returncode,run.stdout.decode(errors='replace'),run.stderr.decode(errors='replace'))
            results[case]=json.loads((d/case/'result.json').read_bytes());print(case,results[case]['checks'],'checks',flush=True)
        for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
        result=dict(status='actual_muted_WASAPI_PCM_order_clipping_clock_drain_stall_and_worker_lifetimes_verified',checks=sum(x['checks'] for x in results.values()),cases=results,
            build_verification_sha256=sha(a.build/'build_verification.json'),DLL_sha256=sha(b['DLL']),verifier_sha256=sha(__file__),
            actual_native_audio_services_replaced=False,captured_user_audio=False,endpoint_volume_changed=False,
            audible_speaker_output_verified=False,game_driver_MIDI_music_effects_or_parent_integrated=False,
            retail_assets_used=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True);return
    lib=C.CDLL(b['DLL']);kernel=C.WinDLL('kernel32',use_last_error=True)
    open_=bind(lib,'jp_audio_open',I,P);submit=bind(lib,'jp_audio_submit',I,P,U);drain=bind(lib,'jp_audio_drain',I,U)
    discard=bind(lib,'jp_audio_discard_queued',I);close=bind(lib,'jp_audio_close',I,U);status=bind(lib,'jp_audio_status',I);snapshot=bind(lib,'jp_audio_snapshot',I,P)
    labels=[];observations={};own=False
    def check(ok,label):
        assert ok,(a.case,label);labels.append(label)
    def info():
        v=Guard(0x19438ef1ac087d24,Info(),0x620fe19b836501a4)
        check(snapshot(C.byref(v.info))==0 and v.before==0x19438ef1ac087d24 and v.after==0x620fe19b836501a4,'bounded native snapshot preserves surrounding canaries')
        return v.info
    def conservation(v):
        check(v.submitted==v.consumed+v.discarded+v.queued_frames and v.client_frames==v.consumed+v.silence,'accepted, consumed, discarded, queued and silent frames conserve independently')
    def cfg(rate=48000,capacity=16384,gain=1,timeout=5000):return Config(C.sizeof(Config),rate,capacity,40,timeout,1,gain,0)
    def start(config):
        nonlocal own
        rc=open_(C.byref(config));own=True
        v=info();check(rc==0 and v.state==2 and v.worker_live==1 and status()==0,'real Windows render endpoint and running audio worker ready')
        check(v.rate==config.rate and v.capacity_frames==config.capacity_frames and v.channels==2 and v.device_buffer_frames>0 and v.clock_frequency>0,'actual device capacity, stream rate and clock frequency reported')
        tid=bind(kernel,'GetCurrentThreadId',U)()
        check(v.creator_thread==tid and v.worker_thread and v.worker_thread!=tid and v.session_muted==1,'distinct real worker thread and queried muted audio session')
        return v
    def finish(timeout=5000):
        nonlocal own
        check(close(timeout)==0,'real audio worker joins and owned handles/buffers retire');own=False
        v=info();check(v.state==4 and not v.worker_live and not v.queued_frames and status()==2,'closed owner cannot accept audio and retains final diagnostics');conservation(v)
        check(close(0)==0,'repeated close does not resurrect a worker');return v
    try:
        check(C.sizeof(Config)==32 and C.sizeof(Info)==144 and status()==2 and close(0)==0,'native full-width audio ABI and harmless cold close')
        check(info().state==0 and snapshot(None)==1 and open_(None)==1,'cold state and null outputs/configurations rejected')
        if a.case.startswith('pcm_'):
            rate=44100 if a.case=='pcm_44100_half' else 48000;gain=.5 if rate==44100 else 0 if a.case=='pcm_zero_gain' else 1
            valid=cfg(rate,gain=gain)
            invalid={'bytes':[0,31,33],'rate':[0,22050,96000],'capacity_frames':[0,4095,1048577],'latency_ms':[0,19,501],'timeout_ms':[0,60001],'muted':[2],'gain':[-.1,1.1,float('nan'),float('inf')],'reserved':[1]}
            for member,values in invalid.items():
                for value in values:
                    c=Config.from_buffer_copy(valid);setattr(c,member,value)
                    check(open_(C.byref(c))==1 and info().state==0,'invalid configuration cannot acquire a native owner: '+member)
            start(valid);check(open_(C.byref(valid))==2,'second open cannot replace the running stream')
            # Caller configuration and PCM storage may vanish after their call.
            valid.rate=0;valid.capacity_frames=1;valid.gain=1-gain
            check(info().rate==rate and info().capacity_frames==16384,'caller configuration was copied, not borrowed')
            sample=(F*2)(.25,-.5);foreign=[];sentinel=Info();C.memset(C.byref(sentinel),0xa5,C.sizeof(sentinel));sentinel_bytes=bytes(sentinel)
            def outsider():foreign.extend([open_(C.byref(valid)),submit(sample,1),drain(0),discard(),close(0),status(),snapshot(C.byref(sentinel))])
            thread=threading.Thread(target=outsider);thread.start();thread.join(5)
            check(not thread.is_alive() and foreign==[3]*7 and bytes(sentinel)==sentinel_bytes,'foreign thread cannot mutate, inspect or close another owner')
            before=info().submitted
            for value in (float('nan'),float('inf'),-float('inf')):
                bad=(F*4)(.5,.25,value,.1);check(submit(bad,2)==1,'nonfinite PCM is rejected before any prefix is enqueued')
            check(submit(None,1)==1 and submit(sample,0)==1 and submit(sample,16385)==1 and info().submitted==before,'invalid spans preserve the accepted count')
            check(drain(60001)==1 and close(60001)==1 and status()==0,'invalid waits do not close a live stream')
            expected=OFFSET;clipped=0;count=0
            pool=(-4.,-2.,-.5,-0.,.125,.5,2.,4.)
            for round_ in range(3):
                for frames in (4093,7,513,8191,4096,31):
                    values=[pool[(count*2+j)%len(pool)] for j in range(frames*2)];data=(F*len(values))(*values);raw=bytearray()
                    for value in values:
                        out=struct.unpack('<f',struct.pack('<f',value*gain))[0];clipped+=int(out>1 or out< -1);raw.extend(struct.pack('<f',max(-1.,min(1.,out))))
                    expected=fnv(raw,expected);deadline=time.monotonic()+5
                    while True:
                        rc=submit(data,frames)
                        if rc!=11:break
                        check(time.monotonic()<deadline,'real endpoint makes bounded progress through a full software queue');time.sleep(.002)
                    check(rc==0,'variable stereo packet accepted through ring wrap')
                    C.memset(data,0x7f,C.sizeof(data));del data;count+=frames
                check(drain(5000)==0,'drain waits for actual output clock after final user packet')
                v=info();conservation(v)
                check(v.submitted==v.consumed==count and v.queued_frames==0 and v.discarded==0,'all submitted stereo frames consumed once')
                check(v.source_hash==expected and v.clipped_samples==clipped,'independent PCM byte hash verifies channel order, source copy, gain, clipping and ring wrap')
                check(v.events>0 and sample_clock(v)>=v.last_user_frame,'actual render events and stream clock prove drain completed')
            # The output worker continues when the application submits nothing.
            before=info();time.sleep(.12);after=info()
            check(after.clock_position>before.clock_position and after.silence>before.silence and after.consumed==before.consumed and after.source_hash==expected,'idle underrun produces silence without inventing source samples')
            observations.update(frames=count,PCM_hash=expected,clipped_samples=clipped,last=fields(after));finish()
        elif a.case=='overflow':
            capacity=262144;start(cfg(capacity=capacity));data=(F*(capacity*2))(*([.25,-.5]*capacity))
            check(submit(data,capacity)==0 and submit(data,capacity)==11,'real bounded queue rejects overflow as an entire packet')
            before=info();begin=time.monotonic();check(drain(0)==10 and time.monotonic()-begin<.5,'nonblocking drain times out while real queued data remains')
            after=info();conservation(after);check(after.submitted==capacity and after.discarded==0,'overflow and timeout never silently discard accepted PCM')
            check(discard()==0,'explicit discard removes only software queued audio');v=info();conservation(v)
            check(v.queued_frames==0 and v.discarded>0 and v.source_hash==fnv(struct.pack('<ff',.25,-.5)*v.consumed),'discard preserves already submitted packet hash and accounting')
            check(drain(5000)==0,'already scheduled user packets drain after software discard')
            check(submit(data,capacity)==0,'another actual long queue before bounded shutdown')
            rc=close(0);check(rc in (0,10),'zero-time close either joins or retains ownership for retry')
            if rc==10:
                check(open_(C.byref(cfg()))==2 and submit(data,1)==2 and info().state==3,'timed-out closing stream cannot be replaced or receive PCM')
                finish()
            else:
                own=False;v=info();conservation(v);check(v.state==4 and not v.worker_live,'completed nonblocking close has no live worker')
            observations['nonblocking_close_result']=rc
            start(cfg());check(info().submitted==0 and info().source_hash==OFFSET,'reopen starts a fresh session without stale samples');finish()
        elif a.case=='stalled_worker':
            first=start(cfg());open_thread=bind(kernel,'OpenThread',P,U,I,U);suspend=bind(kernel,'SuspendThread',U,P);resume=bind(kernel,'ResumeThread',U,P);close_handle=bind(kernel,'CloseHandle',I,P)
            handle=open_thread(2,0,first.worker_thread);check(bool(handle),'test opens only its own actual audio worker for a bounded stall')
            suspended=False
            try:
                check(suspend(handle)==0,'actual owned render worker suspended for underrun test');suspended=True
                # Never call the audio API while its worker may hold the SRW lock.
                time.sleep(.35)
            finally:
                if suspended:check(resume(handle)==1,'same native worker resumed; never terminated')
                check(close_handle(handle)!=0,'temporary OS stress handle closed')
            deadline=time.monotonic()+5
            while True:
                before=info()
                if sample_clock(before)>sample_clock(first)+rate_frames(first,.2):break
                check(time.monotonic()<deadline,'real endpoint clock recovers after suspended-worker underrun');time.sleep(.002)
            data=(F*128)(*([.25,-.5]*64));check(submit(data,64)==0 and drain(5000)==0,'new PCM drains after a real worker stall')
            after=info();conservation(after)
            lag=(before.stream_latency_hns*before.rate+9999999)//10000000
            check(after.last_user_frame>=sample_clock(before)+before.device_buffer_frames+lag and sample_clock(after)>=after.last_user_frame,'drain target covers fresh device time, full buffer and actual maximum stream latency')
            check(after.source_hash==fnv(struct.pack('<ff',.25,-.5)*64) and after.consumed==64,'post-stall original PCM reaches the real render client in order')
            observations.update(before=fields(before),after=fields(after));finish()
        elif a.case=='opening_close':
            outcomes=[]
            for _ in range(5):
                c=cfg(timeout=1);rc=open_(C.byref(c));own=True;check(rc in (0,10),'one-millisecond open observes real completion or bounded timeout')
                stop=close(0);check(stop in (0,10),'closing a possibly opening worker preserves it until a real join')
                if stop==10:
                    check(open_(C.byref(c))==2 and info().state in (3,5),'closing worker cannot publish READY again')
                    finish()
                else:
                    own=False;check(info().state==4 and not info().worker_live,'immediately joined opening worker is actually closed')
                outcomes.append(dict(open=rc,close=stop))
            start(cfg());finish();observations['bounded_open_close_outcomes']=outcomes
        elif a.case=='concurrent_open':
            barrier=threading.Barrier(2);results=[];errors=[]
            def contender():
                try:
                    c=cfg();barrier.wait(timeout=5);rc=open_(C.byref(c));results.append(rc)
                    if rc==0:
                        own_info=Info();assert snapshot(C.byref(own_info))==0 and own_info.state==2
                        assert close(5000)==0
                except BaseException as error:errors.append(repr(error))
            threads=[threading.Thread(target=contender) for _ in range(2)]
            for thread in threads:thread.start()
            for thread in threads:thread.join(10)
            check(not errors and not any(t.is_alive() for t in threads) and sorted(results)==[0,3],'simultaneous real opens acquire one creator, reject the other and retire on the creator')
            check(info().state==4 and not info().worker_live,'simultaneous opening leaves no lost native audio worker');observations['contender_results']=results
    finally:
        if own:
            rc=close(5000)
            if rc:print('cleanup returned',rc,flush=True)
    result=dict(checks=len(labels),labels=labels,observations=observations,case=a.case,muted_test_only=True)
    (d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)

def rate_frames(info,seconds):return int(info.rate*seconds)

if __name__=='__main__':main()
