"""Real Japanese AFS partitions, original file reads and actual native ownership/retirement."""
import argparse,ctypes as C,hashlib,json,struct,subprocess,sys,threading,time
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
from adx_media_fixtures import Disc
from rna_test_runtime import Runtime
from verify_native_adxstm import Ring,Info as StmInfo
from verify_native_gdfs import Info as FSInfo
from verify_native_audio import bind
I,U,Q,P=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p
CASES=('archive','overlay','pool','reads','pending','timed')
class Info(C.Structure):
    _fields_=[(n,Q) for n in ('initializations','shutdowns','service_calls','owned_bytes')]+[(n,U) for n in
        ('active','stopping','live_files','pending_reads','partition_roots','owned_nodes','status')]+[('native_error',I)]
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--case',choices=CASES);a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    b=read(a.build/'build_verification.json');assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256']
    assert sha(a.build/'external_adx_fs.c')==b['adxf_generated_source_sha256']
    assert sha(a.build/'sound_owner_adxf.c')==b['adxf_owner_generated_source_sha256'];d.mkdir()
    if not a.case:
        cases={}
        for name in CASES:
            cmd=[sys.executable,'-B',str(Path(__file__).resolve())]
            for n in ('workspace','build','track'):cmd+=['--'+n,str(getattr(a,n).resolve())]
            cmd+=['--case',name,'--output',str(d/name)]
            run=subprocess.run(cmd,capture_output=True,timeout=120,creationflags=subprocess.CREATE_NO_WINDOW);(d/(name+'.log')).write_bytes(run.stdout+run.stderr)
            assert not run.returncode,(name,run.returncode,(run.stdout+run.stderr).decode(errors='replace')[-6000:])
            cases[name]=read(d/name/'result.json');print(name,cases[name]['checks'],'checks',flush=True)
        result=dict(status='original_ADXF_real_Japanese_AFS_partitions_ranges_clip_bytes_owned_storage_and_System_cleanup_verified',
            runtime_checks=sum(v['checks'] for v in cases.values()),fresh_process_cases=len(cases),cases=cases,
            build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
            fixture_source_sha256=sha(p/'jp_port_tools/rna_test_runtime.py'),ring_fixture_source_sha256=sha(p/'jp_port_tools/verify_native_adxstm.py'),
            disc_fixture_source_sha256=sha(p/'jp_port_tools/adx_media_fixtures.py'),
            full_ADXT_voice_playback_looping_gameplay_verified=False,retail_data_published=False,playable=False,overall_completion_percent=10)
        (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['runtime_checks'],'checks',flush=True);return
    labels=[];rt=None;rings=[];obs={};errors=[]
    def check(ok,label):assert ok,(a.case,label);labels.append(label)
    disc=Disc(a.track.resolve())
    try:
        afs_fad,afs_size=disc.files['EVENT_ADX.AFS'];head=disc.read('EVENT_ADX.AFS',0,8);count=struct.unpack_from('<I',head,4)[0]
        table=disc.read('EVENT_ADX.AFS',8,count*8);entries=[struct.unpack_from('<II',table,i*8) for i in range(count)]
        check(head[:4]==b'AFS\0' and count==2088,'independent actual Japanese voice archive header/table')
        selected={i:disc.sectors(afs_fad+entries[i][0]//2048,(entries[i][1]+2047)//2048) for i in (0,1,2,2087)}
        title_size=disc.files['TITLE.ADX'][1];title_first=disc.read('TITLE.ADX',0,8192)
    finally:disc.close()
    try:
        rt=Runtime(b,a.track,check,timed=a.case in ('pending','timed'));lib=rt.lib;fs=C.CDLL(b['GDFS_DLL'])
        check(C.sizeof(Info)==64 and bind(lib,'jp_adxstm_initialize',I)()==0,'existing native lower file owner')
        check(bind(fs,'gdFsChangeDir',I,C.c_char_p)(b'/SONICADV')==0,'actual retail file directory')
        preexisting=None;carry_ring=None
        if a.case=='pool':
            carry_ring=Ring(rt);rings.append(carry_ring)
            preexisting=bind(lib,'ADXSTM_OpenFname',P,C.c_char_p,P)(b'TITLE.ADX',carry_ring.handle)
            check(bool(preexisting),'actual unrelated lower file stream exists before archive initialization')
        check(bind(lib,'jp_adxf_initialize',I)()==0,'original ADXF joins same sound/System owner')
        bind(lib,'ADXERR_Init',None)();ERROR=C.CFUNCTYPE(None,P,P)
        @ERROR
        def error(ctx,msg):errors.append((ctx,C.string_at(msg).decode(errors='replace')))
        rt.keep.append(error);bind(lib,'ADXERR_EntryErrFunc',None,P,P)(error,0x123456789)
        def info():
            out=Info();check(bind(lib,'jp_adxf_snapshot',I,P)(C.byref(out))==0,'actual original ADXF records/tables and owned storage snapshot');return out
        def stm():
            out=StmInfo();check(bind(lib,'jp_adxstm_snapshot',I,P)(C.byref(out))==0,'actual original lower stream records');return out
        def fi():
            out=FSInfo();check(bind(fs,'jp_gdfs_snapshot',I,P)(C.byref(out))==0,'actual native filesystem handles/requests');return out
        def pt(id):
            out=(Q*9)();check(bind(lib,'peer_adxf_partition',I,U,P)(id,out)==0,'actual native partition pointer/header readback');return list(out)
        def state(h):
            out=(Q*12)();check(bind(lib,'peer_adxf_state',I,P,P)(h,out)==0,'actual original ADXF file record');return list(out)
        def service():check(bind(lib,'jp_adxf_service',I)()==0,'existing shared sound service advances original ADXF/ADXSTM/GDFS')
        def load(id=0,n=count):check(bind(lib,'jp_adxf_partition_load',I,U,C.c_char_p,U)(id,b'EVENT_ADX.AFS',n)==0,'real original archive loader uses guarded native owned work')
        def getrange(id,i):
            fad=I(-99);size=I(-99);ret=bind(lib,'ADXF_GetFileRange',I,I,I,P,P)(id,i,C.byref(fad),C.byref(size));return ret,fad.value,size.value
        def openafs(i):
            h=bind(lib,'ADXF_OpenAfs',P,I,I)(0,i);check(bool(h) and h>0xffffffff,'original archive file opens with native pointer');return h
        def wait(h):
            deadline=time.monotonic()+10
            while bind(lib,'ADXF_GetStat',I,P)(h)==2:
                if a.case=='timed':
                    window=bind(rt.device,'jp_device_window',P)();check(bind(rt.present,'jp_wait_window_idle',I,P,U)(window,5)==1,'actual shared elapsed-time wait automatically services archive read')
                else:service()
                check(time.monotonic()<deadline,'actual archive request bounded progress');time.sleep(.0005)
            check(bind(lib,'ADXF_GetStat',I,P)(h)==3,'original archive read reaches READEND')
        check(bind(lib,'jp_adxstm_shutdown',I)()==2 and stm().active,'lower owner cannot retire while native archive parent is active')
        if a.case=='archive':
            load();root=pt(0);snapshot=info()
            check(root[0]>0xffffffff and root[1:3]==[24,8] and root[3]==0 and root[6]==root[7]==count,'native partition has complete pointer/link/header widths and all2088 entries')
            check(snapshot.owned_nodes==snapshot.partition_roots==1 and snapshot.owned_bytes==root[4] and not snapshot.live_files and not stm().live_streams and not fi().live_handles,
                'real archive loading leaves only its owned metadata and no temporary file/joint/read')
            for i,(off,size) in enumerate(entries):check(getrange(0,i)==(0,afs_fad+off//2048,(size+2047)//2048),'every Japanese AFS file range matches independent original archive table')
            for id,i in [(-1,0),(256,0),(255,0),(0,-1),(0,count),(0,2147483647)]:check(getrange(id,i)==(-3,-1,-1),'negative/unloaded/out-of-range archive query rejects without unsafe table access')
            for fname in (b'NOT_HERE.AFS',b'TITLE.ADX'):
                check(bind(lib,'jp_adxf_partition_load',I,U,C.c_char_p,U)(0,fname,count)==8 and pt(0)[0]==root[0],
                    'failed/malformed actual source load preserves previous authoritative partition')
                check(info().owned_nodes==1 and getrange(0,0)==(0,afs_fad+entries[0][0]//2048,(entries[0][1]+2047)//2048),'failed load frees temporary work and leaves original ranges usable')
            check(bind(lib,'jp_adxf_partition_load',I,U,C.c_char_p,U)(256,b'EVENT_ADX.AFS',count)==1 and bind(lib,'jp_adxf_partition_load',I,U,C.c_char_p,U)(0,b'EVENT_ADX.AFS',0)==1,'invalid native partition capacity/ID rejected before mutation')
            load();check(info().owned_nodes==1 and pt(0)[0]!=root[0],'replacement real load releases prior owned node after successful install')
            check(bind(lib,'jp_adxf_partition_unload',I,U)(0)==0 and info().owned_nodes==0 and info().partition_roots==0 and info().owned_bytes==0,'owned partition unload detaches actual table and releases all guarded storage')
            obs.update(real_voice_archive_entries=count,every_independent_range_verified=True,partition_native_header_bytes=24,transactional_failure_and_replacement=True)
        elif a.case=='overlay':
            load();base=pt(0)
            check(bind(lib,'jp_adxf_partition_add',I,U,C.c_char_p,U)(0,b'EVENT_ADX.AFS',count)==0,'actual original added archive loads its explicit entry table')
            added=pt(0);check(added[3]==base[0] and added[6]==added[7]==count and added[8]==1 and info().owned_nodes==2,'actual native next pointer links added table to original loaded table')
            for i,(off,size) in enumerate(entries):check(getrange(0,i)==(0,afs_fad+off//2048,(size+2047)//2048),'every added-archive range matches actual explicit offsets and sizes')
            # Controlled sparse-overlay input changes actual table entries only;
            # no filesystem/decoder/provider is hooked or replaced.
            entry_at=added[0]+24;saved=C.string_at(entry_at,count*8)
            C.memmove(entry_at,struct.pack('<HHi',0,1,333),8);C.c_uint16.from_address(added[0]+20).value=1
            check(getrange(0,0)==(0,afs_fad+333,1) and getrange(0,1)==(0,afs_fad+entries[1][0]//2048,(entries[1][1]+2047)//2048),'controlled actual sparse overlay uses override first and original loaded-table fallback')
            C.memmove(entry_at,saved,len(saved));C.c_uint16.from_address(added[0]+20).value=count
            check(bind(lib,'jp_adxf_partition_add',I,U,C.c_char_p,U)(0,b'TITLE.ADX',count)==8 and pt(0)[0]==added[0] and info().owned_nodes==2,'failed added archive preserves actual chain and allocation count')
            check(bind(lib,'jp_adxf_partition_unload',I,U)(0)==0 and info().owned_nodes==0,'all owned layers retire without freeing borrowed reader buffers')
            obs.update(real_added_archive_entries=count,controlled_sparse_actual_table_fixture=True,native_link_chain_verified=True)
        elif a.case=='pool':
            check(stm().live_streams==1 and fi().live_handles==1,'archive initialization preserves existing live lower stream and file handle')
            handles=[bind(lib,'ADXF_Open',P,C.c_char_p,P)(b'TITLE.ADX',None) for _ in range(16)]
            check(all(handles) and info().live_files==16 and not bind(lib,'ADXF_Open',P,C.c_char_p,P)(b'TITLE.ADX',None),'all16 real original file slots allocate metadata without a shadow pool')
            check(bind(lib,'jp_adxf_partition_load',I,U,C.c_char_p,U)(0,b'EVENT_ADX.AFS',count)==8 and not info().owned_nodes,'exhausted original file pool rolls back guarded partition allocation')
            foreign=[];thread=threading.Thread(target=lambda:foreign.extend([bind(lib,'jp_adxf_service',I)(),bind(lib,'jp_adxf_shutdown',I)()]));thread.start();thread.join(5)
            check(not thread.is_alive() and foreign==[3,3] and info().live_files==16,'foreign native caller cannot mutate or retire real archive file pool')
            bind(lib,'ADXF_Close',None,P)(handles[7]);replacement=bind(lib,'ADXF_Open',P,C.c_char_p,P)(b'TITLE.ADX',None)
            check(replacement==handles[7] and state(replacement)[11]==56,'actual file slot reuses full native56-byte record')
            for h in handles:bind(lib,'ADXF_Close',None,P)(h)
            check(info().live_files==0 and stm().live_streams==1 and fi().live_handles==1,'archive metadata retirement preserves unrelated actual lower file stream')
            check(bind(lib,'jp_adxf_shutdown',I)()==0 and stm().live_streams==1,'archive shutdown preserves borrowed lower owner and unrelated real file')
            bind(lib,'ADXSTM_Close',None,P)(preexisting);carry_ring.retired()
            obs.update(original_ADXF_pool_slots=16,native_record_bytes=56,actual_exhaustion_rollback_and_slot_reuse=True,live_unrelated_lower_stream_preserved_across_init_and_shutdown=True)
        else:
            load();i=0 if a.case in ('pending','timed') else None
            if a.case in ('reads','timed'):
                rows=[]
                for i in ((0,1,2,2087) if a.case=='reads' else (0,)):
                    h=openafs(i);expected=selected[i];raw=C.create_string_buffer(len(expected)+128);C.memset(raw,0xa5,len(raw));at=(C.addressof(raw)+63)&~31
                    if a.case=='reads' and i==2087:
                        check(bind(lib,'jp_adxf_partition_unload',I,U)(0)==0 and not info().owned_nodes and getrange(0,0)==(-3,-1,-1),
                            'actual partition storage can retire after file metadata is copied without leaving a live table pointer')
                    check(bind(lib,'ADXF_ReadNw32',I,P,I,P)(h,len(expected)//2048+7,at)==len(expected)//2048,'real original archive buffer read clamps before buffer-size multiplication')
                    wait(h);check(C.string_at(at,len(expected))==expected,'every actual selected voice-clip byte including sector padding matches independent disc input')
                    check(C.string_at(at-16,16)==b'\xa5'*16 and C.string_at(at+len(expected),16)==b'\xa5'*16,'whole actual voice-read destination guards')
                    check(bind(lib,'ADXF_GetNumReadSct',I,P)(h)==len(expected)//2048 and bind(lib,'ADXF_Tell',I,P)(h)==len(expected)//2048,'real read counters and file cursor advance exactly by actual sectors')
                    check(bind(lib,'ADXF_ReadNw32',I,P,I,P)(h,1,at)==0 and not stm().live_streams,'EOF does not create a zero-sized stream or retry indefinitely')
                    check(bind(lib,'ADXF_Seek',I,P,I,I)(h,0,0)==0,'original archive file rewind')
                    check(bind(lib,'ADXF_ReadNw32',I,P,I,P)(h,1,at)==1,'rewound original request starts again');wait(h)
                    check(C.string_at(at,2048)==expected[:2048],'rewound first voice sector matches independent source');bind(lib,'ADXF_Close',None,P)(h)
                    rows.append(dict(index=i,real_entry_bytes=entries[i][1],compared_sector_bytes=len(expected),sha256=hashlib.sha256(expected).hexdigest()))
                check(not info().live_files and not stm().live_streams and not fi().pending and not fi().live_handles,'complete voice reads release actual temporary joints, streams and files')
                obs.update(actual_voice_reads=rows,timed_automatic_service=a.case=='timed',open_file_survives_partition_unload=a.case=='reads')
            else:
                h=openafs(0);joint=Ring(rt);rings.append(joint)
                check(bind(lib,'ADXF_ReadSj32',I,P,I,P)(h,8,joint.handle)==8,'real archive request borrows external ring for asynchronous read')
                bind(lib,'ADXSTM_ExecServer',None)()
                check(info().pending_reads==1 and fi().pending and joint.count(0)+joint.count(1)<joint.size,'actual archive read and borrowed joint remain pending')
                check(bind(rt.g2,'jp_g2_gate_set',I,I)(15)==0 and bind(lib,'jp_adxf_shutdown',I)()==7 and info().pending_reads==1,'held actual callback gate preserves archive file, stream and storage ownership')
                check(bind(rt.g2,'jp_g2_gate_set',I,I)(0)==0,'actual callback gate released');rt.close()
                check(not info().active and not info().live_files and not info().owned_nodes and not info().partition_roots,'real System global cleanup retires archive before lower streams and table storage')
                joint.retired();obs.update(actual_pending_archive_System_cleanup=True,guarded_partition_storage_freed_after_read_retirement=True)
        check(not errors,('valid native archive operations emit no original decoder/stream errors',errors))
        if rt.live:
            check(bind(lib,'jp_adxf_shutdown',I)()==0 and info().live_files==info().partition_roots==info().owned_nodes==0,'all real archive state and owned work retire')
            check(bind(lib,'jp_adxstm_shutdown',I)()==0,'lower owner can retire after actual archive parent')
        for joint in rings:joint.close()
        if rt.live:rt.close()
    finally:
        if rt and rt.live:
            bind(rt.g2,'jp_g2_gate_set',I,I)(0);bind(rt.lib,'jp_adxf_shutdown',I)();bind(rt.lib,'jp_adxstm_shutdown',I)();rt.close()
    result=dict(case=a.case,checks=len(labels),assertions=labels,observations=obs)
    (d/'result.json').write_text(json.dumps(result,indent=2)+'\n');print(a.case,len(labels),'checks',flush=True)

if __name__=='__main__':main()
