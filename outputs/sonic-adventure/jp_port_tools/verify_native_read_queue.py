"""Exercise the original queue using actual JP disc bytes and real GDFS I/O."""
import argparse,ctypes as C,faulthandler,hashlib,json,struct,subprocess,sys,threading,time
from pathlib import Path
from verify_native_audio import bind
from native_clock_test_support import Clock
from verify_native_gdfs import Info as FSInfo
I,U,Q,P,S=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p,C.c_size_t
CASES=('normal','full','mixed','cancel','protection','callback','foreign','identity','bad_sector','restart','zero_length')
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('starts','adds','completed','cancels','lease','active_handle')]+[(n,I if n=='source_error' else U) for n in ('phase','status','source_error','queued','current','creator_thread','initialized','reserved')]
class Slot(C.Structure):
    _fields_=[('name',C.c_char*32)]+[(n,Q) for n in ('destination','capacity','file_bytes')]+[('state',I)]+[(n,U) for n in ('index','fad','reserved')]
class Disc:
    def __init__(self,path):self.f=Path(path).open('rb');self.reads=set()
    def sectors(self,fad,n):
        out=bytearray()
        for j in range(n):self.reads.add(fad+j);self.f.seek((fad+j-45150)*2352+16);raw=self.f.read(2048);assert len(raw)==2048;out+=raw
        return bytes(out)
    def directory(self,fad,bytes_):
        raw=self.sectors(fad,(bytes_+2047)//2048);at=0;out={}
        while at<bytes_:
            n=raw[at]
            if not n:at=((at//2048)+1)*2048;continue
            assert n>=34 and at+n<=len(raw);r=raw[at:at+n];name=r[33:33+r[32]].split(b';')[0]
            if name not in (b'\0',b'\1'):out[name.decode().upper()]=(struct.unpack_from('<I',r,2)[0]+150,struct.unpack_from('<I',r,10)[0],bool(r[25]&2))
            at+=n
        return out
    def files(self):
        pvd=self.sectors(45166,1);path_fad=struct.unpack_from('<I',pvd,140)[0]+150;path_size=struct.unpack_from('<I',pvd,132)[0]
        self.sectors(path_fad,(path_size+2047)//2048)
        root=self.directory(struct.unpack_from('<I',pvd,158)[0]+150,struct.unpack_from('<I',pvd,166)[0]);fad,n,_=root['SONICADV']
        return self.directory(fad,n)

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    if not a.case:
        cases={}
        for name in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for key in ('workspace','build','track'):cmd+=['--'+key,str(getattr(a,key).resolve())]
            cmd+=['--case',name,'--output',str(d/name)];run=subprocess.run(cmd,capture_output=True,timeout=120,creationflags=subprocess.CREATE_NO_WINDOW)
            (d/(name+'.log')).write_bytes(run.stdout+run.stderr);assert not run.returncode,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-5000:])
            cases[name]=read(d/name/'result.json');print(name,cases[name]['checks'],'checks',flush=True)
        out=dict(status='original_read_queue_real_JP_bytes_native_capacity_cancellation_and_GDFS_authority_verified',checks=sum(x['checks'] for x in cases.values()),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
            failed_OS_cancel_timeout_or_global_System_Exit_queue_handoff_verified=False,full_game_init_executed=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['checks'],'checks',flush=True);return
    fault=(d/'native_fault.txt').open('w');faulthandler.enable(file=fault,all_threads=True);labels=[];obs={};keep=[]
    def check(ok,label):
        assert ok,(a.case,label);labels.append(label);(d/'progress.json').write_text(json.dumps(dict(case=a.case,checks=len(labels),assertions=labels),indent=2)+'\n')
    disc=Disc(a.track);files=disc.files();eligible=sorted((n,v) for n,v in files.items() if not v[2] and 64*1024<=v[1]<=512*1024 and len(n)<=31)
    check(len(eligible)>=3,'independent actual ISO directory supplies real multi-chunk files');name,(fad,bytes_,_)=next(((n,v) for n,v in eligible if n=='MASARU.DRV'),eligible[0]);rounded=(bytes_+2047)&~2047
    expected=disc.sectors(fad,rounded//2048);track=a.track.resolve()
    if a.case=='bad_sector':
        # A sparse private raw-track fixture retains actual ISO metadata and
        # selected file bytes, with one genuinely malformed sector header.
        import msvcrt
        track=d/'private_bad_sector_track.bin'
        with track.open('w+b') as f:
            kernel=C.WinDLL('kernel32',use_last_error=True);returned=U()
            check(bind(kernel,'DeviceIoControl',I,P,U,P,U,P,U,P,P)(msvcrt.get_osfhandle(f.fileno()),0x900c4,None,0,None,0,C.byref(returned),None)!=0,'real sparse-file fixture enabled')
            f.truncate(Path(a.track).stat().st_size)
            for sector in sorted(disc.reads|set(range(45150,45214))):
                disc.f.seek((sector-45150)*2352);raw=disc.f.read(2352);assert len(raw)==2352
                if sector==fad+3:raw=b'\x01'+raw[1:]
                f.seek((sector-45150)*2352);f.write(raw)
    clock=Clock(b['present_DLL']);fs=C.CDLL(b['GDFS_DLL']);queue=C.CDLL(b['read_queue_DLL'])
    init=bind(queue,'jp_read_queue_initialize',I);add=bind(queue,'jp_read_queue_add',I,C.c_char_p,P,S);serve=bind(queue,'jp_read_queue_serve',I);cancel=bind(queue,'jp_read_queue_cancel',I);shutdown=bind(queue,'jp_read_queue_shutdown',I)
    def info():
        v=Info();check(bind(queue,'jp_read_queue_snapshot',I,P)(C.byref(v))==0,'actual native queue snapshot');return v
    def slot(n):
        v=Slot();check(bind(queue,'jp_read_queue_slot_snapshot',I,U,P)(n,C.byref(v))==0,'actual original queue slot snapshot');return v
    def fi():
        v=FSInfo();check(bind(fs,'jp_gdfs_snapshot',I,P)(C.byref(v))==0,'actual GDFS ownership snapshot');return v
    def buffer():
        raw=(C.c_ubyte*(rounded+96))();C.memset(raw,0xa5,C.sizeof(raw));at=(C.addressof(raw)+63)&~31;keep.append(raw);return raw,at
    def guards(raw,at):
        before=at-C.addressof(raw);check(bytes(raw[:before])==b'\xa5'*before and bytes(raw[before+rounded:])==b'\xa5'*(len(raw)-before-rounded),'actual destination canaries preserved')
    def drain():
        end=time.monotonic()+30
        while True:
            current=Info();assert bind(queue,'jp_read_queue_snapshot',I,P)(C.byref(current))==0
            if not current.queued:break
            assert serve()==0 and time.monotonic()<end
        check(True,'bounded actual source serve drains every live request')
    check(C.sizeof(Info)==80 and C.sizeof(Slot)==72 and shutdown()==0 and init()==5,'native ABI and absent real GDFS rejected')
    Idle=C.CFUNCTYPE(I,P,U)
    @Idle
    def idle(ctx,ms):
        if ms:time.sleep(ms/1000)
        return 1
    keep.append(idle)
    check(clock.init(None,None,0)==0 and bind(clock.lib,'jp_wait_initialize',I,U,U,U,P,P)(60,1,8,C.cast(idle,P),None)==0,'actual monotonic wait/vblank owner initialized')
    check(bind(fs,'jp_gdfs_configure',I,C.c_wchar_p,U,U,U,U)(str(track),45150,2352,1,5000)==0 and bind(fs,'jp_gdfs_initialize',I,U,U)(24,1920)==0 and bind(fs,'gdFsChangeDir',I,C.c_char_p)(b'SONICADV')==0,'original GDFS and actual full SONICADV directory initialized')
    check(init()==0 and info().lease and info().initialized,'original queue initialization claims actual GDFS lifetime')
    check(bind(fs,'jp_gdfs_shutdown',I)()==11 and bind(fs,'gdFsReinit',I)()==-13 and fi().phase==3,'independent GDFS destruction/reinitialization rejected while queue owns client lease')
    terminal=Q();check(bind(fs,'jp_gdfs_exit_claim',I,P)(C.byref(terminal))==0 and bind(fs,'jp_gdfs_exit_finish',I,Q)(terminal.value)==11 and bind(fs,'jp_gdfs_exit_cancel',I,Q)(terminal.value)==0,'final GDFS close rejects before irreversible terminal mutation while client lives')
    raw,at=buffer();before=bytes(raw)
    if a.case=='normal':
        authority=info().lease;other=Q(0xabcdef)
        check(init()==2 and bind(fs,'jp_gdfs_read_queue_claim',I,P)(C.byref(other))==2 and other.value==0xabcdef,'second queue/client initialization cannot replace actual authority or output')
        check(bind(fs,'jp_gdfs_read_queue_enter',I,Q)(authority+1)==2 and bind(fs,'jp_gdfs_read_queue_release',I,Q)(authority+1)==2 and info().lease==authority,'wrong token cannot enter or release existing queue authority')
        for nm,ptr,cap,code in ((None,at,rounded,1),(b'X'*32,at,rounded,1),(b'../X',at,rounded,1),(name.encode(),at+1,rounded,1),(name.encode(),at,rounded-1,9),(name.encode(),None,rounded,1),(b'MISSING FILE',at,rounded,6)):
            check(add(nm,ptr,cap)==code and not info().queued and not fi().live_handles and bytes(raw)==before,'rejected enqueue preserves queue buffers and actual handle pool')
    check(add(name.encode(),at,rounded)==0 and slot(0).destination==at and slot(0).capacity==rounded,'original add enqueues real file with retained native bounds')
    check(add(name.encode(),at+32,rounded-32)==9,'overlapping live destination rejected before duplicate request')
    check(bind(queue,'jp_read_queue_snapshot',I,P)(at)==1 and bytes(raw)==before,'diagnostic output cannot overwrite a live destination')
    if a.case=='full':
        buffers=[(raw,at)]
        for i in range(1,16):
            rr,pp=buffer();buffers.append((rr,pp));check(add(name.encode(),pp,rounded)==0 and slot(i).state==1,'original queue admits each of sixteen distinct live buffers')
        rr,pp=buffer();check(add(name.encode(),pp,rounded)==7 and info().queued==16,'seventeenth request reports genuine fixed-pool saturation')
        drain()
        for rr,pp in buffers:check(C.string_at(pp,rounded)==expected,'all sixteen actual reads equal independent raw-sector oracle');guards(rr,pp)
        check(info().completed==16 and not fi().live_handles,'all original slots and real handles retire')
    elif a.case=='mixed':
        payloads=[(raw,at,rounded,expected)]
        for nm,(ff,nn,_) in eligible[1:3]:
            cap=(nn+2047)&~2047;rr=(C.c_ubyte*(cap+96))();C.memset(rr,0xa5,C.sizeof(rr));pp=(C.addressof(rr)+63)&~31;keep.append(rr)
            check(add(nm.encode(),pp,cap)==0,'original queue accepts distinct actual file metadata');payloads.append((rr,pp,cap,disc.sectors(ff,cap//2048)))
        current=Info();end=time.monotonic()+30
        while True:
            assert serve()==0 and bind(queue,'jp_read_queue_snapshot',I,P)(C.byref(current))==0 and time.monotonic()<end
            if current.completed:break
        check(current.current==2 and slot(0).state==0,'original first read retires before second selected slot progresses')
        rr,pp=buffer();payloads.append((rr,pp,rounded,expected));check(add(name.encode(),pp,rounded)==0,'retired lowest slot can be reused while another file remains active')
        order=[current.current]
        while current.queued:
            assert serve()==0 and bind(queue,'jp_read_queue_snapshot',I,P)(C.byref(current))==0 and time.monotonic()<end
            if current.current and current.current!=order[-1]:order.append(current.current)
        check(order==[2,1,3] and current.completed==4,'original lowest-waiting-slot scheduling survives real completion and slot reuse')
        for rr,pp,cap,oracle in payloads:
            off=pp-C.addressof(rr);check(C.string_at(pp,cap)==oracle and bytes(rr[:off])==b'\xa5'*off and bytes(rr[off+cap:])==b'\xa5'*(len(rr)-off-cap),'each distinct actual file and reused destination matches independent sectors with guards')
    elif a.case=='identity':
        table=(C.c_ubyte*(16+44*1920))();keep.append(table);dh=bind(fs,'gdFsCreateDirhn',P,P,I)(table,1920)
        check(dh and bind(fs,'gdFsLoadDir',I,C.c_char_p,P)(b'/SONICADV',dh)==0,'actual external directory table loaded for identity-change test')
        data=bytearray(table);count=struct.unpack_from('<i',data)[0];found=False
        for i in range(count):
            off=16+i*44
            if bytes(data[off+10:off+42]).split(b'\0')[0].upper()==name.encode():struct.pack_into('<i',data,off+4,bytes_+2048);found=True
        check(found,'actual file directory record located independently');C.memmove(table,bytes(data),len(data))
        check(bind(fs,'gdFsSetDir',I,P)(dh)==0 and serve()==10 and slot(0).state==1 and info().active_handle and bytes(raw)==before,'changed actual metadata is rejected before any destination write while retaining selected ownership')
        check(cancel()==0 and not info().queued and not fi().live_handles,'identity failure can safely cancel and release actual handle')
    elif a.case=='foreign':
        sentinel=Info();C.memset(C.byref(sentinel),0x5a,C.sizeof(sentinel));old=bytes(sentinel);results=[];state=bytes(info())
        def other():results.extend((serve(),cancel(),shutdown(),add(name.encode(),at,rounded),bind(queue,'jp_read_queue_snapshot',I,P)(C.byref(sentinel))))
        t=threading.Thread(target=other);t.start();t.join();check(results==[3]*5 and bytes(sentinel)==old and bytes(info())==state,'foreign caller cannot alter actual slots lease buffers or outputs');drain()
    elif a.case=='callback':
        h=bind(fs,'gdFsOpen',P,C.c_char_p,P)(name.encode(),None);rr,pp=buffer();events=[];opened=[];Callback=C.CFUNCTYPE(None,P)
        @Callback
        def callback(ctx):
            old=Info();bind(queue,'jp_read_queue_snapshot',I,P)(C.byref(old));values=(cancel(),shutdown(),serve(),add(name.encode(),pp,rounded));new=Info();bind(queue,'jp_read_queue_snapshot',I,P)(C.byref(new))
            events.append((values,bytes(old)==bytes(new)));opened.append(bind(fs,'gdFsOpen',P,C.c_char_p,P)(name.encode(),None))
        keep.append(callback);bind(fs,'gdFsEntryRdEndFunc',None,P,Callback,P)(h,callback,None)
        check(bind(fs,'gdFsReqRd32',I,P,I,P)(h,rounded//2048,pp)==rounded//2048,'independent actual client starts ahead of queued read')
        for _ in range(rounded//2048+100):
            check(serve()==0,'original queue cooperates with actual other GDFS client');
            if events:break
        check(events==[((4,4,4,4),True)] and opened[0],'real GDFS callback cannot reenter queue or acquire its handle authority')
        bind(fs,'gdFsClose',None,P)(h);bind(fs,'gdFsClose',None,P)(opened[0]);check(fi().live_handles<=1,'callback-opened independent handle remains independently closeable');drain()
    else:
        check(serve()==0 and info().current==1 and info().active_handle and fi().pending,'actual original queue starts a real multi-chunk pending read')
        if a.case=='protection':
            handle=info().active_handle;state=slot(0);bind(fs,'gdFsClose',None,P)(handle)
            check(bind(fs,'jp_gdfs_read_queue_release',I,Q)(info().lease)==11 and bind(fs,'jp_gdfs_read_queue_close',I,Q,P)(info().lease,handle)==1,'client lease cannot release an owned handle or close outside its entered scope')
            stolen=[];Callback=C.CFUNCTYPE(None,P)
            @Callback
            def forbidden(ctx):stolen.append(True)
            keep.append(forbidden);bind(fs,'gdFsEntryRdEndFunc',None,P,Callback,P)(handle,forbidden,None)
            check(bind(fs,'gdFsStopRd',I,P)(handle)==-10 and bind(fs,'gdFsSeek',I,P,I,I)(handle,0,0)==-10 and bind(fs,'gdFsReqRd32',I,P,I,P)(handle,1,at)==-10 and slot(0).state==state.state and fi().live_handles==1,'independent file APIs cannot steal leased queue handle or buffer');drain()
            check(not stolen,'independent callback registration cannot replace actual queue handle ownership')
        elif a.case=='cancel':
            check(cancel()==0 and not info().queued and not info().active_handle and not fi().live_handles,'real pending cancellation drains before releasing original slots and handle')
            stopped=C.string_at(at,rounded)
            for _ in range(20):bind(fs,'gdFsExecServer',None)()
            check(C.string_at(at,rounded)==stopped,'cancelled native reader never writes released caller buffer again')
        elif a.case=='bad_sector':
            code=0
            deadline=time.monotonic()+15
            while time.monotonic()<deadline:
                code=serve()
                if code:break
                ticks=U();assert bind(clock.lib,'jp_wait_pump',I,P)(C.byref(ticks))==0;time.sleep(.002)
            current=info();selected=slot(0);file_state=fi()
            (d/'failed_read_observation.json').write_text(json.dumps(dict(return_code=code,phase=current.phase,status=current.status,source_error=current.source_error,slot_state=selected.state,handle=current.active_handle,live_handles=file_state.live_handles,source_status=file_state.source_error),indent=2)+'\n')
            check(code==6 and current.phase==2 and selected.state==-1 and current.active_handle and file_state.live_handles==1,'real malformed-sector I/O error retains original selected slot and handle instead of retrying forever')
            check(cancel()==0 and not info().queued and not fi().live_handles,'actual failed read safely drains and cancels');stopped=C.string_at(at,rounded)
            for _ in range(10):bind(fs,'gdFsExecServer',None)()
            check(C.string_at(at,rounded)==stopped,'failed/cancelled real reader cannot write after caller release')
        else:
            drain()
            if a.case=='zero_length':
                table=(C.c_ubyte*(16+44*1920))();keep.append(table);dh=bind(fs,'gdFsCreateDirhn',P,P,I)(table,1920)
                check(dh and bind(fs,'gdFsLoadDir',I,C.c_char_p,P)(b'/SONICADV',dh)==0,'actual directory copy prepared for zero-length entry')
                data=bytearray(table);found=False
                for i in range(struct.unpack_from('<i',data)[0]):
                    off=16+i*44
                    if bytes(data[off+10:off+42]).split(b'\0')[0].upper()==name.encode():struct.pack_into('<i',data,off+4,0);found=True
                check(found,'actual test entry size changed to zero');C.memmove(table,bytes(data),len(data))
                check(bind(fs,'gdFsSetDir',I,P)(dh)==0 and add(name.encode(),None,0)==0 and slot(0).file_bytes==0,'real zero-sector request needs no caller destination')
                drain();check(info().completed==2 and not fi().live_handles,'original zero-length request completes and retires without a buffer write')
    if a.case not in ('cancel','identity','bad_sector'):
        check(C.string_at(at,rounded)==expected and not info().queued,'complete actual destination equals independent ISO/raw-sector bytes')
    guards(raw,at);token=info().lease
    check(shutdown()==0 and not info().initialized and not info().lease,'original queue retires before releasing actual GDFS client lease')
    if a.case=='restart':
        check(init()==0 and info().lease>token and add(name.encode(),at,rounded)==0,'same native owner restarts using a fresh nonrecycled client token');drain();check(C.string_at(at,rounded)==expected and shutdown()==0,'restarted real queue completes then retires')
    check(bind(fs,'jp_gdfs_shutdown',I)()==0 and not fi().work and not fi().dir,'real GDFS arenas and media can retire only after queue releases ownership')
    check(bind(clock.lib,'jp_wait_shutdown',I)()==0,'caller retires actual borrowed wait/vblank owner');clock.shutdown();disc.f.close();faulthandler.disable();fault.close()
    out=dict(case=a.case,checks=len(labels),assertions=labels,observations=dict(file=name,file_bytes=bytes_,rounded_bytes=rounded,expected_payload_sha256=hashlib.sha256(expected).hexdigest(),**obs),
        build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10)
    (d/'result.json').write_text(json.dumps(out,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)
if __name__=='__main__':main()
