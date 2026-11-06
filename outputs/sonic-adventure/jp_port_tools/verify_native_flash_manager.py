"""Real OS images, original FM phases/CRC, independent wire-format oracles."""
import argparse,binascii,ctypes as C,hashlib,json,random,struct,subprocess,sys,threading
from pathlib import Path
from verify_native_audio import bind
I,U,Q,P=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p
GEOMETRY=((0x1a000,0x2000),(0x18000,0x2000),(0x1c000,0x4000),(0x10000,0x8000),(0,0x10000))
CASES=('normal','geometry','queue_contract','full_log','corruption','denied_program','denied_erase',
       'external_change','threads','explicit_drop','invalid_profile')
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('reads','programs','erases','commits')]+[(n,U) for n in ('status','os_error','creator_thread','opened')]
class FMInfo(C.Structure):
    _fields_=[(n,I) for n in ('part','type','address','bytes','capacity','used','request','status','phase','error')]+[('borrowed_work',Q)]
class Block(C.Structure):_fields_=[('address',I),('bytes',I)]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def crc(data):return binascii.crc_hqx(data,65535)^65535
def packet(number,data):
    payload=struct.pack('<H',number)+data;assert len(payload)==62
    return payload+struct.pack('<H',crc(payload))
def fixture(records,part=3):
    image=bytearray(b'\xff'*131072);start,size=GEOMETRY[part];capacity=size//64-(size+32767)//32768-1
    image[start:start+18]=b'KATANA_FLASH____'+bytes((part,0))
    for i,row in enumerate(records):image[start+(i+1)*64:start+(i+2)*64]=row
    at=start+(capacity+1)*64
    for i in range(len(records)):image[at+i//8]&=~(128>>(i%8))
    return image
def decode(image,part=3):
    start,size=GEOMETRY[part];capacity=size//64-(size+32767)//32768-1
    assert image[start:start+18]==b'KATANA_FLASH____'+bytes((part,0))
    bitmap=image[start+(capacity+1)*64:start+size];used=0
    for n in range(capacity):
        if bitmap[n//8]&(128>>(n%8)):break
        used+=1
    result={}
    for n in range(used):
        raw=image[start+(n+1)*64:start+(n+2)*64];number=struct.unpack_from('<H',raw)[0]
        if number==65535:break
        if number<capacity and crc(raw[:62])==struct.unpack_from('<H',raw,62)[0]:result[number]=raw[2:62]
    return used,result

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES+('lock_contender','reopen'));ap.add_argument('--media',type=Path)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['flash_DLL'])==b['flash_DLL_sha256']
    def child(case,output,media=None):
        cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
        for n in ('workspace','build'):cmd+=['--'+n,str(getattr(a,n).resolve())]
        cmd+=['--output',str(output),'--case',case]
        if media:cmd+=['--media',str(media)]
        run=subprocess.run(cmd,capture_output=True,timeout=120,creationflags=subprocess.CREATE_NO_WINDOW)
        (d/(case+'.log')).write_bytes(run.stdout+run.stderr)
        assert run.returncode==0,(case,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-6000:])
        return read(output/'result.json')
    if not a.case:
        cases={}
        for n in CASES:
            cases[n]=child(n,d/n);print(n,cases[n]['checks'],'checks',flush=True)
        result=dict(status='native_persistent_flash_original_FM_CRC_phases_and_real_failure_recovery_verified',
            checks=sum(v['checks'] for v in cases.values()),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
            full_history_UF_library_or_game_init_executed=False,retail_flash_or_user_saves_used=False,
            playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',flush=True);return
    lib=C.CDLL(b['flash_DLL']);labels=[];obs={};keep=[]
    def check(ok,label):
        assert ok,(a.case,label);labels.append(label)
        (d/'progress.json').write_text(json.dumps(dict(case=a.case,checks=len(labels),assertions=labels),indent=2)+'\n')
    def finish():
        result=dict(case=a.case,checks=len(labels),assertions=labels,observations=obs,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__))
        (d/'result.json').write_text(json.dumps(result,indent=2)+'\n')
    open_=bind(lib,'jp_flash_open',I,C.c_wchar_p,U);close=bind(lib,'jp_flash_close',I)
    status=bind(lib,'jp_flash_status',I);clear=bind(lib,'jp_flash_clear_error',None)
    init=bind(lib,'fmMngInit',I,I,P);format_=bind(lib,'fmMngFormat',I,I)
    getfree=bind(lib,'fmMngGetFree',I);error=bind(lib,'fmMngGetErr',I);step=bind(lib,'fmMngExecServer',I)
    end=bind(lib,'fmMngFinish',None);refresh=bind(lib,'fmMngRefresh',I)
    write=bind(lib,'fmMngWrite',I,I,P);read_=bind(lib,'fmMngRead',I,I,P)
    rawwrite=bind(lib,'syBtFlWrite',I,I,P,I);rawread=bind(lib,'syBtFlRead',I,I,P,I);erase=bind(lib,'syBtFlDeleteSct',I,I)
    def snapshot(name,kind):
        v=kind();check(bind(lib,name,I,P)(C.byref(v))==0,'actual '+name);return v
    def info():return snapshot('jp_flash_snapshot',Info)
    def fm():return snapshot('jp_flash_fm_snapshot',FMInfo)
    def aligned_work():
        raw=(C.c_ubyte*(65536+64))();C.memset(C.addressof(raw),0x5a,C.sizeof(raw));keep.append(raw);return C.addressof(raw)+32
    def write_packet(number,data):
        v=C.create_string_buffer(data);keep.append(v);check(write(number,v)==0,'original packet write queued')
        phases=[]
        for _ in range(4):
            code=step();phases.append(code)
            if code!=2:break
        check(phases[-1]==1 and error()==0,'original packet request genuinely completes');return phases
    def read_packet(number,data):
        v=C.create_string_buffer(60);check(read_(number,v)==0 and step()==1 and error()==0,'original packet read completes')
        check(v.raw==data,'original full sixty-byte packet payload matches independent expectation')
    media=a.media.resolve() if a.media else d/'media'
    if not media.exists():media.mkdir()
    path=media/'SONIC_FLASH.DAT';work=aligned_work()
    if a.case=='lock_contender':
        before=sha(path);check(open_(str(media),0)==7,'second process cannot take live profile lock')
        check(sha(path)==before,'rejected lock owner preserves exact disk image');finish();return
    if a.case=='reopen':
        expected=read(media/'expected.json');check(open_(str(media),0)==0,'new process opens persisted native flash')
        check(init(3,work)==0,'new process rebuilds actual original cache from disk')
        for n,data in expected.items():read_packet(int(n),bytes.fromhex(data))
        check(close()==0,'new process closes real original manager and profile');finish();return
    if a.case=='invalid_profile':
        path.write_bytes(b'keep invalid user profile intact');old=path.read_bytes()
        check(open_(str(media),1)==6 and path.read_bytes()==old,'wrong-sized existing profile is rejected without autoformat')
        path.write_bytes(b'\xff'*131072);check(open_(str(media),0)==0,'failed open released actual exclusive lock')
        check(close()==0,'corrected isolated image closes');finish();return
    if a.case in ('full_log','explicit_drop'):
        expected=bytes([253])*60;path.write_bytes(fixture([packet(7,bytes([i%256])*60) for i in range(510)]))
    elif a.case=='corruption':
        good=bytes(range(60));bad=bytearray(packet(1,b'B'*60));bad[-1]^=1
        path.write_bytes(fixture([packet(1,good),bytes(bad),packet(9000,b'C'*60),packet(2,b'D'*60)]))
    check(status()==2 and error()==-11 and close()==0,'cold owner has no manufactured open manager')
    check(open_(str(media),1)==0,'real exclusive native profile opens or creates erased media')
    if a.case not in ('full_log','explicit_drop','corruption'):check(path.read_bytes()==b'\xff'*131072,'fresh profile has no imported BIOS or retail bytes')
    if a.case=='geometry':
        intervals=[]
        for part,(start,size) in enumerate(GEOMETRY):
            v=Block();check(bind(lib,'syBtFlGetInfo',I,I,P)(part,C.byref(v))==0 and (v.address,v.bytes)==(start,size),'documented partition geometry')
            intervals.append((start,start+size))
        check(sorted(intervals)==[(0,65536),(65536,98304),(98304,106496),(106496,114688),(114688,131072)],'five partitions cover exact image without overlap')
        check(rawwrite(0x1a000,C.create_string_buffer(b'X'),1)==-1 and status()==10,'actual factory partition is protected');clear()
        check(erase(0x1a001)==-1 and status()==10,'containing-sector erase also protects factory partition');clear()
        x=C.create_string_buffer(b'\xf0\x0f');check(rawwrite(0,x,2)==2,'first raw flash programming succeeds')
        check(rawwrite(0,C.create_string_buffer(b'\x0f\xf0'),2)==2,'subsequent flash program uses one-to-zero semantics')
        out=C.create_string_buffer(2);check(rawread(0,out,2)==2 and out.raw==b'\0\0','raw programming cannot restore cleared bits')
        check(erase(1)==0 and path.read_bytes()[:65536]==b'\xff'*65536,'erase restores entire containing partition')
        for part in (1,2,3,4):
            size=GEOMETRY[part][1];check(init(part,work)==-8 and format_(0)==0,'original manager formats its actual initially blank partition')
            capacity=size//64-(size+32767)//32768-1
            check(getfree()==capacity and fm().capacity==capacity,'actual manager computes packet capacity for geometry')
            before=path.read_bytes();check(format_(9)==-6 and path.read_bytes()==before,'invalid format type preserves actual image')
            check(write(capacity,C.create_string_buffer(60))==-5 and read_(-1,C.create_string_buffer(60))==-5,'original packet bounds reject missing slots')
            check(bytes((C.c_ubyte*32).from_address(work-32))==b'Z'*32 and bytes((C.c_ubyte*32).from_address(work+size))==b'Z'*32,'original cache writes stay inside borrowed work extent')
            end();C.memset(work,0x5a,65536)
        old=path.read_bytes();check(rawread(131071,C.create_string_buffer(2),2)==-1 and status()==1 and path.read_bytes()==old,'raw range rejection preserves disk');clear()
        check(close()==0,'geometry owner closes');finish();return
    if a.case in ('full_log','explicit_drop'):
        check(init(3,work)==0 and getfree()==0,'actual manager reads independent full-log fixture')
        read_packet(7,expected);data=C.create_string_buffer(b'NEW'*20);keep.append(data)
        check(write(8,data)==0 and step()==2 and fm().phase==1,'full original log enters erase phase after staging real packet')
        check(close()==7 and init(2,work)==-9 and refresh()==-9 and format_(0)==-9,'pending request prevents owner close or conflicting manager replacement')
        check(step()==2 and fm().phase==2,'actual second phase erases history sector')
        if a.case=='explicit_drop':
            end();check(path.read_bytes()[65536:98304]==b'\xff'*32768,'explicit original finish after erase leaves the real sector erased')
            check(close()==0,'explicit cancellation releases borrowed request/work');finish();return
        check(step()==1 and error()==0 and getfree()==508,'actual third phase rewrites compacted original cache')
        used,values=decode(path.read_bytes());check(used==2 and values=={7:expected,8:b'NEW'*20},'independent disk decoder verifies compacted CRC map and latest payloads')
        end();check(init(3,work)==0,'fresh original cache reload after compaction');read_packet(7,expected);read_packet(8,b'NEW'*20)
        check(close()==0,'compaction owner closes');finish();return
    if a.case=='corruption':
        check(init(3,work)==0,'actual original cache tolerates corrupt log entries')
        read_packet(1,good);read_packet(2,b'D'*60)
        check(decode(path.read_bytes())[1]=={1:good,2:b'D'*60},'independent decoder rejects invalid CRC and number entries')
        C.c_ubyte.from_address(work+2*64+2).value^=1
        out=C.create_string_buffer(60);check(read_(1,out)==0 and step()==3 and error()==-4,'genuine corrupted cache reports original CRC failure')
        check(out.raw[0]==(good[0]^1),'original read copies bytes before reporting CRC failure')
        end();check(init(3,work)==0,'reload restores only persisted valid packets');read_packet(1,good)
        check(close()==0,'corruption owner closes');finish();return
    check(init(3,work)==-8 and format_(0)==0,'actual original manager formats blank history partition')
    baseline=b'A'*60;write_packet(5,baseline)
    if a.case=='queue_contract':
        before=path.read_bytes();old=fm()
        check(init(0,work)==-7 and bytes(fm())==bytes(old),'invalid original partition preserves current manager')
        check(init(3,work+1)==-1 and status()==1,'unaligned native work is rejected before original cache use');clear()
        check(write(0,None)==-2 and status()==1 and fm().request==0,'null request buffer cannot start unsafe original write');clear()
        data=C.create_string_buffer(b'A'*60);keep.append(data)
        check(write(0,data)==0 and close()==7 and read_(1,C.create_string_buffer(60))==-9,'queued request retains borrowed buffer and excludes a second request')
        C.memset(data,ord('B'),60)
        check(step()==1 and error()==0,'original first server pass consumes live borrowed data');read_packet(0,b'B'*60)
        before=path.read_bytes();check(write(0,data)==0,'queue actual request for explicit pre-service cancellation')
        end();check(path.read_bytes()==before and init(3,work)==0,'explicit finish before service drops request without disk change');read_packet(0,b'B'*60)
        check(format_(1)==-6 and path.read_bytes()[65536+17]==1,'original protected type is persisted but not writable')
        old=path.read_bytes();out=C.create_string_buffer(60)
        check(read_(0,out)==0 and step()==3 and error()==-6,'original protected manager rejects packet reads')
        check(write(0,data)==0 and step()==3 and error()==-6 and path.read_bytes()==old,'original protected manager rejects writes without disk mutation')
        check(format_(0)==0,'explicit writable reformat recovers original manager')
    elif a.case in ('denied_program','denied_erase'):
        before=path.read_bytes();counter=info();kernel=C.WinDLL('kernel32',use_last_error=True)
        h=bind(kernel,'CreateFileW',P,C.c_wchar_p,U,U,P,U,U,P)(str(path),0x80000000,1,None,3,0x80,None)
        check(h and h!=P(-1).value,'real OS sharing mode denies atomic profile replacement')
        if a.case=='denied_program':
            value=C.create_string_buffer(b'B'*60);keep.append(value)
            check(write(5,value)==0 and step()==3 and error()==-2,'original map/program phase propagates real disk failure')
            obs['original_failed_write_cache_can_differ_from_disk']=fm().error==-2
        else:check(refresh()==-3,'original refresh propagates real denied sector erase')
        now=info();check(path.read_bytes()==before and (now.programs,now.erases,now.commits)==(counter.programs,counter.erases,counter.commits),'failed atomic primitive preserves exact disk and committed RAM counters')
        check(status()==5 and now.os_error!=0 and not list(media.glob('.flash-*.tmp')),'real OS error is observable and temporary output removed')
        check(bind(kernel,'CloseHandle',I,P)(h)!=0,'caller removes actual denial handle')
        clear();end();check(init(3,work)==0,'caller rebuilds original failed-request cache from persisted truth');read_packet(5,baseline)
        write_packet(5,b'C'*60);check(decode(path.read_bytes())[1][5]==b'C'*60,'corrected OS condition permits real retry and disk commit')
    elif a.case=='external_change':
        end();foreign=bytearray(path.read_bytes());foreign[0x18000]=0x12;path.write_bytes(foreign)
        check(rawwrite(0,C.create_string_buffer(b'X'),1)==-1 and status()==9,'real out-of-band image modification is detected before overwrite')
        check(path.read_bytes()==foreign,'changed image is preserved exactly')
        check(close()==0 and open_(str(media),0)==0,'explicit close/reopen adopts external disk state')
        check(init(3,work)==0,'original cache reloads valid untouched history');read_packet(5,baseline)
    elif a.case=='threads':
        previous=fm();out=FMInfo();C.memset(C.byref(out),0x5a,C.sizeof(out));old=bytes(out);results=[]
        def other():results.extend((status(),close(),init(3,work),step(),bind(lib,'jp_flash_fm_snapshot',I,P)(C.byref(out))))
        t=threading.Thread(target=other);t.start();t.join()
        check(results==[3,3,-11,3,3] and bytes(out)==old,'foreign thread rejects native and SDK operations without output mutation')
        check(bytes(fm())==bytes(previous),'wrong-thread calls preserve actual original manager state')
    else:
        crc_fn=bind(lib,'fmcalccrc',U,I,P);rng=random.Random(0x5341464d)
        for n in range(513):
            data=bytes(rng.randrange(256) for _ in range(n));buf=C.create_string_buffer(data)
            check(crc_fn(n,buf)==crc(data),'original table CRC equals independent bitstream CRC for length '+str(n))
        for n in range(9):write_packet(n,bytes([n])*60)
        expected={n:bytes([n])*60 for n in range(9)};used,actual=decode(path.read_bytes())
        check(used==10 and actual==expected,'independent disk oracle sees ten appends and latest nine logical packets')
        old=path.read_bytes();check(rawwrite(0,C.create_string_buffer(b'X'),1)==-1 and status()==7 and path.read_bytes()==old,'direct raw mutation cannot bypass live original manager ownership');clear()
        contender=child('lock_contender',d/'lock_contender',media);obs['contender_checks']=contender['checks']
        end();check(init(3,work)==0,'same original manager rebuilds cache from exact disk')
        for n,data in expected.items():read_packet(n,data)
        (media/'expected.json').write_text(json.dumps({str(n):data.hex() for n,data in expected.items()}))
        check(close()==0,'first owner releases actual image and manager before independent process')
        reopened=child('reopen',d/'reopen',media);obs['independent_reopen_checks']=reopened['checks']
        check(open_(str(media),0)==0 and init(3,work)==0,'original owner reacquires OS lock and cache after independent process')
    check(close()==0 and close()==0,'real manager and profile close without lost cleanup obligation')
    finish()
if __name__=='__main__':main()
