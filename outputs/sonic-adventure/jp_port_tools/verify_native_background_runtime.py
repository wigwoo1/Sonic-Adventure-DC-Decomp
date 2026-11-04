"""Independent SDK packet, native background image and ownership checks."""
import argparse,ctypes as C,hashlib,json,math,struct
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,Record
from native_pvm_test_support import PVM,archive,texture,destination
from native_video_test_support import Video,Context
from native_gpu_test_support import GPU,Config,Report,Failure
from native_present_test_support import Present
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64;F=C.c_float;Callback=C.CFUNCTYPE(None,P)
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def fw(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def rgba(word):return ((word>>16)&255,(word>>8)&255,word&255,(word>>24)&255)
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();b=json.loads((d/'build_verification.json').read_text());assert not a.report.exists()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ['frames_DLL','GPU_DLL','DLL']:assert sha(b[n])==b[n+'_sha256']
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;v=Video(lib);pvm=PVM(lib);gpu=GPU(Path(b['GPU_DLL']));checks=images=pixels=0;groups={};keep=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q)
    state=bind(lib,'kmSetBackGroundRenderState',I,P);plane=bind(lib,'kmSetBackGroundPlane',I,P,I,I);size=bind(lib,'jp_background_vertex_size',U,U);back=bind(lib,'njSetBackColor',None,U,U,U)
    initbg=bind(lib,'kmiInitializeBackGroundPlane',I);clip=bind(lib,'kmSetPixelClipping',I,I,I,I,I);ctx=bind(lib,'jp_queue_set_context',I,U,P)
    check(qi(8<<20)==0 and fi(8,64,32<<20)==0 and t.init(32,128,32<<20)==0 and v.init(64,64,0xff123456,1)==0,'real shared native owners')
    ri=bind(lib,'jp_render_state_initialize',I,U,S);rs=bind(lib,'jp_render_state_shutdown',None);check(ri(128,1<<20)==0,'real Ninja screen and context state')
    def context(isp=0x80800000,tsp=0x20800000,tcw=0):
        c=Context();c.face[:]=[1,1,1,1];c.words[:]=[0x80000002,isp,tsp,tcw,0];check(state(C.byref(c))==0,'actual SDK background render state');return c
    check(initbg()==0,'actual SDK initialization routine')
    for typ in range(15):
        textured=typ in [3,4,5,6,7,8,11,12,13,14];short=typ in [4,6,8,12,14];floating=typ in [1,5,6];intensity=typ in [2,7,8,10,13,14]
        isp=0x80800000|(0x02000000 if textured else 0)|(0x01000000 if textured else 0)|(0x00400000 if short else 0);context(isp)
        n=size(typ);check(n>=20 and n%4==0,'actual SDK vertex layout size');buffers=[];expected=[isp&0xfff00000,0x20800000,0]
        for vertex,(x,y) in enumerate([(0,0),(64,0),(0,64)]):
            value=.25*(vertex+1);words=[fw(value)]*(n//4);words[:4]=[0xe0000000,fw(x),fw(y),fw(.5)];buf=(U*len(words))(*words);buffers.append(buf);expected.extend([fw(x),fw(y),fw(.5)])
            if textured:expected.extend([fw(value)] if short else [fw(value),fw(value)])
            color=int(value*255)*0x01010101 if floating or intensity else fw(value);expected.append(color)
            if textured:expected.append(color)
        ptrs=(P*3)(*[C.addressof(buf) for buf in buffers]);check(plane(ptrs,typ,n)==0,'actual typed original SDK background writer')
        snapshot=v.snapshot();check(snapshot.background_bytes==len(expected)*4 and list(snapshot.background_packet)[:len(expected)]==expected,('independent SDK packet layout oracle',typ,list(snapshot.background_packet)[:len(expected)],expected))
    before=bytes(v.snapshot())
    for typ in [-1,15,16,17,18,1000]:check(plane(None,typ,0)!=0 and bytes(v.snapshot())==before,'invalid/unsupported dispatch cannot index outside table or mutate packet')
    for pointers,n in [(None,32),((P*3)(),32),((P*3)(1,2,3),1)]:check(plane(pointers,0,n)!=0 and bytes(v.snapshot())==before,'null or short host source rejected before dereference')
    for typ in [15,16,17]:check(bind(lib,'kmiProcessVertex_'+str(typ),I,P)(None)!=0 and bytes(v.snapshot())==before,'original unsupported handlers retain failure')
    groups['all_original_SDK_background_packet_formats_and_checked_dispatch']=checks
    def set_colors(colors,qs=(.5,.5,.5),coords=((0,0),(64,0),(0,64))):
        context();buffers=[]
        for (x,y),q,c in zip(coords,qs,colors):buffers.append((U*8)(0xe0000000,fw(x),fw(y),fw(q),0,0,c,0))
        pointers=(P*3)(*[C.addressof(buf) for buf in buffers]);check(plane(pointers,0,32)==0,'actual packed-color background plane');return (coords,qs,colors)
    config=Config.make();bound=bind(gpu.lib,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P)
    def capture():
        check(reset()==0,'actual queue reset');frame=Q();check(submit(C.byref(frame))==0,'real immutable frame captures background packet');return frame.value
    def render(frame,cfg=config):
        nonlocal images,pixels
        out=C.create_string_buffer(cfg.width*cfg.height*4);report=Report();error=Failure();rc=bound(gpu.handle,frame,C.byref(cfg),out,len(out),C.byref(report),C.byref(error));check(rc==0,('actual background GPU render',rc,error.code,error.hresult));images+=1;pixels+=cfg.width*cfg.height;return out.raw,report
    def compare(actual,expected,tolerance=0):
        worst=max(abs(x-y) for x,y in zip(actual,expected));check(len(actual)==len(expected) and worst<=tolerance,('independent full background image oracle',worst,tolerance))
    def oracle(source,width=64,height=64,clip_rect=(0,0,63,63),clear=0xffffffff):
        coords,qs,colors=source;(x0,y0),(x1,y1),(x2,y2)=coords;det=(x1-x0)*(y2-y0)-(y1-y0)*(x2-x0);out=bytearray()
        for y in range(height):
            for x in range(width):
                sx=(x+.5)*64/width;sy=(y+.5)*64/height
                if not(clip_rect[0]<=sx<clip_rect[2]+1 and clip_rect[1]<=sy<clip_rect[3]+1):out.extend(rgba(clear));continue
                w1=((sx-x0)*(y2-y0)-(sy-y0)*(x2-x0))/det;w2=((x1-x0)*(sy-y0)-(y1-y0)*(sx-x0))/det;weights=(1-w1-w2,w1,w2);q=sum(a*b for a,b in zip(weights,qs))
                values=[sum(weights[j]*qs[j]*rgba(colors[j])[ch]/255 for j in range(3))/q for ch in range(3)]+[1]
                out.extend(round(min(1,max(0,c))*255) for c in values)
        return bytes(out)
    for source in [set_colors([0xff00ff00]*3),set_colors([0xffff0000,0xff00ff00,0xff0000ff]),set_colors([0xffff0000,0xff00ff00,0xff0000ff],(.5,.4375,.375))]:
        # Restore the corresponding source; the list above deliberately
        # constructs all three cases before this loop executes.
        set_colors(source[2],source[1],source[0]);frame=capture();image,report=render(frame);compare(image,oracle(source),1);check(report.batches==1 and report.triangles==2,'plane extends across the full viewport as two native triangles');check(release(frame)==0,'background frame retired')
    source=set_colors([0xffff0000,0xff00ff00,0xff0000ff]);check(clip(16,8,47,55)==0,'actual SDK pixel clipping');frame=capture();image,_=render(frame);compare(image,oracle(source,clip_rect=(16,8,47,55)),1);check(release(frame)==0 and clip(0,0,63,63)==0,'clipped frame retired and clip restored')
    context();back(0xff13579b,0xff13579b,0xff13579b);frame=capture();image,_=render(frame);compare(image,bytes(rgba(0xff13579b))*4096);check(release(frame)==0,'unchanged original njSetBackColor rendered and retired')
    reserve=bind(lib,'jp_queue_reserve',I,U,U);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None);start=bind(lib,'njStartVertexNonTexOpaque',P);end=bind(lib,'njEndVertex',None,P)
    for q,expect_red in [(.25,False),(.5,False),(1.,True),(.45,False)]:
        set_colors([0xff00ff00]*3,(.5,.4375,.375));check(reset()==0 and reserve(0,4)==0 and ctx(0,(U*4)(0x80000002,0x80800000,0x20800000,0))==0,'real foreground against background depth')
        get();ptr=start();words=[]
        for i,(x,y) in enumerate([(-64,-64),(192,-64),(-64,192)]):words.extend([0xf0000000 if i==2 else 0xe0000000,fw(x),fw(y),fw(q),0,0,0xffff0000,0])
        raw=struct.pack('<'+'I'*len(words),*words);C.memmove(ptr,raw,len(raw));end(ptr+len(raw));put();frame=Q();check(submit(C.byref(frame))==0,'combined real background/foreground frame')
        image,_=render(frame.value);compare(image,bytes([255,0,0,255] if expect_red else [0,255,0,255])*4096);check(release(frame.value)==0,'foreground depth frame retired')
    # Malformed source geometry is rejected without replacing output pixels.
    set_colors([0xff00ff00]*3,coords=((0,0),(1,1),(2,2)));frame=capture();out=C.create_string_buffer(b'Z'*(4096*4));report=Report();C.memset(C.byref(report),0x5a,C.sizeof(report));old_report=bytes(report);error=Failure()
    check(bound(gpu.handle,frame,C.byref(config),out,4096*4,C.byref(report),C.byref(error))!=0 and out.raw==b'Z'*(4096*4)+b'\0' and bytes(report)==old_report,'degenerate plane preserves caller pixels and report');check(release(frame)==0,'failed background frame retired')
    groups['full_viewport_gradient_perspective_clipping_and_original_back_color']=checks-sum(groups.values())
    # A real uploaded RGB565 surface is pinned by the background even with
    # empty geometry lists. Releasing the game list after capture must work.
    names,listing,filename_buffers=destination(1);payload=archive([texture(b'\xe0\x07'*64,kind=0x101)]);data=C.create_string_buffer(payload)
    check(pvm.load(C.byref(listing),1,28,data,len(payload))==1,'actual native green texture upload');record=Record.from_address(t.lookup(C.byref(listing),0));context(0x82800000,0x20800000|record.tspparambuffer,record.texparambuffer)
    verts=[]
    for x,y in [(0,0),(64,0),(0,64)]:verts.append((U*8)(0xe0000000,fw(x),fw(y),fw(.5),fw(.25),fw(.25),0xffff0000,0))
    pointers=(P*3)(*[C.addressof(buf) for buf in verts]);check(plane(pointers,3,32)==0,'real textured SDK background packet');frame=capture()
    check(t.release_list(C.byref(listing))==1 and t.live()==0 and bind(lib,'jp_frames_orphaned_bytes',S)()>0,'game texture is released while captured background retains its pixels')
    set_colors([0xffffff00]*3);image,report=render(frame);compare(image,bytes([0,255,0,255])*4096);check(report.texture_uploads==1,'captured background uses retained actual texture')
    off=Config.make(textures=0);image,report=render(frame,off);compare(image,bytes([255,0,0,255])*4096);check(report.texture_uploads==report.shader_texture_bindings==report.shader_sampler_bindings==0,'Texture Off uses background vertex colors without texture or sampler bindings')
    check(release(frame)==0 and t.live()==0 and bind(lib,'jp_frames_orphaned_bytes',S)()==0,'retained background texture freed exactly once')
    groups['textured_background_capture_release_mutation_and_Texture_Off']=checks-sum(groups.values())
    # Feed an actual background through original frame_wait and the visible
    # native swapchain. Read at EOR, before flip-discard invalidates old pixels.
    present=Present(Path(b['DLL']));gl=gpu.lib;window=P();err=Failure();device=C.addressof((C.c_byte*1).in_dll(lib,'kmiDeviceExtension'));present.device(device)
    wo=bind(gl,'jp_window_open',I,P,U,U,I,P,P);wc=bind(gl,'jp_window_close',None,P);copy=bind(gl,'jp_window_copy_job',I,P,Q,P);read=bind(gl,'jp_window_readback',I,P,P,S,P);count=bind(gl,'jp_window_presented',Q,P)
    check(wo(gpu.handle,64,64,1,C.byref(window),C.byref(err))==0,'real background presentation window')
    check(present.clock.init(None,None,0)==0 and present.wait_init(60000,1001,8,C.cast(present.window_idle,P),window)==0 and present.init(gpu.handle,window,device,C.byref(config),5000,0)==0,'real original frame-wait host and presenter')
    observed=[]
    @Callback
    def done(arg):
        try:
            state=present.snapshot();out=C.create_string_buffer(4096*4);error=Failure();observed.append((copy(window,state.job,C.byref(error)),read(window,out,len(out),C.byref(error)),out.raw))
        except Exception as e:observed.append(str(e))
    check(present.callback(C.cast(done,P),None)==0,'real EOR observer');present.probe(0,0);present.pacing(2,2);source=set_colors([0xff0000ff]*3);check(reset()==0,'real original frame recording');present.frame();s=present.snapshot()
    check(present.status()==present.wait_status()==0 and s.submissions==s.completions==s.presentations==1 and count(window)==1,'original frame_wait completes actual GPU background and Present')
    check(len(observed)==1 and observed[0][:2]==(0,0),'EOR reads the completed real job');compare(observed[0][2],bytes([0,0,255,255])*4096);images+=1;pixels+=4096
    check(present.shutdown()==0 and present.wait_shutdown()==0,'original presentation owners drained');present.clock.shutdown();wc(window);gpu.close(gpu.handle)
    qs();rs();t.shutdown();check(fs()==0 and pvm.shutdown()==0,'all native owners drained')
    groups['original_frame_wait_GPU_jobs_and_real_Present']=checks-sum(groups.values())
    result=dict(status='original_SDK_background_packets_native_full_viewport_images_texture_retention_and_original_Present_verified',checks=checks,groups=groups,SDK_dispatch_entries_checked=18,supported_SDK_vertex_formats_checked=15,actual_GPU_images=images,pixels_compared=pixels,visible_Kamui_Presents=1,native_plane_interpolation_contract_verified=True,physical_Dreamcast_background_rounding_verified=False,original_njSetBackColor_executed=True,Texture_Off_has_no_texture_or_sampler_bindings=True,background_only_texture_retention_verified=True,full_logo_start_or_boot_executed=False,playable=False,build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
