"""Actual perspective -> original polygon producer -> immutable frame -> D3D.
Synthetic constant-colour geometry, with independent geometric/pixel checks.
Screenshots come from real Win32 backbuffers, not fabricated gameplay.
"""
import argparse,ctypes as C,hashlib,json,math
from pathlib import Path
from native_packet_test_support import bind,Decoder,Plan,Failure as PacketFailure
from native_texture_test_support import Runtime
from native_video_test_support import Video
from native_gpu_test_support import GPU,Config,Report,Failure
from capture_native_boot_progress import png
U=C.c_uint32;I=C.c_int;F=C.c_float;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
class Poly(C.Structure):
    _fields_=[('p',P),('col',P),('tex',P),('num',U)]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();out=a.output.resolve()
    assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();out.mkdir()
    b=json.loads((d/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ['frames_DLL','GPU_DLL']:assert sha(b[n])==b[n+'_sha256']
    rt=Runtime(Path(b['frames_DLL']));lib=rt.dll;video=Video(lib);gpu=GPU(Path(b['GPU_DLL']));gl=gpu.lib;decoder=Decoder(b['GPU_DLL'])
    checks=images=pixels=0;shots=[];cases=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);ctx=bind(lib,'jp_queue_set_context',I,U,P)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q)
    ri=bind(lib,'jp_render_state_initialize',I,U,S);rs=bind(lib,'jp_render_state_shutdown',None);configure=bind(lib,'jp_render_state_configure',I,U,U,F,F,F,F,F)
    contexts=bind(lib,'jp_ninja_contexts_initialize',I);matrix=bind(lib,'njSetMatrix',None,P,P);perspective=bind(lib,'njSetPerspective',None,I);draw=bind(lib,'njDrawPolygon3D',None,P,I,U)
    bound=bind(gl,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P)
    window=P();err=Failure();open_window=bind(gl,'jp_window_open',I,P,U,U,I,P,P);close=bind(gl,'jp_window_close',None,P)
    direct=bind(gl,'jp_window_render_frame',I,P,Q,P,P,P);readback=bind(gl,'jp_window_readback',I,P,P,S,P);present=bind(gl,'jp_window_present',I,P,U,P)
    check(qi(8<<20)==0 and fi(4,16,8<<20)==0 and rt.init(16,64,8<<20)==0 and video.init(64,64,0xff102030,1)==0 and ri(128,1<<20)==0 and contexts()==0,'real shared native owners')
    # njInit3D sets its own white background. Select the test background
    # afterwards through the genuine SDK setter, not harness metadata.
    bind(lib,'njSetBackColor',None,U,U,U)(0xff102030,0xff102030,0xff102030)
    check(open_window(gpu.handle,256,256,1,C.byref(window),C.byref(err))==0,'actual Win32/DXGI window')
    identity=(F*16)(*[float(i%5==0) for i in range(16)]);matrix(None,identity)
    screen=(F*5).in_dll(lib,'_nj_screen_');picture_aspect=F.in_dll(lib,'port_draw_aspect');colors=(U*3)(*[0xff50c080]*3);cfg=Config.make(256,256,0)
    for degrees in [45,70,90]:
        for z in [3.,6.]:
            for aspect in [1.,.75]:
                check(reset()==0 and configure(64,64,32,1,100,aspect,1)==0,'real screen/clip state')
                # The retained GCC polygon producer intentionally uses the
                # port picture-aspect global instead of Ninja's aspect_x.
                picture_aspect.value=aspect
                angle=int(degrees*65536/360);perspective(angle)
                ideal=64/(2*math.tan((angle>>1)*2*math.pi/65536))
                check(abs(screen[0]-ideal)<=max(1,abs(ideal))*3e-6,'independent mathematical perspective distance within FSCA value tolerance')
                # Genuine polygon producer uses the queue's real metadata,
                # with no textures, culling or fog for this isolated test.
                check(ctx(0,(U*4)(0x82000002,0xe0800000,0x20900000,0))==0,'real untextured context')
                xyz=[(-1.,-.75,z),(1.,-.75,z),(0.,1.,z)];points=(F*9)(*[v for row in xyz for v in row]);poly=Poly(C.addressof(points),C.addressof(colors),None,3)
                saved=bytes(points),bytes(colors),bytes(poly);draw(C.byref(poly),3,0x60)
                check(saved==(bytes(points),bytes(colors),bytes(poly)),'original producer inputs remain intact')
                fid=Q();check(submit(C.byref(fid))==0,'real original 3D polygon frame capture')
                plan=P();failure=PacketFailure();check(decoder.frame(fid.value,8<<20,C.byref(plan),C.byref(failure))==0,'actual captured packet decode')
                decoded=C.cast(plan,C.POINTER(Plan)).contents
                check(decoded.vertex_count==3 and decoded.triangle_count==1,'complete original polygon emitted one triangle')
                expected=[(32+x*screen[0]*aspect/z,32+y*screen[0]/z) for x,y,z in xyz]
                for position,index in enumerate([0,2,1]):
                    actual=decoded.vertices[position]
                    x,y=expected[index]
                    check(abs(actual.x-x)<2e-5 and abs(actual.y-y)<2e-5 and abs(actual.inv_w-1/z)<1e-7,('independent projected geometry in actual packets',degrees,z,aspect,index,actual.x,actual.y,actual.inv_w,x,y))
                decoder.destroy(plan)
                buffer=C.create_string_buffer(256*256*4);rep=Report()
                check(bound(gpu.handle,fid,C.byref(cfg),buffer,len(buffer),C.byref(rep),C.byref(err))==0,'real D3D projected polygon render');images+=1
                check(rep.triangles==3 and rep.draw_calls==2 and rep.texture_uploads==rep.shader_texture_bindings==rep.shader_sampler_bindings==0,'one foreground triangle, two genuine SDK background triangles, Texture Off bindings')
                data=buffer.raw;area=lambda a,b,p:(b[0]-a[0])*(p[1]-a[1])-(b[1]-a[1])*(p[0]-a[0])
                sign=1 if area(*expected)>0 else -1;covered=0
                for yy in range(256):
                    for xx in range(256):
                        p=((xx+.5)/4,(yy+.5)/4);edges=[sign*area(expected[i],expected[(i+1)%3],p) for i in range(3)]
                        # Exclude a narrow edge strip where GPU subpixel
                        # quantisation/top-left coverage can legitimately differ.
                        if min(abs(v)/math.dist(expected[i],expected[(i+1)%3]) for i,v in enumerate(edges))<.02:continue
                        inside=min(edges)>0;want=bytes([0x50,0xc0,0x80,255] if inside else [0x10,0x20,0x30,255]);at=(yy*256+xx)*4
                        check(max(abs(x-y) for x,y in zip(data[at:at+4],want))<=1,('independent constant-colour coverage and background',xx,yy,list(data[at:at+4]),list(want)));pixels+=1;covered+=inside
                check(covered>100,'projected triangle has visible independent coverage');cases.append(dict(degrees=degrees,angle=angle,z=z,aspect=aspect,distance=screen[0],projected_vertices=expected,interior_pixels=covered))
                if z==3 and aspect==1:
                    check(direct(window,fid,C.byref(cfg),C.byref(rep),C.byref(err))==0,'actual projected polygon backbuffer')
                    actual=C.create_string_buffer(len(buffer));check(readback(window,actual,len(actual),C.byref(err))==0 and actual.raw==buffer.raw,'real window backbuffer equals independently checked render')
                    path=out/('perspective-'+str(degrees)+'.png');png(path,actual.raw,256,256);shots.append(dict(path=str(path),sha256=sha(path),actual_window_backbuffer=True,synthetic_renderer_test=True,gameplay=False))
                    check(present(window,0,C.byref(err))==0,'actual perspective test Present')
                check(release(fid)==0,'real frame release')
    close(window);gpu.close(gpu.handle);check(fs()==0,'all frames released');rs();rt.shutdown();qs();video.shutdown()
    result=dict(status='actual_perspective_original_3D_polygon_packets_and_D3D_pixels_verified',checks=checks,images_compared=images,pixels_compared=pixels,cases=cases,actual_window_screenshots=shots,actual_Presents=len(shots),screenshots_are_synthetic_renderer_tests=True,original_polygon_producer_called=True,retained_GCC_port_draw_aspect_policy=True,independent_mathematical_projection=True,independent_interior_and_background_pixel_oracle=True,edge_strip_excluded_logical_pixels=.02,Texture_Off_zero_texture_sampler_bindings=True,build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(__file__),full_gameplay_verified=False,playable=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result),flush=True)
if __name__=='__main__':main()
