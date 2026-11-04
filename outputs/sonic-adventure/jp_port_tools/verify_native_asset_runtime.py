"""Real Windows file/PRS decode, original game wrapper and field ownership."""
import argparse,ctypes as C,hashlib,json,shutil,struct,threading
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,Name,List,Info,Record,field
from native_pvm_test_support import PVM,archive,texture,destination,index_archive,image_oracle
from native_asset_test_support import Assets,Limits,literal_prs
from native_video_test_support import Video
from native_gpu_test_support import GPU,Config,Report,Failure
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def fw(v):return struct.unpack('<I',struct.pack('<f',v))[0]
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','inputs','fixtures','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();fixtures=a.fixtures.resolve();b=json.loads((d/'build_verification.json').read_text());assert not a.report.exists() and not fixtures.exists() and fixtures.is_relative_to(r/'work/sonic-adventure')
    for n,h in dict(b['authored_source_sha256'],**b['vendored_MIT_source_sha256']).items():assert sha(r/n)==h,n
    for n in ['frames_DLL','DLL','GPU_DLL']:assert sha(Path(b[n]))==b[n+'_sha256']
    fixtures.mkdir();root=fixtures/'資産';root.mkdir();inputs=json.loads((a.inputs/'input_manifest.json').read_text())
    for row in inputs['assets']:
        source=Path(row['compressed_path']);assert sha(source)==row['compressed_sha256'];target=root/Path(row['disc_path']).name;shutil.copyfile(source,target);assert sha(target)==row['compressed_sha256']
    valid_pvm=archive([texture(bytes(128))]);(root/'SYNTH.PRS').write_bytes(literal_prs(valid_pvm));(root/'TRAIL.PRS').write_bytes(literal_prs(valid_pvm)+b'trailing');(root/'BAD.PRS').write_bytes(b'\x01');(root/'EMPTY.PRS').write_bytes(b'');(root/'NOTPVM.PRS').write_bytes(literal_prs(b'plain decoded bytes'))
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;pvm=PVM(lib);assets=Assets(lib);video=Video(lib);checks=0;groups={};keep=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def clear():assets.clear();pvm.clear();t.clear();video.clear()
    def texinit(records=64,bindings=1024):
        pvm.clear();t.clear();check(t.init(records,bindings,32<<20)==0,'real native texture owner')
    def open_assets(residents=64,file_bytes=4<<20,decoded=16<<20,budget=32<<20):
        limits=Limits(file_bytes,decoded,budget,residents);check(assets.init(str(root),C.byref(limits))==0,'real Unicode-root Windows asset provider');return assets.snapshot().owned_bytes
    def close_assets():clear();check(assets.recover()==0 and assets.evict()==0 and assets.shutdown()==0,'asset owners drained and shut down')
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);ctx=bind(lib,'jp_queue_set_context',I,U,P);fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I)
    check(qi(8<<20)==0 and fi(8,128,32<<20)==0 and video.init(64,64,0xff123456,1)==0,'real queue/frame/video owners')
    texinit(6,6)
    # Complete sidecar capacity: moving bindings reuses the existing entries,
    # preserves record counts and destination filenames, and removes old fields.
    names,listing,buffers=destination(3);payload=archive([texture(bytes([i])*128) for i in range(3)]);src=C.create_string_buffer(payload)
    check(pvm.load(C.byref(listing),3,28,src,len(payload))==1,'actual source texture records')
    to=(Name*3)();outputs=List(to,3);filename=C.create_string_buffer(b'X\0')
    for name in to:name.filename=C.addressof(filename)
    handles=[name.texaddr for name in names];counts=[Record.from_address(t.lookup(C.byref(listing),i)).count for i in range(3)]
    to[0].texaddr=handles[0];check(t.resolve_field(field(to[0])) is None and t.status()==5,'raw copied handle is not a bound renderer field');t.clear()
    check(assets.validate(names,to,3)==0 and assets.move(names,to,3)==0,'whole-list binding move succeeds without acquiring extra references')
    check([name.texaddr for name in to]==handles and [name.filename for name in to]==[C.addressof(filename)]*3 and filename.raw==b'X\0\0','destination pointers and short filename bytes preserved')
    for i in range(3):
        check(t.resolve_field(field(names[i])) is None,'old source field no longer owns handle');t.clear()
        check(Record.from_address(t.lookup(C.byref(outputs),i)).count==counts[i],'record reference count unchanged by relocation')
    before=bytes(to);check(assets.move(to,C.byref(to,16),2)==1 and bytes(to)==before,'partially overlapping name ranges rejected without writes');t.clear()
    check(assets.move(to,to,3)==0 and assets.move(None,None,0)==0,'identity and zero moves preserve ownership')
    check(t.release_list(C.byref(outputs))==1 and t.live()==0,'moved source records release normally')
    # Shared aliases, full table, and a preflight failure late in a move.
    texinit(2,2);names,listing,buffers=destination(2);payload=archive([texture(bytes(128))]*2,gids=[77,77]);src=C.create_string_buffer(payload)
    check(pvm.load(C.byref(listing),2,28,src,len(payload))==1,'two real aliases fill the binding table');to=(Name*2)();outputs=List(to,2)
    check(assets.move(names,to,2)==0 and Record.from_address(t.lookup(C.byref(outputs),0)).count==2,'full binding table alias move uses no vacant slot')
    check(t.release_list(C.byref(outputs))==1 and t.live()==0,'alias surface freed exactly once')
    texinit(4,8);names,listing,buffers=destination(2);payload=archive([texture(bytes(128)),texture(bytes(128))]);src=C.create_string_buffer(payload);check(pvm.load(C.byref(listing),2,28,src,len(payload))==1,'atomic move fixture')
    one,other,buf=destination(1);p2=archive([texture(bytes(128))],gids=[999]);s2=C.create_string_buffer(p2);check(pvm.load(C.byref(other),1,28,s2,len(p2))==1,'occupied destination fixture')
    to=(Name*2)();check(t.store(field(to[1]),t.lookup(C.byref(other),0))==0,'diagnostic destination binding without an additional reference');source_before=bytes(names);dest_before=bytes(to)
    check(assets.move(names,to,2)==6 and bytes(names)==source_before and bytes(to)==dest_before,'late invalid destination cannot partially move first entry');t.clear();t.forget(field(to[1]))
    check(pvm.release(C.byref(listing))==1 and pvm.release(C.byref(other))==1,'atomic fixture released')
    groups['native_field_binding_moves_aliases_full_capacity_and_atomic_validation']=checks
    texinit();base=open_assets()
    for row in inputs['assets']:
        out=P(0x1234);n=S(1234);check(assets.read(Path(row['disc_path']).name.encode(),C.byref(out),C.byref(n))==0,'actual Windows file read and pinned PRS decode')
        check(n.value==row['PVM_bytes'] and hashlib.sha256(C.string_at(out,n.value)).hexdigest()==row['PVM_sha256'],'native decoded bytes equal independently decompressed supplied-disc PVM')
        state=assets.snapshot();check(state.owned_bytes==base+n.value and state.decoded_bytes==n.value,'owned decode buffer counted against budget')
        old=out.value;check(assets.read(b'SEGALOGO.PRS',C.byref(out),C.byref(n))==2 and out.value==old,'nested file read preserves live decoded view');check(assets.release()==0,'decoded view can be released after deliberate error');clear()
    out=P();n=S();check(assets.read(b'TRAIL.PRS',C.byref(out),C.byref(n))==0 and C.string_at(out,n.value)==valid_pvm,'legacy PRS end marker permits trailing file bytes');check(assets.release()==0,'trailing fixture decoded data released')
    for path,want in [(b'../outside.PRS',1),(b'/absolute.PRS',1),(b'C:\\other.PRS',1),(b'a//b.PRS',1),(b'bad:.PRS',1),(b'.',1),(b'',1),(b'MISSING.PRS',4),(b'BAD.PRS',7),(b'EMPTY.PRS',6)]:
        out=P(0x1234);n=S(1234);check(assets.read(path,C.byref(out),C.byref(n))==want and out.value==0x1234 and n.value==1234,'invalid/missing/malformed file preserves output view')
        check(assets.snapshot().owned_bytes==base and assets.snapshot().decoded_bytes==0,'failed read/decode leaks no owned bytes');clear()
    foreign=[]
    def other_thread():foreign.extend([assets.status(),assets.shutdown(),assets.read(b'SYNTH.PRS',C.byref(P()),C.byref(S()))])
    thread=threading.Thread(target=other_thread);thread.start();thread.join();check(foreign==[3,3,3] and assets.status()==0,'foreign thread cannot mutate file/PRS owner');close_assets()
    for max_file,max_decoded,budget in [(1,16<<20,32<<20),(4<<20,1,32<<20),(4<<20,16<<20,base+15984+786848-1)]:
        base_limited=open_assets(file_bytes=max_file,decoded=max_decoded,budget=budget);out=P(0x1234);n=S(1234)
        check(assets.read(b'SEGALOGO.PRS',C.byref(out),C.byref(n))==6 and out.value==0x1234 and n.value==1234,'file, decoded-size and simultaneous-buffer limits enforced')
        check(assets.snapshot().owned_bytes==base_limited,'limit failure reclaims all transient allocations');close_assets()
    groups['real_file_PRS_hashes_budgets_paths_failure_ownership_and_thread']=checks-sum(groups.values())
    # Original wrapper moves into real game names with immutable short filename
    # buffers. It also retains/deduplicates unclaimed archive refs deliberately.
    texinit(6,6);open_assets(residents=0);names=(Name*6)();game=List(names,6);short=C.create_string_buffer(b'a\0')
    for name in names:name.filename=C.addressof(short)
    assets.original(b'SEGALOGO',C.byref(game));check(assets.status()==0 and pvm.loaded(C.byref(game))==6 and t.live()==6,'original 512-entry scratch wrapper hands ownership to actual game list with full binding table')
    check([name.filename for name in names]==[C.addressof(short)]*6 and short.raw==b'a\0\0','original game filename pointers and short contents remain intact')
    check(t.release_list(C.byref(game))==1 and t.live()==0,'game list releases after original wrapper')
    assets.original(b'TITLE',C.byref(game));check(assets.status()==0 and pvm.loaded(C.byref(game))==6,'original static scratch list is reusable without old field bindings');check(t.release_list(C.byref(game))==1,'second wrapper load released');close_assets()
    texinit();open_assets(residents=0);names=(Name*1)();game=List(names,1);old=bytes(names)
    assets.original(b'SEGALOGO',C.byref(game));state=assets.snapshot();check(assets.status()==6 and state.pending_refs==6 and bytes(names)==old,'surplus residency limit fails before any destination transfer')
    clear();check(assets.recover()==0 and t.live()==0,'failed scratch ownership is recovered through the real PVM release path');close_assets()
    open_assets(residents=5)
    for repeat in range(3):
        assets.original(b'SEGALOGO',C.byref(game));check(assets.status()==0 and pvm.loaded(C.byref(game))==1 and assets.snapshot().resident_refs==5 and t.live()==6,'subset keeps one explicit resident reference per unclaimed texture')
        check(t.release_list(C.byref(game))==1 and t.live()==5,'repeat load deduplicates resident aliases without leaking references')
    check(assets.evict()==0 and t.live()==0,'explicit stage/shutdown residency release frees unclaimed surfaces')
    names=(Name*7)();too_big=List(names,7);before=bytes(names);assets.original(b'SEGALOGO',C.byref(too_big))
    check(assets.status()==1 and bytes(names)==before and assets.snapshot().pending_refs==6,'archive smaller than declared game list cannot copy stale scratch slots');clear();check(assets.recover()==0 and t.live()==0,'mismatched-count recovery')
    names=(Name*6)();game=List(names,6);assets.original(b'MISSING',C.byref(game));check(assets.status()==4 and pvm.loaded(C.byref(game))==0 and t.live()==0,'original void wrapper stops on file failure without copying stale handles');clear()
    names,listing,buffers=destination(1);before=bytes(names);check(assets.load(b'NOTPVM.PRS',C.byref(listing),1,28)==-1 and assets.status()==8 and bytes(names)==before and t.live()==0,'malformed decoded PVM preserves output names');clear();check(assets.recover()==0,'empty partial-recovery path');close_assets()
    groups['original_game_scratch_wrapper_transfers_residency_retry_and_failures']=checks-sum(groups.values())
    # Actual file -> PRS -> original wrapper -> field bindings -> GPU pixels.
    texinit();open_assets();gpu=GPU(Path(b['GPU_DLL']));config=Config.make();bound=bind(gpu.lib,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P)
    reserve=bind(lib,'jp_queue_reserve',I,U,U);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None);end=bind(lib,'njEndVertex',None,P);start=bind(lib,'njStartVertexTexOpaque',P);pin=bind(lib,'jp_frames_prepare_texture',I,U);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q)
    images=0;pixels=0;asset_reports=[]
    for row in inputs['assets']:
        count=row['texture_count'];names=(Name*count)();listing=List(names,count);short=C.create_string_buffer(b'g\0');keep.extend([names,listing,short])
        for name in names:name.filename=C.addressof(short)
        basename=Path(row['disc_path']).stem.encode();assets.original(basename,C.byref(listing));check(assets.status()==0 and pvm.loaded(C.byref(listing))==count,'original game wrapper loads complete disc archive from compressed file')
        flags,table,source_images=index_archive(Path(row['PVM']).read_bytes())
        for sequence,image in enumerate(source_images):
            index=next(i for i,entry in enumerate(table) if entry['number']==sequence);record=Record.from_address(t.lookup(C.byref(listing),index));tcw=record.texparambuffer;tsp=record.tspparambuffer
            check(reset()==0 and reserve(0,4)==0 and ctx(0,(U*4)(0x8000000a,0xe0800000,0x20800000|tsp,tcw))==0 and pin(tcw&0x1fffff)==0,'moved game-list binding supplies actual renderer texture')
            get();ptr=start();words=[]
            for i,(x,y,u,v) in enumerate([(-64,-64,-1,-1),(192,-64,3,-1),(-64,192,-1,3)]):words += [0xf0000000 if i==2 else 0xe0000000,fw(x),fw(y),fw(1),fw(u),fw(v),0xffffffff,0]
            raw=struct.pack('<'+'I'*len(words),*words);C.memmove(ptr,raw,len(raw));end(ptr+len(raw));put();frame=Q();check(submit(C.byref(frame))==0,'actual frame retains moved game texture')
            output=C.create_string_buffer(64*64*4);report=Report();failure=Failure();rc=bound(gpu.handle,frame.value,C.byref(config),output,len(output),C.byref(report),C.byref(failure))
            check(rc==0 and output.raw==image_oracle(image,ignore_alpha=True),('real file-loaded game texture image oracle',basename,sequence,rc,failure.code));check(release(frame.value)==0,'file-loaded frame retired');images+=1;pixels+=4096
        check(short.raw==b'g\0\0' and t.release_list(C.byref(listing))==1 and t.live()==0,'file-loaded game names and all texture refs survive/release correctly')
        asset_reports.append(dict(disc_path=row['disc_path'],compressed_sha256=row['compressed_sha256'],decoded_sha256=row['PVM_sha256'],GPU_images=count))
    gpu.close(gpu.handle);state=assets.snapshot();close_assets();qs();t.shutdown();check(fs()==0 and pvm.shutdown()==0,'all native owners drained')
    groups['native_file_original_game_wrapper_and_GPU_texture_probes']=checks-sum(groups.values())
    result=dict(status='native_file_PRS_original_scratch_wrapper_handle_moves_and_disc_pixels_verified',checks=checks,groups=groups,retail_assets=asset_reports,actual_GPU_images=images,pixels_compared=pixels,
        native_decoded_hashes_equal_private_independent_PVM_inputs=True,original_512_entry_wrapper_uses_real_native_file_PRS_PVM=True,field_move_reference_counts_and_full_capacity_verified=True,
        short_original_filename_pointers_preserved=True,resident_surplus_references_bounded_deduplicated_and_releasable=True,partial_failure_cleanup_and_static_scratch_reuse_verified=True,
        Unicode_root_and_path_limits_verified=True,MIT_PRS_codec_unchanged=True,file_stats=dict(files=state.files_read,decodes=state.decodes,pvm_loads=state.pvm_loads,transfers=state.transfers),
        full_game_loop_executed=False,playable=False,overall_completion_percent=10,build_report_sha256=sha(d/'build_verification.json'),input_manifest_sha256=sha(a.inputs/'input_manifest.json'),verifier_sha256=sha(Path(__file__)),support_sha256=sha(Path(__file__).with_name('native_asset_test_support.py')))
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
