"""Original light instructions, complete real Ninja defaults and busy guards."""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime
from native_video_test_support import Video,Context
from model_reference import make_reference
I=C.c_int32;U=C.c_uint32;P=C.c_void_p;Q=C.c_uint64;S=C.c_size_t;F=C.c_float
class Info(C.Structure):_fields_=[(n,U) for n in ('slots','material_bytes','control_bytes','queue_bytes','source_bytes','destination_bytes','pointer_bytes','status')]
class Control(C.Structure):_fields_=[('lights',P*32),('count',I)]
class LightQueue(C.Structure):_fields_=[('lights',I*32),('count',I)]
class Screen(C.Structure):_fields_=[(n,F) for n in ('distance','width','height','cx','cy')]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();out=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not out.exists() and out.is_relative_to(r/'work/sonic-adventure') and not out.is_relative_to(r/'work/sonic-adventure/build')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(a.build/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'] and sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    out.mkdir();t=Runtime(Path(b['frames_DLL']));lib=t.dll;v=Video(lib)
    initialize=bind(lib,'jp_ninja_global_initialize',I);raw_init=bind(lib,'njInitGlobal',None);status=bind(lib,'jp_ninja_global_status',I);snapshot=bind(lib,'jp_ninja_global_snapshot',I,P)
    light_init=bind(lib,'njLightSystemInit',None);screen=Screen.in_dll(lib,'_nj_screen_');control=Control.in_dll(lib,'_nj_light_control_');queue=LightQueue.in_dll(lib,'_nj_light_queue_');material=(C.c_ubyte*52).in_dll(lib,'_nj_light_material_')
    source=(U*16).in_dll(lib,'_nj_cnk_easy_multilight_buffer_');destination=(U*32).in_dll(lib,'_nj_cnk_easy_multilight_buffer_dst_');motion=P.in_dll(lib,'_nj_motion_callback_')
    checks=[];calls=0;seen=set();rng=random.Random(0x4e4a474c)
    def check(ok,label):assert ok,label;checks.append(label)
    info=Info();check(snapshot(C.byref(info))==0 and list(bytes(info)) and (info.slots,info.material_bytes,info.control_bytes,info.queue_bytes,info.source_bytes,info.destination_bytes,info.pointer_bytes)==(32,52,264,132,64,128,8),'actual typed native light layouts and unchanged scalar widths')
    objects=[control,queue,material,source,destination]
    check(all(C.addressof(obj)%32==0 for obj in objects),'all original assembly-reserved light blocks remain 32-byte aligned')
    before=bytes(screen)+bytes(control)+bytes(queue)+bytes(material)+bytes(source)+bytes(destination)
    check(initialize()==1,'uninitialized render owners reject real global init');raw_init()
    check(status()==1 and before==bytes(screen)+bytes(control)+bytes(queue)+bytes(material)+bytes(source)+bytes(destination),'SDK entry guard preserves actual state when prerequisites are missing')
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,_=make_reference(executor);table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary
    def put(at,data):at&=0xffffff;ram[at:at+len(data)]=data
    def rd(at,count):return bytes(ram[at&0xffffff:(at&0xffffff)+count])
    def trace(pc,*unused):
        if pc==table['njLightSystemInit']:seen.add('njLightSystemInit')
    for case in range(8):
        raw=rng.randbytes(52);C.memmove(material,raw,52);put(table['_nj_light_material_'],raw)
        raw_queue=rng.randbytes(132);C.memmove(C.byref(queue),raw_queue,132);put(table['_nj_light_queue_'],raw_queue)
        C.memset(C.byref(control),0xa5,C.sizeof(control));guest_pointers=[]
        for i in range(32):control.lights[i]=0x1234567800000000+i*64;guest_pointers.append(0x8cc40000+i*64)
        control.count=-123;old_control=bytes(control);put(table['_nj_light_control_'],struct.pack('<32Ii',*guest_pointers,-123))
        cpu=executor.State();cpu.pc=table['njLightSystemInit'];cpu.pr=0x8cfed000;cpu.r[15]=0x8cf00000;cpu.sr=0x70000000;fpu=Fpu();fpu.fpscr=0x40001
        reference(ram,cpu,fpu=fpu,trace=trace,instruction_budget=100000);calls+=1;light_init()
        check(cpu.r[15]==0x8cf00000 and bytes(material)==rd(table['_nj_light_material_'],52),'complete original lighting initializer matches every material byte')
        check(bytes(queue)==rd(table['_nj_light_queue_'],132)==bytes(132),'original light queue clears integer indices and count')
        expected=bytearray(old_control);expected[Control.count.offset:Control.count.offset+4]=bytes(4)
        check(bytes(control)==bytes(expected) and rd(table['_nj_light_control_'],128)==struct.pack('<32I',*guest_pointers) and rd(table['_nj_light_control_']+128,4)==bytes(4),'original/native pointer-bearing light control preserves all slots/padding and clears only count')
        check(bytes(material)==struct.pack('<13f',1,0,0,0,1,.5,.5,.5,1,.5,.5,.5,6),'independent original material defaults oracle')
    check(seen=={'njLightSystemInit'},'original light body executed without hooks')
    qi=bind(lib,'jp_queue_initialize',I,S);fi=bind(lib,'jp_frames_initialize',I,U,U,S);reset=bind(lib,'jp_queue_reset_frame',I)
    check(qi(8<<20)==0 and fi(8,32,16<<20)==0 and t.init(8,32,16<<20)==0,'actual native queue/frame/texture owners initialized')
    expected_lights=[0,0,-1,0,0,0,0,0,0,0,0,0,0,.2,.2,.2,0,0,0,1,0,1,1,1,0,0,0,1,65535,1,1,1,0,0,0,1,65535,1,1,1,0,0,0,1,65535,1,1,1]
    expected_src=struct.pack('<16f',*expected_lights[:16]);expected_dst=bytearray(struct.pack('<32f',*expected_lights[16:]));struct.pack_into('<I',expected_dst,0,3)
    context_count=bind(lib,'jp_queue_context_count',U)()
    color_modes=(U*context_count).in_dll(lib,'_nj_ctx_color_mode_');clip=(F*4).in_dll(lib,'_nj_2d_clip_');getctx=bind(lib,'jp_queue_get_context',I,U,P)
    counters={n:I.in_dll(lib,n) for n in ('_nj_calc_vertex_','_nj_calc_polygon_','_nj_draw_polygon_','_nj_vsync_count_')}
    default=[0x80800012,0xc8800000,0x948004c0,0]
    for width,height in [(320,240),(640,480),(1280,720),(641,479)]:
        check(v.init(width,height,0xff000000,1)==0,'actual logical video canvas configured')
        for value in counters.values():value.value=12345
        motion.value=0x123456789abcdef0;C.memset(source,0xa5,64);C.memset(destination,0x5a,128)
        control.count=19;queue.count=32;screen.distance=1;screen.cx=-8;screen.cy=-9
        check(initialize()==0 and status()==0,'complete original global init succeeds with actual quiescent owners')
        check((screen.width,screen.height,screen.cx,screen.cy,screen.distance)==(width,height,width/2,height/2,500),'original logical screen dimensions/center/distance defaults')
        check(list(clip)==[0,0,width-1,height-1],'original two-dimensional clip rectangle')
        for n,value in counters.items():check(value.value==0,'original global counter resets '+n)
        check(F.in_dll(lib,'_nj_scr_aspect_x_').value==F.in_dll(lib,'_nj_scr_aspect_y_').value==1 and F.in_dll(lib,'_nj_near_clip_').value==1 and F.in_dll(lib,'_nj_far_clip_').value==60000,'original aspect and near/far defaults')
        check(U.in_dll(lib,'_nj_control_3d_flag_').value==0 and motion.value is None,'original render-control and native callback pointer resets')
        check(bytes(source)==expected_src and bytes(destination)==bytes(expected_dst),'all 48 original easy-light float words and integer marker match independent oracle')
        check(control.count==queue.count==0 and bytes(material)==struct.pack('<13f',1,0,0,0,1,.5,.5,.5,1,.5,.5,.5,6),'complete global entry actually executes original lighting setup')
        for index in range(bind(lib,'jp_queue_context_count',U)()):
            words=(U*4)();check(getctx(index,words)==0 and list(words)==default,'actual original SDK context header defaults '+str(index))
        check(all(n==0x00080012 for n in color_modes),'all original context color modes initialized')
        check(P.in_dll(lib,'_nj_current_ctx_').value==C.addressof((C.c_ubyte*1).in_dll(lib,'_nj_context_')),'current context uses original shared slot zero')
        check(v.snapshot().cull_bits==struct.unpack('<I',struct.pack('<f',.01))[0],'original culling default reaches actual video state owner')
    # Real active producer: init must fail before touching original state.
    check(bind(lib,'jp_queue_reserve',I,U,U)(0,1)==0,'actual producer space reserved');bind(lib,'njGetkmPointer',None)();check(bool(bind(lib,'njStartVertexNonTexOpaque',P)()),'actual producer opened')
    saved=bytes(screen)+bytes(source)+bytes(destination)+bytes(control)+bytes(queue)
    check(initialize()==2 and saved==bytes(screen)+bytes(source)+bytes(destination)+bytes(control)+bytes(queue),'active producer rejects reinitialization without partial global writes')
    check(bind(lib,'jp_queue_discard_active',I)()==0,'active producer discarded');bind(lib,'njSetkmPointer',None)();check(reset()==0,'real queue reset')
    check(v.init(640,480,0xff000000,1)==0 and initialize()==0,'guarded initialization recovers after producer release')
    bind(lib,'njInitPrint',None,P,U,U)(None,0,0);bind(lib,'text_style_init',None,P,U,U)(None,0,0);bind(lib,'text_print',None,I,P)(0x10001,C.create_string_buffer(b'NINJA GLOBAL INIT'))
    frame=Q();check(bind(lib,'jp_frames_submit',I,P)(C.byref(frame))==0,'actual original game debug-text frame submitted')
    saved=bytes(screen)+bytes(source)+bytes(destination)+bytes(control)+bytes(queue)
    check(initialize()==2 and saved==bytes(screen)+bytes(source)+bytes(destination)+bytes(control)+bytes(queue),'submitted frame lifetime rejects destructive global reset')
    check(bind(lib,'jp_frame_release',I,Q)(frame.value)==0,'real submitted frame released');bind(lib,'njExitPrint',None)();check(initialize()==0,'full original init recovers after submitted work and font are released')
    check(bind(lib,'jp_frames_shutdown',I)()==0,'real frame owner drained');bind(lib,'jp_queue_shutdown',None)();t.shutdown();bind(lib,'jp_video_shutdown',None)()
    result=dict(status='complete_original_Ninja_global_defaults_light_instruction_semantics_and_active_work_guards_verified',checks=len(checks),check_labels=checks,complete_original_light_machine_calls=calls,original_light_function_hooks_used=False,all_48_easy_light_words_and_marker_verified=True,typed_native_pointer_control_and_integer_queue_verified=True,all_original_data_blocks_aligned_32=True,actual_context_headers_and_video_cull_state_verified=True,active_producer_and_submitted_frame_guards=True,real_debug_text_frame_used_for_guard=True,complete_original_global_body_executed_through_native_wrapper=True,complete_global_original_machine_comparison_claimed=False,native_vsnyc_callback_quiescence_is_caller_precondition=True,build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),full_njInitSystem_or_sbInitSystem_or_gameplay_verified=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(checks),'checks',flush=True)
if __name__=='__main__':main()
