"""Original PCM/MIDI contracts and real firmware PCM through one shared owner."""
import argparse,ctypes as C,hashlib,json,random,re,struct,subprocess,sys,time
from pathlib import Path
from pstm_reference_bus import make_bus_reference
from verify_native_audio import bind,Info as AudioInfo
from verify_native_aica import Options as AicaOptions
from verify_native_game_sound import Options as SoundOptions,Info as SoundInfo
from verify_native_system import Options as SystemOptions
from verify_native_device_init import Options as DeviceOptions
from verify_native_input_backend import Source,Sample
from verify_native_sound_clock import Options as ClockOptions
P,I,U,Q,S,B,H=C.c_void_p,C.c_int32,C.c_uint32,C.c_uint64,C.c_size_t,C.c_int8,C.c_int16
CASES=('contracts','driver_reference','driver_cpu','driver_dma','timed')
def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path): return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','track','output'): ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES); ap.add_argument('--reference',type=Path)
    ap.add_argument('--contract-part',choices=('all','producers','status'),default='all'); a=ap.parse_args()
    r=a.workspace.resolve(); p=r/'outputs/sonic-adventure'; d=a.output.resolve(); b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure'); d.mkdir()
    for n,h in b['authored_source_sha256'].items(): assert sha(r/n)==h,n
    for n,h in b['pstm_original_source_sha256'].items(): assert sha(a.upstream/n)==h,n
    for n,h in b['pstm_private_matching_data_sha256'].items(): assert sha(a.build/n)==h,n
    assert sha(a.build/'external_game_sound.c')==b['pstm_generated_source_sha256']
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256']
    if not a.case:
        cases={}
        for name in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','upstream','build','binary','track'): cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--case',name,'--output',str(d/name),'--reference',str(d/'driver_reference')]
            run=subprocess.run(cmd,capture_output=True,timeout=360,creationflags=subprocess.CREATE_NO_WINDOW)
            (d/(name+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace'))
            cases[name]=read(d/name/'result.json'); print(name,cases[name]['checks'],'checks',flush=True)
        result=dict(status='original_PCM_ports_MIDI_status_native_contracts_real_firmware_PCM_and_shared_output_verified',
            runtime_checks=sum(v['checks'] for v in cases.values()),fresh_process_cases=len(cases),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
            original_complete_entry_calls=sum(v.get('original_complete_entry_calls',0) for v in cases.values()),
            original_function_hooks=False,actual_driver_ACK_not_synthesized=True,independent_raw_wire_full_RAM_and_PCM_comparison=True,
            production_ADXT_RNA_GDFS_music_streaming_or_gameplay_verified=False,physical_console_waveform_measurement=False,
            playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n'); print(result['status'],result['runtime_checks'],'checks',flush=True); return
    frames=C.CDLL(b['frames_DLL']); present=C.CDLL(b['present_DLL']); system=C.CDLL(b['System_DLL']); lib=C.CDLL(b['game_sound_DLL'])
    core=C.CDLL(str(a.build/'jp_native_aica.dll')); device=C.CDLL(b['device_DLL']); audio=C.CDLL(str(a.build/'jp_native_audio.dll'))
    labels=[]; observations={}; keep=[]; calls={}; prereqs=False; system_live=False
    def check(ok,label): assert ok,(a.case,label); labels.append(label)
    def peer(name,count):
        out=(Q*count)(); check(bind(lib,name,I,P)(out)==0,'actual '+name); return list(out)
    def memory(at,n):
        out=C.create_string_buffer(n); check(bind(core,'jp_aica_read',I,U,P,U)(at,out,n)==0,'actual driver RAM read'); return out.raw
    def write(at,data): check(bind(core,'jp_aica_write',I,U,P,U)(at,C.create_string_buffer(data),len(data))==0,'actual driver RAM fixture write')
    def render(count):
        output=bytearray()
        while count:
            n=min(count,4093); buf=C.create_string_buffer(n*4)
            check(bind(lib,'jp_sound_render',I,U,P,Q)(n,buf,len(buf))==0,'actual firmware executes and renders shared PCM')
            output+=buf.raw; count-=n
        return bytes(output)
    def flush():
        for _ in range(100):
            check(bind(lib,'jp_sound_service',I)()==0,'actual SDK command transfer service'); render(441)
            q=peer('peer_commands',8); work=peer('peer_observe',20)
            if not any(q[:3]) and not work[7] and not work[8] and memory(work[14],1)==b'\0': return
        raise AssertionError('actual command startup did not drain')
    @Source
    def source(ctx,samples):
        C.memset(samples,0,4*C.sizeof(Sample)); samples[0]=Sample(1,0,0,0,0,0,0,0); return 1
    keep.append(source)
    try:
        for n,args in [('jp_queue_initialize',(8<<20,)),('jp_frames_initialize',(8,128,32<<20)),('jp_textures_initialize',(32,1024,32<<20)),('jp_render_state_initialize',(128,1<<20))]:
            check(bind(frames,n,I,*([S] if len(args)==1 else [U,U,S] if len(args)==3 else [U,S]))(*args)==0,'actual render prerequisite '+n)
        prereqs=True; path=C.create_unicode_buffer(str(a.track.resolve())); keep.append(path)
        opts=SystemOptions(96,0,DeviceOptions(640,480,0,0,5000,0,64<<20),C.addressof(path),2352,3,5000,0,C.cast(source,P).value,None,None,None)
        check(bind(system,'jp_system_configure',I,P)(C.byref(opts))==0 and bind(system,'jp_system_initialize',I,I,I,I)(1,0,1)==0,'actual System/GDFS/device initialization')
        system_live=True
        soundopts=SoundOptions(56,0,2<<20,5000,AicaOptions(32,1 if a.case=='timed' else 0,16384,40,5000,1,1,0),None)
        if a.case=='timed':
            clock=ClockOptions(56,441,8192,5,None,None,0,None,None)
            check(bind(system,'jp_system_start_sound_clock',I,P,P)(C.byref(soundopts),C.byref(clock))==0,'actual shared timed firmware/WASAPI owner')
        else:
            check(bind(lib,'jp_sound_configure',I,P)(C.byref(soundopts))==0 and bind(lib,'jp_sound_initialize',I)()==0,'actual original game sound initialization and supplied firmware')
            flush()
        work=peer('peer_observe',20); layout=peer('peer_pstm_layout',8)
        check(layout[:3]==[44,168,344] and layout[4:]==[136,4,1280,1024],'actual native scalar/port/status wire layouts')
        openport=lib.sdPstmOpenPort; openport.restype=I; openport.argtypes=[P,B]
        if a.case=='contracts':
            sys.path.insert(0,str(p)); import verify_startup as executor
            from verify_fpu_cleanup_transfer import Fpu
            reference,refhash=make_bus_reference(executor); raw=a.binary.read_bytes()
            assert hashlib.sha256(raw).hexdigest()=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
            table={name:int(at,16) for at,name in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
            ram=bytearray(18<<20); ram[0x10000:0x10000+len(raw)]=raw
            # Original G2 CPU reads execute with a no-transfer fixture: all four
            # DMA start registers idle and FIFO empty. This is not a firmware ACK.
            mmio={0xa05f688c:0,**{0xa05f7800+k*32+24:0 for k in range(4)}}
            def put(at,data): off=at&0xffffff; ram[off:off+len(data)]=data
            def get(at,n): off=at&0xffffff; return bytes(ram[off:off+n])
            def original(name,*args):
                cpu=executor.State(); cpu.pc=table[name]; cpu.pr=0x8cfed000; cpu.r[15]=0x8cf00000; cpu.sr=0x70000000
                for i in range(8,15): cpu.r[i]=0x13570000+i
                saved=list(cpu.r[8:])
                for i,v in enumerate(args):
                    if i<4: cpu.r[4+i]=v&0xffffffff
                    else: put(cpu.r[15]+(i-4)*4,struct.pack('<I',v&0xffffffff))
                if name=='sdPstmOpenPort':
                    # Original entry keeps handle in R4, but reads its final
                    # named byte parameter at caller SP+0 (MOV.B @(R0,R15),R4
                    # at0x8c676fde, R0=28-byte frame). Varargs follow at SP+4.
                    for i,v in enumerate(args[1:]): put(cpu.r[15]+i*4,struct.pack('<I',v&0xffffffff))
                f=Fpu(); f.fpscr=0x40001
                reference(ram,cpu,fpu=f,mmio=mmio,instruction_budget=200000)
                check(cpu.pc==0x8cfed000 and list(cpu.r[8:])==saved and cpu.sr&0xf0==0,('complete original CPU/stack/mask',name))
                calls[name]=calls.get(name,0)+1; return I(cpu.r[0]).value
            gw=0x8cd00000; ring=0x8cd10000; output=0x8cd20000
            put(gw,bytes(1140)); put(gw,C.string_at(work[1],344)); put(table['sdgWrkPtr'],struct.pack('<I',gw))
            put(table['sdgPrintFunc'],bytes(4)); put(table['sdgDebugMode'],bytes(4)); put(gw+440,struct.pack('<I',work[13]))
            g2work=0x8cd30000; put(g2work,bytes(292)); put(table['sygG2WrkPtr'],struct.pack('<I',g2work))
            put(g2work,struct.pack('<I',0xa05f7800)); put(g2work+0xe4,struct.pack('<I',1))
            for k in range(4): put(g2work+4+k*56,struct.pack('<I',k))
            def resetqueue():
                pointer=bind(lib,'peer_qsound_second',P)()
                check(bind(lib,'sd2ndHostCmdInitList',I,P,I)(pointer,256)==original('sd2ndHostCmdInitList',ring,256)==0,'actual native and original ring initializer')
                # The original initializer clears entries and cursors but NOT
                # second_num. Set that input fixture explicitly on both sides
                # between independent batches. Never send fuzz commands to ARM
                # or present this test setup as a production reset/cancellation.
                I.from_address(work[1]+layout[2]+28).value=0
                put(gw+364,bytes(4))
            resetqueue()
            def same():
                check(C.string_at(work[1],344)==get(gw,344),'all actual ports and44-byte stream records match original instructions')
                q=peer('peer_commands',8); expected=struct.unpack('<3i',get(gw+364,12))
                check(q[2:5]==list(expected),'full queue count/write/read ownership')
                pointer=bind(lib,'peer_qsound_second',P)()
                check(C.string_at(pointer,256*28)==get(ring,256*28),'all256 complete original/native command entries and retained parameter bytes')
            def mutate(name,g,n,args,types):
                if peer('peer_commands',8)[2]>235: resetqueue()
                expected=original(name,g,*args); actual=bind(lib,name,I,P,*types)(n,*args)
                check(actual==expected,(name,'exact original status')); same(); return actual
            handles=[]
            for index,ch in enumerate((2,1,2,1,2)):
                handle=P(); put(output,b'\xa5'*8)
                expected=original('sdPstmOpenPort',output,ch,15-index,14-index)
                actual=openport(C.byref(handle),B(ch),I(15-index),I(14-index))
                gh=struct.unpack('<I',get(output,4))[0]
                check(actual==expected and bool(handle.value)==bool(gh),('real four-port capacity and native varargs channel values',index,actual,expected,hex(gh),handle.value))
                if gh:
                    check(handle.value==work[1]+gh-gw and handle.value>0xffffffff,'original pool identity retains full native pointer')
                    handles.append((gh,handle.value,ch))
                same()
            g,n,ch=handles[0]
            for value in range(-128,128) if a.contract_part!='status' else ():
                check(mutate('sdPstmSetBaseVol',g,n,(value,),(B,))==0,'every base-volume byte actually applied')
                for target in (0,1,-1):
                    check(mutate('sdPstmSetPan',g,n,(target,value,value*1001),(B,B,I))==0,'every pan byte actually enqueued')
                    check(mutate('sdPstmSetVol',g,n,(target,value,value*1777),(B,B,I))==0,'every volume byte actually enqueued')
                check(mutate('sdPstmSetDrctLev',g,n,(value,value*97),(B,I))==0,'every direct-level byte actually enqueued')
                check(mutate('sdPstmSetFxCh',g,n,(value,value),(B,B))==0,'every FX byte actually enqueued')
                if value%64==0: print('original PCM level contracts through',value,flush=True)
            for value in (-32768,-6400,-6399,-1,0,1,6399,6400,32767) if a.contract_part!='status' else ():
                mutate('sdPstmSetPitch',g,n,(value,0x12345678),(H,I))
            for rate in (48000,44100,37800,32000,24000,22050,18900,16000,12000,11025,8820,8000,0,12345) if a.contract_part!='status' else ():
                mutate('sdPstmPlayMw',g,n,(1,rate,15),(I,C.c_uint16,B))
            for pcm in (-1,0,1,2,3) if a.contract_part!='status' else ():
                for priority in (-1,0,7,15,16): mutate('sdPstmPlay',g,n,(pcm,0x7adb,priority),(I,C.c_uint16,B))
            for use in (-1,0,1,2,3) if a.contract_part!='status' else (): mutate('sdPstmSetUseCh',g,n,(use,),(B,))
            mutate('sdPstmResetPrm',g,n,(),()); mutate('sdPstmStop',g,n,(),())
            # Read real AICA RAM fixtures through the original SDK bus readers.
            # No ARM execution occurs while these controlled status bytes are installed.
            rng=random.Random(0x50434d53)
            sizes={'CurAdr':4,'DrctLev':1,'Err':4,'Flg':4,'FxLev':1,'Pan':1,'Pitch':2,'Speed':2,'TotalSmpFrame':4,'Vol':1,'Stat':28}
            for case in range(12 if a.contract_part!='producers' else 0):
                data=rng.randbytes(1280); write(work[13],data)
                ram[(16<<20)+work[13]:(16<<20)+work[13]+len(data)]=data
                for gh,nh,channels in handles:
                    for target in range(channels):
                        for suffix,size in sizes.items():
                            name='sdPstmGet'+suffix; result=C.create_string_buffer(b'\xa5'*40,40); put(output,b'\xa5'*40)
                            expected=original(name,gh,target,output+4)
                            actual=bind(lib,name,I,P,B,P)(nh,target,C.addressof(result)+4)
                            check(actual==expected==0 and result.raw==get(output,40),(name,'all output fields and external/padding guards'))
                for index in range(8):
                    gh=gw+8+index*8; nh=work[1]+8+index*8
                    for suffix in ('CurAdr','DrctLev','Err','Flg','FxLev','Pan','Pitch','Speed','TotalBeatTime','Vol','Stat'):
                        name='sdMidiGet'+suffix; result=C.create_string_buffer(b'\xa5'*40,40); put(output,b'\xa5'*40)
                        expected=original(name,gh,output+4); actual=bind(lib,name,I,P,P)(nh,C.addressof(result)+4)
                        check(actual==expected==0 and result.raw==get(output,40),(name,'actual bus read, complete output and guards'))
            for gh,nh,channels in handles:
                mutate('sdPstmClosePort',gh,nh,(),())
            result=C.create_string_buffer(b'\xa5'*40,40); put(output,b'\xa5'*40)
            expected=original('sdPstmGetStat',0,0,output+4)
            check(bind(lib,'sdPstmGetStat',I,P,B,P)(None,0,C.addressof(result)+4)==expected and result.raw==get(output,40),'null stream status preserves original error and output guards')
            err=peer('peer_qsound_error',2)
            check(err[1]==C.cast(lib.sdMidiGetStat,P).value and struct.unpack('<I',get(table['sdgErrApi'],4))[0]==table['sdMidiGetStat'],'original stream status error deliberately retains the real MIDI getter identity')
            # Selected upstream GCC repairs are native extensions rather than
            # a claim of matching the original undefined/out-of-bounds behavior.
            resetqueue(); handle=P(); check(openport(C.byref(handle),B(1),I(15))==0,'native mono extension port opens')
            check(bind(lib,'sdPstmSetPan',I,P,B,B,I)(handle,1,-17,0)==0,'upstream mono-right-pan repair accepts initialized value')
            entries=bind(lib,'peer_qsound_second',P)()
            check(peer('peer_commands',8)[2]==2 and C.string_at(entries+28+4+5,1)==bytes([111]),'upstream mono-right-pan sends its initialized single-channel value instead of stack garbage')
            before=C.string_at(work[1],344)
            check(bind(lib,'sdPstmStopAll',I)()==0,'bounded upstream StopAll executes')
            after=C.string_at(work[1],344); allowed={168+k*44+offset+j for k in range(4) for offset in (12,20,36) for j in range(8)}
            check(all(x==y or i in allowed for i,(x,y) in enumerate(zip(before,after))),'StopAll cannot alter ports or bytes outside the four actual stream records')
            check(bind(lib,'sdPstmClosePort',I,P)(handle)==0,'native extension port retires')
            observations.update(original_calls=calls,reference_hashes=refhash,original_function_hooks=False,
                actual_PSTM_ports=4,MIDI_status_ports=8,native_pool_pointer_bits=64,GCC_repairs_qualified_as_extensions=True,
                controlled_real_queue_count_fixture_reset_between_independent_batches=True,
                no_fuzz_commands_sent_to_firmware=True,static_original_G2_MMIO_fixture=True)
        else:
            # Authored, deterministic stereo waveforms in real SDK ring banks.
            # These fixtures contain no retail music and use the existing AICA mixer.
            for bank in (15,14):
                at=(2<<20)-(16-bank)*0x4040
                check(bind(lib,'sdSndMemAllocateBank',I,I,B,I,I)(int.from_bytes(b'SPSR','little'),bank,at,0x4000)==0,'actual SDK PCM bank allocation')
                values=[((i*(37 if bank==15 else 53))%24001)-12000 for i in range(8192)]
                write(at,struct.pack('<8192h',*values))
            handle=P()
            if a.case!='driver_reference':
                check(openport(C.byref(handle),B(2),I(15),I(14))==0,'actual stereo SDK port uses both native varargs')
                check(bind(lib,'sdPstmSetBaseVol',I,P,B)(handle,127)==0,'actual independent base volume')
            if a.case=='timed':
                check(bind(lib,'sdPstmPlayMw',I,P,I,C.c_uint16,B)(handle,1,44100,15)==0,'original PCM play joins the existing timed shared owner')
                window=bind(device,'jp_device_window',P)(); idle=bind(present,'jp_wait_window_idle',I,P,U)
                start=time.perf_counter(); last=U()
                while time.perf_counter()-start<.5:
                    check(idle(window,5)==1,'actual window wait advances shared firmware/output')
                check(bind(lib,'sdPstmGetTotalSmpFrame',I,P,B,P)(handle,0,C.byref(last))==0 and last.value>1000,'real firmware PCM sample count advances under elapsed-time owner')
                out=AudioInfo(); check(bind(audio,'jp_audio_snapshot',I,P)(C.byref(out))==0 and out.submitted>0 and out.consumed>0 and out.session_muted==1,'same existing WASAPI owner consumes mixed firmware PCM')
                check(bind(lib,'sdPstmStop',I,P)(handle)==0,'actual timed port stop requested')
                observations.update(actual_stream_sample_frames=last.value,actual_output_submitted=out.submitted,actual_output_consumed=out.consumed,second_output_client_created=False)
            else:
                if a.case=='driver_dma': check(bind(lib,'sdMemBlkSetTransferMode',I,I)(1)==0,'actual DMA command transfer mode')
                def command(op,params): return bytes([op,0])+bytes(params).ljust(14,b'\0')
                expected=[command(0x26,[0x20,192]),command(0x26,[0x10,0,0,0,0,64]),command(0x21,[0x80,0,0,0,120,15,14]),
                          command(0x25,[0x30,116,0,0,0,116,0,0]),command(0x28,[0,0,128,128,0,0]),command(0x22,[0])]
                shadow=bytearray(C.string_at(work[2],512)); pcm=bytearray(); nonzero=False
                for index,wire in enumerate(expected):
                    if index==2 and a.case!='driver_reference': check(bind(lib,'sdPstmPlayMw',I,P,I,C.c_uint16,B)(handle,1,44100,15)==0,'actual rate table sends play command')
                    if index==3 and a.case!='driver_reference': check(bind(lib,'sdPstmSetVol',I,P,B,B,I)(handle,-1,-12,0)==0,'actual stereo volume command')
                    if index==4 and a.case!='driver_reference': check(bind(lib,'sdPstmSetPitch',I,P,H,I)(handle,128,0)==0,'actual stream pitch command')
                    if index==5 and a.case!='driver_reference': check(bind(lib,'sdPstmStop',I,P)(handle)==0,'actual stream stop command')
                    shadow[:16]=wire; want=bytes(shadow[:32]); shadow[0]=shadow[16]=0
                    check(memory(work[14],1)==b'\0','actual ARM ready before PCM command')
                    if a.case=='driver_reference': write(work[16],want); write(work[14],b'\1')
                    else:
                        check(bind(lib,'jp_sound_service',I)()==0,'actual SDK PCM command transfer')
                        check(memory(work[16],32)==want and memory(work[14],1)==b'\1','all PCM wire and retained padding bytes match independent expected protocol')
                    chunk=render(4410); pcm+=chunk; nonzero |= any(chunk)
                    check(memory(work[14],1)==b'\0','actual firmware acknowledges PCM command without synthetic ACK')
                check(nonzero,'real firmware produces nonzero PCM from authored ring samples')
                final=memory(0,2<<20); (d/'private_final_RAM.bin').write_bytes(final); (d/'private_output.s16le').write_bytes(pcm)
                if a.case!='driver_reference':
                    check(final==(a.reference/'private_final_RAM.bin').read_bytes() and bytes(pcm)==(a.reference/'private_output.s16le').read_bytes(),'complete2MiB RAM and every PCM byte equal independent raw protocol in CPU/DMA modes')
                observations.update(PCM_frames=len(pcm)//4,PCM_sha256=hashlib.sha256(pcm).hexdigest(),RAM_sha256=hashlib.sha256(final).hexdigest(),nonzero_PCM=nonzero)
            if handle: check(bind(lib,'sdPstmClosePort',I,P)(handle)==0,'actual PCM handle closes')
        if a.case=='timed':
            check(bind(lib,'jp_sound_clock_stop',I,U)(1)==0,'actual elapsed-time owner stops before SDK/core retirement')
            # System's timed entry claims the actual sound delegation token.
            # Only that owner may close it; direct sound shutdown must reject.
            check(bind(system,'jp_system_shutdown',I)()==0,'actual delegated System retires clock/SDK/core/output in order')
            final=SoundInfo(); check(bind(lib,'jp_sound_snapshot',I,P)(C.byref(final))==0 and final.resources==0,'no PCM sound resources survive delegated global shutdown')
        else:
            check(bind(lib,'jp_sound_shutdown',I,U)(5000)==0,'actual sound owner retires with all PCM resources')
            check(bind(system,'jp_system_shutdown',I)()==0,'actual System closes after sound')
        system_live=False
        result=dict(case=a.case,contract_part=a.contract_part,checks=len(labels),assertions=labels,observations=observations,original_complete_entry_calls=sum(calls.values()))
        (d/'result.json').write_text(json.dumps(result,indent=2)+'\n'); print(a.case,len(labels),'checks',flush=True)
    finally:
        if system_live:
            if a.case!='timed': bind(lib,'jp_sound_shutdown',I,U)(5000)
            bind(system,'jp_system_shutdown',I)()
        if prereqs:
            bind(frames,'jp_frames_shutdown',I)(); bind(frames,'jp_render_state_shutdown',None)(); bind(frames,'jp_textures_shutdown',None)(); bind(frames,'jp_queue_shutdown',None)()
if __name__=='__main__': main()
