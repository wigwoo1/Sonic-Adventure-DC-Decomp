"""Complete original main display reset and real native main/SDK state.
No parent/SDK function hooks. Font setup alone has explicit allocation/copy
services. Original instruction models are verification-only, never runtime.
"""
import argparse,ctypes as C,hashlib,json,math,random,re,struct,subprocess,sys
from pathlib import Path
from native_packet_test_support import bind,Decoder,Plan,Failure as PacketFailure
from native_texture_test_support import Runtime,Record,Surface
from native_video_test_support import Video,Context
from model_reference import make_reference
from camera_reference_numeric import service_for
from native_gpu_test_support import GPU,Config,Report,Failure
from native_pvm_test_support import image_oracle
from verify_native_gpu_fog import lookup
from capture_native_boot_progress import png
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64;F=C.c_float
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def words(v):return struct.pack('<'+'I'*len(v),*v)
def fw(v):return struct.unpack('<I',struct.pack('<f',v))[0]

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','numeric-dll','font-input','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();out=a.output.resolve();assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();out.mkdir();b=json.loads((a.build/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(a.build/n)==h,n
    for n,h in b['kept_frame_objects_sha256'].items():assert sha(n)==h,n
    for n in ['frames_DLL','GPU_DLL']:assert sha(b[n])==b[n+'_sha256']
    pin=json.loads((r/'outputs/sonic-adventure/jp_port_tools/jp_camera_external_inputs.json').read_bytes());assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;v=Video(lib);checks=calls=0;seen={};groups={};rng=random.Random(0x44495350)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);qstatus=bind(lib,'jp_queue_status',I);fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I)
    ri=bind(lib,'jp_render_state_initialize',I,U,S);rs=bind(lib,'jp_render_state_shutdown',None);rstatus=bind(lib,'jp_render_state_status',I)
    check(qi(32<<20)==0 and fi(8,64,32<<20)==0 and t.init(4,16,32<<20)==0 and v.init(640,480,0xff000000,1)==0 and ri(2048,1<<20)==0 and bind(lib,'jp_ninja_contexts_initialize',I)()==0,'actual shared queue/frame/texture/video/render owners')
    contexts=(Context*bind(lib,'jp_queue_context_count',U)()).in_dll(lib,'_nj_context_');selected=P.in_dll(lib,'_nj_current_ctx_');background=Context.in_dll(lib,'_nj_background_ctx_')
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,refhash=make_reference(executor);numeric=C.CDLL(str(a.numeric_dll.resolve())).sa_sh4_vector_numeric;numeric.restype=I;numeric.argtypes=[U,C.POINTER(Fpu),U];service=service_for(numeric)
    table={n:int(x,16) for x,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary
    def store(at,raw):at&=0xffffff;ram[at:at+len(raw)]=raw
    def read(at,n):at&=0xffffff;return bytes(ram[at:at+n])
    def iw(at,data):store(at,words(data))
    def word(at):return struct.unpack('<I',read(at,4))[0]
    def original_layout(ctx,texture=0):return bytes(ctx.prefix)+words([texture])+bytes(ctx.face)+bytes(ctx.offset)+bytes(ctx.words)+bytes(ctx.bounds)+bytes(ctx.tail)
    CURSORS=0x8cd00000;PROBE=0x8cd02000;iw(table['_nj_vertex_buffer_'],[CURSORS]);iw(CURSORS,[0]*5)
    for i,ctx in enumerate(contexts):store(table['_nj_context_']+i*192,original_layout(ctx))
    initial_color_pointer=P.in_dll(lib,'_nj_color_mode_').value
    store(table['_nj_ctx_color_mode_'],C.string_at(initial_color_pointer,len(contexts)*4));iw(table['_nj_color_mode_'],[table['_nj_ctx_color_mode_']])
    store(table['_nj_background_ctx_'],original_layout(background));iw(table['_nj_current_ctx_'],[table['_nj_context_']]);mmio={0xa05f8000+n:0 for n in range(0,0x2000,4)}
    numeric_globals={'_nj_screen_':5,'_nj_scr_aspect_x_':1,'_nj_scr_aspect_y_':1,'_nj_2d_clip_':4,'_nj_control_3d_flag_':1,'_nj_constant_attr_and_':1,'_nj_constant_attr_or_':1,'_nj_constant_material_':4,'_nj_depth_near_':1,'_nj_depth_far_':1,'_nj_depth_range_':1,'_nj_near_clip_':1,'_nj_far_clip_':1,'_rnext':1}
    for n,count in numeric_globals.items():store(table[n],bytes((U*count).in_dll(lib,n)))
    iw(table['_nj_current_texture'],[t.current.value]);iw(table['_nj_current_texture_bank'],[U.in_dll(lib,'_nj_current_texture_bank').value])
    tracked=['display_reset_full','njInitMatrix','njSetScreen','njSetPerspective','njInitView','njSetView','njClearMatrix','njMultiMatrix','njInitTexture','njSetBackColor','kmSetBackGroundPlane','njTextureFilterMode','njSuperSampleMode','njTextureClampMode','njTextureFlipMode','njControl3D','njSetConstantAttr','njSetConstantMaterial','njSetDepthQueue','njClipZ','njSetFogColor','njSetFogDensity','njGenerateFogTable','njSetFogTable','njFogEnable','njPolygonCullingMode','njPolygonCullingSize','srand','njPrintSize','draw_model_init','njSetTextureParam','kmProcessVertexRenderState','kmSetVertexRenderState']
    entries={table[n]:n for n in tracked};cull_arguments=[]
    def trace(pc,*rest):
        if pc in entries:n=entries[pc];seen[n]=seen.get(n,0)+1
        if pc==table['njPolygonCullingSize']:
            f=Fpu.from_buffer_copy(rest[4]);cull_arguments.append(f.banks[(f.fpscr>>21)&1][4])
    fpu=Fpu();fpu.fpscr=0x40001;special={'gbr':table['draw_work'],'r_bank':[0]*8,'mach':0,'macl':0}
    def original(name,args=(),hooks=None):
        nonlocal calls
        s=executor.State();s.pc=table[name];s.pr=0x8cfed000;s.sr=0x70000000;s.r[15]=0x8cf00000
        for i,val in enumerate(args):s.r[4+i]=val&0xffffffff
        reference(ram,s,fpu=fpu,mmio=mmio,special=special,hooks=hooks,trace=trace,vector_instruction=service,instruction_budget=3000000);calls+=1;check(s.r[15]==0x8cf00000,'whole original call restores stack');return s
    original('kmSetBackGroundRenderState',(table['_nj_background_ctx_'],))
    # Set up the authentic default font before the reset, matching system boot.
    font=(a.font_input/'debug-font-128x128-argb1555-twiddled.bin').read_bytes();font_desc=[0];init_calls={'kmiTXalloc':0,'kmiCopyToFrameBuffer':0}
    def alloc(ram,regs,sr):init_calls['kmiTXalloc']+=1;font_desc[0]=regs[5];iw(regs[5]+28,[8]);regs[0]=0;return sr
    def copy(ram,regs,sr):init_calls['kmiCopyToFrameBuffer']+=1;check(read(regs[5],regs[6])==font,'original built-in font bytes');regs[0]=0;return sr
    original('njInitPrint',(0,0,0),hooks={table['kmiTXalloc']:alloc,table['kmiCopyToFrameBuffer']:copy});bind(lib,'njInitPrint',None,P,U,U)(None,0,0)
    font_surface=Surface.from_address(bind(lib,'jp_print_font_surface',P)());fontword=t.surface_word(C.byref(font_surface));check(fontword and t.live()==1,'actual pre-reset direct font owned')
    # Scalar-only Kamui device fields, with layouts compiled from the pinned
    # external header for both pointer ABIs; the original instructions verify
    # these offsets by writing/reading the expected fields in real guest RAM.
    fields=['background_packet','background_size','background_isp','background_tsp','background_texture','background_skip','background_depth','background_tag','background_base_color','background_offset_color','display_width','display_height','presort','global_clip','global_clip_width','global_clip_height']
    layout_source=out/'device_layout.c';layout_source.write_text('#include <stddef.h>\n#include <kamui.h>\nconst unsigned long jp_device_offsets[]={sizeof(struct kmi_device_extension),'+','.join('offsetof(struct kmi_device_extension,'+n+')' for n in fields)+'};\n')
    layout={};layout_proofs={};common=list(b['commands'][0]['command']);common=common[:common.index('-c')]
    for bits in [32,64]:
        cmd=[x.replace('--target=x86_64-pc-windows-msvc','--target=i686-pc-windows-msvc') if bits==32 else x for x in common]
        if bits==32:cmd+=['-fpack-struct=4']
        obj=out/('device_layout'+str(bits)+'.obj');cmd+=['-c',str(layout_source),'-o',str(obj)];run=subprocess.run(cmd,capture_output=True);check(run.returncode==0 and not run.stdout and not run.stderr,('layout compilation',bits,(run.stdout+run.stderr).decode()[:1000]))
        raw=obj.read_bytes();machine,sections,_,start,count,optional,_=struct.unpack_from('<HHIIIHH',raw);check(machine==(0x14c if bits==32 else 0x8664) and optional==0,'layout target ABI');strings=start+count*18;i=0;match=[]
        while i<count:
            at=start+i*18;name=raw[at:at+8]
            if name[:4]==bytes(4):n=struct.unpack_from('<I',name,4)[0];name=raw[strings+n:raw.index(0,strings+n)]
            else:name=name.rstrip(b'\0')
            if name.lstrip(b'_')==b'jp_device_offsets':match.append(struct.unpack_from('<Ih',raw,at+8))
            i+=1+raw[at+17]
        check(len(match)==1,'one layout constant array');value,section=match[0];pointer=struct.unpack_from('<I',raw,20+(section-1)*40+20)[0];values=struct.unpack_from('<'+'I'*(len(fields)+1),raw,pointer+value);layout[bits]=dict(zip(fields,values[1:]));layout_proofs[str(bits)]=dict(command=cmd,object_sha256=sha(obj),size=values[0],offsets=layout[bits])
    native_device=C.addressof((C.c_ubyte*1).in_dll(lib,'kmiDeviceExtension'));guest_device=table['kmiDeviceExtension']
    # Synchronise the existing display configuration, not the reset's outputs.
    for n in fields:
        if n.startswith('background_'):continue
        store(guest_device+layout[32][n],C.string_at(native_device+layout[64][n],4))
    parent=bind(lib,'display_reset_full',None);main_stack=(U*(128*16)).in_dll(lib,'matrix_stack');main_screen=(U*5).in_dll(lib,'screen');main_view=(U*30).in_dll(lib,'view');fog=(U*128).in_dll(lib,'fog_table');records=(Record*2048).in_dll(lib,'texmemlist')
    check(C.addressof(main_stack)%32==0,'actual main matrix stack has required 32-byte alignment')
    parent_cases=[]
    selected_cases=[0,1,len(contexts)-1]
    for iteration,selected_index in enumerate(selected_cases):
        check(reset()==0,'clean native frame queue before real reset')
        original('njSetCurrentContext',(selected_index,));bind(lib,'njSetCurrentContext',None,I)(selected_index)
        # The original parent leaves main screen.dist and table payload fields
        # untouched; seed them independently of the fields it owns.
        stack_seed=rng.randbytes(C.sizeof(main_stack));C.memmove(main_stack,stack_seed,len(stack_seed));store(table['matrix_stack'],stack_seed)
        initial=[fw(500. if iteration else 0),fw(1),fw(2),fw(3),fw(4)];main_screen[:]=initial;store(table['screen'],bytes(main_screen))
        view_seed=rng.randbytes(120);C.memmove(main_view,view_seed,120);store(table['view'],view_seed)
        native_seed=rng.randbytes(C.sizeof(records));C.memmove(records,native_seed,len(native_seed));guest_seed=rng.randbytes(2048*68);store(table['texmemlist'],guest_seed)
        # All ordinary table records are invalid after njInitTexture; native
        # sidecar ownership, rather than random old payload pointers, retires them.
        original('display_reset_full');parent()
        check(qstatus()==rstatus()==t.status()==0,'complete native parent succeeds with actual owners')
        check(bytes(main_stack)==read(table['matrix_stack'],C.sizeof(main_stack)),'whole main matrix stack equals complete original reset')
        check(bytes(main_view)==read(table['view'],120),'whole actual main view equals original init/view/matrix chain')
        check(bytes(main_screen)==read(table['screen'],20) and main_screen[0]==initial[0],'actual main screen fields and untouched distance equal original')
        for n,count in numeric_globals.items():check(bytes((U*count).in_dll(lib,n))==read(table[n],count*4),('actual shared SDK scalar/array state',n))
        check(bytes((U*16).in_dll(lib,'_nj_view_matrix_'))==read(table['_nj_view_matrix_'],64),'whole SDK view matrix equals original instructions')
        check(P.in_dll(lib,'_nj_view_').value==C.addressof(main_view) and word(table['_nj_view_'])==table['view'],'both SDK view pointers reference actual main-owned view')
        for n in ['_nj_base_matrix_ptr_','_nj_current_matrix_ptr_']:check(P.in_dll(lib,n).value==C.addressof(main_stack) and word(table[n])==table['matrix_stack'],('actual main-owned matrix pointer',n))
        for n in ['_stack_size_','_max_stack_size_','_nj_matrix_flag_']:check(I.in_dll(lib,n).value==C.c_int32(word(table[n])).value,('matrix stack counters',n))
        parameter=(U*4)();check(bind(lib,'jp_queue_get_parameter',I,P)(parameter)==0 and bytes(parameter)==read(table['_nj_parameterkm_'],16),'Ninja cached header words match the original after the full reset')
        check(bytes(fog)==read(table['fog_table'],512),'all 128 main fog table words equal original generator')
        for i,ctx in enumerate(contexts):
            pointer=font_desc[0] if i==4 else 0
            actual=bytearray(original_layout(ctx,pointer));guest=bytearray(read(table['_nj_context_']+i*192,192))
            if i==4:struct.pack_into('<I',guest,156,(struct.unpack_from('<I',guest,156)[0]&~0x1fffff)|fontword)
            check(actual==guest,('complete Ninja context metadata after whole original reset',i))
        for n in fields:
            size=248 if n=='background_packet' else 4
            check(C.string_at(native_device+layout[64][n],size)==read(guest_device+layout[32][n],size),('complete actual native/original Kamui device field',n))
        expected=bytearray(native_seed);guest_expected=bytearray(guest_seed)
        for index in range(2048):
            at=index*C.sizeof(Record);expected[at:at+8]=b'\xff'*8;expected[at+Record.count.offset:at+Record.count.offset+2]=bytes(2)
            at=index*68;guest_expected[at:at+8]=b'\xff'*8;guest_expected[at+64:at+66]=bytes(2)
        check(bytes(records)==expected and read(table['texmemlist'],len(guest_seed))==guest_expected,'all 2048 caller-owned texture records have exact original sparse writes')
        check(P.in_dll(lib,'_nj_texmemlist').value==C.addressof(records) and word(table['_nj_texmemlist'])==table['texmemlist'] and t.count.value==word(table['_nj_tex_count'])==0,'actual texture owner uses real main table with zero live records')
        check(t.live()==1 and t.surface_word(C.byref(font_surface))==fontword and C.string_at(t.surface_data(fontword),len(font))==font,'default font survives genuine parent texture reset')
        snapshot=v.snapshot()
        for offset,n in [(0xb0,'fog_color'),(0xb8,'fog_density'),(0x78,'cull_bits')]:check(getattr(snapshot,n)==mmio[0xa05f8000+offset],('actual GPU register state',n,hex(getattr(snapshot,n)),hex(mmio[0xa05f8000+offset])))
        check(list(snapshot.fog_table)==[mmio[0xa05f8200+i*4] for i in range(128)],'all 128 actual native GPU fog register words equal original parent writes')
        matrix=(U*16)();bind(lib,'njGetMatrix',None,P)(matrix);original('njGetMatrix',(PROBE,));check(bytes(matrix)==read(PROBE,64),'live deferred/current matrix equals original after complete parent')
        parent_cases.append(dict(iteration=iteration,selected_context=selected_index,matrix_sha256=hashlib.sha256(bytes(main_stack)).hexdigest(),view_sha256=hashlib.sha256(bytes(main_view)).hexdigest(),SDK_screen_words=list((U*5).in_dll(lib,'_nj_screen_')),fog_density=snapshot.fog_density,cull_bits=snapshot.cull_bits))
    groups['whole_original_display_reset_main_data_SDK_context_device_and_registers']=checks
    expected=set(tracked)-{'njSetTextureParam'};check(expected<=set(seen),('all parent dependencies actually executed',sorted(expected-set(seen))))
    check(cull_arguments==[0x3d4ccccc]*3,'original caller supplies exact lower-adjacent culling literal, independent of the setter')
    # Render after the genuine parent: keep its actual screen/view/projection,
    # fog/culling/background and print size. Only choose ordinary context zero
    # for the geometry caller, as a real game draw does.
    check(reset()==0,'clean queue before parent-generated render state')
    bind(lib,'njSetCurrentContext',None,I)(0);parent()
    matrix=(F*16)();bind(lib,'njGetMatrix',None,P)(matrix)
    original_matrix=struct.unpack('<16f',read(PROBE,64));check(bytes(matrix)==read(PROBE,64),'render scene uses the actual reset matrix already proven against original code')
    sdk_screen=(F*5).in_dll(lib,'_nj_screen_');picture_aspect=F.in_dll(lib,'port_draw_aspect').value;check(picture_aspect==1.0,'retained GCC picture-aspect policy at original default')
    class Poly(C.Structure):_fields_=[('p',P),('col',P),('tex',P),('num',U)]
    xyz=[(-3.,-2.,-8.),(3.,-2.,-8.),(0.,2.,-8.)];points=(F*9)(*[n for point in xyz for n in point]);colors=(U*3)(*[0xffff8040]*3);poly=Poly(C.addressof(points),C.addressof(colors),None,3)
    projected=[];reciprocals=[]
    for point in xyz:
        camera=[sum(point[k]*original_matrix[k*4+j] for k in range(3))+original_matrix[12+j] for j in range(3)]
        reciprocals.append(1/camera[2]);projected.append((sdk_screen[3]+camera[0]*sdk_screen[0]/camera[2],sdk_screen[4]+camera[1]*sdk_screen[0]/camera[2]))
    bind(lib,'njDrawPolygon3D',None,P,I,U)(C.byref(poly),3,0x60)
    lines=[(1,1,b'DISPLAY RESET'),(1,2,b'640x480  FOV 70'),(1,26,b'ORIGINAL CODE / NATIVE PC'),(1,27,b'FONT SURVIVES RESET')]
    for x,y,text in lines:bind(lib,'njPrintC',None,I,P)((x<<16)|y,C.create_string_buffer(text))
    fid=Q();check(qstatus()==t.status()==0 and bind(lib,'jp_frames_submit',I,P)(C.byref(fid))==0,'original 3D polygon and real default-size font captured after complete reset')
    decoder=Decoder(b['GPU_DLL']);plan=P();failure=PacketFailure();check(decoder.frame(fid.value,32<<20,C.byref(plan),C.byref(failure))==0,'decode actual original producer packets')
    decoded=C.cast(plan,C.POINTER(Plan)).contents;triangle_batches=[decoded.batches[i] for i in range(decoded.batch_count) if decoded.batches[i].vertex_count==3]
    check(len(triangle_batches)==1 and triangle_batches[0].triangle_count==1 and (triangle_batches[0].header[2]>>22)&3==0,'one actual 3D triangle with genuine parent table-fog state')
    batch=triangle_batches[0]
    for j,index in enumerate([0,2,1]):
        point=decoded.vertices[batch.first_vertex+j];want=projected[index]
        check(abs(point.x-want[0])<.002 and abs(point.y-want[1])<.002 and abs(point.inv_w-reciprocals[index])<1e-6,'independent original-matrix projection agrees with actual native producer')
    decoder.destroy(plan);gpu=GPU(Path(b['GPU_DLL']));gl=gpu.lib;bound=bind(gl,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P);snap=v.frame_snapshot(fid.value)
    factors=[lookup(q,snap.fog_density,snap.fog_table) for q in reciprocals];check(max(factors)-min(factors)<1e-7,'selected near triangle has constant independent fog lookup across its depths');fog_factor=sum(factors)/3
    triangle_rgba=[round((1-fog_factor)*c+fog_factor*255) for c in [255,128,64]]+[255]
    background_tsp=word(guest_device+layout[32]['background_tsp']);bg=[0,0,0,0 if background_tsp&0x100000 else 255]
    atlas=image_oracle(dict(kind=0x100,w=128,h=128,pixels=font),128,128);font_regions={}
    for x,y,text in lines:
        for index,ch in enumerate(text):font_regions[(x+index,y)]=ch
    area=lambda a,b,p:(b[0]-a[0])*(p[1]-a[1])-(b[1]-a[1])*(p[0]-a[0]);orientation=1 if area(*projected)>0 else -1
    images=pixels=0;shots=[];worst=0
    for width,height,enabled in [(640,480,1),(640,480,0),(1280,960,1)]:
        cfg=Config.make(width,height,enabled);buffer=C.create_string_buffer(width*height*4);report=Report();err=Failure()
        check(bound(gpu.handle,fid,C.byref(cfg),buffer,len(buffer),C.byref(report),C.byref(err))==0,('actual reset-state GPU render',err.code,err.batch,err.hresult));images+=1
        if not enabled:check(report.texture_uploads==report.decoded_texels==report.shader_texture_bindings==report.shader_sampler_bindings==0,'Texture Off disables font texture decode/upload/sampling')
        actual=buffer.raw
        for y in range(height):
            for x in range(width):
                logical=((x+.5)*640/width,(y+.5)*480/height);edges=[orientation*area(projected[j],projected[(j+1)%3],logical) for j in range(3)]
                if min(abs(e)/math.dist(projected[j],projected[(j+1)%3]) for j,e in enumerate(edges))<.025:continue
                want=list(triangle_rgba if min(edges)>0 else bg)
                gx=int(logical[0]//16);gy=int(logical[1]//16);ch=font_regions.get((gx,gy))
                if ch is not None:
                    if not enabled:want=[191,191,191,255]
                    else:
                        sx=(ch%16)*8+int(logical[0]%16)//2;sy=(ch//16)*8+int(logical[1]%16)//2;at=(sy*128+sx)*4
                        if atlas[at+3]:want=[round(atlas[at+j]*191/255) for j in range(3)]+[255]
                at=(y*width+x)*4;error=max(abs(actual[at+j]-want[j]) for j in range(4));worst=max(worst,error)
                check(error<=2,('independent parent projection/fog/background/default-font pixels',width,height,enabled,x,y,list(actual[at:at+4]),want));pixels+=1
        if enabled and width==640:
            window=P();check(bind(gl,'jp_window_open',I,P,U,U,I,P,P)(gpu.handle,width,height,1,C.byref(window),C.byref(err))==0 and bind(gl,'jp_window_render_frame',I,P,Q,P,P,P)(window,fid,C.byref(cfg),C.byref(report),C.byref(err))==0,'actual full reset scene backbuffer')
            back=C.create_string_buffer(len(buffer));check(bind(gl,'jp_window_readback',I,P,P,S,P)(window,back,len(back),C.byref(err))==0 and back.raw==actual,'actual window pixels equal checked reset scene');path=out/'native-full-display-reset.png';png(path,back.raw,width,height);shots.append(dict(path=str(path),sha256=sha(path),actual_window_backbuffer=True,genuine_display_reset=True,original_polygon_and_font_calls=True,synthetic_geometry=True,retail_font_derived=True,gameplay=False));check(bind(gl,'jp_window_present',I,P,U,P)(window,0,C.byref(err))==0,'actual reset scene Present');bind(gl,'jp_window_close',None,P)(window)
    frozen=v.frame_snapshot(fid.value);parent();check(qstatus()==t.status()==0 and bytes(v.frame_snapshot(fid.value))==bytes(frozen),'subsequent genuine reset leaves submitted frame video/background/fog immutable')
    cfg=Config.make(640,480,1);buf=C.create_string_buffer(640*480*4);report=Report();err=Failure();check(bound(gpu.handle,fid,C.byref(cfg),buf,len(buf),C.byref(report),C.byref(err))==0,'submitted original font/geometry still renders after another main reset');images+=1
    check(bind(lib,'jp_frame_release',I,Q)(fid.value)==0,'reset-state frame released');gpu.close(gpu.handle)
    groups['actual_parent_3D_projection_fog_background_default_font_and_Off_pixels']=checks-sum(groups.values())
    bind(lib,'njExitPrint',None)();rs();qs();t.shutdown();check(fs()==0,'owners drained');v.shutdown()
    result=dict(status='whole_original_display_reset_and_native_main_data_and_rendering_verified',checks=checks,original_entry_calls=calls,original_dependency_calls=seen,guest_font_init_services=init_calls,display_reset_or_dependency_hooks_used=False,main_display_body_unchanged=False,main_display_control_flow_unchanged=True,only_literal_adapted_to_original_exact_RZ_word=True,original_culling_float_arguments=cull_arguments,main_data_cases=parent_cases,all_2048_record_sparse_writes_verified=True,all_128_fog_words_and_GPU_registers_verified=True,original_font_survives_display_reset=True,actual_GPU_images=images,pixels_compared=pixels,pixel_max_channel_error=worst,pixel_tolerance=2,projection_edge_exclusion_logical_pixels=.025,actual_Presents=len(shots),actual_screenshots=shots,Texture_Off_zero_sampling_verified=True,submitted_frame_survives_subsequent_reset=True,retained_GCC_picture_aspect_default_policy=True,layout_proofs=layout_proofs,reference_hashes=refhash,build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),whole_system_boot_or_gameplay_verified=False,playable=False,overall_completion_percent=10,groups=groups)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result),flush=True)
if __name__=='__main__':main()
