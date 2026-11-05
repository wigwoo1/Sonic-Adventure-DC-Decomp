"""Original bank/MIDI/queue producers through real ARM synthesis and Windows PCM."""
import argparse,ctypes as C,hashlib,json,struct,subprocess,sys,time
from pathlib import Path
from verify_native_audio import bind,Info as AudioInfo
from verify_native_aica import Options,Info as CoreInfo
from verify_native_g2_audio import Info as G2Info
from build_native_sd_commands import sha
P=C.c_void_p;I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;B=C.c_uint8
CASES=('playback_cpu','playback_dma','playback_output','queues','all_banks','midi_pack','preflight_errors')
OFFSETS={'SMSB':0,'SMPB':0x80,'SOSB':0x100,'SPSR':0x180,'SFPB':0x200,'SFOB':0x280,'SFPW':0x288}
class Command(C.Structure):_fields_=[('command',U),('params',B*14)]
def digest(raw):return hashlib.sha256(raw).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())
def wire(kind,*params):return bytes([kind&255,0,*params])+bytes(14-len(params))
def midiwire(status,x,y=0,port=0):return wire(0x86,0,0,y,x,(port<<4)|(status&15),(status>>4)&7)

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','audit','probe_check','driver_check','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json');audit=read(a.audit/'dependency_inventory.json');probe=read(a.probe_check/'verification.json');driver_check=read(a.driver_check/'verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    assert sha(b['DLL'])==b['DLL_sha256']
    for field,base in (('authored_source_sha256',r),('additional_original_source_sha256',a.upstream)):
        for n,h in b[field].items():assert sha(base/n)==h,n
    for n,h in b['dependencies'].items():assert sha(a.build/n)==h,n
    if not a.case:
        cases={}
        for case in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','upstream','build','audit','probe_check','driver_check'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--output',str(d/case),'--case',case];run=subprocess.run(cmd,capture_output=True,timeout=180)
            (d/(case+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(case,run.returncode,(run.stdout+run.stderr).decode(errors='replace'))
            cases[case]=read(d/case/'result.json');print(case,cases[case]['checks'],'checks',flush=True)
        result=dict(status='original_SDK_MLT_banks_MIDI_host_queues_ARM_ACK_and_actual_muted_output_verified',runtime_checks=sum(x['checks'] for x in cases.values()),fresh_process_cases=len(cases),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),sound_audit_sha256=sha(a.audit/'dependency_inventory.json'),
            probe_reference_sha256=sha(a.probe_check/'verification.json'),driver_reference_sha256=sha(a.driver_check/'verification.json'),
            original_bank_and_MIDI_host_producers_execute=True,synthetic_bank_copies_or_driver_ACKs=False,original_game_sound_system_init_or_global_audio_exit_integrated=False,
            physical_console_waveform_or_cycle_accuracy_verified=False,audible_speaker_capture_verified=False,retail_data_and_generated_audio_private=True,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    lib=C.CDLL(b['DLL']);core=C.CDLL(str(a.build/'jp_native_aica.dll'));g2=C.CDLL(str(a.build/'jp_native_g2_audio.dll'));audio=C.CDLL(str(a.build/'jp_native_audio.dll'))
    initialize=bind(lib,'peer_initialize',I);observe=bind(lib,'peer_observe',I,P);commands=bind(lib,'peer_commands',I,P);second=bind(lib,'peer_second',I,U,P)
    create=bind(lib,'sdMemBlkCreate',I,P);destroy=bind(lib,'sdMemBlkDestroy',I,P);setprm=bind(lib,'sdMemBlkSetPrm',I,P,P,I,P,P);setmode=bind(lib,'sdMemBlkSetTransferMode',I,I);drv=bind(lib,'sdDrvInit',I,P)
    multi=bind(lib,'jp_sd_download_multi',I,P);getbank=bind(lib,'sdSndMemGetBankStat',I,U,C.c_int8,P,P)
    server=bind(lib,'sdSysServer',I);enqueue=bind(lib,'sdSetHostCmd',I,P);space=bind(lib,'sdSndSetSpace',I,U);fx=bind(lib,'sdSndSetFxPrg',I,C.c_int8,C.c_int8)
    portopen=bind(lib,'sdMidiOpenPort',I,P);portclose=bind(lib,'sdModuleClosePort',I,P);pack=bind(lib,'sdMidiSetMes',I,P,P,I);send=bind(lib,'sdMidiSendMes',I,P,P,C.c_int8)
    co=bind(core,'jp_aica_open',I,P);cx=bind(core,'jp_aica_close',I,U);get=bind(core,'jp_aica_read',I,U,P,U);render=bind(core,'jp_aica_render',I,U,P,Q);pump=bind(core,'jp_aica_pump',I,U);drain=bind(core,'jp_aica_drain',I,U)
    csnapshot=bind(core,'jp_aica_snapshot',I,P);asnapshot=bind(audio,'jp_audio_snapshot',I,P)
    go=bind(g2,'jp_g2_open',I);gx=bind(g2,'jp_g2_close',I);gsnapshot=bind(g2,'jp_g2_snapshot',I,P)
    labels=[];observations={};retained=[];entry_calls={};banks=audit['actual_Japanese_multi_units'];track=Path(audit['media_inputs']['data_track'])
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    def call(name,fn,*args):entry_calls[name]=entry_calls.get(name,0)+1;return fn(*args)
    def obs():
        out=(Q*20)();check(observe(out)==0,'actual SDK owner/table observations');return list(out)
    def queue():
        out=(Q*8)();check(commands(out)==0,'actual host queue observations');return list(out)
    def gi():
        out=G2Info();check(gsnapshot(C.byref(out))==0,'actual native transfer observations');return out
    def ram(at,n):
        out=C.create_string_buffer(n+16);C.memset(out,0xa5,len(out));check(get(at,out,n)==0 and out.raw[n:]==b'\xa5'*16,'independent real RAM read and caller canaries');return out.raw[:n]
    def word(at):return struct.unpack('<I',ram(at,4))[0]
    def make(raw):
        buf=C.create_string_buffer(len(raw)+63);ptr=(C.addressof(buf)+31)&~31;C.memmove(ptr,raw,len(raw));retained.append(buf);handle=P()
        check(ptr>0xffffffff and call('sdMemBlkCreate',create,C.byref(handle))==0 and handle.value>0xffffffff,'real high native bank/driver source and original descriptor')
        check(call('sdMemBlkSetPrm',setprm,handle,ptr,len(raw),0xffffffff,None)==0,'original parameter setter with actual aligned source')
        return handle.value,ptr
    def pcm(frames):
        raw=bytearray()
        while frames:
            n=min(frames,4093);out=C.create_string_buffer(n*4+16);C.memset(out,0xa5,len(out));check(render(n,out,n*4)==0 and out.raw[n*4:]==b'\xa5'*16,'real ARM/AICA rendering and caller canaries');raw+=out.raw[:n*4];frames-=n
        return bytes(raw)
    def bank_data(bank):
        sectors=(bank['bytes']+2047)//2048
        with track.open('rb') as file:file.seek((bank['fad']-45150)*2352);raw=file.read(sectors*2352)
        data=b''.join(raw[i*2352+16:i*2352+2064] for i in range(sectors))[:bank['bytes']]
        check(len(data)==bank['bytes'] and data[:4]==b'SMLT','actual complete stage bank read from user media')
        version=data[4];count=struct.unpack_from('<i',data,8)[0];start=16 if version<1 and struct.unpack_from('<i',data,16)[0]!=-1 else 32
        records=[]
        for i in range(count):
            kind,number,address,size,offset,n=struct.unpack_from('<4s5i',data,start+i*32);kind=kind.decode('ascii')
            records.append(dict(type=kind,number=number,sound_memory_offset=address,allocation_bytes=size,data_offset=offset,data_bytes=n))
        check(records==[{k:row[k] for k in records[0]} for row in bank['bank_records']] if records else not bank['bank_records'],'independent packed32bit record parsing matches qualified media audit')
        return data,records
    def oracle(memory,records,data,mapping):
        expected=bytearray(memory)
        for row in records:
            if row['allocation_bytes']>=0:struct.pack_into('<ii',expected,mapping+OFFSETS[row['type']]+row['number']*8,row['sound_memory_offset'],row['allocation_bytes'])
            if row['data_bytes']>0:
                at,n,offset=row['sound_memory_offset'],row['data_bytes'],row['data_offset'];expected[at:at+n]=data[offset:offset+n]
        return bytes(expected)
    def channel():
        port=P();check(call('sdMidiOpenPort',portopen,C.byref(port))==0 and port.value>0xffffffff,'original SDK opens actual native MIDI port');return port.value
    def midi(port,status,x,y=0):
        message=(B*4)();args=() if status&0xf0==0xf0 else (C.c_int(x),) if status&0xf0 in (0xc0,0xa0) else (C.c_int(x),C.c_int(y))
        check(call('sdMidiSetMes',pack,port,message,status,*args)==0 and call('sdMidiSendMes',send,port,message,0)==0,'original variadic MIDI packer and queue producer execute')
    try:
        flags=int(a.case=='playback_output');options=Options(32,flags,16384,40,5000,1,1,0)
        check(C.sizeof(Command)==20 and co(C.byref(options))==0 and go()==0 and initialize()==0,'actual core/transport/original SDK constructor and host command ABI')
        desired=0 if a.case=='playback_cpu' else 1;check(call('sdMemBlkSetTransferMode',setmode,desired)==0,'choose original CPU or DMA transfer path')
        if a.case!='midi_pack':
            driver=(a.audit/'private_inputs/MASARU.DRV').read_bytes();check(digest(driver)=='5ab700a241e5d5e164c104a76555b35002a6a84370aea17bf8c34d713cdc2cb9','exact real original driver fingerprint')
            h,ptr=make(driver);check(call('sdMemBlkSetPrm',setprm,h,ptr,0,0xffffffff,None)==0 and call('sdDrvInit',drv,h)==0,'actual original driver loader executes')
            check(call('sdMemBlkDestroy',destroy,h)==0,'release outer original driver block')
            if a.case.startswith('playback_'):check(call('sdDrvInit',drv,h)==0,'actual second driver load reuses the released original descriptor')
            mapping,status,host=obs()[15],obs()[14],obs()[16]
        if a.case.startswith('playback_') or a.case=='preflight_errors':
            bank=next(x for x in banks if x['name']=='EMERALDCOAST.MLT');data,records=bank_data(bank)
            check(digest(data)=='362a8b1077e403bf864acbbf756a854e9299f25f9111a5ad26a2add634c25792','independent complete stage bank fingerprint')
            expected=oracle(ram(0,2<<20),records,data,mapping);h,_=make(data)
            check(call('sdMultiUnitDownload',multi,h)==0 and ram(0,2<<20)==expected,'real original MLT/bank/transfer path matches independent complete2MiB byte oracle')
            check(call('sdMemBlkDestroy',destroy,h)==0 and obs()[4]==1,'original MLT internal and outer descriptors both released')
        if a.case.startswith('playback_'):
            port=channel();check(call('sdSndSetSpace',space,int.from_bytes(b'STRO','little'))==0 and call('sdSndSetFxPrg',fx,0,0)==0,'original stereo and effect command producers')
            sequence=[(0xb0,121,0),(0xb0,32,0),(0xc0,0,0),(0xb0,10,64),(0xe0,0,64),(0xb0,1,127),(0xb0,7,106),(0x90,60,127)]
            for s,x,y in sequence:midi(port,s,x,y)
            expected_wire=b''.join([wire(0x8a,0),wire(0x84,0),wire(0x82,0),*[midiwire(s,x,y) for s,x,y in sequence]])+bytes(16)
            check(queue()[2]==11 and word(status)==0,'all11 original-produced commands pending before actual server')
            check(call('sdSysServer',server)==0 and ram(host,len(expected_wire))==expected_wire,'original queue pack/flush/native completion copies exact independently expected wire bytes')
            check(queue()[:3]==[0,0,0] and word(status)&255==1 and not gi().busy,'actual transfer callback signals real driver and clears SDK primary busy state')
            if flags:
                remaining=88200;waits=0
                while remaining:
                    n=min(4093,remaining);deadline=time.monotonic()+5
                    while True:
                        error=pump(n)
                        if error!=8:break
                        assert time.monotonic()<deadline,'actual output queue makes bounded progress'
                        waits+=1;time.sleep(.002)
                    check(error==0,'actual SDK-produced note rendered and copied into real Windows output');remaining-=n
                check(drain(5000)==0,'actual device-clock drain completes original SDK note')
                ai=AudioInfo();ci=CoreInfo();check(asnapshot(C.byref(ai))==0 and csnapshot(C.byref(ci))==0,'actual shared output/core observations')
                check(ai.submitted==ai.consumed==ci.submitted==88200 and ai.discarded==ai.queued_frames==0 and ai.session_muted==1 and ai.events>0,'all real SDK note frames delivered once through queried muted Windows session')
                check(ai.source_hash==probe['real_Windows_delivery']['source_hash']==17774991803589956011,'every actual delivered float packet matches independent nonzero note baseline')
                observations.update(actual_output_frames=88200,actual_output_float_hash=ai.source_hash,actual_output_render_events=ai.events,queue_waits=waits)
            else:
                output=pcm(88200);check(digest(output)==probe['cases']['physical_split']['PCM_sha256'],'entire SDK-produced note matches independently qualified driver PCM');(d/'private_output.s16le').write_bytes(output)
            check(word(status)&255==0,'actual ARM consumes SDK-produced note list without synthetic ACK')
            midi(port,0x80,60);check(call('sdSysServer',server)==0 and ram(host,32)==midiwire(0x80,60)+bytes(16),'original note-off packing and padded transfer')
            tail=pcm(44100);check(digest(tail)==probe['cases']['physical_split']['noteoff_PCM_sha256'],'entire original-SDK note-off tail equals independent baseline')
            final=ram(0,2<<20);reference=(a.driver_check/'driver_dma/private_final_RAM.bin').read_bytes()
            check(final==reference and digest(reference)==driver_check['full_repeated_boot_RAM_sha256'],'complete final2MiB equals prior harness-driven repeated-loader reference')
            check(call('sdModuleClosePort',portclose,port)==0,'release actual MIDI port')
            observations.update(note_command_wire_sha256=digest(expected_wire),final_RAM_sha256=digest(final),tail_PCM_sha256=digest(tail))
        elif a.case=='queues':
            source=[]
            for i in range(256):
                command=Command(0x1234008a,(B*14)(*([255 if i&1 else 0]+[(i+j)&255 for j in range(13)])));source.append(bytes(command.params))
                check(call('sdSetHostCmd',enqueue,C.byref(command))==0,'original host ring accepts each of256 real entries')
                out=(B*19)();check(second(i,out)==0 and bytes(out)==struct.pack('<I',command.command)+bytes(command.params)+b'\x01','original secondary record preserves full command, params and pending bit')
            before=queue();check(call('sdSetHostCmd',enqueue,C.byref(command))!=0 and queue()==before and before[2:4]==[256,0],'257th command rejected without overwriting full ring')
            primary=[bytes(16) for _ in range(32)];cursor=0;batches=[]
            while cursor<256:
                remaining=256-cursor;sent=min(32,remaining//2 if remaining%2==0 else remaining)
                for i in range(sent):primary[i]=wire(0x8a,*source[cursor+i])
                count=(sent+1)&~1;expected_wire=b''.join(primary[:count]);before_started=gi().dma_started
                check(call('sdSysServer',server)==0 and ram(host,len(expected_wire))==expected_wire,'original even-batch quirk, primary packing and retained padding bytes match independent queue model')
                cursor+=sent;batches.append(sent)
                check(queue()[2]==256-cursor and gi().dma_started==before_started+1 and word(status)&255==1,'one real DMA list and remaining queue count per server')
                held=queue();started=gi().dma_started
                check(call('sdSysServer',server)==0 and queue()[:6]==held[:6] and gi().dma_started==started,'real driver waiting status prevents premature resend without invented acknowledgement')
                check(queue()[6:8]==[gi().dma_ended]*2,'next original server refreshes both documented SDK counters after prior native completion')
                pcm(441);check(word(status)&255==0,'actual original ARM acknowledges each queued list')
                for i in range(count):primary[i]=bytes([0])+primary[i][1:]
            check(batches==[32]*7+[16,8,4,2,1,1] and queue()[2]==0,'all256 commands drain with original half-count semantics')
            for i in range(256):
                out=(B*19)();check(second(i,out)==0 and not out[18],'original consumed entry pending flag cleared')
            observations['batch_sizes']=batches
        elif a.case=='all_banks':
            expected=ram(0,2<<20);hashes=[];payloads=0
            for bank in banks:
                data,records=bank_data(bank);expected=oracle(expected,records,data,mapping);h,_=make(data)
                check(call('sdMultiUnitDownload',multi,h)==0,'actual original loader downloads complete Japanese MLT: '+bank['name'])
                check(ram(0,2<<20)==expected,'all real RAM bytes agree with independent parsed bank oracle: '+bank['name'])
                for row in records:
                    if row['allocation_bytes']>=0:
                        address=I();size=I();check(getbank(int.from_bytes(row['type'].encode(),'little'),row['number'],C.byref(address),C.byref(size))==0 and address.value==row['sound_memory_offset'] and size.value==row['allocation_bytes'],'original SDK host allocation table equals independent disc record')
                check(call('sdMemBlkDestroy',destroy,h)==0 and obs()[4]==1 and not gi().busy,'MLT leaves no pending transfer or temporary descriptor')
                hashes.append(dict(name=bank['name'],input_sha256=digest(data),complete_RAM_sha256=digest(expected)));payloads+=sum(row['data_bytes']>0 for row in records)
                if len(hashes)%16==0:print('verified complete banks',len(hashes),flush=True)
            check(len(hashes)==112 and gi().failures==0,'all112 audited Japanese MLT files execute through original DMA loader without ignored transport errors')
            observations.update(actual_banks=112,positive_payloads=payloads,banks=hashes)
        elif a.case=='midi_pack':
            ports=[channel() for _ in range(8)];extra=P(0x1234)
            check(call('sdMidiOpenPort',portopen,C.byref(extra))!=0 and extra.value is None,'actual eight-port capacity and output clearing')
            for number,port in enumerate(ports):
                for status_byte in range(256):
                    x=(status_byte*11+number)&255;y=(status_byte*7+number*3)&255;message=(B*4)(*([0xa5]*4));kind=status_byte&0xf0
                    # Preserve the source SDK's SDD_MIDI_MONO_PRES=0xa0,
                    # including its actual one/two-argument classification.
                    args=() if kind==0xf0 else (C.c_int(x),) if kind in (0xc0,0xa0) else (C.c_int(x),C.c_int(y))
                    check(call('sdMidiSetMes',pack,port,message,status_byte,*args)==0,'original native variadic packer executes for every status/port')
                    expected=bytes(4) if kind==0xf0 else bytes([0 if kind in (0xc0,0xa0) else y,x,(number<<4)|(status_byte&15),(status_byte>>4)&7])
                    if bytes(message)!=expected:
                        diagnostic=dict(port=number,status=status_byte,data1=x,data2=y,actual=bytes(message).hex(),expected=expected.hex(),port_bytes=C.string_at(port,8).hex())
                        (d/'midi_mismatch.json').write_text(json.dumps(diagnostic,indent=2)+'\n')
                    check(bytes(message)==expected,'all four original packed bytes equal independent MIDI bit oracle')
            message=(B*4)(*([0xa5]*4));check(call('sdMidiSetMes',pack,None,message,0x90,C.c_int(1),C.c_int(2))!=0 and bytes(message)==b'\xa5'*4,'null MIDI port cannot overwrite output')
            for port in ports:check(call('sdModuleClosePort',portclose,port)==0,'release each actual MIDI port')
            check(channel()==ports[0],'closed port reused by original allocator');observations['packed_port_status_combinations']=2048
        else:
            # Every malformed unit is rejected before its first allocation/copy.
            initial=ram(0,2<<20);work=obs();mutations=[]
            def mutate(offset,fmt,value):
                raw=bytearray(data);struct.pack_into(fmt,raw,offset,value);return bytes(raw)
            positive=next(i for i,row in enumerate(records) if row['data_bytes']>0);at=32+positive*32
            mutations=[data[:31],mutate(0,'<I',0),mutate(8,'<i',0x7fffffff),mutate(at+16,'<i',len(data)),mutate(at+20,'<i',0x7fffffff),mutate(at+8,'<i',-32),mutate(at+12,'<i',2<<20),mutate(at+4,'<i',16)]
            for malformed in mutations:
                h,_=make(malformed);check(call('sdMultiUnitDownload',multi,h)!=0,'malformed unit rejected by native boundary before original loader')
                check(ram(0,2<<20)==initial,'invalid complete unit preserves every real sound RAM byte')
                check(call('sdMemBlkDestroy',destroy,h)==0 and obs()[4]==work[4],'invalid unit preserves pool ownership')
            check(multi(None)!=0 and obs()[5:9]==work[5:9],'invalid descriptor cannot start a transfer')
        observations.update(entry_calls=entry_calls,last_work=obs(),last_queue=queue());v=gi();observations['transport']={n:getattr(v,n) for n,_ in G2Info._fields_}
        check(gx()==0 and cx(5000)==0,'actual transport then owned core/output retire in dependency order')
        closed=CoreInfo();check(csnapshot(C.byref(closed))==0 and not closed.resources and closed.phase==4,'real core and optional output worker fully closed')
        result=dict(case=a.case,checks=len(labels),assertions=labels,observations=observations);(d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
    finally:
        gx();cx(5000)

if __name__=='__main__':main()
