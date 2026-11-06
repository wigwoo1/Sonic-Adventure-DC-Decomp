"""Execute original game sound startup, real files/firmware and owner failures.

No replacement GDFS, SDK, AICA, configuration, Windows PCM or DMA completion.
Retail inputs and captured RAM/PCM remain private in work/sonic-adventure.
"""
import argparse,ctypes as C,hashlib,json,struct,subprocess,sys,threading
from pathlib import Path
from verify_native_audio import bind,Info as AudioInfo
from verify_native_aica import Options as AicaOptions,Info as AicaInfo,Memory
from verify_native_sound_dispatch import Info as DispatchInfo
from verify_native_system import Options as SystemOptions,Info as SystemInfo
from verify_native_device_init import Options as DeviceOptions
from verify_native_input_backend import Source,Sample
from verify_native_gdfs import Info as GDFSInfo
P=C.c_void_p;I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;S=C.c_size_t
class Options(C.Structure):
    _fields_=[(n,U) for n in ('bytes','sound_mode','max_driver_bytes','close_timeout_ms')]+[('aica',AicaOptions),('driver_name',P)]
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('stage_address','stage_capacity','stage_reservation_bytes','dispatch_token','initializations','shutdowns','driver_bytes','config_sessions')]+[(n,U) for n in ('phase','status','resources','failed_resource','native_error','creator_thread','sound_mode','config_active')]+[('sdk_error',I),('cleanup_error',U)]
CASES=('config','stereo','mono_restart','missing_driver','oversize_driver','wrong_driver','borrowed_core','borrowed_dispatch','cleanup_retry','callback_lifetime','real_output')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def values(v):return {n:getattr(v,n) for n,_ in v._fields_}
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for key in ('game_sound_DLL','present_DLL','System_DLL','GDFS_DLL'):assert sha(b[key])==b[key+'_sha256']
    for n in ('native/jp_native_game_sound.c','native/jp_native_game_sound.h','jp_port_tools/build_native_game_sound.py'):
        key={'c':'sound_owner_source_sha256','h':'sound_owner_header_sha256','py':'sound_owner_builder_sha256'}[n.rsplit('.',1)[1]]
        assert sha(r/'outputs/sonic-adventure'/n)==b[key]
    if not a.case:
        cases={}
        for name in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','build','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--output',str(d/name),'--case',name];run=subprocess.run(cmd,capture_output=True,timeout=120,creationflags=subprocess.CREATE_NO_WINDOW)
            (d/(name+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace'))
            cases[name]=read(d/name/'result.json');print(name,cases[name]['checks'],'checks',flush=True)
        result=dict(status='owned_original_game_sound_startup_native_config_real_driver_rollback_restart_and_frame_service_verified',
            runtime_checks=sum(v['checks'] for v in cases.values()),fresh_process_cases=len(cases),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
            actual_original_initialization_functions=10,actual_initialization_commands=39,
            global_early_final_exit_or_sustained_audio_scheduler_verified=False,full_game_queue_spatial_streams_or_gameplay_verified=False,
            playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    frames=C.CDLL(b['frames_DLL']);present=C.CDLL(b['present_DLL']);system=C.CDLL(b['System_DLL']);fs=C.CDLL(b['GDFS_DLL'])
    sound=C.CDLL(b['game_sound_DLL']);core=C.CDLL(str(a.build/'jp_native_aica.dll'));g2=C.CDLL(str(a.build/'jp_native_g2_audio.dll'));audio=C.CDLL(str(a.build/'jp_native_audio.dll'))
    configure=bind(sound,'jp_sound_configure',I,P);initialize=bind(sound,'jp_sound_initialize',I);close=bind(sound,'jp_sound_shutdown',I,U)
    service=bind(sound,'jp_sound_service',I);render=bind(sound,'jp_sound_render',I,U,P,Q);pump=bind(sound,'jp_sound_pump',I,U)
    labels=[];observations={};keep=[];system_live=False;prereqs=False
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    def snap(module,name,kind):
        v=kind();check(bind(module,name,I,P)(C.byref(v))==0,'real '+name);return v
    def info():return snap(sound,'jp_sound_snapshot',Info)
    def ci():return snap(core,'jp_aica_snapshot',AicaInfo)
    def fi():return snap(fs,'jp_gdfs_snapshot',GDFSInfo)
    def di():return snap(present,'jp_sound_dispatch_snapshot',DispatchInfo)
    def options(mode=0,name=None,cap=2<<20,output=0):
        buf=C.create_string_buffer(name) if name else None
        if buf:keep.append(buf)
        return Options(56,mode,cap,5000,AicaOptions(32,output,16384,40,5000,1,1,0),C.addressof(buf) if buf else None)
    def start_system():
        nonlocal system_live,prereqs
        @Source
        def source(ctx,samples):C.memset(samples,0,4*C.sizeof(Sample));samples[0]=Sample(1,0,0,0,0,0,0,0);return 1
        keep.append(source)
        for n,args in [('jp_queue_initialize',(8<<20,)),('jp_frames_initialize',(8,128,32<<20)),('jp_textures_initialize',(32,1024,32<<20)),('jp_render_state_initialize',(128,1<<20))]:
            check(bind(frames,n,I,*([S] if len(args)==1 else [U,U,S] if len(args)==3 else [U,S]))(*args)==0,'borrow real render prerequisite '+n)
        prereqs=True;track=C.create_unicode_buffer(str(a.track.resolve()));keep.append(track)
        opts=SystemOptions(96,0,DeviceOptions(640,480,0,0,5000,0,64<<20),C.addressof(track),2352,3,5000,0,C.cast(source,P).value,None,None,None)
        check(bind(system,'jp_system_configure',I,P)(C.byref(opts))==0 and bind(system,'jp_system_initialize',I,I,I,I)(1,0,1)==0,'compiled System creates actual native GDFS/wait/input/heap')
        system_live=True
    def scalar_peer(name,count):
        out=(Q*count)();check(bind(sound,name,I,P)(out)==0,'passive actual '+name);return list(out)
    def validate_start(mode):
        s=info();c=ci();check(s.phase==3 and s.status==0 and s.resources==59 and s.cleanup_error==0 and s.config_active==0,'owner reaches READY only with all actual owned resources and closed config')
        check(s.stage_address>0xffffffff and s.stage_address%32==0 and s.stage_capacity==34816 and s.driver_bytes==33408,'actual driver uses high native aligned sector-rounded staging')
        check(hashlib.sha256(C.string_at(s.stage_address,s.driver_bytes)).hexdigest()=='5ab700a241e5d5e164c104a76555b35002a6a84370aea17bf8c34d713cdc2cb9','actual original GDFS bytes match independently known driver')
        kernel=C.WinDLL('kernel32',use_last_error=True);query=bind(kernel,'VirtualQuery',S,P,P,S)
        for address,protect,state in [(s.stage_address-1,1,0x2000),(s.stage_address,4,0x1000)]:
            v=Memory();check(query(address,C.byref(v),C.sizeof(v))==C.sizeof(v) and v.state==state and (v.protect==protect if state==0x1000 else v.allocation_protect==1),'real VirtualAlloc staging guard/committed page')
        v=Memory();query(s.stage_address,C.byref(v),C.sizeof(v));end=s.stage_address+v.size
        v=Memory();check(query(end,C.byref(v),C.sizeof(v))==C.sizeof(v) and v.state==0x2000 and v.allocation_protect==1,'upper no-access reservation after actual committed staging')
        check(c.phase==2 and c.arm_reset==0 and c.frames==88200 and c.resets==1,'original game executed both actual driver boots and ARM release')
        sd=scalar_peer('peer_observe',20);check(sd[4]==1 and sd[7]==sd[8]==0 and sd[9]==1 and sd[10]==88200 and sd[18]==1,'real original SDK retains only its primary descriptor after healthy released-slot path')
        game=scalar_peer('peer_game_sound',18)
        check(game[:6]==[0,0,32,(1<<64)-6,(1<<64)-6,0] and len(set(game[6:14]))==8 and min(game[6:14])>0xffffffff and game[14:]==[0,0,8,0],'actual original ports, slot timers, MIDI counters, FX and QSound initial state')
        queue=scalar_peer('peer_commands',8);check(queue[:6]==[0,0,39,39,0,256],'original initialization produced exactly39 actual queued requests')
        # Independent retail driver version0x00380101 has debug56 >=39: its
        # fine FX levels remain -48, then unsigned center128 gives80.
        expected=[(0x8a,bytes([255 if mode else 0])),(0x84,b'\0'),(0x82,b'\0')]+[(0x83,bytes([n,80 if n<2 else 128,128])) for n in range(4)]+[(0x86,bytes([0,0,0,121,ch,3])) for ch in range(32)]
        commands=[]
        for index,(opcode,params) in enumerate(expected):
            buf=(C.c_uint8*19)();check(bind(sound,'peer_second',I,U,P)(index,buf)==0,'read actual queued initialization command')
            raw=bytes(buf);wire=struct.pack('<I',opcode)+params+bytes(14-len(params))+b'\1'
            check(raw==wire,'independent complete opcode/parameter/padding/pending expectation '+str(index)+': '+raw.hex()+' vs '+wire.hex());commands.append(raw.hex())
        check(di().bound==1 and di().has_queue==0 and fi().live_handles==0,'real SDK frame service bound with explicit missing game queue and no driver-file leak')
        observations['initialization_commands']=commands;observations['owned_info']=values(s);observations['SDK_info']=sd
    def progress():
        before=di().service_calls
        bind(present,'jp_wait_probe_reset',None,U,U)(0,0)
        bind(present,'frame_pacing_init',None,I,I)(1,1)
        bind(present,'frame_set_interval',None,I)(1)
        bind(present,'frame_wait_step',None,I)(0)
        check(bind(present,'jp_wait_status',I)()==0 and di().service_calls>before and info().phase==3,
            'actual game frame/vblank invokes the compiled sound owner without a test callback producer')
        check(di().status==7 and di().has_queue==0,'actual frame request queue remains explicitly unavailable')
        # Firmware receives actual queued commands; no manual status/ACK injection.
        for step in range(100):
            check(service()==0,'genuine SDK host-list servicing');buf=C.create_string_buffer(441*4)
            check(render(441,buf,len(buf))==0,'actual ARM/AICA advances to consume initialization commands')
            queue=scalar_peer('peer_commands',8);sd=scalar_peer('peer_observe',20)
            status=(C.c_uint8*1)();check(bind(core,'jp_aica_read',I,U,P,U)(sd[14],status,1)==0,'read actual driver list status')
            if not any(queue[:3]) and not sd[7] and not sd[8] and status[0]!=1:break
        else:raise AssertionError('actual firmware did not consume initialization requests')
        observations['firmware_consumption_steps']=step+1
        check(info().phase==3,'original init requests complete without quarantining owner')
    try:
        check(C.sizeof(Options)==56 and C.sizeof(Info)==104 and info().phase==0,'native owner ABI and genuinely cold state')
        check(initialize()==2 and close(5000)==2 and service()==2,'unconfigured operations reject without fake success')
        o=options(mode=1 if a.case=='mono_restart' else 0)
        check(configure(C.byref(o))==0,'copy actual native sound configuration')
        if a.case=='config':
            cfginit=bind(sound,'syCfgInit',I,P);get=bind(sound,'syCfgGetSoundMode',I,P);setmode=bind(sound,'syCfgSetSoundMode',I,I);exitcfg=bind(sound,'syCfgExit',I)
            value=I(99);work=C.create_string_buffer(16384);unaligned=C.addressof(work)+1
            check(get(C.byref(value))==-1 and value.value==99 and cfginit(None)==-1,'missing settings session preserves output and rejects null work')
            check(cfginit(unaligned)==0 and cfginit(unaligned)==-1 and get(C.byref(value))==0 and value.value==0,'real unaligned config session stores copied stereo mode and forbids a second session')
            check(setmode(1)==0 and get(C.byref(value))==0 and value.value==1 and info().sound_mode==1,'native mode changes actual session data')
            check(setmode(2)==-1 and get(C.byref(value))==0 and value.value==1 and configure(C.byref(o))==2,'invalid mode/config replacement cannot corrupt an active session')
            foreign=[]
            def outsider():foreign.extend([cfginit(unaligned),get(C.byref(I(44))),setmode(0),exitcfg(),close(5000)])
            t=threading.Thread(target=outsider);t.start();t.join(5)
            check(not t.is_alive() and foreign==[-1,-1,-1,-1,3] and get(C.byref(value))==0 and value.value==1,'foreign operations cannot borrow or retire settings/owner')
            check(exitcfg()==0 and exitcfg()==-1 and get(C.byref(value))==-1,'actual session lifetime ends at exit')
            check(initialize()==5 and info().phase==1 and info().resources==0,'missing real GDFS/wait prevents startup without acquiring any owner')
        else:
            start_system()
            if a.case in ('missing_driver','oversize_driver','wrong_driver'):
                o=options(name=b'absent.drv' if a.case=='missing_driver' else b'emeraldcoast.mlt' if a.case=='wrong_driver' else None,cap=32 if a.case=='oversize_driver' else 2<<20)
                check(configure(C.byref(o))==0,'configure actual failure input')
                code=initialize();v=info();check(code==(8 if a.case=='missing_driver' else 10) and v.phase==6 and v.resources==0 and v.cleanup_error==0,'real open/size/header error rolls back owned core, transport, file and staging')
                check(ci().resources==0 and fi().live_handles==0 and di().bound==0 and service()==2,'failure leaves borrowed System alive and no stale sound callback')
                o=options();check(configure(C.byref(o))==0 and initialize()==0,'same process recovers from real input failure with valid original driver');validate_start(0)
            elif a.case=='borrowed_core':
                aopts=o.aica;check(bind(core,'jp_aica_open',I,P)(C.byref(aopts))==0,'independent actual pre-existing core')
                before=ci();check(initialize()==5 and info().resources==0 and ci().state_address==before.state_address and ci().resources==1,'sound refuses to adopt or retire an existing emulator')
                check(bind(core,'jp_aica_close',I,U)(5000)==0 and initialize()==0,'actual independent owner closes before normal startup');validate_start(0)
            elif a.case=='borrowed_dispatch':
                CB=C.CFUNCTYPE(I,P)
                @CB
                def callback(ctx):return 0
                keep.append(callback);token=Q();check(bind(present,'jp_sound_dispatch_bind',I,P,P,P,P)(callback,None,None,C.byref(token))==0,'pre-existing test-owned dispatch authority')
                check(initialize()==5 and info().resources==0 and di().token==token.value,'existing frame callback cannot be replaced or adopted')
                check(bind(present,'jp_sound_dispatch_unbind',I,Q)(token.value)==0 and initialize()==0,'exact prior owner retires before normal startup');validate_start(0)
            else:
                if a.case=='real_output':o=options(output=1);check(configure(C.byref(o))==0,'select actual muted Windows output')
                check(initialize()==0,'actual original game initialization completes under compiled ownership');validate_start(o.sound_mode)
                if a.case=='cleanup_retry':
                    old=info();check(bind(g2,'jp_g2_gate_set',I,I)(1)==0,'hold actual transport completion gate')
                    check(close(5000)==7 and info().resources==27 and info().stage_address==old.stage_address and info().cleanup_error==7 and di().bound==0 and ci().resources==1,'failed real G2 retirement retains SDK, core and staging after callback unbind')
                    check(initialize()==2 and service()==2,'quarantined owner cannot restart or service retained state')
                    bind(g2,'jp_g2_gate_set',I,I)(0);check(close(5000)==0 and info().resources==0,'same owner retries and completes actual deferred cleanup')
                else:
                    # Invalid public input never quarantines or writes the caller's guard.
                    guard=C.create_string_buffer(b'guard');check(render(0,guard,len(guard))==1 and guard.raw.startswith(b'guard') and info().phase==3,'invalid render arguments preserve output and healthy owner')
                    progress()
                    if a.case=='callback_lifetime':
                        CB=C.CFUNCTYPE(None,P);events=[]
                        @CB
                        def callback(ctx):events.append([close(5000),initialize(),info().phase])
                        keep.append(callback);raw=C.create_string_buffer(96);address=(C.addressof(raw)+31)&~31;keep.append(raw);h=P()
                        check(bind(sound,'sdMemBlkCreate',I,P)(C.byref(h))==0 and bind(sound,'sdMemBlkSetPrm',I,P,P,I,P,P)(h,address,32,callback,None)==0 and bind(sound,'sdMemBlkTransfer',I,P,I)(h,0x1ff000)==0,'actual original asynchronous SDK descriptor/copy and callback')
                        check(service()==0 and events==[[4,4,3]] and bind(sound,'sdMemBlkDestroy',I,P)(h)==0,'actual G2 completion callback cannot recursively retire live owner')
                    if a.case=='real_output':
                        check(pump(4410)==0,'real firmware PCM delivered through owned Windows worker')
                        actual=snap(audio,'jp_audio_snapshot',AudioInfo);check(actual.state==2 and actual.submitted==4410 and actual.session_muted==1,'actual Windows stream owns and received the copied firmware frames')
                        for attempt in range(32):
                            before_frames=ci().frames;rc=pump(8192)
                            if rc==15:
                                check(ci().frames==before_frames and info().phase==3 and info().status==0,'real queue-full backpressure preserves firmware time and a healthy owner');break
                            check(rc==0,'actual output worker accepts available capacity')
                        else:raise AssertionError('could not observe real queue backpressure')
                    if a.case=='mono_restart':
                        old_token=info().dispatch_token;check(close(5000)==0 and configure(C.byref(options()))==0 and initialize()==0 and info().dispatch_token>old_token,'full mono shutdown and stereo restart use fresh callback lifetime')
                        validate_start(0);progress()
                    retired=info().stage_address
                    check(close(5000)==0 and info().resources==0 and ci().resources==0 and di().bound==0,'ordered shutdown retires callback, G2, SDK, audio and owned staging')
                    memory=Memory();kernel=C.WinDLL('kernel32',use_last_error=True)
                    check(bind(kernel,'VirtualQuery',S,P,P,S)(retired,C.byref(memory),C.sizeof(memory))==C.sizeof(memory) and memory.state==0x10000,
                        'actual Windows staging reservation is freed after all borrowed pointers retire')
                    check(bind(present,'sdSysServer',I)()==0x0d000001 and service()==2,'frame forwarding cannot reach closed original SDK')
        if info().resources:check(close(5000)==0,'close remaining healthy test owner')
        if info().phase!=5:check(close(5000)==0,'close configured or rolled-back owner')
        sentinel=Info();C.memset(C.byref(sentinel),0xa5,104);foreign=[]
        def outsider():foreign.extend([configure(C.byref(o)),initialize(),service(),close(5000),bind(sound,'jp_sound_snapshot',I,P)(C.byref(sentinel))])
        t=threading.Thread(target=outsider);t.start();t.join(5)
        check(not t.is_alive() and foreign==[3]*5 and bytes(sentinel)==b'\xa5'*104,'foreign caller cannot inspect/reconfigure/restart or retire original owner')
        observations['final_sound']=values(info())
        if system_live:
            check(fi().phase==3 and fi().live_handles==0,'borrowed GDFS remains healthy after sound shutdown')
            check(bind(system,'jp_system_shutdown',I)()==0,'borrowed compiled System closes only after real sound retirement');system_live=False
        if prereqs:
            check(bind(frames,'jp_frames_shutdown',I)()==0,'close borrowed frame store');bind(frames,'jp_render_state_shutdown',None)();bind(frames,'jp_textures_shutdown',None)();bind(frames,'jp_queue_shutdown',None)();prereqs=False
        result=dict(case=a.case,checks=len(labels),assertions=labels,observations=observations);(d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
    finally:
        close(5000)
        if system_live:bind(system,'jp_system_shutdown',I)()
        if prereqs:
            bind(frames,'jp_frames_shutdown',I)();bind(frames,'jp_render_state_shutdown',None)();bind(frames,'jp_textures_shutdown',None)();bind(frames,'jp_queue_shutdown',None)()
if __name__=='__main__':main()
