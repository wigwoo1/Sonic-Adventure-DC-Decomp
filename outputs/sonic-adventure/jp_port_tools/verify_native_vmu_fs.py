"""Exercise the real original filesystem on fresh private disk-backed VMUs.
No retail saves, patched game callbacks, or replacement filesystem oracle.
"""
import argparse, ctypes as C, hashlib, json, random, struct, subprocess, threading
from pathlib import Path

I=C.c_int32; U=C.c_uint32; P=C.c_void_p
INIT=C.CFUNCTYPE(None); COMPLETE=C.CFUNCTYPE(I,I,I,I,U); PROGRESS=C.CFUNCTYPE(I,I,I,I,I)
class Time(C.Structure):
    _fields_=[('year',C.c_uint16)]+[(n,C.c_uint8) for n in ('month','day','hour','minute','second','weekday')]
class DiskInfo(C.Structure):
    _fields_=[('volume',C.c_ubyte*32)]+[(n,C.c_uint16) for n in ('total','users','free','user_free','exec','exec_free','block_size','icon')]+[('time',Time)]
class FileInfo(C.Structure):
    _fields_=[('size',U),('blocks',C.c_uint16),('type',C.c_uint8),('copy',C.c_uint8),('header_offset',C.c_uint16),('time',Time)]
class FiberInfo(C.Structure):
    _fields_=[('switches',C.c_uint64),('passes',C.c_uint64)]+[(n,U) for n in ('servers','fibers','status','os_error')]
