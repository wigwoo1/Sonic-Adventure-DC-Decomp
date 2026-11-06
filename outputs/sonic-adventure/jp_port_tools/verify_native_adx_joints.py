"""Original instruction contracts, independent FIFO, and real deferred DMA ownership."""
import argparse, ctypes as C, hashlib, json, random, re, struct, sys
from pathlib import Path
from model_reference import make_reference
from verify_native_audio import bind
from verify_native_aica import Options
from verify_native_g2_audio import Info as G2Info, CALLBACK

I, U, Q, P = C.c_int32, C.c_uint32, C.c_uint64, C.c_void_p
class Chunk(C.Structure):
    _fields_ = [('data', P), ('len', I)]
class GuardChunk(C.Structure):
    _fields_ = [('before', Q), ('chunk', Chunk), ('after', Q)]
class Ring(C.Structure):
    _fields_ = [('vtbl', P), ('used', I), ('uuid', P), ('data_len', I), ('free_len', I),
               ('free_pos', I), ('data_pos', I), ('buf', P), ('bsize', I), ('xsize', I), ('err', P), ('obj', P)]
class Memory(C.Structure):
    _fields_ = [('vtbl', P), ('used', I), ('uuid', P), ('rest', I), ('pos', I),
               ('buf', P), ('bsize', I), ('err', P), ('obj', P)]

