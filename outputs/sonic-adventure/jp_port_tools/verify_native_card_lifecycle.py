"""Exercise unchanged game callbacks using real card I/O and frame_vsync.
Only new test cards are used. No game/SDK callbacks are replaced.
"""
import argparse,ctypes as C,hashlib,json,random,struct
from pathlib import Path
from native_packet_test_support import bind
from verify_native_backup_image import Header
from verify_native_vmu_fs import Time,DiskInfo,FiberInfo
I=C.c_int;U=C.c_uint32;P=C.c_void_p;Q=C.c_uint64
class Card(C.Structure):
    _fields_=[('mounted',U),('info_read',U),('error',I),('count',I),('maximum',I),('operation',U),('disk_info',C.c_ubyte*56),('saving',I),('notice',I),('notice_on',I),('drive',I),('initialized',U),('work',Q)]
def sha(p): return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','gpu','container','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();out=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not out.exists() and out.is_relative_to(r/'work/sonic-adventure') and not out.is_relative_to(r/'work/sonic-adventure/build')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(a.build/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'] and sha(b['present_DLL'])==b['present_DLL_sha256']
    out.mkdir();media=out/'media';media.mkdir();lib=C.CDLL(b['frames_DLL']);gpu=C.CDLL(str(a.gpu.resolve()));present=C.CDLL(b['present_DLL'])
    get=bind(lib,'jp_card_state_read',I,U,P); init=bind(lib,'card_init',None); finish=bind(lib,'card_finish',None);busy=bind(lib,'card_notice_busy',I)
    selected=I.in_dll(lib,'save_drive');slot=I.in_dll(lib,'save_slot');saving=I.in_dll(lib,'card_saving')
    snapshot=bind(lib,'jp_rf_snapshot',I,P);exit_fs=bind(lib,'buExit',I);last=bind(lib,'buGetLastError',I,I)
    store={n:bind(lib,'jp_vmu_store_'+n,ret,*args) for n,ret,args in [('initialize',I,[C.c_wchar_p]),('create',I,[U]),('open',I,[U]),('close',I,[U]),('shutdown',I,[]),('clear_error',None,[])]}
    vsync=bind(present,'frame_vsync',None);countdown=bind(present,'jp_wait_probe_countdown',None,I)
    events=bind(present,'jp_wait_probe_events',U);event=bind(present,'jp_wait_probe_event',U,U);clear=bind(present,'jp_wait_probe_clear_events',None)
    format_card=bind(lib,'buFormatDisk',I,I,P,I,P,U);remount=bind(lib,'buRemount',I,I)
    save=bind(lib,'buSaveFile',I,I,C.c_char_p,P,U,P,U);load=bind(lib,'buLoadFile',I,I,C.c_char_p,P,U)
    checks=[];frames=0;maximum_progress=0
    def check(ok,label): assert ok,label;checks.append(label)
    def state(drive=None):
        value=Card();assert get(selected.value if drive is None else drive,C.byref(value))==0;return value
    def tick():
        nonlocal frames,maximum_progress
        vsync();frames+=1;maximum_progress=max(maximum_progress,state().count)
    def wait_for_idle():
        # buGetLastError only observes busy; unlike buStat it never pumps.
        # All progress here must come from the original frame callback.
        for _ in range(10000):
            tick()
            if last(selected.value)!=-1:return last(selected.value)
        raise AssertionError('frame_vsync failed to deliver actual card work')
    def issue(fn,*args,expected=0):
        check(fn(*args)==0,'real SDK command queued'); result=wait_for_idle()
        check(result==expected and state().error==expected,'original game completion records actual SDK result '+str(result))
        check(state().operation==0,'original completion clears in-progress operation')
    def disk():return DiskInfo.from_buffer_copy(bytes(state().disk_info))
    def path(drive):return media/('VMU_'+chr(65+drive//2)+str(1+drive%2)+'.VMU')
    def recover(name):
        raw=path(selected.value).read_bytes();fat=struct.unpack_from('<256H',raw,254*512)
        for block in range(253,240,-1):
            for offset in range(block*512,(block+1)*512,32):
                e=raw[offset:offset+32]
                if e[0] and e[4:16].rstrip(b'\0')==name:
                    n=struct.unpack_from('<H',e,24)[0];at=struct.unpack_from('<H',e,2)[0];seen=set();payload=bytearray()
                    for _ in range(n):
                        check(at<200 and at not in seen,'independent chain has bounded distinct file blocks');seen.add(at);payload.extend(raw[at*512:(at+1)*512]);at=fat[at]
                    check(at==0xfffa,'independent chain has exact original file terminator');return bytes(payload)
        raise AssertionError('actual directory entry missing')
    # A void original card_init ignores buInit's failure result. Verify that it
    # cannot create an initialized backend or a mounted card without an owner.
    selected.value=0;init();check(state().initialized==0 and not state().mounted,'missing storage is never reported as a successful backend init')
    check(store['initialize'](str(media))==0,'real persistent card owner initialized')
    for drive in range(8):check(store['create'](drive)==0 and store['open'](drive)==0,'fresh test card attached')
    raw_sentinel=media/'SONICADV_JP.SAV';raw_sentinel.write_bytes(b'UNRELATED RAW SAVE FORMAT');old_sentinel=sha(raw_sentinel)
    date=Time(2026,11,5,11,23,45,4);volume=C.create_string_buffer(b'ORIGINAL GAME CARD TEST'.ljust(32,b'\0'),32)
    # A zero interval intentionally runs stage/audio work at every vsync.
    # Select interval one, with the act countdown kept above zero, to scope
    # this integration trace to card work and the existing loader observer.
    bind(present,'frame_set_interval',None,I)(1);countdown(1000000);clear()
    # Exercise the actual game-selected drive mask, not an eight-drive work
    # allocation substituted for card_library_work's original one-drive area.
    for drive in range(8):
        selected.value=drive;slot.value=2;saving.value=99;init();s=state()
        check(s.initialized==1 and s.drive==drive and s.work>0xffffffff and s.saving==0 and s.notice==0 and s.notice_on==1,'original selected-drive init/reset owns real native work area')
        check(bind(lib,'jp_card_work_size',U)()==7712 and bind(lib,'jp_card_selected_work_matches',I)()==1,'exact original one-card work area connected to actual filesystem')
        fi=FiberInfo();check(snapshot(C.byref(fi))==0 and fi.servers==fi.fibers==1,'one real native fiber for the game-selected drive')
        tick();check(wait_for_idle()==0,'original frame callback completes mount')
        check(state().mounted==1 and state().info_read==0 and state().error==0,'original mount callback distinguishes present blank card from formatted info')
        check(all(state(d).mounted==state(d).info_read==0 for d in range(8) if d!=drive),'other seven original card records remain cleared')
        check(path(drive).read_bytes()==bytes(131072),'original card init and callbacks never auto-format media')
        issue(format_card,drive,volume,4,C.byref(date),0)
        check(state().mounted==1 and disk().user_free==200 and bytes(disk().volume)==volume.raw,'original format completion refreshes actual disk info')
        # info_read is deliberately set only by the original mount branch.
        check(state().info_read==0,'format preserves original info_read semantics until remount')
        issue(remount,drive);check(state().info_read==1 and state().mounted==1,'original remount records valid disk info')
        check(slot.value==2 and busy()==0,'existing save-slot owner preserved and notice stays idle')
        if drive==0:
            c=json.loads((a.container/'build_verification.json').read_bytes());assert sha(c['DLL'])==c['DLL_sha256'];codec=C.CDLL(c['DLL'])
            bind(lib,'save_reset_all',None)();records=bytes((C.c_ubyte*(3*0x4a0)).in_dll(lib,'saves'));data=C.create_string_buffer(records+bytes(3968-len(records)))
            palette=C.create_string_buffer(bytes(32));icons=C.create_string_buffer(random.Random(0x43415244).randbytes(1024))
            header=Header();header.vms_comment=b'Sonic Adventure';header.btr_comment=b'Native card qualification';header.game_name[:]=b'SONICADV_JP\0\0\0\0\0'
            header.icon_palette=C.addressof(palette);header.icon_data=C.addressof(icons);header.icon_num=2;header.icon_speed=1;header.save_data=C.addressof(data);header.save_size=3968
            image=C.create_string_buffer(5120);check(bind(codec,'jp_backup_image_build',I,P,C.c_size_t,P)(image,len(image),C.byref(header))==10,'actual original default records packaged in original ten-block envelope')
            check(save(drive,b'SONICADV_SYS',image,10,C.byref(date),0x80000000)==0,'actual original-format save queued')
            # The original retry wrapper reports progress after a block call
            # returns; the native I/O first yields, so one frame is not enough.
            for _ in range(32):
                tick()
                if state().operation==12 and state().count>0:break
            check(state().operation==12 and state().count>0 and last(drive)==-1,'original card_progress observes pending real disk write')
            check(wait_for_idle()==0 and state().error==0 and state().operation==0 and disk().user_free==190,'original game completion sees verified ten-block save and actual free space')
            check(recover(b'SONICADV_SYS')==image.raw,'independent FAT/directory walk recovers every original backup byte')
            loaded=C.create_string_buffer(5120);issue(load,drive,b'SONICADV_SYS',loaded,10);check(loaded.raw==image.raw,'actual original-format save loads through real frame-driven card operations')
            parsed=Header();check(bind(codec,'jp_backup_image_analyze',I,P,P,C.c_size_t)(C.byref(parsed),loaded,len(loaded))==0,'loaded original backup container passes actual SDK CRC analysis')
            check(C.string_at(parsed.save_data,len(records))==records and bind(lib,'save_check_all',I,P)(parsed.save_data)==1,'all three actual original game save records survive complete virtual-card round trip')
            # Force a real atomic replacement failure; inspect the real game
            # callback record rather than installing a Python callback.
            kernel=C.WinDLL('kernel32',use_last_error=True);create=kernel.CreateFileW;create.restype=P;create.argtypes=[C.c_wchar_p,U,U,P,U,U,P];close=kernel.CloseHandle;close.restype=I;close.argtypes=[P]
            old=path(0).read_bytes();held=create(str(path(0)),0x80000000,1,None,3,0x80,None);check(held not in (None,P(-1).value),'real reader prevents replacement')
            issue(format_card,0,volume,4,C.byref(date),0,expected=-127)
            check(path(0).read_bytes()==old and state().mounted==1,'original callback reports actual failed write while previous disk survives')
            check(close(held)!=0,'denial reader released');store['clear_error']();issue(remount,0)
            check(state().error==0 and state().info_read==1,'original successful remount clears prior command error')
            check(save(0,b'SONICADV_SYS',image,10,C.byref(date),0x80000000)==0,'busy finish test queues real verified save')
            before=frames;finish();check(last(0)==0 and state().notice_on==0 and state().error==0,'original card_finish busy-wait pumps real SDK work and disables notice')
            check(frames==before,'busy finish completes using native buStat delivery without synthetic frame calls')
        check(store['close'](drive)==0,'actual selected card detached');tick();s=state()
        check(not s.mounted and not s.info_read and not s.count and not s.maximum and not s.operation and bytes(s.disk_info)==bytes(56),'original unmount callback clears complete typed card record')
        check(store['open'](drive)==0,'actual selected card reopened');tick();check(wait_for_idle()==0 and state().mounted==state().info_read==1,'real reinsertion remounts and original callback refreshes info')
        check(exit_fs()==0,'actual filesystem exits before switching selected drive')
    trace=[event(i) for i in range(events())]
    check(trace==[item for _ in range(frames) for item in (1,2)],'each actual frame_vsync calls real card service then existing loader observer')
    check(maximum_progress>10,'original card progress advances over actual mount/format/save work')
    check(sha(raw_sentinel)==old_sentinel,'unrelated raw save records preserved')
    check(store['shutdown']()==0,'real card owner released')
    result=dict(status='original_game_card_init_callbacks_finish_and_real_frame_vsync_storage_round_trip_verified',checks=len(checks),check_labels=checks,actual_frame_vsync_calls=frames,maximum_observed_original_card_progress=maximum_progress,all_eight_selected_drives_tested=True,original_card_work_bytes=7712,one_native_fiber_per_selected_drive=True,original_game_or_SDK_callbacks_replaced=False,original_default_records_and_backup_container_round_trip=True,independent_disk_FAT_directory_payload_oracle=True,actual_failed_write_reaches_original_game_error_record=True,single_save_slot_owner_preserved=True,user_saves_used=False,full_gameplay_or_Chao_emulation_verified=False,build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(checks),'checks',flush=True)
if __name__=='__main__':main()
