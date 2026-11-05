"""Real device-owned startup, original frame calculations and safe shutdown."""
import argparse,ctypes as C,hashlib,json,re,struct,subprocess,sys,threading,time
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,Surface
from native_gpu_test_support import Failure
from native_present_test_support import Info as PresentInfo
from native_clock_test_support import Clock
from native_video_test_support import Video
from verify_native_vertex_lifecycle_original import Config as SystemConfig
from verify_native_ninja_global import Screen
from model_reference import make_reference
I=C.c_int32;U=C.c_uint32;Q=C.c_uint64;P=C.c_void_p;S=C.c_size_t
class Options(C.Structure):_fields_=[(n,U) for n in ('width','height','visible','interval','timeout','reserved')]+[('budget',Q)]
class Info(C.Structure):_fields_=[(n,U) for n in ('phase','status','resources','video','display','bpp','cable','width','height','render_width','render_height')]+[('count',I),('ticks',Q)]
FIELDS=['video_mode','display_mode','bpp','dither','anti_alias','display_width','display_height','picture_lines','fb_clip_x','fb_clip_y','fb_width','fb_lines','fb_read_size','fb_write_stride','fb_write_control','fb_read_control','scaler_control','y_coefficients','tiles_x','tiles_y','field_offset[0]','field_offset[1]','ta_options','skip_lines','level_adjust','unk_2b4']
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def coff_values(path,name,count):
    raw=path.read_bytes();machine,sections,stamp,start,total,opt,flags=struct.unpack_from('<HHIIIHH',raw);assert machine==0x14c and not opt
    strings=start+18*total;i=0
    while i<total:
        symbol,value,section,kind,storage,aux=struct.unpack_from('<8sIhHBB',raw,start+18*i)
        if symbol[:4]==bytes(4):at=strings+struct.unpack_from('<I',symbol,4)[0];symbol=raw[at:raw.index(0,at)]
        else:symbol=symbol.rstrip(b'\0')
        if storage==2 and symbol.decode()==name:
            assert section>0;offset=struct.unpack_from('<I',raw,20+(section-1)*40+20)[0]+value
            return struct.unpack_from('<'+'I'*count,raw,offset)
        i+=1+aux
    raise AssertionError(name)

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();out=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not out.exists() and out.is_relative_to(r/'work/sonic-adventure') and not out.is_relative_to(r/'work/sonic-adventure/build')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(a.build/n)==h,n
    for kind in ('frames','present','GPU','device','vertex_lifecycle'):assert sha(b[kind+'_DLL'])==b[kind+'_DLL_sha256']
    assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    out.mkdir();t=Runtime(Path(b['frames_DLL']));fl=t.dll;gl=C.CDLL(b['GPU_DLL']);pl=C.CDLL(b['present_DLL']);vl=C.CDLL(b['vertex_lifecycle_DLL']);dl=C.CDLL(b['device_DLL']);clock=Clock(Path(b['frames_DLL']));video=Video(fl)
    init=bind(dl,'jp_device_initialize',I,I,I,I);configure=bind(dl,'jp_device_configure',I,P);shutdown=bind(dl,'jp_device_shutdown',I);status=bind(dl,'jp_device_status',I);snapshot=bind(dl,'jp_device_snapshot',I,P)
    gpu_handle=bind(dl,'jp_device_gpu',P);window_handle=bind(dl,'jp_device_window',P)
    def state():v=Info();assert snapshot(C.byref(v))==0;return v
    checks=[];machine_calls=actual_inits=images=presents=0;screenshots=[]
    def check(ok,label):assert ok,label;checks.append(label)
    check(init(1,0,1)==4 and state().phase==state().resources==0,'missing frame owners reject device startup before creating resources')
    before=bytes(state())
    for args in ((3,0,1),(1,2,1),(1,0,0),(0x10001,0,1),(-1,0,1)):
        check(init(*args)==1 and bytes(state())==before,'unsupported mode/format/count rejects without disturbing cold state')
    options=Options(640,480,1,0,5000,0,64<<20)
    bad=Options(0,480,1,0,5000,0,64<<20)
    check(configure(C.byref(bad))==1 and configure(None)==1 and configure(C.byref(options))==0,'bounded options accept actual render size and reject invalid requests')
    check(bind(fl,'jp_queue_initialize',I,S)(8<<20)==0 and bind(fl,'jp_frames_initialize',I,U,U,S)(8,128,32<<20)==0 and t.init(32,1024,32<<20)==0 and bind(fl,'jp_render_state_initialize',I,U,S)(128,1<<20)==0,'actual borrowed queue/frame/texture/render-state owners')
    # A read-only typed peer supplies actual native fields and i686 offsets.
    # Neither target contains or replaces any game/SDK function body.
    peer=out/'typed_device_peer.c';peer.write_text('#include <kamui.h>\n__declspec(dllexport) const unsigned probe_offsets[]={sizeof(struct kmi_device_extension),'+','.join('__builtin_offsetof(struct kmi_device_extension,'+f+')' for f in FIELDS)+'};\n__declspec(dllexport) void probe_read(const volatile struct kmi_device_extension *p,unsigned *out){'+''.join('out['+str(i)+']=(unsigned)p->'+f+';' for i,f in enumerate(FIELDS))+'}\n')
    commands=[];compiler=b['commands'][0]['command'][0]
    base=[compiler,'-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(u/'sdk/include')]
    commands.append(base+['--target=i686-pc-windows-msvc','-c',str(peer),'-o',str(out/'typed_device_32.obj')])
    commands.append(base+['--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(peer),'-o',str(out/'typed_device_peer.dll')])
    for i,cmd in enumerate(commands):
        v=subprocess.run(cmd,capture_output=True);(out/f'peer_build_{i}.txt').write_bytes(v.stdout+v.stderr);check(not v.returncode and not v.stdout and not v.stderr,'actual SDK header layout peer compiles without diagnostics')
    offsets=coff_values(out/'typed_device_32.obj','_probe_offsets',len(FIELDS)+1);guest_size=offsets[0];offset=dict(zip(FIELDS,offsets[1:]));peer_lib=C.CDLL(str(out/'typed_device_peer.dll'));read=bind(peer_lib,'probe_read',None,P,P)
    device=C.addressof((C.c_byte*1).in_dll(fl,'kmiDeviceExtension'))
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,_=make_reference(executor);table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(u/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary;seen=set()
    def put(at,raw):at&=0xffffff;ram[at:at+len(raw)]=raw
    def word(at):return struct.unpack_from('<I',ram,at&0xffffff)[0]
    def iw(at,value):put(at,struct.pack('<I',value&0xffffffff))
    def trace(pc,*unused):
        for n in ('kmiFBControl','store_frame'):
            if n in table and pc==table[n]:seen.add(n)
    def original_frame_control(mode,bpp,w,h):
        nonlocal machine_calls
        start=table['kmiDeviceExtension'];put(start,bytes(guest_size))
        for n,value in [('video_mode',mode&3),('display_mode',mode),('bpp',bpp),('dither',1),('anti_alias',0),('picture_lines',480 if mode==1 else 240)]:iw(start+offset[n],value)
        cpu=executor.State();cpu.pc=table['kmiFBControl'];cpu.pr=0x8cfed000;cpu.r[15]=0x8cf00000;cpu.sr=0x70000000
        cpu.r[4]=w;cpu.r[5]=h;cpu.r[6]=bpp;cpu.r[7]=int((mode&1)!=1 and bool(mode&(4|8|32)));iw(cpu.r[15],1);iw(cpu.r[15]+4,0)
        fpu=Fpu();fpu.fpscr=0x40001;reference(ram,cpu,fpu=fpu,trace=trace,instruction_budget=100000);machine_calls+=1
        observed=(U*len(FIELDS))();read(device,observed);expected=[word(start+offset[n]) for n in FIELDS]
        check(cpu.r[15]==0x8cf00000 and list(observed)==expected,('complete original frame-control fields match actual native typed record',[(n,x,y) for n,x,y in zip(FIELDS,observed,expected) if x!=y]))
    render=bind(pl,'kmRender',U);flip=bind(pl,'kmFlipFrameBuffer',U);poll=bind(pl,'jp_present_poll',I);present_state=bind(pl,'jp_present_snapshot',I,P)
    def pstate():v=PresentInfo();assert present_state(C.byref(v))==0;return v
    live=bind(fl,'jp_frames_live_count',U);jobs=bind(gl,'jp_gpu_jobs_live',U,P);reset=bind(fl,'jp_queue_reset_frame',I)
    text=bind(fl,'text_print',None,I,P);start_font=bind(fl,'njInitPrint',None,P,U,U);font_style=bind(fl,'text_style_init',None,P,U,U)
    end_text=bind(fl,'text_exit',None);end_font=bind(fl,'njExitPrint',None);copy=bind(gl,'jp_window_copy_job',I,P,Q,P);readback=bind(gl,'jp_window_readback',I,P,P,S,P)
    def record():
        check(reset()==0,'fresh actual text recording');text(0x10001,C.create_string_buffer(b'NINJA DEVICE STARTUP'));check(bind(fl,'jp_print_status',I)()==0,'actual original game/Ninja text geometry')
    cfg=SystemConfig.in_dll(vl,'_nj_SystemConfigStruct_')
    modes=[(1,n) for n in (0,1,3,4,5)]+[(0,0),(0x34,1),(0x38,5),(2,4),(0x32,3)]
    for index,(mode,bpp) in enumerate(modes):
        C.memset(C.byref(cfg),0xa5,C.sizeof(cfg));before_cfg=SystemConfig.from_buffer_copy(bytes(cfg))
        options.width=640 if index%2==0 else 800;options.height=480 if index%2==0 else 600
        check(configure(C.byref(options))==0 and init(mode,bpp,1)==0,'complete original Ninja initializer creates actual owned native device/display');actual_inits+=1
        s=state();w=640 if mode==1 or mode&16 else 320;h=480 if mode==1 or mode&32 else 240
        check((s.phase,s.status,s.resources,s.video,s.display,s.bpp,s.width,s.height,s.render_width,s.render_height,s.count)==(4,0,63,mode&3,mode,bpp,w,h,options.width,options.height,1) and s.ticks>=2,'real native owner phases, two startup boundaries, logical/render dimensions and original format state')
        check(gpu_handle() and window_handle() and clock.status()==0 and clock.frequency()>0,'actual owned GPU/window and real performance counter')
        check(init(mode,bpp,1)==2 and configure(C.byref(options))==2 and state().resources==63,'live reinitialization/options cannot overwrite existing owners')
        screen=Screen.in_dll(fl,'_nj_screen_');check((screen.width,screen.height,screen.cx,screen.cy)==(w,h,w/2,h/2),'complete real Ninja global initializer takes actual logical canvas')
        before_cfg.size=C.sizeof(SystemConfig);before_cfg.flags=1;before_cfg.surfaces=C.addressof((P*2).in_dll(fl,'_nj_ppSurfaceArray'));before_cfg.width=before_cfg.height=0
        before_cfg.texture=U.in_dll(fl,'_nj_texture_memory_size').value;before_cfg.descriptor=C.addressof((C.c_byte*1).in_dll(fl,'_nj_vertex_buffer_'));before_cfg.vertex=None;before_cfg.words=0
        for n in range(5):before_cfg.shares[n]=0
        check(bytes(cfg)==bytes(before_cfg),'entire native typed configuration matches original explicit writes and preserves all untouched bytes/padding')
        surfaces=(P*2).in_dll(fl,'_nj_ppSurfaceArray');check(list(surfaces)==[C.addressof(Surface.in_dll(fl,n)) for n in ('_nj_framebuffer_surface0_','_nj_framebuffer_surface1_')],'original surface array points to the actual two original typed owners')
        check(U.in_dll(vl,'_nj_vertex_buffer_size_').value==0 and I.in_dll(fl,'_nj_eov_flag_').value==1 and I.in_dll(fl,'_nj_fatal_err_callback_').value==0,'original initial vertex/error flag state')
        original_frame_control(mode,bpp,w,h)
        foreign=[]
        def thread_work():foreign.extend([init(mode,bpp,1),configure(C.byref(options)),shutdown(),status(),gpu_handle(),window_handle()])
        thread=threading.Thread(target=thread_work);thread.start();thread.join();check(foreign==[3,3,3,3,None,None] and status()==0,'foreign thread cannot mutate or acquire native resource authority')
        check(bind(vl,'jp_vertex_lifecycle_initialize',I,I,I,I,I,I)(-1,0,131072,0,32768)==0,'actual next Ninja vertex-buffer initialization uses the newly created wait/device state')
        start_font(None,0,0);font_style(None,0,0);I.in_dll(fl,'frame_step').value=0;bind(fl,'text_set_color',None,U)(0xffffffff);bind(fl,'njSetBackColor',None,U,U,U)(0xff000000,0xff000000,0xff000000)
        record();check(render()==0,'real game text GPU submission after full native system initialization')
        deadline=time.monotonic()+10
        while True:
            rc=poll()
            if rc==0:break
            assert rc==12 and time.monotonic()<deadline,(rc,status());time.sleep(.001)
        ps=pstate();err=Failure();window=P(window_handle());buffer=C.create_string_buffer(options.width*options.height*4)
        check(copy(window,ps.job,C.byref(err))==0 and readback(window,buffer,len(buffer),C.byref(err))==0,'actual native backbuffer readback');images+=1
        pixels=buffer.raw;check(pixels[:4]==b'\0\0\0\xff' and b'\xff\xff\xff\xff' in pixels and pixels!=b'\0\0\0\xff'*(options.width*options.height),'actual original text produces visible pixels over the original SDK background colour')
        if index==0:
            from capture_native_boot_progress import png
            shot=out/'native-ninja-device-startup.png';png(shot,pixels,options.width,options.height);screenshots.append(dict(path=str(shot),sha256=sha(shot),actual_window_backbuffer=True,retail_asset_derived=True,gameplay=False,diagnostic_scene=True))
        check(flip()==0 and pstate().presentations==1 and live()==jobs(P(gpu_handle()))==0,'actual native Present and frame/job retirement');presents+=1
        end_text();end_font();check(shutdown()==0 and state().phase==state().resources==0 and not gpu_handle() and not window_handle() and clock.status()==4,'owned device shutdown releases native services and handles')
        check(shutdown()==0,'completed device shutdown is idempotent')
    # Real Windows quit delivery interrupts the initial wait before GPU creation.
    user=C.WinDLL('user32');kernel=C.WinDLL('kernel32');tid=bind(kernel,'GetCurrentThreadId',U);post=bind(user,'PostThreadMessageW',I,U,U,C.c_size_t,C.c_ssize_t)
    check(post(tid(),0x12,0,0)!=0 and init(1,0,1)==6,'actual WM_QUIT interrupts startup through native message/wait service')
    check(state().phase==state().resources==0 and clock.status()==4 and shutdown()==0,'partial startup failure releases clock/wait and preserves a retryable cold owner')
    check(init(1,0,1)==0,'actual device startup recovers after consumed quit message');actual_inits+=1
    start_font(None,0,0);font_style(None,0,0);I.in_dll(fl,'frame_step').value=0;record();frame=Q()
    check(bind(fl,'jp_frames_submit',I,P)(C.byref(frame))==0 and frame.value,'actual externally retained frame for shutdown ownership probe')
    check(shutdown()==13 and state().phase==5 and state().resources and live()==1,'shutdown retains owners when a caller still owns an external frame')
    check(bind(fl,'jp_frame_release',I,Q)(frame.value)==0,'actual external frame released');end_text();end_font()
    check(shutdown()==0 and state().phase==state().resources==0,'retry completes the same shutdown after external owner retires')
    # A genuine native budget rejection occurs after all device resources exist.
    options.budget=1;check(configure(C.byref(options))==0 and init(1,0,1)==0,'real device opens with deliberately insufficient render-job budget');actual_inits+=1
    start_font(None,0,0);font_style(None,0,0);I.in_dll(fl,'frame_step').value=0;record()
    check(render()!=0 and status()==10 and pstate().frame and not pstate().job,'real GPU budget failure propagates through device status without fabricated work')
    check(shutdown()==0 and live()==0,'late GPU failure drains captured frame and all owned device services');end_text();end_font()
    check(bind(fl,'jp_frames_shutdown',I)()==0,'borrowed frame owner remains independently usable and drainable');bind(fl,'jp_queue_shutdown',None)();t.shutdown()
    result=dict(status='full_original_Ninja_init_real_native_device_display_original_frame_state_and_cleanup_verified',checks=len(checks),check_labels=checks,
        actual_native_device_initializations=actual_inits,complete_original_frame_control_machine_calls=machine_calls,original_frame_control_function_hooks_used=False,
        actual_GPU_text_images=images,actual_Presents=presents,original_typed_configuration_and_single_descriptor_owner_verified=True,
        actual_partial_quit_failure_late_GPU_failure_and_retry=True,real_native_vertex_lifecycle_after_device_init=True,
        peer_commands=commands,peer_source_sha256=sha(peer),peer_DLL_sha256=sha(out/'typed_device_peer.dll'),guest_device_record_bytes=guest_size,
        screenshots=screenshots,build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),
        console_antialias_and_scanout_fidelity_verified=False,legacy_framebuffer_allocation_implemented=False,
        full_sbInitSystem_sound_main_gameplay_or_localization_verified=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(checks),'checks',flush=True)
if __name__=='__main__':main()
