"""Direct SDK/font surface lifetime through real owner, frames and D3D.
Synthetic constant textures; genuine print functions are not yet integrated.
"""
import argparse,ctypes as C,hashlib,json,struct
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,Info,Record,Name,Surface,field
from native_video_test_support import Video
from native_gpu_test_support import GPU,Config,Report,Failure
from capture_native_boot_progress import png
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def fw(v):return struct.unpack('<I',struct.pack('<f',v))[0]
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();out=a.output.resolve();assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();out.mkdir()
    b=json.loads((d/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ['frames_DLL','GPU_DLL']:assert sha(b[n])==b[n+'_sha256']
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;replace=bind(lib,'jp_texture_replace_standalone_surface',I,P,P,S);sdk=bind(lib,'njInitTexture',None,P,U);limits=bind(lib,'jp_texture_sdk_limits',I,U,S);free=bind(lib,'kmFreeTexture',I,P)
    owned=bind(lib,'jp_texture_surface_owned',I,P);checks=images=pixels=0;shots=[];groups={}
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def font_surface():
        s=Surface();s.Type=0x01010000;s.BitDepth=1;s.PixelFormat=0;s.nWidth=s.nHeight=128;s.TextureSize=32768;s.fSurfaceFlags=4;return s
    red=struct.pack('<H',0xfc00)*16384;blue=struct.pack('<H',0x801f)*16384;font=font_surface();before=bytes(font)
    check(replace(C.byref(font),C.create_string_buffer(red),len(red))==8 and bytes(font)==before,'uninitialized owner rejects without writes')
    check(t.init(8,64,4<<20)==0,'real shared owner');base_cost=t.allocated()
    check(replace(C.byref(font),C.create_string_buffer(red),len(red))==0 and owned(C.byref(font))==1,'real directly owned font-sized surface');word=t.surface_word(C.byref(font))
    check(word and font.pSurface>0xffffffff and font.pSurface%32==0 and t.surface_bytes(word)==len(red) and C.string_at(t.surface_data(word),len(red))==red,'complete aligned standalone payload and shared ID lookup')
    check(t.live()==1 and t.allocated()==base_cost+len(red),'direct surface included in live/budget accounting')
    # Actual original SDK parameter builder must consume the direct surface.
    temporary=Record();temporary.texinfo.texsurface=font;t.param(C.byref(temporary));check(t.status()==0 and temporary.tspparambuffer==36 and temporary.texparambuffer==word,'genuine njSetTextureParam sees direct ownership, UV size and shared resource ID')
    info=Info();s=info.texsurface;s.Type=0x10000;s.BitDepth=1;s.PixelFormat=1<<27;s.nWidth=s.nHeight=8;s.TextureSize=128;s.fSurfaceFlags=4
    check(t.copy(C.byref(info),C.create_string_buffer(b'\xe0\x07'*64),128)==0,'ordinary record surface coexists');name=Name();t.setname(C.byref(name),None,9000,0);t.entry(field(name),C.byref(info),9000,2,0xffffffff);record_address=t.resolve(name.texaddr);t.param(record_address);regular=Record.from_address(record_address);regular_word=t.surface_word(C.byref(s));regular_tsp=regular.tspparambuffer;regular_tcw=regular.texparambuffer
    check(word!=regular_word and t.live()==2 and t.allocated()==base_cost+32768+128,'one shared ID generator and accounting for both resource classes');groups['direct_surface_and_real_SDK_parameter_ownership']=checks
    video=Video(lib);gpu=GPU(Path(b['GPU_DLL']));gl=gpu.lib;qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q);orphan=bind(lib,'jp_frames_orphaned_bytes',S);pinbytes=bind(lib,'jp_frames_pinned_bytes',S)
    reserve=bind(lib,'jp_queue_reserve',I,U,U);ctx=bind(lib,'jp_queue_set_context',I,U,P);prepare=bind(lib,'jp_frames_prepare_texture',I,U);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None);start=bind(lib,'njStartVertexTexTrans',P);end=bind(lib,'njEndVertex',None,P)
    check(qi(8<<20)==0 and fi(8,16,8<<20)==0 and video.init(64,64,0xff102030,1)==0,'actual queue/frame/video owners')
    window=P();error=Failure();check(bind(gl,'jp_window_open',I,P,U,U,I,P,P)(gpu.handle,256,256,1,C.byref(window),C.byref(error))==0,'actual direct-surface test window')
    bound=bind(gl,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P);direct=bind(gl,'jp_window_render_frame',I,P,Q,P,P,P);readback=bind(gl,'jp_window_readback',I,P,P,S,P);present=bind(gl,'jp_window_present',I,P,U,P)
    def capture(tsp,tcw):
        check(reset()==0 and reserve(1,4)==0 and ctx(0,(U*4)(0x8200000a,0xe0800000,0x209000c0|tsp,tcw))==0 and prepare(tcw&0x1fffff)==0,'actual SDK context/resource preparation');get();ptr=start();check(bool(ptr),'real SDK textured start');raw=[]
        for i,(x,y) in enumerate([(-64,-64),(192,-64),(-64,192)]):raw += [0xf0000000 if i==2 else 0xe0000000,fw(x),fw(y),fw(1),fw(.25),fw(.25),0xffa04080,0]
        C.memmove(ptr,struct.pack('<24I',*raw),96);end(ptr+96);put();fid=Q();check(submit(C.byref(fid))==0,'actual immutable frame');return fid.value
    def image(fid,rgba,enabled=1,label=None):
        nonlocal images,pixels
        cfg=Config.make(256,256,enabled);buf=C.create_string_buffer(256*256*4);rep=Report();err=Failure();check(bound(gpu.handle,fid,C.byref(cfg),buf,len(buf),C.byref(rep),C.byref(err))==0,'real D3D direct-surface frame')
        check(buf.raw==bytes(rgba)*65536,'independent full-image colour oracle');images+=1;pixels+=65536
        if not enabled:check(rep.texture_uploads==rep.decoded_texels==rep.shader_texture_bindings==rep.shader_sampler_bindings==0,'Texture Off uses vertex colours without sampled bindings or texture decode/upload')
        if label:
            check(direct(window,fid,C.byref(cfg),C.byref(rep),C.byref(err))==0,'actual swapchain render');actual=C.create_string_buffer(len(buf));check(readback(window,actual,len(actual),C.byref(err))==0 and actual.raw==buf.raw,'actual backbuffer equals checked D3D result');path=out/(label+'.png');png(path,actual.raw,256,256);shots.append(dict(path=str(path),sha256=sha(path),actual_window_backbuffer=True,synthetic_renderer_test=True,gameplay=False));check(present(window,0,C.byref(err))==0,'real Present')
    fa=capture(36,word);fb=capture(regular_tsp,regular_tcw);check(pinbytes()==32768+128 and orphan()==0,'submitted direct and record resources share actual frame owner')
    table=(Record*4)();sdk(table,4);check(t.status()==0 and t.live()==1 and t.surface_word(C.byref(font))==word and owned(C.byref(font))==1,'genuine SDK record-table reset preserves direct surface')
    check(C.string_at(t.surface_data(word),len(red))==red and orphan()==128 and pinbytes()==32768+128,'only old regular payload becomes orphaned; direct payload remains live')
    sdk(None,0);check(t.status()==0 and t.live()==1 and owned(C.byref(font))==1,'direct font remains owned with zero regular records')
    image(fa,[160,0,0,255],label='direct-surface-after-record-reset');image(fb,[0,64,0,255]);fc=capture(36,word);image(fc,[160,0,0,255]);check(pinbytes()==32768+128,'same direct resource is deduplicated across later submitted frames');sdk(table,4);check(t.status()==0 and t.live()==1,'same direct font survives another caller-table initialization')
    # An independent budget lower bound: the direct pixels alone occupy the
    # entire selected limit, leaving no room for any replacement metadata.
    check(limits(16,32768)==0,'set below direct-plus-metadata lower bound');before=bytes(font),t.allocated(),t.live();sdk(table,4)
    check(t.status()==3 and before==(bytes(font),t.allocated(),t.live()) and orphan()==128,'SDK replacement budget includes persistent direct bytes');t.clear();check(limits(16,4<<20)==0,'restore SDK limits')
    old_font=Surface.from_buffer_copy(font);check(replace(C.byref(font),C.create_string_buffer(blue),len(blue))==0,'replace standalone surface transactionally');new_word=t.surface_word(C.byref(font))
    check(new_word not in [word,regular_word] and t.live()==1 and orphan()==32768+128,'new unique direct ID; submitted old direct payload retires through existing frame retention')
    check(owned(C.byref(old_font))==0 and t.status()==5,'old direct descriptor cannot resolve as the new live resource');t.clear();fd=capture(36,new_word)
    image(fd,[0,0,128,255],label='replacement-direct-surface');image(fa,[160,0,0,255]);image(fc,[160,0,0,255]);image(fd,[160,64,128,255],0,'direct-surface-textures-off')
    before=bytes(font),t.allocated(),t.live();check(t.init(2,8,32767)==3 and before==(bytes(font),t.allocated(),t.live()),'failed full-owner replacement preserves the direct resource and its budget');t.clear()
    check(t.init(2,8,4<<20)==0 and t.live()==0 and orphan()==32768*2+128,'explicit full owner replacement retires direct resource as well as record resources')
    image(fa,[160,0,0,255]);image(fb,[0,64,0,255]);image(fc,[160,0,0,255]);image(fd,[0,0,128,255]);t.shutdown();check(orphan()==32768*2+128,'full shutdown preserves all submitted encoded generations')
    for fid in [fa,fb,fc,fd]:check(release(fid)==0,'submitted generation released')
    check(pinbytes()==orphan()==0,'last actual frame reference releases every retired payload exactly once');groups['SDK_reset_replacement_and_actual_submitted_D3D_generations']=checks-sum(groups.values())
    # Tight simultaneous budget: old/new direct allocations coexist during
    # replacement; ordinary copies also account for the live direct payload.
    check(t.init(2,8,4<<20)==0,'measure clean metadata cost');cost=t.allocated();t.shutdown();check(t.init(2,8,cost+32768+128)==0,'exact direct-plus-one-record payload budget')
    font=font_surface();check(replace(C.byref(font),C.create_string_buffer(red),len(red))==0,'direct payload fits exact budget');first_word=t.surface_word(C.byref(font));check(first_word not in [word,new_word,regular_word],'direct and record IDs do not recycle across full shutdown/reinitialization');before=bytes(font),t.allocated(),t.live()
    check(replace(C.byref(font),C.create_string_buffer(blue),len(blue))==3 and before==(bytes(font),t.allocated(),t.live()),'replacement requires simultaneous old/new budget and preserves old descriptor');t.clear()
    info=Info();info.texsurface.Type=0x10000;info.texsurface.BitDepth=1;info.texsurface.PixelFormat=1<<27;info.texsurface.nWidth=info.texsurface.nHeight=8;info.texsurface.TextureSize=128;info.texsurface.fSurfaceFlags=4
    check(t.copy(C.byref(info),C.create_string_buffer(b'\xe0\x07'*64),128)==0 and t.allocated()==cost+32768+128,'ordinary surface fits remaining budget exactly');another=Info.from_buffer_copy(info);another.texsurface.pSurface=another.texsurface.pVirtual=another.texsurface.pPhysical=None;before=t.allocated(),t.live()
    check(t.copy(C.byref(another),C.create_string_buffer(bytes(128)),128)==3 and before==(t.allocated(),t.live()) and not another.texsurface.pSurface,'record copy cannot spend direct-surface bytes again');t.clear()
    invalid=font_surface();invalid.TextureSize=1;before=bytes(invalid),t.allocated();check(replace(C.byref(invalid),C.create_string_buffer(bytes(128)),128)==1 and before==(bytes(invalid),t.allocated()),'invalid direct metadata preserves descriptor and allocation');t.clear()
    alias=Surface.from_buffer_copy(font);check(free(C.byref(font))==0 and not font.pSurface and not font.pVirtual and not font.pPhysical and t.live()==1 and t.allocated()==cost+128,'genuine kmFreeTexture boundary releases direct surface and accounting')
    before=bytes(alias),t.allocated(),t.live();check(free(C.byref(alias))!=0 and before==(bytes(alias),t.allocated(),t.live()),'stale alias cannot double-free a direct surface or affect regular payloads');t.clear()
    check(t.surface_data(first_word) is None,'released direct ID cannot resolve');t.clear();font=font_surface();check(replace(C.byref(font),C.create_string_buffer(red),len(red))==0 and t.live()==2,'active direct and regular resources before full shutdown');t.shutdown();check(t.allocated()==t.live()==0 and t.status()==8,'complete owner cleanup retires both classes')
    bind(gl,'jp_window_close',None,P)(window);gpu.close(gpu.handle);qs();check(fs()==0,'all frames drained');video.shutdown();groups['simultaneous_budgets_invalid_metadata_direct_free_and_cleanup']=checks-sum(groups.values())
    report=dict(status='shared_owner_direct_SDK_surface_persistence_frame_generations_and_D3D_pixels_verified',checks=checks,images_compared=images,pixels_compared=pixels,direct_surface_payload_bytes=32768,SDK_record_reset_preserves_direct_surface=True,explicit_full_owner_reset_retires_direct_surface=True,one_shared_nonrecycled_resource_ID_generator=True,genuine_njSetTextureParam_consumes_direct_surface=True,direct_and_record_budgets_and_counts_verified=True,simultaneous_replacement_budget_failure_preserves_old_resource=True,submitted_direct_and_record_generations_survive_all_resets=True,Texture_Off_zero_sampled_bindings=True,actual_Presents=len(shots),actual_window_screenshots=shots,screenshots_are_synthetic_renderer_tests=True,genuine_print_family_or_font_glyph_rendering_verified=False,build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(__file__),groups=groups,full_system_boot_main_loop_gameplay_verified=False,playable=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