def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def main():
    ap = argparse.ArgumentParser()
    for name in ('workspace', 'upstream', 'build', 'binary', 'output'):
        ap.add_argument('--'+name, type=Path, required=True)
    a = ap.parse_args(); r=a.workspace.resolve(); p=r/'outputs/sonic-adventure'; d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure'); d.mkdir()
    b=json.loads((a.build/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items(): assert sha(r/n)==h,n
    for n,h in b['joints_original_source_sha256'].items(): assert sha(a.upstream/n)==h,n
    assert sha(a.build/'external_adx_joints.c')==b['joints_generated_source_sha256']
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256']
    assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    sys.path.insert(0,str(p)); import verify_startup as executor
    reference, refhash=make_reference(executor)
    table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20); raw=a.binary.read_bytes(); ram[0x10000:0x10000+len(raw)]=raw
    lib=C.CDLL(b['game_sound_DLL']); g2=C.CDLL(str(a.build/'jp_native_g2_audio.dll')); core=C.CDLL(str(a.build/'jp_native_aica.dll'))
    getgate=bind(g2,'jp_g2_gate_get',I); setgate=bind(g2,'jp_g2_gate_set',I,I)
    checks=0; calls={}; sections={}; mask=0; rng=random.Random(0x534a3634); retained=[]
    def check(ok,label):
        nonlocal checks
        assert ok,label
        checks+=1
    def put(at,data):
        offset=at&0xffffff; ram[offset:offset+len(data)]=data
    def get(at,n):
        offset=at&0xffffff; return bytes(ram[offset:offset+n])
    def original(name,*args):
        nonlocal mask
        cpu=executor.State(); cpu.pc=table[name]; cpu.pr=0x8cfed000; cpu.r[15]=0x8cf00000; cpu.sr=0x70000000|(mask<<4)
        for i in range(8,15): cpu.r[i]=0x12340000+i
        saved=list(cpu.r[8:])
        for i,v in enumerate(args):
            if i<4: cpu.r[4+i]=v&0xffffffff
            else: put(cpu.r[15]+4*(i-4),struct.pack('<I',v&0xffffffff))
        reference(ram,cpu,instruction_budget=200000)
        check(cpu.pc==0x8cfed000 and list(cpu.r[8:])==saved,('complete original CPU/stack',name))
        mask=(cpu.sr>>4)&15; calls[name]=calls.get(name,0)+1
        return cpu.r[0]&0xffffffff
    # Authored guest error callback: store the error, actual SR, and call count.
    # It executes SH-4 instructions; no original routine is intercepted.
    callback_at=0x8cfe0000; context_at=0x8cd60000
    put(callback_at,struct.pack('<8H',0x2452,0x0002,0x1401,0x5042,0x7001,0x1402,0x000b,0x0009))
    put(context_at,bytes(16)); context=(I*4)()
    ERROR=C.CFUNCTYPE(None,P,I)
    @ERROR
    def error(obj,code):
        v=(I*4).from_address(obj); v[0]=code; v[1]=getgate(); v[2]+=1
    retained.append(error)
    buf_at=0x8cd10000; buf=(C.c_ubyte*20000)(); buf_ptr=C.addressof(buf)
    def seedbuf():
        data=rng.randbytes(C.sizeof(buf)); C.memmove(buf,data,len(data)); put(buf_at,data)
    def host(at):
        if at==0: return 0
        assert buf_at<=at<=buf_at+C.sizeof(buf),hex(at)
        return buf_ptr+at-buf_at
    def address(name): return C.cast(getattr(lib,name),P).value
    def errors():
        v=struct.unpack('<4i',get(context_at,16))
        check((context[0],context[2],context[3])==(v[0],v[2],v[3]),'real native and instruction callback error/context/count')
        if context[2]: check(context[1]==(v[1]>>4)&15,'callback observes the same actual held interrupt level')
    def buffers():
        check(bytes(buf)==get(buf_at,C.sizeof(buf)),'complete ring/memory bytes and external canaries')
        check(getgate()==mask,'native real callback gate matches original SR mask')
        errors()
    chunks=[GuardChunk() for _ in range(4)]
    sentinel=0x76543210aabbccdd
    def chunk_at(i): return 0x8cd50000+i*32
    def setchunk(i,at,length):
        chunks[i].before=chunks[i].after=sentinel; chunks[i].chunk.data=host(at); chunks[i].chunk.len=length
        put(chunk_at(i),struct.pack('<Ii',at,length)+b'\xa5'*24)
    def chunkcheck(i):
        at,length=struct.unpack('<Ii',get(chunk_at(i),8)); n=chunks[i]
        check(n.before==n.after==sentinel and get(chunk_at(i)+8,24)==b'\xa5'*24,'native and original chunk write guards')
        check((n.chunk.data or 0)==host(at) and n.chunk.len==length,'chunk pointer translation and exact byte count')
        return at,length
    def state(kind,guest,native):
        cls=Ring if kind=='SJRBF' else Memory; n=cls.from_address(native)
        v=struct.unpack('<'+('12I' if kind=='SJRBF' else '9I'),get(guest,48 if kind=='SJRBF' else 36))
        check(n.used==v[1]==1,'actual allocated joint remains live')
        check(C.string_at(n.uuid,16)==get(v[2],16),'original UUID bytes')
        if kind=='SJRBF':
            check([n.data_len,n.free_len,n.free_pos,n.data_pos,n.bsize,n.xsize]==[I(v[i]).value for i in (3,4,5,6,8,9)],'all ring scalar fields')
            bp,err,obj=v[7],v[10],v[11]
        else:
            check([n.rest,n.pos,n.bsize]==[I(v[i]).value for i in (3,4,6)],'all memory scalar fields')
            bp,err,obj=v[5],v[7],v[8]
        check(n.buf==host(bp),'native buffer retains its complete host pointer')
        expected_err=address(kind+'_Error') if err==table[kind+'_Error'] else C.cast(error,P).value if err==callback_at else 0
        expected_obj=native if obj==guest else C.addressof(context) if obj==context_at else 0
        check((n.err or 0)==expected_err and (n.obj or 0)==expected_obj,'actual error callback and owner context pointers')
        slotnames=[None,None,None]+[kind+'_'+suffix for suffix in ('Destroy','GetUuid','Reset','GetChunk','UngetChunk','PutChunk','GetNumData','IsGetChunk','EntryErrFunc')]
        gv=struct.unpack('<12I',get(v[0],48)); nv=list((P*12).from_address(n.vtbl))
        check(gv==tuple(0 if name is None else table[name] for name in slotnames),'actual original vtable targets')
        check(nv==[None if name is None else address(name) for name in slotnames],'all real native vtable targets preserve 64-bit addresses')
        buffers()
    def create(kind,size,extra=0):
        at=buf_at+128
        args=(at,size,extra) if kind=='SJRBF' else (at,size)
        guest=original(kind+'_Create',*args)
        native=bind(lib,kind+'_Create',P,P,I,*([I] if kind=='SJRBF' else []))(host(at),size,*([extra] if kind=='SJRBF' else []))
        check(bool(native)==bool(guest),'actual 32-record pool capacity')
        if native: state(kind,guest,native)
        return guest,native
    def configure(kind,guest,native,enabled=True):
        original(kind+'_EntryErrFunc',guest,callback_at if enabled else 0,context_at if enabled else 0)
        bind(lib,kind+'_EntryErrFunc',None,P,P,P)(native,C.cast(error,P) if enabled else None,C.addressof(context) if enabled else None)
        state(kind,guest,native)
    def take(kind,g,n,line,count,index=0):
        setchunk(index,buf_at+1,99)
        original(kind+'_GetChunk',g,line,count,chunk_at(index))
        # Exercise the actual native function table, rather than a shadow dispatch.
        vtbl=(P*12).from_address(P.from_address(n).value)
        C.CFUNCTYPE(None,P,I,I,P)(vtbl[6])(n,line,count,C.byref(chunks[index].chunk))
        result=chunkcheck(index); state(kind,g,n); return result
    def submit(kind,g,n,line,index=0,undo=False):
        name=kind+('_UngetChunk' if undo else '_PutChunk')
        original(name,g,line,chunk_at(index)); bind(lib,name,None,P,I,P)(n,line,C.byref(chunks[index].chunk))
        chunkcheck(index); state(kind,g,n)
    def reset(kind,g,n):
        original(kind+'_Reset',g); bind(lib,kind+'_Reset',None,P)(n); state(kind,g,n)
    def destroy(kind,g,n):
        original(kind+'_Destroy',g); bind(lib,kind+'_Destroy',None,P)(n)
        if n:
            check(C.string_at(n,C.sizeof(Ring if kind=='SJRBF' else Memory))==bytes(C.sizeof(Ring if kind=='SJRBF' else Memory)),'entire native record cleared on destroy')
            check(get(g,48 if kind=='SJRBF' else 36)==bytes(48 if kind=='SJRBF' else 36),'entire original record cleared on destroy')
    def available(kind,g,n,line,wanted):
        out=I(0x12345678); out_at=0x8cd70000; put(out_at,struct.pack('<I',0x12345678))
        value=original(kind+'_IsGetChunk',g,line,wanted,out_at)
        actual=bind(lib,kind+'_IsGetChunk',I,P,I,I,P)(n,line,wanted,C.byref(out))
        check(actual==I(value).value and out.value==struct.unpack('<i',get(out_at,4))[0],'availability exact count and boolean')
        state(kind,g,n)
    opened=False
    try:
        check(C.sizeof(Chunk)==16 and C.sizeof(Ring)==72 and C.sizeof(Memory)==64,'native layouts differ from guest 8/48/36 without truncation')
        check(bind(core,'jp_aica_open',I,P)(C.byref(Options(32,0,16384,40,5000,1,1,0)))==0,'real owned AICA core opened without another output client')
        opened=True; check(bind(g2,'jp_g2_open',I)()==0,'borrow actual live creator-thread AICA transport')
        start=checks; seedbuf()
        for kind in ('SJRBF','SJMEM'):
            original(kind+'_Init'); bind(lib,kind+'_Init',None)()
            handles=[create(kind,2048,36) for _ in range(33)]
            check(handles[-1]==(0,None),'33rd joint rejected, no fake allocation')
            destroy(kind,*handles[12]); g,n=create(kind,2048,36)
            check(g==handles[12][0] and n==handles[12][1],'destroyed pool slot is reused in original order')
            for index,pair in enumerate(handles[:32]):
                if index!=12: destroy(kind,*pair)
            destroy(kind,g,n); destroy(kind,0,None)
            original(kind+'_Finish'); bind(lib,kind+'_Finish',None)()
        sections['both_original_32_slot_pools_layouts_vtables_and_cleanup']=checks-start
        print('joint pools',checks-start,'checks',flush=True)
        start=checks
        for size,extra in ((64,18),(257,36),(2048,36),(16384,64)):
            seedbuf(); g,n=create('SJRBF',size,extra); configure('SJRBF',g,n)
            fifo=bytearray()
            for step in range(240):
                line=0 if not fifo or (len(fifo)<size and rng.randrange(2)) else 1
                count=rng.randrange(0,min(size,255)+1)
                available('SJRBF',g,n,line,count)
                at,length=take('SJRBF',g,n,line,count)
                check(length<=count,'bounded stream request')
                if step%11==0:
                    submit('SJRBF',g,n,line,undo=True)
                elif line==0:
                    data=rng.randbytes(length); put(at,data); C.memmove(host(at),data,length)
                    submit('SJRBF',g,n,1); fifo.extend(data)
                else:
                    check(get(at,length)==bytes(fifo[:length]),'independent FIFO preserves ordered data across mirrored wraps')
                    del fifo[:length]; submit('SJRBF',g,n,0)
                check(bind(lib,'SJRBF_GetNumData',I,P,I)(n,1)==len(fifo),'independent FIFO occupancy')
                check(bind(lib,'SJRBF_GetNumData',I,P,I)(n,0)==size-len(fifo),'no lost or duplicated free buffer ownership')
            for level in (0,3,15):
                check(setgate(level)==0,'native prior gate set'); mask=level
                for invalid in (-1,2,99):
                    take('SJRBF',g,n,invalid,3)
                    available('SJRBF',g,n,invalid,3)
                    setchunk(0,buf_at+128,4); submit('SJRBF',g,n,invalid)
                    setchunk(0,buf_at+128,4); submit('SJRBF',g,n,invalid,undo=True)
                    expected=original('SJRBF_GetNumData',g,invalid)
                    check(bind(lib,'SJRBF_GetNumData',I,P,I)(n,invalid)==I(expected).value==0,'invalid line callback and zero count')
                    state('SJRBF',g,n)
            check(setgate(0)==0,'restore unmasked owner'); mask=0
            reset('SJRBF',g,n)
            # An incorrectly ordered return must report an error, not invent ownership.
            at,length=take('SJRBF',g,n,0,16); setchunk(0,at+1,length)
            submit('SJRBF',g,n,0,undo=True)
            reset('SJRBF',g,n); configure('SJRBF',g,n,False)
            take('SJRBF',g,n,77,1); destroy('SJRBF',g,n)
        sections['original_ring_wrap_mirrors_independent_FIFO_returns_errors_and_nested_masks']=checks-start
        print('ring flow',checks-start,'checks',flush=True)
        start=checks
        for size in (0,1,17,2048):
            seedbuf(); g,n=create('SJMEM',size); configure('SJMEM',g,n)
            position=0
            for step in range(55):
                count=rng.randrange(128); available('SJMEM',g,n,1,count)
                at,length=take('SJMEM',g,n,1,count)
                check(at==buf_at+128+position and length==min(size-position,count),'independent finite memory stream cursor')
                position+=length
                if step%3==0:
                    submit('SJMEM',g,n,1,undo=True); position-=length
                else: submit('SJMEM',g,n,0)
                if step%13==0: reset('SJMEM',g,n); position=0
                take('SJMEM',g,n,0,12); available('SJMEM',g,n,0,0)
            # The original memory joint rewinds/clamps BEFORE reporting a bad offset.
            take('SJMEM',g,n,1,7); setchunk(0,buf_at+129,99); submit('SJMEM',g,n,1,undo=True)
            setchunk(0,buf_at+128,1); submit('SJMEM',g,n,0,undo=True)
            for invalid in (-1,2,99):
                take('SJMEM',g,n,invalid,2); setchunk(0,buf_at+128,1)
                submit('SJMEM',g,n,invalid); submit('SJMEM',g,n,invalid,undo=True)
                available('SJMEM',g,n,invalid,2)
                expected=original('SJMEM_GetNumData',g,invalid)
                check(bind(lib,'SJMEM_GetNumData',I,P,I)(n,invalid)==I(expected).value==0,'memory invalid count reports original callback')
                state('SJMEM',g,n)
            configure('SJMEM',g,n,False); take('SJMEM',g,n,7,1); destroy('SJMEM',g,n)
        sections['original_finite_memory_flow_rewind_clamp_and_error_order']=checks-start
        print('memory flow',checks-start,'checks',flush=True)
        start=checks
        for name in ('SJ_SplitChunk','mwSjSplitChunk'):
            for alias in (0,1,2):
                for length in (0,1,18,36,256):
                    for count in (0,1,17,length,length+10):
                        for i in range(3): setchunk(i,buf_at+128, length if i==0 else 99)
                        first=0 if alias==1 else 1; second=0 if alias==2 else 2
                        original(name,chunk_at(0),count,chunk_at(first),chunk_at(second))
                        bind(lib,name,None,P,I,P,P)(C.byref(chunks[0].chunk),count,C.byref(chunks[first].chunk),C.byref(chunks[second].chunk))
                        for i in range(3): chunkcheck(i)
                        k=min(length,count)
                        check(chunks[first].chunk.len==k and chunks[second].chunk.len==length-k and chunks[second].chunk.data==buf_ptr+128+k,'independent split and both allowed aliases')
        sections['original_split_wrapper_and_both_input_output_aliases']=checks-start
        start=checks
        original('ADXCRS_Init'); bind(lib,'ADXCRS_Init',None)()
        def crs(name):
            original(name); bind(lib,name,None)()
            check(getgate()==mask,'original nested critical section maps the real held host gate')
        for prior in (0,3,15):
            check(setgate(prior)==0,'critical section prior native level'); mask=prior
            for _ in range(24): crs('ADXCRS_Lock')
            for _ in range(24): crs('ADXCRS_Unlock')
        check(setgate(0)==0,'critical section prior restored'); mask=0
        # Preserve and expose the original over-unlock bug, without using it in production.
        for name in ('ADXCRS_Unlock','ADXCRS_Lock','ADXCRS_Init','ADXCRS_Lock','ADXCRS_Unlock'): crs(name)
        check(getgate()==0,'explicit original init recovers excess-unlock depth')
        sections['original_critical_section_depth_restore_and_excess_unlock_semantics']=checks-start
        start=checks
        # A real queued AICA DMA completion must wait until the *outermost*
        # original ADX critical section releases the shared host gate.
        events=[]
        @CALLBACK
        def completion(obj): events.append((obj,getgate()))
        retained.append(completion)
        handle=P(); backing=C.create_string_buffer(128); ptr=(C.addressof(backing)+31)&~31
        payload=bytes(range(64)); C.memmove(ptr,payload,64)
        check(bind(g2,'jp_g2_dma_open',I,P,U)(C.byref(handle),0)==0,'actual AICA DMA channel opened')
        check(bind(g2,'jp_g2_dma_prepare',I,P,U,P,I,U,U,U,P,P)(handle,0xa0801000,ptr,64,0,0,0,completion,0x1234)==0,'actual aligned AICA transfer prepared')
        check(bind(g2,'jp_g2_dma_trigger',I,P,U)(handle,0)==0,'actual AICA RAM transfer posted')
        copied=C.create_string_buffer(64)
        check(bind(core,'jp_aica_read',I,U,P,U)(0x1000,copied,64)==0 and copied.raw==payload,'DMA moved all actual sound-RAM bytes')
        service=bind(g2,'jp_g2_service',I,U,P)
        crs('ADXCRS_Lock'); crs('ADXCRS_Lock')
        for stage in range(2):
            done=U(0xdeadbeef)
            check(service(1,C.byref(done))==6 and done.value==0xdeadbeef and not events,'pending actual callback is deferred by nested original lock')
            crs('ADXCRS_Unlock')
        done=U()
        check(service(1,C.byref(done))==0 and done.value==1 and events==[(0x1234,0)],'exactly one real completion after outer unlock with original context')
        done=U(99); check(service(1,C.byref(done))==0 and done.value==0 and len(events)==1,'completion not duplicated')
        snapshot=G2Info()
        check(bind(g2,'jp_g2_snapshot',I,P)(C.byref(snapshot))==0 and snapshot.callbacks==snapshot.dma_started==snapshot.dma_ended==1 and snapshot.gate_level==0,'actual shared transport counters and gate quiescent')
        sections['original_nested_lock_defers_actual_DMA_completion_and_preserves_real_RAM']=checks-start
    finally:
        if opened:
            setgate(0)
            check(bind(g2,'jp_g2_close',I)()==0,'actual transport closes after completed callbacks')
            check(bind(core,'jp_aica_close',I,U)(0)==0,'actual borrowed AICA owner closes without outstanding resources')
    result=dict(status='original_stream_joint_pools_flow_pointer_vtables_and_actual_interrupt_gate_verified',
        runtime_checks=checks, sections=sections, complete_original_entry_calls=sum(calls.values()), original_entry_calls=calls,
        original_function_hooks=False, guest_error_callback_instructions_executed=True,
        independent_FIFO=True, actual_AICA_DMA_bytes=64, actual_deferred_DMA_callbacks=1,
        native_joint_record_bytes=[72,64], guest_joint_record_bytes=[48,36], native_chunk_bytes=16, guest_chunk_bytes=8,
        build_verification_sha256=sha(a.build/'build_verification.json'), verifier_sha256=sha(__file__), reference_hashes=refhash,
        raw_SDK_interfaces_require_live_creator_thread_audio_owner=True, malformed_raw_pointer_safety_claimed=False,
        ADXT_GDFS_RNA_PCM_streaming_or_playable_game_verified=False, public_artifact_qualified=False,
        playable=False, overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],checks,'checks',sum(calls.values()),'complete original calls',flush=True)

if __name__=='__main__':
    main()
