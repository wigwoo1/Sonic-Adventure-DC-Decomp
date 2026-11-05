"""Whole original fog calls, real native context/register state and frame capture.

No game/SDK call hooks. Original MMIO writes are retained in the instruction
model's hardware register map, not replaced by function-level callbacks.
Visible GPU table-fog shading is explicitly outside this milestone.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from fractions import Fraction
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime
from native_video_test_support import Video,Context
from native_present_test_support import Present
from build_native_boot_fog import NAMES
U=C.c_uint32;I=C.c_int;F=C.c_float;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def fw(v):return struct.unpack('<I',struct.pack('<f',v))[0]

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();b=json.loads((d/'build_verification.json').read_bytes());assert not a.report.exists()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(d/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'] and b['diagnostic_only_fog_setters_removed']
    pin=json.loads((r/'outputs/sonic-adventure/jp_port_tools/jp_camera_external_inputs.json').read_bytes());assert sha(a.binary)==pin['original_binary_sha256']
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    runtime=Runtime(Path(b['frames_DLL']));lib=runtime.dll;video=Video(lib)
    # The window component imports the already loaded shared frame owner.
    C.CDLL(b['GPU_DLL']);present=Present(Path(b['present_DLL']))
    table={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);image=a.binary.read_bytes();ram[0x10000:0x10000+len(image)]=image;BUF=0x8cd00000;CTX=0x8cd01000
    mmio={0xa05f8000+n:0 for n in range(0,0x2000,4)};checks=calls=0;seen=set();rng=random.Random(0x464f4721)
    tracked=NAMES+['kmSetFogTableColor','kmSetFogDensity','kmSetFogTable','kmiWriteRegister','kmProcessVertexRenderState','kmSetVertexRenderState','njGetkmPointer'];entries={table[n]:n for n in tracked}
    def trace(pc,*args):
        if pc in entries:seen.add(entries[pc])
    def put(addr,data):ram[addr&0xffffff:(addr&0xffffff)+len(data)]=data
    def read(addr,n):at=addr&0xffffff;return bytes(ram[at:at+n])
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def original(name,arg=None):
        nonlocal calls
        f=Fpu();f.fpscr=0x40001|((calls%2)<<21);s=State();s.pc=table[name];s.pr=0x8cfed000;s.sr=0x70000000;s.r[15]=0x8cf00000
        if arg is not None:s.r[4]=arg
        reference(ram,s,fpu=f,mmio=mmio,instruction_budget=1000000,trace=trace);calls+=1;check(s.r[15]==0x8cf00000,'original call restores stack')
    check(video.init(64,64,0xff000000,1)==0,'real native video-register owner initialized')
    generator=bind(lib,'njGenerateFogTable',None,P);host=(U*132)(*[0xa5a5a5a5]*132);pointer=C.addressof(host)+8
    original('njGenerateFogTable',BUF);generator(pointer);actual=C.string_at(pointer,512)
    check(actual==read(BUF,512),'all generated fog float bits equal original instructions in RZ/DN1')
    check(list(host[:2])+list(host[130:])==[0xa5a5a5a5]*4,'generator writes only 128 floats')
    for i,word in enumerate(struct.unpack('<128I',actual)):
        exact=Fraction(16,(1<<(i>>4))*((i&15)+16));nearest=fw(float(exact));value=Fraction(struct.unpack('<f',struct.pack('<I',nearest))[0])
        want=nearest-int(value>exact);check(word==want,'independent exact rational fog density rounded toward zero')
    color=bind(lib,'njSetFogColor',None,U);fade=bind(lib,'njSetFadeColor',None,U);density=bind(lib,'njSetFogDensity',None,U)
    for fn,name,field,offset in [(color,'njSetFogColor','fog_color',0xb0),(fade,'njSetFadeColor','fog_color',0xb0),(density,'njSetFogDensity','fog_density',0xb8)]:
        for value in [0,0xffffffff,0xff000000,0x00ffffff,0xff12]+[rng.getrandbits(32) for _ in range(11)]:
            original(name,value);fn(value);check(getattr(video.snapshot(),field)==mmio[0xa05f8000+offset]==value,'real scalar setter agrees with original MMIO write')
    set_table=bind(lib,'njSetFogTable',None,P);samples=[]
    for case in range(16):
        values=[rng.random() for _ in range(128)] if case else [0.,1.,.5,.25,.75,1/255,127/255,254/255]*16
        native=(F*128)(*values);put(BUF,bytes(native));original('njSetFogTable',BUF);set_table(native);snapshot=video.snapshot();samples.append(native)
        for i in range(128):check(snapshot.fog_table[i]==mmio[0xa05f8200+4*i],('all real packed fog registers agree with complete original call',case,i))
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);qstatus=bind(lib,'jp_queue_status',I)
    contexts=bind(lib,'jp_ninja_contexts_initialize',I);count=bind(lib,'jp_queue_context_count',U);select=bind(lib,'njSetCurrentContext',None,I);getctx=bind(lib,'jp_queue_get_context',I,U,P)
    check(qi(8<<20)==0 and contexts()==0,'real queue and SDK context owners initialized');current=P.in_dll(lib,'_nj_current_ctx_')
    enable=bind(lib,'njFogEnable',None);disable=bind(lib,'njFogDisable',None)
    def original_layout(ctx):
        # The sole pointer in this context is NULL in these untextured cases;
        # its 32/64-bit storage width is the only layout adaptation here.
        assert ctx.texture is None
        return bytes(ctx.prefix)+struct.pack('<I',0)+bytes(ctx.face)+bytes(ctx.offset)+bytes(ctx.words)+bytes(ctx.bounds)+bytes(ctx.tail)
    context_count=count()
    get=bind(lib,'njGetkmPointer',None);parameter=bind(lib,'jp_queue_get_parameter',I,P)
    cursors=0x8cd02000;put(table['_nj_vertex_buffer_'],struct.pack('<I',cursors));put(cursors,bytes(20))
    for index in range(context_count):
        for list_type in [0,2,4]:
            check(contexts()==0,'restore genuine context defaults');select(index);ctx=Context.from_address(current.value)
            ctx.words[0]=(ctx.words[0]&~0x07000000)|(list_type<<24)
            ctx.words[2]=(ctx.words[2]&~0xfc000000)|(rng.randrange(64)<<26)
            put(table['_nj_current_ctx_'],struct.pack('<I',CTX));put(CTX,original_layout(ctx))
            for fn,name,mode in [(enable,'njFogEnable',0),(disable,'njFogDisable',2),(enable,'njFogEnable',0),(disable,'njFogDisable',2)]:
                original(name);fn();check(qstatus()==0,'real SDK context process and queue apply succeed')
                check(original_layout(ctx)==read(CTX,192),'all context fields match original after pointer-width adaptation')
                words=(U*4)();check(getctx(index,words)==0 and bytes(words)==read(CTX+144,16),'native Ninja context metadata agrees with original context metadata')
                check((ctx.words[2]>>22)&3==mode,'table/no-fog bits update actual selected context')
                original('njGetkmPointer');get();words=(U*4)();check(parameter(words)==0 and bytes(words)==read(table['_nj_parameterkm_'],16),'real original and native Ninja parameter headers agree after fog update')
    # Freeze actual video state into a real frame, then modify current state.
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q)
    check(runtime.init(8,128,2<<20)==0 and fi(8,16,2<<20)==0,'real frame and texture owners initialized')
    set_table(samples[0]);color(0xff123456);density(0xff12);frame=Q();check(submit(C.byref(frame))==0,'actual immutable fog-bearing frame submitted');frozen=bytes(video.frame_snapshot(frame.value))
    set_table(samples[-1]);color(0xffabcdef);density(0x8010);check(bytes(video.frame_snapshot(frame.value))==frozen,'pending frame retains original fog state after later setters');check(release(frame.value)==0,'real frame released')
    # Existing stage-hook fixtures still select the stage/demo state, while
    # their fog calls now execute the real SDK setters before passive events.
    vsync=bind(present.lib,'frame_vsync',None);countdown=bind(present.lib,'jp_wait_probe_countdown',None,I)
    events=bind(present.lib,'jp_wait_probe_events',U);event=bind(present.lib,'jp_wait_probe_event',U,U)
    for flags,want_color,want_events in [(0,0xff123456,[4,5,6,3,1,2]),(4,0xffabcdef,[4,3,1,2]),(20,0xffffffff,[4,5,6,3,1,2])]:
        color(0xffabcdef);set_table(samples[0]);before=bytes(video.snapshot());present.probe(0x200,flags);countdown(1);vsync();snap=video.snapshot()
        check(snap.fog_color==want_color,'actual windy hook updates or preserves real register color')
        check([event(i) for i in range(events())]==want_events,'stage events remain passive observations after actual calls')
        if flags==4:check(bytes(snap)==before,'demo-skipped fog hook preserves entire actual register state')
        else:check(list(snap.fog_table)==[0]*128,'actual stage table reaches all native registers')
    check(set(tracked)<=seen,('all real nested SDK/register/context dependencies executed without hooks',sorted(set(tracked)-seen)))
    check(fs()==0,'frame owner shutdown');runtime.shutdown();qs();video.shutdown()
    report=dict(status='original_boot_fog_family_context_registers_and_stage_hooks_verified',checks=checks,original_calls=calls,original_dependency_hooks_used=False,original_dependencies_seen=sorted(seen),original_context_cases=context_count,polygon_list_types_tested=[0,2,4],original_Ninja_parameter_headers_verified=True,separate_Kamui_global_header_transport_verified=False,native_pointer_width_adaptation_only_for_context_layout=True,all_128_generated_float_bits_and_register_words_verified=True,exact_rational_generator_oracle=True,original_FPSCR_context='sampled RZ/DN1 and alternating FR banks',immutable_frame_fog_snapshot_verified=True,stage_hooks_call_real_fog_setters_before_passive_events=True,diagnostic_only_fog_setters_removed=True,visible_GPU_table_fog_implemented=False,full_system_boot_or_main_loop_executed=False,original_binary_sha256=sha(a.binary),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
