"""Whole original tangent/perspective and SDK display calls, without body hooks.
The pinned instruction numeric service supplies FSCA values; this is sampled
RZ/DN1 value/memory verification, not FPSCR emulation or new hardware timing.
"""
import argparse,ctypes as C,hashlib,json,math,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from native_video_test_support import Video,Context
from primitive_reference import make_reference
from camera_reference_numeric import service_for
from build_native_display_setup import NAMES
I=C.c_int32;U=C.c_uint32;F=C.c_float;P=C.c_void_p;S=C.c_size_t
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def fw(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def words(v):return struct.pack('<'+'I'*len(v),*v)
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','numeric-dll','report']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();b=json.loads((d/'build_verification.json').read_bytes());assert not a.report.exists()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(d/n)==h,n
    pin=json.loads((r/'outputs/sonic-adventure/jp_port_tools/jp_camera_external_inputs.json').read_bytes());assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,model_hashes=make_reference(executor);numeric=C.CDLL(str(a.numeric_dll.resolve())).sa_sh4_vector_numeric;numeric.restype=I;numeric.argtypes=[U,C.POINTER(Fpu),U];service=service_for(numeric)
    table={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);image=a.binary.read_bytes();ram[0x10000:0x10000+len(image)]=image;mmio={0xa05f8000+i:0 for i in range(0,0x2000,4)}
    lib=C.CDLL(b['frames_DLL']);assert sha(b['frames_DLL'])==b['frames_DLL_sha256'];screen=(U*5).in_dll(lib,'_nj_screen_');aspect=U.in_dll(lib,'_nj_scr_aspect_x_')
    checks=calls=0;seen=set();groups={};rng=random.Random(0x50525350);entries={table[n]:n for n in ['njTan','njSetPerspective',*NAMES,'kmProcessVertexRenderState','kmSetVertexRenderState','kmSetCullingRegister','kmiWriteRegister','njGetkmPointer']}
    def put(addr,data):at=addr&0xffffff;ram[at:at+len(data)]=data
    def read(addr,n):at=addr&0xffffff;return bytes(ram[at:at+n])
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def trace(pc,*unused):
        if pc in entries:seen.add(entries[pc])
    def original(name,arg=0,float_arg=None):
        nonlocal calls
        f=Fpu();bank=calls&1;f.fpscr=0x40001|(bank<<21);state=executor.State();state.pc=table[name];state.pr=0x8cfed000;state.r[15]=0x8cf00000;state.sr=0x70000000;state.r[4]=arg&0xffffffff
        if float_arg is not None:f.banks[bank][4]=float_arg
        reference(ram,state,fpu=f,mmio=mmio,vector_instruction=service,instruction_budget=100000,trace=trace);calls+=1
        check(state.r[15]==0x8cf00000,'original stack restored');return f.banks[(f.fpscr>>21)&1][0]
    tangent=bind(lib,'njTan',F,I);raw_tangent=bind(lib,'jp_projection_tangent_word',U,U);perspective=bind(lib,'njSetPerspective',None,I)
    for angle in range(65536):check(fw(tangent(angle))==original('njTan',angle),('all 65536 tangent angle words',angle))
    for angle in [0xffffffff,0x80000000,0x7fffffff,0xffff0000]+[rng.getrandbits(32) for _ in range(128)]:
        want=original('njTan',angle);check(fw(tangent(C.c_int32(angle).value))==raw_tangent(angle)==want,'actual export and raw diagnostic preserve 16-bit angle periodicity')
    groups['complete_16bit_tangent_domain_and_highword_periodicity']=checks
    # Signed zero, denormal flush, ordinary widths, infinities and both NaN
    # classes. No host float conversion is used to seed or read screen words.
    width_words=[0,0x80000000,1,0x80000001,0x007fffff,0x00800000,fw(320),fw(640),fw(1920),fw(7680),fw(-640),0x7f7fffff,0x7f800000,0xff800000,0x7fc00001,0x7f800001]
    angles=[0,1,2,0x3fff,0x4000,0x4001,0x7fff,0x8000,0x8001,0xffff,0x10000,0x1ffff,0x80000000,0xffffffff,0x31c7]+[rng.getrandbits(32) for _ in range(65)]
    for angle in angles:
        for width in width_words:
            seed=[rng.getrandbits(32),width,rng.getrandbits(32),rng.getrandbits(32),rng.getrandbits(32)];screen[:]=seed;put(table['_nj_screen_'],bytes(screen));aspect.value=rng.choice([fw(.5),fw(1),fw(2),0x7f800000]);saved_aspect=aspect.value;put(table['_nj_scr_aspect_x_'],words([saved_aspect]))
            original('njSetPerspective',angle);perspective(C.c_int32(angle).value)
            check(bytes(screen)==read(table['_nj_screen_'],20),'all screen words match original perspective, including exceptional classes')
            check(list(screen)[1:]==seed[1:] and aspect.value==saved_aspect,'perspective updates only distance and deliberately omits screen aspect')
    groups['perspective_screen_value_memory_and_exceptional_widths']=checks-sum(groups.values())
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);status=bind(lib,'jp_queue_status',I);video=Video(lib);initialize=bind(lib,'jp_ninja_contexts_initialize',I);count=bind(lib,'jp_queue_context_count',U);select=bind(lib,'njSetCurrentContext',None,I);getctx=bind(lib,'jp_queue_get_context',I,U,P);get=bind(lib,'njGetkmPointer',None);parameter=bind(lib,'jp_queue_get_parameter',I,P)
    check(qi(8<<20)==0 and video.init(640,480,0xff000000,1)==0 and initialize()==0,'actual shared queue/video/context owners')
    current=P.in_dll(lib,'_nj_current_ctx_');CTX=0x8cd00000;CURSORS=0x8cd02000;put(table['_nj_vertex_buffer_'],words([CURSORS]));put(CURSORS,bytes(20));put(table['_nj_current_ctx_'],words([CTX]))
    def layout(ctx):
        assert ctx.texture is None
        return bytes(ctx.prefix)+words([0])+bytes(ctx.face)+bytes(ctx.offset)+bytes(ctx.words)+bytes(ctx.bounds)+bytes(ctx.tail)
    domains={'njTextureFilterMode':[0,1,2,-1,3,99],'njSuperSampleMode':[0,1,-1,2,99],'njTextureClampMode':[0,1,2,3,-1,4,99],'njTextureFlipMode':[0,1,2,3,-1,4,99],'njPolygonCullingMode':[0,1,2,3,-1,4,99]}
    context_count=count();setter_cases=0
    for index in range(context_count):
        for list_code in [0,2,4]:
            for name,values in domains.items():
                check(initialize()==0,'restore genuine SDK defaults');select(index);ctx=Context.from_address(current.value);ctx.words[0]=(ctx.words[0]&~0x07000000)|(list_code<<24);put(CTX,layout(ctx))
                fn=bind(lib,name,None,I)
                for value in values:
                    original(name,value);fn(value);setter_cases+=1
                    check(status()==0 and layout(ctx)==read(CTX,192),('actual original display context fields',name,value,index,list_code))
                    out=(U*4)();check(getctx(index,out)==0 and bytes(out)==read(CTX+144,16),'native Ninja context header agrees with original metadata')
                    original('njGetkmPointer');get();check(parameter(out)==0 and bytes(out)==read(table['_nj_parameterkm_'],16),'actual original/native Ninja parameter words after display setter')
    cull=bind(lib,'njPolygonCullingSize',None,F)
    for value in [0,0x80000000,1,0x80000001,fw(.05),fw(-.05),fw(1e20),0x7f800000,0xff800000,0x7fc00001,0xffbfffff]:
        original('njPolygonCullingSize',float_arg=value);cull(F.from_buffer_copy(words([value])));check(video.snapshot().cull_bits==mmio[0xa05f8078]==(value&0x7fffffff),'real original culling magnitude register bits')
    check(set(entries.values())<=seen,('all real dependencies reached without body hooks',sorted(set(entries.values())-seen)))
    groups['all_six_shared_contexts_display_enums_headers_and_cull_register']=checks-sum(groups.values())
    qs();video.shutdown()
    report=dict(status='original_tangent_perspective_and_display_setup_verified',checks=checks,original_calls=calls,original_dependency_hooks_used=False,original_dependencies_seen=sorted(seen),groups=groups,all_65536_tangent_angles_verified=True,tangent_highword_cases=132,perspective_cases=len(angles)*len(width_words),original_context_count=context_count,polygon_list_codes=[0,2,4],original_display_setter_cases=setter_cases,exceptional_screen_width_classes_verified=True,perspective_changes_only_distance=True,perspective_aspect_omission_preserved=True,actual_Ninja_parameter_headers_verified=True,separate_Kamui_global_header_transport_verified=False,sampled_value_context='single RZ/DN1, traps disabled, alternating FR banks',FSCA_uses_previously_qualified_instruction_value_model=True,new_physical_hardware_or_FPSCR_emulation_verified=False,original_binary_sha256=sha(a.binary),numeric_model_DLL_sha256=sha(a.numeric_dll),reference_model_hashes=model_hashes,build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),full_system_boot_or_main_loop_executed=False,playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
