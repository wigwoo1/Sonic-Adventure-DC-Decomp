"""Actual D3D11 fog pixels against a separate screen-space/manual oracle.

Synthetic geometry/assets only. Native floating interpolation and RGBA8
comparison are qualified here, not console dithering/fixed-point precision.
"""
import argparse,ctypes as C,hashlib,json,math,random,struct
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,Info,Name,List,Record,field
from native_gpu_test_support import GPU,Config,Report,Failure
from native_gpu_jobs_test_support import Jobs
from native_video_test_support import Video
from capture_native_boot_progress import png
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64;F=C.c_float
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def fw(v):return struct.unpack('<I',struct.pack('<f',v))[0]
def rgba(v):return [(v>>n&255)/255 for n in [16,8,0,24]]
def clamp(v):return max(0,min(1,v))

def lookup(q,density,table):
    # Independent arithmetic implementation of the manual's grid. It does
    # not share the shader's IEEE-bit decomposition or code generator.
    exponent=density&255;exponent-=256 if exponent>=128 else 0
    value=math.ldexp(q*((density>>8)&255)/128,exponent)
    value=max(1,min(math.nextafter(256.,0.),value))
    power=math.floor(math.log2(value));step=math.ldexp(1.,power)/16
    cell=min(15,int(value/step)-16);index=power*16+cell
    fraction=(value-(16+cell)*step)/step;word=table[index]
    return ((word>>8&255)*(1-fraction)+(word&255)*fraction)/255

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();out=a.output.resolve();b=json.loads((d/'build_verification.json').read_bytes())
    assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();out.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(d/n)==h,n
    assert sha(b['GPU_DLL'])==b['GPU_DLL_sha256'] and sha(b['frames_DLL'])==b['frames_DLL_sha256']
    runtime=Runtime(Path(b['frames_DLL']));lib=runtime.dll;video=Video(lib);gpu=GPU(Path(b['GPU_DLL']));gl=gpu.lib;checks=images=pixels=0;worst=0;groups={};shots=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q)
    reserve=bind(lib,'jp_queue_reserve',I,U,U);ctx=bind(lib,'jp_queue_set_context',I,U,P);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None);end=bind(lib,'njEndVertex',None,P);pin=bind(lib,'jp_frames_prepare_texture',I,U)
    start=bind(lib,'njStartVertexTexTrans',P);nontex=bind(lib,'njStartVertexNonTexTrans',P)
    bound=bind(gl,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P)
    check(qi(16<<20)==0 and fi(16,32,16<<20)==0 and runtime.init(16,128,16<<20)==0 and video.init(64,64,0xff102030,1)==0,'actual shared owners initialized')
    info=Info();surface=info.texsurface;surface.Type=0x10000;surface.BitDepth=1;surface.PixelFormat=1<<27;surface.nWidth=surface.nHeight=8;surface.fSurfaceFlags=4;surface.TextureSize=128
    check(runtime.copy(C.byref(info),C.create_string_buffer(struct.pack('<H',0x07e0)*64),128)==0,'real authored constant-green RGB565 surface')
    names=(Name*1)();runtime.setname(C.byref(names[0]),None,7000,0);runtime.entry(field(names[0]),C.byref(info),7000,2,0xffffffff);address=runtime.resolve(names[0].texaddr);runtime.param(address);texture=Record.from_address(address).texparambuffer;listing=List(names,1)
    check(runtime.select(C.byref(listing))==1 and runtime.status()==0,'actual SDK texture binding')
    bad_info=Info();bad_surface=bad_info.texsurface;bad_surface.Type=0x10000;bad_surface.BitDepth=1;bad_surface.PixelFormat=3<<27;bad_surface.nWidth=bad_surface.nHeight=8;bad_surface.fSurfaceFlags=4;bad_surface.TextureSize=1
    check(runtime.copy(C.byref(bad_info),C.create_string_buffer(b'X'),1)==0,'owned but undecodable encoded texture fixture')
    bad_names=(Name*1)();runtime.setname(C.byref(bad_names[0]),None,7001,0);runtime.entry(field(bad_names[0]),C.byref(bad_info),7001,2,0xffffffff);bad_address=runtime.resolve(bad_names[0].texaddr);runtime.param(bad_address);bad_texture=Record.from_address(bad_address).texparambuffer
    colors=[0x99406080]*3;offsets=[0]*3;packed=[((255-i*7)%256)<<8|((17+i*13)%256) for i in range(128)]
    def set_state(density=0x8000,table=packed,color=0x20d040b0,vertex=0xff2080e0):
        for i,value in enumerate(table):check(video.write(0x200+i*4,value)==0,'actual table register')
        for reg,value in [(0xb0,color),(0xb4,vertex),(0xb8,density)]:check(video.write(reg,value)==0,'actual fog global register')
    set_state()
    def record(mode=0,z=(1,1,1),base=colors,off=offsets,textured=False,offset=False,smooth=True,blend=(1,0),tcw=texture):
        check(reset()==0 and reserve(1,4)==0,'real triangle producer reservation')
        pcw=0x82000000|(2 if smooth else 0)|(8 if textured else 0)|(4 if offset else 0)
        tsp=(blend[0]<<29)|(blend[1]<<26)|(mode<<22)|0x100000|0xc0
        check(ctx(0,(U*4)(pcw,0xe0800000,tsp,tcw))==0,'real polygon context')
        if textured:check(pin(tcw&0x1fffff)==0,'actual texture lifetime pin')
        get();pointer=(start if textured else nontex)();check(bool(pointer),'actual SDK triangle header')
        raw=[]
        for i,(x,y) in enumerate([(-64,-64),(192,-64),(-64,192)]):raw += [0xf0000000 if i==2 else 0xe0000000,fw(x),fw(y),fw(z[i]),fw(.25),fw(.25),base[i],off[i]]
        C.memmove(pointer,struct.pack('<24I',*raw),96);end(pointer+96);put();fid=Q();check(submit(C.byref(fid))==0,'immutable actual frame submit')
        return fid.value,dict(mode=mode,z=tuple(C.c_float(q).value for q in z),base=list(base),off=list(off),textured=textured,offset=offset,smooth=smooth,blend=blend,snapshot=video.frame_snapshot(fid.value))
    def oracle(case,cfg):
        result=bytearray();state=case['snapshot'];base=[rgba(v) for v in case['base']];offset=[rgba(v) for v in case['off']];clear=rgba(state.clear_argb)
        for y in range(cfg.height):
            for x in range(cfg.width):
                sx=(x+.5)*64/cfg.width;sy=(y+.5)*64/cfg.height;weights=[1-(sx+64)/256-(sy+64)/256,(sx+64)/256,(sy+64)/256]
                q=sum(w*z for w,z in zip(weights,case['z']));cq=[z if z>0 else 1 for z in case['z']];den=sum(w*z for w,z in zip(weights,cq))
                interp=[w*z/den for w,z in zip(weights,cq)] if case['smooth'] else [int(i==cfg.flat_color_vertex) for i in range(3)]
                c=[clamp(sum(interp[j]*base[j][i] for j in range(3))) for i in range(4)]
                o=[clamp(sum(interp[j]*offset[j][i] for j in range(3))) for i in range(4)] if case['offset'] and case['textured'] else [0]*4
                if case['textured'] and cfg.textures:c=[0,c[1],0,c[3]]
                if case['offset']:c[:3]=[c[i]+o[i] for i in range(3)]
                if case['mode'] in [0,3]:
                    f=lookup(q,state.fog_density,state.fog_table);fog=rgba(state.fog_color)
                    if case['mode']==3:c=fog[:3]+[f]
                    else:c[:3]=[c[i]*(1-f)+fog[i]*f for i in range(3)]
                elif case['mode']==1 and case['offset']:
                    fog=rgba(state.vertex_fog_color);c[:3]=[c[i]*(1-o[3])+fog[i]*o[3] for i in range(3)]
                c=list(map(clamp,c));sf,df=case['blend']
                if (sf,df)==(4,5):c=[c[i]*c[3]+clear[i]*(1-c[3]) for i in range(4)]
                else:assert (sf,df)==(1,0)
                result.extend(round(clamp(v)*255) for v in c)
        return bytes(result)
    def compare(actual,expected,label):
        nonlocal pixels,worst
        delta=max(abs(x-y) for x,y in zip(actual,expected));worst=max(worst,delta);check(len(actual)==len(expected) and delta<=2,(label,delta));pixels+=len(actual)//4
    def render(fid,case,cfg=None):
        nonlocal images
        cfg=cfg or Config.make(8,8);buffer=C.create_string_buffer(cfg.width*cfg.height*4);rep=Report();err=Failure()
        check(bound(gpu.handle,fid,C.byref(cfg),buffer,len(buffer),C.byref(rep),C.byref(err))==0,('actual D3D fog',err.code,err.batch,err.hresult));images+=1
        compare(buffer.raw,oracle(case,cfg),'independent full-image fog oracle')
        if not cfg.textures:check(rep.texture_uploads==rep.decoded_texels==rep.shader_texture_bindings==rep.shader_sampler_bindings==0,'Off compiles zero texture/sampler bindings and performs no upload/decode')
        return buffer.raw,cfg,rep
    # Every table cell at its lower bound and interior midpoint, plus both
    # clamps, using independently specified high/low bytes in each word.
    for index in range(128):
        power=math.ldexp(1,index//16);lower=power*(1+(index%16)/16)
        for value in [lower,lower+power/32]:
            fid,case=record(z=(value,)*3);render(fid,case);check(release(fid)==0,'table-grid frame release')
    for value in [0,.0001,.999,255.999,256,1e20]:
        fid,case=record(z=(value,)*3);render(fid,case);check(release(fid)==0,'table-clamp frame release')
    groups['all_128_table_cells_pairs_interpolation_and_clamps']=checks
    for density in [0,0x0100,0xff07,0x8080,0x807f,0xff80,0x000080ff,0xffff8000]:
        set_state(density=density)
        for value in [1e-20,.01,2,1e20]:
            fid,case=record(z=(value,)*3);render(fid,case);check(release(fid)==0,'density-domain frame release')
    groups['signed_density_exponent_and_mantissa_extremes']=checks-sum(groups.values())
    set_state(density=0x8005)
    for mode in range(4):
        for enabled in [0,1]:
            for smooth in [True,False]:
                fid,case=record(mode,z=(.125,2,8),base=[0x40102080,0x8020a040,0xe0c06020],off=[0x00201008,0x80201008,0xff201008],textured=True,offset=True,smooth=smooth)
                cfg=Config.make(32,32,enabled);cfg.flat_color_vertex=2;render(fid,case,cfg);check(release(fid)==0,'shading-domain frame release')
    # No Offset flag means vertex fog is disabled, even with a nonzero word.
    fid,case=record(1,off=[0xffffffff]*3);render(fid,case);check(release(fid)==0,'vertex-no-offset frame release')
    for mode in [0,1,3]:
        fid,case=record(mode,z=(.125,.75,2),base=[0x80604020]*3,off=[0x40802010]*3,textured=True,offset=True,blend=(4,5));render(fid,case,Config.make(32,32));check(release(fid)==0,'fog-before-blend frame release')
    # Ownership is mandatory at frame production. Off still must not decode
    # a retained encoded payload which the texture path cannot render.
    fid,case=record(0,z=(.125,.75,2),textured=True,tcw=bad_texture);render(fid,case,Config.make(16,16,0))
    cfg=Config.make(16,16,1);buffer=C.create_string_buffer(16*16*4);C.memset(buffer,0x5a,len(buffer));bad_report=Report();saved=bytes(bad_report);err=Failure()
    check(bound(gpu.handle,fid,C.byref(cfg),buffer,len(buffer),C.byref(bad_report),C.byref(err))!=0 and buffer.raw==b'Z'*len(buffer) and bytes(bad_report)==saved,'texture On rejects undecodable payload and preserves output/report');check(release(fid)==0,'Off undecodable-texture frame release')
    groups['all_four_modes_texture_Off_perspective_flat_and_blending']=checks-sum(groups.values())
    fid,case=record(0,z=(.125,.75,2));first=video.frame_snapshot(fid);set_state(density=0x807f,table=[0]*128,color=0xff000000)
    check(bytes(video.frame_snapshot(fid))==bytes(first),'captured fog registers remain immutable after all later setters');data,cfg,rep=render(fid,case,Config.make(32,32))
    exact=rep.accounted_payload_bytes;cfg.budget=exact;render(fid,case,cfg);cfg.budget=exact-1;buffer=C.create_string_buffer(32*32*4);C.memset(buffer,0x5a,len(buffer));saved=bytes(rep);err=Failure()
    check(bound(gpu.handle,fid,C.byref(cfg),buffer,len(buffer),C.byref(rep),C.byref(err))!=0 and buffer.raw==b'Z'*len(buffer) and bytes(rep)==saved,'one-byte-short budget preserves output/report')
    cfg.budget=64<<20;raw_report=Report();check(gpu.draw(gpu.handle,fid,C.byref(cfg),buffer,len(buffer),C.byref(raw_report),C.byref(err))==5,'unbound legacy path rejects fog without captured state')
    check(release(fid)==0,'immutable frame release');groups['capture_binding_budget_and_failure_preservation']=checks-sum(groups.values())
    # Actual original generator/setters feed rendered frames, then GPU-only
    # window and asynchronous job paths must produce the same pixels.
    generated=(F*128)();bind(lib,'njGenerateFogTable',None,P)(generated);bind(lib,'njSetFogTable',None,P)(generated);bind(lib,'njSetFogDensity',None,U)(0x8007);bind(lib,'njSetFogColor',None,U)(0xffc0d8ff)
    window=P();open_window=bind(gl,'jp_window_open',I,P,U,U,I,P,P);close_window=bind(gl,'jp_window_close',None,P);direct=bind(gl,'jp_window_render_frame',I,P,Q,P,P,P);readback=bind(gl,'jp_window_readback',I,P,P,S,P);present=bind(gl,'jp_window_present',I,P,U,P);presented=bind(gl,'jp_window_presented',Q,P)
    check(open_window(gpu.handle,256,256,1,C.byref(window),C.byref(err))==0,'actual native Win32/DXGI window')
    kept=None
    for mode,label in [(2,'without-fog'),(0,'table-fog')]:
        fid,case=record(mode,z=(.002,.02,.1),base=[0xff306018,0xffa03020,0xff204080]);data,cfg,rep=render(fid,case,Config.make(256,256,0))
        check(direct(window,fid,C.byref(cfg),C.byref(rep),C.byref(err))==0,'GPU-only captured-state render to actual backbuffer');copy=C.create_string_buffer(len(data));check(readback(window,copy,len(copy),C.byref(err))==0 and copy.raw==data,'real window backbuffer equals independently checked image')
        path=out/(label+'.png');png(path,copy.raw,256,256);shots.append(dict(label=label,path=str(path),sha256=sha(path),actual_window_backbuffer=True,synthetic_rendering_test=True,gameplay=False))
        check(present(window,0,C.byref(err))==0,'real fog test Present')
        if kept:check(release(kept[0])==0,'comparison frame release')
        kept=(fid,case,data,cfg)
    fid,case,data,cfg=kept;jobs=Jobs(gpu);job=Q();rep=Report();check(jobs.submit(gpu.handle,fid,C.byref(cfg),C.byref(job),C.byref(rep),C.byref(err))==0,'actual asynchronous fog job submission')
    check(release(fid)==0,'retire source frame immediately after actual GPU submission');set_state(density=0x8000,table=[0]*128,color=0xff000000)
    timing,pending=jobs.wait(job.value);check(jobs.copy(window,job.value,C.byref(err))==0,'completed fog job GPU-only copy');copy=C.create_string_buffer(len(data));check(readback(window,copy,len(copy),C.byref(err))==0 and copy.raw==data,'asynchronous result keeps captured fog after source/frame retirement')
    check(jobs.present(window,job.value,0,C.byref(err))==0 and presented(window)==3,'real asynchronous Present and counter');check(jobs.release(gpu.handle,job.value,C.byref(err))==0,'actual completed job release')
    close_window(window);gpu.close(gpu.handle);check(fs()==0,'frame owner released');runtime.shutdown();qs();video.shutdown();groups['original_SDK_to_actual_backbuffers_and_async_jobs']=checks-sum(groups.values())
    report=dict(status='actual_GPU_fog_modes_and_captured_state_pixels_verified',checks=checks,images_compared=images,pixels_compared=pixels,maximum_RGBA8_channel_error=worst,tolerance_RGBA8_channels=2,groups=groups,all_128_table_cells_and_midpoints_tested=True,signed_density_exponent_and_extremes_tested=True,normal_table_vertex_none_and_table_mode2_tested=True,Texture_Off_zero_texture_and_sampler_bindings=True,Texture_Off_undecodable_owned_texture_payload_not_decoded=True,real_original_generator_setters_used=True,immutable_frame_fog_pixels_verified=True,direct_backbuffer_pixels_verified=True,async_job_survives_source_frame_release=True,actual_Presents=3,actual_window_screenshots=shots,screenshots_are_synthetic_renderer_tests=True,native_float_policy=True,console_fixed_point_dithering_or_timing_verified=False,full_game_loop_or_gameplay_verified=False,build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),shader_header_sha256=sha(r/'outputs/sonic-adventure/native/jp_native_gpu_fog_shader.h'),playable=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
