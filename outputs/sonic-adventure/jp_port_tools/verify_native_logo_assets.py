"""Verify the unchanged original logo composition through real file/GPU/Present.
Retail pixels and original generated metadata remain private inputs.
"""
import argparse,ctypes as C,hashlib,json,re
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,List
from native_asset_test_support import Assets,Limits
from native_pvm_test_support import PVM,index_archive,image_oracle
from native_video_test_support import Video
from native_gpu_test_support import GPU,Config,Report,Failure
from native_present_test_support import Present
U=C.c_uint32;I=C.c_int;Q=C.c_uint64;P=C.c_void_p;S=C.c_size_t;Callback=C.CFUNCTYPE(None,P)
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','probe','inputs','asset-root','report']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.probe.resolve();b=json.loads((d/'build_verification.json').read_text());build=Path(b['asset_build']);owner=json.loads((build/'build_verification.json').read_text())
    assert not a.report.exists() and a.report.resolve().is_relative_to(r/'work/sonic-adventure')
    assert b['exit_code']==b['diagnostic_bytes']==0 and b['original_body_unchanged'] and sha(build/'build_verification.json')==b['asset_build_sha256']
    for n,h in dict(owner['authored_source_sha256'],**owner['vendored_MIT_source_sha256']).items():assert sha(r/n)==h,n
    for n in ['DLL','frame_owner_DLL']:assert sha(b[n])==b[n+'_sha256']
    for n,row in b['private_generated_metadata'].items():assert sha(d/'inputs/progress/demo'/n)==row['sha256']
    manifest=json.loads((a.inputs/'input_manifest.json').read_text());source=next(row for row in manifest['assets'] if row['disc_path']=='SONICADV/SEGALOGO.PRS')
    assert sha(source['PVM'])==source['PVM_sha256'] and sha(a.asset_root/'SEGALOGO.PRS')==source['compressed_sha256']
    flags,entries,images=index_archive(Path(source['PVM']).read_bytes());assert len(entries)==len(images)==6
    # Decode each source image independently, then place texels using the
    # original metadata. No native packet decoding or renderer output is used.
    tiles=[image_oracle(image,256,256,True) for image in images]
    anims=[tuple(map(int,re.findall(r'-?\d+',row))) for row in (d/'inputs/progress/demo/logo_texanims.inc').read_text().splitlines() if '{' in row]
    assert len(anims)==6 and all(v[:2]==(256,256) and v[4:8]==(0,0,256,256) and v[9]==0 for v in anims)
    expected=bytearray(bytes([0,0,0,255])*(640*480));coverage=bytearray(640*480)
    for sx,sy,cx,cy,u1,v1,u2,v2,index,attr in anims:
        image=tiles[entries[index]['number']];left=320-cx;top=-cy
        for y in range(max(0,top),min(480,top+sy)):
            for x in range(max(0,left),min(640,left+sx)):
                at=y*640+x;pixel=((y-top)*256+x-left)*4
                assert not coverage[at];coverage[at]=1;expected[4*at:4*at+4]=image[pixel:pixel+4]
    assert all(coverage);expected=bytes(expected)
    t=Runtime(Path(b['frame_owner_DLL']));lib=t.dll;video=Video(lib);assets=Assets(lib);pvm=PVM(lib);gpu=GPU(Path(owner['GPU_DLL']));probe=C.CDLL(b['DLL']);checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);ctx=bind(lib,'jp_queue_set_context',I,U,P)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q)
    ri=bind(lib,'jp_render_state_initialize',I,U,S);rs=bind(lib,'jp_render_state_shutdown',None);status=bind(lib,'jp_render_state_status',I);queue_status=bind(lib,'jp_queue_status',I)
    load=bind(probe,'jp_logo_asset_load',I);draw=bind(probe,'jp_logo_asset_draw',None);listing=bind(probe,'jp_logo_asset_list',C.POINTER(List));free=bind(probe,'jp_logo_asset_release',I)
    check(qi(8<<20)==0 and fi(8,128,32<<20)==0 and t.init(16,1024,32<<20)==0 and video.init(640,480,0xff000000,1)==0 and ri(128,1<<20)==0,'real shared queue/frame/texture/video/render owners')
    limits=Limits(4<<20,16<<20,32<<20,64);check(assets.init(str(a.asset_root.resolve()),C.byref(limits))==0,'actual file/PRS asset provider')
    check(load()==0 and pvm.loaded(listing())==6 and t.live()==6,'unchanged logo list filled by original adapted game wrapper')
    before=[listing().contents.textures[i].filename for i in range(6)]
    def record():
        check(reset()==0 and ctx(0,(U*4)(0x8000000a,0xe0800000,0x20800000,0))==0,'point-filtered opaque context with fog disabled')
        draw();check(status()==queue_status()==t.status()==0,'unchanged six-sprite logo draw succeeds on actual native owners')
    config=Config.make(640,480);config.logical_width=640;config.logical_height=480
    record();frame=Q();check(submit(C.byref(frame))==0,'original logo submits actual captured frame')
    out=C.create_string_buffer(640*480*4);report=Report();failure=Failure();bound=bind(gpu.lib,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P)
    rc=bound(gpu.handle,frame.value,C.byref(config),out,len(out),C.byref(report),C.byref(failure))
    check(rc==0,('native GPU logo render',rc,failure.code,failure.hresult))
    mismatch=next((i for i,(x,y) in enumerate(zip(out.raw,expected)) if x!=y),None)
    check(out.raw==expected,('640x480 independent original-logo pixel oracle',mismatch,None if mismatch is None else (out.raw[mismatch],expected[mismatch])))
    check(report.batches==6 and report.triangles==12 and report.texture_uploads==6,'all original pieces and twelve triangles use actual texture uploads')
    check(release(frame.value)==0,'captured logo frame retired')
    present=Present(Path(owner['DLL']));gl=gpu.lib;window=P();error=Failure();device=C.addressof((C.c_byte*1).in_dll(lib,'kmiDeviceExtension'));present.device(device)
    wo=bind(gl,'jp_window_open',I,P,U,U,I,P,P);wc=bind(gl,'jp_window_close',None,P);copy=bind(gl,'jp_window_copy_job',I,P,Q,P);read=bind(gl,'jp_window_readback',I,P,P,S,P);count=bind(gl,'jp_window_presented',Q,P)
    check(wo(gpu.handle,640,480,1,C.byref(window),C.byref(error))==0,'real visible native logo window')
    check(present.clock.init(None,None,0)==0 and present.wait_init(60000,1001,8,C.cast(present.window_idle,P),window)==0,'real logical clock and original frame-wait host')
    check(present.init(gpu.handle,window,device,C.byref(config),5000,0)==0,'presenter shares actual frame owner and SDK device')
    observed=[]
    @Callback
    def done(argument):
        try:
            state=present.snapshot();pixels=C.create_string_buffer(640*480*4);err=Failure()
            observed.append(dict(copy=copy(window,state.job,C.byref(err)),read=read(window,pixels,len(pixels),C.byref(err)),matches=pixels.raw==expected,sha256=hashlib.sha256(pixels.raw).hexdigest()))
        except Exception as e:observed.append(dict(error=str(e)))
    check(present.callback(C.cast(done,P),None)==0,'actual EOR observer slot')
    present.probe(0,0);present.pacing(2,2);prior=count(window)
    for iteration in range(2):
        record();present.frame();state=present.snapshot()
        check(present.status()==present.wait_status()==0 and state.submissions==state.completions==state.presentations==iteration+1,'original frame_wait performs real GPU completion and Present')
        check(count(window)==prior+iteration+1 and not state.frame and not state.job,'real Present counter advances and frame/job owners retire')
        check(len(observed)==iteration+1 and observed[-1].get('copy')==observed[-1].get('read')==0 and observed[-1]['matches'],('EOR image equals full original-logo oracle',observed[-1] if observed else None))
    check(present.shutdown()==0 and present.wait_shutdown()==0,'presenter and logical wait drained');present.clock.shutdown()
    wc(window);gpu.close(gpu.handle)
    check([listing().contents.textures[i].filename for i in range(6)]==before,'original filename pointers preserved after repeated complete frames')
    check(free()==1 and t.live()==0,'original logo texture release frees all six surfaces')
    check(assets.shutdown()==0,'asset owner shutdown');qs();rs();t.shutdown();check(fs()==0 and pvm.shutdown()==0,'all native owners drained')
    result=dict(status='unchanged_original_logo_composition_file_PRS_GPU_original_frame_wait_and_Present_verified',checks=checks,original_logo_draw_body_unchanged=True,six_original_texture_entries=True,real_file_PRS_game_wrapper_used=True,independent_full_image_oracle=True,width=640,height=480,actual_GPU_images=3,pixels_compared=640*480*3,visible_Kamui_Presents=2,original_frame_wait_executed=True,full_game_boot_or_logo_start_executed=False,full_game_loop_executed=False,gameplay_verified=False,playable=False,retail_pixels_or_metadata_published=False,probe_build_sha256=sha(d/'build_verification.json'),asset_build_sha256=sha(build/'build_verification.json'),expected_image_sha256=hashlib.sha256(expected).hexdigest(),verifier_sha256=sha(Path(__file__)),overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
