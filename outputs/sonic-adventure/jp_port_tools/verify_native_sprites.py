"""Execute whole original sprite/texture dependencies against native packets.
No original routine is replaced by a hook. GCC replay/frame policy is an
additional host contract; full production replay and rasterization stay gated.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_texture_test_support import List,Record
from primitive_reference import make_reference
from camera_reference_numeric import service_for

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(v):return struct.pack('<'+'I'*len(v),*v)
def fw(x):return struct.unpack('<I',struct.pack('<f',x))[0]
class Point(C.Structure):_fields_=[('x',C.c_float),('y',C.c_float),('z',C.c_float)]
class Frame(C.Structure):_fields_=[(n,C.c_int16) for n in ['sx','sy','cx','cy','u1','v1','u2','v2','texid','attr']]
class Sprite(C.Structure):_fields_=[('p',Point),('sx',C.c_float),('sy',C.c_float),('ang',C.c_int32),('tlist',C.c_void_p),('tanim',C.c_void_p)]

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','numeric-dll','report']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.build.resolve()
    assert not a.report.exists() and a.report.resolve().is_relative_to(r/'work/sonic-adventure')
    pin=json.loads((p/'jp_port_tools/jp_polygon2d_external_inputs.json').read_text());assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
    for n,h in pin['files_sha256'].items():assert sha(a.upstream/n)==h,n
    b=json.loads((d/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    dll=Path(b['DLL']);assert sha(dll)==b['DLL_sha256']
    sys.path.insert(0,str(p));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,reference_hashes=make_reference(executor)
    table={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    lib=C.CDLL(str(dll));P=C.c_void_p;U=C.c_uint32;I=C.c_int32;F=C.c_float
    def bind(n,result,*args):
        f=getattr(lib,n);f.restype=result;f.argtypes=list(args);return f
    init=bind('jp_queue_initialize',I,C.c_size_t);reset=bind('jp_queue_reset_frame',I);shutdown=bind('jp_queue_shutdown',None)
    used=bind('jp_queue_committed_bytes',C.c_size_t,U);data=bind('jp_queue_data',P,U);status=bind('jp_queue_status',I)
    setctx=bind('jp_queue_set_context',I,U,C.POINTER(U));getctx=bind('jp_queue_get_context',I,U,C.POINTER(U));getparam=bind('jp_queue_get_parameter',I,C.POINTER(U))
    cursor=bind('jp_queue_parameter_cursor',P,U);descriptor=bind('jp_queue_descriptor_cursor',P,U)
    hint=bind('jp_queue_cache_hint_count',C.c_uint64);last=bind('jp_queue_last_cache_hint',P)
    prepare=bind('jp_sprite_probe_prepare_texture',P);record=bind('jp_textures_record',P,U);texstatus=bind('jp_textures_status',I)
    callback=bind('jp_sprite_probe_callback',None,I,I);frame_step=bind('jp_sprite_probe_frame',None,I,I);untimed=bind('jp_sprite_probe_untimed',I)
    setmatrix=bind('njSetMatrix',None,P,P);setdepth=bind('sprite_set_depth',None,F);getdepth=bind('sprite_get_depth',F)
    draw2=bind('njDrawSprite2D',None,P,I,F,U);draw3=bind('njDrawSprite3D',None,P,I,U);drawgame=bind('sprite_draw_3d',None,P,I,U)
    select_texture=bind('njSetTexture',I,P);select_index=bind('njSetkmTextureNum',I,U);get=bind('njGetkmPointer',None);put=bind('njSetkmPointer',None)
    recordcalls=bind('jp_polygon_probe_calls',U)
    control=U.in_dll(lib,'_nj_control_3d_flag_');material=(U*4).in_dll(lib,'_nj_constant_material_');screen=(F*5).in_dll(lib,'_nj_screen_')
    aspect_y=F.in_dll(lib,'_nj_scr_aspect_y_');draw_aspect=F.in_dll(lib,'port_draw_aspect');pillar=I.in_dll(lib,'port_pillarbox')
    clip=(F*4).in_dll(lib,'_nj_2d_clip_');near=F.in_dll(lib,'_nj_near_clip_');far=F.in_dll(lib,'_nj_far_clip_')
    cache=U.in_dll(lib,'_nj_current_texture');bank_cache=U.in_dll(lib,'_nj_current_texture_bank')
    numeric=C.CDLL(str(a.numeric_dll.resolve())).sa_sh4_vector_numeric;numeric.restype=I;numeric.argtypes=[U,C.POINTER(Fpu),U];service=service_for(numeric)
    BASE=0x8c000000;CTX=0x8cd00000;CURSORS=0x8cd00200;COLOR=0x8cd00400;SP=0x8cd01000;ANIM=0x8cd01100;TLIST=0x8cd01200;TNAME=0x8cd01300;REC=0x8cd01400
    ram=bytearray(16<<20);raw=a.binary.read_bytes();ram[0x10000:0x10000+len(raw)]=raw
    def store(addr,raw):ram[addr-BASE:addr-BASE+len(raw)]=raw
    def iw(addr,v):store(addr,words(v))
    def word(addr):return struct.unpack_from('<I',ram,addr-BASE)[0]
    iw(table['_nj_current_ctx_'],[CTX]);iw(table['_nj_vertex_buffer_'],[CURSORS]);iw(table['_nj_color_mode_'],[COLOR])
    bases=[0x8cd10000+i*0x10000 for i in range(5)];indices=[0,2,4,1,3]
    mmio={0xff000038:0,0xff00003c:0};mmio.update({addr:0xde000000+(addr&31) for base in bases for addr in range((base&0x3ffffff)|0xe0000000,((base&0x3ffffff)|0xe0000000)+65536,4)})
    captured=[];seen=set();counts={};outcomes={};near_masks={'sprite_clip_near':set(),'sprite_send_clipped':set()};checks=0;packet_words=0;rp=random.Random(0x53505249)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    required=['njDrawSprite2D','njDrawSprite3D','sprite_draw_3d','sprite_set_depth','sprite_get_depth','njGetkmPointer','njSetkmPointer','njSetkmColorMode','njSetTexture','njSetkmTextureNum','njStartVertexTexOpaqueIntens','njStartVertexTexTransIntens','njEndVertex']
    required+=['sprite_emit_billboard','sprite_emit_transformed','sprite_emit_2d','sprite_clip_near','sprite_send_facing','sprite_send_flat','sprite_send_clipped']
    entries={table[n]:n for n in required}
    def trace(pc,op,regs,*rest):
        if pc in entries:seen.add(entries[pc])
        if pc==table['sprite_clip_near']:near_masks['sprite_clip_near'].add(regs[4]&15)
        if pc==table['sprite_send_clipped']:near_masks['sprite_send_clipped'].add(regs[4]&15)
    def pref(addr):captured.append((addr,[mmio[addr+4*i] for i in range(8)]))
    def original(name,n,attr,priority,matrix,bank=0,arg0=None):
        f=Fpu();f.fpscr=0x40001|(bank<<21);f.banks[bank][4]=fw(priority);f.banks[bank^1][:]=struct.unpack('<16I',bytes(matrix))
        s=executor.State();s.pc=table[name];s.pr=0x8cfed000;s.sr=0x70000000;s.r[15]=0x8cf00000;s.r[4]=SP if arg0 is None else arg0;s.r[5]=n;s.r[6]=attr
        reference(ram,s,fpu=f,mmio=mmio,vector_instruction=service,cache_prefetch=pref,trace=trace,instruction_budget=500000)
        counts[name]=counts.get(name,0)+1;f.reference_r0=s.r[0];return f
    def scenario(name,case):
        nonlocal packet_words
        check(reset()==0,'queue reset/guards');frame_step(0,0);callback(0,1)
        for li,j in enumerate(indices):iw(CURSORS+4*j,[bases[li]])
        tlist=prepare();check(bool(tlist) and texstatus()==0,'actual owned synthetic texture')
        rec=Record.from_address(record(0));iw(REC,[rec.globalIndex,rec.bank,rec.tspparambuffer,rec.texparambuffer,rec.texaddr]);iw(TNAME,[0,0,REC]);iw(TLIST,[TNAME,1])
        headers=[0x80800000,0x1a000000,0xabcdefff,0x12345678];check(setctx(0,(U*4)(*headers))==0,'set context');iw(CTX+144,headers)
        control.value=[0,0x400,0x1000,0x1400][case%4];iw(table['_nj_control_3d_flag_'],[control.value]);iw(table['_nj_current_texture'],[cache.value]);iw(table['_nj_current_texture_bank'],[bank_cache.value])
        rgba=[fw(0.25),fw(0.5),fw(0.75),fw(1.)];material[:]=rgba;iw(table['_nj_constant_material_'],rgba)
        screen[:]=[64.,640.,480.,320.,240.];store(table['_nj_screen_'],bytes(screen));aspect_y.value=draw_aspect.value=1.;pillar.value=0
        store(table['_nj_scr_aspect_y_'],struct.pack('<f',1.));store(table['_nj_scr_aspect_x_'],struct.pack('<f',1.))
        clip[:]=[0.,0.,640.,480.];store(table['_nj_2d_clip_'],bytes(clip));near.value=1.;far.value=100.;store(table['_nj_near_clip_'],struct.pack('<f',1.));store(table['_nj_far_clip_'],struct.pack('<f',100.))
        mat=(F*16)(*[1. if i%5==0 else 0. for i in range(16)])
        attr=case%64
        if name!='njDrawSprite2D':
            if case%8 in (0,1,2,3):mat[2]=[-0.1,0.1,-0.15,0.15][case%4];mat[6]=[-0.1,-0.1,0.15,0.15][case%4]
            if case%17==0:mat[12]=5000. # off-screen culling
        frame=Frame(16,16,8,8,-256,17,512,768,0,0)
        sx=[1.,0.5,2.,-1.][case%4];sy=[0.75,1.5,-0.5,2.][(case//4)%4]
        z=[0.25,0.75,1.,1.5,2.,20.,101.,-2.][case%8]
        if case>=192:
            attr=rp.randrange(64);sx=rp.uniform(0.1,2.);sy=rp.uniform(0.1,2.);z=rp.uniform(-1.,3.)
            if name!='njDrawSprite2D':mat[2]=rp.uniform(-0.3,0.3);mat[6]=rp.uniform(-0.3,0.3);mat[12]=0.
        angle=C.c_int32(rp.getrandbits(32)).value
        if case>=448:
            attr=case-448;sx=1.;sy=0.75;z=2.;mat[2]=mat[6]=0.
            mat[12],mat[13]=[(0.,0.),(5000.,0.),(-5000.,0.),(0.,5000.),(0.,-5000.)][case%5]
            angle=C.c_int32([0,0x4000,0x8000,0xc000,0x10000,0x7fffffff,0x80000000,0xffffffff][case%8]).value
            frame.sx=[16,0,-16,32767,-32768][case%5];frame.u1=-32768;frame.v1=32767;frame.u2=32767;frame.v2=-32768
        setmatrix(None,mat)
        sprite=Sprite(Point(0.,0.,z),sx,sy,angle,tlist,C.addressof(frame))
        if name=='njDrawSprite2D':sprite.p=Point(320.+case%11,240.-case%7,z)
        if name=='njDrawSprite2D' and case>=448:sprite.p.x+=mat[12];sprite.p.y+=mat[13]
        before=bytes(sprite);frameraw=bytes(frame)
        store(SP,struct.pack('<5fIII',sprite.p.x,sprite.p.y,sprite.p.z,sprite.sx,sprite.sy,sprite.ang&0xffffffff,TLIST,ANIM));store(ANIM,frameraw)
        pri=[-1.,-2.,0.,-5.25][case%4];depth=[0.,0.25,-0.25,1.][case%4];setdepth(depth);original('sprite_set_depth',0,0,depth,mat,case%2)
        check(fw(getdepth())==original('sprite_get_depth',0,0,0.,mat,case%2).banks[case%2][0],'real depth getter/setter')
        captured.clear();hints_before=hint();original(name,0,attr,pri,mat,case%2)
        if name=='njDrawSprite2D':draw2(C.byref(sprite),0,pri,attr)
        elif name=='njDrawSprite3D':draw3(C.byref(sprite),0,attr)
        else:drawgame(C.byref(sprite),0,attr)
        check(status()==0 and texstatus()==0,('native queue/texture status',name,case,status(),texstatus()))
        check(bytes(sprite)==before and bytes(frame)==frameraw,'sprite/frame inputs preserved')
        check(untimed()==0,'balanced game timing observer')
        li=1 if attr&32 else 0;committed=word(CURSORS+4*indices[li])-bases[li]
        check(used(li)==committed,('committed byte count',name,case,used(li),committed))
        check(hint()==hints_before+len(captured),'same header/vertex prefetch count')
        outcomes[name+('_culled' if not committed else '_drawn')]=outcomes.get(name+('_culled' if not committed else '_drawn'),0)+1
        if committed:
            check(committed%32==0 and data(li)>0xffffffff,'full-width, aligned committed stream')
            by_offset={addr-((bases[li]&0x3ffffff)|0xe0000000):v for addr,v in captured}
            check(set(by_offset)==set(range(0,committed,32)),'every committed header/vertex emitted once')
            native=struct.unpack('<'+'I'*(committed//4),C.string_at(data(li),committed))
            for offset,row in by_offset.items():
                for j in range(8):
                    if offset==0 or j<7:check(native[offset//4+j]==row[j],('packet',name,case,offset,j,hex(native[offset//4+j]),hex(row[j])));packet_words+=1
                    else:check(native[offset//4+j]==0,'undefined vertex word zero')
            check(last()==data(li)+next(reversed(by_offset)),'actual header/vertex prefetch ordering endpoint')
        elif name=='njDrawSprite3D':check(C.string_at(data(li),9*32)==bytes(9*32),'culled header reservation discarded')
        for i,j in enumerate(indices):
            check(word(CURSORS+4*j)-bases[i]==used(i),'all original/native list cursor offsets')
            if data(i):check(cursor(i)==descriptor(i)==data(i)+used(i),'native parameter/descriptor agreement')
        out=(U*4)();check(getctx(0,out)==0 and list(out)==[word(CTX+144+4*i) for i in range(4)],('context',name,case))
        check(cache.value==word(table['_nj_current_texture']) and bank_cache.value==word(table['_nj_current_texture_bank']),'actual texture cache state')
    check(init(8<<20)==0,'queue initialize')
    for name in ['njDrawSprite2D','njDrawSprite3D','sprite_draw_3d']:
        for case in range(512):scenario(name,case)
        print(name,'passed',flush=True)
    for name,masks in near_masks.items():
        check(bool(masks),('original clipping body reached',name))
        check({m>>1 for m in masks}|{m&7 for m in masks}==set(range(8)),('all triangle clipping table indices',name,sorted(masks)))
    # Complete SDK texture selector: cache miss/hit, bank changes, address
    # rejection and the original update-before-rejection cache behavior.
    mat=(F*16)(*[1. if i%5==0 else 0. for i in range(16)])
    tlist=prepare();rec=Record.from_address(record(0));iw(TNAME,[0,0,REC]);iw(TLIST,[TNAME,1]);iw(table['_nj_current_texture'],[cache.value]);iw(table['_nj_current_texture_bank'],[bank_cache.value])
    select_texture(tlist);original('njSetTexture',0,0,0.,mat,arg0=TLIST)
    for i in range(64):
        rec.globalIndex=1000+i;rec.bank=(i%4)*16;rec.tspparambuffer=rp.getrandbits(32);rec.texparambuffer=(rp.getrandbits(32)&~0x1fffff)|[0,0x100000,0x100001,0x1fffff][i%4]
        iw(REC,[rec.globalIndex,rec.bank,rec.tspparambuffer,rec.texparambuffer,rec.texaddr]);get();original('njGetkmPointer',0,0,0.,mat)
        for repeat in range(2):
            expected=original('njSetkmTextureNum',0,0,0.,mat,arg0=0)
            check(select_index(0)==C.c_int32(expected.reference_r0).value,'SDK selector return/cache hit')
            param=(U*4)();check(getparam(param)==0 and list(param)==[word(table['_nj_parameterkm_']+4*j) for j in range(4)],'SDK selector parameter bits')
            check(cache.value==word(table['_nj_current_texture']) and bank_cache.value==word(table['_nj_current_texture_bank']),'SDK selector cache words')
        put();original('njSetkmPointer',0,0,0.,mat)
    # GCC policy has no original-JP counterpart. Validate suppression/balanced
    # timing using the real compiled game entry, outside the original counts.
    check(reset()==0,'policy reset');frame=Frame(16,16,8,8,0,0,256,256,0,0);sprite=Sprite(Point(0,0,2),1,1,0,prepare(),C.addressof(frame))
    frame_step(2,0);before=recordcalls();drawgame(C.byref(sprite),0,16);check(used(0)==0 and recordcalls()==before and untimed()==0,'game frame suppression')
    frame_step(0,0);callback(1,0);before=recordcalls();draw3(C.byref(sprite),0,16)
    check(used(0)==0 and recordcalls()==before+1 and status()==0,'SDK replay callback suppresses 3D draw')
    callback(0,1);shutdown();lib.jp_textures_shutdown()
    check(set(required)<=seen,'whole original sprite and SDK dependencies reached')
    report=dict(status='complete_original_sprite_packet_contracts_verified',checks=checks,defined_packet_words_compared=packet_words,
        original_calls_by_function=counts,original_draw_calls=sum(counts.get(n,0) for n in ['njDrawSprite2D','njDrawSprite3D','sprite_draw_3d']),outcomes=outcomes,
        original_near_clip_masks={n:sorted(v) for n,v in near_masks.items()},all_eight_triangle_clipping_table_indices_reached=True,additional_direct_texture_selector_calls=128,
        targeted_offscreen_directions=['left','right','top','bottom'],targeted_cardinal_and_wrapped_angles=True,targeted_signed_UV_extremes_and_degenerate_sizes=True,
        original_function_bodies_executed=sorted(seen),original_dependency_hooks_used=False,original_binary_sha256=sha(a.binary),numeric_DLL_sha256=sha(a.numeric_dll),
        DLL_sha256=sha(dll),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),reference_hashes=reference_hashes,
        actual_native_texture_owners_and_SDK_selection=True,diagnostic_replay_and_frame_observers=True,complete_production_replay_executed=False,
        numerical_policy='RZ_DN1_value_only',FPSCR_flags_compared=False,hardware_validation=False,full_frame_executed=False,rendered_pixels_verified=False,
        gameplay_verified=False,playable=False,overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],checks,'checks;',report['original_draw_calls'],'whole original sprite draws')

if __name__=='__main__':main()
