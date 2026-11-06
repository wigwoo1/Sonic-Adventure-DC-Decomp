"""Unmodified Japanese title prefix through actual SJD/RNA/SDK/AICA, with exact PCM order."""
import argparse,ctypes as C,hashlib,json,struct,subprocess,sys,time,wave
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
from adx_media_fixtures import Disc
from adx_step_oracle import StereoADX
from rna_test_runtime import Runtime,Joints
from verify_native_audio import bind
from verify_native_g2_audio import Info as G2Info
from verify_native_gdfs import Info as FSInfo
I,U,P,H=C.c_int32,C.c_uint32,C.c_void_p,C.c_int16

def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--frames',type=int,default=88192);ap.add_argument('--case',choices=('cpu','dma'));ap.add_argument('--reference',type=Path)
    ap.add_argument('--input-source',choices=('fixture','gdfs','gdfs_async'),default='gdfs_async')
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and a.frames>0 and a.frames%32==0
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256'];d.mkdir()
    if not a.case:
        cases={}
        for name in ('cpu','dma'):
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','build','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--frames',str(a.frames),'--case',name,'--input-source',a.input_source,'--reference',str(d/'cpu'),'--output',str(d/name)]
            run=subprocess.run(cmd,capture_output=True,timeout=180,creationflags=subprocess.CREATE_NO_WINDOW)
            (d/(name+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-5000:])
            cases[name]=read(d/name/'result.json');print(name,cases[name]['checks'],'checks',flush=True)
        result=dict(status='unaltered_Japanese_title_prefix_SJD_RNA_SDK_actual_firmware_PCM_CPU_DMA_and_cleanup_verified',
            runtime_checks=sum(v['checks'] for v in cases.values()),fresh_process_cases=2,cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
            fixture_source_sha256=sha(p/'jp_port_tools/rna_test_runtime.py'),oracle_source_sha256=sha(p/'jp_port_tools/adx_step_oracle.py'),
            bit_arithmetic_source_sha256=sha(p/'sh4_binary32.py'),disc_fixture_source_sha256=sha(p/'jp_port_tools/adx_media_fixtures.py'),
            compressed_input_source=a.input_source,actual_original_GDFS_input_verified=a.input_source!='fixture',
            retail_header_rewritten=False,synthetic_end_marker_added=False,production_GDFS_ADXT_AFS_game_scheduler_verified=False,
            full_song_or_gameplay_verified=False,retail_data_and_WAV_captures_publishable=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    checks=[];rt=None;group=None;sjd=None;sji=None;closed=False;steps=[];errors=[]
    def check(ok,label):assert ok,(a.case,label);checks.append(label)
    disc=Disc(a.track.resolve())
    try:
        header=disc.read('TITLE.ADX',0,64);offset=struct.unpack_from('>H',header,2)[0]+4
        rate,total=struct.unpack_from('>II',header,8);cutoff=struct.unpack_from('>H',header,16)[0]
        check(header[:2]==b'\x80\0' and header[4:8]==bytes((3,18,4,2)) and rate==44100 and cutoff==500 and total>a.frames,
            'actual unchanged Japanese stereo ADX header and longer declared song')
        payload_len=a.frames//32*36;packet=disc.read('TITLE.ADX',0,offset+payload_len);payload=packet[offset:]
        check(packet[:64]==header and len(payload)==payload_len,'unaltered file prefix includes original header with no synthetic end marker')
        capacity=((len(packet)+2047)//2048)*2048 if a.input_source!='fixture' else len(packet)
        rounded_packet=disc.read('TITLE.ADX',0,capacity)
    finally:disc.close()
    source=C.create_string_buffer(capacity+128);C.memset(source,0xa5,len(source));source_at=(C.addressof(source)+63)&~31
    if a.input_source=='fixture':C.memmove(source_at,packet,len(packet))
    fs_observations={}
    try:
        rt=Runtime(b,a.track,check);lib=rt.lib
        if a.input_source!='fixture':
            fs=C.CDLL(b['GDFS_DLL']);info0=FSInfo();info1=FSInfo()
            check(bind(fs,'jp_gdfs_snapshot',I,P)(C.byref(info0))==0 and info0.phase==3 and not info0.pending and not info0.live_handles,
                'actual System-owned filesystem is live without another media owner')
            check(bind(fs,'gdFsChangeDir',I,C.c_char_p)(b'/SONICADV')==0,'actual original filesystem selects retail game directory')
            handle=bind(fs,'gdFsOpen',P,C.c_char_p,P)(b'TITLE.ADX',None);check(bool(handle) and handle>0xffffffff,'original GDFS opens actual title file with full native handle')
            callbacks=[];CALLBACK=C.CFUNCTYPE(None,P)
            @CALLBACK
            def completed(ctx):callbacks.append(ctx)
            rt.keep.append(completed)
            try:
                size=I();check(bind(fs,'gdFsGetFileSize',I,P,P)(handle,C.byref(size))==1 and size.value==disc.files['TITLE.ADX'][1],
                    'native GDFS metadata agrees with independent read-only ISO parser')
                check(bind(fs,'gdFsSetTmode',I,P,I)(handle,int(a.case=='dma'))==0,'explicit native file-transfer mode')
                if a.input_source=='gdfs_async':
                    bind(fs,'gdFsEntryRdEndFunc',None,P,P,P)(handle,completed,0x123456789)
                    check(bind(fs,'gdFsReqRd32',I,P,I,P)(handle,capacity//2048,source_at)==capacity//2048,'real original nonblocking title read requested')
                    deadline=time.monotonic()+8;passes=0
                    while bind(fs,'gdFsGetStat',I,P)(handle) in (2,3,4):
                        bind(fs,'gdFsExecServer',None)();passes+=1
                        check(time.monotonic()<deadline,'actual asynchronous filesystem progress bounded')
                        time.sleep(.001)
                    check(bind(fs,'gdFsGetStat',I,P)(handle)==1 and callbacks==[0x123456789],
                        'actual title read completes and retires borrowed full-width callback context')
                    fs_observations.update(actual_server_passes=passes,read_end_callbacks=len(callbacks))
                else:check(bind(fs,'gdFsRead',I,P,I,P)(handle,capacity//2048,source_at)==0,'actual original blocking title read completed')
                check(C.string_at(source_at,capacity)==rounded_packet,'every native file-read byte including sector padding equals independent disc input')
            finally:bind(fs,'gdFsClose',None,P)(handle)
            check(bind(fs,'jp_gdfs_snapshot',I,P)(C.byref(info1))==0 and not info1.pending and not info1.live_handles and info1.read_bytes>info0.read_bytes and info1.requests>info0.requests,
                'original GDFS read and close leave no pending request or borrowed file handle')
            fs_observations.update(read_bytes=info1.read_bytes-info0.read_bytes,requests=info1.requests-info0.requests,native_input_bytes=capacity)
        bind(lib,'SJMEM_Init',None)();bind(lib,'ADXSJD_Init',None)();bind(lib,'ADXERR_Init',None)()
        ERROR=C.CFUNCTYPE(None,P,P)
        @ERROR
        def error(ctx,msg):errors.append((ctx,C.string_at(msg).decode(errors='replace')))
        rt.keep.append(error);bind(lib,'ADXERR_EntryErrFunc',None,P,P)(error,0x123456789);bind(lib,'mwRnaErrEntryFunc',None,P,P)(error,0x123456789)
        group=Joints(rt,2,1024);outs=(P*2)(*group.handles)
        sji=bind(lib,'SJMEM_Create',P,P,I)(source_at,len(packet));sjd=bind(lib,'ADXSJD_Create',P,P,I,P)(sji,2,outs)
        check(bool(sji) and bool(sjd) and sji>0xffffffff and sjd>0xffffffff,'real native memory joint and SJD/basic/expander records')
        renderer=rt.create(group.handles);check(bool(renderer),'same stereo joints feed actual middleware PCM ports')
        check(bind(lib,'sdMemBlkSetTransferMode',I,I)(int(a.case=='dma'))==0,'explicit actual CPU or DMA SDK mode')
        bind(lib,'mwRnaSetStartSmpl',None,P,I)(renderer,1024);bind(lib,'mwRnaSetSfreq',None,P,I)(renderer,rate);bind(lib,'mwRnaSetOutVol',None,P,I)(renderer,0)
        bind(lib,'ADXSJD_Start',None,P)(sjd);bind(lib,'ADXSJD_SetMaxDecSmpl',None,P,I)(sjd,256);bind(lib,'mwRnaStart',None,P)(renderer)
        coef=(H*2)();bind(lib,'ADX_GetCoefficient',None,I,I,P,P)(cutoff,rate,coef,C.byref(coef,2))
        check(list(coef)==[7334,-3283],'real retail header coefficients match independently established constants')
        oracle=StereoADX((7334,-3283));expected=[bytearray(),bytearray()];ring=[bytearray(rt.memory(v,16384)) for v in rt.state(renderer)[16:18]]
        before=G2Info();check(bind(rt.g2,'jp_g2_snapshot',I,P)(C.byref(before))==0,'real DMA counters before title playback')
        decoded=0;last_bytes=0;sent=0;last_copy=None;flushing=False;output=bytearray();wraps=0
        for iteration in range(2500):
            if decoded<a.frames:
                bind(lib,'ADXSJD_ExecServer',None)()
                now=bind(lib,'ADXSJD_GetDecNumSmpl',I,P)(sjd);used=bind(lib,'ADXSJD_GetDecDtLen',I,P)(sjd)
                check(decoded<=now<=a.frames and last_bytes<=used<=payload_len,'actual decoder counters remain within supplied real prefix')
                if used>last_bytes:
                    step=oracle.decode_step(payload[last_bytes:used]);count=now-decoded
                    check(len(step[0])==len(step[1])==count*2 and count>0,'each actual decoder step matches independent compressed-block accounting')
                    for ch in range(2):expected[ch]+=step[ch]
                    steps.append(dict(frames=count,bytes=used-last_bytes));decoded=now;last_bytes=used
                if decoded==a.frames:
                    check(bind(lib,'ADXSJD_GetStat',I,P)(sjd)==2 and bind(lib,'ADXSJD_GetTotalNumSmpl',I,P)(sjd)==total,
                        'prefix exhaustion does not pretend original longer song reached DECEND')
                    check(bind(lib,'SJMEM_GetNumData',I,P,I)(sji,1)==0,'all supplied real compressed bytes consumed')
                    bind(lib,'ADXSJD_Stop',None,P)(sjd);bind(lib,'ADXSJD_Destroy',None,P)(sjd);sjd=None
            rt.service();state=rt.state(renderer);signature=(state[12],state[13],state[19],state[23])
            if state[5] and state[23]>0 and signature!=last_copy and not flushing:
                count=state[23];check(state[19]==sent%8192 and sent+count<=decoded,'actual copy position follows independently decoded prefix order')
                for ch in range(2):
                    chunk=bytes(expected[ch][sent*2:(sent+count)*2]);check(C.string_at(state[12+ch],count*2)==chunk,
                        'every held decoder PCM sample matches independent step-aware recurrence')
                    at=sent%8192*2;ring[ch][at:at+count*2]=chunk
                    check(rt.memory(state[16+ch],16384)==bytes(ring[ch]),'every sound-ring byte and untouched byte matches independent writes')
                if sent//8192!=(sent+count)//8192:wraps+=1
                sent+=count;last_copy=signature
            group.guards()
            if sent==a.frames and not flushing:bind(lib,'mwRnaFlush',None,P)(renderer);flushing=True
            output+=rt.render(256)
            if flushing and not bind(lib,'mwRnaGetPlaySw',I,P)(renderer) and not bind(lib,'mwRnaGetNumData',I,P)(renderer):break
        else:raise AssertionError('actual retail prefix failed to decode/copy/drain within bounded service passes')
        check(decoded==sent==a.frames and last_bytes==len(payload) and wraps>=2,'entire declared test prefix decodes, wraps, copies and drains')
        check(not errors,('actual retail playback emits no middleware or decoder errors',errors))
        check(any(output),'actual supplied sound firmware produces nonzero title-music PCM')
        final=rt.memory(0,2<<20);(d/'private_final_RAM.bin').write_bytes(final);(d/'private_output.s16le').write_bytes(output)
        after=G2Info();check(bind(rt.g2,'jp_g2_snapshot',I,P)(C.byref(after))==0,'actual DMA counters after title playback')
        delta=after.dma_started-before.dma_started;check(delta>0 if a.case=='dma' else delta==0,'selected real transfer mode exercised without hidden mode substitution')
        if a.case=='dma':check(final==(a.reference/'private_final_RAM.bin').read_bytes() and output==(a.reference/'private_output.s16le').read_bytes(),
            'every firmware-mixed PCM byte and all2MiB driver RAM match CPU and DMA title pipelines')
        wav=d/'native-JP-title-SJD-RNA-AICA-demo.wav'
        with wave.open(str(wav),'wb') as f:f.setnchannels(2);f.setsampwidth(2);f.setframerate(rate);f.writeframes(output)
        check(bind(lib,'jp_rna_shutdown',I)()==0,'actual RNA returns borrowed decoder chunks while SDK/G2 remain live')
        group.retired();group.close();closed=True
        bind(lib,'SJMEM_Destroy',None,P)(sji);sji=None;bind(lib,'ADXERR_Finish',None)();rt.close()
        check(C.string_at(source_at,capacity)==rounded_packet and C.string_at(source_at-16,16)==b'\xa5'*16 and C.string_at(source_at+capacity,16)==b'\xa5'*16,
            'entire retail input prefix and surrounding guards remain unchanged through cleanup')
        result=dict(case=a.case,checks=len(checks),assertions=checks,source_name='TITLE.ADX',source_file_bytes=disc.files['TITLE.ADX'][1],
            original_track_sha256=disc.sha256,original_header_sha256=hashlib.sha256(header).hexdigest(),header_bytes=offset,
            unmodified_prefix_sha256=hashlib.sha256(packet).hexdigest(),compressed_payload_bytes=len(payload),original_declared_song_frames=total,
            prefix_frames=a.frames,rate=rate,decoder_steps=steps,ring_wraps=wraps,actual_new_DMA_transfers=delta,actual_firmware_rendered_frames=len(output)//4,
            independently_decoded_channel_sha256=[hashlib.sha256(x).hexdigest() for x in expected],PCM_sha256=hashlib.sha256(output).hexdigest(),
            final_RAM_sha256=hashlib.sha256(final).hexdigest(),private_demo_WAV=str(wav),private_demo_WAV_sha256=sha(wav),
            compressed_input_source=a.input_source,actual_GDFS_observations=fs_observations,
            retail_header_rewritten=False,synthetic_end_marker_added=False,second_output_client_created=False,full_song_completed=False)
        (d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(checks),'checks;',a.frames,'actual title frames through SJD/RNA/AICA',flush=True)
    finally:
        if rt and rt.live:
            if sjd:bind(rt.lib,'ADXSJD_Stop',None,P)(sjd);bind(rt.lib,'ADXSJD_Destroy',None,P)(sjd)
            bind(rt.lib,'jp_rna_shutdown',I)()
            if group and not closed:group.close()
            if sji:bind(rt.lib,'SJMEM_Destroy',None,P)(sji)
            bind(rt.lib,'ADXERR_Finish',None)();rt.close()

if __name__=='__main__':main()
