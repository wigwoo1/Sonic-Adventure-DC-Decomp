"""Actual original file-ring streaming, ownership, cancellation and retail music flow."""
import argparse,ctypes as C,hashlib,json,struct,subprocess,sys,threading,time,wave
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
from adx_media_fixtures import Disc
from adx_step_oracle import StereoADX
from rna_test_runtime import Runtime,Joints
from verify_native_adx_joints import Chunk
from verify_native_audio import bind,Info as AudioInfo
from verify_native_gdfs import Info as FSInfo
from verify_native_g2_audio import Info as G2Info
I,U,Q,P=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p
CASES=('pool','file','contention_cancel','global_pending','eos_callback','title_cpu','title_dma','timed')
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('initializations','shutdowns','service_calls')]+[(n,U) for n in ('active','stopping','live_streams','pending_reads','status')]+[('native_error',I)]
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())

class Ring:
    def __init__(self,rt,size=8192,extra=36):
        self.rt=rt;self.size=size;self.extra=extra;self.raw=C.create_string_buffer(size+extra+128);C.memset(self.raw,0xa5,len(self.raw))
        self.at=(C.addressof(self.raw)+63)&~31
        self.handle=bind(rt.lib,'SJRBF_Create',P,P,I,I)(self.at,size,extra)
        rt.check(bool(self.handle),'actual caller-owned compressed input ring')
    def count(self,line):return bind(self.rt.lib,'SJRBF_GetNumData',I,P,I)(self.handle,line)
    def take(self,n):
        ck=Chunk();bind(self.rt.lib,'SJRBF_GetChunk',None,P,I,I,P)(self.handle,1,n,C.byref(ck))
        data=C.string_at(ck.data,ck.len) if ck.len else b''
        bind(self.rt.lib,'SJRBF_PutChunk',None,P,I,P)(self.handle,0,C.byref(ck));return data
    def guards(self):self.rt.check(C.string_at(self.at-16,16)==b'\xa5'*16 and C.string_at(self.at+self.size+self.extra,16)==b'\xa5'*16,'entire input ring outer guards')
    def retired(self):self.rt.check(self.count(0)==self.size and self.count(1)==0,'original Stop returns complete input ring before its destruction');self.guards()
    def close(self):bind(self.rt.lib,'SJRBF_Destroy',None,P)(self.handle)

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);ap.add_argument('--reference',type=Path);a=ap.parse_args()
    r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256']
    assert sha(a.build/'external_adx_stms.c')==b['adxstm_generated_source_sha256']
    assert sha(a.build/'sound_owner_adxstm.c')==b['adxstm_owner_generated_source_sha256'];d.mkdir()
    if not a.case:
        cases={}
        for name in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','build','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--case',name,'--reference',str(d/'title_cpu'),'--output',str(d/name)]
            run=subprocess.run(cmd,capture_output=True,timeout=180,creationflags=subprocess.CREATE_NO_WINDOW)
            (d/(name+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-6000:])
            cases[name]=read(d/name/'result.json');print(name,cases[name]['checks'],'checks',flush=True)
        result=dict(status='original_ADXSTM_native_file_refill_cancellation_global_lifetime_and_retail_SJD_RNA_firmware_pipeline_verified',
            runtime_checks=sum(v['checks'] for v in cases.values()),fresh_process_cases=len(cases),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
            fixture_source_sha256=sha(p/'jp_port_tools/rna_test_runtime.py'),oracle_source_sha256=sha(p/'jp_port_tools/adx_step_oracle.py'),
            disc_fixture_source_sha256=sha(p/'jp_port_tools/adx_media_fixtures.py'),full_ADXT_AFS_looping_voice_gameplay_verified=False,
            live_timed_retail_music_verified=False,retail_inputs_RAM_PCM_WAV_private=True,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    labels=[];rt=None;rings=[];group=None;sjd=None;group_closed=False;obs={};errors=[]
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    timed=a.case in ('global_pending','timed')
    try:
        rt=Runtime(b,a.track,check,timed=timed);lib=rt.lib;fs=C.CDLL(b['GDFS_DLL'])
        check(C.sizeof(Info)==48 and bind(lib,'jp_adxstm_initialize',I)()==0,'file streams join actual sound/System owner')
        bind(lib,'ADXERR_Init',None)();ERROR=C.CFUNCTYPE(None,P,P)
        @ERROR
        def error(ctx,msg):errors.append((ctx,C.string_at(msg).decode(errors='replace')))
        rt.keep.append(error);bind(lib,'ADXERR_EntryErrFunc',None,P,P)(error,0x123456789)
        def info():
            out=Info();check(bind(lib,'jp_adxstm_snapshot',I,P)(C.byref(out))==0,'actual original file-stream pool snapshot');return out
        def fi():
            out=FSInfo();check(bind(fs,'jp_gdfs_snapshot',I,P)(C.byref(out))==0,'actual borrowed filesystem snapshot');return out
        def state(handle):
            out=(Q*20)();check(bind(lib,'peer_adxstm_state',I,P,P)(handle,out)==0,'actual original file request and full-width joint state');return list(out)
        def service():check(bind(lib,'jp_adxstm_service',I)()==0,'same sound service polls real file I/O and original stream server')
        def ring():v=Ring(rt);rings.append(v);return v
        def open_(joint,name=b'TITLE.ADX'):
            handle=bind(lib,'ADXSTM_OpenFname',P,C.c_char_p,P)(name,joint.handle)
            check(bool(handle) and handle>0xffffffff,'actual original filename stream constructor');return handle
        check(bind(fs,'gdFsChangeDir',I,C.c_char_p)(b'/SONICADV')==0,'actual filesystem retail directory')
        if a.case=='pool':
            joint=ring();before=fi().live_handles
            # Controlled original stream-pool fixture: no dummy GDFS operations.
            # NULL GDFS is stored only; these 32 records are never started.
            handles=[bind(lib,'ADXSTMF_Create',P,P,I,I,P)(None,0,4096,joint.handle) for _ in range(32)]
            check(all(handles) and info().live_streams==32 and not bind(lib,'ADXSTMF_Create',P,P,I,I,P)(None,0,4096,joint.handle),'actual original32-slot pool exhausts without a shadow registry')
            for name,args,types in [('ADXSTM_OpenFname',(b'TITLE.ADX',joint.handle),(C.c_char_p,P)),
                ('ADXSTM_OpenRange',(45150,1,joint.handle),(I,I,P)),('ADXSTM_OpenFileRange',(b'TITLE.ADX',0,1,joint.handle),(C.c_char_p,I,I,P))]:
                check(not bind(lib,name,P,*types)(*args) and fi().live_handles==before,'failed real constructor closes its actual filename/range handle')
            foreign=[];thread=threading.Thread(target=lambda:foreign.extend([bind(lib,'jp_adxstm_service',I)(),bind(lib,'jp_adxstm_shutdown',I)()]))
            thread.start();thread.join(5);check(not thread.is_alive() and foreign==[3,3] and info().live_streams==32,'foreign caller cannot mutate or retire original file pool')
            check(bind(lib,'jp_adxstm_shutdown',I)()==0 and info().live_streams==0,'actual pool records retire and caller-owned ring stays alive')
            check(bind(lib,'jp_adxstm_initialize',I)()==0,'actual file owner can restart after complete retirement')
            handle=open_(joint);check(state(handle)[19]==120,'file-close adapter uses actual verified native GDFS handle layout')
            bind(lib,'ADXSTM_Close',None,P)(handle);check(info().live_streams==0 and fi().live_handles==before,'successful real constructor and close reuse exhausted slots')
            obs.update(original_stream_pool_slots=32,controlled_unstarted_NULL_GDFS_pool_fixture=True,three_real_constructor_rollback_paths=True)
        elif a.case in ('contention_cancel','global_pending'):
            handles=[]
            for _ in range(2 if a.case=='contention_cancel' else 1):
                joint=ring();handle=open_(joint);handles.append(handle)
                bind(lib,'ADXSTM_SetReqRdSize',I,P,I)(handle,2);check(bind(lib,'ADXSTM_Start',I,P)(handle)==1,'actual file stream starts')
            bind(lib,'ADXSTM_ExecServer',None)()
            check(info().pending_reads==1 and fi().pending and rings[0].count(0)+rings[0].count(1)<rings[0].size,'actual accepted read retains its borrowed free chunk')
            if a.case=='contention_cancel':
                check(state(handles[1])[2]==0 and rings[1].count(0)==rings[1].size and rings[1].count(1)==0,
                    'original GDFS contention rejects competing request and returns its entire borrowed free chunk')
                first=handles[0];before=state(first)
                # Force a real provider rejection through its existing thread
                # guard. The public native owner already rejects foreign callers;
                # this internal raw-call fixture checks Stop's error propagation.
                thread=threading.Thread(target=lambda:bind(lib,'ADXSTM_Stop',None,P)(first));thread.start();thread.join(5)
                after=state(first)
                check(not thread.is_alive() and after[2]==1 and after[1]==before[1] and after[15]==before[15] and rings[0].count(0)+rings[0].count(1)<rings[0].size,
                    'actual provider cancellation rejection preserves request and borrowed ring instead of resetting it')
                check(info().native_error!=0,'real provider cancellation rejection is observable')
                check(bind(lib,'jp_adxstm_shutdown',I)()==0 and not info().live_streams and not fi().pending and not fi().live_handles,
                    'creator retry cancels accepted request before original records retire')
                obs.update(real_competing_request_rejected_and_chunk_returned=True,internal_foreign_raw_Stop_fixture_preserves_pending_chunk=True,actual_creator_close_retry=True)
            else:
                check(bind(rt.g2,'jp_g2_gate_set',I,I)(15)==0,'actual outer callback gate held')
                check(bind(lib,'jp_adxstm_shutdown',I)()==7 and info().pending_reads==1,'blocked close preserves actual file and chunk ownership')
                check(bind(rt.g2,'jp_g2_gate_set',I,I)(0)==0,'outer callback gate released');rt.close()
                check(not info().active and not info().live_streams and not info().pending_reads,'System global shutdown retires files before borrowed filesystem')
                obs.update(actual_pending_read_System_global_cleanup=True)
        elif a.case in ('file','eos_callback','timed'):
            disc=Disc(a.track.resolve())
            try:
                fad,size=disc.files['TITLE.ADX'];expected=disc.sectors(fad,(size+2047)//2048)
            finally:disc.close()
            joint=ring();handle=open_(joint);bind(lib,'ADXSTM_SetReqRdSize',I,P,I)(handle,2)
            received=bytearray();events=[];reentrant=[];EOS=C.CFUNCTYPE(None,P)
            @EOS
            def eos(ctx):events.append(ctx);reentrant.append(bind(lib,'jp_adxstm_shutdown',I)())
            rt.keep.append(eos);bind(lib,'ADXSTM_EntryEosFunc',None,P,P,P)(handle,eos,0x123456789)
            limit=1 if a.case=='eos_callback' else 64 if a.case=='timed' else len(expected)//2048
            bind(lib,'ADXSTM_SetEos',None,P,I)(handle,limit);bind(lib,'ADXSTM_Start',I,P)(handle)
            deadline=time.monotonic()+30;pressure=0;steps=0
            while len(received)<limit*2048:
                steps+=1
                if a.case=='timed':
                    window=bind(rt.device,'jp_device_window',P)();check(bind(rt.present,'jp_wait_window_idle',I,P,U)(window,8)==1,'actual shared wait automatically services file refill')
                else:service()
                if steps%11<3:pressure+=1
                else:
                    chunk=joint.take((7,4093,2049,3001,8192)[steps%5]);at=len(received)
                    check(chunk==expected[at:at+len(chunk)],'every file-ring byte and mirrored chunk matches independent disc sectors');received+=chunk
                joint.guards();check(time.monotonic()<deadline,'continuous real file refill makes bounded progress');time.sleep(.0005)
            check(len(received)==limit*2048 and events==[0x123456789] and reentrant==[4],'actual EOS callback once with full-width context and no reentrant teardown')
            if a.case=='file':check(bind(lib,'ADXSTM_GetStat',I,P)(handle)==3,'actual full retail file reaches READEND without fabricated completion')
            if a.case=='timed':
                audio=AudioInfo();check(info().service_calls>0 and bind(rt.audio,'jp_audio_snapshot',I,P)(C.byref(audio))==0 and audio.worker_live and audio.session_muted,
                    'automatic file service shares existing timed AICA and unique muted output worker')
            bind(lib,'ADXSTM_Close',None,P)(handle);joint.retired()
            check(not info().live_streams and not fi().live_handles and not fi().pending,'completed continuous source closes all actual requests/handles')
            obs.update(retail_file_bytes=size,sector_bytes_compared=len(received),source_sha256=hashlib.sha256(expected[:size]).hexdigest(),compared_sha256=hashlib.sha256(received).hexdigest(),
                independent_full_file_compared=a.case=='file',held_output_pressure_passes=pressure,actual_EOS_callbacks=len(events),timed_automatic_service=a.case=='timed')
        else:
            disc=Disc(a.track.resolve());frames=88192
            try:
                header=disc.read('TITLE.ADX',0,64);offset=struct.unpack_from('>H',header,2)[0]+4;payload=disc.read('TITLE.ADX',offset,frames//32*36)
                check(header[4:8]==bytes((3,18,4,2)) and struct.unpack_from('>I',header,8)[0]==44100,'actual unmodified retail stereo music source')
            finally:disc.close()
            joint=ring();handle=open_(joint);bind(lib,'ADXSTM_SetReqRdSize',I,P,I)(handle,1)
            # Real file prebuffering before decoder/play start. No AICA time is
            # advanced during this I/O wait, and no replacement reader is used.
            bind(lib,'ADXSTM_Start',I,P)(handle);prebuffer_deadline=time.monotonic()+8;prebuffer_passes=0
            while joint.count(1)<joint.size:
                service();prebuffer_passes+=1;check(time.monotonic()<prebuffer_deadline,'real file ring prebuffers through original ADXSTM');time.sleep(.0005)
            check(joint.count(1)==joint.size,'real first input ring full before playback')
            before=G2Info();check(bind(rt.g2,'jp_g2_snapshot',I,P)(C.byref(before))==0,'actual sound-transfer counters before stream creation')
            check(bind(lib,'sdMemBlkSetTransferMode',I,I)(int(a.case=='title_dma'))==0,'explicit actual sound SDK CPU/DMA mode')
            group=Joints(rt,2,1024);outs=(P*2)(*group.handles);bind(lib,'ADXSJD_Init',None)()
            sjd=bind(lib,'ADXSJD_Create',P,P,I,P)(joint.handle,2,outs);check(bool(sjd),'actual decoder consumes continuously refilled file ring')
            renderer=rt.create(group.handles);check(bool(renderer),'actual RNA consumes same decoded channel rings')
            bind(lib,'mwRnaSetStartSmpl',None,P,I)(renderer,1024);bind(lib,'mwRnaSetOutVol',None,P,I)(renderer,0)
            bind(lib,'ADXSJD_Start',None,P)(sjd);bind(lib,'ADXSJD_SetMaxDecSmpl',None,P,I)(sjd,256);bind(lib,'ADXSJD_SetTrapNumSmpl',None,P,I)(sjd,frames)
            bind(lib,'mwRnaStart',None,P)(renderer)
            oracle=StereoADX((7334,-3283));expected=[bytearray(),bytearray()];ring_model=[bytearray(rt.memory(v,16384)) for v in rt.state(renderer)[16:18]]
            decoded=used=sent=0;last_copy=None;flushing=False;output=bytearray();wraps=0;steps=[];file_retired=False;deadline=time.monotonic()+45
            for iteration in range(5000):
                if decoded<frames:
                    bind(lib,'ADXSJD_ExecServer',None)();now=bind(lib,'ADXSJD_GetDecNumSmpl',I,P)(sjd);last=bind(lib,'ADXSJD_GetDecDtLen',I,P)(sjd)
                    check(decoded<=now<=frames and used<=last<=len(payload),'real file-fed decoder stays within explicit prefix trap without header rewrite')
                    if last>used:
                        values=oracle.decode_step(payload[used:last]);count=now-decoded;check(len(values[0])==len(values[1])==count*2,'actual decoder step follows independent compressed-block recurrence')
                        for ch in range(2):expected[ch]+=values[ch]
                        steps.append(count);decoded=now;used=last
                    if decoded==frames:
                        bind(lib,'ADXSJD_Stop',None,P)(sjd);bind(lib,'ADXSJD_Destroy',None,P)(sjd);sjd=None
                        bind(lib,'ADXSTM_Close',None,P)(handle);file_retired=True
                        check(not info().live_streams and not fi().pending and not fi().live_handles,'real file read cancels/closes after decoder stop and before RNA drain')
                        joint.retired()
                service();value=rt.state(renderer);signature=(value[12],value[13],value[19],value[23])
                if value[5] and value[23]>0 and signature!=last_copy and not flushing:
                    if value[19]!=sent%8192 or sent+value[23]>decoded:
                        (d/'copy_order_diagnostic.json').write_text(json.dumps(dict(iteration=iteration,decoded=decoded,sent=sent,used=used,
                            state=value,last_copy=last_copy,signature=signature,input_free=joint.count(0),input_data=joint.count(1),
                            output_free=[group.count(ch,0) for ch in range(2)],output_data=[group.count(ch,1) for ch in range(2)],
                            file_state=None if file_retired else state(handle),decoder_steps=steps,errors=errors),indent=2)+'\n')
                    count=value[23];check(value[19]==sent%8192 and sent+count<=decoded,'actual sound copy follows independent decoded order')
                    for ch in range(2):
                        chunk=bytes(expected[ch][sent*2:(sent+count)*2]);check(C.string_at(value[12+ch],count*2)==chunk,'every actual file-decoded PCM sample matches independent recurrence')
                        at=sent%8192*2;ring_model[ch][at:at+count*2]=chunk
                        check(rt.memory(value[16+ch],16384)==bytes(ring_model[ch]),'entire AICA ring and untouched bytes match independent writes')
                    if sent//8192!=(sent+count)//8192:wraps+=1
                    sent+=count;last_copy=signature
                group.guards();joint.guards()
                if sent==frames and not flushing:bind(lib,'mwRnaFlush',None,P)(renderer);flushing=True
                # Offline decoder steps can be shortened at input-ring wraps.
                # Render a smaller quantum so actual producer backpressure,
                # rather than a false one-step==256-samples assumption, governs
                # the flow. Real elapsed-time ADXT scheduling remains separate.
                output+=rt.render(128);check(time.monotonic()<deadline,'real file-decoder-sound pipeline makes bounded progress')
                if flushing and not bind(lib,'mwRnaGetPlaySw',I,P)(renderer) and not bind(lib,'mwRnaGetNumData',I,P)(renderer):break
                time.sleep(.0001)
            else:raise AssertionError('real continuously streamed music did not finish the explicit prefix')
            check(file_retired and decoded==sent==frames and used==len(payload) and wraps>=2 and any(output),'complete bounded retail prefix streams, wraps and produces nonzero real firmware PCM')
            final=rt.memory(0,2<<20);(d/'private_final_RAM.bin').write_bytes(final);(d/'private_output.s16le').write_bytes(output)
            after=G2Info();check(bind(rt.g2,'jp_g2_snapshot',I,P)(C.byref(after))==0,'actual sound-transfer counters after continuous playback')
            dma_count=after.dma_started-before.dma_started
            check(dma_count>0 if a.case=='title_dma' else dma_count==0,'selected transfer mode performs real DMA or exclusively CPU copies')
            if a.case=='title_dma':check(output==(a.reference/'private_output.s16le').read_bytes() and final==(a.reference/'private_final_RAM.bin').read_bytes(),
                'all mixed firmware PCM bytes and full2MiB sound RAM agree across CPU/DMA continuous file pipelines')
            wav=d/'native-JP-title-ADXSTM-SJD-RNA-AICA-demo.wav'
            with wave.open(str(wav),'wb') as f:f.setnchannels(2);f.setsampwidth(2);f.setframerate(44100);f.writeframes(output)
            check(bind(lib,'jp_rna_shutdown',I)()==0,'actual RNA retires after file/decoder without losing borrowed channel chunks');group.retired();group.close();group_closed=True
            obs.update(retail_prefix_frames=frames,firmware_output_frames=len(output)//4,decoder_steps=steps,ring_wraps=wraps,
                actual_input_ring_bytes=joint.size,continuously_refilled_original_ADXSTM=True,header_rewritten=False,full_song_completed=False,
                prebuffer_service_passes=prebuffer_passes,offline_render_quantum=128,new_actual_DMA_transfers=dma_count,
                PCM_sha256=hashlib.sha256(output).hexdigest(),RAM_sha256=hashlib.sha256(final).hexdigest(),private_demo_WAV=str(wav),private_demo_WAV_sha256=sha(wav))
        check(not errors,('valid stream operations do not emit original middleware errors',errors))
        if rt.live:
            check(bind(lib,'jp_adxstm_shutdown',I)()==0,'native file owner retires without a second lifetime')
            check(not info().active and not info().live_streams and not info().pending_reads,'no original file-stream record survives owner retirement')
        for joint in rings:joint.retired();joint.close()
        if rt.live:rt.close()
    finally:
        if rt and rt.live:
            if sjd:bind(rt.lib,'ADXSJD_Stop',None,P)(sjd);bind(rt.lib,'ADXSJD_Destroy',None,P)(sjd)
            bind(rt.g2,'jp_g2_gate_set',I,I)(0);bind(rt.lib,'jp_adxstm_shutdown',I)();bind(rt.lib,'jp_rna_shutdown',I)()
            if group and not group_closed:group.close()
            rt.close()
    result=dict(case=a.case,checks=len(labels),assertions=labels,observations=obs)
    (d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)

if __name__=='__main__':main()
