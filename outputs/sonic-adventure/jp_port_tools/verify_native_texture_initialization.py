"""Whole original initializer plus actual caller-table/frame/GPU ownership.
No original function hooks. Synthetic pixel fixtures; no gameplay claim.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,Info,Record,Name,List,Surface,field
from native_video_test_support import Video
from native_gpu_test_support import GPU,Config,Report,Failure
from capture_native_boot_progress import png
U=C.c_uint32;I=C.c_int;F=C.c_float;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def words(v):return struct.pack('<'+'I'*len(v),*v)
def fw(v):return struct.unpack('<I',struct.pack('<f',v))[0]
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();out=a.output.resolve();assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();out.mkdir()
    b=json.loads((d/'build_verification.json').read_bytes());pin=json.loads((r/'outputs/sonic-adventure/jp_port_tools/jp_camera_external_inputs.json').read_bytes());assert sha(a.binary)==pin['original_binary_sha256']
    historical=json.loads((r/'outputs/sonic-adventure/analysis/jp_ninja_math_v2_verification.json').read_bytes());assert sha(a.upstream/'config/1st_read/symbols.txt')==historical['original_symbols_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ['frames_DLL','GPU_DLL']:assert sha(b[n])==b[n+'_sha256']
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;sdk=bind(lib,'njInitTexture',None,P,U);limits=bind(lib,'jp_texture_sdk_limits',I,U,S)
    path=(C.c_ubyte*256).in_dll(lib,'_nj_current_texture_path');err_surface=Surface.in_dll(lib,'_nj_tex_err_surface');err=(U*8).in_dll(lib,'_nj_texerr_');selected=P.in_dll(lib,'_nj_current_texlist');bank=U.in_dll(lib,'_nj_current_texture_bank');base=P.in_dll(lib,'_nj_texmemlist');size=U.in_dll(lib,'_nj_texmemsize')
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));from verify_startup import State,reference
    table={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary;RECORDS=0x8cd00000;INFO=0x8ce00000;HANDLE=INFO+0x100
    def put(addr,raw):at=addr&0xffffff;ram[at:at+len(raw)]=raw
    def read(addr,n):at=addr&0xffffff;return bytes(ram[at:at+n])
    def word(addr):return struct.unpack('<I',read(addr,4))[0]
    checks=calls=images=pixels=0;seen=set();groups={};shots=[];keep=[];rng=random.Random(0x54455849)
    entries={table[n]:n for n in ['njInitTexture','njInitTexMemList','njEntryTexMemList']}
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def trace(pc,*rest):
        if pc in entries:seen.add(entries[pc])
    def original(name,*args):
        nonlocal calls
        s=State();s.pc=table[name];s.pr=0x8cfed000;s.sr=0x70000000;s.r[15]=0x8cf00000
        for i,value in enumerate(args[:4]):s.r[4+i]=value&0xffffffff
        if len(args)>4:put(s.r[15],words(args[4:]))
        reference(ram,s,trace=trace,instruction_budget=2000000);calls+=1;check(s.r[15]==0x8cf00000,'whole original stack restored')
    check(limits(16,32<<20)==0,'host SDK owner limits')
    counts=[0,1,2,3,4,7,32,128,1024,2048,4096];null_zero=0
    for iteration in range(4):
        for count in counts:
            before=rng.randbytes(64+count*C.sizeof(Record));arena=C.create_string_buffer(before);keep.append(arena);pointer=C.addressof(arena)+32
            guest=rng.randbytes(64+count*68);put(RECORDS-32,guest)
            use_null=count==0 and iteration%2==0
            if use_null:pointer=0;null_zero+=1
            initial_path=rng.randbytes(256);C.memmove(path,initial_path,256);put(table['_nj_current_texture_path'],initial_path)
            surface_bytes=rng.randbytes(C.sizeof(Surface));C.memmove(C.byref(err_surface),surface_bytes,len(surface_bytes));guest_surface=rng.randbytes(40);put(table['_nj_tex_err_surface'],guest_surface)
            untouched=rng.randbytes(32);C.memmove(err,untouched,32);put(table['_nj_texerr_'],untouched)
            selected.value=0x1234567880000000+iteration;t.current.value=rng.getrandbits(32);bank.value=rng.getrandbits(32)
            saved=selected.value,t.current.value,bank.value;put(table['_nj_current_texlist'],words([saved[0]&0xffffffff]));put(table['_nj_current_texture'],words([saved[1]]));put(table['_nj_current_texture_bank'],words([saved[2]]))
            original('njInitTexture',0 if use_null else RECORDS,count);sdk(pointer,count)
            expected=bytearray(before);guest_expected=bytearray(guest)
            for index in range(count):
                start=32+index*C.sizeof(Record);expected[start:start+8]=b'\xff'*8;expected[start+Record.count.offset:start+Record.count.offset+2]=bytes(2)
                start=32+index*68;guest_expected[start:start+8]=b'\xff'*8;guest_expected[start+64:start+66]=bytes(2)
            check(arena.raw[:-1]==expected and read(RECORDS-32,len(guest))==guest_expected,'complete original/native sparse writes, input/output guards and padding')
            expected_surface=bytearray(surface_bytes);expected_surface[:28]=bytes(28);offset=Surface.pSurface.offset;expected_surface[offset:offset+C.sizeof(P)]=bytes(C.sizeof(P));guest_expected_surface=bytearray(guest_surface);guest_expected_surface[:32]=bytes(32)
            check(bytes(err_surface)==expected_surface and read(table['_nj_tex_err_surface'],40)==guest_expected_surface,'exact error surface fields; virtual/physical pointers and native padding preserved')
            check(bytes(path)==b'\0'+initial_path[1:]==read(table['_nj_current_texture_path'],256),'only texture path byte zero changes')
            check(bytes(err)==untouched==read(table['_nj_texerr_'],32) and (selected.value,t.current.value,bank.value)==saved,'original texture error/list/cache state stays untouched')
            check(word(table['_nj_current_texlist'])==saved[0]&0xffffffff and word(table['_nj_current_texture'])==saved[1] and word(table['_nj_current_texture_bank'])==saved[2],'original list/cache state also unchanged')
            check(base.value==(pointer or None) and size.value==count and t.count.value==word(table['_nj_tex_count'])==0 and t.status()==0,'same caller-owned table/count is the actual shared owner')
            check(word(table['_nj_texmemlist'])==(0 if use_null else RECORDS) and word(table['_nj_texmemsize'])==count,'original table pointer/count')
            if count:
                info=Info();name=Name();put(INFO,bytes(44));put(HANDLE,bytes(4));original('njEntryTexMemList',HANDLE,INFO,0x1234,0,0xffffffff);t.entry(field(name),C.byref(info),0x1234,0,0xffffffff)
                check(t.resolve(name.texaddr)==pointer and word(HANDLE)==RECORDS and t.count.value==word(table['_nj_tex_count'])==1,'real allocator cursor resets to slot zero after every initializer')
            else:check(t.nextinfo(1) is None and t.status()==2,'zero-capacity native table rejects allocation safely');t.clear()
    check(set(entries.values())<=seen,'whole genuine initializer/callee/entry executed without hooks');groups['whole_original_value_memory_and_native_layout_contract']=checks
    # These are bounded native policies outside the original valid caller
    # contract: reject bad pointers/overlap/budget without touching the owner.
    check(t.init(4,16,4<<20)==0 and limits(16,4<<20)==0,'existing owned allocation still initializes')
    own=t.record(0);own_bytes=C.string_at(own,4*C.sizeof(Record));saved_path=bytes(path);saved_surface=bytes(err_surface)
    def snapshot():return base.value,size.value,t.count.value,C.string_at(base.value,size.value*C.sizeof(Record)),t.allocated(),t.live(),bytes(path),bytes(err_surface)
    for pointer,count,expected_error in [(0,1,1),(own+1,1,1),(own+C.sizeof(Record),1,1),(own,5,1),(own,0xffffffff,1)]:
        before=snapshot();sdk(pointer,count);check(t.status()==expected_error and snapshot()==before,'invalid/overlapping/oversized caller table preserves owner and SDK metadata');t.clear()
    external=(Record*4)();check(limits(16,1)==0,'choose predictable too-small replacement budget');before=snapshot();sdk(external,4)
    check(t.status()==3 and snapshot()==before and bytes(external)==bytes(C.sizeof(external)),'budget rejection preserves caller and prior owner');t.clear();check(limits(16,4<<20)==0,'restore budget')
    sdk(own,2);check(t.status()==0 and base.value==own and size.value==2 and C.string_at(own+2*C.sizeof(Record),2*C.sizeof(Record))==own_bytes[2*C.sizeof(Record):],'owned table reinitializes/shrinks in place without freeing allocation')
    check(t.allocated()>=4*C.sizeof(Record)+16*(C.sizeof(P)+C.sizeof(U)),'budget still charges retained four-record heap allocation plus minimum binding storage after shrinking to two live records')
    check(limits(1,600)==0,'budget below independently known retained allocation/binding lower bound');before=snapshot();sdk(None,0)
    check(t.status()==3 and snapshot()==before,'replacement cannot bypass budget through undercounted shrunken heap storage');t.clear();check(limits(16,4<<20)==0,'restore ordinary replacement limits')
    sdk(own,4);check(t.status()==0 and base.value==own and size.value==4,'original owned allocation capacity remains available after shrink')
    check(limits(0,1)==1 and limits(16,0)==1,'invalid host limits rejected');t.clear()
    # Calling the lower-level guarded API directly must not bypass the owner.
    before=snapshot();bind(lib,'njInitTexMemList',None,P,U)(external,4);check(t.status()==6 and snapshot()==before,'unguarded table replacement remains rejected');t.clear()
    sdk(external,4);check(t.status()==0 and base.value==C.addressof(external),'switch from owned to caller-owned table');t.shutdown();external[3].dummy=1234
    check(external[3].dummy==1234 and t.init(4,16,4<<20)==0,'shutdown never frees caller storage and old public initialization still works');groups['native_ownership_budget_alias_and_zero_capacity_policies']=checks-sum(groups.values())
    # Actual submitted textured frame survives SDK table replacement and
    # subsequent texture-owner shutdown, including D3D pixels and Texture Off.
    video=Video(lib);gpu=GPU(Path(b['GPU_DLL']));gl=gpu.lib;qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q);orphan=bind(lib,'jp_frames_orphaned_bytes',S);pinned=bind(lib,'jp_frames_pinned_bytes',S)
    reserve=bind(lib,'jp_queue_reserve',I,U,U);ctx=bind(lib,'jp_queue_set_context',I,U,P);pin=bind(lib,'jp_frames_prepare_texture',I,U);get=bind(lib,'njGetkmPointer',None);putkm=bind(lib,'njSetkmPointer',None);start=bind(lib,'njStartVertexTexTrans',P);end=bind(lib,'njEndVertex',None,P)
    check(qi(8<<20)==0 and fi(4,8,8<<20)==0 and video.init(64,64,0xff102030,1)==0,'actual frame/queue/video owners')
    window=P();error=Failure();check(bind(gl,'jp_window_open',I,P,U,U,I,P,P)(gpu.handle,256,256,1,C.byref(window),C.byref(error))==0,'real texture initializer test window')
    bound=bind(gl,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P);direct=bind(gl,'jp_window_render_frame',I,P,Q,P,P,P);readback=bind(gl,'jp_window_readback',I,P,P,S,P);present=bind(gl,'jp_window_present',I,P,U,P)
    def record(color):
        info=Info();s=info.texsurface;s.Type=0x10000;s.BitDepth=1;s.PixelFormat=1<<27;s.nWidth=s.nHeight=8;s.fSurfaceFlags=4;s.TextureSize=128
        check(t.copy(C.byref(info),C.create_string_buffer(struct.pack('<H',color)*64),128)==0,'real owned synthetic RGB565 texture')
        name=Name();t.setname(C.byref(name),None,7000,0);t.entry(field(name),C.byref(info),7000,2,0xffffffff);address=t.resolve(name.texaddr);t.param(address);record=Record.from_address(address);tcw=record.texparambuffer;keep.extend([name,info])
        check(reset()==0 and reserve(1,4)==0 and ctx(0,(U*4)(0x8200000a,0xe0800000,0x209000c0,tcw))==0 and pin(tcw&0x1fffff)==0,'real texture header and resource pin');get();pointer=start();check(bool(pointer),'genuine SDK textured vertex start')
        raw=[]
        for i,(x,y) in enumerate([(-64,-64),(192,-64),(-64,192)]):raw += [0xf0000000 if i==2 else 0xe0000000,fw(x),fw(y),fw(1),fw(.25),fw(.25),0xffa04080,0]
        C.memmove(pointer,words(raw),96);end(pointer+96);putkm();fid=Q();check(submit(C.byref(fid))==0,'real immutable frame from SDK producer');return fid.value,name.texaddr,tcw&0x1fffff
    def image(fid,enabled,rgba,label=None):
        nonlocal images,pixels
        cfg=Config.make(256,256,enabled);buffer=C.create_string_buffer(256*256*4);rep=Report();err=Failure();check(bound(gpu.handle,fid,C.byref(cfg),buffer,len(buffer),C.byref(rep),C.byref(err))==0,'actual bound D3D texture frame')
        check(buffer.raw==bytes(rgba)*(256*256),'independent full-image texture/vertex color oracle');images+=1;pixels+=256*256
        if not enabled:check(rep.texture_uploads==rep.decoded_texels==rep.shader_texture_bindings==rep.shader_sampler_bindings==0,'Texture Off has no sampled bindings or decoding/upload')
        if label:
            check(direct(window,fid,C.byref(cfg),C.byref(rep),C.byref(err))==0,'actual window render');actual=C.create_string_buffer(len(buffer));check(readback(window,actual,len(actual),C.byref(err))==0 and actual.raw==buffer.raw,'actual backbuffer equals checked image')
            path=out/(label+'.png');png(path,actual.raw,256,256);shots.append(dict(path=str(path),sha256=sha(path),actual_window_backbuffer=True,synthetic_renderer_test=True,gameplay=False));check(present(window,0,C.byref(err))==0,'actual texture test Present')
    old,handle,word_id=record(0x07e0);image(old,1,[0,64,0,255],'before-initialization')
    caller=(Record*4)();before=snapshot();check(limits(16,1)==0,'choose too-small replacement budget with actual live resources');sdk(caller,4)
    check(t.status()==3 and snapshot()==before and orphan()==0 and t.live()==1 and bytes(caller)==bytes(C.sizeof(caller)),'failed SDK reset leaves actual live texture, caller table and captured resources intact');t.clear()
    check(t.resolve(handle) is not None and limits(16,4<<20)==0,'old live handle still resolves after rejected reset');image(old,1,[0,64,0,255])
    sdk(caller,4);check(t.status()==0 and base.value==C.addressof(caller) and t.live()==0 and orphan()==pinned()==128,'SDK reset retires old owner and retains submitted texture payload')
    check(t.resolve(handle) is None and t.status()==5,'old handle cannot alias the new table');t.clear();check(t.surface_data(word_id) is None,'retained frame resource is not a new live texture');t.clear()
    image(old,1,[0,64,0,255],'retained-after-initialization');image(old,0,[160,64,128,255],'textures-off-after-initialization')
    new,new_handle,new_word=record(0x001f);check(handle!=new_handle and word_id!=new_word,'record and surface IDs never recycle across genuine SDK reset')
    image(new,1,[0,0,128,255]);sdk(caller,4);check(t.status()==0 and base.value==C.addressof(caller) and t.live()==0 and orphan()==pinned()==256,'same borrowed game table reinitializes in place while both generations remain submitted')
    t.shutdown();check(orphan()==pinned()==256,'both submitted generations survive owner shutdown');image(old,1,[0,64,0,255]);image(new,1,[0,0,128,255])
    check(release(old)==0 and orphan()==pinned()==128 and release(new)==0 and orphan()==pinned()==0,'each submitted generation releases its final payload exactly once')
    bind(gl,'jp_window_close',None,P)(window);gpu.close(gpu.handle);qs();check(fs()==0,'frame owner drains');video.shutdown();groups['actual_frame_generations_D3D_pixels_backbuffers_and_Texture_Off']=checks-sum(groups.values())
    report=dict(status='genuine_texture_initializer_original_memory_caller_table_ownership_and_actual_GPU_lifetimes_verified',checks=checks,original_calls=calls,original_dependencies_seen=sorted(seen),original_function_hooks_used=False,original_contexts=4*len(counts),record_counts=counts,original_game_record_count=2048,null_zero_record_cases=null_zero,complete_native_and_original_arenas_checked=True,padding_and_untouched_error_surface_pointers_preserved=True,original_path_error_list_cache_semantics_preserved=True,native_owned_and_borrowed_table_transitions_verified=True,native_budget_invalid_alias_rejection_preserves_state=True,old_handles_and_surface_IDs_do_not_recycle=True,real_submitted_frames_survive_SDK_table_reset_and_owner_shutdown=True,images_compared=images,pixels_compared=pixels,Texture_Off_zero_sampled_bindings=True,actual_Presents=len(shots),actual_window_screenshots=shots,screenshots_are_synthetic_renderer_tests=True,native_reset_retires_owned_payloads_beyond_original_memory_contract=True,original_binary_sha256=sha(a.binary),original_symbols_sha256=sha(a.upstream/'config/1st_read/symbols.txt'),frozen_instruction_executor_sha256=sha(r/'outputs/sonic-adventure/verify_startup.py'),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(__file__),groups=groups,full_system_boot_main_loop_gameplay_verified=False,playable=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
