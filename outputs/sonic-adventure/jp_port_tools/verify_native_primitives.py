"""Whole original strip, point and clipped-polygon execution; no body hooks.
Software instruction-model comparison in sampled RZ/DN1 contexts, not new
hardware validation. Native extended scratch/undefined-color policy is separate.
"""
import argparse,ctypes as C,hashlib,json,math,random,re,struct,sys
from pathlib import Path
from native_texture_test_support import Record
from primitive_reference import make_reference
from camera_reference_numeric import service_for

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(v):return struct.pack('<'+'I'*len(v),*v)
class Poly(C.Structure):_fields_=[('p',C.c_void_p),('col',C.c_void_p),('tex',C.c_void_p),('num',C.c_uint32)]

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','numeric-dll','report']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.build.resolve();assert not a.report.exists()
    pin=json.loads((p/'jp_port_tools/jp_polygon2d_external_inputs.json').read_text());assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
    for n,h in dict(pin['files_sha256'],**json.loads((p/'jp_port_tools/jp_primitive_external_inputs.json').read_text())['files_sha256']).items():assert sha(a.upstream/n)==h,n
    b=json.loads((d/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    dll=Path(b['DLL']);assert sha(dll)==b['DLL_sha256'];lib=C.CDLL(str(dll));U=C.c_uint32;I=C.c_int32;F=C.c_float;P=C.c_void_p
    def bind(n,result,*args):
        f=getattr(lib,n);f.restype=result;f.argtypes=list(args);return f
    init=bind('jp_queue_initialize',I,C.c_size_t);reset=bind('jp_queue_reset_frame',I);shutdown=bind('jp_queue_shutdown',None);status=bind('jp_queue_status',I)
    used=bind('jp_queue_committed_bytes',C.c_size_t,U);data=bind('jp_queue_data',P,U);setctx=bind('jp_queue_set_context',I,U,C.POINTER(U));getctx=bind('jp_queue_get_context',I,U,C.POINTER(U));getparam=bind('jp_queue_get_parameter',I,C.POINTER(U))
    get=bind('njGetkmPointer',None);put=bind('njSetkmPointer',None);prepare=bind('jp_sprite_probe_prepare_texture',P);record=bind('jp_textures_record',P,U);texstatus=bind('jp_textures_status',I);texclear=bind('jp_textures_clear_error',None)
    setmatrix=bind('njSetMatrix',None,P,P);hint=bind('jp_queue_cache_hint_count',C.c_uint64)
    names=['njDrawPolygon','njDrawTexture','njDrawTextureMemList','njDrawPolygon3D','njCalcPoint','njSetkmTextureNumG','njSetkmTextureMemList','njNearClipping','njFarClipping']
    funcs={n:bind(n,None,*args) for n,args in [('njDrawPolygon',[P,I,I]),('njDrawTexture',[P,I,I,I]),('njDrawTextureMemList',[P,I,I,I]),('njDrawPolygon3D',[P,I,U]),('njCalcPoint',[P,P,P])]}
    near_clip=bind('njNearClipping',I,P,I,P,U);far_clip=bind('njFarClipping',I,P,I)
    raw_writers={n:bind(n,None,I,P,P) for n in ['njtri_filled','njtri_filled_with_tex','njtri_wiredpoly','njtri_wiredpoly_with_tex']}
    reserve=bind('jp_queue_reserve',I,U,U);mode=bind('njSetkmColorMode',None,U);end=bind('njEndVertex',None,P)
    starts={n:bind(n,P) for n in ['njStartVertexNonTexOpaque','njStartVertexNonTexTrans','njStartVertexTexOpaque','njStartVertexTexTrans']}
    selectors={n:bind(n,I,U) for n in ['njSetkmTextureNumG','njSetkmTextureMemList']}
    recordcalls=bind('jp_polygon_probe_calls',U);recmeta=bind('jp_polygon_probe_meta',P);recflat=bind('jp_primitive_probe_saved_flat',P)
    control=U.in_dll(lib,'_nj_control_3d_flag_');cache=U.in_dll(lib,'_nj_current_texture');bankcache=U.in_dll(lib,'_nj_current_texture_bank')
    screen=(F*5).in_dll(lib,'_nj_screen_');clip=(F*4).in_dll(lib,'_nj_2d_clip_');ay=F.in_dll(lib,'_nj_scr_aspect_y_');ax=F.in_dll(lib,'_nj_scr_aspect_x_');aspect=F.in_dll(lib,'port_draw_aspect');pillar=I.in_dll(lib,'port_pillarbox')
    near=F.in_dll(lib,'_nj_near_clip_');far=F.in_dll(lib,'_nj_far_clip_')
    sys.path.insert(0,str(p));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,reference_hashes=make_reference(executor);numeric=C.CDLL(str(a.numeric_dll.resolve())).sa_sh4_vector_numeric;numeric.restype=I;numeric.argtypes=[U,C.POINTER(Fpu),U];service=service_for(numeric)
    table={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    BASE=0x8c000000;CTX=0x8cd00000;CURSORS=0x8cd00200;COLOR=0x8cd00400;VERT=0x8cd02000;COL=0x8cd04000;TEX=0x8cd05000;POLY=0x8cd01000;RECS=0x8cd06000;MATRIX=0x8cd07000;OUT=0x8cd08000
    ram=bytearray(16<<20);raw=a.binary.read_bytes();ram[0x10000:0x10000+len(raw)]=raw
    def store(addr,raw):ram[addr-BASE:addr-BASE+len(raw)]=raw
    def read(addr,n):return bytes(ram[addr-BASE:addr-BASE+n])
    def iw(addr,v):store(addr,words(v))
    def word(addr):return struct.unpack('<I',read(addr,4))[0]
    iw(table['_nj_current_ctx_'],[CTX]);iw(table['_nj_vertex_buffer_'],[CURSORS]);iw(table['_nj_color_mode_'],[COLOR]);iw(table['_nj_texmemlist'],[RECS]);iw(table['_nj_texmemsize'],[4])
    bases=[0x8cd10000+i*0x10000 for i in range(5)];indices=[0,2,4,1,3];mmio={0xff000038:0,0xff00003c:0}
    for base in bases:
        at=(base&0x3ffffff)|0xe0000000;mmio.update({addr:0xde000000+(addr&31) for addr in range(at,at+65536,4)})
    rng=random.Random(0x5052494d);checks=0;packet_words=0;calls={};seen=set();fmac=0;captured=[];outcomes={};ram_prefetches=0
    allnames=names+['njGetkmPointer','njSetkmPointer','njSetkmColorMode','njStartVertexNonTexOpaque','njStartVertexNonTexTrans','njStartVertexTexOpaque','njStartVertexTexTrans','njEndVertex','njSearchTexList','njtri_filled','njtri_filled_with_tex','njtri_wiredpoly','njtri_wiredpoly_with_tex']
    entries={table[n]:n for n in allnames}
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    def trace(pc,op,*rest):
        nonlocal fmac
        if pc in entries:seen.add(entries[pc])
        if op&0xf00f==0xf00e:fmac+=1
    def pref(addr):
        nonlocal ram_prefetches
        if addr&0xf0000000==0xe0000000:captured.append((addr,[mmio[addr+4*j] for j in range(8)]))
        else:ram_prefetches+=1 # ordinary input cache hint; no architectural RAM change
    identity=(F*16)(*[1. if i%5==0 else 0. for i in range(16)])
    def original(name,args,matrix=identity,bank=0):
        f=Fpu();f.fpscr=0x40001|(bank<<21);f.banks[bank^1][:]=struct.unpack('<16I',bytes(matrix))
        s=executor.State();s.pc=table[name];s.pr=0x8cfed000;s.sr=0x70000000;s.r[15]=0x8cf00000
        for i,v in enumerate(args):s.r[4+i]=v&0xffffffff
        reference(ram,s,fpu=f,mmio=mmio,vector_instruction=service,cache_prefetch=pref,trace=trace,instruction_budget=1000000)
        calls[name]=calls.get(name,0)+1;return C.c_int32(s.r[0]).value
    def setup(case):
        check(reset()==0,'reset/guards');check(prepare() and texstatus()==0,'real synthetic texture')
        for i,j in enumerate(indices):iw(CURSORS+4*j,[bases[i]])
        for i in range(4):
            rec=Record.from_address(record(i));store(RECS+i*68,bytes(68));iw(RECS+i*68,[rec.globalIndex,rec.bank,rec.tspparambuffer,rec.texparambuffer,rec.texaddr])
        headers=[rng.getrandbits(32) for _ in range(4)];iw(CTX+144,headers);check(setctx(0,(U*4)(*headers))==0,'context')
        control.value=[0,0x400,0x1000,0x1400][case%4];iw(table['_nj_control_3d_flag_'],[control.value]);iw(table['_nj_current_texture'],[cache.value]);iw(table['_nj_current_texture_bank'],[bankcache.value])
        screen[:]=[64,640,480,320,240];store(table['_nj_screen_'],bytes(screen));clip[:]=[0,0,640,480];store(table['_nj_2d_clip_'],bytes(clip))
        ax.value=ay.value=aspect.value=1.;pillar.value=0;store(table['_nj_scr_aspect_x_'],struct.pack('<f',1.));store(table['_nj_scr_aspect_y_'],struct.pack('<f',1.))
        near.value=1.;far.value=100.;store(table['_nj_near_clip_'],struct.pack('<f',1.));store(table['_nj_far_clip_'],struct.pack('<f',100.));captured.clear()
    def compare_stream(name,case,li,textured):
        nonlocal packet_words
        length=word(CURSORS+4*indices[li])-bases[li];check(status()==0 and texstatus()==0,'native state')
        check(used(li)==length,('committed bytes',name,case,used(li),length))
        outcomes[name+('_drawn' if length else '_culled')]=outcomes.get(name+('_drawn' if length else '_culled'),0)+1
        if length:
            check(data(li)>0xffffffff and length%32==0,'native pointer/block width')
            host=struct.unpack('<'+'I'*(length//4),C.string_at(data(li),length));packets={addr-((bases[li]&0x3ffffff)|0xe0000000):v for addr,v in captured}
            check(set(packets)==set(range(0,length,32)),'all complete original packets')
            for offset,row in packets.items():
                defined=range(4 if textured else 3) if not offset else ([0,1,2,3,4,5,6] if textured else [0,1,2,3,6])
                for j in range(8):
                    if j in defined:check(host[offset//4+j]==row[j],('packet',name,case,offset,j,hex(host[offset//4+j]),hex(row[j])));packet_words+=1
                    else:check(host[offset//4+j]==0,'undefined native word zero')
        for i,j in enumerate(indices):check(word(CURSORS+4*j)-bases[i]==used(i),'all list lengths')
        result=(U*4)();check(getctx(0,result)==0 and list(result)==[word(CTX+144+4*i) for i in range(4)],'context header state')
        check(cache.value==word(table['_nj_current_texture']) and bankcache.value==word(table['_nj_current_texture_bank']),'texture cache words')
    check(init(16<<20)==0,'initialize')
    # Whole flat strip calls: arbitrary payload words are copy-only at 4:3.
    for name in ['njDrawPolygon','njDrawTexture','njDrawTextureMemList']:
        for case in range(128):
            setup(case);n=[1,2,3,4,5,8,16,32,64,129][case%10];textured=name!='njDrawPolygon';stride=24 if textured else 16;raw=rng.randbytes(n*stride)
            arena=C.create_string_buffer(b'\xa5'*32+raw+b'\x5a'*32);ptr=C.addressof(arena)+32;store(VERT,raw);trans=[0,1,-1,0x12345678][case%4];index=123 if name=='njDrawTexture' else 0
            before=recordcalls()
            original(name,[VERT,n,index,trans] if textured else [VERT,n,trans],bank=case%2)
            funcs[name](ptr,n,index,trans) if textured else funcs[name](ptr,n,trans)
            check(bytes(arena)==b'\xa5'*32+raw+b'\x5a'*32+b'\0','input payload/guards')
            check(recordcalls()==before+1 and C.string_at(recflat(),n*stride)==raw,'real replay call boundary/snapshot')
            compare_stream(name,case,int(bool(trans)),textured)
        print(name,'passed',flush=True)
    # Parent transforms, clipping, four writers, filled and outlined polygons.
    parent_counts=set();parent_modes=set()
    for case in range(768):
        setup(case);n=3+case%30;choice=(case//30)%8;attr=(0x80000000 if choice&1 else 0)|(0x20 if choice&2 else 0)|(0x40 if choice&4 else 0)
        parent_counts.add(n);parent_modes.add(attr)
        mat=(F*16)(*identity);mat[2]=rng.uniform(-0.15,0.15);mat[6]=rng.uniform(-0.15,0.15);mat[14]=[0.,0.5,1.,2.,101.][(case//30+case)%5]
        setmatrix(None,mat);points=[]
        for i in range(n):
            theta=i*2*math.pi/n;points.extend([math.cos(theta)*4,math.sin(theta)*4,0. if case%5 else 2.])
        point_raw=struct.pack('<'+'f'*(3*n),*points);colors=rng.randbytes(4*n);uv=rng.randbytes(4*n)
        pa=C.create_string_buffer(b'\xa5'*32+point_raw+b'\x5a'*32);ca=C.create_string_buffer(b'\xa5'*32+colors+b'\x5a'*32);ta=C.create_string_buffer(b'\xa5'*32+uv+b'\x5a'*32)
        poly=Poly(C.addressof(pa)+32,C.addressof(ca)+32,C.addressof(ta)+32,n);before=bytes(poly)
        store(VERT,point_raw);store(COL,colors);store(TEX,uv);iw(POLY,[VERT,COL,TEX,n]);captured.clear()
        original('njDrawPolygon3D',[POLY,n,attr],mat,case%2);funcs['njDrawPolygon3D'](C.byref(poly),n,attr)
        check(bytes(poly)==before and bytes(pa)==b'\xa5'*32+point_raw+b'\x5a'*32+b'\0','3D point record/input guards')
        check(bytes(ca)==b'\xa5'*32+colors+b'\x5a'*32+b'\0' and bytes(ta)==b'\xa5'*32+uv+b'\x5a'*32+b'\0','3D color/UV guards')
        compare_stream('njDrawPolygon3D',case,int(bool(attr&0x40)),bool(attr&0x80000000))
    print('njDrawPolygon3D passed',flush=True)
    # Stored/current point paths with complete input/output arena checks.
    for case in range(512):
        arena=C.create_string_buffer(rng.randbytes(320));before=bytes(arena);store(MATRIX,before)
        m=(F*16)(*[rng.uniform(-4,4) for _ in range(16)]);C.memmove(C.addressof(arena)+32,m,64);store(MATRIX+32,bytes(m));setmatrix(None,m)
        source_offset=128;dest_offset=[128,132,196,32,40,60][case%6];source_raw=struct.pack('<3f',*[rng.uniform(-10,10) for _ in range(3)])
        C.memmove(C.addressof(arena)+source_offset,source_raw,12);store(MATRIX+source_offset,source_raw)
        current=(case//6)%2==0;original('njCalcPoint',[0 if current else MATRIX+32,MATRIX+source_offset,MATRIX+dest_offset],m,(case//12)%2)
        funcs['njCalcPoint'](None if current else C.addressof(arena)+32,C.addressof(arena)+source_offset,C.addressof(arena)+dest_offset)
        check(bytes(arena)==read(MATRIX,C.sizeof(arena)),('complete point arena/alias contract',case,current,dest_offset))
    print('njCalcPoint passed',flush=True)
    independent_point_checks=0
    for current in [False,True]:
        for i in range(32):
            m=(F*16)(*[1. if k%5==0 else 0. for k in range(16)]);m[0]=2;m[5]=3;m[10]=4;m[12]=5;m[13]=-6;m[14]=7
            source=(F*3)(i+1,i+2,-i-3);dest=(F*3)();setmatrix(None,m);funcs['njCalcPoint'](None if current else m,source,dest)
            expected=struct.pack('<3f',2*(i+1)+5,3*(i+2)-6,4*(-i-3)+7)
            check(bytes(dest)==expected,'independent exact affine point geometry');independent_point_checks+=1
    # Standalone near clipping has caller-owned 2*n storage: alternating
    # near/far vertices can produce more than the old parent's 33 slots.
    OUTP=OUT+0x100;OUTC=OUT+0x1000;OUTT=OUT+0x2000;VPTR=OUT+0x3000
    standalone_max=0
    for case in range(128):
        setup(case);n=2+case%31;textured=bool(case&1);attr=0x80000000 if textured else 0
        positions=struct.pack('<'+'f'*(3*n),*[x for i in range(n) for x in (float(i%5)-2.,float(i%7)-3.,[0.5,1.,1.5,2.,0.99999994,1.000000119][(i+case)%6])])
        cols=rng.randbytes(4*n);uvs=rng.randbytes(4*n);pa=C.create_string_buffer(positions);ca=C.create_string_buffer(cols);ta=C.create_string_buffer(uvs)
        inp=Poly(C.addressof(pa),C.addressof(ca),C.addressof(ta) if textured else None,n)
        po=C.create_string_buffer(rng.randbytes(2*n*12+64));co=C.create_string_buffer(rng.randbytes(2*n*4+64));to=C.create_string_buffer(rng.randbytes(2*n*4+64))
        out=Poly(C.addressof(po)+32,C.addressof(co)+32,C.addressof(to)+32 if textured else None,0x12345678)
        store(VERT,positions);store(COL,cols);store(TEX,uvs);iw(POLY,[VERT,COL,TEX if textured else 0,n]);iw(OUT,[OUTP+32,OUTC+32,OUTT+32 if textured else 0,out.num]);store(OUTP,bytes(po));store(OUTC,bytes(co));store(OUTT,bytes(to))
        expected=original('njNearClipping',[POLY,n,OUT,attr],bank=case%2);actual=near_clip(C.byref(inp),n,C.byref(out),attr)
        check(actual==expected and 0<=actual<=2*n,'standalone clip count');standalone_max=max(standalone_max,actual)
        check(bytes(po)==read(OUTP,C.sizeof(po)) and bytes(co)==read(OUTC,C.sizeof(co)) and bytes(to)==read(OUTT,C.sizeof(to)),'complete clipped output arenas/guards/unused UV')
        check(bytes(pa)[:len(positions)]==positions and bytes(ca)[:len(cols)]==cols and bytes(ta)[:len(uvs)]==uvs,'standalone inputs preserved')
        check(far_clip(C.byref(inp),n)==original('njFarClipping',[POLY,n]),'standalone far result')
    print('standalone near/far clipping passed',flush=True)
    # Raw writers retain their own >=2 caller contract, including null plain
    # fill colors. Reserve the full bound before entering these C writers.
    for writer in raw_writers:
        textured=writer.endswith('with_tex');filled=writer.startswith('njtri_filled')
        for case in range(64):
            setup(case);n=2+case%63;raw=struct.pack('<'+'f'*(3*n),*[x for i in range(n) for x in (float(i%7)-3.,float(i%5)-2.,1. if case%4==0 else 2.+i/16)])
            cols=rng.randbytes(4*n);uvs=rng.randbytes(4*n);pa=C.create_string_buffer(raw);ca=C.create_string_buffer(cols);ta=C.create_string_buffer(uvs);white=filled and not textured and case%3==0
            poly=Poly(C.addressof(pa),None if white else C.addressof(ca),C.addressof(ta) if textured else None,n);store(VERT,raw);store(COL,cols);store(TEX,uvs);iw(POLY,[VERT,0 if white else COL,TEX if textured else 0,n])
            check(reserve(0,1+n*(1 if filled else 4))==0,'raw writer reserve');get();original('njGetkmPointer',[])
            color_mode=0x80002|(8 if textured else 0);mode(color_mode);original('njSetkmColorMode',[color_mode]);start='njStartVertexTexOpaque' if textured else 'njStartVertexNonTexOpaque'
            native=starts[start]();guest=original(start,[])&0xffffffff;C.memset(native,0,n*(32 if filled else 128));vp=P(native);iw(VPTR,[guest])
            original(writer,[n,POLY,VPTR],bank=case%2);raw_writers[writer](n,C.byref(poly),C.byref(vp));end(vp.value);original('njEndVertex',[word(VPTR)]);put();original('njSetkmPointer',[])
            compare_stream(writer,case,0,textured)
        print(writer,'passed',flush=True)
    for name in selectors:
        setup(0);rec=Record.from_address(record(0));get();original('njGetkmPointer',[])
        for case in range(128):
            rec.globalIndex=1000+case;rec.bank=case%5*16;rec.tspparambuffer=rng.getrandbits(32);rec.texparambuffer=(rng.getrandbits(32)&~0x1fffff)|[0,0x100000,0x100001,0x1fffff][case%4]
            iw(RECS,[rec.globalIndex,rec.bank,rec.tspparambuffer,rec.texparambuffer,rec.texaddr]);index=rec.globalIndex if name.endswith('NumG') else 0
            for repeat in range(2):
                if repeat:rec.tspparambuffer^=0x3f;rec.texparambuffer^=0x20000000;iw(RECS,[rec.globalIndex,rec.bank,rec.tspparambuffer,rec.texparambuffer,rec.texaddr])
                expected=original(name,[index]);check(selectors[name](index)==expected,('selector return',name,case,repeat))
                out=(U*4)();check(getparam(out)==0 and list(out)==[word(table['_nj_parameterkm_']+4*i) for i in range(4)],'selector exact parameter bits')
                check(cache.value==word(table['_nj_current_texture']) and bankcache.value==word(table['_nj_current_texture_bank']),'selector exact cache state')
        put();original('njSetkmPointer',[]);print(name,'passed',flush=True)
    # The host's larger parent scratch is checked by composing the complete
    # original transform/clip/writer bodies with adequately sized caller RAM.
    # Do not execute the old parent's overflowing 33-slot local scratch.
    native_extended_cases=0
    for case in range(12):
        setup(case);n=32;choice=case%8;textured=bool(choice&1);filled=bool(choice&2);trans=bool(choice&4)
        attr=(0x80000000 if textured else 0)|(0x20 if filled else 0)|(0x40 if trans else 0);null_color=textured and case>=8
        point_raw=struct.pack('<96f',*[x for i in range(n) for x in (math.cos(i*2*math.pi/n)*4,math.sin(i*2*math.pi/n)*4,0.5 if i%2 else 1.5)])
        colors=bytes([255])*(4*n) if null_color else rng.randbytes(4*n);uv=rng.randbytes(4*n)
        pa=C.create_string_buffer(point_raw);ca=C.create_string_buffer(colors);ta=C.create_string_buffer(uv);poly=Poly(C.addressof(pa),None if null_color else C.addressof(ca),C.addressof(ta),n)
        store(VERT,point_raw);store(COL,colors);store(TEX,uv);VIEW=OUT+0x4000
        setmatrix(None,identity)
        for i in range(n):original('njCalcPoint',[0,VERT+12*i,VIEW+12*i],identity,case%2)
        iw(POLY,[VIEW,COL,TEX,n]);iw(OUT,[OUTP+32,OUTC+32,OUTT+32,n]);store(OUTP,bytes(2*n*12+64));store(OUTC,bytes(2*n*4+64));store(OUTT,bytes(2*n*4+64))
        check(original('njFarClipping',[POLY,n])==1,'extended oracle far gate')
        left=original('njNearClipping',[POLY,n,OUT,attr],identity,case%2);check(left==48,'48 clipped vertices exceed old parent capacity')
        original('njGetkmPointer',[]);color_mode=(0x02100002 if trans else 0x80002)|(8 if textured else 0);original('njSetkmColorMode',[color_mode])
        start=('njStartVertexTex' if textured else 'njStartVertexNonTex')+('Trans' if trans else 'Opaque');guest=original(start,[])&0xffffffff;iw(VPTR,[guest])
        writer=('njtri_filled' if filled else 'njtri_wiredpoly')+('_with_tex' if textured else '')
        original(writer,[left,OUT,VPTR],identity,case%2);original('njEndVertex',[word(VPTR)]);original('njSetkmPointer',[])
        funcs['njDrawPolygon3D'](C.byref(poly),n,attr);compare_stream('native_extended_parent_composed_oracle',case,int(trans),textured);check(reset()==0,'extended parent queue guards')
        native_extended_cases+=1
    print('extended parent composition passed',flush=True)
    # Native caller-safety policy: bad counts/pointers and queue budget cannot
    # emit or record a partial draw. These undefined original callers are not
    # executed in the original binary.
    setup(0);tiny=C.create_string_buffer(128);ptr=C.addressof(tiny);valid=Poly(ptr,ptr,ptr,4)
    for name,args in [('njDrawPolygon',[None,1,0]),('njDrawPolygon',[ptr,0,0]),('njDrawPolygon',[ptr,-1,0]),('njDrawPolygon',[ptr,0x7fffffff,0]),
                      ('njDrawTexture',[ptr,0,123,0]),('njDrawTextureMemList',[ptr,-1,0,0]),('njDrawPolygon3D',[None,3,0]),('njDrawPolygon3D',[C.byref(valid),2,0]),('njDrawPolygon3D',[C.byref(valid),33,0])]:
        before=recordcalls();funcs[name](*args);check([used(i) for i in range(5)]==[0]*5 and recordcalls()==before and status()==0,('native argument preflight',name))
    get();out=(U*4)();check(getparam(out)==0,'selector failure baseline');prior=(cache.value,bankcache.value,bytes(out))
    check(selectors['njSetkmTextureNumG'](0xeeeeeeee)==-1,'unknown global ID rejected');check(selectors['njSetkmTextureMemList'](4)==-1,'record-index bound rejected')
    check(getparam(out)==0 and (cache.value,bankcache.value,bytes(out))==prior,'rejected selectors preserve queue/cache state');texclear();put()
    shutdown();check(init(128)==0,'small budget init');before=recordcalls();funcs['njDrawPolygon'](ptr,4,0)
    check(status()==4 and used(0)==0 and recordcalls()==before,'whole-strip budget failure before recording/writing')
    shutdown();lib.jp_textures_shutdown()
    check(set(allnames)<=seen,'all real original bodies reached')
    report=dict(status='complete_original_strip_point_and_clipped_polygon_contracts_verified',checks=checks,defined_packet_words_compared=packet_words,original_calls_by_function=calls,original_function_bodies_executed=sorted(seen),original_dependency_hooks_used=False,
        outcomes=outcomes,original_FMAC_instructions=fmac,original_non_SQ_cache_hints=ram_prefetches,original_binary_sha256=sha(a.binary),numeric_DLL_sha256=sha(a.numeric_dll),DLL_sha256=sha(dll),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),reference_hashes=reference_hashes,
        parent_counts_tested=sorted(parent_counts),parent_mode_combinations_tested=sorted(parent_modes),standalone_clipped_output_max=standalone_max,
        stored_and_current_full_partial_and_matrix_output_aliases_crossed=True,independent_point_geometry_checks=independent_point_checks,
        extended_parent_cases=native_extended_cases,extended_parent_clipped_count=48,extended_parent_oracle='composition of complete original point/clip/writer SDK bodies in caller-sized buffers; original parent unsafe stack not executed',native_null_textured_color_cases_compared_to_explicit_white_composition=True,
        qualification_scope='sampled original 4:3 RZ/DN1 defined inputs; convex clipped parent, caller-owned standalone clipping/writers, stored/current point alias ordering and selector bit contracts',native_extended_scratch_and_null_color_policy_compared_to_original=False,
        complete_production_replay_executed=False,hardware_validation=False,FPSCR_flags_compared=False,full_frame_executed=False,rendered_pixels_verified=False,playable=False,overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],checks,'checks;',sum(calls.values()),'original entry calls')
if __name__=='__main__':main()
