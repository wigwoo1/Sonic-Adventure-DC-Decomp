"""Original middleware copies, real CPU/DMA playback, shared clock and lifetime."""
import argparse,ctypes as C,hashlib,json,struct,subprocess,sys,threading,time
from pathlib import Path
from verify_native_audio import bind,Info as AudioInfo
from verify_native_aica import Options as AicaOptions
from verify_native_g2_audio import Info as G2Info
from rna_test_runtime import Runtime,Joints,Info
I,U,Q,P,H=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p,C.c_int16
CASES=('clock','pool','copy_cpu','copy_dma','cleanup_pending','callback','timed60','timed120','timed240')
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())
def wave(ch,start,count):return struct.pack('<'+'h'*count,*[(((i*(37 if ch==0 else 53))%24001)-12000) for i in range(start,start+count)])

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);ap.add_argument('--reference',type=Path);a=ap.parse_args()
    r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256']
    for n,h in b['rna_generated_sources_sha256'].items():assert sha(a.build/n)==h,n
    assert sha(a.build/'sound_owner_rna.c')==b['rna_owner_generated_source_sha256']
    if not a.case:
        cases={}
        for name in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','build','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--output',str(d/name),'--case',name,'--reference',str(d/'copy_cpu')]
            run=subprocess.run(cmd,capture_output=True,timeout=100,creationflags=subprocess.CREATE_NO_WINDOW);(d/(name+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace'))
            cases[name]=read(d/name/'result.json');print(name,cases[name]['checks'],'checks',flush=True)
        result=dict(status='original_RNA_mwSnd_real_copy_playback_shared_clock_service_and_ordered_lifetimes_verified',
            runtime_checks=sum(x['checks'] for x in cases.values()),cases=cases,fresh_process_cases=len(cases),
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
            fixture_source_sha256=sha(Path(__file__).parent/'rna_test_runtime.py'),production_game_ADXT_GDFS_AFS_voice_streaming_or_gameplay_verified=False,
            retail_inputs_and_driver_RAM_PCM_captures_private=True,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    labels=[];observations={};rt=None;groups=[]
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    if a.case=='clock':
        frame=C.CDLL(b['frames_DLL']);lib=C.CDLL(b['game_sound_DLL']);core=C.CDLL(str(a.build/'jp_native_aica.dll'));g2=C.CDLL(str(a.build/'jp_native_g2_audio.dll'))
        count=bind(lib,'syTmrGetCount',U);error=bind(lib,'jp_rna_clock_error',I);begin=bind(lib,'jp_rna_clock_begin',I)
        check(count()==0 and error()!=0,'cold SDK timer reports missing real transport without inventing a clock')
        SOURCE=C.CFUNCTYPE(I,P,C.POINTER(Q));tick=[0];healthy=[True]
        @SOURCE
        def source(ctx,out):out[0]=tick[0];return int(healthy[0])
        init=bind(frame,'jp_clock_initialize',I,P,P,Q);wall=bind(frame,'jp_clock_wall_counts',Q)
        check(init(source,None,781250)==0,'actual shared clock with deterministic count fixture')
        check(bind(core,'jp_aica_open',I,P)(C.byref(AicaOptions(32,0,16384,40,5000,1,1,0)))==0 and bind(g2,'jp_g2_open',I)()==0,'real AICA/G2 owner for SDK count lifetime')
        try:
            check(begin()==0 and count()==0,'new valid lifetime explicitly clears cold timer error')
            for value in (1,17,(1<<32)-1,1<<32,(1<<32)+17,(1<<33)+257):
                tick[0]=value;check(count()==value&0xffffffff and wall()==value,'native low32 wrap uses the same64-bit frame time domain')
            last=count();tick[0]-=1;check(count()==last and error()!=0,'backwards real source freezes count and exposes sticky failure')
            check(begin()!=0,'cannot clear a failed borrowed clock without restoring that clock')
            check(init(source,None,10000000)==0 and begin()==0,'explicit shared-clock recovery')
            anchor=tick[0]
            for delta in (1,13,101,1000,10000001):
                tick[0]=anchor+delta;check(count()==delta*781250//10000000,'fractional time conversion preserves existing clock arithmetic')
            last=count();healthy[0]=False;check(count()==last and error()!=0,'provider failure retains count and sticky status')
        finally:
            check(bind(g2,'jp_g2_close',I)()==0 and bind(core,'jp_aica_close',I,U)(0)==0,'actual test transport/core retire')
            bind(frame,'jp_clock_shutdown',None)()
        observations.update(existing_clock_rate=781250,independent_timer_created=False,wrap_fraction_failure_and_recovery=True)
    else:
        timed=a.case.startswith('timed') or a.case in ('cleanup_pending',)
        try:
            rt=Runtime(b,a.track,check,timed=timed);lib=rt.lib
            check(C.sizeof(Info)==48 and rt.info().active==1,'native RNA owner ABI and actual activation')
            ERROR=C.CFUNCTYPE(None,P,P);messages=[];reentrant=[];diagnostics=[];current=[None]
            @ERROR
            def error(ctx,msg):
                messages.append((ctx,C.string_at(msg)))
                item=dict(message=C.string_at(msg).decode(),SDK_error=rt.peer('peer_qsound_error',2))
                if current[0]:
                    item['RNA_state']=rt.state(current[0]);snd=item['RNA_state'][22]
                    item['blocks']=[]
                    for ch in range(2):
                        mh=P.from_address(snd+32+ch*8).value;record=(Q*8)()
                        if mh and bind(lib,'peer_record',I,P,P)(mh,record)==0:item['blocks'].append(list(record))
                    transport=G2Info()
                    if bind(rt.g2,'jp_g2_snapshot',I,P)(C.byref(transport))==0:item['G2']={name:getattr(transport,name) for name,_ in transport._fields_}
                diagnostics.append(item)
                if a.case=='callback':reentrant.append(bind(lib,'jp_rna_shutdown',I)())
            rt.keep.append(error);context=0x123456789
            bind(lib,'mwRnaErrEntryFunc',None,P,P)(error,context)
            if a.case=='pool':
                before=rt.peer('peer_observe',20)[4];handles=[]
                for i in range(5):
                    group=Joints(rt,1);groups.append(group);handle=rt.create(group.handles)
                    if i<4:check(bool(handle) and handle>0xffffffff,'actual middleware renderer uses native pointer');handles.append(handle)
                    else:check(not handle,'fifth mono renderer fails actual four-port capacity')
                check(rt.info().live_renderers==4 and bind(lib,'peer_sndbuf_used',I)()==4,'failed original constructor releases its actual sound buffer')
                check(rt.peer('peer_mwsnd_counts',3)[0]==4 and rt.peer('peer_observe',20)[4]==before+4,'no memory-block or middleware-port leak after failed creation')
                bind(lib,'mwRnaDestroy',None,P)(handles[1]);replacement=rt.create(groups[4].handles)
                check(replacement==handles[1] and rt.info().live_renderers==4,'actual failed-creation retry reuses original slot')
                results=[]
                thread=threading.Thread(target=lambda:results.extend([bind(lib,'jp_rna_service',I)(),bind(lib,'jp_rna_shutdown',I)()]));thread.start();thread.join(5)
                check(not thread.is_alive() and results==[3,3] and rt.info().live_renderers==4,'foreign caller cannot service or dispose live middleware')
                check(bind(lib,'jp_rna_shutdown',I)()==0 and rt.info().live_renderers==0,'explicit RNA teardown releases every actual renderer')
                check(bind(lib,'peer_sndbuf_used',I)()==0 and rt.peer('peer_observe',20)[4]==before,'all real constructor buffers and memory blocks returned')
                size=len(messages);bind(lib,'mwRnaErrCallFunc',None,P)(C.c_char_p(b'after owner retirement'))
                check(len(messages)==size,'borrowed error callback retired with RNA owner')
                check(bind(lib,'jp_rna_initialize',I)()==0,'actual middleware owner can restart without a shadow pool')
                observations.update(actual_renderer_capacity=4,failed_mono_constructor_rollback_verified=True,error_context_high_bits_preserved=all(x[0]==context for x in messages))
            else:
                group=Joints(rt,2);groups.append(group);handle=rt.create(group.handles);check(bool(handle),'actual stereo renderer creation');current[0]=handle
                bind(lib,'mwRnaSetStartSmpl',None,P,I)(handle,1024)
                bind(lib,'mwRnaSetOutVol',None,P,I)(handle,0)
                if a.case=='callback':bind(lib,'mwRnaSetSfreq',None,P,I)(handle,22050)
                if a.case in ('copy_cpu','callback','copy_dma','cleanup_pending'):
                    mode=1 if a.case in ('copy_dma','cleanup_pending') else 0
                    check(bind(lib,'sdMemBlkSetTransferMode',I,I)(mode)==0,'explicit actual SDK CPU/DMA mode')
                transport_before=G2Info();check(bind(rt.g2,'jp_g2_snapshot',I,P)(C.byref(transport_before))==0,'actual transport counters before streaming')
                bind(lib,'mwRnaStart',None,P)(handle)
                if a.case=='cleanup_pending':
                    for ch in range(2):check(group.fill(ch,wave(ch,0,512))==1024,'actual source data staged')
                    bind(lib,'mwRnaExecServer',None)()
                    check(rt.info().pending_transfers>0 and group.count(0,0)+group.count(0,1)<group.bytes,'real DMA and borrowed chunk remain pending')
                    check(bind(rt.g2,'jp_g2_gate_set',I,I)(15)==0,'explicit outer callback gate')
                    check(bind(lib,'jp_rna_shutdown',I)()==7 and rt.info().live_renderers==1,'blocked close retains actual pool and transfers')
                    check(bind(rt.g2,'jp_g2_gate_set',I,I)(0)==0,'outer callback gate released')
                    rt.close();group.retired()
                    observations.update(actual_pending_DMA_global_cleanup=True,blocked_close_preserves_ownership=True)
                elif a.case.startswith('timed'):
                    rate=int(a.case[5:]);fed=[0,0];window=bind(rt.device,'jp_device_window',P)();idle=bind(rt.present,'jp_wait_window_idle',I,P,U)
                    start=time.perf_counter()
                    while time.perf_counter()-start<.6:
                        n=min(group.count(0,0),group.count(1,0))//2
                        for ch in range(2):
                            fed[ch]+=group.fill(ch,wave(ch,fed[ch],n))//2
                        check(idle(window,max(1,round(1000/rate)))==1,'real window wait drives existing shared clock/RNA service')
                    state=rt.state(handle);info=rt.info();out=AudioInfo()
                    check(state[26]>1000 and info.service_calls>0,'actual stream position and automatic middleware service advance')
                    check(bind(rt.audio,'jp_audio_snapshot',I,P)(C.byref(out))==0 and out.submitted>0 and out.consumed>0 and out.session_muted==1,'same actual Windows PCM worker consumes firmware audio')
                    observations.update(frame_wait_rate=rate,source_frames_fed=fed,actual_stream_sample_frames=state[26],service_calls=info.service_calls,submitted=out.submitted,consumed=out.consumed,second_output_client_created=False)
                    rt.close();group.retired()
                else:
                    total=24576;fed=0;sent=0;last=None;expected=[bytearray(rt.memory(v,16384)) for v in rt.state(handle)[16:18]];output=bytearray();flushing=False;wraps=0
                    for iteration in range(500):
                        n=min((128,256,512,768,1024)[iteration%5],total-fed,group.count(0,0)//2,group.count(1,0)//2)
                        values=[group.fill(ch,wave(ch,fed,n)) for ch in range(2)];check(values[0]==values[1],'stereo input lines remain balanced');fed+=values[0]//2
                        rt.service();state=rt.state(handle)
                        signature=(state[12],state[13],state[19],state[23])
                        if state[5] and state[23]>0 and signature!=last and not flushing:
                            count=state[23];check(state[19]==sent%8192,'actual middleware ring position follows independent sample order')
                            for ch in range(2):
                                data=wave(ch,sent,count);check(C.string_at(state[12+ch],count*2)==data,'held original chunk contains exactly the independent next PCM samples')
                                offset=(sent%8192)*2;expected[ch][offset:offset+count*2]=data
                                check(rt.memory(state[16+ch],16384)==bytes(expected[ch]),'entire real AICA ring matches independent writes and untouched bytes')
                            if sent//8192!=(sent+count)//8192:wraps+=1
                            sent+=count;last=signature
                        group.guards()
                        if sent==total and not flushing:bind(lib,'mwRnaFlush',None,P)(handle);flushing=True
                        output+=rt.render(256)
                        if flushing and bind(lib,'mwRnaGetPlaySw',I,P)(handle)==0 and bind(lib,'mwRnaGetNumData',I,P)(handle)==0:break
                    else:raise AssertionError('actual RNA stream did not finish/drain')
                    check(fed==sent==total and wraps>=2 and any(output),'complete authored stream wraps, plays nonzero firmware PCM and drains')
                    if a.case=='callback':check(messages and reentrant and all(x==4 for x in reentrant),'real short-copy error callback cannot reenter owner shutdown')
                    else:
                        if messages:(d/'transfer_diagnostics.json').write_text(json.dumps(diagnostics,indent=2)+'\n')
                        check(not messages,('44.1kHz valid stream emits no original middleware errors',sorted(set(x[1] for x in messages))))
                    final=rt.memory(0,2<<20);(d/'private_final_RAM.bin').write_bytes(final);(d/'private_output.s16le').write_bytes(output)
                    transport_after=G2Info();check(bind(rt.g2,'jp_g2_snapshot',I,P)(C.byref(transport_after))==0,'actual transport counters after streaming')
                    if a.case=='copy_cpu':check(transport_after.dma_started==transport_before.dma_started,'CPU qualification starts no hidden DMA transfers')
                    if a.case=='copy_dma':check(transport_after.dma_started>transport_before.dma_started,'DMA qualification executes real DMA transfers')
                    if a.case=='copy_dma':
                        check(final==(a.reference/'private_final_RAM.bin').read_bytes() and bytes(output)==(a.reference/'private_output.s16le').read_bytes(),'every mixed PCM byte and full2MiB firmware state match CPU and DMA streaming')
                    observations.update(source_frames=total,rendered_frames=len(output)//4,ring_wraps=wraps,PCM_sha256=hashlib.sha256(output).hexdigest(),RAM_sha256=hashlib.sha256(final).hexdigest(),actual_borrowed_chunks=True,new_DMA_transfers=transport_after.dma_started-transport_before.dma_started)
            if rt.live:rt.close()
            for group in groups:group.retired();group.close()
        finally:
            if rt and rt.live:
                bind(rt.g2,'jp_g2_gate_set',I,I)(0);rt.close()
    result=dict(case=a.case,checks=len(labels),assertions=labels,observations=observations)
    (d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
if __name__=='__main__':main()