def sha(p): return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','output'): ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args(); r=a.workspace.resolve(); build=a.build.resolve(); out=a.output.resolve()
    assert not out.exists() and out.is_relative_to(r/'work/sonic-adventure') and not out.is_relative_to(r/'work/sonic-adventure/build')
    b=json.loads((build/'build_verification.json').read_bytes()); assert sha(build/'jp_native_vmu_fs.dll')==b['DLL_sha256']
    for name,h in b['authored_source_sha256'].items(): assert sha(r/name)==h,name
    for name,h in b['generated_source_sha256'].items(): assert sha(build/name)==h,name
    out.mkdir(); media=out/'media'; media.mkdir(); dll=C.CDLL(str(build/'jp_native_vmu_fs.dll'))
    def bind(name,result,args):
        f=getattr(dll,name); f.restype=result; f.argtypes=args; return f
    store={}
    for n,result,args in [('initialize',I,[C.c_wchar_p]),('shutdown',I,[]),('create',I,[U]),('open',I,[U]),('close',I,[U]),('clear_error',None,[])]:
        store[n]=bind('jp_vmu_store_'+n,result,args)
    api={}
    for n,result,args in [
        ('buInit',I,[I,I,P,INIT]),('buExit',I,[]),('buExecServer',None,[]),('buDisableServer',None,[]),
        ('buSetCompleteCallback',None,[COMPLETE]),('buSetProgressCallback',None,[PROGRESS]),('buSetCallbackParam',None,[U]),
        ('buStat',I,[I]),('buGetLastError',I,[I]),('buIsReady',I,[I]),('buIsFormat',I,[I]),('buRemount',I,[I]),
        ('buFormatDisk',I,[I,P,I,C.POINTER(Time),U]),('buGetDiskInfo',I,[I,C.POINTER(DiskInfo)]),
        ('buSaveFile',I,[I,C.c_char_p,P,U,C.POINTER(Time),U]),('buSaveExecFile',I,[I,C.c_char_p,P,U,C.POINTER(Time),U]),
        ('buLoadFile',I,[I,C.c_char_p,P,U]),('buLoadFileEx',I,[I,C.c_char_p,P,U,U]),
        ('buGetFileSize',I,[I,C.c_char_p]),('buGetFileInfo',I,[I,C.c_char_p,C.POINTER(FileInfo)]),
        ('buFindFirstFile',I,[I,P]),('buFindNextFile',I,[I,P]),('buDeleteFile',I,[I,C.c_char_p]),
        ('buRenameFile',I,[I,C.c_char_p,C.c_char_p]),('buRewriteExecFile',I,[I,C.c_char_p,P,U,U]),
        ('buCreateFile',I,[I,C.c_char_p,C.POINTER(Time),C.POINTER(P)]),('buOpenFile',I,[I,C.c_char_p,C.POINTER(P)]),
        ('buWriteFile',I,[P,P,U]),('buReadFile',I,[P,P,U]),('buCloseFile',I,[P]),('buReadDiskImage',I,[I,P])]:
        api[n]=bind(n,result,args)
    checks=[]; events=[]; progress=[]; callback_threads=[]
    def check(ok,label):
        assert ok,label
        checks.append(label)
    def path(drive): return media/('VMU_'+chr(65+drive//2)+str(1+drive%2)+'.VMU')
    def complete(drive,op,status,param):
        events.append((drive,op,status,param)); callback_threads.append(threading.get_ident())
        # The real callback may query status. It must not recursively resume
        # the executing server or corrupt another drive's fiber context.
        api['buStat'](drive)
        return 0
    def report(drive,op,count,total):
        progress.append((drive,op,count,total)); callback_threads.append(threading.get_ident()); return 0
    done=COMPLETE(complete); report_cb=PROGRESS(report); init_calls=[]
    def initialized():
        init_calls.append(1); api['buDisableServer'](); api['buSetCompleteCallback'](done); api['buSetProgressCallback'](report_cb)
    init=INIT(initialized); no_init=INIT()
    def settle(drive):
        for ticks in range(10000):
            if api['buStat'](drive)==0: return ticks
        raise AssertionError('real command never completed')
    def command(drive,op,fn,*args,expected=0):
        fiber_before=FiberInfo(); fiber_after=FiberInfo()
        assert snapshot(C.byref(fiber_before))==0
        prior=len(events); check(fn(*args)==0,'operation '+str(op)+' queued on drive '+str(drive))
        check(len(events)==prior,'queued operation does not fake immediate completion')
        settle(drive); assert snapshot(C.byref(fiber_after))==0
        check(fiber_after.passes>fiber_before.passes,'queued command actually executes through the native fiber server')
        check(len(events)==prior+1 and events[-1][:3]==(drive,op,expected),'original completion reports actual operation/result: '+str(events[prior:]))
        check(api['buGetLastError'](drive)==expected,'original last-error matches command result')
    def disk(drive):
        info=DiskInfo(); check(api['buGetDiskInfo'](drive,C.byref(info))==0,'original disk-info succeeds'); return info
    def read_file(drive,name,payload):
        buf=C.create_string_buffer(b'\xa5'*(len(payload)+64)); assert C.addressof(buf)>0xffffffff
        command(drive,11,api['buLoadFile'],drive,name,C.byref(buf,32),len(payload)//512)
        check(buf.raw[32:32+len(payload)]==payload and buf.raw[:32]==buf.raw[-33:-1]==b'\xa5'*32,'original file load and both output canaries match independent input')
    def entries(drive):
        image=path(drive).read_bytes(); result={}; root=image[255*512:]; words=struct.unpack_from('<12H',root,64)
        check(root[:16]==b'\x55'*16 and words[:7]==(255,0,255,254,1,253,13),'serialized root has expected original card geometry')
        fat=struct.unpack_from('<256H',image,254*512)
        for block in range(253,240,-1):
            for offset in range(block*512,(block+1)*512,32):
                entry=image[offset:offset+32]
                if not entry[0]: continue
                name=entry[4:16].rstrip(b'\0'); first=struct.unpack_from('<H',entry,2)[0]; count=struct.unpack_from('<H',entry,24)[0]
                chain=[]; payload=bytearray(); current=first
                for _ in range(count):
                    check(0<=current<200 and current not in chain,'independent FAT walk stays in distinct user blocks')
                    chain.append(current); payload.extend(image[current*512:(current+1)*512]); current=fat[current]
                check(current==0xfffa,'independent FAT walk reaches exact end-of-file marker')
                result[name]=(bytes(payload),chain,entry)
        return result

    # Fresh images only; a separate record-format file must remain unchanged.
    snapshot=bind('jp_rf_snapshot',I,[C.POINTER(FiberInfo)])
    sentinel=media/'SONICADV_JP.SAV'; sentinel.write_bytes(b'PRESERVE INDEPENDENT RAW SAVE RECORDS'); sentinel_hash=sha(sentinel)
    check(store['initialize'](str(media))==0,'real card owner initialized')
    for drive in range(8):
        check(store['create'](drive)==0 and store['open'](drive)==0,'fresh exact-size card created and attached')
    work=C.create_string_buffer(b'\xa5'*(8*7712+64)); work_ptr=C.byref(work,32)
    check(api['buInit'](33,255,work_ptr,init)==-256 and not init_calls,'unsupported capacity rejected before callbacks')
    check(api['buInit'](15,255,work_ptr,init)==0 and init_calls==[1],'actual eight-drive SDK init callback')
    check(api['buInit'](15,255,work_ptr,init)==-1,'duplicate init rejected without destroying live fibers')
    api['buSetCallbackParam'](0xfedcba98); api['buExecServer']()
    for drive in range(8): settle(drive)
    check(len(events)==8 and all(x[1:]==(1,0,0xfedcba98) for x in events),'all eight real original mount callbacks delivered')
    check(all(api['buIsReady'](d)==1 and api['buIsFormat'](d)<0 for d in range(8)),'unformatted media mounts without silently formatting it')
    check(all(path(d).read_bytes()==bytes(131072) for d in range(8)),'mounting leaves every unformatted image byte unchanged')
    date=Time(2026,11,5,10,32,47,4); volume=C.create_string_buffer(b'CODEX VMU RESEARCH'.ljust(32,b'\0'),32)
    for drive in (0,7):
        command(drive,3,api['buFormatDisk'],drive,volume,3,C.byref(date),0)
        check(api['buIsFormat'](drive)==0,'original filesystem reports formatted card')
        info=disk(drive); check(info.users==200 and info.user_free==200 and info.exec==128 and info.block_size==512 and info.icon==3,'real disk-info reports standard user/exec capacity')
        check(bytes(info.volume)==volume.raw and bytes(info.time)==bytes(date),'original volume/time round-trip')
        check(entries(drive)=={},'independent parser finds no files after formatting')
        image=path(drive).read_bytes(); check(image[255*512+48:255*512+56]==bytes.fromhex('2026110510324703'),'independent packed BCD date and weekday oracle')
    rng=random.Random(0x564d5546); payload=rng.randbytes(10*512); buf=C.create_string_buffer(payload)
    assert C.addressof(buf)>0xffffffff
    prior=len(events); check(api['buSaveFile'](0,b'SONICADV_JP',buf,10,C.byref(date),0x80000000)==0,'original verified backup-file save queued')
    check(api['buExit']()==-1,'busy exit leaves filesystem alive')
    check(api['buSaveFile'](0,b'BUSY',buf,1,C.byref(date),0)==-1,'second command cannot replace pending write')
    check(settle(0)>0 and len(events)==prior+1 and events[-1][:3]==(0,12,0),'actual write, close and verify readback complete after rejected exit')
    check(progress and all(x[0] in range(8) for x in progress),'original progress callbacks delivered across real storage operations')
    parsed=entries(0); check(parsed[b'SONICADV_JP'][0]==payload and len(parsed)==1,'independent persisted directory/FAT/payload exactly matches ten-block input')
    check(disk(0).user_free==190,'normal file consumes actual ten user blocks')
    fi=FileInfo(); check(api['buGetFileInfo'](0,b'SONICADV_JP',C.byref(fi))==0 and fi.size==5120 and fi.blocks==10 and bytes(fi.time)==bytes(date),'original file-info byte count/block count/time survive native struct layout')
    read_file(0,b'SONICADV_JP',payload)
    raw_image=C.create_string_buffer(131072); command(0,15,api['buReadDiskImage'],0,raw_image)
    check(raw_image.raw==path(0).read_bytes(),'original whole-card read matches every persisted byte')
    command(0,12,api['buSaveFile'],0,b'SONICADV_JP',buf,3,C.byref(date),0x80000000)
    check(entries(0)[b'SONICADV_JP'][0]==payload[:1536] and disk(0).user_free==197,'overwriting an existing save releases its old FAT chain')
    command(0,12,api['buSaveFile'],0,b'SONICADV_JP',buf,10,C.byref(date),0x80000000)
    unchanged=path(0).read_bytes()
    command(0,12,api['buSaveFile'],0,b'TOO_BIG',buf,201,C.byref(date),0,expected=-252)
    check(path(0).read_bytes()==unchanged,'disk-full rejection does not alter persisted files')
    command(1,12,api['buSaveFile'],1,b'NO_FORMAT',buf,1,C.byref(date),0,expected=-253)
    check(path(1).read_bytes()==bytes(131072),'save to unformatted card cannot format or write silently')
    partial=C.create_string_buffer(2*512); command(0,14,api['buLoadFileEx'],0,b'SONICADV_JP',partial,3,2)
    check(partial.raw==payload[3*512:5*512],'original partial file load seeks through actual FAT chain')
    command(0,8,api['buRenameFile'],0,b'SONICADV_JP',b'RENAMED')
    check(api['buGetFileSize'](0,b'SONICADV_JP')<0 and api['buGetFileSize'](0,b'RENAMED')==10,'original rename changes real lookup result')
    name=C.create_string_buffer(16); check(api['buFindFirstFile'](0,name)>=0 and name.value==b'RENAMED' and api['buFindNextFile'](0,name)<0,'original directory iteration uses persisted name')
    # Handles themselves are 64-bit pointers and traverse the widened command
    # fields in write, close and read operations, not only data-buffer pointers.
    handle=P(); check(api['buCreateFile'](0,b'HANDLE',C.byref(date),C.byref(handle))==0 and handle.value>0xffffffff,'original native file handle created above 4GB')
    command(0,6,api['buWriteFile'],handle,buf,2)
    command(0,4,api['buCloseFile'],handle)
    check(api['buOpenFile'](0,b'HANDLE',C.byref(handle))==0,'original handle reopened')
    small=C.create_string_buffer(1024); command(0,5,api['buReadFile'],handle,small,2)
    check(small.raw==payload[:1024],'native handle read matches independent input')
    command(0,4,api['buCloseFile'],handle)
    executable=rng.randbytes(3*512); exe=C.create_string_buffer(executable)
    command(7,13,api['buSaveExecFile'],7,b'VMUGAME',exe,3,C.byref(date),0x80000000)
    parsed=entries(7); check(parsed[b'VMUGAME'][0]==executable and parsed[b'VMUGAME'][1]==[0,1,2] and parsed[b'VMUGAME'][2][0]==0xcc,'actual executable occupies consecutive blocks from zero')
    patch=C.create_string_buffer(rng.randbytes(512)); command(7,20,api['buRewriteExecFile'],7,b'VMUGAME',patch,1,1)
    executable=executable[:512]+patch.raw[:512]+executable[1024:]; read_file(7,b'VMUGAME',executable)
    # A real Windows reader denies replacement after the owner's staging/flush.
    # Original format fails on its first system-block write; no fake I/O hook.
    kernel=C.WinDLL('kernel32',use_last_error=True); create=kernel.CreateFileW; create.restype=P; create.argtypes=[C.c_wchar_p,U,U,P,U,U,P]
    close=kernel.CloseHandle; close.argtypes=[P]; close.restype=I
    before=path(0).read_bytes(); held=create(str(path(0)),0x80000000,1,None,3,0x80,None)
    check(held not in (None,P(-1).value),'actual target reader prevents atomic replacement')
    command(0,3,api['buFormatDisk'],0,volume,3,C.byref(date),0,expected=-127)
    check(path(0).read_bytes()==before and not list(media.glob('.vmu-*.tmp')),'failed real format write preserves complete previous on-disk card')
    check(close(held)!=0,'replacement-denial handle released'); store['clear_error']()
    command(0,1,api['buRemount'],0); read_file(0,b'RENAMED',payload)
    # Original vmsfs_fopen maps check_chain's missing-file error to FILE_BROKEN.
    # The size-query path (nblock=0) returns FILE_NOT_FOUND before opening.
    untouched=small.raw
    command(0,11,api['buLoadFile'],0,b'NO_SUCH_FILE',small,1,expected=-191)
    command(0,11,api['buLoadFile'],0,b'NO_SUCH_FILE',small,0,expected=-251)
    check(small.raw==untouched,'both original missing-file paths leave caller output unchanged')
    for name_to_delete in (b'RENAMED',b'HANDLE'):
        command(0,7,api['buDeleteFile'],0,name_to_delete)
    check(disk(0).user_free==200 and entries(0)=={},'delete frees actual FAT chains and directory entries')
    # End/reopen is a real lifecycle; a fresh work cache reads the persisted
    # executable from the card instead of relying on previous filesystem BSS.
    check(api['buExit']()==0,'idle exit releases all native fiber stacks')
    fiber_info=FiberInfo(); snapshot=bind('jp_rf_snapshot',I,[C.POINTER(FiberInfo)])
    check(snapshot(C.byref(fiber_info))==0 and fiber_info.servers==fiber_info.fibers==0,'no live fiber resources after filesystem exit')
    check(api['buInit'](15,255,work_ptr,init)==0,'filesystem reinitializes without stale server ownership')
    api['buSetCallbackParam'](0xfedcba98); api['buExecServer']()
    for drive in range(8): settle(drive)
    read_file(7,b'VMUGAME',executable)
    prior=len(events); check(store['close'](7)==0,'card detached through real block owner'); api['buExecServer']()
    check(len(events)==prior+1 and events[-1][:3]==(7,2,0) and api['buIsReady'](7)==0,'original removal callback and mount-state transition')
    check(store['open'](7)==0,'persisted card reopened'); api['buExecServer'](); settle(7); read_file(7,b'VMUGAME',executable)
    observed=[]
    t=threading.Thread(target=lambda: observed.append(api['buStat'](0))); t.start(); t.join()
    check(observed==[-1],'foreign thread cannot pump the game-thread filesystem')
    check(all(tid==threading.get_ident() for tid in callback_threads),'all original callbacks stay on the owning game thread')
    check(work.raw[:32]==work.raw[-33:-1]==b'\xa5'*32,'eight-drive original work allocation preserves both canaries')
    check(sha(sentinel)==sentinel_hash,'separate raw save-record file never altered')
    check(api['buExit']()==0 and store['shutdown']()==0,'filesystem and real disk owner cleanly shut down')
    result=dict(status='original_VMS_filesystem_real_card_mount_format_file_and_executable_IO_verified',checks=len(checks),check_labels=checks,build_verification_sha256=sha(build/'build_verification.json'),DLL_sha256=sha(build/'jp_native_vmu_fs.dll'),verifier_sha256=sha(Path(__file__)),test_root=str(media),original_complete_callbacks=len(events),original_progress_callbacks=len(progress),native_pointer_addresses_above_4GB=True,independent_serialized_FAT_directory_payload_checks=True,real_replacement_failure_verified=True,only_fresh_test_images_used=True,original_game_save_data_used=False,card_init_or_native_gameplay_verified=False,VMU_minigame_emulator_implemented=False,whole_project_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n'); print(result['status'],len(checks),flush=True)

if __name__=='__main__': main()
