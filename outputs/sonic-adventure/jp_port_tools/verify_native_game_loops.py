"""Unmodified retail loop points through original game tasks, EOS and decoder trap.

Observe actual SDK records/callbacks; never replace them or shorten the track.
All retail audio and RAM/capture data remain private.
"""
import argparse,ctypes as C,hashlib,json,struct,subprocess,sys,time,wave
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
from rna_test_runtime import Runtime
from verify_native_game_stream import Info as GameInfo
from verify_native_adxt import Info as AdxtInfo
from verify_native_adx_stream_decoder import SJD
from verify_native_adx_joints import Chunk
from verify_native_adx_joints import Ring as OriginalRing
from verify_native_audio import bind,Info as AudioInfo
from verify_native_g2_audio import Info as G2Info
from adx_media_fixtures import Disc
from adx_step_oracle import StereoADX
I,U,Q,P=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p
CASES=('cpu','dma','restart','timed')
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);ap.add_argument('--reference',type=Path);a=ap.parse_args()
    r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256'];d.mkdir()
    if not a.case:
        cases={}
        for name in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','build','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--case',name,'--output',str(d/name),'--reference',str(d/'cpu')]
            run=subprocess.run(cmd,capture_output=True,timeout=420,creationflags=subprocess.CREATE_NO_WINDOW);(d/(name+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-7000:])
            cases[name]=read(d/name/'result.json');print(name,cases[name]['checks'],'checks',flush=True)
        result=dict(status='original_game_retail_loop_trap_EOS_seek_pressure_stop_restart_and_timed_playback_verified',runtime_checks=sum(x['checks'] for x in cases.values()),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),production_code_changed=False,
            retail_data_published=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    labels=[];rt=None;messages=[];obs={};pool=False
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    disc=Disc(a.track.resolve())
    try:data=disc.read('JINGLE_5.ADX',0,disc.files['JINGLE_5.ADX'][1])
    finally:disc.close()
    header=data[:128];offset=struct.unpack_from('>H',data,2)[0]+4
    check(sha_bytes(header)=='3b89dcfb5f688be32c2a2d01dbf9ccf634dacbc3de35d8b19641b2435e24604c','original complete Japanese loop header unchanged')
    meta=dict(total=1194791,start_sample=136352,start_byte=153600,end_sample=1194788,end_byte=1344372,rate=44100)
    try:
        rt=Runtime(b,a.track,check,timed=a.case=='timed');lib=rt.lib;frames=rt.frames;fs=C.CDLL(b['GDFS_DLL'])
        for name in ('jp_adxstm_initialize','jp_adxf_initialize','jp_adxt_initialize'):check(bind(lib,name,I)()==0,'actual shared parent '+name)
        check(bind(fs,'gdFsChangeDir',I,C.c_char_p)(b'/SONICADV')==0,'actual retail source directory')
        ERROR=C.CFUNCTYPE(None,P,P)
        @ERROR
        def error(ctx,msg):messages.append((ctx,C.string_at(msg).decode(errors='replace')))
        rt.keep.append(error);bind(lib,'mwRnaErrEntryFunc',None,P,P)(error,0x123456789)
        bind(frames,'jp_heap_reset',None)();bind(frames,'task_init',None)();pool=True;C.c_int32.in_dll(frames,'event_skipping').value=0
        def taskpass():bind(frames,'task_step_all',None)()
        def info():
            out=GameInfo();check(bind(lib,'jp_game_stream_snapshot',I,P)(C.byref(out))==0,'actual game stream owner/requests');return out
        def owner():
            out=AdxtInfo();check(bind(lib,'jp_adxt_snapshot',I,P)(C.byref(out))==0,'actual ADXT schedule/owned buffers');return out
        def state(h):
            out=(Q*33)();check(bind(lib,'peer_adxt_state',I,P,P)(h,out)==0,'actual original looping ADXT record');return list(out)
        def file_state(h):
            out=(Q*20)();check(bind(lib,'peer_adxstm_state',I,P,P)(h,out)==0,'actual loop reader/EOS callback record');return list(out)
        def action(kind):
            value=I(-1);check(bind(lib,'jp_game_stream_action',I,U,I,P)(kind,0,C.byref(value))==0,'actual original stop/restart control');return value.value
        check(bind(lib,'jp_game_stream_initialize',I)()==0,'schedule actual original game stream task');taskpass();initial=info();check(initial.ready,'actual game task initializes real music/voice owners')
        music=initial.music;index=None
        for i in range(110):
            name=C.create_string_buffer(32);check(bind(lib,'peer_game_stream_music_file',I,U,P,U)(i,name,32)==0,'actual original music filename lookup')
            if name.value==b'JINGLE_5.ADX':index=i
        check(index is not None,'real loop candidate appears in original game music table')
        check(bind(lib,'jp_game_stream_request_music',I,I)(index)==0,'queue actual game music request');taskpass()
        check(bind(lib,'sdMemBlkSetTransferMode',I,I)(int(a.case=='dma'))==0,'selected actual SDK CPU/DMA mode')
        sample_oracle=StereoADX((7334,-3283));expected=[bytearray(),bytearray()];decoded=used=loops=0;cursor=offset;position=0
        output=bytearray();trap_events=[];seeks=[];last_fpos=None;pressure=[];accum=0;restart_done=False;restarted_decoded=0
        deadline=time.monotonic()+300;initial_handles=(initial.task,initial.music,initial.voice);last_server=owner().service_calls;modeled_steps=0
        first_pcm=bytearray();before_dma=G2Info();check(bind(rt.g2,'jp_g2_snapshot',I,P)(C.byref(before_dma))==0,'actual transfer counters before loop playback')
        for iteration in range(30000):
            before_state=state(music);before_sjd=SJD.from_address(before_state[4]);before_dec=before_sjd.decnsmpl
            before_free=OriginalRing.from_address(before_state[9]).free_len
            before_input=bind(lib,'SJRBF_GetNumData',I,P,I)(before_state[8],1) if before_state[8] else 0
            before_service=owner().service_calls
            if a.case=='timed':
                hwnd=bind(rt.device,'jp_device_window',P)();check(bind(rt.present,'jp_wait_window_idle',I,P,U)(hwnd,5)==1,'actual elapsed-time service progresses loop');taskpass()
            else:rt.service()
            value=state(music);sjd=SJD.from_address(value[4]);now=sjd.decnsmpl;at=sjd.decdtlen;lp=value[21]
            if value[5]:
                file=file_state(value[5]);fpos=bind(fs,'gdFsTell',I,P)(file[4])
                if last_fpos is not None and fpos<last_fpos:
                    check(fpos==meta['start_byte']//2048,'actual EOS callback seeks to original loop start sector');seeks.append(dict(decoded=now,from_sector=last_fpos,to_sector=fpos))
                last_fpos=fpos
                if value[1]>=2:
                    check(file[16]==C.cast(lib.adxt_eos_entry,P).value and file[17]==music,'actual original EOS function and native-width player context remain installed')
                    check(sjd.trapfunc==C.cast(lib.adxt_trap_entry,P).value and sjd.trapobj==music,'actual original decoder trap function and native-width context remain installed')
            if now>decoded and a.case!='timed':
                delta=at-used;count=now-decoded;check(delta>0 and delta%36==0 and cursor+delta<=len(data),'whole actual stereo input blocks remain within genuine loop source')
                values=sample_oracle.decode_step(data[cursor:cursor+delta]);nblk=delta//18;left=meta['end_sample']-position
                pad=(-left)%32;predicted=32*nblk//2-(pad if (left+31)//32<=nblk else 0);predicted=min(predicted,meta['total']-position)
                check(count==predicted,'reported samples follow original stereo pad accounting including its preserved block-count quirk')
                for ch,pcm in enumerate(values):
                    chunk=pcm[:count*2];expected[ch]+=chunk;base=value[14]+ch*16448;ring_at=decoded%8192*2;first=min(len(chunk),16384-ring_at)
                    actual=C.string_at(base+ring_at,first)+C.string_at(base,len(chunk)-first)
                    if actual!=chunk:
                        (d/'private_sample_mismatch.json').write_text(json.dumps(dict(iteration=iteration,decoded=decoded,now=now,cursor=cursor,delta=delta,count=count,position=position,loops=lp,state=value,messages=messages),indent=2)+'\n')
                    check(actual==chunk,'every actual loop-decoded sample matches independent binary32 recurrence with continuous history')
                cursor+=delta;position+=count;modeled_steps+=1
            if lp>loops:
                if a.case=='timed':
                    # A native wait may deliver multiple logical audio steps.
                    # Observe the resulting continuation; exact callback-entry
                    # reset values are checked in every offline case.
                    check(lp==loops+1 and meta['start_sample']<=sjd.decpos<meta['end_sample'],'automatic service observes the next original loop within declared source bounds')
                    check(sjd.trapcnt==sjd.decpos-meta['start_sample'] and sjd.trapnsmpl==meta['end_sample']-meta['start_sample'],'post-loop timed sample count uses the actual reset trap origin/span')
                    check(sjd.trapdtlen>=meta['start_byte'] and (sjd.trapdtlen-meta['start_byte'])%36==0,'post-loop timed compressed offset continues on genuine stereo block boundaries')
                else:
                    check(lp==loops+1 and sjd.decpos==meta['start_sample'] and sjd.trapcnt==0 and sjd.trapdtlen==meta['start_byte'],'actual original trap resets decoder position/count/data offset to unchanged loop metadata')
                if a.case!='timed':check(position==meta['end_sample'] and cursor==meta['end_byte'],'independent source cursor and delivered sample position reach original loop end exactly')
                check(len(seeks)>lp-1,'real file EOS seek precedes each actual decoder loop trap')
                trap_events.append(dict(loop=lp,cumulative_samples=now,source_byte_before=cursor,decoded_source_position=sjd.decpos,observed_trap_samples=sjd.trapcnt,observed_trap_bytes=sjd.trapdtlen))
                cursor=meta['start_byte'];position=meta['start_sample'];loops=lp
            decoded,used=now,at
            if a.case!='timed' and owner().service_calls>before_service and before_state[1]==3:
                check(now-before_dec<=before_free//2,'real producer respects actual output free capacity before its original server step')
                if before_free<4480 and before_input>=2520 and now-before_dec<2240:
                    pressure.append(dict(decoded=now,loop=lp,free_bytes=before_free,input_bytes=before_input,produced_samples=now-before_dec))
            check(value[1] in (1,2,3) and not info().errors and not messages,('looping player stays active without recovery/file/transfer errors',value[1],messages))
            if a.case=='restart' and loops==1 and not restart_done:
                before=owner();check(bind(lib,'jp_game_stream_request_music',I,I)(index)==0,'queue same music at actual first loop boundary');taskpass()
                value=state(music);check(value[21]==0 and SJD.from_address(value[4]).decnsmpl==0,'real game restart resets original loop/decoder counters')
                check((info().task,info().music,info().voice)==initial_handles and owner().owned_bytes==before.owned_bytes,'real restart reuses original task/players/native work without allocating another owner')
                decoded=used=loops=0;cursor=offset;position=0;sample_oracle=StereoADX((7334,-3283));expected=[bytearray(),bytearray()];restart_done=True;last_fpos=None
            if a.case=='timed' and loops>=1:break
            if a.case in ('cpu','dma') and loops>=2 and decoded>=meta['end_sample']+(meta['end_sample']-meta['start_sample'])+8192:break
            if a.case=='restart' and restart_done and decoded>=32768:restarted_decoded=decoded;break
            if a.case!='timed':
                output+=rt.render(256);accum+=256
                if accum>=735:taskpass();accum-=735
                time.sleep(.0001)
            if iteration and iteration%3000==0:print(a.case,'frames',len(output)//4,'decoded',decoded,'loops',loops,flush=True)
            check(time.monotonic()<deadline,'actual unchanged retail loop playback makes bounded progress')
        else:raise AssertionError(('loop did not reach required boundary',decoded,loops,messages))
        if a.case in ('cpu','dma'):
            check(len(trap_events)==2 and len(seeks)>=2 and pressure and any(output),'two full original loops observe real output capacity pressure and produce firmware PCM')
            ram=rt.memory(0,2<<20);(d/'private_output.s16le').write_bytes(output);(d/'private_final_RAM.bin').write_bytes(ram)
            if a.case=='dma':check(output==(a.reference/'private_output.s16le').read_bytes() and ram==(a.reference/'private_final_RAM.bin').read_bytes(),'every mixed firmware PCM byte and full sound RAM agree between CPU/DMA loop playback')
            # Share a bounded actual capture spanning the first loop, not an
            # invented waveform or a shortened/re-authored retail loop.
            center=meta['end_sample'];start=max(0,center-44100);end=min(len(output)//4,center+44100)
            wav=d/'native-JP-first-music-loop.wav'
            with wave.open(str(wav),'wb') as f:f.setnchannels(2);f.setsampwidth(2);f.setframerate(44100);f.writeframes(output[start*4:end*4])
            obs.update(PCM_sha256=sha_bytes(output),RAM_sha256=sha_bytes(ram),independent_PCM_sha256=[sha_bytes(x) for x in expected],private_WAV=str(wav),firmware_frames=len(output)//4,actual_output_capacity_pressure=pressure)
        if a.case=='restart':check(restart_done and restarted_decoded>=32768,'real boundary restart decodes a verified fresh prefix')
        if a.case=='timed':
            au=AudioInfo();check(bind(rt.audio,'jp_audio_snapshot',I,P)(C.byref(au))==0 and au.worker_live and au.session_muted,'actual timed loop shares the existing unique muted output worker')
        after_dma=G2Info();check(bind(rt.g2,'jp_g2_snapshot',I,P)(C.byref(after_dma))==0,'actual transfer counters after loop playback');dma_count=after_dma.dma_started-before_dma.dma_started
        check(dma_count>0 if a.case=='dma' else dma_count==0,'selected real CPU/DMA mode persists through original loop path')
        action(0);check(state(music)[1]==0,'actual game stop retires looping playback');check(bind(lib,'jp_game_stream_shutdown',I)()==0,'actual game task/players/partition cleanup after loops')
        check(not info().active and not info().task and not owner().owned_work,'no game stream task/work survives loop cleanup')
        rt.close();bind(frames,'task_destroy_all',None)();bind(frames,'jp_heap_reset',None)();pool=False
        obs.update(original_loop_metadata=meta,header_sha256=sha_bytes(header),full_source_sha256=sha_bytes(data),unmodified_source=True,source_header_bytes=offset,trap_events=trap_events,actual_EOS_seeks=seeks,
            independent_decoder_steps=modeled_steps,actual_DMA_transfers=dma_count,boundary_restart_verified=restart_done,timed_automatic=a.case=='timed',production_code_changed=False)
    finally:
        if rt and rt.live:
            bind(rt.lib,'jp_game_stream_shutdown',I)();rt.close()
        if rt and pool:bind(rt.frames,'task_destroy_all',None)();bind(rt.frames,'jp_heap_reset',None)()
    result=dict(case=a.case,checks=len(labels),assertions=labels,observations=obs);(d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)

def sha_bytes(raw):return hashlib.sha256(raw).hexdigest()
if __name__=='__main__':main()
