"""Real original SDK construction, CPU/DMA queues and repeated ARM-driver boot."""
import argparse,ctypes as C,hashlib,json,struct,subprocess,sys
from pathlib import Path
from verify_native_audio import bind
from verify_native_aica import Options,Info as CoreInfo
from verify_native_g2_audio import Info as G2Info,CALLBACK
from build_native_sd_driver import sha
P=C.c_void_p;U=C.c_uint32;I=C.c_int32;Q=C.c_uint64
CASES=('cpu','dma','driver_cpu','driver_dma','driver_control','failed_copy')
def digest(raw):return hashlib.sha256(raw).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','audit','probe_check','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes());audit=json.loads((a.audit/'dependency_inventory.json').read_bytes());probe=json.loads((a.probe_check/'verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    assert sha(b['DLL'])==b['DLL_sha256']
    for field,base in (('authored_source_sha256',r),('original_source_sha256',a.upstream)):
        for n,h in b[field].items():assert sha(base/n)==h,n
    for n,h in b['dependencies'].items():assert sha(a.build/n)==h,n
    if not a.case:
        cases={}
        for case in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','upstream','build','audit','probe_check'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--output',str(d/case),'--case',case]
            run=subprocess.run(cmd,capture_output=True,timeout=80);(d/(case+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(case,run.returncode,(run.stdout+run.stderr).decode(errors='replace'))
            cases[case]=json.loads((d/case/'result.json').read_bytes());print(case,cases[case]['checks'],'checks',flush=True)
        # A CPU-only restart preserves chip/DSP state. Compare both SDK upload
        # paths against an independent two-boot core sequence, not a cold chip.
        ram=(d/'driver_control/private_final_RAM.bin').read_bytes()
        assert (d/'driver_cpu/private_final_RAM.bin').read_bytes()==ram
        assert (d/'driver_dma/private_final_RAM.bin').read_bytes()==ram
        baseline=(a.probe_check/'physical_split/private_final_RAM.bin').read_bytes()
        assert digest(baseline)==probe['cases']['physical_split']['driver_RAM_sha256']
        assert len(ram)==len(baseline)==2<<20
        changed=[i for i,(x,y) in enumerate(zip(baseline,ram)) if x!=y]
        bank=next(x for x in audit['actual_Japanese_multi_units'] if x['name']=='EMERALDCOAST.MLT')
        effect_work=next(x for x in bank['bank_records'] if x['type']=='SFPW')
        assert all(effect_work['sound_memory_offset']<=i<effect_work['sound_memory_offset']+effect_work['allocation_bytes'] for i in changed)
        result=dict(status='compiled_original_SDK_real_constructor_transfer_completion_and_repeated_driver_boot_verified',runtime_checks=sum(x['checks'] for x in cases.values())+5,fresh_process_cases=len(cases),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),probe_reference_sha256=sha(a.probe_check/'verification.json'),
            complete_repeated_original_sdDrvInit_native_executions=4,original_SDK_bank_or_host_command_producers_integrated=False,game_sound_system_init_or_global_audio_exit_integrated=False,
            full_RAM_equal_across_CPU_DMA_and_independent_two_boot_control=True,full_repeated_boot_RAM_sha256=digest(ram),cold_single_boot_RAM_sha256=digest(baseline),
            cold_vs_restarted_chip_changed_RAM_bytes=len(changed),cold_vs_restarted_chip_first_changed_RAM_address=hex(min(changed)) if changed else None,cold_vs_restarted_chip_last_changed_RAM_address=hex(max(changed)) if changed else None,
            cold_vs_restarted_differences_confined_to_actual_effect_work_allocation=True,effect_work_allocation=effect_work,
            physical_console_cycle_accuracy_verified=False,retail_audio_private=True,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    lib=C.CDLL(b['DLL']);g2=C.CDLL(str(a.build/'jp_native_g2_audio.dll'));core=C.CDLL(str(a.build/'jp_native_aica.dll'))
    init=bind(lib,'peer_initialize',I);observe=bind(lib,'peer_observe',I,P);record=bind(lib,'peer_record',I,P,P)
    create=bind(lib,'sdMemBlkCreate',I,P);destroy=bind(lib,'sdMemBlkDestroy',I,P);setprm=bind(lib,'sdMemBlkSetPrm',I,P,P,I,P,P)
    transfer=bind(lib,'sdMemBlkTransfer',I,P,I);mode=bind(lib,'sdMemBlkSetTransferMode',I,I);driver=bind(lib,'sdDrvInit',I,P)
    clear=bind(lib,'sdSndMemClear',I);keep=bind(lib,'sdARM7ResetKeep',I)
    go=bind(g2,'jp_g2_open',I);gx=bind(g2,'jp_g2_close',I);gsnapshot=bind(g2,'jp_g2_snapshot',I,P);service=bind(g2,'jp_g2_service',I,U,P)
    busget=bind(g2,'jp_g2_read',I,U,P,U,I,U,U);busput=bind(g2,'jp_g2_write',I,P,U,U,I,U,U)
    co=bind(core,'jp_aica_open',I,P);cx=bind(core,'jp_aica_close',I,U);csnapshot=bind(core,'jp_aica_snapshot',I,P)
    get=bind(core,'jp_aica_read',I,U,P,U);putcore=bind(core,'jp_aica_write',I,U,P,U);reset=bind(core,'jp_aica_set_arm_reset',I,U);render=bind(core,'jp_aica_render',I,U,P,Q)
    labels=[];retained=[];observations={};callbacks=[];callback_errors=[];entry_calls={}
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    def call(name,fn,*args):entry_calls[name]=entry_calls.get(name,0)+1;return fn(*args)
    def obs():
        out=(Q*20)();check(observe(out)==0,'actual SDK work observation');return list(out)
    def gi():
        out=G2Info();check(gsnapshot(C.byref(out))==0,'real transport counters');return out
    def rec(handle):
        out=(Q*8)();check(record(handle,out)==0,'actual native descriptor observation');return list(out)
    def aligned(raw):
        buf=C.create_string_buffer(len(raw)+63);ptr=(C.addressof(buf)+31)&~31;C.memmove(ptr,raw,len(raw));retained.append(buf);return ptr
    def readmem(at,n):
        out=C.create_string_buffer(n);check(get(at,out,n)==0,'independent actual RAM read');return out.raw
    def make(raw,callback=0xffffffff,context=None):
        handle=P();ptr=aligned(raw)
        check(call('sdMemBlkCreate',create,C.byref(handle))==0 and handle.value>0xffffffff,'original allocator gives actual high native descriptor')
        check(call('sdMemBlkSetPrm',setprm,handle,ptr,len(raw),callback,context)==0,'original setter retains host buffer and callback context')
        return handle.value,ptr
    def finish(budget=64):
        done=U();check(service(budget,C.byref(done))==0,'real posted completions invoke original SDK handler');return done.value
    def pcm(frames):
        data=bytearray()
        while frames:
            n=min(frames,4093);buf=C.create_string_buffer(n*4+32);C.memset(buf,0xa5,len(buf))
            check(render(n,buf,n*4)==0 and buf.raw[n*4:]==b'\xa5'*32,'actual driver rendering preserves caller canaries');data+=buf.raw[:n*4];frames-=n
        return bytes(data)
    try:
        check(init()!=0,'SDK adapter cannot fabricate initialization without real transport')
        options=Options(32,0,16384,40,5000,1,1,0)
        check(co(C.byref(options))==0 and go()==0 and init()==0,'real core, native transport and original sdLibInit construct actual work')
        start=obs();check(start[0]+31<=0x58c and start[1]>0xffffffff and start[2]>0xffffffff and not(start[1]&31) and not(start[2]&31),'measured native work fits original buffer and full pointers align to32')
        check(start[4]==1 and start[5:11]==[0,0,0,0,0,0] and start[18]==0 and gi().channel_open,'original constructor creates primary descriptor, queue and real DMA channel')
        observations['native_work_bytes']=start[0]
        if a.case=='cpu':
            check(call('sdSndMemClear',clear)==0 and readmem(0,2<<20)==b'SEGA'*((2<<20)//4),'original reset/fixed-source CPU fill clears complete real 2MiB RAM')
            h,ptr=make(bytes(range(128)));check(call('sdMemBlkTransfer',transfer,h,0x1000)==0,'original synchronous CPU transfer executes')
            v=obs();check(readmem(0x1000,128)==bytes(range(128)) and rec(h)[1]==1 and rec(h)[6]==1 and v[5:9]==[1,2,0,0] and gi().dma_started==0,'CPU completion copies bytes, sets sync done and preserves original double end count')
            context=aligned(b'full-context')
            @CALLBACK
            def callback(ctx):
                try:callbacks.append(dict(context=ctx,work=obs(),record=rec(h2),bytes=readmem(0x1100,64).hex(),gate=gi().gate_level))
                except BaseException as e:callback_errors.append(repr(e))
            h2,p2=make(bytes((i+73)&255 for i in range(64)),callback,context)
            check(call('sdMemBlkTransfer',transfer,h2,0x1100)==0 and not callback_errors and len(callbacks)==1,'original CPU completion invokes actual native callback once')
            event=callbacks[0];check(event['context']==context and event['work'][7]==h2 and event['work'][6]==3 and event['record'][1]==1 and event['gate']==15,'callback sees finished block, retained current transfer, odd end count and guarded original handler')
            check(obs()[5:9]==[2,4,0,0] and call('sdMemBlkDestroy',destroy,h)==0 and call('sdMemBlkDestroy',destroy,h2)==0 and obs()[4]==1,'CPU epilogue clears transfer and released descriptors preserve primary owner')
        elif a.case=='dma':
            check(call('sdMemBlkSetTransferMode',mode,1)==0,'select original DMA path')
            contexts=[aligned(bytes([i+1])*32) for i in range(3)];handles=[];payloads=[]
            @CALLBACK
            def callback(ctx):
                try:callbacks.append(dict(context=ctx,work=obs(),transport_ended=gi().dma_ended,gate=gi().gate_level))
                except BaseException as e:callback_errors.append(repr(e))
            for i in range(3):
                payload=bytes((i*47+j)&255 for j in range(64));payloads.append(payload)
                h,_=make(payload,callback,contexts[i]);handles.append(h)
                check(call('sdMemBlkTransfer',transfer,h,0x1000+i*64)==0,'original DMA queue accepts actual descriptor')
            check(not callbacks and obs()[5:9]==[1,0,handles[0],2] and gi().busy,'original queue waits behind first pending completion')
            for i in range(3):
                check(finish(1)==1 and not callback_errors and len(callbacks)==i+1,'one actual completion advances original queue exactly once')
                e=callbacks[-1];check(e['context']==contexts[i] and e['work'][7]==handles[i] and e['work'][6]==i*2+1 and e['transport_ended']==i+1 and e['gate']==15,'each original callback precedes transfer clearing and sees actual transport end')
            check(readmem(0x1000,192)==b''.join(payloads) and obs()[5:9]==[3,6,0,0] and gi().dma_started==gi().dma_ended==3,'all queued actual copies drain in order and preserve bookkeeping')
            h,_=make(b'synchronous DMA payload'.ljust(64,b'!'))
            check(call('sdMemBlkTransfer',transfer,h,0x2000)==0 and rec(h)[6]==1 and not gi().busy,'original synchronous DMA wait services actual completion and returns')
            for h in handles+[h]:check(call('sdMemBlkDestroy',destroy,h)==0,'release completed actual SDK descriptor')
            check(obs()[4]==1 and obs()[5:9]==[4,8,0,0] and obs()[19]==0,'healthy DMA path has no ignored G2 errors or leaked descriptor')
        elif a.case.startswith('driver_'):
            desired=int(a.case=='driver_dma');check(call('sdMemBlkSetTransferMode',mode,desired)==0,'select original driver upload mode')
            raw=(a.audit/'private_inputs/MASARU.DRV').read_bytes()
            check(digest(raw)=='5ab700a241e5d5e164c104a76555b35002a6a84370aea17bf8c34d713cdc2cb9','exact original driver header and payload preserved')
            if a.case=='driver_control':
                # Independent low-level owner sequence: no SDK driver loader,
                # descriptor, G2 copy, wait adaptation or constructor data feeds
                # this control's uploads/reset/wait. Chip state survives reset.
                for iteration in range(2):
                    check(reset(1)==0,'independent control asserts real CPU reset')
                    check(putcore(0,C.create_string_buffer(b'SEGA'*((2<<20)//4)),2<<20)==0 and putcore(0,C.create_string_buffer(raw[32:]),len(raw)-32)==0,'independent direct-core full fill and exact program upload')
                    check(reset(0)==0 and not any(pcm(44100)),'independent actual CPU boot runs to silence without SDK loader')
                words=struct.unpack('<6I',readmem(0xc0,24));mapping,driver_status,host=words[3],words[2],words[4]
                observations['independent_direct_core_boots']=2
            else:
                outer,ptr=make(raw);check(call('sdMemBlkSetPrm',setprm,outer,ptr,0,0xffffffff,None)==0,'same zero-length outer descriptor used by original game driver loader')
                check(call('sdDrvInit',driver,outer)==0,'complete compiled SDK uploads, starts and waits for actual original ARM driver')
                first=obs();check(first[4]==2 and first[5:11]==[1,2,0,0,1,44100] and first[11:18]==[0x20,0x13f00,0x13600,0x13400,0x14000,0x13200,0x14800],'first actual boot has exact driver tables, readiness, transfer bookkeeping and live descriptors')
                check(rec(outer)[2]==ptr and rec(outer)[3]==0 and rec(outer)[6]==1,'first boot preserves outer descriptor header and executes its sync completion')
                check(call('sdMemBlkDestroy',destroy,outer)==0 and rec(outer)[0]==0 and rec(outer)[2]==ptr,'original release retains driver header before repeated init')
                check(call('sdDrvInit',driver,outer)==0,'complete repeated SDK driver init executes after outer descriptor release')
                second=obs();alias=rec(outer)
                check(second[4]==1 and second[5:11]==[2,4,0,0,1,88200] and alias[0]==0 and alias[2]==ptr+32 and alias[3]==len(raw)-32 and alias[6]==1,'second init really reallocates released outer slot as internal payload descriptor then releases it')
                check(gi().dma_started==gi().dma_ended==2*desired and second[19]==0 and readmem(0x13400,4)==bytes(4),'both actual boots use selected copy path and ARM clears real ready status')
                mapping,driver_status,host=second[15],second[14],second[16]
                observations.update(first_boot=first,second_boot=second,reused_descriptor=alias)
            # Exercise the newly bootstrapped driver after its complete original
            # init sequence. Bank/command producers remain diagnostic harnesses.
            bank=next(x for x in audit['actual_Japanese_multi_units'] if x['name']=='EMERALDCOAST.MLT');sectors=(bank['bytes']+2047)//2048
            with Path(audit['media_inputs']['data_track']).open('rb') as file:
                file.seek((bank['fad']-45150)*2352);disc=file.read(sectors*2352)
            bankraw=b''.join(disc[j*2352+16:j*2352+2064] for j in range(sectors))[:bank['bytes']]
            check(digest(bankraw)=='362a8b1077e403bf864acbbf756a854e9299f25f9111a5ad26a2add634c25792','actual original stage bank read with independent expected payload hash')
            def put(at,data):check(busput(C.create_string_buffer(data),0xa0800000+at,1,len(data),1,1)==0,'actual native bus copies bank/command data')
            def word(at):return struct.unpack('<I',readmem(at,4))[0]
            offsets={'SMSB':0,'SMPB':0x80,'SOSB':0x100,'SFPB':0x200,'SFOB':0x280,'SFPW':0x288}
            for row in bank['bank_records']:
                if row['allocation_bytes']>=0:put(mapping+offsets[row['type']]+row['number']*8,struct.pack('<II',row['sound_memory_offset'],row['allocation_bytes']))
                if row['data_bytes']>0:put(row['sound_memory_offset'],bankraw[row['data_offset']:row['data_offset']+row['data_bytes']])
            def command(kind,*params):return bytes([kind,0,*params])+bytes(14-len(params))
            def midi(kind,x,y=0):return command(0x86,0,0,y,x,0,(kind>>4)&7)
            def send(commands):
                check(word(driver_status)&255==0,'real bootstrapped driver ready for command list')
                put(host,b''.join(commands)+(bytes(16) if len(commands)%2 else b''));put(driver_status,struct.pack('<I',(word(driver_status)&0xffffff00)|1))
            send([command(0x8a,0),command(0x84,0),command(0x82,0),midi(0xb0,121),midi(0xb0,32),midi(0xc0,0),midi(0xb0,10,64),midi(0xe0,0,64),midi(0xb0,1,127),midi(0xb0,7,106),midi(0x90,60,127)])
            output=pcm(88200);check(word(driver_status)&255==0,'actual driver consumes note commands after repeated original init')
            expected=probe['cases']['physical_split'];note_hash=digest(output)
            check(note_hash==expected['PCM_sha256'],'entire note PCM matches independent qualified real-driver baseline')
            send([midi(0x80,60)]);tail=pcm(44100)
            check(digest(tail)==expected['noteoff_PCM_sha256'],'entire note-off PCM matches independent qualified baseline')
            ram=readmem(0,2<<20);(d/'private_final_RAM.bin').write_bytes(ram)
            observations.update(note_PCM_sha256=note_hash,tail_PCM_sha256=digest(tail),RAM_sha256=digest(ram))
            (d/'private_output.s16le').write_bytes(output)
        else:
            count=[];context=aligned(b'failure-context')
            @CALLBACK
            def callback(ctx):count.append(ctx)
            h,_=make(bytes(range(64)),callback,context);before=readmem((2<<20)-32,32)
            error=call('sdMemBlkTransfer',transfer,h,(2<<20)-32)
            check(error!=0 and not count and readmem((2<<20)-32,32)==before,'out-of-range real transfer reports failure without data changes or successful callback')
            check(gi().failures and gi().dma_started==0,'real transport error remains observable even if later SDK work runs')
            # Original bookkeeping can remain PROGRESS on failure. Do not claim
            # whole-owner rollback/recovery until the native sound owner exists.
            observations['failed_original_bookkeeping']=obs();observations['failed_descriptor']=rec(h)
        check(not callback_errors,'actual native callbacks did not conceal Python assertion failures')
        observations['entry_calls']=entry_calls;observations['last_work']=obs();last=gi();observations['transport']={n:getattr(last,n) for n,_ in G2Info._fields_}
        check(gx()==0,'native transport closes after all delivered callbacks')
        ci=CoreInfo();check(csnapshot(C.byref(ci))==0 and ci.phase==2 and ci.resources==1,'transport close leaves actual core ownership with its caller')
        check(cx(5000)==0,'actual core then closes independently')
        result=dict(case=a.case,checks=len(labels),assertions=labels,observations=observations)
        (d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
    finally:
        done=U();service(64,C.byref(done));gx();cx(5000)

if __name__=='__main__':main()
