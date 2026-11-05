"""Whole original boot setter calls and native borrowed-storage boundaries."""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from native_video_test_support import Video
from model_reference import make_reference
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();out=a.output.resolve();assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();out.mkdir();b=json.loads((a.build/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(a.build/n)==h,n
    for n,h in b['kept_frame_objects_sha256'].items():assert sha(n)==h,n
    for n,h in b['boot_buffer_external_sources_sha256'].items():assert sha(a.upstream/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'];lib=C.CDLL(b['frames_DLL']);video=Video(lib)
    pin=json.loads((r/'outputs/sonic-adventure/jp_port_tools/jp_camera_external_inputs.json').read_bytes());assert sha(a.binary)==pin['original_binary_sha256']
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,refhash=make_reference(executor);table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);raw=a.binary.read_bytes();ram[0x10000:0x10000+len(raw)]=raw;mmio={0xa05f8000+i:0 for i in range(0,0x2000,4)};checks=0;calls={};rng=random.Random(0x42554646)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def put(at,data):at&=0xffffff;ram[at:at+len(data)]=data
    def read(at,n):at&=0xffffff;return bytes(ram[at:at+n])
    def original(name,args):
        state=executor.State();state.pc=table[name];state.pr=0x8cfed000;state.sr=0x70000000;state.r[15]=0x8cf00000
        for i,val in enumerate(args):state.r[4+i]=val&0xffffffff
        fpu=Fpu();fpu.fpscr=0x40001;reference(ram,state,fpu=fpu,mmio=mmio,instruction_budget=10000);calls[name]=calls.get(name,0)+1;check(state.r[15]==0x8cf00000,'whole original entry restores stack')
    init=bind(lib,'jp_render_state_initialize',I,U,S);stop=bind(lib,'jp_render_state_shutdown',None);status=bind(lib,'jp_render_state_status',I);clear=bind(lib,'jp_render_state_clear_error',None);guards=bind(lib,'jp_render_state_check_guards',I);scratch=bind(lib,'jp_render_state_scratch',P);capacity=bind(lib,'jp_render_state_capacity',U);allocated=bind(lib,'jp_render_state_allocated_bytes',S);reset=bind(lib,'jp_render_state_reset_scratch',I);register=bind(lib,'jp_render_state_register_buffer',I,P,S,U);borrowed=bind(lib,'jp_render_state_buffer_is_borrowed',I);extent=bind(lib,'jp_render_state_buffer_bytes',S);sdk=bind(lib,'njInit3D',None,P,I)
    point=P.in_dll(lib,'_nj_vertex_buf_');count=I.in_dll(lib,'_nj_vertex_num_');storage=C.create_string_buffer(0x40000+128);address=(C.addressof(storage)+63)&~31;C.memset(address-32,0xa7,0x40000+64)
    check(register(address,0x40000,4096)==6,'uninitialized registration rejected');check(init(4,128)==0,'legacy owned initialization');owned=scratch();cost=allocated();check(cost==128 and capacity()==4 and not borrowed(),'legacy allocation/capacity')
    for pointer,size,vertices in [(None,0x40000,4096),(address+1,0x40000,4096),(address,0x3ffff,4096),(address,0x40000,0),(address,0x40000,0x80000000),(address,(1<<64)-1,4096),(owned,256,4)]:
        before=(point.value,count.value,capacity(),allocated(),borrowed());check(register(pointer,size,vertices)==1,'invalid extent/alignment/overlap rejected');clear();check((point.value,count.value,capacity(),allocated(),borrowed())==before,'failed registration preserves all ownership')
    check(register(address,0x40000,4096)==0 and point.value==owned and scratch()==owned,'registration alone does not select caller');check(C.string_at(address-32,0x40000+64)==bytes([0xa7])*(0x40000+64),'registration leaves all caller bytes unchanged')
    for pointer,vertices in [(address+32,4096),(address,0),(address,-1),(address,4097)]:
        sdk(pointer,vertices);check(status()==4 and point.value==owned and count.value==4 and not borrowed(),'unregistered or excessive SDK binding is atomic');clear()
    fields={'_nj_control_3d_flag_':4,'_nj_constant_attr_and_':4,'_nj_constant_attr_or_':4,'_nj_constant_material_':16,'_nj_calc_vertex_':4,'_nj_calc_polygon_':4,'_nj_draw_polygon_':4,'_nj_depth_near_':4,'_nj_depth_far_':4,'_nj_depth_range_':4}
    for vertices in [1,2,2047,2048,2049,4095,4096]:
        for name,length in fields.items():data=rng.randbytes(length);C.memmove(C.addressof((C.c_byte*length).in_dll(lib,name)),data,length);put(table[name],data)
        original('njInit3D',[table['texture_buffer'],vertices]);sdk(address,vertices)
        check(point.value==scratch()==address and count.value==vertices and capacity()==min(vertices,2048) and extent()==0x40000 and allocated()==cost and borrowed()==1,'SDK count and independent model safety limit')
        for name,length in fields.items():check(bytes((C.c_byte*length).in_dll(lib,name))==read(table[name],length),('whole original initialization state',name))
        check(int.from_bytes(read(table['_nj_vertex_buf_'],4),'little')==table['texture_buffer'] and int.from_bytes(read(table['_nj_vertex_num_'],4),'little')==vertices,'original relocated pointer and count')
    check(register(address,0x40000,4096)==0,'identical live registration retained');check(register(address+32,0x40000,4096)==4,'live registration cannot be replaced');clear()
    check(init(8,cost)==2,'failed reinitialization budget rejected');clear();check(scratch()==address and borrowed()==1 and allocated()==cost,'failed owned replacement retains caller binding')
    check(reset()==0 and C.string_at(address,4096*16)==bytes(4096*16),'active projected extent reset');check(C.string_at(address+4096*16,0x40000-4096*16)==bytes([0xa7])*(0x40000-4096*16),'original 64-byte-record tail not cleared');check(C.string_at(address-32,32)==C.string_at(address+0x40000,32)==bytes([0xa7])*32,'caller canaries intact')
    before=C.string_at(address-32,0x40000+64);stop();check(allocated()==0 and point.value is None and count.value==0 and borrowed()==0,'shutdown releases only owned allocation');check(C.string_at(address-32,0x40000+64)==before,'shutdown never frees or changes caller memory')
    check(init(4,1024)==0 and register(address,0x40000,4096)==0,'reuse actual owner');sdk(address,4096);check(init(8,1024)==0 and not borrowed() and capacity()==8 and point.value==scratch(),'successful owned replacement forgets registration');sdk(address,4096);check(status()==4,'stale registration rejected');clear()
    groups={'borrowed_owner_boundaries_and_whole_original_Init3D':checks};check(video.init(640,480,0xff000000,1)==0,'actual video owner')
    memory=U.in_dll(lib,'_nj_texture_memory_size');sector=U.in_dll(lib,'_nj_texture_buffer_max_size');texture=P.in_dll(lib,'_nj_texture_buffer_st');shape=P.in_dll(lib,'_nj_shape_buf_')
    texture_set=bind(lib,'njInitTextureBuffer',None,P,U);shape_set=bind(lib,'njInitShape',None,P);memory_set=bind(lib,'njSetTextureMemorySize',None,U);border_set=bind(lib,'njSetBorderColor',None,U)
    sizes=[0,1,2047,2048,2049,4095,4096,0x40000,0xffffffff]+[rng.getrandbits(32) for _ in range(96)]
    for i,size in enumerate(sizes):
        native=address+(i%4)*32 if i%5 else None;guest=table['texture_buffer']+(i%4)*32 if native else 0
        original('njInitTextureBuffer',[guest,size]);texture_set(native,size);check(texture.value==native and sector.value==int.from_bytes(read(table['_nj_texture_buffer_max_size'],4),'little')==size//2048*2048,'original sector rounding and relocated 64-bit pointer');check(int.from_bytes(read(table['_nj_texture_buffer_st'],4),'little')==guest,'original texture pointer')
        original('njSetTextureMemorySize',[size]);memory_set(size);check(memory.value==int.from_bytes(read(table['_nj_texture_memory_size'],4),'little')==size,'original texture-memory configuration word')
        original('njInitShape',[guest]);shape_set(native);check(shape.value==native and int.from_bytes(read(table['_nj_shape_buf_'],4),'little')==guest,'original shape pointer')
        original('njSetBorderColor',[size]);border_set(size);check(video.snapshot().border_color==mmio[0xa05f8040]==size,'whole original border setter and actual native video register')
    groups['whole_original_texture_shape_memory_border_setters']=checks-sum(groups.values())
    main=C.addressof((C.c_byte*0x40000).in_dll(lib,'texture_buffer'));main_shape=C.addressof((C.c_float*6144).in_dll(lib,'shape_buffer'));check(main%32==0,'actual main texture buffer alignment');check(register(main,0x40000,4096)==0,'actual main caller extent');texture_set(main,0x40000);sdk(main,4096);shape_set(main_shape);memory_set(0x11c000);border_set(0)
    check(texture.value==point.value==scratch()==main and sector.value==0x40000 and count.value==4096 and shape.value==main_shape and memory.value==0x11c000 and video.snapshot().border_color==0 and status()==0,'actual original system-boot graphics-buffer subset')
    check(guards()==0,'owner guards intact');stop();video.shutdown()
    result=dict(status='original_boot_buffer_setters_and_borrowed_scratch_lifetime_verified',checks=checks,original_calls_by_function=calls,original_entry_calls=sum(calls.values()),original_dependency_hooks_used=False,all_64bit_pointer_bindings_verified=True,borrowed_storage_never_freed_or_cleared_outside_projected_extent=True,failed_registration_and_reinitialization_atomic=True,stale_registration_rejected=True,original_main_shared_texture_and_vertex_buffer_bound=True,original_main_shape_buffer_bound=True,original_SDK_vertex_capacity=4096,retained_GCC_model_lighting_limit=2048,texture_sector_rounding_cases=len(sizes),texture_memory_word_only_not_native_GPU_VRAM_reallocation=True,whole_system_boot_or_gameplay_executed=False,reference_hashes=refhash,original_binary_sha256=sha(a.binary),build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),groups=groups,playable=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k not in ['reference_hashes','groups']}),flush=True)
if __name__=='__main__':main()
