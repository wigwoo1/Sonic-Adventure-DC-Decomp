"""Real frame consumption, independent native-float pixel oracle and Off proof.
These are synthetic assets sent through actual SDK/model/sprite/strip providers;
not a playable game, console capture comparison or complete frame-loop proof.
"""
import argparse,ctypes as C,hashlib,json,math,struct,zlib
from pathlib import Path
from native_gpu_test_support import GPU,Config,Report,Failure
from native_packet_test_support import Decoder,Plan,Failure as PacketFailure,bind
from native_texture_test_support import Runtime,Info,Record,Name,List,field
from verify_native_model_preflight import Vec,Material,Mesh,Model,Span,Bound
from verify_native_sprites import Point,Frame,Sprite
U=C.c_uint32;I=C.c_int;F=C.c_float;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def fw(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def pack(w):return struct.pack('<'+'I'*len(w),*w)
def rgba(c):return [((c>>s)&255)/255 for s in [16,8,0,24]]
def clamp(x):return min(1,max(0,x))
def png(path,w,h,data):
    def chunk(n,d):return struct.pack('>I',len(d))+n+d+struct.pack('>I',zlib.crc32(n+d))
    path.write_bytes(b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR',struct.pack('>2I5B',w,h,8,6,0,0,0))+chunk(b'IDAT',zlib.compress(b''.join(b'\0'+data[y*w*4:(y+1)*w*4] for y in range(h)),9))+chunk(b'IEND',b''))
def sample(tex,u,v,tsp):
    w,h,data=tex
    def index(i,n,clamped,mirrored):
        if clamped:return min(n-1,max(0,i))
        if mirrored:
            j=i%(n*2);return j if j<n else n*2-1-j
        return i%n
    def pixel(x,y):return data[index(y,h,tsp&0x8000,tsp&0x20000)*w+index(x,w,tsp&0x10000,tsp&0x40000)]
    if not tsp&0x6000:return pixel(math.floor(u*w),math.floor(v*h))
    x=u*w-0.5;y=v*h-0.5;ix=math.floor(x);iy=math.floor(y);dx=x-ix;dy=y-iy
    p=[pixel(ix,iy),pixel(ix+1,iy),pixel(ix,iy+1),pixel(ix+1,iy+1)];weights=[(1-dx)*(1-dy),dx*(1-dy),(1-dx)*dy,dx*dy]
    return [sum(weights[j]*p[j][i] for j in range(4)) for i in range(4)]
def expected(plan,cfg,x,y,textures):
    # Independent barycentric renderer. Skip points within a small edge margin
    # rather than claiming a physical PVR/D3D fixed-point coverage match.
    dest=[round(clamp(v)*255)/255 for v in cfg.clear_rgba];z=0;uncertain=False
    maximum=max([1.]+[plan.vertices[i].inv_w for i in range(plan.vertex_count)]);scale=math.ldexp(1,-math.frexp(maximum)[1])
    px=(x+0.5)*cfg.logical_width/cfg.width;py=(y+0.5)*cfg.logical_height/cfg.height
    for lane in [0,2,1]:
        for bi in range(plan.batch_count):
            b=plan.batches[bi]
            if b.list!=lane:continue
            pcw,isp,tsp,tcw=list(b.header)[:4]
            for ti in range(b.first_triangle,b.first_triangle+b.triangle_count):
                v=[plan.vertices[j] for j in plan.triangles[ti].indices];a,bv,cv=v
                area=(bv.x-a.x)*(cv.y-a.y)-(bv.y-a.y)*(cv.x-a.x)
                if abs(area)<1e-10:continue
                cull=(isp>>27)&3
                if cull==1 and abs(area)<cfg.cull_small_area or cull==2 and area<0 or cull==3 and area>0:continue
                weights=[((bv.x-px)*(cv.y-py)-(bv.y-py)*(cv.x-px))/area,((cv.x-px)*(a.y-py)-(cv.y-py)*(a.x-px))/area]
                weights.append(1-sum(weights))
                if min(weights)<-0.0001:continue
                if min(weights)<0.0001:uncertain=True;continue
                depth=sum(w*p.inv_w*scale for w,p in zip(weights,v));mode=(isp>>29)&7
                passed=[False,depth<z,depth==z,depth<=z,depth>z,depth!=z,depth>=z,True][mode]
                if not passed:continue
                q=[p.inv_w*scale if p.inv_w>0 else scale for p in v];div=sum(w*r for w,r in zip(weights,q))
                interpolation=[w*r/div for w,r in zip(weights,q)]
                colors=v if pcw&2 else [v[cfg.flat_color_vertex]]*3
                base=[sum(interpolation[j]*clamp(colors[j].base[i])*(clamp(colors[j].base_intensity) if i<3 else 1) for j in range(3)) for i in range(4)]
                offset=[sum(interpolation[j]*clamp(colors[j].offset[i])*(clamp(colors[j].offset_intensity) if i<3 else 1) for j in range(3)) for i in range(4)]
                if not tsp&0x100000:base[3]=1
                color=base.copy()
                if cfg.textures and pcw&8:
                    u=sum(interpolation[j]*v[j].u for j in range(3));vv=sum(interpolation[j]*v[j].v for j in range(3));t=sample(textures[tcw&0x1fffff],u,vv,tsp)
                    if not tsp&0x6000:
                        tw,th,_=textures[tcw&0x1fffff]
                        if min(abs(u*tw-round(u*tw)),abs(vv*th-round(vv*th)))<0.0001:uncertain=True
                    if tsp&0x80000:t=t[:3]+[1.]
                    mode=(tsp>>6)&3
                    if mode==0:color=t
                    elif mode==1:color=[base[i]*t[i] for i in range(3)]+[t[3]]
                    elif mode==2:color=[base[i]*(1-t[3])+t[i]*t[3] for i in range(3)]+[base[3]]
                    else:color=[base[i]*t[i] for i in range(4)]
                if pcw&4:color=[color[i]+offset[i] for i in range(3)]+[color[3]]
                color=list(map(clamp,color))
                if lane==2 and color[3]<cfg.punch_alpha:continue
                def blend_factor(n,source,i):
                    other=dest[i] if source else color[i]
                    return [0,1,other,1-other,color[3],1-color[3],dest[3],1-dest[3]][n]
                sf=(tsp>>29)&7;df=(tsp>>26)&7
                dest=[round(clamp(color[i]*blend_factor(sf,True,i)+dest[i]*blend_factor(df,False,i))*255)/255 for i in range(4)]
                if not isp&0x04000000:z=depth
    return bytes(round(clamp(v)*255) for v in dest),uncertain
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();assert not a.report.exists()
    b=json.loads((d/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(Path(b['DLL']))==b['DLL_sha256'] and sha(Path(b['frames_DLL']))==b['frames_DLL_sha256']
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;gpu=GPU(Path(b['DLL']));decoder=Decoder(Path(b['DLL']))
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);ctx=bind(lib,'jp_queue_set_context',I,U,P)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,C.POINTER(Q));release=bind(lib,'jp_frame_release',I,Q)
    pin=bind(lib,'jp_frames_prepare_texture',I,U);reserve=bind(lib,'jp_queue_reserve',I,U,U);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None);end=bind(lib,'njEndVertex',None,P)
    qstatus=bind(lib,'jp_queue_status',I);fstatus=bind(lib,'jp_frames_status',I);surface=bind(lib,'jp_frame_surface',P,Q,U,C.POINTER(S))
    starts={n:bind(lib,n,P,*([P,P] if n.endswith('H') else [P] if 'Intens' in n else [])) for n in ['njStartVertexNonTexOpaque','njStartVertexNonTexTrans','njStartVertexTexOpaque','njStartVertexTexTrans','njStartVertexTexPunch','njStartVertexTexOpaqueIntens','njStartVertexTexOpaqueIntensH']}
    checks=images=compared=actual_parents=0;groups={};keep=[];texture_map={};live_frames=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    check(qi(64<<20)==0 and fi(16,32,64<<20)==0 and t.init(32,128,16<<20)==0,'real queue/frame/texture owners')
    def texture(fmt=1,pattern=True,short=False,mip=False):
        n=len(keep);info=Info();s=info.texsurface;s.Type=0x10000;s.BitDepth=1;s.PixelFormat=fmt<<27;s.nWidth=s.nHeight=8;s.fSurfaceFlags=4|(1 if mip else 0)
        values=[];pixels=[]
        for y in range(8):
            for x in range(8):
                if fmt==1:
                    value=([0xf800,0x07e0,0x001f,0xffff][(x//2+y//2)%4] if pattern else 0x07e0);rr=(value>>11)&31;gg=(value>>5)&63;bb=value&31;color=[((rr<<3)|(rr>>2))/255,((gg<<2)|(gg>>4))/255,((bb<<3)|(bb>>2))/255,1]
                elif fmt==2:
                    value=[0xf248,0x842f,0x42f4,0x0248][(x//2+y//2)%4];color=[((value>>8)&15)/15,((value>>4)&15)/15,(value&15)/15,(value>>12)/15]
                else:value=0;color=[0,0,0,1]
                values.append(value);pixels.append(color)
        # Independent recursive encoded ordering comes from texture verifier.
        from verify_native_texels import positions
        raw=struct.pack('<64H',*(values[y*8+x] for x,y in positions(8,8)))
        if mip:
            raw=bytearray(176)
            for side,offset in [(1,6),(2,8),(4,16),(8,48)]:raw[offset:offset+side*side*2]=struct.pack('<H',0x07e0)*(side*side)
            pixels=[[0,1,0,1]]*64
        if short:raw=b'\x01'
        s.TextureSize=len(raw);check(t.copy(C.byref(info),C.create_string_buffer(bytes(raw)),len(raw))==0,'actual owned encoded surface')
        names=(Name*1)();name=names[0];t.setname(C.byref(name),None,1000+n,0);t.entry(field(name),C.byref(info),1000+n,2,0xffffffff);address=t.resolve(name.texaddr);t.param(address)
        check(t.status()==0 and bool(address),'real SDK entry/parameter');record=Record.from_address(address);word=record.texparambuffer&0x1fffff
        listing=List(names,1);keep.append((info,name,names,listing));check(t.select(C.byref(listing))==1,'SDK selection')
        texture_map[word]=(8,8,pixels);return record.texparambuffer
    tcw=texture();alpha_tcw=texture(2);bad_tcw=texture(3,short=True);mip_tcw=texture(pattern=False,mip=True)
    triangle=[(8,8,0.5,0,0,0xff804020),(56,8,0.25,1,0,0xff408020),(8,56,0.75,0,1,0xff204080)]
    def emit(vertices=triangle,lane=0,textured=True,tcw=tcw,tsp=0x208000c0,isp=0xe0800000,offset=False,reverse=False):
        check(reserve(lane,1+len(vertices))==0,'whole producer bound');pcw=0x80000000|([0,2,4][lane]<<24)|2|(8 if textured else 0)|(4 if offset else 0)
        check(ctx(0,(U*4)(pcw,isp,tsp,tcw))==0,'actual context');
        if textured:check(pin(tcw&0x1fffff)==0,'resource preflight')
        get();name='njStartVertexTexPunch' if lane==2 else 'njStartVertex'+('Tex' if textured else 'NonTex')+('Trans' if lane else 'Opaque');ptr=starts[name]();check(bool(ptr),'real SDK header')
        words=[];verts=list(vertices)
        if reverse:verts[0],verts[1]=verts[1],verts[0]
        for j,(x,y,q,u,v,col) in enumerate(verts):words.extend([0xf0000000 if j==len(verts)-1 else 0xe0000000,fw(x),fw(y),fw(q),fw(u),fw(v),col,0x00204060 if offset else 0])
        raw=pack(words);C.memmove(ptr,raw,len(raw));end(ptr+len(raw));put();check(qstatus()==fstatus()==0,'real batch completion')
    def capture():
        fid=Q();check(submit(C.byref(fid))==0,'real owned frame');live_frames.append(fid.value);return fid.value
    def finish(fid):check(release(fid)==0,'source frame release');live_frames.remove(fid)
    def verify(fid,cfg=None,preview=None):
        nonlocal images,compared
        cfg=cfg or Config.make();rc,image,report,err=gpu.image(fid,cfg)
        if rc:
            probe=P();pe=PacketFailure()
            if decoder.frame(fid,64<<20,C.byref(probe),C.byref(pe))==0:
                pp=Plan.from_address(probe.value);print('GPU rejected material headers',[[hex(v) for v in pp.batches[k].header[:4]] for k in range(pp.batch_count)],flush=True);decoder.destroy(probe)
        check(rc==0,f'GPU draw {rc}/{err.batch}/{err.hresult}')
        check(report.width==cfg.width and report.height==cfg.height and report.feature_level==0xb000,'native hardware output dimensions/feature')
        if not cfg.textures:check(report.texture_uploads==report.decoded_texels==report.shader_texture_bindings==report.shader_sampler_bindings==0,'Off performs no texture decode/upload and compiled shader has no texture/sampler')
        out=P();pf=PacketFailure();check(decoder.frame(fid,64<<20,C.byref(out),C.byref(pf))==0,'qualified plan for independent pixel oracle');plan=Plan.from_address(out.value)
        for y in range(cfg.height):
            for x in range(cfg.width):
                want,edge=expected(plan,cfg,x,y,texture_map)
                if edge:continue
                got=image[(y*cfg.width+x)*4:(y*cfg.width+x+1)*4]
                check(max(abs(a-b) for a,b in zip(got,want))<=2,('native GPU/reference pixel',images,x,y,list(got),list(want)));compared+=1
        decoder.destroy(out);images+=1
        if preview:png(d/preview,cfg.width,cfg.height,image)
        return image,report
    # Perspective UV, Gouraud color, texture modes, mirror/clamp and bilinear.
    for shade in range(4):
        for linear in [False,True]:
            check(reset()==0,'material recording reset');verts=[(x,y,q,u*1.5-0.25,v*1.5-0.25,c) for x,y,q,u,v,c in triangle]
            emit(verts,tcw=alpha_tcw,tsp=0x20800000|(shade<<6)|(0x2000 if linear else 0)|0x40000,offset=shade%2==0);fid=capture()
            verify(fid);off=Config.make(textures=0);verify(fid,off);finish(fid)
    groups['perspective_material_sampling_and_Off']=checks
    # Both triangle orientations, all cull/depth encodings and native blend pairs.
    for cull in range(4):
        for reverse in [False,True]:
            check(reset()==0,'cull reset');emit(textured=False,isp=0xe0000000|(cull<<27),reverse=reverse);fid=capture();cfg=Config.make();cfg.cull_small_area=3000 if cull==1 else 0;verify(fid,cfg);finish(fid)
    for mode in range(8):
        check(reset()==0,'depth reset');emit(textured=False,isp=(mode<<29)|0x00800000);fid=capture();verify(fid);finish(fid)
    for sf,df in [(1,0),(4,5),(1,1),(2,3),(6,7),(3,2),(5,4)]:
        check(reset()==0,'blend reset');emit(textured=False);v=[(x,y,q,u,v,0x8040c080) for x,y,q,u,v,c in triangle]
        emit(v,lane=1,textured=False,tsp=(sf<<29)|(df<<26)|0x00900000,isp=0xe4000000);fid=capture();verify(fid);finish(fid)
    groups['cull_depth_and_blend']=checks-sum(groups.values())
    for alpha in [0x00,0x7f,0x80,0xff]:
        check(reset()==0,'punch reset');v=[(x,y,q,u,v,(alpha<<24)|0x00ffffff) for x,y,q,u,v,c in triangle];emit(v,lane=2,tcw=alpha_tcw,tsp=0x009000c0);fid=capture();verify(fid);finish(fid)
    check(reset()==0,'mip reset');v=[(x,y,q,0.2,0.2,0xffffffff) for x,y,q,u,v,c in triangle];emit(v,tcw=mip_tcw,tsp=0x208004c0);fid=capture();_,rep=verify(fid);check(rep.decoded_texels==85,'all four mip levels decoded/uploaded');finish(fid)
    groups['punch_and_mip_upload']=checks-sum(groups.values())
    # Failure is transactional; Off remains usable even for unsupported encoded
    # texture data. It still uses a real retained resource identity in the frame.
    check(reset()==0,'unsupported texture reset');emit(tcw=bad_tcw);fid=capture();off=Config.make(textures=0);verify(fid,off)
    config=Config.make();out=C.create_string_buffer(b'Z'*(64*64*4));report=Report();C.memset(C.byref(report),0x5a,C.sizeof(report));old_report=bytes(report);err=Failure()
    check(gpu.draw(gpu.handle,fid,C.byref(config),out,len(out),C.byref(report),C.byref(err))==6,'unsupported texture rejected')
    check(out.raw[:-1]==b'Z'*(64*64*4) and bytes(report)==old_report,'failed texture preserves pixels/report');finish(fid)
    check(reset()==0,'failure frame reset');emit();fid=capture()
    for mutation in ['budget','width','textures','logical','output']:
        c=Config.make();n=64*64*4
        if mutation=='budget':c.budget=128
        elif mutation=='width':c.width=8193
        elif mutation=='textures':c.textures=2
        elif mutation=='logical':c.logical_width=float('nan')
        else:n-=1
        check(gpu.draw(gpu.handle,fid,C.byref(c),out,n,C.byref(report),C.byref(err))!=0,'bad output/config/budget rejected')
        check(out.raw[:-1]==b'Z'*(64*64*4) and bytes(report)==old_report,'configuration failure atomicity')
    verify(fid);finish(fid);groups['transactional_errors_and_unsupported_texture_Off']=checks-sum(groups.values())
    # Complete actual model, 2D/3D sprite and flat-strip parents, unchanged.
    init=bind(lib,'jp_render_state_initialize',I,U,S);matrix=bind(lib,'njSetMatrix',None,P,P);mi=bind(lib,'draw_model_init',None);light=bind(lib,'draw_set_light',None,F,F,F)
    mb=bind(lib,'draw_models_begin',None);me=bind(lib,'draw_models_end',None);dm=bind(lib,'draw_model_unculled',None,P);measure=bind(lib,'jp_model_preflight',I,P,S,S,S,S,P,P);mr=bind(lib,'jp_model_reserve',I,P)
    draw2=bind(lib,'njDrawSprite2D',None,P,I,F,U);draw3=bind(lib,'njDrawSprite3D',None,P,I,U);flat=bind(lib,'njDrawTexture',None,P,I,I,I)
    frame=bind(lib,'jp_sprite_probe_frame',None,I,I);callback=bind(lib,'jp_sprite_probe_callback',None,I,I);check(init(2048,1<<20)==0,'real renderer scratch')
    # Select the first synthetic 8x8 texture and keep header/cache consistent.
    listing=keep[0][3];check(t.select(C.byref(listing))==1,'actual parent texture list')
    for iteration in range(8):
        check(reset()==0 and ctx(0,(U*4)(0x8000000a,0xe0800000,0x208000c0,tcw))==0 and pin(tcw&0x1fffff)==0,'actual parent recording')
        C.c_uint32.in_dll(lib,'_nj_current_texture').value=0xffffffff
        identity=(F*16)(*[1. if j%5==0 else 0. for j in range(16)]);matrix(None,identity);mi();light(0,0,1);frame(0,0);callback(0,1)
        U.in_dll(lib,'_nj_control_3d_flag_').value=0
        pts=(Vec*4)(Vec(-0.5,-0.5,2),Vec(0.5,-0.5,2),Vec(-0.5,0.5,0.5 if iteration%2 else 2),Vec(0.5,0.5,2));norms=(Vec*4)(*[Vec(0,0,1)]*4)
        indices=(C.c_int16*6)(0,1,2,2,1,3);uv=(C.c_int16*12)(0,0,255,0,0,255,0,255,255,0,255,255)
        meshes=(Mesh*1)(Mesh(0,2,indices,None,None,None,C.cast(uv,P)));materials=(Material*1)(Material(0xffffffff,0,1.,0,0x204000));spans=(Span*1)(Span(6,6))
        model=Model(pts,norms,4,meshes,materials,1,1,Vec(0,0,2),3);bound=Bound();measured=measure(C.byref(model),4,4,1,1,spans,C.byref(bound))
        if measured:print('Actual model preflight diagnostics',measured,'texture status',t.status(),'queue',qstatus(),'frames',fstatus(),flush=True)
        check(measured==0 and mr(C.byref(bound))==0,'actual model preflight');mb();dm(C.byref(model));me();actual_parents+=1
        anim=Frame(64,64,32,32,0,0,256,256,0,0);sprite=Sprite(Point(100,100,2),1,1,0,C.addressof(listing),C.addressof(anim));draw2(C.byref(sprite),0,F(0.5),0x10);sprite.p=Point(0,0,2);sprite.sx=sprite.sy=0.02;draw3(C.byref(sprite),0,0x10);actual_parents+=2
        vertices=(U*18)(fw(350),fw(100),fw(0.5),fw(0),fw(0),0xffffffff,fw(550),fw(100),fw(0.5),fw(1),fw(0),0xffffffff,fw(350),fw(300),fw(0.5),fw(0),fw(1),0xffffffff)
        # Original raw strip writer requires its whole bound before entry.
        check(reserve(0,4)==0,'actual strip bound');flat(vertices,3,1000,0);actual_parents+=1
        check(qstatus()==fstatus()==0,'actual complete producer status');fid=capture();cfg=Config.make(160,120);cfg.logical_width=640;cfg.logical_height=480
        verify(fid,cfg,'actual_producer_preview.png' if iteration==0 else None);cfg.textures=0;verify(fid,cfg);finish(fid)
    groups['complete_actual_parent_pixel_consumption']=checks-sum(groups.values())
    # Empty full-size 8K render target/readback (capability, not FPS/gameplay).
    check(reset()==0,'empty 8K recording');fid=capture();cfg=Config.make(7680,4320,textures=0);cfg.budget=1024<<20;cfg.clear_rgba=(F*4)(0.25,0.5,0.75,1)
    rc,image,rep,err=gpu.image(fid,cfg);check(rc==0 and rep.width==7680 and rep.height==4320,'real 8K target/readback');check(rep.triangles==rep.texture_uploads==0,'empty 8K statistics')
    for x,y in [(0,0),(7679,4319),(3840,2160),(127,2500)]:
        pixel=image[(y*7680+x)*4:(y*7680+x+1)*4]
        # D3D11.3 specification 3.2.3.6 permits 0.6 integer ULP for
        # float-to-UNORM. At 0.5*255, either 127 or 128 is permitted.
        check(all(abs(pixel[i]-cfg.clear_rgba[i]*255)<=0.60001 for i in range(4)),('8K sampled clear pixel',x,y,list(pixel)))
    del image;finish(fid);groups['8K_empty_target_capability']=checks-sum(groups.values())
    check(not live_frames,'all frame IDs retired');gpu.close(gpu.handle);qs();t.shutdown();check(fs()==0,'all owners cleanup')
    report=dict(status='native_offscreen_GPU_frame_consumer_verified',checks=checks,images_compared=images,interior_pixels_compared=compared,groups=groups,actual_complete_parent_draws=actual_parents,
        output_API='D3D11 hardware feature level 11_0',texture_Off_shader_reflection_has_no_texture_or_sampler_bindings=True,Off_skips_texture_payload_decode_and_upload=True,
        eight_K_empty_target_readback_verified=True,clear_UNORM_integer_ULP_tolerance=0.6,native_float_pixel_oracle_max_channel_error=2,coverage_and_point_sampling_boundary_margin_excluded=True,console_pixel_fidelity_verified=False,
        comparison_scope='synthetic textures and valid real producer captures; native float color, explicit first-flat convention, submission-order translucency',
        DLL_sha256=sha(Path(b['DLL'])),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),support_sha256=sha(Path(__file__).parent/'native_gpu_test_support.py'),
        original_game_producers_modified=False,full_game_frame_loop_executed=False,window_flip_or_vsync_implemented=False,gameplay_verified=False,playable=False,overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
if __name__=='__main__':main()
