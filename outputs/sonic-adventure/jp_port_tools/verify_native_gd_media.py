"""Real overlapped I/O/cancellation and original ISO code on user-owned media."""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys,threading
from pathlib import Path
from native_packet_test_support import bind
from model_reference import make_reference
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p;S=C.c_size_t
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('file_bytes','requests','completed','cancelled','raw_bytes_read','payload_bytes')]+[(n,U) for n in ('first_fad','sectors','raw_sector_bytes','maximum_request_sectors','request_fad','request_sectors','state','status','os_error','initialized')]
def sha(path):
    h=hashlib.sha256()
    with Path(path).open('rb') as f:
        for block in iter(lambda:f.read(8<<20),b''):h.update(block)
    return h.hexdigest()
def digest(raw):return hashlib.sha256(raw).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','iso','binary','gdi','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes());ib=json.loads((a.iso/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for meta in (b,ib):
        for n,h in meta['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in ib['original_source_sha256'].items():assert sha(u/n)==h,n
    assert sha(b['DLL'])==b['DLL_sha256']==ib['media_DLL_sha256'] and sha(ib['DLL'])==ib['DLL_sha256']
    lib=C.CDLL(b['DLL']);peer=C.CDLL(ib['DLL']);checks=0;groups={};rng=random.Random(0x47444653)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def group(n):groups[n]=checks-sum(groups.values())
    open_=bind(lib,'jp_gd_media_open',I,C.c_wchar_p,U,U,U);request=bind(lib,'jp_gd_media_request',I,U,U);poll=bind(lib,'jp_gd_media_poll',I);wait=bind(lib,'jp_gd_media_wait',I,U);cancel=bind(lib,'jp_gd_media_cancel',I,U)
    release=bind(lib,'jp_gd_media_release',I);close=bind(lib,'jp_gd_media_close',I,U);view=bind(lib,'jp_gd_media_view',I,P,P);snapshot=bind(lib,'jp_gd_media_snapshot',I,P);status=bind(lib,'jp_gd_media_status',I)
    parse=bind(peer,'peer_parse_record',I,P,S,P);listing=bind(peer,'peer_list',I,I,I,I,P,S);search=bind(peer,'peer_search',I,C.c_char_p,P,I,I)
    def info():out=Info();check(snapshot(C.byref(out))==0,'native transport snapshot');return out
    def read_native(fad,n):
        check(request(fad,n)==0,'actual overlapped sector request submitted');check(wait(5000)==0 and info().state==2,'actual file request completed and decoded')
        p=P(0x1234);size=S(123);check(view(C.byref(p),C.byref(size))==0 and size.value==n*2048 and p.value>0xffffffff,'owned full-width contiguous payload view')
        raw=C.string_at(p,size.value);check(release()==0 and info().state==0,'completed request buffers actually released');return raw
    check(C.sizeof(Info)==88 and bind(peer,'peer_entry_bytes',U)()==44,'native state and original serialized directory-entry ABI')
    p=P(0x1234);size=S(0x5678);check(status()==2 and view(C.byref(p),C.byref(size))==2 and p.value==0x1234 and size.value==0x5678 and close(0)==2,'cold transport preserves outputs and reports unavailable owner')
    fixture=d/'fixtures';fixture.mkdir();cooked=fixture/'cooked.bin';payload=b''.join(bytes([j])*2048 for j in range(256));cooked.write_bytes(payload)
    short=fixture/'short.bin';short.write_bytes(b'bad')
    check(open_(None,45150,2048,8)==1 and open_(str(short),45150,2048,8)==8 and open_(str(cooked),0,2048,8)==1 and open_(str(cooked),45150,2336,8)==1,'invalid paths/geometry/formats cannot open media')
    check(open_(str(cooked),45150,2048,256)==0 and open_(str(cooked),45150,2048,256)==4,'real cooked track opens once')
    check(info().file_bytes==len(payload) and info().sectors==256,'actual file length determines sector bounds')
    for fad,n in ((45149,1),(45150,0),(45406,1),(45405,2),(45150,257),(0xffffffff,0xffffffff)):
        check(request(fad,n)==5 and info().state==0,'invalid/overflow/out-of-track request cannot start I/O')
    check(read_native(45153,7)==payload[3*2048:10*2048],'cooked multi-sector bytes match independently created file data')
    check(request(45150,256)==0 and request(45151,1)==9 and release()==9,'pending OS write buffers cannot be replaced or freed')
    foreign=[];sentinel=Info();C.memset(C.byref(sentinel),0xa5,C.sizeof(sentinel));before=bytes(sentinel)
    thread=threading.Thread(target=lambda:foreign.extend([request(45150,1),poll(),wait(0),cancel(0),release(),close(0),snapshot(C.byref(sentinel))]));thread.start();thread.join()
    check(foreign==[3]*7 and bytes(sentinel)==before,'foreign thread cannot observe/mutate/close pending owner')
    observed_wait=wait(0);check(observed_wait in (0,10),'zero-time read wait either observes actual completion or bounded timeout')
    check(wait(5000)==0 and info().state==2,'same pending request completes after nonblocking observation')
    before_p=P(0x1234);check(view(C.byref(before_p),C.byref(before_p))==1 and before_p.value==0x1234,'overlapping view outputs reject without a partial write')
    check(release()==0 and request(45150,256)==0,'another real request for cancellation')
    cancelled=cancel(0);check(cancelled in (0,10),'cancellation has a bounded nonblocking result')
    check(cancel(5000)==0 and info().state==5 and view(C.byref(p),C.byref(size))==4 and release()==0,'actual cancellation drains OS before buffer release and never publishes cancelled output')
    check(wait(60001)==1 and cancel(60001)==1 and close(60001)==1,'unsupported wait lengths reject without closing owner')
    try:
        with cooked.open('r+b'):pass
        blocked=False
    except PermissionError:blocked=True
    check(blocked,'real file owner prevents a concurrent writer from modifying its input')
    check(request(45150,256)==0,'real request pending during close');rc=close(0);check(rc in (0,10),'close drains or preserves actual live I/O after bounded timeout')
    if rc==10:check(close(5000)==0,'same timed-out close retries actual owned I/O safely')
    check(status()==2 and sha(cooked)==digest(payload),'close retires owner and preserves original cooked file bytes');group('actual_cooked_IO_lifetimes_cancellation_and_threads')
    # Raw geometry validates every sector, including a bad later header.
    def raw_sector(n):
        out=bytearray(2352);out[:16]=bytes.fromhex('00ffffffffffffffffffff00c2037401');out[16:2064]=bytes([n])*2048;return bytes(out)
    raw=fixture/'raw.bin';raw.write_bytes(raw_sector(3)+raw_sector(9));bad=fixture/'bad-raw.bin';badraw=bytearray(raw.read_bytes());badraw[2352+15]=2;bad.write_bytes(badraw)
    check(open_(str(raw),45150,2352,8)==0 and read_native(45150,2)==bytes([3])*2048+bytes([9])*2048 and close(5000)==0,'GD extended-minute headers and raw2352 payload stripping work on every sector')
    check(open_(str(bad),45150,2352,8)==0 and request(45150,2)==0 and wait(5000)==8 and info().state==3,'bad second raw-sector header fails entire request')
    p=P(0x1234);size=S(0x5678);check(view(C.byref(p),C.byref(size))==4 and p.value==0x1234 and size.value==0x5678 and release()==0 and close(5000)==0,'raw validation failure never exposes a partial decoded payload')
    group('raw_Mode1_payload_validation_and_no_partial_output')
    # User-owned GDI is metadata only; no disc contents enter public artifacts.
    gdi=a.gdi.resolve();gdi_hash=sha(gdi);text=gdi.read_text();lines=text.strip().splitlines();check(int(lines[0])==3,'supplied Japanese GDI has three described tracks')
    row=next(line.split() for line in lines[1:] if line.split()[0]=='3');check(row[:4]==['3','45000','4','2352'] and row[-1]=='0','actual data track geometry matches original first-FAD45150')
    track=gdi.parent/row[4];original_hash=sha(track);track_bytes=track.stat().st_size;first=45150;total=track_bytes//2352
    check(open_(str(track),first,2352,8192)==0 and info().sectors==total,'real Japanese raw data track opened read-only')
    with track.open('rb') as f:
        def independent(fad,n=1):
            out=bytearray()
            for j in range(n):
                f.seek((fad-first+j)*2352+16);block=f.read(2048);assert len(block)==2048;out+=block
            return bytes(out)
        for fad,n in [(45150,1),(45166,1),(449999,2),(first+total-1,1)]+[(rng.randrange(first,first+total-8),rng.randrange(1,9)) for _ in range(32)]:
            check(read_native(fad,n)==independent(fad,n),'real native sector output matches independent raw-file extraction, including beyond minute99')
        pvd=read_native(45166,1);check(pvd[0]==1 and pvd[1:6]==b'CD001','actual primary volume descriptor reaches native reader')
        root_fad=struct.unpack_from('<I',pvd,0x9e)[0]+150;root_bytes=struct.unpack_from('<I',pvd,0xa6)[0]
        def directory(fad,size):
            records=[]
            for n in range((size+2047)//2048):
                data=independent(fad+n);at=0
                while at<2048 and data[at]:
                    length=data[at];rec=data[at:at+length];assert length>=34 and len(rec)==length
                    nlen=rec[32];flag=rec[25];name=rec[33:33+nlen]
                    if flag&2:name=b'.' if name==b'\0' else b'..' if name==b'\1' else name
                    else:name=name[:-2]
                    sector,fsize=struct.unpack_from('<I',rec,2)[0],struct.unpack_from('<I',rec,10)[0]
                    sid=((218+sum(name.upper()))>>2)&255
                    records.append((sector,fsize,flag,sid,name,rec));at+=length
            return records
        root=directory(root_fad,root_bytes);sonic=next(row for row in root if row[4]==b'SONICADV');sonic_fad=sonic[0]+150;sonic_bytes=sonic[1];game=directory(sonic_fad,sonic_bytes)
        sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
        from verify_fpu_cleanup_transfer import Fpu
        reference,reference_hashes=make_reference(executor);symbols={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(u/'config/1st_read/symbols.txt').read_text(),re.M)}
        check(sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf','preserved Japanese instruction oracle hash')
        ram=bytearray(16<<20);image=a.binary.read_bytes();ram[0x10000:0x10000+len(image)]=image;calls={}
        def put(at,data):at&=0xffffff;ram[at:at+len(data)]=data
        def original(name,*args):
            cpu=executor.State();cpu.pc=symbols[name];cpu.pr=0x8cfed000;cpu.r[15]=0x8cf00000;cpu.sr=0x70000000
            for j,value in enumerate(args):cpu.r[4+j]=value&0xffffffff
            fpu=Fpu();fpu.fpscr=0x40001;reference(ram,cpu,fpu=fpu,instruction_budget=400000);calls[name]=calls.get(name,0)+1;check(cpu.r[15]==0x8cf00000,'complete original directory call preserves stack');return cpu.r[0]
        catalogues={};all_records=root+game
        for label,fad,size,records in [('root',root_fad,root_bytes,root),('SONICADV',sonic_fad,sonic_bytes,game)]:
            capacity=1920;length=16+capacity*44;out=(C.c_ubyte*(length+64))();C.memset(out,0xa5,len(out))
            check(len(records)<capacity and listing(fad,size,capacity,out,length)==0,'original full directory loader reads actual media through native I/O')
            full_directory_reads=bind(peer,'peer_sector_reads',U)()
            check(struct.unpack('<3i',bytes(out[:12]))==(len(records),capacity,fad) and bytes(out[length:])==b'\xa5'*64,'original directory header/count and caller guard bytes')
            for j,(sector,fsize,flag,sid,name,record) in enumerate(records):
                data=bytes(out[16+j*44:16+(j+1)*44]);check(struct.unpack_from('<iiBB',data)==(sector,fsize,flag,sid) and data[10:42].split(b'\0')[0]==name,'every native original directory entry matches independent ISO9660 parsing')
                expected=next(k+1 for k,x in enumerate(records) if x[4]==name)
                check(search(name,out,capacity,1)==expected and search(name.lower(),out,capacity,0)==expected,'original exact and case-insensitive search returns actual directory ordinal')
                put(0x8cf10000,record);put(0x8cf11000,b'\xa5'*60);old=original('read_dirrec',0x8cf10000,0x8cf11000);entry=(C.c_ubyte*60)(*([0xa5]*60));buf=C.create_string_buffer(record)
                check(parse(buf,len(record),entry)==old==len(record) and bytes(entry)==bytes(ram[0xf11000:0xf11000+60]),'native original record parser matches complete unhooked original instructions and canaries')
            check(search(b'NOT_PRESENT.XXX',out,capacity,0)==0,'actual original missing-name result')
            put(0x8cd00000,bytes(out[:length]))
            for j in sorted(set([0,len(records)//2,len(records)-1]+list(range(0,len(records),79)))):
                name=records[j][4].lower()+b'\0';put(0x8cf12000,name)
                old=original('gdFsSearchName',0x8cf12000,0x8cd00000,0);check(old==search(name[:-1],out,capacity,0),'original complete directory search instruction path matches relocated native table')
            limited=(C.c_ubyte*128)(*([0x5a]*128));check(listing(fad,size,1,limited,60)==-12 and bytes(limited)==b'\x5a'*128,'original DIROVER failure is staged without damaging caller table')
            catalogues[label]=dict(fad=fad,bytes=size,entries=len(records),table_sha256=digest(bytes(out[:length])),original_directory_sector_reads=full_directory_reads)
        # Real game file payloads, not merely directory names.
        files=[row for row in all_records if not row[2]&2 and row[1]>0]
        chosen=[min(files,key=lambda row:row[1]),next(row for row in game if row[4].endswith(b'.PRS') and 0<row[1]<=8192*2048),next(row for row in root if row[4]==b'1ST_READ.BIN')];file_proofs=[]
        assert len({row[4] for row in chosen})==3
        for sector,size,flag,sid,name,record in chosen:
            sectors=(size+2047)//2048;check(sectors<=8192,'selected actual game file fits declared transport request bound')
            data=read_native(sector+150,sectors)[:size];check(data==independent(sector+150,sectors)[:size],'complete actual game file bytes match independent extraction')
            file_proofs.append(dict(name=name.decode('ascii'),bytes=size,sha256=digest(data),fad=sector+150))
    real_state=info();check(close(5000)==0 and sha(track)==original_hash and sha(gdi)==gdi_hash,'original Japanese GDI/data track remain byte-for-byte unchanged after all native reads')
    group('actual_Japanese_GD_track_original_ISO_directory_and_file_payloads')
    # Malformed records are rejected before entering unsafe original copies.
    for raw in [b'\0'*64,bytes([255])+b'\0'*63,bytes([34])+b'\0'*31+bytes([255])+b'\0'*31]:
        source=C.create_string_buffer(raw);out=(C.c_ubyte*60)(*([0xa5]*60));check(parse(source,len(raw),out)==-1 and bytes(out)==b'\xa5'*60,'malformed record cannot write outside original name field or change output')
    group('malformed_original_record_boundary')
    result=dict(status='real_native_GD_media_cancellation_original_ISO_directories_and_actual_JP_file_bytes_verified',checks=checks,groups=groups,
        original_complete_machine_calls=sum(calls.values()),original_calls=calls,original_instruction_function_hooks_used=False,reference_hashes=reference_hashes,
        media=dict(gdi=str(gdi),gdi_sha256=sha(gdi),data_track=str(track),data_track_sha256=original_hash,data_track_bytes=track_bytes,raw_sector_bytes=2352,first_fad=45150,sectors=total),
        native_real_media_requests=real_state.requests,native_real_payload_bytes=real_state.payload_bytes,catalogues=catalogues,actual_game_files=file_proofs,
        original_inputs_modified=False,zero_wait_observed_result=observed_wait,zero_cancel_observed_result=cancelled,
        build_verification_sha256=sha(a.build/'build_verification.json'),ISO_build_verification_sha256=sha(a.iso/'build_verification.json'),verifier_sha256=sha(__file__),
        full_GDFS_DCF_system_bootstrap_audio_or_gameplay_verified=False,original_ISO_peer_DLL_publishable=False,authored_transport_DLL_publishable=True,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',sum(calls.values()),'original calls',flush=True)
if __name__=='__main__':main()
