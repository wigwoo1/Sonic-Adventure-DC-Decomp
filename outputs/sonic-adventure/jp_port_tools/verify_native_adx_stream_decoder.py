"""Real original SJD->ADXB->ADXPD pipeline with partial input and bounded rings."""
import argparse, ctypes as C, hashlib, json, random, re, struct, sys
from pathlib import Path
from model_reference import make_reference
from camera_reference_numeric import service_for
from verify_native_audio import bind
from verify_native_aica import Options
from verify_native_adx_joints import Chunk

I,U,P,H=C.c_int32,C.c_uint32,C.c_void_p,C.c_int16
class SJD(C.Structure):
    _fields_=[('used',C.c_int8),('stat',C.c_int8),('maxnch',C.c_int8),('adxb',P),('sji',P),('sjo',P*2),
              ('ick',Chunk),('ock',Chunk*2)]+[(n,I) for n in ('decnsmpl','decdtlen','decpos','maxdecsmpl','trapnsmpl','trapcnt','trapdtlen')]+[('trapfunc',P),('trapobj',P),('spsd',C.c_ubyte*64)]
def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def bits(value): return struct.unpack('<I',struct.pack('<f',value))[0]
def signed(value): return I(value).value

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','output'): ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args(); r=a.workspace.resolve(); p=r/'outputs/sonic-adventure'; d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure'); d.mkdir()
    b=json.loads((a.build/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items(): assert sha(r/n)==h,n
    for n,h in b['stream_decoder_original_source_sha256'].items(): assert sha(a.upstream/n)==h,n
    for n,h in b['stream_decoder_generated_source_sha256'].items(): assert sha(a.build/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256']
    assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    sys.path.insert(0,str(p)); import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    from sh4_binary32 import multiply,multiply_add,truncate_i32
    reference,refhash=make_reference(executor)
    numeric_path=r/'work/sonic-adventure/build/sh4_vector_numeric.dll'
    assert sha(numeric_path)=='2e56428fadb1903c319da319cf6fa3731839eab1b16163e4e820821d6e4c2db2'
    service=service_for(bind(C.CDLL(str(numeric_path)),'sa_sh4_vector_numeric',I,U,C.POINTER(Fpu),U))
    table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20); raw=a.binary.read_bytes(); ram[0x10000:0x10000+len(raw)]=raw
    lib=C.CDLL(b['game_sound_DLL']); core=C.CDLL(str(a.build/'jp_native_aica.dll')); g2=C.CDLL(str(a.build/'jp_native_g2_audio.dll'))
    checks=0; calls={}; records=[]; sections={}; rng=random.Random(0x534a4443)
    def check(ok,label):
        nonlocal checks
        assert ok,label
        checks+=1
    def put(at,data): offset=at&0xffffff; ram[offset:offset+len(data)]=data
    def get(at,n): offset=at&0xffffff; return bytes(ram[offset:offset+n])
    def original(name,*args):
        cpu=executor.State(); cpu.pc=table[name]; cpu.pr=0x8cfed000; cpu.r[15]=0x8cf00000; cpu.sr=0x70000000
        for i in range(8,15): cpu.r[i]=0x12340000+i
        saved=list(cpu.r[8:])
        for i,v in enumerate(args):
            if i<4: cpu.r[4+i]=v&0xffffffff
            else: put(cpu.r[15]+4*(i-4),struct.pack('<I',v&0xffffffff))
        f=Fpu(); f.fpscr=0x40001
        reference(ram,cpu,fpu=f,vector_instruction=service,instruction_budget=500000)
        check(cpu.pc==0x8cfed000 and list(cpu.r[8:])==saved and cpu.sr&0xf0==0,('whole original pipeline CPU/stack/interrupt return',name))
        calls[name]=calls.get(name,0)+1; return cpu.r[0]&0xffffffff
    def pair(name,g,n,*args):
        original(name,g,*args); bind(lib,name,None,P,*([I]*len(args)))(n,*args)
    check(C.sizeof(SJD)==200,'actual native SJD record retains all 64-bit joints/chunks/callbacks')
    check(bind(core,'jp_aica_open',I,P)(C.byref(Options(32,0,16384,40,5000,1,1,0)))==0,'real AICA owner opens without output client')
    try:
        check(bind(g2,'jp_g2_open',I)()==0,'actual shared callback gate owner')
        start=checks
        # Native variadic formatting must use the host ABI, not SHC stack walking.
        ERROR=C.CFUNCTYPE(None,P,P); messages=[]
        @ERROR
        def error(obj,msg): messages.append((obj,C.string_at(msg)))
        bind(lib,'ADXERR_Init',None)(); bind(lib,'ADXERR_EntryErrFunc',None,P,P)(error,0x123456789)
        # Do not set argtypes on the variadic function: explicitly typed promoted arguments.
        err=lib.ADXERR_CallErrFunc; err.restype=None
        err(C.c_char_p(b'native %s %d %08x'),C.c_char_p(b'ADX'),I(-123),U(0xabcdef01))
        check(messages==[(0x123456789,b'native ADX -123 abcdef01')],'real error formatter preserves native varargs and callback context above 4GB')
        bind(lib,'ADXERR_Finish',None)()
        sections['native_variadic_error_reporting']=checks-start
        # Both finite-memory input and incrementally filled ring input exercise
        # actual output starvation, wrap, compressed-block fragments and traps.
        for mode in ('SJMEM','SJRBF'):
            for nch,total in ((1,1),(1,33),(1,257),(2,31),(2,97),(2,513)):
                start=checks; original('ADXSJD_Init'); bind(lib,'ADXSJD_Init',None)()
                for name in ('SJRBF_Init','SJMEM_Init','ADXCRS_Init'):
                    original(name); bind(lib,name,None)()
                header=bytearray(64); struct.pack_into('>HH',header,0,0x8000,60)
                header[4:8]=bytes([3,18,4,nch]); struct.pack_into('>IIH',header,8,44100,total,500)
                payload=b''.join(struct.pack('>H',rng.randrange(200))+rng.randbytes(16) for _ in range(((total+31)//32)*nch))
                packet=bytes(header)+payload+b'\x80\x01'+bytes(16)
                areas={0x8cd10000:(C.c_ubyte*4096)(),0x8cd20000:(C.c_ubyte*1024)()}
                for at,buf in areas.items():
                    data=b'\xa5'*C.sizeof(buf); C.memmove(buf,data,len(data)); put(at,data)
                def host(at):
                    if not at: return 0
                    for base,buf in areas.items():
                        if base<=at<=base+C.sizeof(buf): return C.addressof(buf)+at-base
                    raise AssertionError(hex(at))
                def seed(at,data): put(at,data); C.memmove(host(at),data,len(data))
                if mode=='SJMEM': seed(0x8cd10080,packet)
                gi=original(mode+'_Create',0x8cd10080,len(packet) if mode=='SJMEM' else 128,*(() if mode=='SJMEM' else (36,)))
                ni=bind(lib,mode+'_Create',P,P,I,*(() if mode=='SJMEM' else (I,)))(host(0x8cd10080),len(packet) if mode=='SJMEM' else 128,*(() if mode=='SJMEM' else (36,)))
                outs=[]; samples=128; stride=160
                for ch in range(nch):
                    at=0x8cd20080+ch*stride*2
                    go=original('SJRBF_Create',at,samples*2,64)
                    no=bind(lib,'SJRBF_Create',P,P,I,I)(host(at),samples*2,64); outs.append((go,no))
                put(0x8cd80000,struct.pack('<'+('I'*nch),*[g for g,n in outs])); native_outs=(P*nch)(*[n for g,n in outs])
                gs=original('ADXSJD_Create',gi,nch,0x8cd80000); ns=bind(lib,'ADXSJD_Create',P,P,I,P)(ni,nch,native_outs)
                check(bool(gs) and bool(ns) and ns>0xffffffff,'actual stream decoder and subordinate original basic decoder created')
                mapping={gi:ni,**{g:n for g,n in outs}}
                trap_at=0x8cfe0000; trap_obj=0x8cd90000; put(trap_obj,bytes(4))
                put(trap_at,struct.pack('<5H',0x6042,0x7001,0x2402,0x000b,0x0009))
                TRAP=C.CFUNCTYPE(None,P); traps=[]
                @TRAP
                def trap(obj): traps.append(obj)
                trap_context=0x123456789
                if total>128:
                    original('ADXSJD_EntryTrapFunc',gs,trap_at,trap_obj)
                    bind(lib,'ADXSJD_EntryTrapFunc',None,P,P,P)(ns,trap,trap_context)
                pair('ADXSJD_Start',gs,ns)
                if total>128: pair('ADXSJD_SetTrapNumSmpl',gs,ns,64)
                pair('ADXSJD_SetMaxDecSmpl',gs,ns,96)
                produced=[[] for _ in range(nch)]; consumed=[[] for _ in range(nch)]; hist=[[0,0] for _ in range(nch)]
                fed=0; lastdt=0; iterations=0; starvation=0; partial=0; output_pressure=0; trap_rearmed=False; wrapped=False
                def same():
                    nv=SJD.from_address(ns); gv=get(gs,144)
                    check((nv.used,nv.stat,nv.maxnch)==tuple(gv[:3]),'actual stream state and maximum channel count')
                    fields=struct.unpack_from('<7i',gv,44)
                    check([getattr(nv,k) for k in ('decnsmpl','decdtlen','decpos','maxdecsmpl','trapnsmpl','trapcnt','trapdtlen')]==list(fields),'all stream sample/input/trap counters match complete original calls')
                    check(nv.sji==mapping[struct.unpack_from('<I',gv,8)[0]],'native input joint ownership')
                    check(list(nv.sjo)[:nch]==[mapping[x] for x in struct.unpack_from('<2I',gv,12)[:nch]],'native channel joint ownership')
                    check(bytes(nv.spsd)==gv[80:144],'preserved SPSD header storage')
                    check(len(traps)==struct.unpack('<I',get(trap_obj,4))[0] and all(x==trap_context for x in traps),'real trap callbacks and complete 64-bit native contexts')
                    for at,buf in areas.items(): check(bytes(buf)==get(at,C.sizeof(buf)),'entire input/output rings, mirrored extra bytes and surrounding guards')
                    for suffix,stored in (('GetStat',gv[1]),('GetDecNumSmpl',fields[0]),('GetDecDtLen',fields[1]),('GetDecPos',fields[2]),('GetTrapCnt',fields[5]),('GetTrapDtLen',fields[6])):
                        name='ADXSJD_'+suffix
                        # The pinned symbol inventory omits several tiny getters;
                        # their actual original record fields remain observable.
                        expected=I(original(name,gs)).value if name in table else stored
                        check(bind(lib,name,I,P)(ns)==expected,'original public stream counter '+suffix)
                    return nv
                def take(g,n,line,count):
                    put(0x8cd70000,b'\xa5'*16); ck=Chunk()
                    original('SJRBF_GetChunk',g,line,count,0x8cd70000)
                    bind(lib,'SJRBF_GetChunk',None,P,I,I,P)(n,line,count,C.byref(ck))
                    at,length=struct.unpack('<Ii',get(0x8cd70000,8))
                    check((ck.data or 0)==host(at) and ck.len==length,'stream transport chunk exact native pointer/count')
                    return at,ck
                def submit(g,n,line,at,ck):
                    put(0x8cd70000,struct.pack('<Ii',at,ck.len))
                    original('SJRBF_PutChunk',g,line,0x8cd70000)
                    bind(lib,'SJRBF_PutChunk',None,P,I,P)(n,line,C.byref(ck))
                def oracle(data):
                    values=[[] for _ in range(nch)]; new=[bits(v[0]*65536) for v in hist]; old=[bits(v[1]*65536) for v in hist]
                    for index in range(len(data)//18):
                        ch=index%nch; gain=(struct.unpack_from('>H',data,index*18)[0]&8191)+1
                        for byte in data[index*18+2:index*18+18]:
                            for code in (byte>>4,byte&15):
                                step=code if code<8 else code-16
                                if nch==1:
                                    value=max(-32768,min(32767,(signed(7334*hist[ch][0]-3283*hist[ch][1])>>12)+gain*step))
                                    hist[ch]=[value,hist[ch][0]]
                                else:
                                    x=multiply_add(bits(gain),bits(step*65536),multiply_add(bits(7334/4096),new[ch],multiply(bits(-3283/4096),old[ch],1)[0],1)[0],1)[0]
                                    value=signed(truncate_i32(x)[0])>>16; old[ch],new[ch]=new[ch],x
                                values[ch].append(value)
                    if nch==2:
                        for ch in range(nch): hist[ch]=[H(signed(truncate_i32(x)[0])>>16).value for x in (new[ch],old[ch])]
                    return values
                same()
                while iterations<500:
                    iterations+=1
                    if mode=='SJRBF' and fed<len(packet):
                        wanted=min([7,9,17,35,36,73][iterations%6],len(packet)-fed)
                        at,ck=take(gi,ni,0,wanted)
                        if ck.len: seed(at,packet[fed:fed+ck.len]); fed+=ck.len
                        if ck.len and ck.len%18: partial+=1
                        submit(gi,ni,1,at,ck)
                    prior=SJD.from_address(ns)
                    before=prior.decnsmpl
                    free_before=bind(lib,'SJRBF_GetNumData',I,P,I)(outs[0][1],0)
                    blocked=prior.stat==2 and prior.decpos<total and free_before<64
                    # ExecHndl is inlined/unlabelled in this original image.
                    # Run the complete real server, including its pool traversal.
                    original('ADXSJD_ExecServer'); bind(lib,'ADXSJD_ExecServer',None)()
                    nv=same()
                    if blocked:
                        check(nv.decnsmpl==before and bind(lib,'SJRBF_GetNumData',I,P,I)(outs[0][1],0)==free_before,'actual output below one ADX block cannot consume samples or claim buffer ownership')
                        output_pressure+=1
                    if nv.decdtlen>lastdt:
                        values=oracle(payload[lastdt:nv.decdtlen]); count=nv.decnsmpl-before
                        for ch in range(nch): produced[ch].extend(values[ch][:count])
                        lastdt=nv.decdtlen
                    elif nv.stat==2: starvation+=1
                    if traps and not trap_rearmed:
                        pair('ADXSJD_SetTrapNumSmpl',gs,ns,-1); trap_rearmed=True
                    # Hold output for several passes to apply actual backpressure.
                    if iterations%7>=3 or nv.stat==3:
                        for ch,(go,no) in enumerate(outs):
                            at,ck=take(go,no,1,2*[13,32,55,96][iterations%4])
                            data=list(struct.unpack('<'+'h'*(ck.len//2),get(at,ck.len)))
                            offset=len(consumed[ch]); check(data==produced[ch][offset:offset+len(data)],'every streaming PCM sample matches independent step-aware recurrence')
                            consumed[ch].extend(data); submit(go,no,0,at,ck)
                            if at+ck.len>0x8cd20080+ch*stride*2+samples*2: wrapped=True
                    if nv.stat==3 and all(len(x)==total for x in consumed): break
                check(iterations<500 and all(len(x)==total for x in consumed),'complete stream reaches DECEND and returns precisely its declared samples')
                check(len(traps)==(1 if total>128 else 0),'trap fires once at the original limit and resumes after rearming')
                check(lastdt==len(payload),'all required complete compressed blocks accounted for')
                pair('ADXSJD_Stop',gs,ns)
                stale_adxb=SJD.from_address(ns).adxb
                original('ADXSJD_Destroy',gs); bind(lib,'ADXSJD_Destroy',None,P)(ns)
                check(C.string_at(ns,4)==bytes(4) and get(gs,4)==bytes(4),'Destroy preserves the original four-byte prefix clear')
                check(SJD.from_address(ns).adxb==stale_adxb,'native destroy does not partially overwrite a 64-bit pointer')
                for kind,g,n in [(mode,gi,ni)]+[('SJRBF',g,n) for g,n in outs]:
                    original(kind+'_Destroy',g); bind(lib,kind+'_Destroy',None,P)(n)
                records.append(dict(input=mode,channels=nch,declared_samples=total,iterations=iterations,partial_feeds=partial,
                                    waiting_passes=starvation,output_backpressure_passes=output_pressure,trap_callbacks=len(traps),mirrored_output_crossing_observed=wrapped,
                                    decoded_bytes=lastdt,checks=checks-start,PCM_sha256=[hashlib.sha256(struct.pack('<'+'h'*len(x),*x)).hexdigest() for x in consumed]))
                print(mode,nch,total,'samples',checks-start,'checks',flush=True)
        check(sum(x['output_backpressure_passes'] for x in records)>0,'actual output starvation exercised, independently of header/input waits')
        # Exhaust the genuine ADXPD pool before creating a stream: native failure
        # must not register a writer through NULL, an original low-address bug.
        start=checks; bind(lib,'ADXSJD_Init',None)(); bind(lib,'SJRBF_Init',None)()
        expanders=[bind(lib,'ADXPD_Create',P)() for _ in range(8)]
        outbuf=(H*160)(); joint=bind(lib,'SJRBF_Create',P,P,I,I)(outbuf,256,64)
        outputs=(P*1)(joint)
        check(all(expanders) and not bind(lib,'ADXSJD_Create',P,P,I,P)(None,1,outputs),'exhausted real subordinate pool safely returns NULL')
        for obj in expanders: bind(lib,'ADXPD_Destroy',None,P)(obj)
        sjd=bind(lib,'ADXSJD_Create',P,P,I,P)(None,1,outputs)
        check(bool(sjd),'failed construction releases actual basic decoder pool slot')
        bind(lib,'ADXSJD_Destroy',None,P)(sjd)
        handles=[bind(lib,'ADXSJD_Create',P,P,I,P)(None,1,outputs) for _ in range(9)]
        check(all(handles[:8]) and handles[8] is None and all(handles[i+1]-handles[i]==200 for i in range(7)),'actual eight-slot SJD pool uses complete native records and refuses ninth stream')
        bind(lib,'ADXSJD_Destroy',None,P)(handles[3]); replacement=bind(lib,'ADXSJD_Create',P,P,I,P)(None,1,outputs)
        check(replacement==handles[3],'actual destroyed streaming slot reused without leaking subordinate decoders')
        for index,obj in enumerate(handles[:8]):
            if index!=3: bind(lib,'ADXSJD_Destroy',None,P)(obj)
        bind(lib,'ADXSJD_Destroy',None,P)(replacement); bind(lib,'SJRBF_Destroy',None,P)(joint)
        sections['subordinate_pool_failure_rollback_and_reuse']=checks-start
    finally:
        check(bind(g2,'jp_g2_close',I)()==0,'real shared transport retires after streaming')
        check(bind(core,'jp_aica_close',I,U)(0)==0,'real AICA owner cleanup after all stream records retire')
    result=dict(status='actual_original_SJD_ADXB_ADXPD_joint_streaming_values_flow_and_cleanup_verified',runtime_checks=checks,
        complete_original_entry_calls=sum(calls.values()),original_entry_calls=calls,original_function_hooks=False,
        cases=records,sections=sections,independent_step_aware_PCM_recurrence=True,native_error_varargs_verified=True,
        native_null_subordinate_creation_rollback_verified=True,original_four_byte_destroy_semantics_preserved=True,
        build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),reference_hashes=refhash,
        real_retail_media_GDFS_ADXT_RNA_PCM_output_or_playable_game_verified=False,public_artifact_qualified=False,
        playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],checks,'checks',sum(calls.values()),'complete original calls',flush=True)
if __name__=='__main__': main()
