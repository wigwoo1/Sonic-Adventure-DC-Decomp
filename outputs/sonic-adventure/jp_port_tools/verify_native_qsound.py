"""Original QSound instructions, native identities and real ARM consumption.

Guest queue/slot fixtures are explicit test inputs. Original functions execute
without hooks. Native setup uses the real System/GDFS/SDK/driver owners. The
independent raw-list case publishes a command doorbell, never a fabricated ACK.
"""
import argparse,ctypes as C,hashlib,json,re,struct,subprocess,sys,threading
from pathlib import Path
from verify_native_audio import bind
from verify_native_game_sound import Options as SoundOptions,Info as SoundInfo
from verify_native_aica import Options as AicaOptions,Info as AicaInfo
from verify_native_system import Options as SystemOptions,Info as SystemInfo
from verify_native_device_init import Options as DeviceOptions
from verify_native_input_backend import Source,Sample
from verify_native_sound_clock import Options as ClockOptions
from model_reference import make_reference
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p;S=C.c_size_t;ID=C.c_ssize_t
CASES=('packing','copy_ownership','guard_lifecycle','driver_noop','driver_reference','driver_cpu','driver_dma','timed')
POSES=(bytes([0x80|v for v in (0,20,40,60,80,100,124)]),bytes([0x80|v for v in (124,100,80,60,40,20,0)]),bytes([0x80|60]*7))
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def fields(v):return {n:getattr(v,n) for n,_ in v._fields_}
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--reference',type=Path);ap.add_argument('--case',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256'] and sha(a.build/'external_game_sound.c')==b['qsound_generated_source_sha256']
    for n,h in b['qsound_original_source_sha256'].items():assert sha(a.upstream/n)==h,n
    assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    if not a.case:
        cases={}
        for name in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','upstream','build','binary','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--output',str(d/name),'--case',name,'--reference',str(d/'driver_reference')]
            run=subprocess.run(cmd,capture_output=True,timeout=180,creationflags=subprocess.CREATE_NO_WINDOW);(d/(name+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace'))
            cases[name]=read(d/name/'result.json');print(name,cases[name]['checks'],'checks',flush=True)
        result=dict(status='original_QSound_producer_copy_ownership_native64_and_actual_SDK_ARM_path_verified',
            runtime_checks=sum(v['checks'] for v in cases.values()),fresh_process_cases=len(cases),cases=cases,
            original_complete_instruction_calls=sum(v.get('original_complete_calls',0) for v in cases.values()),
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
            original_function_hooks_used=False,synthetic_driver_ACKs=False,native_identity_bits=64,wire_position_bytes=7,
            full_game_request_queue_spatial_acoustic_or_streams_verified=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    frames=C.CDLL(b['frames_DLL']);present=C.CDLL(b['present_DLL']);system=C.CDLL(b['System_DLL']);lib=C.CDLL(b['game_sound_DLL']);core=C.CDLL(str(a.build/'jp_native_aica.dll'));device=C.CDLL(b['device_DLL'])
    native=bind(lib,'jp_sound_set_qsound',I,P,U);sdk=bind(lib,'sdQsndSetPos',I,P);sysclose=bind(system,'jp_system_shutdown',I)
    configure=bind(lib,'jp_sound_configure',I,P);initialize=bind(lib,'jp_sound_initialize',I);close=bind(lib,'jp_sound_shutdown',I,U)
    labels=[];keep=[];observations={};system_live=False;prereqs=False;calls={}
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    def snap(module,name,kind):v=kind();check(bind(module,name,I,P)(C.byref(v))==0,'actual '+name);return v
    def si():return snap(lib,'jp_sound_snapshot',SoundInfo)
    def peer(name,n,*args):out=(Q*n)();check(bind(lib,name,I,*([U] if args else []),P)(*args,out)==0,'passive '+name);return list(out)
    def queue():return peer('peer_commands',8)
    def error():return peer('peer_qsound_error',2)
    def memory(at,n):buf=C.create_string_buffer(n);check(bind(core,'jp_aica_read',I,U,P,U)(at,buf,n)==0,'actual ARM/AICA memory read');return buf.raw
    def render(n):
        result=bytearray()
        while n:
            count=min(n,4093);buf=C.create_string_buffer(count*4);check(bind(lib,'jp_sound_render',I,U,P,Q)(count,buf,len(buf))==0,'real original driver executes and renders');result+=buf.raw;n-=count
        return bytes(result)
    def flush():
        for step in range(100):
            check(bind(lib,'jp_sound_service',I)()==0,'original SDK services real transfer');render(441)
            q=queue();w=peer('peer_observe',20)
            if not any(q[:3]) and not w[7] and not w[8] and memory(w[14],1)!=b'\1':return
        raise AssertionError('actual driver did not accept startup commands')
    def opts(output=0):return SoundOptions(56,0,2<<20,5000,AicaOptions(32,output,16384,40,5000,1,1,0),None)
    def raw_start():check(configure(C.byref(opts()))==0 and initialize()==0,'actual original sound initialization and owned driver');flush()
    def entry(index):buf=C.create_string_buffer(28);check(bind(lib,'peer_qsound_entry',I,U,P)(index,buf)==0,'actual complete second-list entry');return buf.raw
    @Source
    def source(ctx,samples):C.memset(samples,0,4*C.sizeof(Sample));samples[0]=Sample(1,0,0,0,0,0,0,0);return 1
    path=C.create_unicode_buffer(str(a.track.resolve()));options=SystemOptions(96,0,DeviceOptions(640,480,0,0,5000,0,64<<20),C.addressof(path),2352,3,5000,0,C.cast(source,P).value,None,None,None)
    if a.case in ('packing','copy_ownership'):
        sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
        from verify_fpu_cleanup_transfer import Fpu
        ref,refhash=make_reference(executor);raw=a.binary.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(raw)]=raw
        table={name:int(at,16) for at,name in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
        def put(at,data):at&=0xffffff;ram[at:at+len(data)]=data
        def get(at,n):return bytes(ram[(at&0xffffff):(at&0xffffff)+n])
        def word(at):return struct.unpack('<I',get(at,4))[0]
        def original(name,*args):
            cpu=executor.State();cpu.pc=table[name];cpu.pr=0x8cfed000;cpu.r[15]=0x8cf00000;cpu.sr=0x70000000|((sum(calls.values())%16)<<4)
            for i in range(8,15):cpu.r[i]=0x13570000+i
            saved=list(cpu.r[8:]);mask=cpu.sr&0xf0
            for i,value in enumerate(args):cpu.r[4+i]=value&0xffffffff
            f=Fpu();f.fpscr=0x40001;ref(ram,cpu,fpu=f,instruction_budget=1000000);calls[name]=calls.get(name,0)+1
            check(list(cpu.r[8:])==saved and (cpu.sr&0xf0)==mask and cpu.pc==0x8cfed000,'complete unhooked original call preserves stack/callee registers and interrupt mask')
            return C.c_int32(cpu.r[0]).value
        obj=(a.build/'qsound_layout32.obj').read_bytes();machine,count,_,_,_,optional,_=struct.unpack_from('<HHIIIHH',obj);check(machine==0x14c,'actual guest-width compiler layout')
        layout=None
        for j in range(count):
            at=20+optional+j*40;size,offset=struct.unpack_from('<II',obj,at+16)
            if obj[at:at+8].rstrip(b'\0')==b'.rdata' and size>=36:
                values=struct.unpack_from('<9I',obj,offset)
                if values[0]==0x51534c59:layout=values
        check(layout and layout[1:]==(1140,344,28,20,52,8,16,48),'original work/command/slot offsets match compiled assertions')
    try:
        check(native(None,7)==2 and si().phase==0,'owned QSound cannot invent a ready SDK')
        for name,args in [('jp_queue_initialize',(8<<20,)),('jp_frames_initialize',(8,128,32<<20)),('jp_textures_initialize',(32,1024,32<<20)),('jp_render_state_initialize',(128,1<<20))]:
            check(bind(frames,name,I,*([S] if len(args)==1 else [U,U,S] if len(args)==3 else [U,S]))(*args)==0,'actual borrowed render prerequisite '+name)
        prereqs=True;check(bind(system,'jp_system_configure',I,P)(C.byref(options))==0 and bind(system,'jp_system_initialize',I,I,I,I)(1,0,1)==0,'compiled System creates actual native filesystem/wait');system_live=True
        if a.case=='packing':
            work=0x8cd00000;ring=0x8ce00000;data=0x8cf10000
            put(table['sdgWrkPtr'],bytes(4));check(sdk(None)==original('sdQsndSetPos',0)==0x0d000001,'original SDK no-init path rejects before reading null input')
            check(error()[0]==word(table['sdgErrResult']) and error()[1]==C.cast(lib.sdQsndSetPos,P).value and word(table['sdgErrApi'])==table['sdQsndSetPos'],'actual original error trap identifies producer')
            vectors=[bytes((value if i==channel else (i*23)&255) for i in range(7)) for channel in range(7) for value in range(256)]
            for start_at in range(0,len(vectors),256):
                raw_start();check(queue()[2]==0,'real startup commands acknowledged before queue-only vectors')
                pointer=bind(lib,'peer_qsound_second',P)();check(pointer>0xffffffff and bind(lib,'sd2ndHostCmdInitList',I,P,U)(pointer,256)==0,'actual original empty host-ring initializer')
                put(work,bytes(layout[1]));put(table['sdgWrkPtr'],struct.pack('<I',work));put(table['sdgPrintFunc'],bytes(4));put(table['sdgDebugMode'],bytes(4));put(table['position_command'],bytes(20))
                check(original('sd2ndHostCmdInitList',ring,256)==0,'original instructions initialize guest control ring fixture')
                for index,positions in enumerate(vectors[start_at:start_at+256]):
                    buf=C.create_string_buffer(b'G'+positions+b'H');keep[:]=[buf];put(data,b'G'+positions+b'H')
                    use_game=index&1;value=native(C.addressof(buf)+1,7) if use_game else sdk(C.addressof(buf)+1)
                    result=original('sound_set_qsound' if use_game else 'sdQsndSetPos',data+1)
                    expected=struct.pack('<I',0x88)+positions+bytes(7)+bytes(2)+struct.pack('<II',1,0)
                    check(value==result==0 and entry(index)==get(ring+index*28,28)==expected,'all seven-byte SDK/game producer values and full28byte entry equal independent wire oracle and original instructions')
                    check(buf.raw[:9]==b'G'+positions+b'H' and get(data,9)==b'G'+positions+b'H','producer reads exactly declared positions without modifying input/guards')
                    q=queue();check(q[2:5]==[index+1,(index+1)%256,0] and word(work+344+20)==q[2] and word(work+344+24)==q[3],'actual guest/native queue counters and wrap match')
                before=[entry(i) for i in range(256)];buf=C.create_string_buffer(POSES[0]);put(data,POSES[0])
                check(sdk(buf)==original('sdQsndSetPos',data)==0x0a000001 and native(buf,7)==15 and si().phase==3 and si().status==0,'original full queue error becomes explicit nonfatal owned backpressure')
                check([entry(i) for i in range(256)]==before and word(table['sdgErrApi'])==table['sdQsndSetPos'] and error()[1]==C.cast(lib.sdQsndSetPos,P).value,'full original ring cannot overwrite pending commands and preserves producer error identity')
                check(close(5000)==0,'real owner retires unsubmitted queue vectors without sending invalid spatial bytes to firmware')
            observations.update(position_byte_cases=len(vectors),original_calls=calls,reference_hashes=refhash)
        elif a.case=='copy_ownership':
            raw_start();prepare=bind(lib,'peer_qsound_prepare',I,U,I,ID);allocate=bind(lib,'peer_qsound_allocate',I,U,P);free=bind(lib,'peer_qsound_free',I,U)
            def owners():
                out=(Q*140)();check(bind(lib,'peer_qsound_owners',I,P,U)(out,140)==0,'actual native owner records');return list(out)
            def same():
                guest=list(struct.unpack('<140I',get(table['qsound_owners'],560)))
                expected=[0 if x==0 else (C.c_int32((x-1)&0xffffffff).value+1)&((1<<64)-1) for x in guest]
                check(owners()==expected,'entire native copy-owner table equals original logical identities')
            def setslot(slot,se,id):
                check(prepare(slot,se,id)==0,'native source slot receives test request identity')
                record=bytearray(52);struct.pack_into('<i',record,8,C.c_int32(id).value);struct.pack_into('<i',record,16,se);struct.pack_into('<i',record,48,-2);put(table['sound_slots']+slot*52,record)
            def alloc(slot):
                out=I(99);check(allocate(slot,C.byref(out))==0 and out.value==original('qsound_alloc',slot),'actual game copy allocation equals complete original call')
                state=peer('peer_qsound_slot',3,slot);check(C.c_int64(state[2]).value==C.c_int32(word(table['sound_slots']+slot*52+48)).value,'actual original QSound channel/failure is preserved');same();return out.value
            def release(slot):check(free(slot)==0,'native game copy release');original('qsound_free',slot);same()
            put(table['qsound_owners'],bytes(560));ses=struct.unpack('<35h',get(table['qsound_ses'],70));rows=[struct.unpack('<4h',get(table['qsound_info']+row*8,8)) for row in range(35)]
            for se in range(-2,1301):check(bind(lib,'peer_qsound_range',I,I)(se)==original('qsound_range',se),'complete supported/unsupported original QSound range search')
            for row,se in enumerate(ses):
                copies,channel,range_,next_=rows[row]
                for slot in range(copies):setslot(slot,se,row*16+slot+10);alloc(slot)
                setslot(copies,se,77777);check(alloc(copies)==-1,'actual per-effect copy capacity is enforced')
                release(copies)
                for slot in range(copies):release(slot)
            for se in (-5,0,1300):
                setslot(0,se,0x3141);check(alloc(0)==-1,'unsupported effect cannot acquire a spatial copy');release(0)
            se=ses[next(i for i,row in enumerate(rows) if row[0]>=2)]
            for id in (-1,-2,-2147483648,2147483647,0,0x12345678):
                for slot in (0,1):setslot(slot,se,id);alloc(slot)
                release(0);release(1)
            # Native-only pointer-width extension: two labels with identical
            # low32 bits must not alias; uintptr arithmetic defines wrap.
            for ids in ((0x100000042,0x200000042),((1<<63)-1,-(1<<63))):
                for slot,id in enumerate(ids):check(prepare(slot,se,id)==0 and allocate(slot,C.byref(I()))==0,'actual native-width owner allocation')
                encoded=[(id+1)&((1<<64)-1) for id in ids];check(all(value in owners() for value in encoded),'full-width labels and boundary increment survive without signed overflow')
                check(free(1)==0 and encoded[0] in owners() and encoded[1] not in owners(),'freeing same-low32 later identity cannot release another copy');check(free(0)==0 and not any(owners()),'all native copy owners return to free state')
            context=C.create_string_buffer(b'actual native task identity');keep.append(context);address=C.addressof(context)
            check(address>0xffffffff and prepare(0,se,address)==0 and allocate(0,C.byref(I()))==0 and address+1 in owners(),'actual high native context pointer retained as an identity')
            check(close(5000)==0,'retire actual owner with retained test copy');raw_start();check(not any(owners()),'original reset/retirement clears widened owner table on actual restart')
            observations.update(original_calls=calls,reference_hashes=refhash,actual_effect_rows=35,native_identity_bits=64,legacy_minus_one_zero_sentinel_preserved=True)
        elif a.case=='guard_lifecycle':
            raw_start();before=queue();buf=C.create_string_buffer(POSES[0])
            check(native(None,7)==1 and native(buf,6)==1 and queue()==before,'owned input errors cannot enqueue or quarantine sound')
            kernel=C.WinDLL('kernel32',use_last_error=True);alloc=bind(kernel,'VirtualAlloc',P,P,S,U,U);release=bind(kernel,'VirtualFree',I,P,S,U)
            region=alloc(None,8192,0x2000,1);check(region and alloc(region,4096,0x1000,4)==region,'real no-access guard page for exact seven-byte input')
            try:
                pointer=region+4096-7;C.memmove(pointer,POSES[0],7);check(native(pointer,7)==0 and C.string_at(pointer,7)==POSES[0],'actual producer cannot read an eighth byte into a protected page')
            finally:check(release(region,0,0x8000)!=0,'release independent guarded input')
            foreign=[];thread=threading.Thread(target=lambda:foreign.append(native(buf,7)));thread.start();thread.join(5)
            check(not thread.is_alive() and foreign==[3] and si().phase==3,'foreign caller cannot mutate live QSound producer')
            check(close(5000)==0 and native(buf,7)==2 and sdk(None)==0x0d000001,'closed native/game SDK paths cannot reach retired work')
        elif a.case.startswith('driver_'):
            raw_start();work=peer('peer_observe',20);status,host=work[14],work[16];before=memory(0,2<<20);output=bytearray()
            if a.case in ('driver_cpu','driver_dma'):check(bind(lib,'sdMemBlkSetTransferMode',I,I)(0 if a.case=='driver_cpu' else 1)==0,'actual selected original SDK copy mode')
            for positions in POSES:
                old=memory(host,32);wire=bytes([0x88,0])+positions+bytes(7)+bytes([0])+old[17:32]
                check(memory(status,1)==b'\0','actual ARM ready before independent QSound command')
                if a.case=='driver_reference':
                    data=C.create_string_buffer(wire);one=C.create_string_buffer(b'\1')
                    check(bind(core,'jp_aica_write',I,U,P,U)(host,data,32)==0 and bind(core,'jp_aica_write',I,U,P,U)(status,one,1)==0,'independent raw host publishes actual command and waiting doorbell, not an ACK')
                elif a.case!='driver_noop':
                    data=C.create_string_buffer(positions);check(native(data,7)==0 and bind(lib,'jp_sound_service',I)()==0,'original game/SDK producer and actual transfer send position list')
                    check(memory(host,32)==wire and memory(status,1)==b'\1','all real primary command and retained padding bytes equal independent raw protocol')
                output+=render(4410);check(memory(status,1)==b'\0','actual original ARM consumes QSound list without synthetic ready acknowledgement')
                if a.case in ('driver_cpu','driver_dma'):check(bind(lib,'jp_sound_service',I)()==0,'real SDK finishes accepted command bookkeeping')
            final=memory(0,2<<20);(d/'private_final_RAM.bin').write_bytes(final);(d/'private_output.s16le').write_bytes(output)
            if a.case in ('driver_cpu','driver_dma'):
                check(final==(a.reference/'private_final_RAM.bin').read_bytes() and bytes(output)==(a.reference/'private_output.s16le').read_bytes(),'complete2MiB driver state and PCM equal independent raw-command execution in both copy modes')
            observations.update(final_RAM_sha256=hashlib.sha256(final).hexdigest(),PCM_sha256=hashlib.sha256(output).hexdigest(),changed_RAM_bytes=sum(x!=y for x,y in zip(before,final)))
        else:
            clock=ClockOptions(56,441,8192,5,None,None,0,None,None)
            check(bind(system,'jp_system_start_sound_clock',I,P,P)(C.byref(opts(1)),C.byref(clock))==0,'actual timed original game sound under System')
            window=bind(device,'jp_device_window',P)();buf=C.create_string_buffer(POSES[0]);check(native(buf,7)==0,'actual position request accepted during real-time sound')
            for turn in range(100):
                check(bind(present,'jp_wait_window_idle',I,P,U)(window,5)==1,'native real-clock wait services actual QSound request')
                q=queue();work=peer('peer_observe',20)
                if not any(q[:3]) and memory(work[14],1)==b'\0':break
            else:raise AssertionError('timed real driver did not consume position command')
            check(si().phase==3,'timed QSound forwarding preserves native sound health')
        if si().resources and a.case!='timed':check(close(5000)==0,'actual sound owner retires before borrowed core')
        check(sysclose()==0,'compiled System closes only after actual QSound/sound ownership');system_live=False
        result=dict(case=a.case,checks=len(labels),assertions=labels,observations=observations,original_complete_calls=sum(calls.values()))
        (d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
    finally:
        if system_live:close(5000);sysclose()
        if prereqs:
            bind(frames,'jp_frames_shutdown',I)();bind(frames,'jp_render_state_shutdown',None)();bind(frames,'jp_textures_shutdown',None)();bind(frames,'jp_queue_shutdown',None)()
if __name__=='__main__':main()
