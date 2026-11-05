"""Actual retail ARM driver/bank execution and real muted Windows PCM delivery.

The SDK command wire format is supplied by this diagnostic harness. Original
SH-4 SDK/game code is not executed, compiled into the probe, or replaced with
success callbacks. Physical console waveform/cycle fidelity remains unqualified.
"""
import argparse,ctypes as C,hashlib,json,struct,subprocess,sys,time
from pathlib import Path
from verify_native_audio import Config,Info,bind,fnv,sha

P=C.c_void_p;U=C.c_uint32;I=C.c_int32
CASES=('baseline_441','physical_441','physical_split')

def read(p):return json.loads(Path(p).read_bytes())
def digest(raw):return hashlib.sha256(raw).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','core','baseline','build','audit','audio','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();w=r/'work/sonic-adventure';u=a.core.resolve()
    assert not d.exists() and d.is_relative_to(w);d.mkdir()
    b=read(a.build/'build_verification.json');base=read(a.baseline/'build_verification.json');audit=read(a.audit/'dependency_inventory.json');audio=read(a.audio/'build_verification.json')
    assert b['core_RAM_bytes']==2<<20 and base['core_RAM_bytes']==8<<20 and b['upstream_revision']==base['upstream_revision']=='2998a4bf550949cd2daee249e725a64462cf15e0'
    for meta in (b,base):
        assert sha(meta['DLL'])==meta['DLL_sha256']
        for name,h in meta['upstream_source_sha256'].items():assert sha(u/name)==h,name
        for name,h in meta['authored_source_sha256'].items():
            candidate=r/name
            if sha(candidate)!=h:
                candidate=w/'jp-aica-core-builder-source-v2'/candidate.name
            assert sha(candidate)==h,name
    assert sha(a.build/'external_dcsound_2MiB.c')==b['generated_source_sha256']
    if 'generated_YAM_source_sha256' in b:assert sha(a.build/'external_yam_timing.c')==b['generated_YAM_source_sha256']
    if not a.case:
        reports={}
        for case in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for key in ('workspace','core','baseline','build','audit','audio'):cmd+=['--'+key,str(getattr(a,key).resolve())]
            cmd+=['--output',str(d/case),'--case',case]
            run=subprocess.run(cmd,capture_output=True,timeout=40);(d/(case+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(case,run.returncode,(run.stdout+run.stderr).decode(errors='replace'))
            reports[case]=read(d/case/'result.json');print(case,reports[case]['checks'],'checks',flush=True)
        reference=reports['baseline_441']
        for case in CASES[1:]:
            assert reports[case]['PCM_sha256']==reference['PCM_sha256'] and reports[case]['noteoff_PCM_sha256']==reference['noteoff_PCM_sha256'],case
            assert reports[case]['driver_RAM_sha256']==reference['driver_RAM_sha256'],case
        # Real output from the adapted core, not a generated sine or substitute.
        pcm=(d/'physical_split/output.s16le').read_bytes();assert digest(pcm)==reference['PCM_sha256']
        for n,h in audio['authored_source_sha256'].items():assert sha(r/n)==h,n
        assert sha(audio['DLL'])==audio['DLL_sha256'];lib=C.CDLL(audio['DLL']);open_=bind(lib,'jp_audio_open',I,P);submit=bind(lib,'jp_audio_submit',I,P,U);drain=bind(lib,'jp_audio_drain',I,U);close=bind(lib,'jp_audio_close',I,U);snapshot=bind(lib,'jp_audio_snapshot',I,P)
        config=Config(C.sizeof(Config),44100,16384,40,5000,1,1,0);assert open_(C.byref(config))==0
        expected=14695981039346656037;frames=0;queue_waits=0
        try:
            for at in range(0,len(pcm),4096*4):
                part=pcm[at:at+4096*4];values=[x[0]/32768 for x in struct.iter_unpack('<h',part)];samples=(C.c_float*len(values))(*values);expected=fnv(bytes(samples),expected);deadline=time.monotonic()+5
                while True:
                    rc=submit(samples,len(part)//4)
                    if rc!=11:break
                    assert time.monotonic()<deadline;queue_waits+=1;time.sleep(.002)
                assert rc==0;frames+=len(part)//4;C.memset(samples,0x7f,C.sizeof(samples))
            assert drain(5000)==0;out=Info();assert snapshot(C.byref(out))==0
            assert out.submitted==out.consumed==frames==88200 and not out.discarded and not out.queued_frames and out.source_hash==expected and out.events and out.session_muted==1
            output=dict(frames=frames,source_hash=expected,events=out.events,queue_wait_observations=queue_waits,session_muted=bool(out.session_muted),clock_position=out.clock_position,clock_frequency=out.clock_frequency)
        finally:assert close(5000)==0
        assert snapshot(C.byref(out))==0 and out.state==4 and not out.worker_live
        result=dict(status='actual_original_ARM_driver_bank_note_command_ack_and_muted_WASAPI_delivery_verified_as_private_probe',cases=reports,
            runtime_assertions=sum(x['checks'] for x in reports.values()),matched_8MiB_baseline_and_2MiB_adaptation_and_packet_sizes=True,real_Windows_delivery=output,
            control_includes_same_RAM_store_and_DSP_ordering_adaptations=base.get('ARM_RAM_stores_flush_pending_AICA',False) and base.get('AICA_DSP_runs_when_inaudible',False),control_is_unmodified_upstream=False,
            build_verification_sha256=sha(a.build/'build_verification.json'),baseline_build_sha256=sha(a.baseline/'build_verification.json'),audio_build_sha256=sha(a.audio/'build_verification.json'),sound_audit_sha256=sha(a.audit/'dependency_inventory.json'),verifier_sha256=sha(__file__),
            SDK_wire_commands_supplied_by_diagnostic_harness=True,original_SDK_or_game_init_integrated=False,
            physical_console_waveform_or_cycle_accuracy_verified=False,noteoff_immediate_or_exact_zero_silence_verified=False,audible_speaker_output_verified=False,retail_audio_private=True,
            public_emulator_artifact_qualified=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_assertions'],'case checks',flush=True);return
    meta=base if a.case=='baseline_441' else b;lib=C.CDLL(meta['DLL']);checks=[]
    def check(ok,label):assert ok,(a.case,label);checks.append(label)
    init=bind(lib,'jp_aica_probe_init',I);size=bind(lib,'dcsound_get_state_size',U);clear=bind(lib,'dcsound_clear_state',None,P);upload=bind(lib,'dcsound_upload_to_ram',None,P,U,P,U);get=bind(lib,'dcsound_getword',U,P,U);putword=bind(lib,'dcsound_setword',None,P,U,U);execute=bind(lib,'dcsound_execute',I,P,I,P,P);pc=bind(lib,'dcsound_get_pc',U,P);arm=bind(lib,'dcsound_get_arm_state',P,P);getreg=bind(lib,'arm_getreg',U,P,I)
    check(init()==0,'actual ARM and AICA library initialization');n=size();check(meta['core_RAM_bytes']<n<meta['core_RAM_bytes']+(1<<20),'state allocation follows actual configured RAM extent')
    storage=C.create_string_buffer(b'\xa5'*(n+64),n+64);state=P(C.addressof(storage)+32);clear(state)
    check(state.value>0xffffffff and arm(state)>0xffffffff,'core state and ARM callbacks retain full native pointer widths')
    def put(at,raw):
        check(at>=0 and at+len(raw)<=meta['core_RAM_bytes'],'diagnostic upload is bounded by actual physical state storage')
        data=C.create_string_buffer(raw);upload(state,at,data,len(raw));C.memset(data,0x7f,len(raw))
    total_cycles=0;pcs=[];packet_frames=[]
    def render(frames,split=False):
        nonlocal total_cycles
        raw=bytearray();pattern=(17,4093,7,512,3,2048,77,8191) if split else (441,);j=0
        while frames:
            count=min(frames,pattern[j%len(pattern)]);j+=1;pcm=C.create_string_buffer(b'\xa5'*(count*4+64),count*4+64);samples=U(count)
            rc=execute(state,count*128,C.addressof(pcm)+32,C.byref(samples));total_cycles+=max(rc,0);pcs.append(pc(state));packet_frames.append(samples.value)
            check(rc>=0 and samples.value==count and pcm.raw[:32]==pcm.raw[-32:]==b'\xa5'*32,'actual ARM runs without unsupported instruction and fills exactly the bounded stereo buffer')
            raw.extend(pcm.raw[32:32+count*4]);frames-=count
        check(storage.raw[:32]==storage.raw[-32:]==b'\xa5'*32,'emulation and register/RAM access preserve state canaries')
        return bytes(raw)
    # Actual ARM load/store instructions independently exercise the 2 MiB alias.
    code=struct.pack('<10I',0xe59f0018,0xe5901000,0xe59f2014,0xe5923000,0xe3a0407f,0xe5824004,0xeafffffe,0,0x1000,0x201000)
    put(0,code);putword(state,0x1000,0x5aa512ef);render(10)
    check(getreg(arm(state),1)==0x5aa512ef and getreg(arm(state),3)==(0 if a.case=='baseline_441' else 0x5aa512ef),'actual ARM load observes configured RAM mirroring')
    check(get(state,0x1004)==(0 if a.case=='baseline_441' else 0x7f),'actual ARM store through upper alias updates the correct physical word')
    clear(state);total_cycles=0;pcs=[];packet_frames=[];put(0,b'SEGA'*((2<<20)//4))
    program_path=a.audit/'private_inputs/MASARU.arm.bin';program=program_path.read_bytes();check(digest(program)==audit['actual_Japanese_driver']['program_sha256'],'exact extracted Japanese ARM driver hash preserved');put(0,program)
    before_status=get(state,get(state,0xc8));boot=render(44100,a.case=='physical_split');status=get(state,0xc8);host=get(state,0xd0);mapping=get(state,0xcc)
    check(before_status==0x41474553 and get(state,status)==0 and len(set(pcs))>1 and pc(state)!=0,'actual driver executes, clears its real status area and reaches command service')
    check(not any(boot),'boot with no note commands produces silence')
    bank=next(x for x in audit['actual_Japanese_multi_units'] if x['name']=='EMERALDCOAST.MLT');track=Path(audit['media_inputs']['data_track']);sectors=(bank['bytes']+2047)//2048
    with track.open('rb') as f:f.seek((bank['fad']-45150)*2352);disc=f.read(sectors*2352)
    raw=b''.join(disc[j*2352+16:j*2352+2064] for j in range(sectors))[:bank['bytes']]
    check(len(raw)==bank['bytes'] and raw[:4]==b'SMLT' and digest(raw)=='362a8b1077e403bf864acbbf756a854e9299f25f9111a5ad26a2add634c25792','actual Japanese stage bank matches independent exploratory payload hash')
    offsets={'SMSB':0,'SMPB':0x80,'SOSB':0x100,'SFPB':0x200,'SFOB':0x280,'SFPW':0x288}
    for record in bank['bank_records']:
        if record['allocation_bytes']>=0:
            at=mapping+offsets[record['type']]+record['number']*8;putword(state,at,record['sound_memory_offset']);putword(state,at+4,record['allocation_bytes'])
        if record['data_bytes']>0:put(record['sound_memory_offset'],raw[record['data_offset']:record['data_offset']+record['data_bytes']])
    def command(kind,*params):return bytes([kind,0,*params])+bytes(14-len(params))
    def midi(status_,data1,data2=0):return command(0x86,0,0,data2,data1,0,(status_>>4)&7)
    def send(commands):
        check(get(state,status)&255==0,'real driver status ready before publishing a command list')
        wire=b''.join(commands)
        if len(commands)%2:wire+=bytes(16)
        put(host,wire);putword(state,status,(get(state,status)&0xffffff00)|1)
        check(get(state,status)&255==1,'actual command list marked waiting in driver memory')
    send([command(0x8a,0),command(0x84,0),command(0x82,0),midi(0xb0,121),midi(0xb0,32,0),midi(0xc0,0),midi(0xb0,10,64),midi(0xe0,0,64),midi(0xb0,1,127),midi(0xb0,7,106),midi(0x90,60,127)])
    output=render(88200,a.case=='physical_split');check(get(state,status)&255==0,'actual ARM driver acknowledges and consumes note command list')
    samples=struct.unpack('<'+'h'*(len(output)//2),output);check(sum(x!=0 for x in samples)>1000 and max(samples)>100 and min(samples)< -100,'actual original driver and bank synthesize nonzero signed stereo PCM')
    send([midi(0x80,60,0)]);tail=render(44100,a.case=='physical_split');check(get(state,status)&255==0,'actual ARM driver acknowledges note-off list')
    # A request to stop a voice need not make a routed effect tail exactly zero.
    # Compare the complete tail against the larger-RAM control with the same
    # ordering adaptations; report measured residue without claiming hardware or
    # note-off envelope conformance from a driver acknowledgement alone.
    late=struct.unpack('<'+'h'*(4410*2),tail[-4410*4:])
    # Memory is read through the actual public core accessor, not internal offsets.
    ram=b''.join(struct.pack('<I',get(state,at)) for at in range(0,2<<20,4));(d/'output.s16le').write_bytes(output);(d/'noteoff.s16le').write_bytes(tail);(d/'private_final_RAM.bin').write_bytes(ram)
    report=dict(status='actual_retail_driver_note_and_noteoff_executed',checks=len(checks),labels=checks,PCM_sha256=digest(output),noteoff_PCM_sha256=digest(tail),driver_RAM_sha256=digest(ram),
        PCM_frames=len(output)//4,nonzero_samples=sum(x!=0 for x in samples),minimum=min(samples),maximum=max(samples),total_executed_cycles=total_cycles,packet_calls=len(packet_frames),state_bytes=n,
        driver_program_sha256=digest(program),bank_sha256=digest(raw),core_RAM_bytes=meta['core_RAM_bytes'],private_core_DLL_sha256=sha(meta['DLL']),render_packet_pattern='variable' if a.case=='physical_split' else '441',commands_supplied_by_harness=True,
        final_tail_peak=max(abs(x) for x in late),final_tail_RMS=(sum(x*x for x in late)/len(late))**.5,exact_zero_tail_expected=False,
        final_ARM_registers=[getreg(arm(state),j) for j in range(18)])
    (d/'result.json').write_text(json.dumps(report,indent=2)+'\n');print(a.case,len(checks),'checks',flush=True)

if __name__=='__main__':main()
