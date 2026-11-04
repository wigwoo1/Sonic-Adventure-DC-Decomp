"""Execute complete original model producer bodies against the native runtime.
Sampled 4:3 RZ/DN1 value/packet contracts; no hardware or whole-frame claim.
Explicit GCC changes (odd-count fix, screen rejection, later frames and skip)
are tested separately and are never counted as original bit equivalence.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from verify_native_model_preflight import Vec,Material,Mesh,Model,Span,Bound
from native_texture_test_support import Record
from model_reference import make_reference,verify_register_extension
from camera_reference_numeric import service_for
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(v):return struct.pack('<'+'I'*len(v),*v)
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','numeric-dll','report']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.build.resolve();assert not a.report.exists()
    pin=json.loads((p/'jp_port_tools/jp_polygon2d_external_inputs.json').read_text())
    assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
    for n,h in pin['files_sha256'].items():assert sha(a.upstream/n)==h,n
    b=json.loads((d/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    dll=Path(b['DLL']);assert sha(dll)==b['DLL_sha256'];lib=C.CDLL(str(dll));U=C.c_uint32;I=C.c_int32;F=C.c_float;P=C.c_void_p
    def bind(n,result,*args):
        f=getattr(lib,n);f.restype=result;f.argtypes=list(args);return f
    init=bind('jp_queue_initialize',I,C.c_size_t);reset=bind('jp_queue_reset_frame',I);qstatus=bind('jp_queue_status',I)
    used=bind('jp_queue_committed_bytes',C.c_size_t,U);data=bind('jp_queue_data',P,U);setctx=bind('jp_queue_set_context',I,U,C.POINTER(U));getctx=bind('jp_queue_get_context',I,U,C.POINTER(U))
    prepare=bind('jp_sprite_probe_prepare_texture',P);record=bind('jp_textures_record',P,U);select=bind('njSetTexture',I,P);texstatus=bind('jp_textures_status',I)
    stateinit=bind('jp_render_state_initialize',I,U,C.c_size_t);scratch=bind('jp_render_state_scratch',P);guards=bind('jp_render_state_check_guards',I)
    setmatrix=bind('njSetMatrix',None,P,P);initialize_model=bind('draw_model_init',None);setlight=bind('draw_set_light',None,F,F,F);palette=bind('draw_set_palette',None,I)
    project=bind('jp_model_probe_project',I,P,P);shade=bind('jp_model_probe_shade',None,P,P,U);offsets=bind('jp_model_probe_offsets',P);palettes=bind('jp_model_probe_palettes',P)
    attrs=bind('jp_model_probe_attrs',None);cull=bind('jp_model_probe_cull',I,P);alpha=bind('jp_model_probe_alpha',None,P,I,P,U)
    measure=bind('jp_model_preflight',I,P,C.c_size_t,C.c_size_t,C.c_size_t,C.c_size_t,P,P);reserve=bind('jp_model_reserve',I,P)
    modeldraw={n:bind(n,None,P) for n in ['draw_model','draw_model_nested','draw_model_unculled']}
    begin=bind('draw_models_begin',None);end=bind('draw_models_end',None);frame=bind('jp_sprite_probe_frame',None,I,I);untimed=bind('jp_sprite_probe_untimed',I)
    get_shading=bind('draw_get_shading',None,P);put_shading=bind('draw_put_shading',None,P);divide_light=bind('draw_divide_light',None,F);reset_light=bind('draw_reset_light',None)
    in_view=bind('draw_in_view',I,F,F,F,F);recordcalls=bind('jp_polygon_probe_calls',U)
    control=U.in_dll(lib,'_nj_control_3d_flag_');attr_and=U.in_dll(lib,'_nj_constant_attr_and_');attr_or=U.in_dll(lib,'_nj_constant_attr_or_');constant=(F*4).in_dll(lib,'_nj_constant_material_')
    cache=U.in_dll(lib,'_nj_current_texture');bankcache=U.in_dll(lib,'_nj_current_texture_bank');skip=I.in_dll(lib,'event_skipping')
    screen=(F*5).in_dll(lib,'_nj_screen_');clip=(F*4).in_dll(lib,'_nj_2d_clip_');ay=F.in_dll(lib,'_nj_scr_aspect_y_');aspect=F.in_dll(lib,'port_draw_aspect');viewaspect=F.in_dll(lib,'port_view_aspect')
    near=F.in_dll(lib,'_nj_near_clip_');far=F.in_dll(lib,'_nj_far_clip_')
    sys.path.insert(0,str(p));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,reference_hashes=make_reference(executor);register_checks=verify_register_extension(executor,reference)
    numeric=C.CDLL(str(a.numeric_dll.resolve())).sa_sh4_vector_numeric;numeric.restype=I;numeric.argtypes=[U,C.POINTER(Fpu),U];service=service_for(numeric)
    symbols={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    BASE=0x8c000000;CTX=0x8cd00000;CURSORS=0x8cd00200;COLOR=0x8cd00400;MODEL=0x8cd01000;MESH=0x8cd02000;MAT=0x8cd03000;POINTS=0x8cc00000;NORMALS=0x8cc10000;INDICES=0x8cc20000;UV=0x8cc30000;RECS=0x8cd0e000;TL=0x8cd0f000;TN=TL+16;OUT=0x8ce00000
    work=symbols['draw_work'];ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary
    def store(at,raw):ram[at-BASE:at-BASE+len(raw)]=raw
    def read(at,n):return bytes(ram[at-BASE:at-BASE+n])
    def iw(at,v):store(at,words(v))
    def word(at):return struct.unpack('<I',read(at,4))[0]
    iw(symbols['_nj_current_ctx_'],[CTX]);iw(symbols['_nj_vertex_buffer_'],[CURSORS]);iw(symbols['_nj_color_mode_'],[COLOR]);iw(symbols['_nj_vertex_buf_'],[OUT])
    iw(symbols['_nj_texmemlist'],[RECS]);iw(symbols['_nj_texmemsize'],[4]);iw(symbols['_nj_current_texlist'],[TL]);iw(TL,[TN,1]);iw(TN,[0,0,RECS])
    bases=[0x8cd10000+i*0x10000 for i in range(5)];indices=[0,2,4,1,3]
    mmio={0xff000038:0,0xff00003c:0}
    for base in bases+[OUT]:
        at=(base&0x3ffffff)|0xe0000000;mmio.update({v:0 for v in range(at,at+65536,4)})
    captured=[];ram_hints=0;instruction_entries={};calls={};seen=set();fmac=0;rng=random.Random(0x4d4f444c);checks=0;defined_words=0;mesh_kinds=set();material_modes=set();model_counts=set();controls=set();clip_masks={'near_clip':set(),'near_clip_tex':set(),'near_clip_env':set()}
    entrynames=['draw_model_init','draw_model','draw_model_nested','draw_model_unculled','cull_model','project_points','shade_points','draw_meshes','set_point_alpha','strips','strips_tex','strips_env_uv','strips_clip','strips_tex_clip','strips_env_clip','near_clip','near_clip_tex','near_clip_env','draw_set_light','draw_set_palette','draw_models_begin','draw_models_end','draw_in_view','draw_divide_light','draw_reset_light']
    instruction_entries={symbols[n]:n for n in entrynames}
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    def trace(pc,op,*rest):
        nonlocal fmac
        if pc in instruction_entries:seen.add(instruction_entries[pc])
        # Original R0 has BEHIND_A/B/C in bits 0..2 and NEARER_A/B/C in
        # bits 4..6. Either category flags a corner; keep its three-bit mask.
        if pc in [symbols[n] for n in clip_masks]:
            flags=rest[0][0];clip_masks[instruction_entries[pc]].add((flags|(flags>>4))&7)
        if op&0xf00f==0xf00e:fmac+=1
    def pref(at):
        nonlocal ram_hints
        if at&0xf0000000!=0xe0000000:ram_hints+=1;return
        start=at&~31;row=[mmio[start+4*j] for j in range(8)];target=(start&0x3ffffff)|BASE
        if OUT<=target<OUT+65536:store(target,words(row))
        else:captured.append((start,row))
    identity=(F*16)(*[1. if i%5==0 else 0. for i in range(16)])
    special={'gbr':work,'r_bank':[0]*8}
    def original(name,args=(),matrix=identity,bank=0,fargs=()):
        f=Fpu();f.fpscr=0x40001|(bank<<21);f.banks[bank^1][:]=struct.unpack('<16I',bytes(matrix))
        for i,value in enumerate(fargs):f.banks[bank][4+i]=struct.unpack('<I',struct.pack('<f',value))[0]
        s=executor.State();s.pc=symbols[name];s.pr=0x8cfed000;s.sr=0x70000000;s.r[15]=0x8cf00000
        for i,v in enumerate(args):s.r[4+i]=v&0xffffffff
        reference(ram,s,fpu=f,mmio=mmio,special=special,vector_instruction=service,cache_prefetch=pref,cache_invalidate=lambda at:None,trace=trace,instruction_budget=1000000)
        calls[name]=calls.get(name,0)+1;return s
    def setup(case):
        check(reset()==0,'queue reset');listing=prepare();check(listing and texstatus()==0,'real owned texture');check(select(listing)==1,'texture selection')
        for i,j in enumerate(indices):iw(CURSORS+4*j,[bases[i]])
        for i in range(4):
            rec=Record.from_address(record(i));store(RECS+68*i,bytes(68));iw(RECS+68*i,[rec.globalIndex,rec.bank,rec.tspparambuffer,rec.texparambuffer,rec.texaddr])
        header=[0x80000000,0x20800000,0x20800000,0];iw(CTX+144,header);check(setctx(0,(U*4)(*header))==0,'context header')
        control.value=0;attr_and.value=0xffffffff;attr_or.value=0;constant[:]=[0.5,1,1,1]
        iw(symbols['_nj_control_3d_flag_'],[control.value]);iw(symbols['_nj_constant_attr_and_'],[attr_and.value]);iw(symbols['_nj_constant_attr_or_'],[attr_or.value]);store(symbols['_nj_constant_material_'],bytes(constant))
        iw(symbols['_nj_current_texture'],[cache.value]);iw(symbols['_nj_current_texture_bank'],[bankcache.value]);iw(symbols['frame_step'],[0]);frame(0,0);skip.value=0
        screen[:]=[64,640,480,320,240];clip[:]=[0,0,639,479];ay.value=aspect.value=viewaspect.value=1.;near.value=1.;far.value=1000.
        for n,raw in [('_nj_screen_',bytes(screen)),('_nj_2d_clip_',bytes(clip)),('_nj_scr_aspect_x_',struct.pack('<f',1.)),('_nj_scr_aspect_y_',struct.pack('<f',1.)),('_nj_near_clip_',struct.pack('<f',1.)),('_nj_far_clip_',struct.pack('<f',1000.))]:store(symbols[n],raw)
        initialize_model();original('draw_model_init');attrs()
        colors=rng.randbytes((8*256*2+2048)*4);C.memmove(palettes(),colors,len(colors));store(symbols['draw_palettes'],colors[:(8*256*2+512)*4])
        # GCC enlarges unlit offsets beyond the original 512 slots; these
        # extra entries remain zero under the pinned runtime's load contract.
        C.memset(palettes()+(8*256*2+512)*4,0,(2048-512)*4)
        for n in range(64):mmio[((OUT&0x3ffffff)|0xe0000000)+n*4]=0
        C.memset(scratch(),0,2048*16);store(OUT,bytes(2048*16));captured.clear()
    def model_fixture(case,n,mesh_count=1):
        pts=(Vec*(n+2))();norms=(Vec*(n+2))()
        for i in range(n+2):
            pts[i]=Vec(rng.uniform(-2,2),rng.uniform(-2,2),[0.5,0.75,1.,1.25,2.,3.][(i+case)%6] if case%3 else rng.uniform(2,8))
            norms[i]=Vec(*[rng.uniform(-1,1) for _ in range(3)])
        meshes=(Mesh*mesh_count)();materials=(Material*mesh_count)();spans=(Span*mesh_count)();keep=[];index_cursor=INDICES;uv_cursor=UV
        for j in range(mesh_count):
            kind=(case+j)%4;count=1+(case+j)%3;packed=[];tex=[]
            for k in range(count):
                size=(3 if kind==0 else 4) if kind<2 else 3+(case+j+k)%7
                if kind>=2:packed.append(size| (0x8000 if k&1 else 0))
                packed.extend((i+k+j)%n for i in range(size));tex.extend(rng.randrange(-255,256) for _ in range(size*2))
            iv=(C.c_uint16*len(packed))(*packed);uv=(C.c_int16*len(tex))(*tex);keep.extend([iv,uv])
            textured=((case//4+j)%3)!=0;env=textured and (case//12+j)%2;flags=(0x200000 if textured else 0)|(0x400000 if env else 0)|(0x100000 if (case//24+j)%2 else 0)|(0x2000000 if (case//48+j)%2 else 0)|(0x80000 if (case+j)%2 else 0)
            mesh_kinds.add(kind);material_modes.add((bool(textured),bool(env),bool(flags&0x100000),bool(flags&0x2000000)))
            materials[j]=Material(rng.getrandbits(32),0x00112233 if case&1 else 0,1.,0,flags)
            meshes[j]=Mesh((kind<<14)|j,count,C.cast(iv,C.POINTER(C.c_int16)),None,None,None,C.cast(uv,P));spans[j]=Span(len(packed),len(tex)//2)
            store(index_cursor,bytes(iv));store(uv_cursor,bytes(uv));store(MESH+24*j,struct.pack('<HHIIIII',meshes[j].kind,count,index_cursor,0,0,0,uv_cursor));store(MAT+20*j,bytes(materials[j]))
            index_cursor+=0x100;uv_cursor+=0x100
        model=Model(pts,norms,n,meshes,materials,mesh_count,mesh_count,Vec(0,0,3),20.)
        store(POINTS,bytes(pts));store(NORMALS,bytes(norms));store(MODEL,struct.pack('<IIIIIHH4f',POINTS,NORMALS,n,MESH,MAT,mesh_count,mesh_count,0.,0.,3.,20.))
        return model,[pts,norms,meshes,materials,spans,*keep],spans
    def lighting(case,mat):
        v=[rng.uniform(-1.25,1.25) for _ in range(3)];setlight(*v);original('draw_set_light',matrix=mat,bank=case%2,fargs=v)
        pal=case%8;palette(pal);original('draw_set_palette',[pal],mat,case%2)
    def compare_points(n,label):
        actual=struct.unpack('<'+'I'*(n*4),C.string_at(scratch(),n*16));expected=struct.unpack('<'+'I'*(n*4),read(OUT,n*16))
        for i,(av,ev) in enumerate(zip(actual,expected)):check(av==ev,('projected/shaded point',label,i,hex(av),hex(ev)))
        check(guards()==0,'native scratch guards')
    def compare_packets(label):
        nonlocal defined_words
        for li in range(5):
            length=word(CURSORS+4*indices[li])-bases[li];check(used(li)==length,('list length',label,li,used(li),length))
            if not length:continue
            host=struct.unpack('<'+'I'*(length//4),C.string_at(data(li),length));packets={at-((bases[li]&0x3ffffff)|0xe0000000):row for at,row in captured if ((bases[li]&0x3ffffff)|0xe0000000)<=at<((bases[li]&0x3ffffff)|0xe0000000)+65536}
            check(set(packets)==set(range(0,length,32)),('complete original packet stream',label,li))
            textured=False
            for off,row in sorted(packets.items()):
                header=row[0]&0xe0000000==0x80000000
                if header:textured=bool(row[0]&8);defined=range(4 if textured else 3)
                else:defined=range(8) if textured else [0,1,2,3,6]
                for j in defined:check(host[off//4+j]==row[j],('model packet',label,li,off,j,hex(host[off//4+j]),hex(row[j])));defined_words+=1
        actual=(U*4)();check(getctx(0,actual)==0 and list(actual)==[word(CTX+144+4*i) for i in range(4)],('context after model',label))
        check(qstatus()==0 and texstatus()==0 and guards()==0 and untimed()==0,'native guards/state/untimed balanced')
        check(cache.value==word(symbols['_nj_current_texture']),'texture cache after model')
    check(init(16<<20)==0 and stateinit(2048,1<<20)==0,'owned native stores initialized')
    for case in range(128):
        setup(case);n=2+2*(case%32);model,keep,spans=model_fixture(case,n)
        mat=(F*16)(*identity);mat[0]=rng.uniform(0.75,1.25);mat[5]=rng.uniform(0.75,1.25);mat[12]=rng.uniform(-0.2,0.2);mat[13]=rng.uniform(-0.2,0.2);mat[14]=0.25
        setmatrix(None,mat);lighting(case,mat)
        s=original('project_points',[MODEL],mat,case%2);actual=project(C.byref(model),scratch());check(actual==s.r[13],('projection near count',case,actual,s.r[13]))
        # Projection defines xyz only; original store-queue color lanes have no
        # stable value until shading. Compare xyz before the full shaded arena.
        for i in range(n):check(C.string_at(scratch()+16*i,12)==read(OUT+16*i,12),('projected XYZ',case,i))
        iw(work+52,[MAT]);iw(work+44,[model.materials[0].flags]);iw(work+60,[OUT])
        original('shade_points',[MODEL],mat,case%2);shade(C.byref(model),scratch(),model.materials[0].flags);compare_points(n,case)
        for i in range(n):check(C.c_uint32.from_address(offsets()+4*i).value==word(symbols['lit_offsets']+4*i),('lit offset',case,i))
    print('Complete original projection and shading passed',flush=True)
    for case in range(294):
        setup(case);n=[4,8,16,32,64,128][case%6] if case<288 else [512,1900,2048][case%3];model,keep,spans=model_fixture(case,n,1+case%3);model_counts.add(n)
        mat=(F*16)(*identity);mat[14]=[0.,0.125,0.25][case%3];setmatrix(None,mat);lighting(case,mat)
        control.value=[0,4,0x10,0x20,0x14,0x24][(case//4)%6];controls.add(control.value)
        attr_and.value=0xffffffff^(0x100000 if case&1 else 0);attr_or.value=0x100000 if case&2 else 0
        constant[0]=[-0.25,0.,0.5,1.,1.25][(case//8)%5]
        iw(symbols['_nj_control_3d_flag_'],[control.value]);iw(symbols['_nj_constant_attr_and_'],[attr_and.value]);iw(symbols['_nj_constant_attr_or_'],[attr_or.value]);store(symbols['_nj_constant_material_'],bytes(constant))
        bound=Bound();check(measure(C.byref(model),n,n,model.mesh_count,model.material_count,spans,C.byref(bound))==0,'real model preflight');check(reserve(C.byref(bound))==0,'whole model reservation')
        name=['draw_model','draw_model_nested','draw_model_unculled'][case%3]
        if name!='draw_model':begin();original('draw_models_begin',matrix=mat,bank=case%2)
        original(name,[MODEL],mat,case%2);modeldraw[name](C.byref(model))
        if name!='draw_model':end();original('draw_models_end',matrix=mat,bank=case%2)
        compare_points(n,('whole',case));compare_packets((name,case))
        check(used(0)<=bound.opaque*32 and used(1)<=bound.translucent*32,'complete actual producer obeys preflight bound')
        if case%48==47:print('Whole model cases',case+1,'passed',flush=True)
    print('Complete original model draws passed',flush=True)
    # Force all eight near-plane corner masks for each real strip mode.
    # Masks zero and seven bypass the clipper; the other six must reach it.
    targeted_clip_cases=0
    for mode in range(3):
        for mask in range(8):
            setup(mask);model,keep,spans=model_fixture(0,4)
            for i in range(4):model.points[i]=Vec([-0.25,0.25,0.,0.][i],[-0.25,-0.25,0.25,0.][i],0.5 if i<3 and mask&(1<<i) else 2.)
            model.meshes[0].count=1;model.materials[0].flags=(0x200000 if mode else 0)|(0x400000 if mode==2 else 0)|(0x100000 if mask&1 else 0)
            store(POINTS,bytes(keep[0]));store(MAT,bytes(model.materials[0]));store(MESH+2,struct.pack('<H',1));setmatrix(None,identity);lighting(mask,identity)
            bound=Bound();check(measure(C.byref(model),4,4,1,1,spans,C.byref(bound))==0 and reserve(C.byref(bound))==0,'all-mask preflight')
            begin();original('draw_models_begin');original('draw_model_unculled',[MODEL]);modeldraw['draw_model_unculled'](C.byref(model));end();original('draw_models_end')
            compare_points(4,('mask',mode,mask));compare_packets(('mask',mode,mask));targeted_clip_cases+=1
            check(used(0)<=bound.opaque*32 and used(1)<=bound.translucent*32,'targeted actual clipped producer stays within bound')
    check(all(v==set(range(1,7)) for v in clip_masks.values()),'all six cut masks reached in all three original clippers')
    # Cross-model state and queue-growth lifetime: keep one begin/end around
    # three full models, changing the shared current matrix between draws.
    for run in range(12):
        setup(run);begin();original('draw_models_begin')
        for j in range(3):
            case=run*3+j;model,keep,spans=model_fixture(case,32,3);mat=(F*16)(*identity);mat[12]=j*0.125;setmatrix(None,mat);lighting(case,mat)
            bound=Bound();check(measure(C.byref(model),32,32,3,3,spans,C.byref(bound))==0 and reserve(C.byref(bound))==0,'multi-model validated reservation')
            original('draw_model_unculled',[MODEL],mat,case%2);modeldraw['draw_model_unculled'](C.byref(model));compare_points(32,('multi',run,j))
        end();original('draw_models_end');compare_packets(('multi',run))
    # Original culling / public visibility and light-state APIs, with all
    # branches of their real implementations left intact.
    for case in range(128):
        setup(case);model,keep,spans=model_fixture(case,4);mat=(F*16)(*identity);mat[12]=rng.uniform(-5,5);mat[14]=rng.uniform(-5,5);setmatrix(None,mat)
        center=[rng.uniform(-200,200),rng.uniform(-200,200),rng.uniform(-10,1100)];radius=rng.uniform(0,20);model.center=Vec(*center);model.radius=radius
        store(MODEL+24,struct.pack('<4f',*center,radius));s=original('cull_model',[MODEL],mat,case%2);check(cull(C.byref(model))==int(not(s.sr&1)),('original sphere culling',case))
        step=case%3;frame(step,0);iw(symbols['frame_step'],[step]);s=original('draw_in_view',matrix=mat,bank=case%2,fargs=[*center,radius]);check(in_view(*center,radius)==s.r[0],('public visibility',case));frame(0,0)
        lighting(case,mat);saved=C.create_string_buffer(32);get_shading(saved)
        check(bytes(saved)[:24]==read(symbols['light_dir'],24),'light and base state');check(C.c_void_p.from_buffer(saved,24).value==palettes()+(case%8)*2048,'native palette pointer')
        scale=[0.125,0.75,1.,2.,16.,-0.75][case%6];divide_light(scale);original('draw_divide_light',matrix=mat,bank=case%2,fargs=[scale]);after=C.create_string_buffer(32);get_shading(after);check(bytes(after)[:24]==read(symbols['light_dir'],24),'complete divide-light state')
        reset_light();original('draw_reset_light',matrix=mat,bank=case%2);get_shading(after);check(bytes(after)[:24]==read(symbols['light_dir'],24),'complete reset-light state')
        setlight(1,2,3);put_shading(saved);get_shading(after);check(bytes(after)==bytes(saved),'whole shading snapshot round trip')
    # Known GCC odd-count correction: compare defined points/colors, and
    # explicitly account for the original padded extra point in its count.
    odd_cases=0
    for n in [1,3,5,7,15,31,63,127]:
        for padded_near in [False,True]:
            setup(n);model,keep,spans=model_fixture(0,n)
            for i in range(n):model.points[i]=Vec(0.125*i,-0.25*i,0.5 if i%2 else 2.)
            model.points[n]=Vec(0,0,0.5 if padded_near else 2.);store(POINTS,bytes(keep[0]));setmatrix(None,identity);lighting(n,identity)
            s=original('project_points',[MODEL]);actual=project(C.byref(model),scratch());expected=sum(i%2 for i in range(n))
            check(actual==expected and s.r[13]==expected+int(padded_near),'explicit original odd-count correction')
            for i in range(n):check(C.string_at(scratch()+16*i,12)==read(OUT+16*i,12),'odd-count defined point projection')
            iw(work+52,[MAT]);iw(work+44,[model.materials[0].flags]);iw(work+60,[OUT]);original('shade_points',[MODEL]);shade(C.byref(model),scratch(),model.materials[0].flags);compare_points(n,('odd',n,padded_near));odd_cases+=1
    # GCC-specific frame/skip policy has no matching original behavior. Call
    # the real complete entries with preflight and reservations, inspect their
    # actual output and observer boundaries separately from original counts.
    policy_cases=0
    for step,later,skipping in [(2,0,0),(1,0,0),(0,0,1),(1,1,0)]:
        setup(step+later);model,keep,spans=model_fixture(0,4);setmatrix(None,identity);lighting(0,identity)
        bound=Bound();check(measure(C.byref(model),4,4,1,1,spans,C.byref(bound))==0 and reserve(C.byref(bound))==0,'policy model preflight')
        frame(step,later);skip.value=skipping;before=recordcalls();modeldraw['draw_model'](C.byref(model))
        draws=step==1 and later and not skipping
        check(bool(used(0)+used(1))==bool(draws) and recordcalls()==before+int(bool(draws)) and untimed()==0 and guards()==0 and qstatus()==0,'complete GCC frame and skip policy');policy_cases+=1
    frame(0,0);skip.value=0
    # GCC replaces original accidental lit_offsets reads after unlit slot
    # 511 with explicit zero offsets. Check that policy independently on the
    # real model draw; do not call it original-parent packet equivalence.
    setup(0);model,keep,spans=model_fixture(0,2048);model.meshes[0].count=1;model.materials[0].flags=0x2200000
    for j,index in enumerate([511,512,2047]):model.meshes[0].indices[j]=index
    store(MAT,bytes(model.materials[0]));setmatrix(None,identity);setlight(0,0,1)
    bound=Bound();check(measure(C.byref(model),2048,2048,1,1,spans,C.byref(bound))==0 and reserve(C.byref(bound))==0,'extended unlit policy preflight')
    begin();modeldraw['draw_model_unculled'](C.byref(model));end()
    check(used(0)==128 and used(1)==0,'extended unlit three-point model output')
    packet=struct.unpack('<32I',C.string_at(data(0),128));expected=[C.c_uint32.from_address(palettes()+(8*256*2+511)*4).value,0,0]
    check([packet[15],packet[23],packet[31]]==expected and guards()==0 and qstatus()==0,'defined GCC unlit extension offsets')
    check(mesh_kinds==set(range(4)) and len(material_modes)==12 and controls=={0,4,0x10,0x20,0x14,0x24},'all mesh kinds, texture/env/alpha/light modes and material control modes')
    print('Cross-model lifetime, culling, light state, odd-count and GCC policy cases passed',flush=True)
    report=dict(status='sampled_complete_actual_model_RZ_contracts_verified',checks=checks,register_extension_checks=register_checks,defined_packet_words_compared=defined_words,
        original_calls_by_function=calls,original_entry_calls=sum(calls.values()),original_function_bodies_executed=sorted(seen),original_dependency_hooks_used=False,original_FMAC_instructions=fmac,
        original_binary_sha256=sha(a.binary),numeric_DLL_sha256=sha(a.numeric_dll),DLL_sha256=sha(dll),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),reference_hashes=reference_hashes,
        meshset_types_tested=sorted(mesh_kinds),material_modes_tested=sorted(material_modes),model_point_counts_tested=sorted(model_counts),material_control_modes_tested=sorted(controls),original_near_clip_flags={n:sorted(v) for n,v in clip_masks.items()},all_eight_corner_masks_in_each_strip_mode=True,targeted_clip_cases=targeted_clip_cases,odd_count_correction_cases=odd_cases,GCC_frame_skip_policy_cases=policy_cases,GCC_extended_unlit_offsets_verified=True,three_model_shared_queue_runs=12,
        qualification_scope='sampled finite valid 4:3 RZ/DN1 original projection/shading, culling, light state and full-model packet contracts with native x64 records, real owners and complete reservations; odd-count correction and GCC frame/skip policy explicitly separated',
        complete_retained_replay_executed=False,hardware_validation=False,FPSCR_flags_compared=False,full_frame_executed=False,rendered_pixels_verified=False,playable=False,overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],checks,'checks',flush=True)
if __name__=='__main__':main()
