"""Exercise real repaired game queue, bounded loader, retained frames and GPU.
No retail assets, fake texture handles, fake GPU completion or CPU image upload.
"""
import argparse,ctypes as C,hashlib,json,random,struct,threading
from pathlib import Path
from native_texture_test_support import Runtime,Info,Name,List,Record,field
from native_packet_test_support import bind
from native_gpu_test_support import GPU,Config,Failure
from native_present_test_support import Present
from native_video_test_support import Video
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
class Bitmap(C.Structure):
    _fields_=[('mode',C.c_uint8),('x',C.c_int16),('y',C.c_int16)]+[(n,C.c_uint16) for n in ['width','height','texture_width','texture_height']]+[(n,C.c_int16) for n in ['cursor_x','cursor_y','letter_spacing','line_spacing']]+[('ink',C.c_uint16),('text_color',U),('back_color',U),('pixels',P),('index',U),('texlist',List),('texname',Name)]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def fw(v):return struct.unpack('<I',struct.pack('<f',v))[0]

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();b=json.loads((d/'build_verification.json').read_text());assert not a.report.exists()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ['DLL','frames_DLL','GPU_DLL']:assert sha(Path(b[n]))==b[n+'_sha256']
    runtime=Runtime(Path(b['frames_DLL']));lib=runtime.dll;video=Video(lib)
    status=bind(lib,'jp_memory_status',I);clear=bind(lib,'jp_memory_clear_error',None);begin=bind(lib,'jp_memory_bind_source',I,P,S);end=bind(lib,'jp_memory_unbind_source',I)
    shutdown=bind(lib,'jp_memory_shutdown',I);global_index=bind(lib,'njInitTextureGlobalIndex',None,U)
    load=bind(lib,'jp_memory_load_list',I,P,P,S);sdkload=bind(lib,'njLoadTexture',I,P);loadone=bind(lib,'njLoadTextureLN',I,P,U)
    rawsize=bind(lib,'jp_memory_raw_size',I,U,U,U,P);setinfo=bind(lib,'njSetTextureInfo',None,P,P,I,I,I)
    queue=bind(lib,'bitmap_queue_add',None,P);flush=bind(lib,'bitmap_flush_queue',None);empty=bind(lib,'bitmap_clear_queue',None);queued=I.in_dll(lib,'texture_queue_count')
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);ctx=bind(lib,'jp_queue_set_context',I,U,P)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);live=bind(lib,'jp_frames_live_count',U)
    reserve=bind(lib,'jp_queue_reserve',I,U,U);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None);vertex_end=bind(lib,'njEndVertex',None,P);pin=bind(lib,'jp_frames_prepare_texture',I,U)
    start=bind(lib,'njStartVertexTexOpaque',P)
    checks=0;groups={};keep=[];rng=random.Random(0x424d5055)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def recover():end();clear();runtime.clear()
    def init(records=32,bindings=128,budget=8<<20):
        recover();empty();check(runtime.init(records,bindings,budget)==0,'actual texture owner initialized')
    check(C.sizeof(Bitmap)==80 and Bitmap.texlist.offset==48 and Bitmap.texname.offset==64,'native external bitmap layout')
    check(qi(4<<20)==0 and fi(8,8,8<<20)==0 and video.init(64,64,0xff123456,1)==0,'actual queue/frame/video owners')
    check(bind(lib,'kmSetPaletteMode',I,I)(3)==0,'real SDK palette mode')
    init();base=runtime.allocated()
    def name_list(data,kind=0x900,w=8,h=8,gid=1000,attr=0x40800000):
        src=C.create_string_buffer(data);info=Info();setinfo(C.byref(info),src,kind,w,h)
        names=(Name*1)();runtime.setname(C.byref(names[0]),C.byref(info),gid,attr);listing=List(names,1)
        keep.extend([src,info,names,listing]);return src,info,names,listing
    def encoded(kind,w,h):
        log=w.bit_length()-1;count=w*h
        if kind in [0x100,0xd00,0x900]:return count*2,0
        if kind==0x200:return 2+2*sum(4**i for i in range(log+1)),4
        if kind==0x300:return 2048+count//4,0
        if kind==0x400:return 2048+1+sum(4**i//4 for i in range(1,log+1)),0
        if kind==0x500:return count//2,0
        if kind==0x600:return 1+sum(max(1,4**i//2) for i in range(log+1)),0
        if kind==0x700:return count,0
        if kind==0x800:return 3+sum(4**i for i in range(log+1)),0
        raise AssertionError(kind)
    kinds=[0x100,0x200,0x300,0x400,0x500,0x600,0x700,0x800,0x900,0xd00]
    formats=images=0
    for kind in kinds:
        for w,h in [(8,8),(16,16),(32,32)]+([(16,8)] if kind in [0x500,0x700,0x900,0xd00] else []):
            for fmt in ([0] if 0x500<=kind<=0x800 else [0,1,2]):
                count,pad=encoded(kind,w,h);data=rng.randbytes(count);src,info,names,listing=name_list(data,kind|fmt,w,h,1000+formats)
                n=S(0);check(rawsize(kind|fmt,w,h,C.byref(n))==0 and n.value==count,'independent chain byte-count oracle')
                check(load(C.byref(listing),src,count)==1 and status()==runtime.status()==0,'bounded actual SDK list load')
                address=runtime.lookup(C.byref(listing),0);check(bool(address),'real handle resolves after load');record=Record.from_address(address);surface=record.texinfo.texsurface
                word=runtime.surface_word(C.byref(surface));check(surface.TextureSize==count+pad and runtime.surface_bytes(word)==count+pad,'actual surface allocation size')
                check(C.string_at(runtime.surface_data(word),count+pad)==bytes(pad)+data,'exact encoded copy and mip prefix')
                check(info.texaddr==C.addressof(src),'borrowed source descriptor never advanced')
                check(record.count==1 and record.globalIndex==1000+formats and surface.nWidth==w and surface.nHeight==h,'actual SDK record metadata')
                if 0x500<=kind<=0x800:check(surface.BitDepth==3,'palette bit depth follows real SDK mode')
                check(runtime.release_list(C.byref(listing))==1 and runtime.live()==0 and runtime.allocated()==base,'actual release reclaims allocation')
                formats+=1
    groups['encoded_formats_sizes_mip_padding_and_actual_release']=checks
    # Sharing and continuation use the same native binding table. Different
    # palette banks use distinct SDK records sharing one actual owned surface.
    init(3,8);src,info,names,listing=name_list(bytes([0x31])*32,0x500,gid=100,attr=0x41000000)
    check(load(C.byref(listing),src,32)==1,'palette source uploaded')
    record0=Record.from_address(runtime.lookup(C.byref(listing),0));word=runtime.surface_word(C.byref(record0.texinfo.texsurface))
    aliases=(Name*3)();aliased=List(aliases,3)
    for i,bank in enumerate([0,16,32]):runtime.setname(C.byref(aliases[i]),None,(bank<<26)|100,0x41000000)
    check(sdkload(C.byref(aliased))==1 and runtime.live()==1,'three aliases and two other palette banks use one surface')
    records=[Record.from_address(runtime.lookup(C.byref(aliased),i)) for i in range(3)]
    check([v.bank for v in records]==[0,16,32] and records[0].count==2,'bank records and same-bank reference count')
    check([v.texparambuffer>>21&63 for v in records]==[0,16,32],'actual SDK parameter builder applies palette bank')
    check(runtime.release_list(C.byref(listing))==1 and runtime.live()==1,'first alias release retains live surface')
    check(runtime.release_list(C.byref(aliased))==1 and runtime.live()==0,'last other-bank release frees surface exactly once')
    # Nonpal continuation shares a reference-counted record even with bank bits;
    # this avoids creating two nonpalette records which would double-free it.
    init(1,4);data=bytes([0xff])*128;src=C.create_string_buffer(data);info=Info();setinfo(C.byref(info),src,0x900,8,8)
    names=(Name*3)();listing=List(names,3)
    runtime.setname(C.byref(names[0]),C.byref(info),123,0x40800000)
    for i in [1,2]:runtime.setname(C.byref(names[i]),None,i<<26,0x40010000)
    check(load(C.byref(listing),src,128)==1 and runtime.live()==1,'continuations share with full record pool')
    record=Record.from_address(runtime.lookup(C.byref(listing),0));check(record.count==3 and len({v.texaddr for v in names})==1,'continuation reference count and handles')
    check(runtime.release_list(C.byref(listing))==1 and runtime.live()==0,'continuation release exactly once')
    groups['sharing_palette_banks_continuation_and_capacity']=checks-sum(groups.values())
    # Bounded chunk parsing, optional global index, unknown chunks and user IDs.
    init();chunks=struct.pack('<4sI4s',b'COMM',4,b'note')+struct.pack('<4sII',b'GBIX',4,789)+struct.pack('<4sIIHH',b'PVRT',136,0x901,8,8)+bytes([0x55])*128
    src,info,names,listing=name_list(chunks,0,0,0,gid=0,attr=0x40000000)
    check(load(C.byref(listing),src,len(chunks))==1 and Record.from_address(runtime.lookup(C.byref(listing),0)).globalIndex==789,'bounded chunks retain GBIX after unknown chunk')
    check(runtime.release_list(C.byref(listing))==1,'chunk load released')
    pvrt=struct.pack('<4sIIHH',b'PVRT',136,0x900,8,8)+bytes([0x55])*128
    src,info,names,listing=name_list(pvrt,0,0,0,gid=0,attr=0x40000000)
    check(load(C.byref(listing),src,len(pvrt))==1 and Record.from_address(runtime.lookup(C.byref(listing),0)).globalIndex==0xffffffef,'unindexed PVR uses real user global index')
    check(runtime.release_list(C.byref(listing))==1,'unindexed load released')
    global_index(0);src,info,names,listing=name_list(pvrt,0,0,0,gid=0,attr=0x40000000)
    check(load(C.byref(listing),src,len(pvrt))==1 and Record.from_address(runtime.lookup(C.byref(listing),0)).globalIndex==0,'last user global index usable')
    src2,info2,names2,list2=name_list(pvrt,0,0,0,gid=0,attr=0x40000000)
    check(load(C.byref(list2),src2,len(pvrt))==-1 and status()==2 and runtime.live()==1,'exhausted user indices cannot alias a different upload');recover()
    check(runtime.release_list(C.byref(listing))==1,'user-index fixture released');global_index(0xffffffef)
    groups['bounded_PVR_chunks_and_generated_indices']=checks-sum(groups.values())
    # Failures must leave no new handle or owned allocation. Deliberately short
    # source views include truncated headers and malicious chunk lengths.
    failures=[(b'PVRT',0x900,8,8,0x40800000,4),(struct.pack('<4sI',b'COMM',0xffffffff),0,0,0,0x40000000,8),
        (pvrt[:-1],0,0,0,0x40000000,len(pvrt)-1),(bytes(127),0x900,8,8,0x40800000,127),
        (bytes(128),0x900,7,8,0x40800000,128),(bytes(128),0xb00,8,8,0x40800000,128),
        (bytes(128),0x903,8,8,0x40800000,128),(bytes(128),0x900,8,8,0xc0800000,128),
        (bytes(128),0x900,8,8,0x60800000,128),(bytes(128),0x900,8,8,0x40c00000,128),
        (bytes(128),0x900,8,8,0x00800000,128)]
    for data,kind,w,h,attr,limit in failures:
        recover();src,info,names,listing=name_list(data,kind,w,h,gid=987,attr=attr);before=bytes(names);allocated=runtime.allocated()
        check(load(C.byref(listing),src,limit)==-1 and status()!=0,'invalid or unsupported source fails explicitly')
        check(bytes(names)==before and runtime.allocated()==allocated and runtime.live()==0,'failure preserves name and allocated surfaces')
    recover();check(sdkload(None)==-1,'null list refused')
    src,info,names,listing=name_list(bytes(128));check(sdkload(C.byref(listing))==-1 and status()==3,'missing bounded view refused');recover()
    check(begin(src,128)==0 and begin(src,128)==2 and end()==0,'nested source view rejected and released');recover()
    check(begin(src,128)==0,'owner-thread source scope');foreign=[]
    def other():foreign.extend([begin(src,128),end(),status(),loadone(C.byref(listing),0)])
    t=threading.Thread(target=other);t.start();t.join();check(foreign==[6,6,6,-1] and status()==0,'foreign thread cannot mutate borrowed view or load');end()
    foreign.clear();t=threading.Thread(target=other);t.start();t.join();check(foreign==[6,6,6,-1] and status()==0,'serialized owner persists after borrowed source view ends')
    check(begin(src,128)==0 and shutdown()==2 and end()==0,'shutdown cannot drop an active source view');recover()
    init(1,1);src,info,names,listing=name_list(bytes(128),gid=100);check(load(C.byref(listing),src,128)==1,'capacity fixture loaded')
    src2,info2,names2,list2=name_list(bytes(128),gid=101);before=runtime.allocated()
    check(load(C.byref(list2),src2,128)==-1 and runtime.live()==1 and runtime.allocated()==before,'full table cannot leak payload');recover()
    check(runtime.release_list(C.byref(listing))==1,'capacity fixture released')
    init(2,4,1024);src,info,names,listing=name_list(bytes(2048),0x900,32,32)
    check(load(C.byref(listing),src,2048)==-1 and runtime.live()==0,'surface budget failure creates no live surface');recover()
    groups['bounds_unsupported_states_thread_capacity_and_budget']=checks-sum(groups.values())
    # Nonadjacent bitmaps with guard bytes prove the original GCC pointer repair
    # uses the real queue entries rather than stepping from its first structure.
    init(16,64);bitmaps=[]
    for i in range(10):
        arena=C.create_string_buffer(b'\xa5'*256);bitmap=Bitmap.from_buffer(arena,64);C.memset(C.addressof(bitmap),0,C.sizeof(bitmap));pixels=C.create_string_buffer(struct.pack('<H',0x8000|i)*64)
        bitmap.width=bitmap.height=bitmap.texture_width=bitmap.texture_height=8;bitmap.pixels=C.addressof(pixels);bitmap.index=2000+i
        bitmaps.append((arena,bitmap,pixels));keep.extend([arena,bitmap,pixels]);queue(C.byref(bitmap))
    check(queued.value==8,'original queue capacity is eight')
    flush();check(queued.value==0 and status()==0 and runtime.live()==8,'original GCC queue uploads eight real nonadjacent bitmaps')
    for i,(arena,bitmap,pixels) in enumerate(bitmaps):
        check(arena.raw[:64]==b'\xa5'*64 and arena.raw[144:256]==b'\xa5'*112,'bitmap guards intact')
        if i<8:
            record=Record.from_address(runtime.lookup(C.byref(bitmap.texlist),0));word=runtime.surface_word(C.byref(record.texinfo.texsurface))
            check(C.string_at(runtime.surface_data(word),128)==pixels.raw[:128] and record.globalIndex==2000+i,'each actual queued pointer supplies correct pixels')
        else:check(not bitmap.texlist.textures,'overflow entries remain unuploaded')
    for _,bitmap,_ in bitmaps[:8]:check(runtime.release_list(C.byref(bitmap.texlist))==1,'queued texture released');bitmap.texlist.textures=C.POINTER(Name)()
    # Failed prefix consumption, retry and clear preserve caller-owned bitmaps.
    first,bad,last=[row[1] for row in bitmaps[:3]];bad.texture_width=7
    for bitmap in [first,bad,last]:queue(C.byref(bitmap))
    flush();check(status()==1 and queued.value==2 and runtime.live()==1,'failure preserves only failed and remaining queue pointers')
    recover();bad.texture_width=8;flush();check(queued.value==0 and runtime.live()==3,'repair and retry upload the pending queue')
    for bitmap in [first,bad,last]:check(runtime.release_list(C.byref(bitmap.texlist))==1,'retry texture released');bitmap.texlist.textures=C.POINTER(Name)()
    queue(C.byref(first));empty();flush();check(queued.value==0 and runtime.live()==0,'clear discards queue without uploads')
    first.texlist.textures=C.pointer(first.texname);first.texlist.nbTexture=1;first.texname.texaddr=0xffffffff
    queue(C.byref(first));flush();check(status()==5 and queued.value==1 and runtime.live()==0,'failed old texture release latches upload failure and preserves queue')
    recover();first.texlist.textures=C.POINTER(Name)();empty()
    groups['original_GCC_bitmap_queue_pointer_repair_retry_and_clear']=checks-sum(groups.values())
    # Real queue retention, asset-derived GPU image and original frame path.
    gpu=GPU(Path(b['GPU_DLL']));present=Present(Path(b['DLL']));device=C.addressof((C.c_byte*1).in_dll(lib,'kmiDeviceExtension'));present.device(device)
    gl=gpu.lib;window=P();failure=Failure();openwin=bind(gl,'jp_window_open',I,P,U,U,I,P,P);closewin=bind(gl,'jp_window_close',None,P)
    copy=bind(gl,'jp_window_copy_job',I,P,Q,P);read=bind(gl,'jp_window_readback',I,P,P,S,P);count=bind(gl,'jp_window_presented',Q,P)
    check(openwin(gpu.handle,64,64,1,C.byref(window),C.byref(failure))==0,'real native window')
    check(present.clock.init(None,None,0)==0 and present.wait_init(60000,1001,8,C.cast(present.window_idle,P),window)==0,'real QPC and logical callback service')
    config=Config.make();check(present.init(gpu.handle,window,device,C.byref(config),5000,0)==0,'actual native presenter')
    present.probe(0,0);present.pacing(2,2)
    # A flip-discard swapchain does not preserve the presented backbuffer.
    # Observe the real completed GPU job in its EOR callback, before flip.
    observed=[];Callback=C.CFUNCTYPE(None,P)
    @Callback
    def completed(argument):
        snapshot=present.snapshot();out=C.create_string_buffer(64*64*4)
        copied=copy(window,snapshot.job,C.byref(failure));read_result=read(window,out,len(out),C.byref(failure))
        observed.append((copied,read_result,out.raw,snapshot.completions))
    keep.append(completed);check(present.callback(C.cast(completed,P),None)==0,'real EOR observes completed job before flip-discard')
    def record_bitmap(bitmap):
        ptr=runtime.lookup(C.byref(bitmap.texlist),0);tcw=Record.from_address(ptr).texparambuffer
        check(reset()==0 and reserve(0,4)==0 and ctx(0,(U*4)(0x8000000a,0xe0800000,0x20800000,tcw))==0 and pin(tcw&0x1fffff)==0,'record actual bitmap texture and retain real allocation')
        get();ptr=start();check(bool(ptr),'real SDK textured writer');words=[]
        for i,(x,y) in enumerate([(-64,-64),(192,-64),(-64,192)]):words += [0xf0000000 if i==2 else 0xe0000000,fw(x),fw(y),fw(1),fw(.25),fw(.25),0xffffffff,0]
        raw=struct.pack('<'+'I'*len(words),*words);C.memmove(ptr,raw,len(raw));vertex_end(ptr+len(raw));put();return tcw
    arena,bitmap,pixel=bitmaps[0];C.memmove(pixel,struct.pack('<H',0xfc00)*64,128);queue(C.byref(bitmap));flush();check(status()==0 and runtime.live()==1,'real red bitmap uploaded')
    record_bitmap(bitmap);old_handle=bitmap.texname.texaddr
    # Refresh after recording: queued packets retain the old red allocation,
    # while the live bitmap becomes green before original frame_wait flushes.
    C.memmove(pixel,struct.pack('<H',0x83e0)*64,128);queue(C.byref(bitmap))
    prior=count(window);present.frame();check(present.status()==status()==0 and count(window)==prior+1 and queued.value==0,'original frame_wait calls actual bitmap flush and real render/flip')
    check(observed==[(0,0,bytes([255,0,0,255])*4096,1)],'queued frame still renders old red pixels after bitmap refresh');images+=1
    check(bitmap.texname.texaddr!=old_handle and live()==0 and runtime.live()==1,'old frame retired; refreshed live bitmap owns new handle')
    record_bitmap(bitmap);present.frame();check(present.status()==0 and len(observed)==2 and observed[-1]==(0,0,bytes([0,255,0,255])*4096,2),'next original frame renders newly uploaded green bitmap');images+=1
    # Invalid queued bitmap stops original frame before GPU submission/flip.
    record_bitmap(bitmap);bad.texture_width=7;queue(C.byref(bad));before=present.snapshot().submissions;prior=count(window);present.frame()
    check(status()==1 and present.snapshot().submissions==before and count(window)==prior and queued.value==1,'bitmap failure stops original frame without fabricated presentation')
    recover();empty();bad.texture_width=8;check(reset()==0,'discard retained recording after explicit recovery')
    check(present.shutdown()==0 and present.wait_shutdown()==0,'presenter clean shutdown');closewin(window);gpu.close(gpu.handle)
    check(runtime.release_list(C.byref(bitmap.texlist))==1 and runtime.live()==0,'final bitmap and retained surfaces released')
    qs();runtime.shutdown();check(fs()==0 and shutdown()==0,'shared owners and serialized upload owner shut down cleanly')
    groups['retained_bitmap_refresh_original_frame_and_real_GPU_images']=checks-sum(groups.values())
    result=dict(status='real_bitmap_queue_bounded_memory_upload_and_native_frame_pixels_verified',checks=checks,groups=groups,encoded_format_cases=formats,actual_GPU_images_compared=images,backbuffer_pixels_compared=images*4096,
        original_GCC_bitmap_queue_pointer_repair_verified=True,source_views_bounded=True,actual_SDK_list_dispatch_record_parameter_and_release=True,
        palette_bank_shared_surfaces_and_continuation_verified=True,refresh_retains_queued_original_pixels=True,original_frame_wait_uses_real_bitmap_queue=True,
        failed_bitmap_upload_stops_frame=True,source_descriptors_unchanged=True,other_game_diagnostic_providers_remain_traces=True,
        full_game_loop_executed=False,playable=False,overall_completion_percent=10,build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)))
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
