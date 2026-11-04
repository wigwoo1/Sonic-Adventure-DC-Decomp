"""Original SDK state setters, immutable frames and real DXGI presentation.
Synthetic assets only; this is not Kamui scheduling or a playable game loop.
"""
import argparse,ctypes as C,hashlib,json,random,struct,sys,time
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
from native_packet_test_support import bind
from native_gpu_test_support import GPU,Config,Report,Failure
from native_video_test_support import Video,Snapshot,Context
from native_texture_test_support import Runtime,Info,Name,List,Record,field
from sh4_binary32 import multiply
U=C.c_uint32;I=C.c_int;F=C.c_float;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def fw(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def fv(v):return struct.unpack('<f',struct.pack('<I',v))[0]
def channel(v):return int(fv(multiply(fw(v),fw(255.),mode=1)[0]))
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();assert not a.report.exists();b=json.loads((d/'build_verification.json').read_text())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert b['exit_code']==b['diagnostic_bytes']==0 and sha(Path(b['DLL']))==b['DLL_sha256']
    runtime=Runtime(Path(b['frames_DLL']));lib=runtime.dll;video=Video(lib);gpu=GPU(Path(b['DLL']));gl=gpu.lib
    bound=bind(gl,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P)
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);ctx=bind(lib,'jp_queue_set_context',I,U,P)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q)
    reserve=bind(lib,'jp_queue_reserve',I,U,U);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None);end=bind(lib,'njEndVertex',None,P)
    pin=bind(lib,'jp_frames_prepare_texture',I,U);start=bind(lib,'njStartVertexTexOpaque',P);qstatus=bind(lib,'jp_queue_status',I)
    background=bind(lib,'kmSetBackGroundRenderState',I,P);fog=bind(lib,'kmSetFogTable',I,P);palette_data=bind(lib,'kmSetPaletteData',I,P);palette_bank=bind(lib,'kmSetPaletteBankData',I,I,I,P)
    palette_mode=bind(lib,'kmSetPaletteMode',I,I);alpha=bind(lib,'kmSetAlphaThreshold',I,I);cull=bind(lib,'kmSetCullingRegister',I,F);clamp=bind(lib,'kmSetColorClampValue',I,U,U)
    fogcolor=bind(lib,'kmSetFogTableColor',I,U);vfogcolor=bind(lib,'kmSetFogVertexColor',I,U);density=bind(lib,'kmSetFogDensity',I,U);border=bind(lib,'kmSetBorderColor',I,U)
    clip=bind(lib,'kmSetPixelClipping',I,I,I,I,I);global_clip=bind(lib,'kmSetGlobalClipping',I,I,I);sort=bind(lib,'kmSetAutoSortMode',I,I);stride=bind(lib,'kmSetStrideWidth',I,I)
    checks=images=pixels=presented=0;groups={};rng=random.Random(0x56494445);keep=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    check(video.init(64,64,0xff102030,1)==0 and qi(16<<20)==0 and fi(8,16,2<<20)==0 and runtime.init(8,128,2<<20)==0,'real stateful owners')
    # Known-source complete SDK setter bodies. Float products use the independent
    # integer SH-4 binary32 oracle, then the SDK's truncation and bit layout.
    context=Context()
    for case in range(32):
        for i in range(4):context.face[i]=rng.random();context.offset[i]=rng.random()
        check(background(C.byref(context))==0,'real SDK background setter');s=video.snapshot()
        expected=sum(channel(context.face[i])<<shift for i,shift in enumerate([24,16,8,0]));check(s.clear_argb==expected,'RZ packed background value')
    table=(F*128)(*[rng.random() for _ in range(128)]);check(fog(table)==0,'real SDK fog table');s=video.snapshot();values=[channel(v)&255 for v in table]
    for i in range(128):check(s.fog_table[i]==(values[i]<<8|values[min(i+1,127)]),'RZ fog register packing')
    control=0
    for n in [-1,0,1,127,128,255,300]:
        check(alpha(n)==0,'real SDK alpha threshold');nn=max(0,min(255,n));control=(control&~0x00ff0000)|(nn<<15)
        check(video.snapshot().fb_write_control==control,'preserved original alpha shift/carry behavior')
    check(cull(F(-0.25))==0 and video.snapshot().cull_bits==fw(0.25),'culling magnitude bits')
    check(fogcolor(0x12345678)==vfogcolor(0x87654321)==density(0xabcd)==border(0x11223344)==0,'real scalar global setters')
    check(clamp(0xffffffff,0)==0 and stride(96)==0 and video.snapshot().texture_control&31==3,'clamp/stride native registers')
    check(clip(48,0,16,63)==0xdddd,'original bad clip status')
    for mode in range(4):
        colors=(U*1024)(*[rng.getrandbits(32) for _ in range(1024)]);check(palette_mode(mode)==0 and palette_data(colors)==0,'real SDK full palette')
        s=video.snapshot();out=(U*1024)();check(video.palette(C.byref(s),out)==0,'palette conversion')
        for i,c in enumerate(colors):
            five=lambda v:int((format(v,'05b')*2)[:8],2);six=lambda v:int((format(v,'06b')*2)[:8],2)
            if mode==0:want=((255 if c&0x8000 else 0)<<24)|(five((c>>10)&31)<<16)|(five((c>>5)&31)<<8)|five(c&31)
            elif mode==1:want=0xff000000|(five((c>>11)&31)<<16)|(six((c>>5)&63)<<8)|five(c&31)
            elif mode==2:want=(((c>>12)&15)*17<<24)|(((c>>8)&15)*17<<16)|(((c>>4)&15)*17<<8)|((c&15)*17)
            else:want=c
            check(out[i]==want,'independent palette channel conversion')
    old=video.snapshot();patch=(U*1024)(*[0x11223344]*1024);check(palette_bank(0,1025,patch)==0,'real SDK range palette');s=video.snapshot()
    check(list(s.palette[:1023])==[0x11223344]*1023 and s.palette[1023]==old.palette[1023],'preserved SDK palette final-entry exclusion')
    groups['sixteen_actual_SDK_setters_and_RZ_value_contracts']=checks
    def background_color(color):
        vals=[(color>>shift&255)/255 for shift in [24,16,8,0]];context.face=(F*4)(*vals);context.offset=(F*4)(0,0,0,0);check(background(C.byref(context))==0,'actual clear color')
    # A PAL8 surface whose palette is supplied only by captured frame state.
    info=Info();s=info.texsurface;s.Type=0x10000;s.BitDepth=1;s.PixelFormat=6<<27;s.nWidth=s.nHeight=8;s.TextureSize=64;s.fSurfaceFlags=0x24
    check(runtime.copy(C.byref(info),C.create_string_buffer(bytes([7]*64)),64)==0,'real PAL8 surface')
    names=(Name*1)();runtime.setname(C.byref(names[0]),None,1000,0);runtime.entry(field(names[0]),C.byref(info),1000,2,0xffffffff);runtime.param(runtime.resolve(names[0].texaddr));tcw=Record.from_address(runtime.resolve(names[0].texaddr)).texparambuffer
    listing=List(names,1);check(runtime.select(C.byref(listing))==1,'real list selection');keep.extend([info,names,listing])
    check(palette_mode(3)==0 and cull(F(0))==0 and sort(0)==0 and clip(16,16,47,47)==0 and stride(32)==0,'supported bound state')
    colors=(U*1024)(*[0xff00ff00]*1024);check(palette_data(colors)==0,'green palette');background_color(0xff0000ff)
    def record():
        check(reset()==0 and reserve(0,4)==0 and ctx(0,(U*4)(0x8000000a,0xe0800000,0x20800000,tcw))==0 and pin(tcw&0x1fffff)==0,'real snapshot batch bound')
        get();ptr=start();check(bool(ptr),'SDK frame header');vertices=[]
        for i,(x,y) in enumerate([(-64,-64),(192,-64),(-64,192)]):vertices += [0xf0000000 if i==2 else 0xe0000000,fw(x),fw(y),fw(1),fw(0.25),fw(0.25),0xffff0000,0]
        raw=struct.pack('<24I',*vertices);C.memmove(ptr,raw,len(raw));end(ptr+len(raw));put();check(qstatus()==0,'real frame batch complete');q=Q();check(submit(C.byref(q))==0,'atomic stateful frame submit');return q.value
    first=record();first_state=video.frame_snapshot(first);check(first_state.palette[7]==0xff00ff00 and first_state.clear_argb==0xff0000ff,'captured actual state')
    colors[7]=0xffffff00;check(palette_data(colors)==0 and clip(0,0,31,63)==0,'modify later palette/clip');background_color(0xff202020);second=record();second_state=video.frame_snapshot(second)
    check(second_state.generation>first_state.generation and bytes(video.frame_snapshot(first))==bytes(first_state),'older palette/fog/clip state immutable')
    def image(fid,w=64,h=64,off=False):
        nonlocal checks,images,pixels
        cfg=Config.make(w,h,textures=0 if off else 1);out=C.create_string_buffer(w*h*4);report=Report();e=Failure();rc=bound(gpu.handle,fid,C.byref(cfg),out,len(out),C.byref(report),C.byref(e));check(rc==0,('bound frame GPU',rc,e.batch,e.hresult))
        snap=video.frame_snapshot(fid);l=snap.clip_x&65535;r=snap.clip_x>>16;t=snap.clip_y&65535;bot=snap.clip_y>>16
        clear=bytes([(snap.clear_argb>>shift)&255 for shift in [16,8,0,24]]);color=bytes([255,0,0,255]) if off else bytes([(snap.palette[7]>>shift)&255 for shift in [16,8,0,24]])
        for y in range(h):
            for x in range(w):
                inside=l*w//64<=x<((r+1)*w+63)//64 and t*h//64<=y<((bot+1)*h+63)//64
                got=out.raw[(y*w+x)*4:(y*w+x+1)*4];want=color if inside else clear;check(max(abs(v-u) for v,u in zip(got,want))<=1,('captured-state pixel',x,y,list(got),list(want)));pixels+=1
        if off:check(report.texture_uploads==report.decoded_texels==report.shader_texture_bindings==report.shader_sampler_bindings==0,'bound Off preserves no-texture contract')
        images+=1;return out.raw,cfg,report
    before,cfg,report=image(first);image(second);image(first,128,128);image(first,off=True)
    runtime.shutdown();video.shutdown();check(before==image(first)[0],'retained state and texture survive both owners shutdown')
    # Restore explicit native device state for the remaining lifecycle probes;
    # the earlier frames remain independent of the new owner generation.
    check(video.init(64,64,0xff000000,1)==0,'new live render state');image(second);check(release(second)==0,'out of order state completion')
    cfg.budget=report.accounted_payload_bytes;out=C.create_string_buffer(64*64*4);rr=Report();e=Failure();check(bound(gpu.handle,first,C.byref(cfg),out,len(out),C.byref(rr),C.byref(e))==0,'exact bound-state payload budget')
    cfg.budget-=1;C.memset(out,0x5a,len(out));old_report=bytes(rr);check(bound(gpu.handle,first,C.byref(cfg),out,len(out),C.byref(rr),C.byref(e))!=0 and out.raw==b'Z'*len(out) and bytes(rr)==old_report,'bound failure preserves image/report')
    groups['captured_palette_clip_clear_and_owner_lifetimes']=checks-sum(groups.values())
    # Real swapchain upload and readback are compared before flip-discard.
    window_open=bind(gl,'jp_window_open',I,P,U,U,I,P,P);window_close=bind(gl,'jp_window_close',None,P);pump=bind(gl,'jp_window_pump',I,P)
    resize=bind(gl,'jp_window_resize',I,P,U,U,P);upload=bind(gl,'jp_window_upload',I,P,P,S,P);readback=bind(gl,'jp_window_readback',I,P,P,S,P)
    present=bind(gl,'jp_window_present',I,P,U,P);count=bind(gl,'jp_window_presented',Q,P);draw=bind(gl,'jp_window_draw_frame',I,P,Q,P,U,P,P)
    window=P();check(window_open(gpu.handle,128,128,1,C.byref(window),C.byref(e))==0,'real visible Win32/DXGI window');check(pump(window)==1,'message pump')
    for size in [128,192,96]:
        check(resize(window,size,size,C.byref(e))==0 and pump(window)==1,'native window/swapchain resize');data,c,rep=image(first,size,size);buffer=C.create_string_buffer(data)
        check(upload(window,buffer,len(data),C.byref(e))==0,'swapchain upload');copied=C.create_string_buffer(len(data));check(readback(window,copied,len(data),C.byref(e))==0 and copied.raw==data,'actual backbuffer pixels exact')
        rc=present(window,1,C.byref(e));check(rc==0,('real visible Present',rc,e.hresult));presented+=1;check(count(window)==presented,'successful Present counter')
    c=Config.make();c.budget=32<<20;check(draw(window,first,C.byref(c),1,C.byref(rr),C.byref(e))==0,'actual bound-frame window pipeline');presented+=1;check(count(window)==presented,'bound Present counted')
    # GPU shutdown is deferred until its last real window releases the device.
    gpu.close(gpu.handle);check(pump(window)==0,'closing renderer rejects further window work');window_close(window);gpu.handle=None
    check(release(first)==0,'last retained frame release');qs();check(fs()==0,'all stateful frame resources released');video.shutdown()
    groups['visible_swapchain_pixels_resize_Present_and_device_lifetime']=checks-sum(groups.values())
    result=dict(status='stateful_frame_rendering_and_native_window_verified',checks=checks,bound_images_compared=images,bound_pixels_compared=pixels,visible_successful_Presents=presented,swapchain_sizes=[128,192,96],
        groups=groups,actual_SDK_setting_routines=16,immutable_palette_fog_clip_clear_snapshots_verified=True,state_texture_shutdown_reinitialization_survived=True,
        real_backbuffer_upload_readback_verified=True,renderer_shutdown_deferred_until_windows_close=True,original_game_producers_modified=False,
        build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),support_sha256=sha(Path(__file__).parent/'native_video_test_support.py'),
        Kamui_render_flip_scheduling_implemented=False,real_game_frame_loop_executed=False,fog_rendering_implemented=False,gameplay_verified=False,playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
