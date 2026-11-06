"""Actual game effects/file-bank path in the shared sound/stream/firmware owner.

Disc parsing and RAM/command expectations are independent observations, never
substitute producers or driver acknowledgements. Retail captures stay private.
"""
import argparse,ctypes as C,hashlib,json,struct,subprocess,sys,threading,time,wave
from pathlib import Path
from rna_test_runtime import Runtime,Joints
from adx_media_fixtures import Disc
from inventory_native_effect_banks import parse,OFFSETS
from verify_native_audio import bind
from verify_native_game_stream import Info as StreamInfo
from verify_native_g2_audio import Info as G2Info
I,U,Q,P=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p
class Request(C.Structure):
    _fields_=[('bytes',U),('kind',U),('id',C.c_int64)]+[(n,I) for n in ('se','priority','volume','updates')]+[(n,C.c_float) for n in ('x','y','z','radius')]
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('queue_calls','requests','loads','loaded_bytes','stage_address','stage_capacity')]+[(n,U) for n in ('live_slots','muted','retained_stage','status')]+[(n,I) for n in ('sdk_error','native_error')]
CASES=('lifecycle','banks_cpu','banks_dma','queue_cpu','queue_dma','streams_cpu','streams_dma','reservations','slot_rules','voice_queue','stage_banks')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);ap.add_argument('--reference',type=Path);a=ap.parse_args()
    r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256']
    assert sha(a.build/'external_game_sound.c')==b['effects_generated_source_sha256']
    assert sha(a.build/'sound_owner_effects.c')==b['effects_owner_generated_sha256'];d.mkdir()
    if not a.case:
        cases={}
        for name in CASES:
            ref='queue_cpu' if name=='queue_dma' else 'streams_cpu' if name=='streams_dma' else 'banks_cpu'
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','build','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--output',str(d/name),'--case',name,'--reference',str(d/ref)]
            run=subprocess.run(cmd,capture_output=True,timeout=180,creationflags=subprocess.CREATE_NO_WINDOW)
            (d/(name+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-7000:])
            cases[name]=read(d/name/'result.json');print(name,cases[name]['checks'],'checks',flush=True)
        result=dict(status='original_effects_queue_owned_banks_and_shared_stream_lifetimes_verified',checks=sum(x['checks'] for x in cases.values()),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
            physical_console_cycle_or_waveform_accuracy_verified=False,complete_gameplay_verified=False,retail_captures_published=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True);return
    labels=[];obs={};rt=None;disc=None;pool=False;groups=[];renderers=[]
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    try:
        rt=Runtime(b,a.track,check);lib=rt.lib;frames=rt.frames;fs=C.CDLL(b['GDFS_DLL'])
        check(C.sizeof(Request)==48 and C.sizeof(Info)==72,'native effects request/snapshot ABI')
        infofn=bind(lib,'jp_effects_snapshot',I,P);load=bind(lib,'jp_effects_load_file',I,C.c_char_p,U,I)
        queue=bind(lib,'jp_effects_queue',I);request=bind(lib,'jp_effects_request',I,P,P);control=bind(lib,'jp_effects_control',I,U,C.c_int64,I)
        setmode=bind(lib,'sdMemBlkSetTransferMode',I,I)
        def info():
            x=Info();check(infofn(C.byref(x))==0,'actual shared effects owner snapshot');return x
        def slot(n):
            x=(C.c_int64*13)();check(bind(lib,'peer_effects_slot',I,U,P)(n,x)==0,'actual original shared slot record');return list(x)
        def commands():return rt.peer('peer_commands',8)
        def drain():
            for _ in range(100):
                rt.service();q=commands()
                if not any(q[:3]):return
                rt.render(128)
            raise AssertionError('actual ARM/SDK queue did not drain')
        def req(se=17,kind=0,id=0x123456789abc,priority=1,volume=0,updates=3,xyz=(0,0,0),radius=100):
            x=Request(48,kind,id,se,priority,volume,updates,*xyz,radius);out=I(-99)
            check(request(C.byref(x),C.byref(out))==0,'original effects request via owned native API');return out.value
        def ctl(action,id=0,value=0):check(control(action,id,value)==0,'original effects control '+str(action))
        check(bind(fs,'gdFsChangeDir',I,C.c_char_p)(b'/SONICADV')==0,'actual retail bank directory')
        check(setmode(int(a.case.endswith('_dma')))==0,'actual selected SDK transfer mode')
        check(queue()==0,'initial original QSound/empty-slot queue update');drain()
        if a.case=='lifecycle':
            before=info();sentinel=I(1234)
            invalid=Request(47,0,1,17,0,0,1,0,0,0,1)
            check(request(C.byref(invalid),C.byref(sentinel))==1 and sentinel.value==1234,'invalid request ABI rejected without writing result')
            for field,value in (('kind',6),('se',-1),('id',-1),('volume',128),('updates',2147483647),('x',float('nan'))):
                bad=Request(48,0,1,17,0,0,1,0,0,0,1);setattr(bad,field,value)
                check(request(C.byref(bad),C.byref(sentinel))==1 and sentinel.value==1234,'invalid request field '+field+' rejected')
            no_camera=Request(48,1,1,17,0,0,1,0,0,0,1)
            check(request(C.byref(no_camera),C.byref(sentinel))==5,'spatial request requires actual camera state')
            voice=Request(48,0,1,1275,0,0,1,0,0,0,1)
            check(request(C.byref(voice),C.byref(sentinel))==2 and sentinel.value==1234,'voice-effect request requires actual ready game archive')
            for name,kind,bank in ((b'../EMERALDCOAST.MLT',0,0),(b'',0,0),(b'EMERALDCOAST.MLT',4,0),(b'EMERALDCOAST.MLT',1,16)):
                check(load(name,kind,bank)==1,'invalid bank-load argument rejected before I/O')
            errors=[]
            def foreign():errors.append(infofn(C.byref(Info())))
            t=threading.Thread(target=foreign);t.start();t.join();check(errors==[3],'foreign-thread effects snapshot rejected')
            check(info().loads==before.loads and not info().retained_stage and not info().live_slots,'invalid calls leave real owner storage and slots unchanged')
            check(load(b'NO_SUCH_BANK.MLT',0,0)!=0 and not info().retained_stage,'real open failure retains no staging')
            check(load(b'FISHMISS.ADX',0,0)!=0 and not info().retained_stage,'real non-MLT file rejected and owned staging released')
            check(load(b'EMERALDCOAST.MLT',0,0)==0 and info().loads==1,'valid bank load recovers after rejected requests')
            obs.update(real_failure_recovery=True)
        elif a.case.startswith('banks_'):
            disc=Disc(a.track.resolve());count=payloads=0;loaded=0;hashes=[];total_records=0
            mapping=rt.peer('peer_observe',20)[15];get=bind(lib,'sdSndMemGetBankStat',I,U,C.c_int8,P,P)
            before_dma=G2Info();check(bind(rt.g2,'jp_g2_snapshot',I,P)(C.byref(before_dma))==0,'actual bank transfer counters before corpus')
            for name,(_,size) in sorted(disc.files.items()):
                if not name.endswith('.MLT'):continue
                data=disc.read(name,0,size);parsed=parse(data,lambda ok,label:check(ok,name+': '+label));records=parsed['bank_records']
                expected=bytearray(rt.memory(0,2<<20))
                for rec in records:
                    if rec['allocation_bytes']>=0:struct.pack_into('<ii',expected,mapping+OFFSETS[rec['type']]+rec['number']*8,rec['address'],rec['allocation_bytes'])
                    if rec['data_bytes']>0:
                        off,n,at=rec['data_offset'],rec['data_bytes'],rec['address'];expected[at:at+n]=data[off:off+n];payloads+=1
                check(load(name.encode(),0,0)==0,'original game file-bank loader '+name)
                memory=rt.memory(0,2<<20);check(memory==expected,'all 2 MiB RAM equal independent bank-map/payload oracle '+name)
                for rec in records:
                    if rec['allocation_bytes']>=0:
                        at=I();n=I();check(get(int.from_bytes(rec['type'].encode(),'little'),rec['number'],C.byref(at),C.byref(n))==0 and (at.value,n.value)==(rec['address'],rec['allocation_bytes']),'actual SDK host bank table '+name)
                count+=1;loaded+=size;total_records+=len(records);v=info()
                check(v.loads==count and v.loaded_bytes==loaded and not any((v.stage_address,v.stage_capacity,v.retained_stage,v.status)),'each file releases its owned staging only after real completion')
                hashes.append(dict(file=name,file_sha256=hashlib.sha256(data).hexdigest(),RAM_sha256=hashlib.sha256(memory).hexdigest()))
                if count%16==0:print('verified owned game bank files',count,flush=True)
            check((count,payloads,total_records)==(112,711,2324),'complete retail bank corpus through original game file loaders')
            after_dma=G2Info();check(bind(rt.g2,'jp_g2_snapshot',I,P)(C.byref(after_dma))==0,'actual bank transfer counters after corpus')
            transfers=after_dma.dma_started-before_dma.dma_started
            check(transfers>0 if a.case.endswith('_dma') else transfers==0,'corpus uses selected real CPU/DMA transfer path')
            if a.case.endswith('_dma'):check(hashes==read(a.reference/'result.json')['observations']['files'],'all per-file input and full sound-RAM hashes agree for CPU and DMA loading')
            obs.update(files=hashes,bank_files=count,payloads=payloads,bank_records=total_records,real_DMA_transfers=transfers)
        elif a.case.startswith('queue_'):
            check(load(b'EMERALDCOAST.MLT',0,0)==0,'load real original common effects bank')
            before=commands();idx=req();check(idx==0 and slot(idx)[:5]==[-1,120,0x123456789abc,0x11,17],'once request keeps native-width identity and original 120-update start state')
            check(queue()==0,'original queue produces actual MIDI start')
            check(slot(idx)[1]==120 and slot(idx)[3]==0x10 and commands()[2]==before[2]+7,'first update starts once without consuming the 120-update lifetime')
            # The once request's volume is not flagged; original queue uses 0.
            messages=[(0xb0,32,0),(0xc0,17,0),(0xb0,10,64),(0xe0,0,64),(0xb0,1,127),(0x90,60,127),(0xb0,7,106)]
            for i,(status,x,y) in enumerate(messages):
                expected=struct.pack('<I',0x86)+bytes((0,0,y,x,status&15,(status>>4)&7))+bytes(8)+b'\x01'
                actual=(C.c_uint8*19)();check(bind(lib,'peer_second',I,U,P)((before[3]+i)%256,actual)==0 and bytes(actual)==expected,'independent original MIDI packet '+str(i))
            drain();pcm=bytearray()
            # Use the actual frame-wait dispatch for a logical queue update.
            step=bind(rt.present,'frame_wait_step',None,I);q0=info().queue_calls
            step(0);check(info().queue_calls==q0+1 and slot(idx)[1]==119,'real frame-wait callback advances actual queue exactly once')
            ctl(2,0x123456789abc,-30);ctl(3,0x123456789abc,-64);ctl(4,0x123456789abc,4096)
            check(queue()==0 and slot(idx)[1]==118 and slot(idx)[12]==-30,'actual identity volume/pan/pitch update preserves live slot')
            drain();ctl(5);t0=slot(idx)[1]
            for _ in range(3):check(queue()==0 and slot(idx)[1]==t0,'original muted queue holds update-count lifetime')
            ctl(6);check(queue()==0 and slot(idx)[1]==t0-1,'original unmute resumes same slot timer');drain()
            ctl(1,0x123456789abc);check(queue()==0 and slot(idx)[1:5]==[0,0,0,-1],'original stop-id retires the real slot');drain()
            # A new ring sound exercises the original alternating pan program.
            ring=req(7);check(ring==1 and queue()==0,'real ring request uses next original channel');rt.service()
            for _ in range(690):pcm+=rt.render(128);rt.service()
            check(any(pcm),'original retail bank and ARM firmware generate nonzero effects PCM')
            ctl(1,0x123456789abc);check(queue()==0,'stop original ring request');drain()
            for _ in range(345):pcm+=rt.render(128);rt.service()
            ram=rt.memory(0,2<<20);(d/'private_effects_output.s16le').write_bytes(pcm);(d/'private_effects_RAM.bin').write_bytes(ram)
            if a.case.endswith('_dma'):check(pcm==(a.reference/'private_effects_output.s16le').read_bytes() and ram==(a.reference/'private_effects_RAM.bin').read_bytes(),'complete effects PCM and RAM agree for CPU/DMA execution')
            with wave.open(str(d/'native-JP-original-effects.wav'),'wb') as f:f.setnchannels(2);f.setsampwidth(2);f.setframerate(44100);f.writeframes(pcm)
            obs.update(PCM_sha256=hashlib.sha256(pcm).hexdigest(),RAM_sha256=hashlib.sha256(ram).hexdigest(),frames=len(pcm)//4,private_WAV=str(d/'native-JP-original-effects.wav'))
        elif a.case=='reservations':
            messages=[];ERROR=C.CFUNCTYPE(None,P,P)
            @ERROR
            def error(ctx,text):messages.append(C.string_at(text).decode(errors='replace'))
            rt.keep.append(error);bind(lib,'mwRnaErrEntryFunc',None,P,P)(error,None)
            groups=[Joints(rt,2) for _ in range(4)];renderers=[rt.create(g.handles) for g in groups]
            check(all(renderers) and rt.info().live_renderers==4,'actual four stereo middleware ports own all eight stream buffers')
            drain();before=rt.memory(0,2<<20)
            check(load(b'EMERALDCOAST.MLT',0,0)==11,'bank preflight rejects actual active eight-ring overlap')
            check(rt.memory(0,2<<20)==before and not info().retained_stage and not info().loads,'overlap rejection leaves all sound RAM, callbacks and owned staging unchanged')
            for h,g in zip(renderers,groups):bind(lib,'mwRnaDestroy',None,P)(h);g.retired();g.close()
            groups.clear();renderers.clear();drain()
            check(load(b'EMERALDCOAST.MLT',0,0)==0,'bank load succeeds after actual conflicting buffers retire')
            groups=[Joints(rt,2) for _ in range(3)];renderers=[rt.create(g.handles) for g in groups[:2]]
            check(all(renderers) and rt.info().live_renderers==2,'actual first four channel buffers coexist with the loaded bank map')
            failed=rt.create(groups[2].handles)
            # mwSndOpenPort reports its refusal, then its original mwRnaCreate
            # caller reports failed port creation before rolling back the slot.
            (d/'reservation_refusal.json').write_text(json.dumps(dict(returned_handle=failed,live_renderers=rt.info().live_renderers,messages=messages),indent=2)+'\n')
            check(not failed and rt.info().live_renderers==2 and len(messages)==2,'fifth/sixth channel reservation rejected through original two-level error chain without leaked renderer')
            check(bind(lib,'peer_sndbuf_used',I)()==4,'failed factory keeps all four original free buffer records free')
            groups[2].retired();groups[2].close();groups.pop()
            for h,g in zip(renderers,groups):bind(lib,'mwRnaDestroy',None,P)(h);g.retired();g.close()
            groups.clear();renderers.clear();drain();before=rt.memory(0,2<<20)
            check(load(b'FISHMISS.ADX',3,0)==11 and rt.memory(0,2<<20)==before and not info().retained_stage,'typed program-bank load rejects real wrong-format input before copying')
            obs.update(actual_eight_ring_overlap_rejected=True,actual_bank_then_fifth_ring_rejected=True,partial_factory_allocation_rolled_back=True)
        elif a.case=='slot_rules':
            check(load(b'EMERALDCOAST.MLT',0,0)==0,'real bank for slot-control rules')
            idx=req(kind=3,updates=3);check(slot(idx)[1]==4 and queue()==0 and slot(idx)[1]==4,'new finite request starts with original updates-plus-one convention')
            for remaining in (3,2,1):check(queue()==0 and slot(idx)[1]==remaining,'finite original queue countdown '+str(remaining))
            check(queue()==0 and slot(idx)[1:5]==[0,0,0,-1],'finite request clears identity, flags and effect at zero');drain()
            idx=req(kind=3,updates=2);check(queue()==0,'start refreshable request')
            again=req(kind=3,updates=8);check(again==idx and slot(idx)[1]==9 and slot(idx)[3]==0x10,'refresh finds same high native ID without another start flag')
            check(queue()==0 and slot(idx)[1]==8,'refresh consumes one update on the existing slot');ctl(1,0x123456789abc);check(queue()==0,'retire finite request');drain()
            held=req(kind=2,priority=9,volume=-80);check(queue()==0,'start original held request')
            for _ in range(7):check(queue()==0 and slot(held)[1]==-1,'held request keeps original negative infinite lifetime')
            ctl(0,value=17);check(queue()==0 and slot(held)[4]==-1,'stop by effect number retires held request');drain()
            redraw=I.in_dll(frames,'frame_redrawing');redraw.value=1
            try:check(req()==-1,'original split-frame redraw suppresses new request allocation')
            finally:redraw.value=0
            # Explicit camera fixture uses the real imported pointer, with its
            # actual compiled layout; it is not a replacement camera producer.
            layout=(U*5)();check(bind(lib,'peer_sound_state_layout',I,P)(layout)==0,'actual task-work camera layout')
            camera=P.in_dll(frames,'camera_work');saved=camera.value;work=C.create_string_buffer(layout[0]);camera.value=C.addressof(work)
            try:
                spatial=req(kind=4,xyz=(0,0,0),updates=3);check(queue()==0 and slot(spatial)[12]==0,'spatial falloff is zero at actual camera position')
                moving=req(kind=4,xyz=(600,0,0),updates=3);check(moving==spatial and queue()==0 and slot(spatial)[12]==-127,'same spatial request follows to exact far-distance clamp')
                ctl(7);check(queue()==0 and not info().live_slots,'kill-all plus original queue retires all actual slots')
            finally:camera.value=saved
            drain();obs.update(finite_held_refresh_stop_redraw_and_exact_spatial_endpoints=True)
        elif a.case=='stage_banks':
            disc=Disc(a.track.resolve());load_stage=bind(lib,'sound_load_stage',None)
            set_stage=bind(lib,'stage_set_stage_act',None,C.c_uint8,C.c_uint8);get_stage=bind(lib,'stage_get_stage_act',C.c_uint16)
            set_character=bind(frames,'set_character_number',None,C.c_int16)
            fixtures=((0,'SONICTAILS'),(2,'SONICTAILS'),(3,'KNUCKLES'),(5,'AMY'),(6,'E102'),(7,'BIG'))
            count=info().loads;total=info().loaded_bytes
            # Invoke the compiled legacy game routine on its trusted creator
            # thread. Stage/character are explicit fixtures, not live gameplay.
            for character,suffix in fixtures:
                drain();set_stage(1,0);set_character(character);load_stage()
                names=('EMERALDCOAST.MLT','V_'+suffix+'.MLT','P_'+suffix+'.MLT')
                count+=3;total+=sum(disc.files[n][1] for n in names)
                check(get_stage()==0x100 and info().loads==count and info().loaded_bytes==total,'original beach stage selects correct real character banks '+suffix)
                check(not info().retained_stage and not info().status,'original stage bank closure releases all owned file staging')
            drain();set_stage(9,0);set_character(0);load_stage()
            names=('CASINOPOLIS.MLT','CASINO_SONICTAILS.MLT','CASINO_SONICTAILSVOICE.MLT')
            count+=3;total+=sum(disc.files[n][1] for n in names)
            check(info().loads==count and info().loaded_bytes==total,'original casino branch uses its own Sonic/Tails bank set')
            check(bind(lib,'stage_get_previous_stage_act',C.c_uint16)()==0x100,'actual original stage setter preserves previous stage/act')
            bind(lib,'stage_set_next_stage_act',None,C.c_uint8,C.c_uint8)(1,2)
            check(bind(lib,'stage_get_next_stage_act',C.c_uint16)()==0x102,'actual original next-stage getter/setter shares authoritative state')
            drain();obs.update(real_game_stage_bank_calls=7,owned_file_loads=21,fixture_characters=[x[0] for x in fixtures],actual_stage_gameplay_tested=False)
        elif a.case=='voice_queue':
            check(bind(lib,'jp_adxstm_initialize',I)()==0 and bind(lib,'jp_adxf_initialize',I)()==0 and bind(lib,'jp_adxt_initialize',I)()==0,'real file/archive/player parents for voice handoff')
            bind(frames,'jp_heap_reset',None)();bind(frames,'task_init',None)();pool=True
            check(bind(lib,'jp_game_stream_initialize',I)()==0,'schedule actual voice-owner task');bind(frames,'task_step_all',None)()
            def stream_info():
                value=StreamInfo();check(bind(lib,'jp_game_stream_snapshot',I,P)(C.byref(value))==0,'actual shared music/voice requests');return value
            v=stream_info();check(v.ready,'actual voice player and archive ready')
            idx=req(se=1275);check(queue()==0 and stream_info().voice_request==594,'original effects queue forwards first voice effect through original offset rule')
            check(slot(idx)[1]==0 and not slot(idx)[3]&0x10,'voice handoff clears lifetime and stop-note flag')
            check(queue()==0 and slot(idx)[1:5]==[0,0,0,-1],'following update retires only the temporary effect slot')
            for h in (v.music,v.voice):
                s=(Q*33)();check(bind(lib,'peer_adxt_state',I,P,P)(h,s)==0,'actual game ADXT player for mute readback')
            ctl(5)
            handles=[]
            for h in (v.music,v.voice):
                s=(Q*33)();check(bind(lib,'peer_adxt_state',I,P,P)(h,s)==0,'actual game ADXT owns real RNA renderer');handles.append(s[6])
            check([C.c_int32(rt.state(h)[30]).value for h in handles]==[-960,-960],'effects mute reaches both actual original stream renderers')
            ctl(6);check([C.c_int32(rt.state(h)[30]).value for h in handles]==[-10,0],'effects unmute restores original separate music/voice levels')
            disc=Disc(a.track.resolve());count=struct.unpack_from('<I',disc.read('EVENT_ADX.AFS',4,4))[0]
            bad=Request(48,0,1,count+681,0,0,1,0,0,0,1);out=I(5678);before=stream_info().voice_request
            check(request(C.byref(bad),C.byref(out))==1 and out.value==5678 and stream_info().voice_request==before,'voice request beyond independently parsed archive count cannot replace pending request')
            last=req(se=count+680);check(queue()==0 and stream_info().voice_request==count-1,'last real archive entry is reachable through original effects queue')
            bind(frames,'task_step_all',None)();check(stream_info().voice_request==-1,'actual original task consumes forwarded voice request')
            check(bind(lib,'jp_game_stream_shutdown',I)()==0,'forwarded pending voice closes through real game task destructor')
            obs.update(voice_archive_entries=count,first_voice_effect_index=594,last_voice_effect_index=count-1,actual_both_streams_mute_unmute=True)
        else:
            check(bind(lib,'jp_adxstm_initialize',I)()==0 and bind(lib,'jp_adxf_initialize',I)()==0 and bind(lib,'jp_adxt_initialize',I)()==0,'real stream parent owners')
            bind(frames,'jp_heap_reset',None)();bind(frames,'task_init',None)();pool=True
            check(bind(lib,'jp_game_stream_initialize',I)()==0,'schedule original music/voice task');bind(frames,'task_step_all',None)()
            stream=StreamInfo();check(bind(lib,'jp_game_stream_snapshot',I,P)(C.byref(stream))==0 and stream.ready,'actual original stream task creates shared players')
            check(bind(lib,'jp_game_stream_request_music',I,I)(46)==0 and bind(lib,'jp_game_stream_request_voice',I,I)(0)==0,'original real music and voice requests')
            bind(frames,'task_step_all',None)();pcm=bytearray();loaded=False;accum=0
            for iteration in range(12000):
                rt.service();states=[]
                for h in (stream.music,stream.voice):
                    s=(Q*33)();check(bind(lib,'peer_adxt_state',I,P,P)(h,s)==0,'actual original shared ADXT player');states.append(list(s))
                if not loaded and all(bind(lib,'mwRnaGetPlaySw',I,P)(s[6])==1 for s in states):
                    before=[rt.state(s[6]) for s in states];check(load(b'EMERALDCOAST.MLT',0,0)==0 and load(b'V_SONICTAILS.MLT',0,0)==0 and load(b'P_SONICTAILS.MLT',0,0)==0,'original stage plus Sonic voice/effects banks load during simultaneous music/voice playback')
                    check(before==[rt.state(s[6]) for s in states],'bank loads preserve every real active RNA field and ring pointer')
                    idx=req(7);check(idx>=0 and queue()==0,'actual effect starts beside live music and voice');loaded=True
                if all(s[1]==5 for s in states):break
                pcm+=rt.render(128);accum+=128
                if accum>=735:
                    bind(frames,'task_step_all',None)();check(queue()==0,'logical game update services same effects queue during playback');accum-=735
            check(loaded and all(s[1]==5 for s in states),'complete concurrent original music/voice plus effects playback')
            check(any(pcm),'real combined firmware PCM is nonzero');ram=rt.memory(0,2<<20)
            (d/'private_mixed_output.s16le').write_bytes(pcm);(d/'private_mixed_RAM.bin').write_bytes(ram)
            if a.case.endswith('_dma'):check(pcm==(a.reference/'private_mixed_output.s16le').read_bytes() and ram==(a.reference/'private_mixed_RAM.bin').read_bytes(),'complete music/voice/effects PCM and RAM agree across CPU/DMA')
            with wave.open(str(d/'native-JP-music-voice-effects.wav'),'wb') as f:f.setnchannels(2);f.setsampwidth(2);f.setframerate(44100);f.writeframes(pcm)
            check(bind(lib,'jp_game_stream_shutdown',I)()==0,'original stream task releases shared players/work before parent shutdown')
            obs.update(PCM_sha256=hashlib.sha256(pcm).hexdigest(),RAM_sha256=hashlib.sha256(ram).hexdigest(),frames=len(pcm)//4,private_WAV=str(d/'native-JP-music-voice-effects.wav'))
        rt.close()
        if pool:bind(frames,'task_destroy_all',None)();bind(frames,'jp_heap_reset',None)();pool=False
    finally:
        if disc:disc.close()
        if rt and rt.live:
            for h,g in zip(renderers,groups):bind(rt.lib,'mwRnaDestroy',None,P)(h);g.close()
            if pool:bind(rt.lib,'jp_game_stream_shutdown',I)()
            rt.close()
        if rt and pool:bind(rt.frames,'task_destroy_all',None)();bind(rt.frames,'jp_heap_reset',None)()
    result=dict(case=a.case,checks=len(labels),assertions=labels,observations=obs)
    (d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
if __name__=='__main__':main()
