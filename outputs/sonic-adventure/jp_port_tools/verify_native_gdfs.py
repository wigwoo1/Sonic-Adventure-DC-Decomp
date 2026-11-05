"""Qualify original native GDFS over real user-owned media, not a read stub."""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys,threading,time
from pathlib import Path
from native_packet_test_support import bind
from native_clock_test_support import Clock
from model_reference import make_reference
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p
Callback=C.CFUNCTYPE(None,P);ErrorCallback=C.CFUNCTYPE(None,P,I);Idle=C.CFUNCTYPE(I,P,U)
class Info(C.Structure):
    _fields_=[(n,I if n=='source_error' else U) for n in ('phase','status','source_error','live_handles','max_handles','max_entries','dir_entries','vblank','handle_bytes','work_bytes','dir_bytes','pending')]+[(n,Q) for n in ('work','dir','read_bytes','requests','callbacks','vblanks')]
def sha(p):
    h=hashlib.sha256()
    with Path(p).open('rb') as f:
        for block in iter(lambda:f.read(8<<20),b''):h.update(block)
    return h.hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(a.build/n)==h,n
    for n,h in b['original_source_sha256'].items():assert sha(u/n)==h,n
    for key in ('DLL','frames_DLL','GPU_DLL','present_DLL','media_DLL'):assert sha(b[key])==b[key+'_sha256']
    C.CDLL(b['frames_DLL']);C.CDLL(b['GPU_DLL']);clock=Clock(b['present_DLL']);chain=clock.lib;lib=C.CDLL(b['DLL']);media=C.CDLL(b['media_DLL'])
    configure=bind(lib,'jp_gdfs_configure',I,C.c_wchar_p,U,U,U,U);init=bind(lib,'jp_gdfs_initialize',I,U,U);close=bind(lib,'jp_gdfs_shutdown',I);snapshot=bind(lib,'jp_gdfs_snapshot',I,P)
    wi=bind(chain,'jp_wait_initialize',I,U,U,U,P,P);wc=bind(chain,'jp_wait_shutdown',I);pump=bind(chain,'jp_wait_pump',I,P);count=bind(chain,'jp_vblank_chain_count',U)
    add=bind(chain,'syChainAddHandler',U,C.c_int16,P,U,P);delete=bind(chain,'syChainDeleteHandler',None,U)
    open_=bind(lib,'gdFsOpen',P,C.c_char_p,P);fast=bind(lib,'gdFsOpenFast',P,C.c_char_p,P);range_=bind(lib,'gdFsOpenRange',P,I,I);hc=bind(lib,'gdFsClose',None,P);change=bind(lib,'gdFsChangeDir',I,C.c_char_p)
    read=bind(lib,'gdFsRead',I,P,I,P);req=bind(lib,'gdFsReqRd32',I,P,I,P);stream=bind(lib,'gdFsReqGdRd',I,P,I);trans=bind(lib,'gdFsTrans32',I,P,I,P);server=bind(lib,'gdFsExecServer',None)
    stat=bind(lib,'gdFsGetStat',I,P);trstat=bind(lib,'gdFsGetTransStat',I,P);nr=bind(lib,'gdFsGetNumRd',I,P);seek=bind(lib,'gdFsSeek',I,P,I,I);tell=bind(lib,'gdFsTell',I,P);eof=bind(lib,'gdFsCheckEof',I,P,P)
    mode=bind(lib,'gdFsSetTmode',I,P,I);stop=bind(lib,'gdFsStopRd',I,P);pickup=bind(lib,'gdFsMovePickup',I,P);reinit=bind(lib,'gdFsReinit',I)
    cbread=bind(lib,'gdFsEntryRdEndFunc',None,P,P,P);cbtrans=bind(lib,'gdFsEntryTrEndFunc',None,P,P,P)
    geterr=bind(lib,'gdFsGetGlobalErr',I);queries={n:bind(lib,n,I,P,P) for n in ('gdFsGetFileSize','gdFsGetFileSctSize','gdFsGetFileFad')}
    create_dir=bind(lib,'gdFsCreateDirhn',P,P,I);load_dir=bind(lib,'gdFsLoadDir',I,C.c_char_p,P);set_dir=bind(lib,'gdFsSetDir',I,P)
    checks=0;groups={};calls={};log=d/'progress.txt';owner=threading.get_ident();callback_errors=[];events=[];cookie=Q(0xfeed1234aabbccdd)
    def check(ok,label):
        nonlocal checks
        checks+=1
        if not ok:raise AssertionError((checks,label))
    def group(label):
        groups[label]=checks-sum(groups.values())
        with log.open('a') as f:f.write(f'{label}: {groups[label]} checks\n')
    def info():
        out=Info();check(snapshot(C.byref(out))==0,'actual native GDFS snapshot');return out
    def value(name,h):
        out=I(-77);check(queries[name](h,C.byref(out))==1,'original file metadata query');return out.value
    @Idle
    def idle(context,ms):
        if ms:time.sleep(ms/1000)
        return 1
    @Callback
    def noop(arg):pass
    def start_wait():check(clock.init(None,None,0)==0 and wi(60,1,8,C.cast(idle,P),None)==0,'real QPC/shared wait initialized')
    def configured(path=a.track,chunk=3,raw=None):check(configure(str(path.resolve()),45150,raw or (2352 if path==a.track else 2048),chunk,5000)==0,'copied media path and bounded real chunks configured')
    def aligned(n,offset=0):
        raw=(C.c_ubyte*(n+96))();C.memset(raw,0xa5,len(raw));at=(C.addressof(raw)+63)&~31;return raw,at+offset
    def data(at,n):return C.string_at(at,n)
    def guards(raw,at,n):
        before=at-C.addressof(raw);check(bytes(raw[:before])==b'\xa5'*before and bytes(raw[before+n:])==b'\xa5'*(len(raw)-before-n),'caller buffer guards untouched')
    def finish_request(h,use_wait=False):
        deadline=time.monotonic()+8
        while stat(h) in (2,3,4):
            if use_wait:
                done=U();check(pump(C.byref(done))==0,'actual shared vblank service pumps original GDFS handler');time.sleep(.002)
            else:server()
            if time.monotonic()>deadline:raise AssertionError('bounded real request did not finish')
        check(stat(h)==1,'actual original request reaches complete')
    check(C.sizeof(Info)==96 and close()==0 and init(8,1920)==2,'native ABI and cold ownership')
    check(configure(None,45150,2352,3,5000)==1 and configure(str(a.track),45150,2352,0,5000)==1,'invalid configuration rejected')
    configured();check(init(8,1920)==7 and info().phase==4,'missing real chain rejects startup')
    check(close()==0 and not info().work and not info().dir,'failed pre-chain startup releases all private arenas/media')
    start_wait()
    mo=bind(media,'jp_gd_media_open',I,C.c_wchar_p,U,U,U);mc=bind(media,'jp_gd_media_close',I,U);ms=bind(media,'jp_gd_media_status',I)
    check(mo(str(a.track.resolve()),45150,2352,8)==0 and init(8,1920)==6,'preexisting real media owner is not stolen')
    check(close()==0 and ms()==0 and mc(5000)==0,'failed GDFS startup leaves another media owner intact')
    fixture=d/'bad-volume.bin';fixture.write_bytes(bytes(40*2048));configured(fixture)
    check(init(8,1920)==6 and close()==0 and ms()==2,'bad actual volume fails preflight and closes owned media')
    configured();check(init(0,1920)==1 and init(8,8193)==1,'invalid pool capacities preserve configured phase')
    check(init(8,1920)==0 and count()==1,'original startup acquires exactly one real priority80 handler')
    state=info();check(state.phase==3 and state.dir_entries==248 and state.handle_bytes==120 and state.work>0xffffffff and state.dir>0xffffffff,'root loaded with full-width native handle/work pointers')
    sentinel=Info();C.memset(C.byref(sentinel),0xa5,C.sizeof(sentinel));before=bytes(sentinel);foreign=[]
    def foreign_thread():foreign.extend([snapshot(C.byref(sentinel)),close(),configure(str(a.track),45150,2352,3,5000)])
    thread=threading.Thread(target=foreign_thread);thread.start();thread.join();check(foreign==[3,3,3] and bytes(sentinel)==before,'foreign thread cannot inspect/mutate owner or overwrite output')
    check(snapshot(P(state.work))==1 and not range_(45149,1) and not range_(45150,0) and not range_(0x7fffffff,0x7fffffff),'protected owner arenas and invalid media ranges reject')
    group('native_owner_real_startup_failures_and_pointer_width')
    track_hash=sha(a.track);check(track_hash=='4ac0e256cf91520f794d7f831c193847b16397da3bde5c890e04e5330499c76c','user-owned input hash matches qualified Japanese disc')
    with a.track.open('rb') as f:
        def independent(fad,n):
            out=bytearray()
            for j in range(n):f.seek((fad-45150+j)*2352+16);out+=f.read(2048)
            return bytes(out)
        check(change(b'SONICADV')==0 and info().dir_entries==1863,'original relative path loads complete actual game directory')
        old=info();old_dir=data(old.dir,old.dir_bytes)
        check(change(b'not-a-directory')==-8 and data(old.dir,old.dir_bytes)==old_dir,'failed directory load preserves existing current table')
        small=(C.c_ubyte*(16+44*2+64))();C.memset(small,0xa5,len(small));sd=create_dir(small,2);initial=bytes(small)
        check(sd==C.addressof(small) and load_dir(b'/SONICADV',sd)==-12 and bytes(small)==initial,'insufficient external directory is not partially committed')
        cap=1920;external=(C.c_ubyte*(16+44*cap+64))();C.memset(external,0xa5,len(external));ed=create_dir(external,cap)
        check(load_dir(b'/',ed)==0 and struct.unpack_from('<i',bytes(external))[0]==248 and bytes(external[-64:])==b'\xa5'*64,'external real directory and trailing guards')
        check(set_dir(ed)==0 and info().dir_entries==248 and change(b'/SONICADV')==0,'original directory copy and absolute path table traversal')
        held=[open_(b'ADV00.PRS',None) for _ in range(8)];check(all(held) and len(set(held))==8 and not open_(b'ADV00.PRS',None) and geterr()==-9,'original finite handle pool fails with NOHNDL')
        check(info().live_handles==8,'owned handle population excludes system handle')
        for h in held:hc(h)
        check(info().live_handles==0 and stat(held[0])==-10,'closed handle is invalid and pool retained')
        check(not open_(b'x'*32,None) and not open_(b'../ADV00.PRS',None) and not open_(b'absent.prs',None),'invalid and nonexistent names do not open')
        group('original_directory_handle_pool_and_staged_failure_behavior')
        files=[]
        for name,fad,size,expected in [(b'ADV00.PRS',539434,1057178,'9bb063c8a77e9cfac242183ce1169c2283cecaf0359e498b0309e5487fe1953a'),(b'SET0001S.BIN',543608,64,'16abab341fb7f370e27e4dadcf81766dd0dfd0ae64469477bb2cf6614938b2af')]:
            h=fast(name.lower(),None);check(bool(h),'original case-insensitive fast open')
            nsct=(size+2047)//2048;check(value('gdFsGetFileSize',h)==size and value('gdFsGetFileSctSize',h)==nsct and value('gdFsGetFileFad',h)==fad,'actual original file metadata')
            for transfer_mode,unaligned in ((0,0),(1,0),(1,1)):
                check(mode(h,transfer_mode)==0 and seek(h,0,0)==0,'CPU/DMA mode and original seek')
                raw,at=aligned(nsct*2048,unaligned);check(read(h,nsct+10,at)==0,'real blocking read clamps at EOF')
                got=data(at,nsct*2048);check(got==independent(fad,nsct) and hashlib.sha256(got[:size]).hexdigest()==expected,'all game-file bytes and final-sector padding match independent input')
                guards(raw,at,len(got));done=I(-1);check(tell(h)==nsct and eof(h,C.byref(done))==0 and done.value==1 and read(h,1,at)==0,'original final offset/EOF/no further reads')
            files.append(dict(name=name.decode(),fad=fad,bytes=size,sha256=expected));hc(h)
        h=range_(545837,3313);check(bool(h),'actual program range opens');raw,at=aligned(3313*2048)
        check(read(h,3313,at)==0 and hashlib.sha256(data(at,6783232)).hexdigest()==sha(a.binary),'native GDFS reads exact preserved Japanese program bytes');guards(raw,at,3313*2048);hc(h)
        group('real_CPU_DMA_unaligned_full_files_and_exact_program_hash')
        # Pure handle routines run as whole original SH-4 functions, without hooks.
        sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
        from verify_fpu_cleanup_transfer import Fpu
        reference,refhash=make_reference(executor);symbols={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(u/'config/1st_read/symbols.txt').read_text(),re.M)}
        ram=bytearray(16<<20);image=a.binary.read_bytes();ram[0x10000:0x10000+len(image)]=image;guest=0x8ce40000;outp=0x8ce50000
        def original(name,*args):
            cpu=executor.State();cpu.pc=symbols[name];cpu.pr=0x8cfed000;cpu.r[15]=0x8cf00000;cpu.sr=0x70000000
            for j,arg in enumerate(args):cpu.r[4+j]=arg&0xffffffff
            fpu=Fpu();fpu.fpscr=0x40001;reference(ram,cpu,fpu=fpu,instruction_budget=100000);calls[name]=calls.get(name,0)+1
            check(cpu.r[15]==0x8cf00000,'whole original handle call preserves stack');return C.c_int32(cpu.r[0]).value
        h=open_(b'ADV00.PRS',None);nsct=value('gdFsGetFileSctSize',h)
        check(original('gdFsCreateHnG',guest,1,539434,1057178)==C.c_int32(guest).value,'unhooked original handle construction')
        rng=random.Random(0x47444653)
        for sector,origin in [(0,0),(1,1),(-1,2),(1,1),(nsct+1,0),(-1,0),(0,7)]+[(rng.randrange(-nsct,nsct*2),rng.randrange(3)) for _ in range(100)]:
            check(seek(h,sector,origin)==original('gdFsSeek',guest,sector,origin),'native seek returns same error/acceptance as original instructions')
            check(tell(h)==original('gdFsTell',guest),'native sector position matches original')
            iseof=I(-1);check(eof(h,C.byref(iseof))==original('gdFsCheckEof',guest,outp) and iseof.value==struct.unpack_from('<i',ram,outp&0xffffff)[0],'native EOF value matches original')
        for name in queries:
            check(queries[name](h,C.byref(iseof))==original(name,guest,outp) and iseof.value==struct.unpack_from('<i',ram,outp&0xffffff)[0],'native file metadata matches complete original routine')
        check(seek(h,0,0)==0 and mode(h,1)==0,'fresh async test offset/mode')
        group('complete_unhooked_original_handle_instruction_oracles')
        raw,at=aligned(16*2048);expected=independent(539434,16)
        @Callback
        def observed(arg):
            try:events.append((int(C.cast(arg,P).value),threading.get_ident(),data(at,16*2048)==expected,stat(h),tell(h),close(),reinit()))
            except BaseException as e:callback_errors.append(repr(e))
        cbread(h,observed,C.byref(cookie));cbtrans(h,observed,C.byref(cookie))
        check(req(h,16,at)==16 and req(h,1,at)==0,'async real read starts once and busy request is refused')
        check(change(b'/')==-13 and seek(h,0,0)==-13,'busy directory/active seek cannot invalidate destination')
        finish_request(h,True);server()
        check(len(events)==2 and not callback_errors and all(e==(C.addressof(cookie),owner,True,1,16,9,-13) for e in events),'actual DMA copy/read callbacks preserve full arguments, bytes, owner and lifecycle boundaries')
        guards(raw,at,16*2048);check(nr(h)==16*2048 and pickup(h)==0,'actual async byte count and real seek read/discard')
        cbread(h,None,None);cbtrans(h,None,None);hc(h)
        # A read callback can queue another nonblocking read on the same handle.
        h=open_(b'ADV00.PRS',None);r1,p1=aligned(2048);r2,p2=aligned(2048);queued=[]
        @Callback
        def again(arg):
            try:
                queued.append((tell(h),stat(h)))
                server()
                if len(queued)==1:queued.append(('request',req(h,1,p2)))
            except BaseException as e:callback_errors.append(repr(e))
        cbread(h,again,None);check(req(h,1,p1)==1,'first callback-driven real read accepted');finish_request(h);server()
        check(queued==[(1,1),('request',1),(2,1)] and data(p1,2048)==independent(539434,1) and data(p2,2048)==independent(539435,1) and not callback_errors,'callback requeue completes distinct actual sectors without recursive server execution');hc(h)
        group('actual_async_vblank_completion_callbacks_and_callback_requeue')
        h=open_(b'ADV00.PRS',None);events.clear();pieces=[];offset=0
        @Callback
        def transferred(arg):
            try:events.append((C.cast(arg,P).value,threading.get_ident(),close(),reinit()))
            except BaseException as e:callback_errors.append(repr(e))
        cbtrans(h,transferred,C.byref(cookie));check(stream(h,12)==12,'real streamed request accepted')
        for length in (32,4096,8192,12256):
            raw,piece=aligned(length);check(trans(h,length,piece)==0,'actual streamed transfer submitted')
            deadline=time.monotonic()+8
            while trstat(h)==1:
                server()
                if time.monotonic()>deadline:raise AssertionError('stream transfer deadline')
            check(data(piece,length)==independent(539434,12)[offset:offset+length],'stream bytes remain continuous across actual I/O chunks')
            guards(raw,piece,length);pieces.append(data(piece,length));offset+=length
        server();check(offset==12*2048 and tell(h)==12 and stat(h)==1 and len(events)==4 and all(e==(C.addressof(cookie),owner,9,-13) for e in events) and not callback_errors,'each actual stream transfer has one deferred callback and full read completes')
        check(b''.join(pieces)==independent(539434,12),'complete multi-chunk stream equals independent disc bytes')
        check(seek(h,0,0)==0 and stream(h,12)==12,'new real stream before cancellation');raw,piece=aligned(12*2048)
        check(trans(h,31,piece)==-15 and trans(h,32,piece+1)==-14 and trans(h,13*2048,piece)==-24,'original stream size/alignment/remaining guards')
        check(trans(h,12*2048,piece)==0,'real transfer queued before cancel');before_callbacks=len(events)
        check(stop(h)==0,'actual stream cancellation drains native request');frozen=data(piece,12*2048)
        for _ in range(8):server()
        check(data(piece,12*2048)==frozen and len(events)==before_callbacks,'cancelled destination stays detached and receives no late callback');hc(h)
        group('actual_stream_transfers_fragmentation_alignment_and_cancel')
        h=open_(b'SET0001S.BIN',None);raw,at=aligned(2048)
        check(read(h,0x7fffffff,at)==0 and data(at,2048)==independent(543608,1),'largest signed blocking sector count clamps before original addition can overflow')
        check(seek(h,0,0)==0 and req(h,0x7fffffff,at)==1,'largest signed async sector count clamps at file end');finish_request(h)
        check(seek(h,0,0)==0 and stream(h,0x7fffffff)==1 and trans(h,2048,at)==0,'largest signed streaming sector count clamps at file end');finish_request(h)
        check(data(at,2048)==independent(543608,1),'all overflow-boundary reads preserve actual one-sector file data');guards(raw,at,2048);hc(h)
        h=open_(b'ADV00.PRS',None);dma_events=[]
        @Callback
        def cpu_read(arg):dma_events.append('read')
        @Callback
        def dma_copy(arg):dma_events.append('transfer')
        cbread(h,cpu_read,None);cbtrans(h,dma_copy,None);check(mode(h,0)==0 and read(h,1,at)==0 and dma_events==['read'],'CPU read does not invent DMA-transfer interrupt callback')
        hc(h);h=open_(b'ADV00.PRS',None);recycled=[]
        @Callback
        def reuse(arg):
            try:
                hc(h);new=open_(b'SET0001S.BIN',None);recycled.append(new);cbtrans(new,dma_copy,None)
            except BaseException as e:callback_errors.append(repr(e))
        cbread(h,reuse,None);cbtrans(h,dma_copy,None);dma_events.clear()
        check(req(h,1,at)==1,'DMA read before read-callback closes/reuses handle');server()
        deadline=time.monotonic()+8
        while not recycled:
            server()
            if time.monotonic()>deadline:raise AssertionError('callback-recycle deadline')
        check(recycled==[h] and not dma_events and not callback_errors,'pending completion cannot call a newly recycled handle callback');hc(h)
        group('overflow_boundary_CPU_callback_and_recycled_handle_regressions')
        h=open_(b'ADV00.PRS',None);raw,at=aligned(32*2048);check(req(h,32,at)==32 and reinit()==0,'original reinit drains real live I/O before resetting handle pool')
        frozen=data(at,32*2048);check(info().live_handles==0 and info().dir_entries==248 and stat(h)==-10 and count()==1,'reinit resets original state while retaining one shared handler')
        for _ in range(8):server()
        check(data(at,32*2048)==frozen,'reinit never writes a detached old caller buffer')
        scratch=(C.c_ubyte*128)();C.memset(scratch,0xa5,128)
        check(bind(lib,'gdFsGetToc',I,I,P)(0,scratch)==-26 and bytes(scratch)==b'\xa5'*128 and bind(lib,'gdFsDaPlay',I,I,I,I)(3,4,0)==-26,'unsupported console audio/TOC refuse without success or output writes')
        check(close()==0 and count()==0 and not info().work and not info().dir and ms()==2,'shutdown releases original handler, buffers and media')
        handles=[add(0x900,noop,j,None) for j in range(255)];check(all(handles) and init(8,1920)==7,'full genuine chain pool propagates startup failure')
        check(close()==0 and count()==255,'failed full-chain startup preserves every other registered handler')
        for item in handles:delete(item)
        check(init(8,1920)==0 and close()==0 and count()==0,'same owner can successfully restart after real startup failure')
        # Small private ISO image: real raw read fails on its file's mode2 header.
        def record(name,sector,size,flags):
            n=33+len(name);out=bytearray(n+(n&1));out[0]=len(out)
            struct.pack_into('<I',out,2,sector);struct.pack_into('>I',out,6,sector)
            struct.pack_into('<I',out,10,size);struct.pack_into('>I',out,14,size)
            out[25]=flags;struct.pack_into('<H',out,28,1);struct.pack_into('>H',out,30,1);out[32]=len(name);out[33:33+len(name)]=name;return out
        pvd=bytearray(independent(45166,1));struct.pack_into('<I',pvd,132,10);struct.pack_into('>I',pvd,136,10);struct.pack_into('<I',pvd,140,45019)
        root_record=record(b'\0',45020,2048,2);pvd[156:190]=root_record
        payload=[bytearray(2048) for _ in range(22)];payload[16]=pvd;payload[19][:10]=b'\x01\0'+struct.pack('<IH',45020,1)+b'\0\0'
        directory=root_record+record(b'\1',45020,2048,2)+record(b'BAD.BIN;1',45021,2048,0);payload[20][:len(directory)]=directory;payload[21][:]=b'\x77'*2048
        bad_file=d/'bad-file-raw.bin';header=bytes.fromhex('00ffffffffffffffffffff00c2037401');bad_image=bytearray()
        for j,sector in enumerate(payload):
            raw_sector=bytearray(2352);raw_sector[:16]=header;raw_sector[16:2064]=sector
            if j==21:raw_sector[15]=2
            bad_image+=raw_sector
        bad_file.write_bytes(bad_image);configured(bad_file,raw=2352);check(init(8,16)==0,'actual raw error-fixture volume boots through original GDFS')
        h=open_(b'BAD.BIN',None);check(bool(h),'original open finds bad-sector fixture file');errors=[]
        @ErrorCallback
        def observed_error(arg,error):
            try:errors.append((C.cast(arg,P).value,error,threading.get_ident(),close()))
            except BaseException as e:callback_errors.append(repr(e))
        bind(lib,'gdFsEntryErrFunc',None,P,P,P)(h,observed_error,C.byref(cookie));bind(lib,'gdFsEntryErrFuncAll',None,P,P)(observed_error,C.byref(cookie))
        raw,at=aligned(2048);check(req(h,1,at)==1,'actual I/O error request accepted');deadline=time.monotonic()+8
        while stat(h) in (2,3,4):
            server()
            if time.monotonic()>deadline:raise AssertionError('actual read-error deadline')
        check(stat(h)==5 and bind(lib,'gdFsGetErrStat',I,P)(h)==-19 and errors==[(C.addressof(cookie),-19,owner,9)]*2 and not callback_errors,'real raw-header error reaches original global and per-handle error callbacks')
        native_stats=info();check(native_stats.callbacks==12,'all actual user read/transfer/error callback deliveries counted once')
        check(data(at,2048)==b'\xa5'*2048 and close()==0 and count()==0,'failed actual read exposes no partial payload and cleans up original owner')
        check(wc()==0,'filesystem retires before shared wait');clock.shutdown()
        check(sha(a.track)==track_hash and sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf','retail/reference inputs preserved exactly')
        group('real_pending_reinit_shutdown_chain_failure_and_input_preservation')
    result=dict(status='original_native_GDFS_real_JP_startup_handles_reads_stream_callbacks_cancel_and_shutdown_verified',checks=checks,groups=groups,
        build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),reference_hashes=refhash,original_complete_machine_calls=sum(calls.values()),original_calls=calls,original_function_hooks_used=False,
        actual_files=files,original_program_sha256=sha(a.binary),data_track_sha256=track_hash,data_track_bytes=a.track.stat().st_size,native_handle_bytes=state.handle_bytes,
        actual_native_GDFS_callbacks=native_stats.callbacks,actual_stream_transfer_callbacks=len(events),native_real_read_bytes=native_stats.read_bytes,native_requests=native_stats.requests,native_vblanks=native_stats.vblanks,
        original_inputs_modified=False,source_proof_files_preserved=True,real_media_IO_and_original_GDFS_used=True,
        simulated_GD_DCF_read_provider_used=False,console_BIOS_DMA_or_overlay_success_stubs_used=False,CDDA_TOC_subcode_supported=False,
        slow_IO_timeout_branch_observed=False,full_system_bootstrap_audio_or_gameplay_verified=False,combined_DLL_publishable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',sum(calls.values()),'original calls',flush=True)
if __name__=='__main__':main()
