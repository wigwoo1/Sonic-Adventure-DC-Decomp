"""Actual full-parent asset/character lifetime transfer and cleanup failure tests."""
import argparse,ctypes as C,faulthandler,hashlib,json,os,shutil,struct,subprocess,sys,threading
from pathlib import Path
from native_packet_test_support import bind
from native_asset_test_support import Assets,Limits
from native_pvm_test_support import PVM,destination
from native_texture_test_support import List
from verify_native_exit_logo import ExitInfo
from verify_native_history import Info as HistoryInfo
from verify_native_game_sound import Info as SoundInfo
from verify_native_read_queue import Info as QueueInfo
from verify_native_gdfs import Info as FSInfo
from verify_native_flash_manager import decode
I,U,Q,P,S=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p,C.c_size_t
ASSET=32;CHARACTER=33554432;MOTION=16777216;OWNED=3915778|4194304|8388608;FINAL=OWNED|64|128|512|1024
CASES=('asset_restart','character_restart','early','final_live','adopt','partial_asset','partial_character','independent_child','loader_busy','foreign','outer_guard','constructor','late_claim','motion_combo','card_callback','history_retry','history_global','process_success','process_busy')
class AssetInfo(C.Structure):
    _fields_=[(n,Q) for n in ('next_token','token','decoded_bytes','owned_bytes')]+[(n,U) for n in ('terminal','closing','finished','initialized','creator_thread','status','pending_refs','resident_refs')]
