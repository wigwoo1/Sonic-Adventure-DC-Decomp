"""Real UV16 background sampling/offset colors, using an asymmetric texture."""
import argparse,ctypes as C,hashlib,json,struct
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,Record
from native_pvm_test_support import PVM,archive,texture,destination,morton
from native_video_test_support import Video,Context
from native_gpu_test_support import GPU,Config,Report,Failure
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def fw(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();b=json.loads((d/'build_verification.json').read_text());assert not a.report.exists()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;pvm=PVM(lib);v=Video(lib);gpu=GPU(Path(b['GPU_DLL']));checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q)
    check(qi(1<<20)==0 and fi(4,16,8<<20)==0 and t.init(8,32,8<<20)==0 and v.init(64,64,0xff000000,1)==0,'real production owners')
    encoded=bytearray(128)
    for y in range(8):
        for x in range(8):struct.pack_into('<H',encoded,2*morton(x,y,8,8),[0xf800,0x07e0,0x001f,0xffff][(y//4)*2+x//4])
    names,listing,buffers=destination(1);payload=archive([texture(bytes(encoded),kind=0x101)]);data=C.create_string_buffer(payload);check(pvm.load(C.byref(listing),1,28,data,len(payload))==1,'asymmetric actual RGB565 texture')
    rec=Record.from_address(t.lookup(C.byref(listing),0));context=Context();context.face[:]=[1,1,1,1];context.words[:]=[0x8000000e,0x83c00000,0x20800000|rec.tspparambuffer,rec.texparambuffer,0]
    check(bind(lib,'kmSetBackGroundRenderState',I,P)(C.byref(context))==0,'actual textured/offset/UV16 render state')
    # Packed upper half-floats: U=.125, V=.625. An axis swap samples green,
    # whereas the correct bottom-left quadrant is blue.
    uv=(fw(.125)&0xffff0000)|(fw(.625)>>16);vertices=[(U*8)(0xe0000000,fw(x),fw(y),fw(.5),uv,0,0xff00ff00,0x00ff0000) for x,y in [(0,0),(64,0),(0,64)]];pointers=(P*3)(*[C.addressof(vertex) for vertex in vertices])
    check(bind(lib,'kmSetBackGroundPlane',I,P,I,I)(pointers,4,32)==0,'actual UV16 packed-color SDK packet');frame=Q();check(submit(C.byref(frame))==0,'real immutable background frame')
    draw=bind(gpu.lib,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P)
    for enabled,expected,uploads in [(1,bytes([255,0,255,255]),1),(0,bytes([255,255,0,255]),0)]:
        cfg=Config.make(textures=enabled);out=C.create_string_buffer(4096*4);report=Report();error=Failure()
        check(draw(gpu.handle,frame.value,C.byref(cfg),out,len(out),C.byref(report),C.byref(error))==0,'real native UV16 render')
        check(out.raw==expected*4096,'independent UV16 axis, encoded sample, offset and Off-color oracle')
        check(report.texture_uploads==uploads and (enabled or report.shader_texture_bindings==report.shader_sampler_bindings==0),'actual sampled versus vertex-only shader bindings')
    check(release(frame.value)==0 and t.release_list(C.byref(listing))==1 and t.live()==0,'all actual texture/frame references released');gpu.close(gpu.handle);qs();t.shutdown();check(fs()==0 and pvm.shutdown()==0,'all owners drained')
    result=dict(status='real_UV16_asymmetric_background_sampling_offset_and_Texture_Off_verified',checks=checks,actual_GPU_images=2,pixels_compared=8192,independent_axis_swap_detecting_oracle=True,build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
