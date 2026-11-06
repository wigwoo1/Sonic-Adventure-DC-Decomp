"""Feed the verified native ADX demo to real session-muted WASAPI output."""
import argparse,ctypes as C,hashlib,json,struct,time,wave
from pathlib import Path
from verify_native_audio import bind,Config,Info,fnv,fields,sample_clock
I=C.c_int32;U=C.c_uint32;P=C.c_void_p
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','media','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json');media=read(a.media/'verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    assert media['build_verification_sha256']==sha(a.build/'build_verification.json')
    wav=Path(media['private_decoder_demo_WAV']);assert sha(wav)==media['private_decoder_demo_WAV_sha256']
    prior=read(r/'outputs/sonic-adventure/analysis/jp_native_sound_math_package.json');dll=a.build/'jp_native_audio.dll'
    assert sha(dll)==prior['unchanged_prior_DLLs']['jp_native_audio.dll']['sha256']
    with wave.open(str(wav),'rb') as source:
        rate=source.getframerate();frames=source.getnframes();assert source.getnchannels()==2 and source.getsampwidth()==2 and rate in (44100,48000)
        pcm=source.readframes(frames)
    shorts=struct.unpack('<'+'h'*(len(pcm)//2),pcm);float_bytes=struct.pack('<'+'f'*len(shorts),*[x/32768 for x in shorts]);expected=fnv(float_bytes)
    data=C.create_string_buffer(float_bytes);lib=C.CDLL(str(dll));open_=bind(lib,'jp_audio_open',I,P);submit=bind(lib,'jp_audio_submit',I,P,U);drain=bind(lib,'jp_audio_drain',I,U);close=bind(lib,'jp_audio_close',I,U);snapshot=bind(lib,'jp_audio_snapshot',I,P)
    checks=0;pressure=0;own=False
    def check(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    def info():v=Info();check(snapshot(C.byref(v))==0,'actual output owner snapshot');return v
    start=time.perf_counter()
    try:
        cfg=Config(C.sizeof(Config),rate,16384,40,5000,1,1,0);check(open_(C.byref(cfg))==0,'real WASAPI session and independent COM/render worker opened');own=True
        first=info();check(first.session_muted==1 and first.worker_live and first.creator_thread!=first.worker_thread,'unique muted session and actual separate output worker')
        at=0
        while at<frames:
            count=min(1024,frames-at);rc=submit(C.addressof(data)+at*8,count)
            if rc==11:
                pressure+=1;check(time.perf_counter()-start<10,'actual output pressure bounded without dropping PCM');time.sleep(.002);continue
            check(rc==0,'actual decoded PCM accepted in order');at+=count
        check(drain(5000)==0,'decoded music passes actual stream clock and conservative device latency')
        final=info();check(final.submitted==final.consumed==frames and final.queued_frames==final.discarded==0,'all native ADX demo frames delivered exactly once')
        check(final.source_hash==expected and not final.clipped_samples,'actual render-client float PCM matches every verified decoder sample')
        check(sample_clock(final)>=final.last_user_frame,'real output clock passes the last scheduled user frame')
        check(close(5000)==0,'actual output worker joined');own=False;retired=info();check(retired.state==4 and not retired.worker_live,'no output worker/context remains live')
    finally:
        if own:assert close(5000)==0
    result=dict(status='verified_native_ADX_music_PCM_actual_WASAPI_order_clock_drain_and_worker_cleanup_verified',runtime_checks=checks,
        frames=frames,rate=rate,actual_queue_pressure_observations=pressure,actual_delivered_float_FNV=expected,actual_output=fields(final),
        elapsed_seconds=time.perf_counter()-start,build_verification_sha256=sha(a.build/'build_verification.json'),media_proof_sha256=sha(a.media/'verification.json'),
        verifier_sha256=sha(__file__),audio_DLL_sha256=sha(dll),private_decoder_WAV_sha256=sha(wav),
        only_unique_session_muted=True,endpoint_volume_changed=False,audio_capture_or_acoustic_speaker_output_verified=False,
        GDFS_live_ADXT_music_voice_stream_scheduler_or_full_gameplay_verified=False,retail_media_published=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks;',frames,'actual frames',flush=True)
if __name__=='__main__':main()
