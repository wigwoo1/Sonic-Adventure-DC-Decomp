"""Actual owned AICA/reset/RAM/PCM lifetimes and real-driver regressions."""
import argparse,ctypes as C,hashlib,json,struct,subprocess,sys,threading,time
from pathlib import Path
from verify_native_audio import Config,Info as AudioInfo,bind,sha

P=C.c_void_p;U=C.c_uint32;I=C.c_int32;Q=C.c_uint64
CASES=('basic','driver','output','borrowed_output','opening_failure','core_failure')
class Options(C.Structure):
    _fields_=[(n,U) for n in ('bytes','flags','capacity_frames','latency_ms','timeout_ms','muted')]+[('gain',C.c_float),('reserved',U)]
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('state_address','state_bytes','frames','cycles','read_bytes','write_bytes','submitted','render_calls','resets')]+[(n,U) for n in ('phase','status','native_error','resources','arm_reset','creator_thread','output_enabled','reserved')]
class Memory(C.Structure):
    _fields_=[('base',P),('allocation',P),('allocation_protect',U),('partition',C.c_uint16),('size',C.c_size_t),('state',U),('protect',U),('type',U)]
def read(p):return json.loads(Path(p).read_bytes())
def digest(raw):return hashlib.sha256(raw).hexdigest()
def fields(info):return {n:getattr(info,n) for n,_ in info._fields_}

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','audit','probe_check','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json');audit=read(a.audit/'dependency_inventory.json');probe=read(a.probe_check/'verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    assert sha(b['DLL'])==b['DLL_sha256'] and sha(b['audio_DLL'])==b['audio_DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    if not a.case:
        results={}
        for case in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for name in ('workspace','build','audit','probe_check'):cmd+=['--'+name,str(getattr(a,name).resolve())]
            cmd+=['--output',str(d/case),'--case',case];run=subprocess.run(cmd,capture_output=True,timeout=40);(d/(case+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(case,run.returncode,(run.stdout+run.stderr).decode(errors='replace'))
            results[case]=read(d/case/'result.json');print(case,results[case]['checks'],'checks',flush=True)
        result=dict(status='owned_native_AICA_real_reset_RAM_driver_render_output_rollback_and_close_verified',runtime_checks=sum(x['checks'] for x in results.values()),fresh_process_cases=len(CASES),cases=results,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),probe_reference_sha256=sha(a.probe_check/'verification.json'),
            original_SDK_or_game_init_integrated=False,physical_console_waveform_or_cycle_accuracy_verified=False,audible_speaker_output_verified=False,retail_audio_private=True,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    lib=C.CDLL(b['DLL']);audio=C.CDLL(b['audio_DLL']);kernel=C.WinDLL('kernel32',use_last_error=True)
    open_=bind(lib,'jp_aica_open',I,P);close=bind(lib,'jp_aica_close',I,U);status=bind(lib,'jp_aica_status',I);snapshot=bind(lib,'jp_aica_snapshot',I,P)
    get=bind(lib,'jp_aica_read',I,U,P,U);put=bind(lib,'jp_aica_write',I,U,P,U);render=bind(lib,'jp_aica_render',I,U,P,Q);pump=bind(lib,'jp_aica_pump',I,U);drain=bind(lib,'jp_aica_drain',I,U)
    getreg=bind(lib,'jp_aica_read_register',I,U,P);setreg=bind(lib,'jp_aica_write_register',I,U,U);reset=bind(lib,'jp_aica_set_arm_reset',I,U)
    query=bind(kernel,'VirtualQuery',C.c_size_t,P,P,C.c_size_t);current_thread=bind(kernel,'GetCurrentThreadId',U)
    labels=[];observations={};own=False
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    def info():
        out=Info();check(snapshot(C.byref(out))==0,'native owner diagnostic snapshot');return out
    def config(flags=0,capacity=16384,timeout=5000):return Options(C.sizeof(Options),flags,capacity,40,timeout,1,1,0)
    def memory(address):
        out=Memory();check(query(address,C.byref(out),C.sizeof(out))==C.sizeof(out),'actual native page reservation query');return out
    def begin(options=None):
        nonlocal own
        opts=options or config();rc=open_(C.byref(opts));own=True;check(rc==0,'actual native core owner opens')
        v=info();check(v.phase==2 and v.resources==(3 if opts.flags else 1) and v.arm_reset and v.creator_thread==current_thread() and status()==0,'core ownership, ARM-held reset and real output resources reflect constructor result');return v
    def end(timeout=5000):
        nonlocal own
        address=info().state_address;check(close(timeout)==0,'actual native core and audio resources close');own=False;v=info()
        check(v.phase==4 and not v.resources and not v.state_address,'closed state publishes no live core memory')
        if address:check(memory(address).state==0x10000,'former guarded core reservation actually freed')
        check(close(0)==0,'repeated close is harmless');return v
    def readmem(at,n):
        out=C.create_string_buffer(b'\xa5'*(n+16),n+16);check(get(at,out,n)==0 and out.raw[n:]==b'\xa5'*16,'actual bounded RAM read preserves caller canaries');return out.raw[:n]
    def writemem(at,raw):
        data=C.create_string_buffer(raw);check(put(at,data,len(raw))==0,'actual copied RAM write');C.memset(data,0x7f,len(raw))
    def word(at):return struct.unpack('<I',readmem(at,4))[0]
    def writeword(at,value):writemem(at,struct.pack('<I',value))
    def render_pcm(frames):
        all_=bytearray()
        while frames:
            count=min(4093,frames);out=C.create_string_buffer(b'\xa5'*(count*4+32),count*4+32)
            check(render(count,out,count*4)==0 and out.raw[count*4:]==b'\xa5'*32,'real bounded stereo render and caller guard bytes');all_.extend(out.raw[:count*4]);frames-=count
        return bytes(all_)
    try:
        check(C.sizeof(Options)==32 and C.sizeof(Info)==104 and C.sizeof(Memory)==48 and status()==2 and close(0)==0,'native options/snapshot/page-query ABIs and cold lifecycle')
        check(info().phase==0 and open_(None)==1 and snapshot(None)==1,'cold invalid calls cannot invent a native owner')
        if a.case=='basic':
            for member,value in [('bytes',0),('flags',2),('capacity_frames',4095),('latency_ms',19),('timeout_ms',0),('muted',2),('gain',float('nan')),('reserved',1)]:
                opts=config();setattr(opts,member,value);check(open_(C.byref(opts))==1 and info().resources==0,'invalid native configuration rejected before resource acquisition: '+member)
            opts=config();v=begin(opts);opts.flags=1;opts.capacity_frames=1
            check(info().output_enabled==0 and open_(C.byref(config()))==2,'copied configuration and singleton owner cannot be replaced')
            page=4096;before=memory(v.state_address-1);inside=memory(v.state_address);after=memory(v.state_address+((v.state_bytes+page-1)//page)*page)
            check(v.state_address>0xffffffff and before.state==after.state==0x2000 and inside.state==0x1000 and inside.protect==4,'actual guarded high-address RAM state is committed read/write between reserved guard pages')
            for at,raw in ((0,b'abcde'),(1,b'1234567'),((2<<20)-9,b'last-nine')):
                writemem(at,raw);check(readmem(at,len(raw))==raw,'unaligned RAM bytes and final physical byte copied exactly')
            out=C.create_string_buffer(b'\xa5'*32,32);before_bytes=out.raw
            for at,n in ((2<<20,1),((2<<20)-1,2),(0xffffffff,4),(0,0),(0,0xffffffff)):
                check(get(at,out,n)==1 and put(at,out,n)==1 and out.raw==before_bytes,'out-of-range RAM spans cannot wrap or modify output')
            check(get(0,v.state_address,4)==1 and put(0,v.state_address,4)==1 and snapshot(v.state_address)==1 and getreg(0,v.state_address)==1,'core reservation cannot alias mutable API buffers')
            check(render(1,out,3)==1 and render(8193,out,32)==1 and reset(2)==1 and setreg(0x2c00,2)==1 and getreg(3,out)==1,'invalid render/reset/register requests preserve ready core')
            data=C.create_string_buffer(b'xyz');sentinel=Info();C.memset(C.byref(sentinel),0xa5,C.sizeof(sentinel));sentinel_bytes=bytes(sentinel);foreign=[]
            def outsider():foreign.extend([open_(C.byref(config())),get(0,out,1),put(0,data,1),getreg(0,out),setreg(0,0),reset(0),render(1,out,4),pump(1),drain(0),close(0),status(),snapshot(C.byref(sentinel))])
            thread=threading.Thread(target=outsider);thread.start();thread.join(5)
            check(not thread.is_alive() and foreign==[3]*12 and bytes(sentinel)==sentinel_bytes,'foreign thread cannot inspect, alter or retire the live core')
            # Store marker at 0x1000 and loop; pending FIQ must not preempt reset.
            writemem(0,struct.pack('<5I',0xe59f0008,0xe3a0107f,0xe5801000,0xeafffffe,0x1000));writeword(0x1000,0);writeword(0x2000,0x12345678)
            check(setreg(0x2890,0x100)==0,'configure actual emulated timer before held reset')
            timer=U();check(getreg(0x2890,C.byref(timer))==0,'read actual emulated timer');previous=timer.value
            held=render_pcm(441);check(not any(held) and word(0x1000)==0 and info().cycles==0,'held CPU executes no instructions while sound samples advance')
            check(getreg(0x2890,C.byref(timer))==0 and timer.value!=previous,'actual AICA timer advances independently of held CPU')
            check(reset(0)==0,'release actual reset line');render_pcm(441)
            check(word(0x1000)==127 and info().cycles>0,'reset masks pending interrupts so first ARM instruction path really runs')
            check(reset(1)==0 and word(0x2000)==0x12345678,'CPU reset preserves RAM');writeword(0x1000,0);cycles=info().cycles
            render_pcm(441);check(word(0x1000)==0 and info().cycles==cycles and info().resets==1,'reasserted reset holds fresh CPU without clearing chip state')
            check(reset(0)==0,'release second actual reset');render_pcm(441);check(word(0x1000)==127,'CPU restarts at its real vector after reset')
            check(pump(1)==2 and drain(0)==2,'explicit offline mode never reports device delivery');observations['last']=fields(info());end()
            begin();check(word(0x2000)==0 and info().frames==0,'fresh open has a new zeroed chip context');end()
        elif a.case=='driver':
            begin();writemem(0,b'SEGA'*((2<<20)//4));program=(a.audit/'private_inputs/MASARU.arm.bin').read_bytes();check(digest(program)==audit['actual_Japanese_driver']['program_sha256'],'original driver program fingerprint');writemem(0,program);check(reset(0)==0,'release actual driver CPU')
            check(not any(render_pcm(44100)),'original driver boots with silent unrequested notes');driver_status=word(0xc8);host=word(0xd0);mapping=word(0xcc);check(word(driver_status)==0,'actual driver initialized command-service status')
            bank=next(x for x in audit['actual_Japanese_multi_units'] if x['name']=='EMERALDCOAST.MLT');track=Path(audit['media_inputs']['data_track']);sectors=(bank['bytes']+2047)//2048
            with track.open('rb') as f:f.seek((bank['fad']-45150)*2352);disc=f.read(sectors*2352)
            raw=b''.join(disc[j*2352+16:j*2352+2064] for j in range(sectors))[:bank['bytes']];check(digest(raw)==probe['cases']['physical_split']['bank_sha256'],'same actual Japanese bank supplied to owned core')
            offsets={'SMSB':0,'SMPB':0x80,'SOSB':0x100,'SFPB':0x200,'SFOB':0x280,'SFPW':0x288}
            for row in bank['bank_records']:
                if row['allocation_bytes']>=0:
                    at=mapping+offsets[row['type']]+row['number']*8;writeword(at,row['sound_memory_offset']);writeword(at+4,row['allocation_bytes'])
                if row['data_bytes']>0:writemem(row['sound_memory_offset'],raw[row['data_offset']:row['data_offset']+row['data_bytes']])
            def command(kind,*params):return bytes([kind,0,*params])+bytes(14-len(params))
            def midi(kind,x,y=0):return command(0x86,0,0,y,x,0,(kind>>4)&7)
            def send(commands):
                check(word(driver_status)&255==0,'real driver ready before wire commands');wire=b''.join(commands)+(bytes(16) if len(commands)%2 else b'');writemem(host,wire);writeword(driver_status,(word(driver_status)&0xffffff00)|1)
            send([command(0x8a,0),command(0x84,0),command(0x82,0),midi(0xb0,121),midi(0xb0,32),midi(0xc0,0),midi(0xb0,10,64),midi(0xe0,0,64),midi(0xb0,1,127),midi(0xb0,7,106),midi(0x90,60,127)])
            pcm=render_pcm(88200);check(word(driver_status)&255==0 and digest(pcm)==probe['cases']['physical_split']['PCM_sha256'],'owned reset/RAM/render boundary preserves exact real game-bank note output')
            send([midi(0x80,60)]);tail=render_pcm(44100);check(digest(tail)==probe['cases']['physical_split']['noteoff_PCM_sha256'],'owned boundary preserves the entire real note-off tail')
            ram=readmem(0,2<<20);check(digest(ram)==probe['cases']['physical_split']['driver_RAM_sha256'],'complete final 2 MiB RAM agrees with qualified prior driver probe')
            observations.update(note_PCM_sha256=digest(pcm),tail_PCM_sha256=digest(tail),RAM_sha256=digest(ram),last=fields(info()));(d/'private_output.s16le').write_bytes(pcm);end()
        elif a.case=='output':
            v=begin(config(1,4096));before=info();check(pump(8192)==8 and info().frames==before.frames,'queue-full pump rejects before emulation advances')
            check(pump(4096)==0,'actual native core PCM accepted by actual owned Windows stream');check(drain(0) in (0,7),'nonblocking drain reports real progress or bounded timeout');check(drain(5000)==0,'actual stream clock drains generated core PCM')
            ai=AudioInfo();check(bind(audio,'jp_audio_snapshot',I,P)(C.byref(ai))==0 and ai.consumed==ai.submitted==4096 and ai.session_muted==1,'shared actual audio owner observes copied core frames in queried muted session')
            check(info().submitted==4096 and info().output_enabled==1,'factory records actual output, distinct from offline rendering')
            check(pump(4096)==0,'another actual pending core packet before shutdown');address=info().state_address;rc=close(0);check(rc in (0,7),'bounded owner close joins or retains actual worker/core')
            if rc==7:
                check(info().phase==3 and info().state_address==address and info().resources==3 and open_(C.byref(config()))==2,'timed-out close retains the same core until audio worker joins');end()
            else:
                own=False;check(info().phase==4 and memory(address).state==0x10000,'completed nonblocking close freed actual core state')
            observations['zero_close_result']=rc;begin(config(1));end()
        elif a.case=='borrowed_output':
            ac=Config(C.sizeof(Config),44100,16384,40,5000,1,1,0);ao=bind(audio,'jp_audio_open',I,P);ax=bind(audio,'jp_audio_close',I,U);ast=bind(audio,'jp_audio_status',I)
            check(ao(C.byref(ac))==0,'independent real output owner exists before core acquisition')
            try:check(open_(C.byref(config(1)))==2 and not info().resources and close(0)==0 and ast()==0,'factory rejects and preserves a pre-existing actual audio session')
            finally:check(ax(5000)==0,'independent caller retires its own output session')
            begin(config(1));end()
        elif a.case=='opening_failure':
            outcomes=[]
            for _ in range(5):
                rc=open_(C.byref(config(1,timeout=1)));own=True;check(rc in (0,6),'one-millisecond constructor exposes real audio initialization result')
                v=info()
                if rc==6:check(v.phase==5 and v.native_error in (10,7,8,9,4,5,6),'actual output constructor failure recorded without claiming READY')
                check(close(5000)==0,'same timed-out constructor fully cleans up real resources');own=False;check(info().resources==0,'constructor rollback or retry retains no orphaned core/audio resources');outcomes.append(dict(result=rc,resources_before_close=v.resources,native_error=v.native_error))
            observations['outcomes']=outcomes;begin(config(1));end()
        elif a.case=='core_failure':
            begin();writemem(0,struct.pack('<I',0xee000010));check(reset(0)==0,'release CPU on deliberately unsupported coprocessor instruction')
            out=C.create_string_buffer(b'\xa5'*64,64);check(render(1,out,4)==5 and out.raw==b'\xa5'*64,'real core error does not publish partial PCM')
            check(info().phase==5 and info().resources==1 and status()==5 and word_if_allowed(get)==5,'failed core retains ownership and rejects further bus work');end();begin();end()
    finally:
        if own:
            rc=close(5000)
            if rc:print('cleanup returned',rc,flush=True)
    (d/'result.json').write_text(json.dumps(dict(checks=len(labels),labels=labels,observations=observations),indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)

def word_if_allowed(get):
    out=U();return get(0,C.byref(out),4)

if __name__=='__main__':main()
