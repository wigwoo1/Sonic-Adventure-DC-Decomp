"""Prepare private authentic font inputs and render its atlas on actual D3D.
This verifies font data/ownership/pixels, not the unported print functions.
"""
import argparse,ctypes as C,hashlib,json,re,struct
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,Surface,Record
from native_video_test_support import Video
from native_gpu_test_support import GPU,Config,Report,Failure
from native_pvm_test_support import image_oracle
from capture_native_boot_progress import png
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def fw(v):return struct.unpack('<I',struct.pack('<f',v))[0]
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();out=a.output.resolve();assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();b=json.loads((a.build/'build_verification.json').read_bytes())
    pin=json.loads((r/'outputs/sonic-adventure/jp_port_tools/jp_camera_external_inputs.json').read_bytes());assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.upstream/'config/1st_read/symbols.txt')==pin['files_sha256']['config/1st_read/symbols.txt']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ['frames_DLL','GPU_DLL']:assert sha(b[n])==b[n+'_sha256']
    table={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    split=(a.upstream/'config/1st_read/splits.txt').read_text();matches=re.findall(r'^sdk/ninja/debug\.c\s+DSG\s+(0x[0-9a-f]+)\s+(0x[0-9a-f]+)\s*$',split,re.M);assert len(matches)==1;start,end=map(lambda x:int(x,16),matches[0]);assert start==table['_nj_debug_char_'] and end-start==32768
    binary=a.binary.read_bytes();load=0x8c010000;literal=0x8c622908;assert struct.unpack_from('<I',binary,literal-load)[0]==start and 0<=start-load<end-load<=len(binary);data=binary[start-load:end-load]
    out.mkdir();raw=out/'debug-font-128x128-argb1555-twiddled.bin';raw.write_bytes(data);include=out/'include/ninja/debug/_nj_debug_char_.inc';include.parent.mkdir(parents=True);include.write_text('\n'.join(','.join('0x%02x'%x for x in data[i:i+16])+',' for i in range(0,len(data),16))+'\n')
    assert bytes(int(x,16) for x in re.findall(r'0x([0-9a-f]{2})',include.read_text()))==data
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;video=Video(lib);gpu=GPU(Path(b['GPU_DLL']));gl=gpu.lib;checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q)
    check(qi(8<<20)==0 and fi(4,8,8<<20)==0 and t.init(4,16,4<<20)==0 and video.init(64,64,0xff000000,1)==0,'actual shared owners')
    surface=Surface();surface.Type=0x01010000;surface.BitDepth=1;surface.PixelFormat=0;surface.nWidth=surface.nHeight=128;surface.TextureSize=32768;surface.fSurfaceFlags=4
    check(bind(lib,'jp_texture_replace_standalone_surface',I,P,P,S)(C.byref(surface),C.create_string_buffer(data),len(data))==0,'authentic original font data copied to actual direct owner')
    word=t.surface_word(C.byref(surface));bind(lib,'njInitTexture',None,P,U)(None,0);check(t.status()==0 and t.live()==1 and t.surface_word(C.byref(surface))==word and C.string_at(t.surface_data(word),len(data))==data,'authentic font survives genuine zero-record initialization')
    record=Record();record.texinfo.texsurface=surface;t.param(C.byref(record));check(t.status()==0 and record.tspparambuffer==36 and record.texparambuffer==word,'actual original SDK texture parameter generation')
    check(bind(lib,'jp_queue_reserve',I,U,U)(1,4)==0 and bind(lib,'jp_queue_set_context',I,U,P)(0,(U*4)(0x8200000a,0xe0800000,0x209000c0|record.tspparambuffer,record.texparambuffer))==0,'real atlas context')
    bind(lib,'njGetkmPointer',None)();pointer=bind(lib,'njStartVertexTexTrans',P)();check(bool(pointer),'real SDK atlas batch start');words=[]
    for i,(x,y,u,v) in enumerate([(-64,-64,-1,-1),(192,-64,3,-1),(-64,192,-1,3)]):words += [0xf0000000 if i==2 else 0xe0000000,fw(x),fw(y),fw(1),fw(u),fw(v),0xffffffff,0]
    C.memmove(pointer,struct.pack('<24I',*words),96);bind(lib,'njEndVertex',None,P)(pointer+96);bind(lib,'njSetkmPointer',None)();fid=Q();check(submit(C.byref(fid))==0,'real captured atlas frame')
    cfg=Config.make(128,128,1);buffer=C.create_string_buffer(128*128*4);rep=Report();error=Failure();bound=bind(gl,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P)
    check(bound(gpu.handle,fid,C.byref(cfg),buffer,len(buffer),C.byref(rep),C.byref(error))==0,'actual D3D authentic font atlas');expected=image_oracle(dict(kind=0x100,w=128,h=128,pixels=data),128,128)
    check(buffer.raw==expected and rep.decoded_texels==16384,'all authentic atlas texels/alpha equal independent twiddle/ARGB1555 oracle')
    window=P();check(bind(gl,'jp_window_open',I,P,U,U,I,P,P)(gpu.handle,128,128,1,C.byref(window),C.byref(error))==0,'actual font atlas window');check(bind(gl,'jp_window_render_frame',I,P,Q,P,P,P)(window,fid,C.byref(cfg),C.byref(rep),C.byref(error))==0,'direct font atlas backbuffer')
    actual=C.create_string_buffer(len(buffer));check(bind(gl,'jp_window_readback',I,P,P,S,P)(window,actual,len(actual),C.byref(error))==0 and actual.raw==expected,'actual backbuffer equals independent original-font oracle');shot=out/'original-font-atlas.png';png(shot,actual.raw,128,128);check(bind(gl,'jp_window_present',I,P,U,P)(window,0,C.byref(error))==0,'actual font atlas Present')
    cfg.textures=0;check(bound(gpu.handle,fid,C.byref(cfg),buffer,len(buffer),C.byref(rep),C.byref(error))==0 and buffer.raw==b'\xff'*(128*128*4) and rep.texture_uploads==rep.decoded_texels==rep.shader_texture_bindings==rep.shader_sampler_bindings==0,'Texture Off uses only white vertex geometry with zero font sampling')
    bind(gl,'jp_window_close',None,P)(window);gpu.close(gpu.handle);check(release(fid)==0,'atlas frame released');qs();t.shutdown();check(fs()==0,'atlas owners drained');video.shutdown()
    report=dict(status='authentic_Japanese_debug_font_data_prepared_and_native_atlas_pixels_verified',checks=checks,original_binary_sha256=sha(a.binary),font_start_address=hex(start),font_end_address=hex(end),font_bytes=32768,font_payload_sha256=sha(raw),original_njInitPrint_font_pointer_literal=hex(literal),literal_matches_font_symbol=True,split_symbol_and_complete_binary_range_verified=True,private_raw_font_path=str(raw),private_include_path=str(include),private_include_sha256=sha(include),private_include_roundtrip_exact=True,actual_GPU_images=2,atlas_pixels_compared=16384,actual_Presents=1,actual_window_screenshot=dict(path=str(shot),sha256=sha(shot),actual_window_backbuffer=True,original_game_font_atlas=True,retail_derived=True,gameplay=False),Texture_Off_zero_sampled_bindings=True,retail_font_payload_and_screenshot_publishable=False,genuine_njInitPrint_njCharacter_or_text_printing_executed=False,source_files_sha256={n:sha(a.upstream/n) for n in ['sdk/ninja/njDebug.c','sdk/ninja/debug.c','config/1st_read/symbols.txt','config/1st_read/splits.txt']},independent_image_oracle_sha256=sha(Path(__file__).with_name('native_pvm_test_support.py')),build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),full_system_boot_main_loop_gameplay_verified=False,playable=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
