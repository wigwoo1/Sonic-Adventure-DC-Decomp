"""Real frame/vblank -> bound original SDK -> ARM/AICA sound integration."""
import argparse,ctypes as C,hashlib,json,subprocess,sys,threading
from pathlib import Path
from verify_native_audio import bind
from verify_native_aica import Options as AicaOptions
from verify_native_system import Options as SystemOptions,Info as SystemInfo
from verify_native_device_init import Options as DeviceOptions
from verify_native_input_backend import Source,Sample
P=C.c_void_p;I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;S=C.c_size_t;CB=C.CFUNCTYPE(I,P)
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('token','service_calls','queue_calls','failures')]+[(n,U) for n in ('bound','status','creator_thread','in_callback','active_kind','has_queue')]+[('native_result',I),('reserved',U)]
CASES=('binding','frame_sdk')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','audit','probe_check','driver_check','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json');audit=read(a.audit/'dependency_inventory.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    assert sha(b['present_DLL'])==b['present_DLL_sha256'] and sha(b['sound_SDK_DLL'])==b['sound_SDK_DLL_sha256']
    for path,key in (('native/jp_native_sound_dispatch.c','dispatch_source_sha256'),('native/jp_native_sound_dispatch.h','dispatch_header_sha256'),('jp_port_tools/build_native_sound_dispatch.py','builder_sha256')):assert sha(r/'outputs/sonic-adventure'/path)==b[key]
    if not a.case:
        cases={}
        for case in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','build','audit','probe_check','driver_check'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--output',str(d/case),'--case',case];run=subprocess.run(cmd,capture_output=True,timeout=90);(d/(case+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(case,run.returncode,(run.stdout+run.stderr).decode(errors='replace'))
            cases[case]=read(d/case/'result.json');print(case,cases[case]['checks'],'checks',flush=True)
        result=dict(status='native_sound_dispatch_lifetime_and_real_frame_GDFS_SDK_ARM_note_path_verified',runtime_checks=sum(x['checks'] for x in cases.values()),cases=cases,fresh_process_cases=len(cases),
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),original_frame_and_SDK_functions_execute=True,
            game_sound_constructor_or_original_game_request_queue_implemented=False,global_audio_exit_or_sustained_clock_pacing_qualified=False,
            full_game_audio_verified=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    present=C.CDLL(b['present_DLL']);sb=bind(present,'jp_sound_dispatch_bind',I,P,P,P,P);unbind=bind(present,'jp_sound_dispatch_unbind',I,Q)
    service=bind(present,'jp_sound_dispatch_service',I,P);queue=bind(present,'jp_sound_dispatch_queue',I,P);snapshot=bind(present,'jp_sound_dispatch_snapshot',I,P)
    frame_server=bind(present,'sdSysServer',I);frame_queue=bind(present,'sound_send_queue',None)
    labels=[];callbacks=[];callback_errors=[];observations={};token=Q();retained=[]
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    def info():
        out=Info();check(snapshot(C.byref(out))==0,'actual bound sound service state');return out
    def native_out(function):
        out=I(0x12345678);code=function(C.byref(out));return code,out.value
    try:
        check(C.sizeof(Info)==64 and info().bound==0 and frame_server()==0x0d000001,'cold frame sound hook returns actual no-init error instead of diagnostic success')
        frame_queue();check(info().status==2 and native_out(service)==(2,0x12345678),'unbound queue and service preserve output and report unavailable ownership')
        if a.case=='binding':
            context=C.create_string_buffer(b'actual high context');address=C.addressof(context)
            @CB
            def real_service(ctx):
                try:
                    current=info();callbacks.append(dict(context=ctx,active=current.active_kind,inside=current.in_callback,unbind=unbind(token.value),nested=native_out(service),replace=sb(real_service,None,ctx,C.byref(Q()))))
                    return 0
                except BaseException as e:callback_errors.append(repr(e));return -1
            check(sb(None,None,address,C.byref(token))==1 and token.value==0,'missing real service rejected before binding')
            check(sb(real_service,None,address,None)==1,'missing ownership token output rejected')
            check(address>0xffffffff and sb(real_service,None,address,C.byref(token))==0 and token.value>0,'actual high callback context and nonrecycled ownership token')
            wrong=Q(0x1234);check(sb(real_service,None,None,C.byref(wrong))==4 and wrong.value==0x1234,'existing binding cannot be replaced')
            check(native_out(queue)==(7,0x12345678) and info().has_queue==0,'optional unimplemented game request queue reports explicit unavailable')
            check(native_out(service)==(0,0) and not callback_errors and len(callbacks)==1,'actual registered callback executes exactly once')
            event=callbacks[0];check(event['context']==address and event['inside']==1 and event['active']==1 and event['unbind']==event['replace']==5 and event['nested']==(5,0x12345678),'live callback retains context; nested call, replacement and retirement rejected')
            check(frame_server()==0 and info().service_calls==2,'original frame symbol forwards through same single bound owner')
            sentinel=Info();C.memset(C.byref(sentinel),0xa5,64);foreign=[]
            def outsider():foreign.extend([sb(real_service,None,None,C.byref(wrong)),unbind(token.value),native_out(service),native_out(queue),snapshot(C.byref(sentinel))])
            thread=threading.Thread(target=outsider);thread.start();thread.join(5)
            check(not thread.is_alive() and foreign==[3,3,(3,0x12345678),(3,0x12345678),3] and bytes(sentinel)==b'\xa5'*64,'foreign thread cannot inspect, dispatch, replace or retire the bound creator')
            check(unbind(token.value+1)==2 and info().bound and unbind(token.value)==0,'exact token required before callback lifetime release')
            old_token=token.value
            @CB
            def failed(ctx):return 0x06000001
            check(sb(failed,failed,None,C.byref(token))==0 and token.value>old_token,'new session cannot recycle an old binding token')
            check(frame_server()==0x06000001 and info().status==6 and info().native_result==0x06000001,'actual failed service result reaches original SDK-facing symbol')
            frame_queue();check(info().queue_calls==1 and info().failures==2,'void game-facing queue preserves actual callback failure in observable state')
            check(unbind(old_token)==2 and unbind(token.value)==0 and frame_server()==0x0d000001,'stale token cannot retire new session; unbound calls cannot reach retired context')
            observations['callback_events']=callbacks
        else:
            frames=C.CDLL(b['frames_DLL']);system=C.CDLL(b['System_DLL']);fs=C.CDLL(b['GDFS_DLL']);sdk=C.CDLL(b['sound_SDK_DLL']);g2=C.CDLL(str(a.build/'jp_native_g2_audio.dll'));core=C.CDLL(str(a.build/'jp_native_aica.dll'))
            @Source
            def input_source(ctx,samples):C.memset(samples,0,4*C.sizeof(Sample));samples[0]=Sample(1,0,0,0,0,0,0,0);return 1
            for name,args in [('jp_queue_initialize',(8<<20,)),('jp_frames_initialize',(8,128,32<<20)),('jp_textures_initialize',(32,1024,32<<20)),('jp_render_state_initialize',(128,1<<20))]:
                fn=bind(frames,name,I,*([S] if len(args)==1 else [U,U,S] if len(args)==3 else [U,S]));check(fn(*args)==0,'real borrowed render prerequisite: '+name)
            path=C.create_unicode_buffer(audit['media_inputs']['data_track']);options=SystemOptions(C.sizeof(SystemOptions),0,DeviceOptions(640,480,0,0,5000,0,64<<20),C.addressof(path),2352,3,5000,0,C.cast(input_source,P).value,None,None,None)
            config=bind(system,'jp_system_configure',I,P);system_init=bind(system,'jp_system_initialize',I,I,I,I);system_close=bind(system,'jp_system_shutdown',I)
            aica_close=bind(core,'jp_aica_close',I,U);g2close=bind(g2,'jp_g2_close',I);sdk_server=bind(sdk,'sdSysServer',I)
            check(config(C.byref(options))==0 and system_init(1,0,1)==0,'actual compiled system owner creates real window/input/RTC/GDFS and wait services')
            try:
                aopts=AicaOptions(32,0,16384,40,5000,1,1,0)
                check(bind(core,'jp_aica_open',I,P)(C.byref(aopts))==0 and bind(g2,'jp_g2_open',I)()==0 and bind(sdk,'peer_initialize',I)()==0,'actual core/G2/original SDK owner before frame binding')
                setprm=bind(sdk,'sdMemBlkSetPrm',I,P,P,I,P,P);create=bind(sdk,'sdMemBlkCreate',I,P);destroy=bind(sdk,'sdMemBlkDestroy',I,P);drv=bind(sdk,'sdDrvInit',I,P);multi=bind(sdk,'jp_sd_download_multi',I,P)
                check(bind(sdk,'sdMemBlkSetTransferMode',I,I)(1)==0,'actual SDK DMA transfer mode')
                def load(name):
                    handle=bind(fs,'gdFsOpen',P,C.c_char_p,P)(name,None);size=I();check(handle and bind(fs,'gdFsGetFileSize',I,P,P)(handle,C.byref(size))!=0 and size.value>0,'original filesystem opens sound file and reports actual size')
                    sectors=(size.value+2047)//2048;count=sectors*2048;buf=C.create_string_buffer(count+96);C.memset(buf,0xa5,len(buf));base=C.addressof(buf);pointer=(base+63)&~31;offset=pointer-base;retained.append(buf)
                    check(bind(fs,'gdFsRead',I,P,I,P)(handle,sectors,pointer)==0 and buf.raw[:offset]==b'\xa5'*offset and buf.raw[offset+count:]==b'\xa5'*(len(buf)-offset-count),'actual native sector-rounded sound file read preserves full staging guards')
                    bind(fs,'gdFsClose',None,P)(handle);return pointer,size.value
                pointer,size=load(b'masaru.drv');check(hashlib.sha256(C.string_at(pointer,size)).hexdigest()=='5ab700a241e5d5e164c104a76555b35002a6a84370aea17bf8c34d713cdc2cb9','driver loaded through actual original GDFS matches independent retail fingerprint')
                h=P();check(create(C.byref(h))==0 and setprm(h,pointer,0,0xffffffff,None)==0 and drv(h)==0 and destroy(h)==0 and drv(h)==0,'actual original two-driver-init/released-slot sequence on native GDFS staging')
                pointer,size=load(b'emeraldcoast.mlt');check(create(C.byref(h))==0 and setprm(h,pointer,size,0xffffffff,None)==0 and multi(h)==0 and destroy(h)==0,'actual original GDFS->MLT->SDK->G2 bank download')
                port=P();check(bind(sdk,'sdMidiOpenPort',I,P)(C.byref(port))==0,'actual original MIDI port for frame-produced test requests')
                pack=bind(sdk,'sdMidiSetMes',I,P,P,I);send=bind(sdk,'sdMidiSendMes',I,P,P,C.c_int8);request_stage=[1];sequence=[(0xb0,121,0),(0xb0,32,0),(0xc0,0,0),(0xb0,10,64),(0xe0,0,64),(0xb0,1,127),(0xb0,7,106),(0x90,60,127)]
                def midi(s,x,y=0):
                    data=(C.c_uint8*4)();args=(C.c_int(x),) if s&0xf0 in (0xc0,0xa0) else (C.c_int(x),C.c_int(y))
                    return pack(port,data,s,*args) or send(port,data,0)
                @CB
                def producer(ctx):
                    try:
                        callbacks.append(('queue',request_stage[0]));value=0
                        if request_stage[0]==1:
                            value=bind(sdk,'sdSndSetSpace',I,U)(int.from_bytes(b'STRO','little')) or bind(sdk,'sdSndSetFxPrg',I,C.c_int8,C.c_int8)(0,0)
                            for s,x,y in sequence:value=value or midi(s,x,y)
                        elif request_stage[0]==2:value=midi(0x80,60)
                        request_stage[0]=0;return value
                    except BaseException as e:callback_errors.append(repr(e));return -1
                @CB
                def actual_service(ctx):
                    try:callbacks.append(('service',request_stage[0]));return sdk_server()
                    except BaseException as e:callback_errors.append(repr(e));return -1
                context=C.create_string_buffer(b'owned test request source');check(sb(actual_service,producer,C.addressof(context),C.byref(token))==0,'bind original SDK service and explicit real API-request producer to frame owner')
                probe=bind(present,'jp_wait_probe_reset',None,U,U);probe(0,0)
                bind(present,'frame_pacing_init',None,I,I)(1,1);step=bind(present,'frame_wait_step',None,I);interval=bind(present,'frame_set_interval',None,I)
                def tick():
                    interval(1);step(0);check(bind(present,'jp_wait_status',I)()==0 and not callback_errors,'original frame step and native real-clock vblank execute bound callbacks')
                tick();tick();check(callbacks[:4]==[('service',1),('queue',1),('service',0),('queue',0)],'actual frame wait preserves service-before-request order across real vertical ticks')
                get=bind(core,'jp_aica_read',I,U,P,U);render=bind(core,'jp_aica_render',I,U,P,Q)
                def pcm(frames):
                    out=bytearray()
                    while frames:
                        n=min(frames,4093);buf=C.create_string_buffer(n*4);check(render(n,buf,len(buf))==0,'actual original driver consumes frame-produced commands and synthesizes PCM');out+=buf.raw;frames-=n
                    return bytes(out)
                output=pcm(88200);expected=read(a.probe_check/'verification.json')['cases']['physical_split']
                check(hashlib.sha256(output).hexdigest()==expected['PCM_sha256'],'entire frame/GDFS/SDK-produced note equals independent driver reference')
                request_stage[0]=2;tick();tick();tail=pcm(44100)
                check(hashlib.sha256(tail).hexdigest()==expected['noteoff_PCM_sha256'],'entire frame-produced note-off tail equals independent reference')
                memory=C.create_string_buffer(2<<20);check(get(0,memory,len(memory))==0 and memory.raw==(a.driver_check/'driver_dma/private_final_RAM.bin').read_bytes(),'all2MiB driver RAM equals prior independent request-harness baseline')
                check(unbind(token.value)==0,'retire actual callbacks before borrowed SDK/core cleanup');before=len(callbacks)
                check(g2close()==0 and aica_close(5000)==0,'actual transport and core close after callback lifetime ends')
                tick();check(len(callbacks)==before and frame_server()==0x0d000001,'later real vertical tick cannot reach retired SDK/context')
                observations.update(callbacks=callbacks,full_RAM_sha256=hashlib.sha256(memory.raw).hexdigest(),actual_note_frames=88200,actual_tail_frames=44100,request_producer_is_test_client=True,original_game_queue_executed=False)
                (d/'private_output.s16le').write_bytes(output)
            finally:
                unbind(token.value);g2close();aica_close(5000)
                check(system_close()==0,'actual compiled system owner closes filesystem/RTC/input/device/heap after sound test owner')
                check(bind(frames,'jp_frames_shutdown',I)()==0,'release borrowed frame storage');bind(frames,'jp_render_state_shutdown',None)();bind(frames,'jp_textures_shutdown',None)();bind(frames,'jp_queue_shutdown',None)()
        check(not callback_errors,'callback failures are not concealed');v=info();observations['dispatch']={n:getattr(v,n) for n,_ in Info._fields_}
        result=dict(case=a.case,checks=len(labels),assertions=labels,observations=observations);(d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
    finally:unbind(token.value)
if __name__=='__main__':main()