class CharacterInfo(C.Structure):
    _fields_=[(n,Q) for n in ('next_token','token')]+[(n,U) for n in ('terminal','closing','finished','active','creator_thread','status','users','partial')]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def exercise(c,case,inputs):
    d,b=c['d'],c['b'];f,system,boot,game,ex,hist,sound,queue,fs=(c[n] for n in ('f','system','boot','game','ex','hist','sound','queue','fs'))
    check,snapshot,si,info,shutdown,clean,keep=(c[n] for n in ('check','snapshot','si','info','shutdown','clean','keep'));assets=Assets(f);pvm=PVM(f)
    ast=lambda:snapshot(f,'jp_assets_exit_snapshot',AssetInfo);cht=lambda:snapshot(f,'jp_character_exit_snapshot',CharacterInfo);ei=lambda:snapshot(ex,'jp_exit_snapshot',ExitInfo)
    start=bind(system,'jp_system_start_assets',I,C.c_wchar_p,P);adopt=bind(system,'jp_system_adopt_assets',I);start_char=bind(system,'jp_system_start_character_textures',I);adopt_char=bind(system,'jp_system_adopt_character_textures',I)
    guard=bind(system,'jp_system_main_initialization_guard',I,U);delegate=bind(system,'jp_system_bind_exit',I,U);local=bind(system,'jp_system_shutdown',I);boot_close=bind(boot,'jp_main_boot_shutdown',I,U)
    acq=bind(f,'jp_character_textures_acquire',I,U);rel=bind(f,'jp_character_textures_release',I,U);char_init=bind(f,'jp_character_textures_initialize',I);char_close=bind(f,'jp_character_textures_shutdown',I)
    claim_a=bind(f,'jp_assets_exit_claim',I,P);claim_c=bind(f,'jp_character_exit_claim',I,P);cancel_a=bind(f,'jp_assets_exit_cancel',I,Q);cancel_c=bind(f,'jp_character_exit_cancel',I,Q)
    can_a=bind(f,'jp_assets_exit_can_retire',I,Q);after_c=bind(f,'jp_assets_exit_can_retire_after_character',I,Q,Q);can_c=bind(f,'jp_character_exit_can_retire',I,Q)
    check(C.sizeof(AssetInfo)==64 and C.sizeof(CharacterInfo)==48,'exact actual asset/character capability ABIs')
    if case=='constructor':check(c['events'] and all(e==(4,4,4,4,0,0) for e in c['events']),'actual constructor input callbacks reject all asset/character starts/adoptions before acquisition')
    root=Path(inputs)/'assets';limits=Limits(4<<20,8<<20,32<<20,0 if case=='partial_asset' else 512)
    if case=='partial_character':
        copy=d/'private_missing_second_file';shutil.copytree(root,copy);(copy/'SON_EFF.PRS').unlink();root=copy
    path_buffer=C.create_unicode_buffer(str(root));keep.append(path_buffer)
    if case=='outer_guard':check(guard(1)==0,'caller holds real preexisting System initialization guard')
    if case=='adopt':
        check(assets.init(str(root),C.byref(limits))==0,'independent actual asset provider initialized')
        raw=P();count=S();check(assets.read(b'SONIC.PRS',C.byref(raw),C.byref(count))==0,'independent actual decoded data view acquired')
        before=si().resources;check(start(path_buffer,C.byref(limits))==2 and adopt()==2 and si().resources==before,'start/unprotected adoption preserve independent asset provider and view')
        token=Q();check(claim_a(C.byref(token))==0 and guard(1)==0,'independent asset authority and outer guard established')
        check(adopt()==2 and ast().token==token.value and si().resources==before,'preclaimed actual asset owner rejects before parent resource mutation')
        check(cancel_a(token.value)==0 and adopt()==0 and guard(0)==0 and assets.release()==0,'same live decoded owner adopted under actual guard then caller releases its view')
    else:check(start(path_buffer,C.byref(limits))==0,'same System starts actual copied-root asset provider')
    a_token=ast().token;check(a_token and ast().initialized and si().resources==OWNED|ASSET,'single System registration retains exact real asset token and existing bit32')
    # Prove that root/limit buffers are copied by the REAL asset initializer.
    path_buffer.value='Z:\\ABSENT';limits.file_bytes=1;check(True,'caller asset options changed after native copy')
    has_char=case not in ('asset_restart','partial_asset','loader_busy')
    if case=='independent_child':
        check(char_init()==0 and acq(0)==0,'independent genuine character provider borrows System-owned assets')
        before=si().resources;check(start_char()==2 and local()==12 and si().failed_resource==ASSET and si().resources==before and cht().active and not cht().token,'actual independent child blocks parent asset retirement without being stolen')
        check(rel(0)==0 and char_close()==0,'independent caller retires its own real child');has_char=False
    elif has_char:
        if case=='adopt':
            check(char_init()==0 and acq(0)==0,'independent real character owner and live logical user established')
            before=si().resources;check(start_char()==2 and adopt_char()==2 and si().resources==before,'start/unprotected adoption preserve independent live character owner')
            raw=Q();check(claim_c(C.byref(raw))==0 and guard(1)==0,'independent character authority and outer guard established')
            check(adopt_char()==2 and cht().token==raw.value and si().resources==before,'preclaimed character rejects before resource mutation')
            check(cancel_c(raw.value)==0 and adopt_char()==0 and guard(0)==0,'actual live character provider adopted under same existing guard')
        else:check(start_char()==0,'same System starts and claims genuine character texture owner')
    c_token=cht().token if has_char else 0;mask=FINAL|ASSET|(CHARACTER if has_char else 0)
    if has_char:check(c_token and cht().active and si().resources==OWNED|ASSET|CHARACTER,'same System owns exact child capability and existing single provider')
    if case not in ('partial_asset','partial_character','asset_restart','loader_busy','adopt','independent_child'):
        check(acq(0)==0,'actual original Sonic file/list loops acquire live character textures')
    if case=='partial_asset':
        names,listing,buffers=destination(1);keep.extend([names,listing,buffers]);assets.original(b'SONIC',C.byref(listing))
        check(assets.status()==6 and ast().pending_refs>0 and not ast().decoded_bytes,'real insufficient resident table retains actual pending PVM refs for owner cleanup')
    if case=='partial_character':check(acq(0)==6 and cht().partial and not cht().users and ast().status==4,'real missing second file retains partial child textures before logical-user increment')
    raw=P();size=S();live_data=case in ('asset_restart','final_live','late_claim','process_busy','process_success')
    if live_data:check(assets.read(b'SONIC.PRS',C.byref(raw),C.byref(size))==0 and raw.value>0xffffffff and C.string_at(raw.value,4)==b'PVMH','actual full-width owned decoded retail view exposed')
    before_exit=ei().cleanup_calls;before_resources=si().resources;before_hash=hashlib.sha256(C.string_at(raw.value,size.value)).hexdigest() if live_data else None
    if case=='outer_guard':
        check(clean()==4 and ei().cleanup_calls==before_exit,'child start retains outer guard and global cleanup cannot release it');check(guard(0)==0,'caller releases actual outer guard')
    if case=='foreign':
        out_a=AssetInfo();out_c=CharacterInfo();C.memset(C.byref(out_a),0x5a,C.sizeof(out_a));C.memset(C.byref(out_c),0x5a,C.sizeof(out_c));old_a,old_c=bytes(out_a),bytes(out_c);values=[];state_a,state_c=bytes(ast()),bytes(cht())
        def worker():values.extend((start(str(root),C.byref(Limits(4<<20,8<<20,32<<20,512))),start_char(),adopt(),adopt_char(),local(),boot_close(5000),shutdown(5000),clean(),cancel_a(a_token),cancel_c(c_token),bind(f,'jp_assets_exit_snapshot',I,P)(C.byref(out_a)),bind(f,'jp_character_exit_snapshot',I,P)(C.byref(out_c)),assets.shutdown(),char_close()))
        t=threading.Thread(target=worker);t.start();t.join();check(values==[3]*14 and bytes(out_a)==old_a and bytes(out_c)==old_c and bytes(ast())==state_a and bytes(cht())==state_c,'foreign thread cannot mutate any actual owner or capability output')
    final=case in ('final_live','late_claim','process_success')
    if final:
        state_a,state_c=bytes(ast()),bytes(cht());check(delegate(FINAL|ASSET)==11 and si().failed_resource==CHARACTER and bytes(ast())==state_a and bytes(cht())==state_c,'missing explicit child bit rejects before final capability transfer')
        check(delegate(FINAL|CHARACTER)==11 and si().failed_resource==ASSET,'missing actual asset bit reports existing ASSETS ownership')
        if case=='late_claim':
            check(bind(f,'jp_queue_reserve',I,U,U)(0,4)==0 and bind(f,'njStartVertexNonTexOpaque',P)(),'real unfinished recorder conflicts with later device claim')
            check(delegate(mask)==11 and bytes(ast())==state_a and bytes(cht())==state_c and si().resources==before_resources,'late real claim failure preserves exact bootstrap child/asset authorities')
            check(bind(f,'jp_queue_discard_active',I)()==0 and bind(f,'jp_queue_reset_frame',I)()==0,'caller retires real conflicting recording')
        check(delegate(mask)==0 and not si().resources and si().phase==4 and ast().token==a_token and cht().token==c_token,'final handoff retains live users and decoded data with same exact capabilities')
    if live_data or (has_char and cht().users):
        live_child=bool(has_char and cht().users);bit=CHARACTER if live_child else ASSET;native=8 if live_child else 12
        check(local()==12 and boot_close(5000)==8 and shutdown(5000)==11,'all ordinary parents reject actual live decoded/character consumers')
        check(clean()==6 and ei().failed_resource==bit and ei().native_error==native and ei().cleanup_calls==before_exit,'global preflight rejects before irreversible bootstrap flags/counters')
        check(not ast().closing and not ast().terminal and (not has_char or not cht().closing) and snapshot(sound,'jp_sound_snapshot',SoundInfo).resources and snapshot(hist,'jp_history_snapshot',HistoryInfo).resources and snapshot(queue,'jp_read_queue_snapshot',QueueInfo).initialized and snapshot(fs,'jp_gdfs_snapshot',FSInfo).work,'blocked cleanup preserves exact real capabilities and every required parent')
        if live_data:check(ast().decoded_bytes==size.value and hashlib.sha256(C.string_at(raw.value,size.value)).hexdigest()==before_hash,'all decoded view bytes remain valid after failed parent cleanup')
        check(assets.shutdown()==2 and (not has_char or char_close()==2),'independent raw shutdown cannot bypass delegated authority')
    if case=='process_busy':
        observations=dict(profile=str(c['path']),before_profile_sha256=sha(c['path']));(d/'progress.json').write_text(json.dumps(dict(case=case,checks=len(c['labels']),assertions=c['labels'],observations=observations),indent=2)+'\n')
        bind(ex,'syBtExit',None)();raise AssertionError('real process exit returned')
    if has_char and cht().users:
        check(rel(0)==0 and not cht().users,'actual caller releases its genuine logical texture user')
        if live_data:check(clean()==6 and ei().failed_resource==ASSET and not ast().closing and not cht().closing,'remaining decoded view still blocks entire graph after texture user release')
    if live_data:check(assets.release()==0 and not ast().decoded_bytes,'actual caller releases owned decoded view before retirement')
    if has_char:check(can_c(c_token)==0 and can_a(a_token)==12 and after_c(a_token,c_token)==0,'asset preflight requires exact child retirement; ordered parent can preflight both without mutation')
    if case=='loader_busy':
        source=C.create_string_buffer(bytes(32));keep.append(source);check(pvm.begin(source,32)==0,'actual caller-owned raw memory source bound')
        check(clean()==6 and ei().failed_resource==ASSET and bind(f,'jp_asset_loader_activity',I)()==1 and not ast().closing,'borrowed memory source prevents any asset/parent retirement')
        check(pvm.end()==0,'caller unbinds its own actual source');names,listing,buffers=destination(1);keep.extend([names,listing,buffers])
        check(pvm.destination(C.byref(listing),1,28)==0 and clean()==6 and pvm.active()==1 and not ast().closing,'caller-owned PVM destination is retained rather than forcibly unbound')
        check(pvm.unbind()==0 and can_a(a_token)==0,'caller releases destination and exact asset may now retire')
    if case=='motion_combo':
        from verify_native_motion_store import View
        motions=C.CDLL(b['motion_DLL']);check(bind(system,'jp_system_start_motions',I,S,U)(8<<20,8)==0 and bind(motions,'jp_motions_select',I,I)(0)==0,'same System owns genuine motion dependency alongside characters/assets')
        view=View();check(bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)(0,b'sonic_updraft_action',C.byref(view))==0,'actual action user remains after texture users retire')
        check(clean()==6 and ei().failed_resource==MOTION and not ast().closing and not cht().closing,'later motion preflight blocks before earlier character/asset mutation')
        check(bind(motions,'jp_motions_release',I,Q)(view.token)==0,'caller retires actual final action view')
    card_events=[]
    if case=='card_callback':
        from verify_native_vmu_fs import Time
        def settle():
            for _ in range(10000):
                bind(f,'buExecServer',None)()
                if bind(f,'buGetLastError',I,I)(0)!=-1:return
            raise AssertionError('actual card work did not settle')
        settle();date=Time(2026,11,6,12,0,0,5);label=C.create_string_buffer(b'ASSET CHARACTER EXIT'.ljust(32,b'\0'),32);keep.extend([date,label])
        check(bind(f,'buFormatDisk',I,I,P,I,P,U)(0,label,0,C.byref(date),0)==0,'actual card format queued');settle()
        original=bind(f,'card_complete',I,I,I,I,U);Callback=C.CFUNCTYPE(I,I,I,I,U)
        @Callback
        def completed(drive,op,status,param):
            asset=AssetInfo();child=CharacterInfo();bind(f,'jp_assets_exit_snapshot',I,P)(C.byref(asset));bind(f,'jp_character_exit_snapshot',I,P)(C.byref(child));pointer=P(0xabcdef);length=S(0xabcdef)
            card_events.append((asset.initialized,asset.finished,child.active,child.finished,assets.read(b'SONIC.PRS',C.byref(pointer),C.byref(length)),acq(0),pointer.value,length.value,local(),clean(),boot_close(5000),bind(hist,'peer_history_play',I,U,U,U,P)(0,0,0,None),status))
            return original(drive,op,status,param)
        data=C.create_string_buffer(bytes((i*31+7)&255 for i in range(5120)));keep.extend([completed,data]);bind(f,'buSetCompleteCallback',None,Callback)(completed)
        check(bind(f,'buSaveFile',I,I,C.c_char_p,P,U,P,U)(0,b'ASSETEXIT',data,10,C.byref(date),0)==0 and bind(f,'buGetLastError',I,I)(0)==-1,'real save completion pending before ordered asset/character shutdown')
    if case.startswith('history_'):
        profile=c['path'];old_hash=sha(profile);kernel=C.WinDLL('kernel32',use_last_error=True);handle=bind(kernel,'CreateFileW',P,C.c_wchar_p,U,U,P,U,U,P)(str(profile),0x80000000,1,None,3,0x80,None)
        check(handle and handle!=P(-1).value,'actual Windows sharing condition prevents later History write')
        check(shutdown(5000)==11 and sha(profile)==old_hash and ast().finished and cht().finished and not ast().initialized and not cht().active and not ast().terminal and not cht().terminal,'later real History failure retains completed ordinary child capabilities without losing parent responsibility')
        check(start(str(root),C.byref(Limits(4<<20,8<<20,32<<20,512)))==2 and start_char()==2 and snapshot(hist,'jp_history_snapshot',HistoryInfo).resources and snapshot(sound,'jp_sound_snapshot',SoundInfo).resources and snapshot(fs,'jp_gdfs_snapshot',FSInfo).work,'partial cleanup cannot replace children and preserves remaining actual providers')
        check(bind(kernel,'CloseHandle',I,P)(handle)!=0 and bind(hist,'jp_history_recover',I)()==0,'actual failed save condition repaired for same-owner retry')
    if case=='process_success':
        observations=dict(profile=str(c['path']),before_profile_sha256=sha(c['path']));(d/'progress.json').write_text(json.dumps(dict(case=case,checks=len(c['labels']),assertions=c['labels'],observations=observations),indent=2)+'\n')
        bind(ex,'syBtExit',None)();raise AssertionError('real process exit returned')
    terminal=case in ('early','final_live','late_claim','history_global')
    if terminal:check(clean()==0,'actual global cleanup closes real child then assets before all remaining parents')
    check(shutdown(5000)==0 and not info().parent_owned and not info().scope_held,'actual higher constructor synchronizes completed ordered cleanup')
    check(not ast().initialized and not ast().owned_bytes and not ast().decoded_bytes and not ast().pending_refs and not cht().active and not cht().users,'actual asset allocations/decoded/pending and character references fully retired')
    if terminal:
        check(ast().token==a_token and ast().terminal and ast().finished and (not has_char or cht().token==c_token and cht().terminal and cht().finished),'global cleanup retains irreversible exact completed capabilities')
        check(cancel_a(a_token)==2 and (not has_char or cancel_c(c_token)==2) and assets.init(str(root),C.byref(Limits(4<<20,8<<20,32<<20,512)))==2 and clean()==0,'terminal state cannot cancel/reinitialize; repeated actual cleanup is idempotent')
    else:check(not ast().token and not ast().terminal and (not has_char or not cht().token and not cht().terminal),'complete ordinary cleanup cancels actual child capabilities for restart')
    if case in ('asset_restart','character_restart'):
        check(bind(f,'jp_queue_reset_frame',I)()==0,'borrowed native frame queue reset for real restart');c['configure']();check(c['initialize']()==0,'complete original game_init restarts')
        check(start(str(root),C.byref(Limits(4<<20,8<<20,32<<20,512)))==0 and ast().token>a_token and cancel_a(a_token)==2,'new asset lifetime rejects stale actual capability')
        if has_char:check(start_char()==0 and cht().token>c_token and cancel_c(c_token)==2 and acq(0)==0 and rel(0)==0,'new genuine character lifetime rejects stale capability and loads real files')
        check(shutdown(5000)==0,'restarted real parent and new child lifetimes retire cleanly')
    if case=='card_callback':check(card_events and all(e==(0,1,0,1,2,2,0xabcdef,0xabcdef,4,4,4,0,0) for e in card_events),'real card callbacks run after actual child/asset retirement; late loads/new users/reentrant cleanup reject while History remains usable')
    check(not snapshot(sound,'jp_sound_snapshot',SoundInfo).resources and not snapshot(hist,'jp_history_snapshot',HistoryInfo).resources and not snapshot(queue,'jp_read_queue_snapshot',QueueInfo).initialized and not snapshot(fs,'jp_gdfs_snapshot',FSInfo).work,'all actual audio/History/queue/filesystem providers retire')
    check(bind(f,'jp_lcd_shutdown',I)()==0 and bind(f,'jp_vmu_store_shutdown',I)()==0,'caller closes genuine borrowed LCD/VMU owners')
    if not final:
        check(bind(f,'jp_queue_reset_frame',I)()==0 and bind(f,'jp_frames_shutdown',I)()==0,'caller closes borrowed frame owner');bind(f,'jp_render_state_shutdown',None)();bind(f,'jp_textures_shutdown',None)();bind(f,'jp_queue_shutdown',None)()
    faulthandler.disable();c['fault'].close();out=dict(case=case,checks=len(c['labels']),assertions=c['labels'],observations=c['obs'],build_verification_sha256=sha(c['a'].build/'build_verification.json'),verifier_sha256=sha(__file__),full_set_character_animation_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
    (d/'result.json').write_text(json.dumps(out,indent=2)+'\n');print(case,len(c['labels']),'checks',flush=True)
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','inputs','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure';b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    if a.case:
        original=p/'jp_port_tools/verify_native_game_init.py';raw=original.read_bytes();marker=b"        if a.case=='normal':\n";assert raw.count(marker)==1;prefix=raw.split(marker)[0]
        anchor=b"('jp_textures_initialize',(32,1024,32<<20))";assert prefix.count(anchor)==1;prefix=prefix.replace(anchor,b"('jp_textures_initialize',(1024,4096,128<<20))")
        if a.case=='constructor':
            anchor=b'        if current.stage==3 and current.scope_held:\n';assert prefix.count(anchor)==1
            added=b'''            from native_asset_test_support import Limits
            from verify_native_asset_character_exit import AssetInfo,CharacterInfo
            asset=AssetInfo();child=CharacterInfo();bind(f,'jp_assets_exit_snapshot',I,P)(C.byref(asset));bind(f,'jp_character_exit_snapshot',I,P)(C.byref(child));limits=Limits(4<<20,8<<20,32<<20,512)
            events.append((bind(system,'jp_system_start_assets',I,C.c_wchar_p,P)('Z:\\\\ABSENT',C.byref(limits)),bind(system,'jp_system_adopt_assets',I)(),bind(system,'jp_system_start_character_textures',I)(),bind(system,'jp_system_adopt_character_textures',I)(),asset.initialized,child.active))
''';prefix=prefix.replace(anchor,anchor+added)
        caller=d/'private_actual_parent_caller.py';caller.write_bytes(prefix+("        from verify_native_asset_character_exit import exercise\n        exercise(locals(), "+repr(a.case)+", "+repr(str(a.inputs.resolve()))+")\nif __name__=='__main__':main()\n").encode())
        cmd=[sys.executable,'-B',str(caller),'--workspace',str(r),'--build',str(a.build.resolve()),'--track',str(a.track.resolve()),'--output',str(d/'run'),'--case','normal']
        run=subprocess.run(cmd,capture_output=True,env=dict(os.environ,PYTHONPATH=str(p/'jp_port_tools')),timeout=180,creationflags=subprocess.CREATE_NO_WINDOW);(d/'stdout.txt').write_bytes(run.stdout);(d/'stderr.txt').write_bytes(run.stderr)
        expected=70 if a.case=='process_busy' else 0;assert run.returncode==expected,(a.case,run.returncode,run.stderr.decode(errors='replace')[-6500:])
        v=read(d/'run'/('progress.json' if a.case.startswith('process_') else 'result.json'));v.update(actual_exit_code=run.returncode,original_parent_verifier_sha256=sha(original),derived_caller_sha256=sha(caller))
        if a.case.startswith('process_'):
            path=Path(v['observations']['profile'])
            if a.case=='process_busy':assert sha(path)==v['observations']['before_profile_sha256']
            else:_,packets=decode(path.read_bytes());title=b''.join(packets[24+i] for i in range(4));assert struct.unpack_from('<H',title,124)[0]>=1
            v['independent_post_process_profile_verified']=True
        (d/'result.json').write_text(json.dumps(v,indent=2)+'\n');print(a.case,v['checks'],'checks;',run.returncode,'exit',flush=True);return
    cases={}
    for name in CASES:
        cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
        for n in ('workspace','build','inputs','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
        cmd+=['--output',str(d/name),'--case',name];run=subprocess.run(cmd,capture_output=True,timeout=240,creationflags=subprocess.CREATE_NO_WINDOW);(d/(name+'.log')).write_bytes(run.stdout+run.stderr)
        assert not run.returncode,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-6500:]);cases[name]=read(d/name/'result.json');print(name,cases[name]['checks'],'checks',flush=True)
    out=dict(status='same_actual_asset_character_System_MainBoot_bootstrap_final_Exit_live_views_recovery_and_restart_verified',checks=sum(v['checks'] for v in cases.values()),cases=cases,
        build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),actual_process_exit_codes=[0,70],native_services_replaced=False,
        full_set_character_animation_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['checks'],'checks',flush=True)
if __name__=='__main__':main()
