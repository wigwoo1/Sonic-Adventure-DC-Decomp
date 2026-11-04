"""Real SDK context/blending and original black/white title-fade composition."""
import argparse,ctypes as C,hashlib,json,re
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,List,Record,Surface
from native_asset_test_support import Assets,Limits
from native_pvm_test_support import PVM,index_archive,image_oracle
from native_video_test_support import Video,Context
from native_gpu_test_support import GPU,Config,Report,Failure
from native_present_test_support import Present
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64;Callback=C.CFUNCTYPE(None,P)
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','probe','inputs','asset-root','report']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();b=json.loads((d/'build_verification.json').read_text());pb=json.loads((a.probe/'build_verification.json').read_text());assert not a.report.exists()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ['frames_DLL','GPU_DLL','DLL']:assert sha(b[n])==b[n+'_sha256']
    assert pb['asset_build_sha256']==sha(d/'build_verification.json') and pb['frame_owner_DLL_sha256']==b['frames_DLL_sha256'] and sha(pb['DLL'])==pb['DLL_sha256']
    manifest=json.loads((a.inputs/'input_manifest.json').read_text());row=next(v for v in manifest['assets'] if v['disc_path']=='SONICADV/SEGALOGO.PRS');assert sha(row['PVM'])==row['PVM_sha256'] and sha(a.asset_root/'SEGALOGO.PRS')==row['compressed_sha256']
    flags,entries,images=index_archive(Path(row['PVM']).read_bytes());tiles=[image_oracle(image,256,256,True) for image in images];metadata=a.probe/'inputs/progress/demo/logo_texanims.inc'
    assert sha(metadata)==pb['private_generated_metadata']['logo_texanims.inc']['sha256']
    animations=[list(map(int,re.findall(r'-?\d+',line))) for line in metadata.read_text().splitlines() if '{' in line];base=bytearray(640*480*4);coverage=bytearray(640*480)
    for sx,sy,cx,cy,u1,v1,u2,v2,index,attr in animations:
        assert (sx,sy,u1,v1,u2,v2,attr)==(256,256,0,0,256,256,0);left=320-cx;top=-cy;tile=tiles[entries[index]['number']]
        for y in range(max(0,top),min(480,top+sy)):
            for x in range(max(0,left),min(640,left+sx)):
                at=y*640+x;src=((y-top)*256+x-left)*4;assert not coverage[at];coverage[at]=1;base[at*4:at*4+4]=tile[src:src+4]
    assert all(coverage);base=bytes(base)
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;v=Video(lib);assets=Assets(lib);pvm=PVM(lib);gpu=GPU(Path(b['GPU_DLL']));probe=C.CDLL(pb['DLL']);checks=images_count=pixels=0;groups={};keep=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);qc=bind(lib,'jp_queue_clear_error',None);queue_status=bind(lib,'jp_queue_status',I)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q);ri=bind(lib,'jp_render_state_initialize',I,U,S);rs=bind(lib,'jp_render_state_shutdown',None)
    init=bind(lib,'jp_ninja_contexts_initialize',I);getctx=bind(lib,'jp_queue_get_context',I,U,P);countctx=bind(lib,'jp_queue_context_count',U);select=bind(lib,'njSetCurrentContext',None,I);blend=bind(lib,'njColorBlendingMode',None,I,I);process=bind(lib,'kmProcessVertexRenderState',I,P);initialize=bind(lib,'njInitContext',None,P);apply=bind(lib,'kmSetVertexRenderState',I,P)
    setlevel=bind(lib,'title_fade_set',None,I);step=bind(lib,'title_fade_step',I,I);level=bind(lib,'jp_title_fade_level',I);draws=[bind(lib,'title_fade_draw',None),bind(lib,'title_fade_draw_white',None)]
    check(qi(8<<20)==0 and fi(8,128,32<<20)==0 and t.init(32,1024,32<<20)==0 and v.init(640,480,0xff000000,1)==0 and ri(128,1<<20)==0,'real shared queue/frame/texture/video/render owners')
    check(init()==0,'actual original Ninja context defaults and SDK header generation');current=P.in_dll(lib,'_nj_current_ctx_')
    default=[0x80800012,0xc8800000,0x948004c0,0]
    for index in range(countctx()):
        words=(U*4)();check(getctx(index,words)==0 and list(words)==default,('SDK default context header bits',index,list(words)));select(index);check(queue_status()==0 and list(Context.from_address(current.value).words)[:4]==default,'real context switch retains header owner')
    select(0);ptr=current.value;colors=P.in_dll(lib,'_nj_color_mode_');color_ptr=colors.value
    for index in [-1,countctx(),1000]:
        select(index);check(queue_status()!=0 and current.value==ptr and colors.value==color_ptr,'invalid context selection preserves both owned pointers');qc()
    bad=Context.from_buffer_copy(bytes(Context.from_address(ptr)));bad.prefix[0]=0x4000;bad.prefix[21]=4;before=bytes(bad)
    check(process(C.byref(bad))!=0 and bytes(bad)==before,'invalid filter cannot index outside SDK table or mutate context');check(process(None)!=0 and apply(None)!=0,'null context APIs reject safely')
    factors=[[(5,4),(4,5),(6,5),(2,5),(7,5),(3,5),(5,5),(4,5),(4,5),(4,5),(1,5),(0,5)],[(5,4),(4,5),(4,6),(4,5),(4,7),(4,5),(4,5),(4,3),(4,4),(4,2),(4,1),(4,0)]]
    for target in range(2):
        for mode,(source,destination) in enumerate(factors[target]):
            check(init()==0,'real defaults restored before blend API');blend(target,mode);ctx=Context.from_address(current.value);tsp=ctx.words[2]
            check(queue_status()==0 and (tsp>>29&7,tsp>>26&7)==(source,destination),'original Ninja blend enum maps through actual SDK header processor')
            check((ctx.words[0],ctx.words[1],ctx.words[3])==(default[0],default[1],0),'blend-only update preserves unrelated header words')
    check(init()==0,'native default state restored')
    # Active producers own their header until commit/cancel.
    reserve=bind(lib,'jp_queue_reserve',I,U,U);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None);start=bind(lib,'njStartVertexNonTexOpaque',P);discard=bind(lib,'jp_queue_discard_active',I)
    check(reserve(0,1)==0,'actual queue reservation');get();check(bool(start()),'actual active producer');saved=bytes(Context.from_address(current.value));initialize(current)
    check(queue_status()!=0 and bytes(Context.from_address(current.value))==saved,'context initialization cannot mutate an active producer');qc();check(discard()==0,'active probe discarded');put();check(reset()==0 and init()==0,'owner restored after guarded rejection')
    groups['original_context_defaults_blending_guards_and_single_owner']=checks
    setlevel(0)
    for n in range(1,33):check(step(8)==0 and level()==8*n,'fade-in reaches overshoot before original pre-add clamp')
    check(step(8)==1 and level()==255,'original positive clamp completion');check(step(8)==0 and level()==263 and step(8)==1 and level()==255,'original plateau alternates overshoot and clamp')
    setlevel(263)
    for expected in range(255,-34,-8):check(step(-8)==0 and level()==expected,'original fade-out crosses black and reaches minus 33')
    check(step(-8)==-1 and level()==0,'original minus-30 threshold ends on following call');setlevel(-30);check(step(-8)==0 and level()==-38 and step(-8)==-1 and level()==0,'exact negative boundary and completion')
    for n in [-64,0,128,255,256,300]:setlevel(n);check(step(0)==0 and level()==n,'zero step preserves state beyond clamp thresholds')
    groups['original_title_fade_state_and_pre_add_completion_boundaries']=checks-sum(groups.values())
    limits=Limits(4<<20,16<<20,32<<20,64);check(assets.init(str(a.asset_root.resolve()),C.byref(limits))==0,'real native file/PRS asset path')
    load=bind(probe,'jp_logo_asset_load',I);logo=bind(probe,'jp_logo_asset_draw',None);listing=bind(probe,'jp_logo_asset_list',C.POINTER(List));free=bind(probe,'jp_logo_asset_release',I)
    check(load()==0 and pvm.loaded(listing())==6,'original adapted game wrapper loads supplied logo')
    record=Record.from_address(t.lookup(listing(),0));metadata_context=Context();initialize(C.byref(metadata_context));metadata_context.prefix[0]=8;metadata_context.prefix[8]=3;metadata_context.texture=C.addressof(record.texinfo.texsurface)
    check(process(C.byref(metadata_context))==0 and metadata_context.words[2]&63==record.tspparambuffer&63 and metadata_context.words[3]==record.texparambuffer,'real context generator uses actual native texture token and metadata')
    fake=Surface.from_buffer_copy(bytes(record.texinfo.texsurface));fake.pSurface=0x1234;metadata_context.texture=C.addressof(fake);before=bytes(metadata_context)
    check(process(C.byref(metadata_context))!=0 and bytes(metadata_context)==before,'foreign texture surface rejected without host-pointer truncation');t.clear()
    check(init()==0,'real native defaults after metadata probes');bind(lib,'njSetBackColor',None,U,U,U)(0,0,0)
    config=Config.make(640,480);config.logical_width=640;config.logical_height=480;bound=bind(gpu.lib,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P)
    def expected(n,white=False,off=False):
        original=bytes([255,255,255,255])*(640*480) if off else base
        if n>=255:return original
        if n<=0:return bytes([255 if white else 0]*3+[255])*(640*480)
        table=bytes(255-round((255-c)*n/255) if white else round(c*n/255) for c in range(256));result=bytearray(original.translate(table));alpha=255-n;result[3::4]=bytes([round((alpha*alpha+255*n)/255)])*(640*480);return bytes(result)
    def record_frame(n,white=False):
        check(reset()==0,'actual frame recording reset');setlevel(n);draws[1 if white else 0]();logo();check(queue_status()==t.status()==0,'original title-fade and logo packet producers')
    def compare(actual,want,tolerance=1):
        worst=max(abs(x-y) for x,y in zip(actual,want));check(len(actual)==len(want) and worst<=tolerance,('independent composed fade/logo pixel oracle',worst,tolerance))
    cases=[-33,0,1,8,64,128,192,247,254,255,256,263]
    for white in [False,True]:
        for n in cases:
            record_frame(n,white);frame=Q();check(submit(C.byref(frame))==0,'actual immutable composed frame');out=C.create_string_buffer(640*480*4);report=Report();failure=Failure()
            check(bound(gpu.handle,frame.value,C.byref(config),out,len(out),C.byref(report),C.byref(failure))==0,('actual composed GPU image',n,white,failure.code))
            compare(out.raw,expected(n,white),0 if n<=0 or n>=255 else 1);check(release(frame.value)==0,'composed frame retired');images_count+=1;pixels+=640*480
    off=Config.make(640,480,textures=0);off.logical_width=640;off.logical_height=480;record_frame(128);frame=Q();check(submit(C.byref(frame))==0,'actual Texture Off frame');out=C.create_string_buffer(640*480*4);report=Report();failure=Failure()
    check(bound(gpu.handle,frame.value,C.byref(off),out,len(out),C.byref(report),C.byref(failure))==0,'actual Off composed render');compare(out.raw,expected(128,off=True));check(report.texture_uploads==report.shader_texture_bindings==report.shader_sampler_bindings==0,'Off fade/logo composition has no sampled texture bindings');check(release(frame.value)==0,'Off frame retired');images_count+=1;pixels+=640*480
    groups['original_black_white_fades_native_blending_and_actual_logo_images']=checks-sum(groups.values())
    present=Present(Path(b['DLL']));gl=gpu.lib;window=P();failure=Failure();device=C.addressof((C.c_byte*1).in_dll(lib,'kmiDeviceExtension'));present.device(device)
    wo=bind(gl,'jp_window_open',I,P,U,U,I,P,P);wc=bind(gl,'jp_window_close',None,P);copy=bind(gl,'jp_window_copy_job',I,P,Q,P);read=bind(gl,'jp_window_readback',I,P,P,S,P);presented=bind(gl,'jp_window_presented',Q,P)
    check(wo(gpu.handle,640,480,1,C.byref(window),C.byref(failure))==0,'real native fade window');check(present.clock.init(None,None,0)==0 and present.wait_init(60000,1001,8,C.cast(present.window_idle,P),window)==0 and present.init(gpu.handle,window,device,C.byref(config),5000,0)==0,'real original frame-wait host and presenter')
    observed=[]
    @Callback
    def done(argument):
        try:
            state=present.snapshot();rgba=C.create_string_buffer(640*480*4);error=Failure();observed.append((copy(window,state.job,C.byref(error)),read(window,rgba,len(rgba),C.byref(error)),rgba.raw))
        except Exception as e:observed.append(str(e))
    check(present.callback(C.cast(done,P),None)==0,'real EOR observer');present.probe(0,0);present.pacing(2,2)
    for index,n in enumerate([0,128,255]):
        record_frame(n);present.frame();state=present.snapshot();check(present.status()==present.wait_status()==0 and state.submissions==state.completions==state.presentations==index+1 and presented(window)==index+1,'original frame_wait completes actual faded GPU job and Present')
        check(len(observed)==index+1 and observed[-1][:2]==(0,0),'EOR reads actual completed faded image');compare(observed[-1][2],expected(n));images_count+=1;pixels+=640*480
    check(present.shutdown()==0 and present.wait_shutdown()==0,'presentation owners drained');present.clock.shutdown();wc(window);gpu.close(gpu.handle)
    check(free()==1 and t.live()==0 and assets.shutdown()==0,'all original logo textures and assets released');qs();rs();t.shutdown();check(fs()==0 and pvm.shutdown()==0,'all native owners drained')
    groups['original_frame_wait_faded_images_and_real_Present']=checks-sum(groups.values())
    result=dict(status='original_black_white_title_fades_real_Ninja_contexts_texture_tokens_GPU_logo_composition_and_Present_verified',checks=checks,groups=groups,original_fade_logic_retained=True,original_unsigned_alpha_shift_repairs=2,original_context_default_generation=True,actual_GPU_images=images_count,pixels_compared=pixels,visible_Kamui_Presents=3,independent_full_image_oracle=True,fade_in_pre_add_clamp_verified=True,fade_out_negative_threshold_verified=True,context_updates_guard_actual_queue_owner=True,Texture_Off_sampled_bindings_zero=True,full_original_logo_step_or_boot_executed=False,physical_hardware_RGBA_precision_verified=False,playable=False,build_report_sha256=sha(d/'build_verification.json'),probe_build_sha256=sha(a.probe/'build_verification.json'),verifier_sha256=sha(Path(__file__)),overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
