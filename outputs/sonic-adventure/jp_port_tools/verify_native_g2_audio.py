"""Independent RAM traversal model and actual native transfer/callback tests."""
import argparse,ctypes as C,hashlib,json,struct,subprocess,sys,threading
from pathlib import Path
from verify_native_audio import bind
from verify_native_aica import Options,Info as CoreInfo
from build_native_g2_audio import sha
P=C.c_void_p;U=C.c_uint32;I=C.c_int32;Q=C.c_uint64
RAM=0xa0800000;REG=0xa0700000;CASES=('cpu','dma','core_lifetime')
CALLBACK=C.CFUNCTYPE(None,P)
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('cpu_reads','cpu_writes','dma_started','dma_ended','callbacks','bytes_copied')]+[(n,U) for n in ('ready','status','core_status','creator_thread','channel_open','busy','in_callback','gate_level','configured','failures')]

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    assert sha(b['DLL'])==b['DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['dependencies'].items():assert sha(a.build/n)==h,n
    if not a.case:
        cases={}
        for case in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve()),'--workspace',str(r),'--build',str(a.build.resolve()),'--output',str(d/case),'--case',case]
            run=subprocess.run(cmd,capture_output=True,timeout=40);(d/(case+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(case,run.returncode,(run.stdout+run.stderr).decode(errors='replace'))
            cases[case]=json.loads((d/case/'result.json').read_bytes());print(case,cases[case]['checks'],'checks',flush=True)
        result=dict(status='native_G2_actual_RAM_register_DMA_completion_gate_and_core_lifetime_verified',runtime_checks=sum(x['checks'] for x in cases.values()),fresh_process_cases=len(cases),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),SDK_driver_bootstrap_integrated=False,physical_console_DMA_timing_verified=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    g=C.CDLL(b['DLL']);core=C.CDLL(str(a.build/'jp_native_aica.dll'))
    open_=bind(g,'jp_g2_open',I);close=bind(g,'jp_g2_close',I);snapshot=bind(g,'jp_g2_snapshot',I,P)
    read=bind(g,'jp_g2_read',I,U,P,U,I,U,U);write=bind(g,'jp_g2_write',I,P,U,U,I,U,U)
    dmaopen=bind(g,'jp_g2_dma_open',I,P,U);prepare=bind(g,'jp_g2_dma_prepare',I,P,U,P,I,U,U,U,P,P);trigger=bind(g,'jp_g2_dma_trigger',I,P,U)
    busy=bind(g,'jp_g2_dma_busy',I,P,P);service=bind(g,'jp_g2_service',I,U,P);getgate=bind(g,'jp_g2_gate_get',I);setgate=bind(g,'jp_g2_gate_set',I,I)
    copen=bind(core,'jp_aica_open',I,P);cclose=bind(core,'jp_aica_close',I,U);csnapshot=bind(core,'jp_aica_snapshot',I,P)
    cget=bind(core,'jp_aica_read',I,U,P,U);cput=bind(core,'jp_aica_write',I,U,P,U);render=bind(core,'jp_aica_render',I,U,P,Q)
    labels=[];observations={};callbacks=[];retained=[]
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    def info():
        v=Info();check(snapshot(C.byref(v))==0,'native transport snapshot');return v
    def actual():
        buf=C.create_string_buffer(512);check(cget(0x1000,buf,512)==0,'independent full RAM window read');return buf.raw
    def seed(raw):check(cput(0x1000,C.create_string_buffer(raw),len(raw))==0,'independent full RAM window seed')
    def aligned(raw):
        buf=C.create_string_buffer(len(raw)+63);ptr=(C.addressof(buf)+31)&~31;C.memmove(ptr,raw,len(raw));retained.append(buf);return ptr
    def completed(budget=64):
        done=U(0xdeadbeef);check(service(budget,C.byref(done))==0,'explicit completion service');return done.value
    try:
        check(C.sizeof(Info)==88 and open_()==5 and close()==0,'cold ABI and missing real core rejected')
        options=Options(32,0,16384,40,5000,1,1,0);check(copen(C.byref(options))==0 and open_()==0,'borrow real held-reset core')
        check(info().ready and getgate()==0 and open_()==2,'actual transport ready and second open rejected')
        if a.case=='cpu':
            count=7;baseline=bytes((i*29+7)&255 for i in range(512))
            for width in (1,2,4):
                for source_mode in range(3):
                    for destination_mode in range(3):
                        seed(baseline);host=C.create_string_buffer(bytes((i*11+5)&255 for i in range(512)),512)
                        expected=bytearray(baseline);source=host.raw
                        def offset(i,mode):return 128+(i*width if mode==1 else -i*width if mode==2 else 0)
                        for i in range(count):expected[offset(i,destination_mode):offset(i,destination_mode)+width]=source[offset(i,source_mode):offset(i,source_mode)+width]
                        check(write(C.addressof(host)+128,RAM+0x1080,width,count,source_mode,destination_mode)==0,'real CPU write width/address modes')
                        check(actual()==expected,'write agrees with independent per-unit model including untouched bytes')
                        seed(baseline);C.memset(host,0xa5,512);expected=bytearray(host.raw)
                        for i in range(count):expected[offset(i,destination_mode):offset(i,destination_mode)+width]=baseline[offset(i,source_mode):offset(i,source_mode)+width]
                        check(read(RAM+0x1080,C.addressof(host)+128,width,count,source_mode,destination_mode)==0,'real CPU read width/address modes')
                        check(host.raw==expected,'read agrees with independent per-unit model including canaries')
            out=C.create_string_buffer(b'\xa5'*64,64);before=actual()
            invalid=[(RAM-1,1,1,1,1,1),(RAM+(2<<20)-2,4,1,1,1,1),(RAM,4,-1,1,1,1),
                (RAM,3,1,1,1,1),(RAM,4,0,1,1,1),(RAM,4,0x7fffffff,1,1,1),(RAM,4,2,2,1,1),
                (RAM+0x1000,4,1,3,1,1),(0xffffffff,4,2,1,1,1),(REG,1,1,1,1,7),(REG,4,2,1,1,7)]
            for bus,width,n,sm,dm,error in invalid:
                check(read(bus,out,width,n,sm,dm)==error and out.raw==b'\xa5'*64,'invalid read span preserves entire output')
                check(write(out,bus,width,n,dm,sm)==error and actual()==before,'invalid write span preserves actual RAM')
            check(read(RAM,None,4,1,1,1)==1 and read(RAM,2,4,2,1,2)==1 and read(RAM,0xffffffffffffffff,4,1,1,1)==1,'null and overflowing host traversals rejected before access')
            ci=CoreInfo();check(csnapshot(C.byref(ci))==0,'actual core address observation')
            check(read(RAM,ci.state_address,4,1,1,1)==1 and write(ci.state_address,RAM,4,1,1,1)==1,'borrowed core memory cannot alias host transfer buffers')
            word=U(0);check(read(REG+0x2c00,C.byref(word),4,1,0,0)==0 and word.value==1,'actual held ARM control register')
            marker=struct.pack('<5I',0xe59f0008,0xe3a0107f,0xe5801000,0xeafffffe,0x1000)
            seed(bytes(512));check(write(C.create_string_buffer(marker),RAM,4,5,1,1)==0,'load actual ARM marker program through native G2')
            check(write(C.byref(word),REG+0x2c00,4,1,0,0)==0,'write actual reset register')
            # The preceding read yielded one; explicitly release rather than
            # confusing the observed value with the next requested value.
            word.value=0;check(write(C.byref(word),REG+0x2c00,4,1,0,0)==0,'release actual ARM through original register address')
            pcm=C.create_string_buffer(441*4);check(render(441,pcm,len(pcm))==0 and actual()[:4]==struct.pack('<I',127),'real core executes bus-loaded instructions after reset release')
            sentinel=Info();C.memset(C.byref(sentinel),0xa5,88);foreign=[]
            def outsider():foreign.extend([open_(),read(RAM,out,4,1,1,1),write(out,RAM,4,1,1,1),close(),snapshot(C.byref(sentinel)),getgate(),setgate(15)])
            thread=threading.Thread(target=outsider);thread.start();thread.join(5)
            check(not thread.is_alive() and foreign==[3,3,3,3,3,-3,3] and bytes(sentinel)==b'\xa5'*88,'foreign thread cannot access, change, gate or close creator core transport')
        elif a.case=='dma':
            handle=P();check(dmaopen(C.byref(handle),1)==7 and not handle.value,'unsupported expansion channel is explicit')
            check(dmaopen(C.byref(handle),0)==0 and handle.value>0xffffffff,'real native-width channel handle')
            other=P(0x1234);check(dmaopen(C.byref(other),0)==2 and other.value==0x1234,'duplicate open preserves output')
            first=bytes(range(64));second=bytes((i+91)&255 for i in range(64));p1=aligned(first);p2=aligned(second);context=aligned(b'context!')
            callback_errors=[]
            @CALLBACK
            def callback(ctx):
                try:
                    v=info();callbacks.append(dict(context=ctx,ended=v.dma_ended,started=v.dma_started,busy=v.busy,in_callback=v.in_callback,close=close(),data=actual()[:128].hex()))
                    if len(callbacks)==1:
                        check(prepare(handle,RAM+0x1040,p2,64,0,1,1,callback,context)==0 and trigger(handle,0)==0,'callback really prepares and starts next transfer after idle/end publication')
                except BaseException as e:callback_errors.append(repr(e))
            check(prepare(handle,RAM+0x1000,p1+1,64,0,1,1,callback,context)==1,'misaligned host is rejected without 32-bit pointer truncation')
            check(prepare(handle,RAM+0x1000,p1,64,0,1,1,callback,context)==0,'prepare actual root-to-sound transfer')
            check(trigger(handle,2)==7 and info().dma_started==0,'unimplemented interrupt trigger cannot silently run')
            seed(bytes(512));check(trigger(handle,0)==0 and actual()[:64]==first and not callbacks,'data copied before callback delivery')
            C.memset(p1,0x7f,64);flag=U();check(busy(handle,C.byref(flag))==0 and flag.value==1,'busy retained until actual completion service')
            check(trigger(handle,0)==6 and prepare(handle,RAM+0x1000,p1,64,0,1,1,None,None)==6 and close()==6,'pending transfer cannot be replaced or retired')
            check(setgate(15)==0 and getgate()==15 and setgate(15)==0,'nested SDK mask retains real callback gate')
            done=U(99);check(service(1,C.byref(done))==6 and done.value==99 and not callbacks,'completion cannot interrupt guarded SDK bookkeeping')
            check(setgate(0)==0 and not callbacks,'restoring gate does not unexpectedly deliver callback inside original epilogue')
            check(completed(1)==1 and not callback_errors and len(callbacks)==1,'one completion budget preserves next queued transfer')
            check(info().busy and completed()==1 and not callback_errors and len(callbacks)==2,'next actual copy completes on later service')
            check(actual()[:128]==first+second and completed()==0,'both exact payloads persist and completion not repeated')
            check(all(x['context']==context and x['busy']==0 and x['in_callback']==1 and x['close']==6 for x in callbacks),'callback sees idle/end state with full native context and cannot free live dispatch')
            check([(x['started'],x['ended']) for x in callbacks]==[(1,1),(2,2)],'end counters increment before callbacks')
            dest=aligned(b'\xa5'*64);check(prepare(handle,RAM+0x1040,dest,64,1,0,0,None,None)==0 and trigger(handle,0)==0,'actual sound-to-root DMA copy with no callback')
            check(C.string_at(dest,64)==second and completed()==1 and info().dma_ended==3 and info().callbacks==2,'reverse DMA data and no-callback completion')
            observations['callbacks']=callbacks
        else:
            check(cclose(5000)==0,'explicitly exercise caller violation: borrowed core closed first')
            out=C.create_string_buffer(b'\xa5'*16,16)
            check(read(RAM,out,4,1,1,1)==5 and out.raw==b'\xa5'*16,'absent core cannot be reported as successful transfer')
        final=info();observations['transport']={n:getattr(final,n) for n,_ in Info._fields_}
        check(close()==0 and not info().ready and close()==0,'transport releases staging/gate and repeated close is harmless')
        if a.case!='core_lifetime':
            ci=CoreInfo();check(csnapshot(C.byref(ci))==0 and ci.phase==2 and ci.resources==1,'transport close preserves borrowed real core')
        check(cclose(5000)==0,'real core finally closes separately')
        result=dict(case=a.case,checks=len(labels),assertions=labels,observations=observations)
        (d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
    finally:
        # Failure evidence remains. Service valid pending work before releasing
        # the borrowed resource; never force a callback owner to disappear.
        setgate(0);done=U();service(64,C.byref(done));close();cclose(5000)

if __name__=='__main__':main()
