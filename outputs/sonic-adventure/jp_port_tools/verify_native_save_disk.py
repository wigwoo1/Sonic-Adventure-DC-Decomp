"""Real Windows file persistence around the qualified original save state.

Uses actual game exports and actual filesystem operations, including a fresh
process and a real Windows sharing violation. No mocked I/O or game callbacks.
"""
import argparse,ctypes as C,hashlib,json,subprocess,sys,threading
from pathlib import Path
from native_packet_test_support import bind

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
class Info(C.Structure):
    _fields_=[('reads',C.c_uint64),('writes',C.c_uint64),('applies',C.c_uint64),('status',C.c_uint32),('os_error',C.c_uint32),('record_bytes',C.c_uint32),('record_count',C.c_uint32)]

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','output']:ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--worker',action='store_true');a=ap.parse_args()
    r=a.workspace.resolve();d=a.build.resolve();out=a.output.resolve();b=read(d/'build_verification.json')
    assert sha(b['DLL'])==b['DLL_sha256'] and sha(b['frame_DLL'])==b['frame_DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    game=C.CDLL(b['frame_DLL']);disk=C.CDLL(b['DLL']);I=C.c_int;U=C.c_uint32;P=C.c_void_p
    init=bind(disk,'jp_save_disk_initialize',I,C.c_wchar_p);shutdown=bind(disk,'jp_save_disk_shutdown',I)
    status=bind(disk,'jp_save_disk_status',I);clear=bind(disk,'jp_save_disk_clear_error',None)
    snapshot=bind(disk,'jp_save_disk_snapshot',I,P);write=bind(disk,'jp_save_disk_write',I);load=bind(disk,'jp_save_disk_read',I);apply=bind(disk,'jp_save_disk_apply',I,U)
    reset=bind(game,'save_reset_all',None);store=bind(game,'save_store',None);crc=bind(game,'jp_save_checksum',U,P)
    sizes={'saves':3552,'event_flags':512,'events_played':512,'story_records':96,'dat_8c7a2718':8,'stage_clears':344,'lives':1,'play_time':4,'stage_last_cleared':4,'save_slot':4,'character_current':2}
    arrays={n:(C.c_ubyte*s).in_dll(game,n) for n,s in sizes.items()}
    def data(n):return bytes(arrays[n])
    def setdata(n,v):assert len(v)==sizes[n];C.memmove(arrays[n],v,len(v))
    def scalar(n,v):setdata(n,v.to_bytes(sizes[n],'little',signed=True))
    def integer(n):return int.from_bytes(data(n),'little',signed=True)
    checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def info():
        v=Info();check(snapshot(C.byref(v))==0,'snapshot');return v
    root=out/'保存データ';target=root/'SONICADV_JP.SAV'
    if a.worker:
        check(init(str(root))==0,'fresh process initialize');check(load()==0,'fresh process read');check(apply(2)==0,'fresh process genuine save_load')
        report={n:integer(n) for n in ['save_slot','character_current','play_time','lives','stage_last_cleared']}
        report.update(file_sha256=sha(target),snapshots_sha256=hashlib.sha256(data('saves')).hexdigest(),event_flags_sha256=hashlib.sha256(data('event_flags')).hexdigest())
        check(shutdown()==0,'fresh process shutdown');(out/'fresh_process.json').write_text(json.dumps(report,indent=2)+'\n');return
    assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();root.mkdir(parents=True)
    check(status()==2 and write()==2 and load()==2 and apply(0)==2,'uninitialized APIs reject')
    check(shutdown()==0 and shutdown()==0,'idempotent shutdown')
    for value in [None,'',str(root/'missing')]:check(init(value)==1,'invalid root rejected')
    check(init(str(root))==0,'Unicode existing directory accepted');check(init(str(root))==3,'duplicate initialization rejected')
    v=info();check((v.reads,v.writes,v.applies,v.record_bytes,v.record_count)==(0,0,0,1184,3),'initial counters and exact format')
    reset();check(load()==5 and status()==5,'missing file sticky IO error');v=info();check(v.os_error in [2,3],'real file-not-found error');clear()
    # Store each slot using real original packers and genuine shared progress.
    for slot in range(3):
        scalar('save_slot',slot);scalar('character_current',[0,3,7][slot]);scalar('play_time',123456+slot);scalar('lives',11+slot);scalar('stage_last_cleared',20+slot)
        for n in ['event_flags','events_played']:setdata(n,bytes((i+slot)%2 for i in range(512)))
        for n in ['story_records','dat_8c7a2718','stage_clears']:setdata(n,bytes((i+slot)%256 for i in range(sizes[n])))
        store()
    saved=data('saves');check(write()==0,'real flushed atomic write');check(target.read_bytes()==saved,'disk bytes exactly three original records')
    check(not list(root.glob('.sa-native-save-*')),'no temporary left after success')
    # Read must not secretly apply a slot or overwrite current game progress.
    for n in arrays:
        if n!='saves':setdata(n,bytes([0x55])*sizes[n])
    before={n:data(n) for n in arrays if n!='saves'};setdata('saves',bytes([0xa5])*3552)
    check(load()==0 and data('saves')==saved,'read restores exact serialized snapshots')
    check(all(data(n)==v for n,v in before.items()),'read leaves active progress and slot untouched')
    for slot in range(3):
        unknown=data('story_records')[1::12];check(apply(slot)==0,'genuine save_load selected slot')
        check((integer('save_slot'),integer('character_current'),integer('play_time'),integer('lives'),integer('stage_last_cleared'))==(slot,[0,3,7][slot],123456+slot,11+slot,20+slot),'applied original scalar fields')
        check(data('event_flags')==data('events_played')==bytes((i+slot)%2 for i in range(512)),'genuine unpacked progress flags')
        check(data('story_records')[1::12]==unknown,'runtime unknown story bytes preserved by original load')
        check(data('saves')==saved,'apply does not rewrite snapshots')
    # Recompute a valid CRC on a deliberately out-of-range character; CRC alone
    # cannot protect the original load's character-indexed lives lookup.
    bad_character=bytearray(saved);bad_character[0x258]=8
    record=C.create_string_buffer(bytes(bad_character[:1184]));bad_character[:4]=int(crc(record)).to_bytes(4,'little')
    corrupted=bytearray(saved);corrupted[111]^=1
    for name,payload in [('short',saved[:-1]),('long',saved+b'X'),('empty',b''),('CRC',bytes(corrupted)),('character',bytes(bad_character))]:
        target.write_bytes(payload);before={n:data(n) for n in arrays};check(load()==6 and status()==6,'reject '+name)
        check(all(data(n)==v for n,v in before.items()),'failed read preserves all live state '+name)
        check(write()==6 and target.read_bytes()==payload,'sticky failure prevents subsequent write '+name);clear()
    target.write_bytes(saved)
    for slot in [3,0xffffffff]:
        before={n:data(n) for n in arrays};check(apply(slot)==1,'invalid apply slot');check(all(data(n)==v for n,v in before.items()),'invalid slot preserves live state');clear()
    for payload in [bytes(corrupted),bytes(bad_character)]:
        setdata('saves',payload);check(write()==6,'invalid current save rejected before disk write');check(data('saves')==payload and target.read_bytes()==saved,'invalid write preserves memory and prior file');clear()
        check(apply(0)==6,'invalid current save rejected before game load');clear()
    setdata('saves',saved)
    # OS-level lock forbids replacement while allowing this test to inspect
    # the prior file. This tests the actual MoveFileEx failure and cleanup path.
    kernel=C.WinDLL('kernel32',use_last_error=True)
    create=bind(kernel,'CreateFileW',P,C.c_wchar_p,U,U,P,U,U,P);close=bind(kernel,'CloseHandle',I,P)
    handle=create(str(target),0x80000000,1,None,3,0x80,None);check(handle!=P(-1).value,'real replacement lock opened')
    try:
        check(write()==5,'open file lock rejects replacement');v=info();check(v.os_error in [5,32],('real Windows replacement denial reported',v.os_error))
        check(target.read_bytes()==saved,'failed atomic replacement preserves previous file');check(not list(root.glob('.sa-native-save-*')),'failed replacement removes own temporary')
    finally:check(close(handle)!=0,'lock closed')
    clear();check(write()==0,'retry succeeds after releasing OS lock')
    baseline=bytes(info());foreign=[]
    def thread():foreign.extend([init(str(root)),status(),write(),load(),apply(0),shutdown(),snapshot(C.byref(Info()))]);clear()
    t=threading.Thread(target=thread);t.start();t.join();check(foreign==[4]*7,'foreign thread denied across all stateful APIs');check(bytes(info())==baseline,'foreign thread cannot mutate owner state')
    v=info();check((v.reads,v.writes,v.applies)==(1,2,3),'successful operation counters exclude failures')
    check(shutdown()==0 and status()==2,'owner shutdown');check(init(str(root))==0 and load()==0,'same process reopen');check(shutdown()==0,'reopened service shutdown')
    cmd=[sys.executable,'-B',str(Path(__file__).resolve()),'--workspace',str(r),'--build',str(d),'--output',str(out),'--worker']
    run=subprocess.run(cmd,capture_output=True,creationflags=subprocess.CREATE_NO_WINDOW);(out/'worker_stdout.txt').write_bytes(run.stdout);(out/'worker_stderr.txt').write_bytes(run.stderr)
    check(run.returncode==0 and not run.stderr,('fresh process',run.returncode,run.stderr.decode()[-2000:]));worker=read(out/'fresh_process.json')
    check([worker[n] for n in ['save_slot','character_current','play_time','lives','stage_last_cleared']]==[2,7,123458,13,22],'fresh process restored actual original progress')
    check(worker['file_sha256']==worker['snapshots_sha256']==hashlib.sha256(saved).hexdigest(),'fresh process read exact durable bytes')
    check(worker['event_flags_sha256']==hashlib.sha256(bytes(i%2 for i in range(512))).hexdigest(),'fresh process genuine flag unpacker')
    check(not list(root.glob('.sa-native-save-*')),'all tested operations leave no temporary')
    proof=read(Path(b['frame_DLL']).parents[1]/'save-state-verification.json');check(proof['checks']==1514 and not proof['original_dependency_hooks_used'],'shared game provider independently verified against original machine code')
    report=dict(status='real_native_disk_save_read_apply_and_cross_process_persistence_verified',checks=checks,build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),original_state_proof_sha256=sha(Path(b['frame_DLL']).parents[1]/'save-state-verification.json'),fresh_process_report_sha256=sha(out/'fresh_process.json'),file_sha256=sha(target),serialized_record_bytes=1184,slots=3,real_Windows_IO=True,Unicode_path_verified=True,atomic_replace_failure_preserves_previous_file=True,no_orphan_temporary_files=True,malformed_inputs_preserve_live_state=True,original_CRC_and_character_bounds_checked=True,foreign_thread_rejected=True,read_does_not_apply_slot=True,actual_original_save_load_used=True,game_or_IO_callbacks_mocked=False,complete_scene_semantic_validation=False,VMU_menu_or_game_main_loop_integrated=False,playable=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
