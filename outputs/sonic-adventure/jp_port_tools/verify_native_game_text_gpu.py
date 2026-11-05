"""Render actual game text with the genuine font and independent skew oracle.
Screenshots contain retail-derived font pixels and remain private.
"""
import argparse,ctypes as C,hashlib,json
from fractions import Fraction
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime
from native_video_test_support import Video
from native_gpu_test_support import GPU,Config,Report,Failure
from native_pvm_test_support import image_oracle
from capture_native_boot_progress import png
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','gpu','font-input','output']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();out=a.output.resolve();assert not out.exists() and out.is_relative_to(r/'work/sonic-adventure');out.mkdir();b=json.loads((a.build/'build_verification.json').read_bytes());assert sha(b['frames_DLL'])==b['frames_DLL_sha256']
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;v=Video(lib);checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);fi=bind(lib,'jp_frames_initialize',I,U,U,S);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q);pinned=bind(lib,'jp_frames_pinned_bytes',S);orphan=bind(lib,'jp_frames_orphaned_bytes',S)
    check(qi(8<<20)==0 and fi(8,16,8<<20)==0 and t.init(4,16,8<<20)==0 and v.init(320,128,0xff000000,1)==0 and bind(lib,'jp_ninja_contexts_initialize',I)()==0,'actual owners and logical viewport')
    bind(lib,'njInitPrint',None,P,U,U)(None,0,0);check(v.init(320,128,0xff000000,1)==0,'explicit scene background after context default initialization');bind(lib,'text_style_init',None,P,U,U)(None,0,0);bind(lib,'text_set_size',None,C.c_uint16)(16);bind(lib,'text_set_color',None,U)(0xffffffff);I.in_dll(lib,'frame_step').value=0
    text=bind(lib,'text_print',None,I,P);front=bind(lib,'text_print_front',None,I,P);number=bind(lib,'text_print_number',None,I,I,I);hexprint=bind(lib,'text_print_hex',None,I,U,I)
    lines=[(1,1,b'SONIC ADVENTURE'),(1,3,b'Game debug text'),(1,5,b'120'),(5,5,b'DEADBEEF')]
    text(0x10001,C.create_string_buffer(lines[0][2]));front(0x10003,C.create_string_buffer(lines[1][2]));number(0x10005,120,3);hexprint(0x50005,0xdeadbeef,8)
    fid=Q();check(bind(lib,'jp_print_status',I)()==0 and submit(C.byref(fid))==0 and pinned()==32768,'real game callers submit a font-retaining frame')
    bind(lib,'text_exit',None)();check(t.live()==1,'game text exit retains Ninja-owned font');bind(lib,'njExitPrint',None)();check(orphan()==32768,'SDK font release preserves submitted frame payload')
    rawfont=(a.font_input/'debug-font-128x128-argb1555-twiddled.bin').read_bytes();atlas=image_oracle(dict(kind=0x100,w=128,h=128,pixels=rawfont),128,128)
    width,height=640,256;expected=bytearray(b'\0\0\0\xff'*(width*height));off=bytearray(expected)
    # The game uses a 19px-wide parallelogram in a 16px cell. Top-left is
    # x+1, bottom-left x-4. Solve its independent affine coordinates at pixel
    # centres; sample the original font with its 1/1024 inset and 63/1024 span.
    for gridx,gridy,string in lines:
        for index,ch in enumerate(string):
            if ch==48:ch=79
            x=(gridx+index)*16;y=gridy*16
            for py in range(y*2,(y+16)*2):
                vertical=Fraction(2*py+1-4*y,64);left=Fraction(x+1)-5*vertical
                for px in range(max(0,(x-4)*2),min(width,(x+20)*2)):
                    horizontal=(Fraction(2*px+1,4)-left)/19
                    if not 0<=horizontal<1:continue
                    offset=(py*width+px)*4;off[offset:offset+4]=b'\xff'*4
                    sx=int(Fraction((ch%16)*8)+Fraction(1,8)+horizontal*Fraction(63,8));sy=int(Fraction((ch//16)*8)+Fraction(1,8)+vertical*Fraction(63,8))
                    sample=(sy*128+sx)*4
                    if atlas[sample+3]:expected[offset:offset+4]=atlas[sample:sample+4]
    gpu=GPU(a.gpu.resolve());gl=gpu.lib;check(bool(gpu.handle),'actual GPU device');bound=bind(gl,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P);report=Report();err=Failure();cfg=Config.make(width,height,1);cfg.logical_width=320;cfg.logical_height=128;buffer=C.create_string_buffer(width*height*4)
    check(bound(gpu.handle,fid,C.byref(cfg),buffer,len(buffer),C.byref(report),C.byref(err))==0,'actual game text GPU draw');png(out/'game-debug-text-observed.png',buffer.raw,width,height)
    check(buffer.raw==expected,('every actual italic glyph pixel equals independent font/affine oracle',sum(x!=y for x,y in zip(buffer.raw,expected))))
    cfg_off=Config.make(width,height,0);cfg_off.logical_width=320;cfg_off.logical_height=128;buffer_off=C.create_string_buffer(len(buffer));off_report=Report();check(bound(gpu.handle,fid,C.byref(cfg_off),buffer_off,len(buffer_off),C.byref(off_report),C.byref(err))==0,'actual Texture Off GPU draw');png(out/'game-debug-text-off-observed.png',buffer_off.raw,width,height)
    check(buffer_off.raw==off and off_report.shader_texture_bindings==off_report.shader_sampler_bindings==0,'Texture Off renders vertex-coloured quads with zero sampled textures')
    window=P();check(bind(gl,'jp_window_open',I,P,U,U,I,P,P)(gpu.handle,width,height,1,C.byref(window),C.byref(err))==0,'actual game-text native window')
    check(bind(gl,'jp_window_render_frame',I,P,Q,P,P,P)(window,fid,C.byref(cfg),C.byref(report),C.byref(err))==0,'actual window draws original game text frame')
    actual=C.create_string_buffer(len(buffer));check(bind(gl,'jp_window_readback',I,P,P,S,P)(window,actual,len(actual),C.byref(err))==0 and actual.raw==expected,'actual window backbuffer equals independent oracle');shot=out/'native-original-game-debug-text.png';png(shot,actual.raw,width,height)
    check(bind(gl,'jp_window_present',I,P,U,P)(window,0,C.byref(err))==0,'actual text window Present');bind(gl,'jp_window_close',None,P)(window)
    check(release(fid)==0 and pinned()==orphan()==0,'last frame releases retained font');gpu.close(gpu.handle);bind(lib,'jp_queue_shutdown',None)();t.shutdown();check(bind(lib,'jp_frames_shutdown',I)()==0,'all real owners drained');v.shutdown()
    result=dict(status='original_game_debug_text_italic_pixels_real_window_Off_and_font_lifetime_verified',checks=checks,actual_GPU_images=3,independent_oracle_pixels=width*height*2,actual_window_backbuffer_pixels=width*height,actual_Presents=1,all_actual_pixels_match_independent_oracle=True,Texture_Off_no_sampling=True,frame_survives_original_font_release=True,native_game_functions_replaced=False,raw_font_sha256=hashlib.sha256(rawfont).hexdigest(),build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),screenshots=[dict(path=str(shot),sha256=sha(shot),actual_window_backbuffer=True,retail_asset_derived=True,gameplay=False,test_scene=True)],screenshots_published=False,whole_gameplay_or_localization_verified=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',flush=True)
if __name__=='__main__':main()
