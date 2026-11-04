"""Verify all ordinary packet layouts and actual model/sprite/strip captures.
Independent constructed values, strip geometry, protected boundaries and real
producer snapshots; not a hardware color-rounding or pixel comparison proof.
"""
import argparse,ctypes as C,hashlib,json,random,struct
from pathlib import Path
from native_packet_test_support import Decoder,Span as PacketSpan,Failure,Vertex,Triangle,Batch,Plan,bind
from native_texture_test_support import Runtime,Info,Record,Name,List,field
from verify_native_model_preflight import Vec,Material,Mesh,Model,Span,Bound
from verify_native_sprites import Point,Frame,Sprite
U=C.c_uint32;I=C.c_int;F=C.c_float;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def fw(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def pack(words):return struct.pack('<'+'I'*len(words),*words)

def fixture(ct,texture,uv16,offset,n,lane=0,previous=False):
    control=0x80000000|([0,2,4][lane]<<24)|(ct<<4)|(texture<<3)|(offset<<2)|2|uv16
    face=[0.625,0.25,0.5,0.75];spec=[0.125,0.875,0.375,0.0625]
    header=[control,0xe0000000,0x20800000,0x08000001]+[0x7fc00000]*4
    if ct==2:
        if offset:header += [fw(v) for v in face+spec]
        else:header[4:8]=[fw(v) for v in face]
    if previous:header[0]=(control&~0x30)|0x30
    tex_type=(3 if ct==0 else 5 if ct==1 else 7)+uv16
    typ=tex_type if texture else 0 if ct==0 else 1 if ct==1 else 2
    vertices=[];expected=[]
    for j in range(n):
        x,y,z,u,v=float(j),float(j%2),0.25,0.125,0.75
        w=[0xf0000000 if j==n-1 else 0xe0000000,fw(x),fw(y),fw(z),0x7fc00000,0x7fc00000,0x7fc00000,0x7fc00000]
        if texture:
            if uv16:w[4]=(fw(u)&0xffff0000)|(fw(v)>>16)
            else:w[4:6]=[fw(u),fw(v)]
        base=[32/255,64/255,128/255,192/255];off=[16/255,48/255,80/255,112/255]
        if ct==0:w[6:8]=[0xc0204080,0x70103050]
        elif ct==1:
            base=[0.25,0.5,0.75,0.625];off=[0.875,0.375,0.0625,0.125]
            argb=[fw(v) for v in [0.625,0.25,0.5,0.75]];oargb=[fw(v) for v in [0.125,0.875,0.375,0.0625]]
            if texture:w+=argb+oargb
            else:w[4:8]=argb
        else:
            base=[face[i] for i in [1,2,3,0]];off=[spec[i] for i in [1,2,3,0]];w[6]=fw(0.5)
            if texture:w[7]=fw(0.25)
        expected.append((x,y,z,u if texture else 0.,v if texture else 0.,base,off if offset and texture else [0.]*4,0.5 if ct>=2 else 1.,0.25 if ct>=2 and offset and texture else 1.))
        vertices+=w
    return pack(header+vertices),typ,expected

def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();assert not a.report.exists()
    b=json.loads((d/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(Path(b['frames_DLL']))==b['frames_DLL_sha256'] and sha(Path(b['DLL']))==b['DLL_sha256']
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;decoder=Decoder(Path(b['DLL']));checks=cases=actual_draws=actual_frames=0;groups={};layouts=set();rng=random.Random(0x5041434b)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    def decoded(raw,lane=0):
        lists=[b'']*5;lists[lane]=raw;rc,out,err=decoder.raw(lists);check(rc==0,f'decode {rc}/{err.list}/{err.offset}')
        plan=Plan.from_address(out.value);check(C.addressof(plan.triangles.contents)%C.alignment(Triangle)==0,'triangle alignment')
        return out,plan
    def expect_fail(lists,code=None):
        rc,out,err=decoder.raw(lists);check(rc!=0 and (code is None or rc==code),'explicit reject')
        check(out.value==0x12345678 and err.code==rc,'atomic output failure');return err
    for lane in range(3):
        for ct in range(3):
            for tex in range(2):
                for uv16 in range(2):
                    for offset in range(2):
                        for n in [1,2,3,4,5,16,33]:
                            raw,typ,expected=fixture(ct,tex,uv16,offset,n,lane);out,plan=decoded(raw,lane);cases+=1;layouts.add(typ)
                            check((plan.batch_count,plan.vertex_count,plan.triangle_count)==(1,n,max(0,n-2)),'decoded counts')
                            batch=plan.batches[0];check((batch.list,batch.vertex_type,batch.vertex_count,batch.triangle_count)==(lane,typ,n,max(0,n-2)),'batch layout')
                            check(list(batch.header)[:4]==list(struct.unpack('<4I',raw[:16])),'raw material bits retained')
                            for j,e in enumerate(expected):
                                v=plan.vertices[j]
                                got=[v.x,v.y,v.inv_w,v.u,v.v,*v.base,*v.offset,v.base_intensity,v.offset_intensity]
                                want=[*e[:5],*e[5],*e[6],*e[7:]]
                                check(all(abs(x-y)<1e-7 for x,y in zip(got,want)),'independent numeric/color fixture')
                            for k in range(max(0,n-2)):
                                check(list(plan.triangles[k].indices)==([k,k+1,k+2] if k%2==0 else [k+1,k,k+2]),'alternating strip winding')
                            cost=plan.allocation_bytes;decoder.destroy(out)
                            lists=[b'']*5;lists[lane]=raw
                            rc,out,err=decoder.raw(lists,cost-1);check(rc==7 and out.value==0x12345678,'budget failure atomic')
                            rc,out,err=decoder.raw(lists,cost);check(rc==0,'exact allocation budget');decoder.destroy(out)
    groups['all_nine_layouts_three_lists_and_budget_boundaries']=checks
    # Header face-cache reuse, strip resets and malformed transitions.
    for lane in range(3):
        seed,_,_=fixture(2,1,0,1,3,lane)
        prev,_,_=fixture(2,1,0,1,5,lane,True)
        # previous-face headers are 32 bytes, without the explicit face tail.
        prev=prev[:32]+prev[64:]
        out,plan=decoded(seed+prev,lane);check((plan.batch_count,plan.vertex_count,plan.triangle_count)==(2,8,4),'previous face counts')
        check(list(plan.vertices[3].base)==list(plan.vertices[0].base),'previous face preserved');decoder.destroy(out)
        lists=[b'']*5;lists[lane]=prev;expect_fail(lists,4)
        raw,_,_=fixture(0,0,0,0,5,lane)
        out,plan=decoded(raw[:96]+pack([0xf0000000])+raw[100:],lane)
        check(plan.triangle_count==1,'strip restart does not bridge triangles');decoder.destroy(out)
        for size in [32+32,32+64,32+96,32+128]:
            lists[lane]=raw[:size];expect_fail(lists,4)
        lists[lane]=raw[32:];expect_fail(lists,4)
        bad=bytearray(raw);struct.pack_into('<I',bad,4+32,0x7f800000);lists[lane]=bytes(bad);e=expect_fail(lists,6);check(e.offset==32 and e.list==lane,'failure location')
        lists[lane]=raw[:-1];expect_fail(lists,1)
    raw,_,_=fixture(1,1,1,1,3)
    expect_fail([raw[:64],b'',b'',b'',b''],2)
    for control in [0xa0000000,0x80000040,0x20000000,0,0xdeadbeef]:
        bad=pack([control]+[0]*7);expect_fail([bad,b'',b'',b'',b''])
    for lane in [3,4]:
        lists=[b'']*5;lists[lane]=bytes(32);expect_fail(lists,5)
    for lane in [1,2]:
        raw,_,_=fixture(0,0,0,0,3);lists=[b'']*5;lists[lane]=raw;expect_fail(lists,4)
    for n in [0,1,31,33,63]:expect_fail([b'X'*n if n else b'X',b'',b'',b'',b''],1)
    groups['cache_strip_command_nonfinite_and_truncation_failures']=checks-sum(groups.values())
    # Place each input immediately before a NOACCESS page. Any speculative
    # read beyond the exact span fails the process, including 64-byte forms.
    kernel=C.WinDLL('kernel32',use_last_error=True)
    alloc=bind(kernel,'VirtualAlloc',P,P,S,U,U);protect=bind(kernel,'VirtualProtect',I,P,S,U,C.POINTER(U));free=bind(kernel,'VirtualFree',I,P,S,U)
    page=4096;address=alloc(None,page*2,0x3000,4);check(bool(address),'guard allocation');old=U()
    check(protect(address+page,page,1,C.byref(old))!=0,'NOACCESS trailing page')
    for ct in range(3):
        raw,_,_=fixture(ct,1,1,1,3)
        for length in range(0,len(raw)+1,32):
            ptr=address+page-length
            if length:C.memmove(ptr,raw,length)
            spans=(PacketSpan*5)(PacketSpan(ptr,length),PacketSpan(),PacketSpan(),PacketSpan(),PacketSpan());out=P(0x12345678);err=Failure()
            rc=decoder.decode(spans,1<<20,C.byref(out),C.byref(err))
            check(rc in [0,2,4],'bounded protected input')
            if not rc:decoder.destroy(out)
            else:check(out.value==0x12345678,'protected failure atomic')
    check(free(address,0,0x8000)!=0,'guard free')
    groups['protected_page_exact_input_boundaries']=checks-sum(groups.values())
    # Real complete parents and real frame/resource owners, with valid values.
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,C.POINTER(Q));release=bind(lib,'jp_frame_release',I,Q)
    reserve=bind(lib,'jp_queue_reserve',I,U,U);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None);ctx=bind(lib,'jp_queue_set_context',I,U,P)
    init=bind(lib,'jp_render_state_initialize',I,U,S);matrix=bind(lib,'njSetMatrix',None,P,P);modelinit=bind(lib,'draw_model_init',None);light=bind(lib,'draw_set_light',None,F,F,F)
    mb=bind(lib,'draw_models_begin',None);me=bind(lib,'draw_models_end',None);dm=bind(lib,'draw_model_unculled',None,P)
    measure=bind(lib,'jp_model_preflight',I,P,S,S,S,S,P,P);mr=bind(lib,'jp_model_reserve',I,P)
    draw2=bind(lib,'njDrawSprite2D',None,P,I,F,U);draw3=bind(lib,'njDrawSprite3D',None,P,I,U);flat=bind(lib,'njDrawTexture',None,P,I,I,I)
    pin=bind(lib,'jp_frames_prepare_texture',I,U);frame=bind(lib,'jp_sprite_probe_frame',None,I,I);callback=bind(lib,'jp_sprite_probe_callback',None,I,I)
    status=bind(lib,'jp_queue_status',I);flist=bind(lib,'jp_frame_list',P,Q,U,C.POINTER(S));surface=bind(lib,'jp_frame_surface',P,Q,U,C.POINTER(S))
    control=U.in_dll(lib,'_nj_control_3d_flag_');check(qi(16<<20)==0 and fi(4,8,8<<20)==0 and init(2048,1<<20)==0,'real owners initialize')
    check(t.init(8,128,1<<20)==0,'real texture initialize')
    info=Info();s=info.texsurface;s.Type=0x10000;s.BitDepth=1;s.PixelFormat=0x08000000;s.nWidth=s.nHeight=8;s.TextureSize=128;s.fSurfaceFlags=4
    payload=pack([0xffff0000]*32);check(t.copy(C.byref(info),C.create_string_buffer(payload),len(payload))==0,'encoded payload copy')
    names=(Name*1)();t.setname(C.byref(names[0]),None,1000,0);t.entry(field(names[0]),C.byref(info),1000,2,0xffffffff);t.param(t.resolve(names[0].texaddr))
    listing=List(names,1);check(t.select(C.byref(listing))==1,'actual SDK selector');word=t.surface_word(C.byref(s));tcw=Record.from_address(t.resolve(names[0].texaddr)).texparambuffer
    for iteration in range(32):
        # Context replacement must agree with the still-live SDK texture cache.
        # A zero TCW while _nj_current_texture still names 1000 would make the
        # original model's legitimate cache hit skip its texture setter.
        check(reset()==0 and ctx(0,(U*4)(0x8000000a,0xe0800000,0x20800000,tcw))==0,'actual header context');check(pin(word)==0,'texture ownership preflight')
        control.value=0;identity=(F*16)(*[1. if i%5==0 else 0. for i in range(16)]);matrix(None,identity);modelinit();light(0,0,1);frame(0,0);callback(0,1)
        pts=(Vec*8)(*(Vec((j%3-1)*0.25,(j%2-0.5)*0.25,0.5 if (j+iteration)%4==0 else 2.) for j in range(8)));norms=(Vec*8)(*(Vec(0,0,1) for _ in range(8)))
        meshes=(Mesh*3)();materials=(Material*3)();spans=(Span*3)();arrays=[]
        for j in range(3):
            indices=(C.c_int16*3)(j,j+1,j+2);uvs=(C.c_int16*6)(0,0,255,0,128,255);arrays.extend([indices,uvs])
            meshes[j]=Mesh(j,1,indices,None,None,None,C.cast(uvs,P));materials[j]=Material(0xffffffff,0,1.,0,0x200000|(0x400000 if j==2 else 0)|(0x100000 if j==1 else 0));spans[j]=Span(3,3)
        model=Model(pts,norms,8,meshes,materials,3,3,Vec(0,0,2),3.);bound=Bound()
        check(measure(C.byref(model),8,8,3,3,spans,C.byref(bound))==0 and mr(C.byref(bound))==0,'actual model bound')
        mb();dm(C.byref(model));me();actual_draws+=1
        animation=Frame(16,16,8,8,0,0,256,256,0,0);sprite=Sprite(Point(320,240,2),1,1,0,C.addressof(listing),C.addressof(animation))
        draw2(C.byref(sprite),0,F(0.5),0x10);sprite.p=Point(0,0,2);draw3(C.byref(sprite),0,0x10);actual_draws+=2
        # NJS_TEXTURE_VTX: x,y,z,u,v,color. Finite test geometry, not arbitrary
        # packet bytes used by the earlier lifetime-only verifier.
        flat_vertices=(U*18)(fw(100),fw(100),fw(0.5),fw(0),fw(0),0xffffffff,fw(200),fw(100),fw(0.5),fw(1),fw(0),0xffffffff,fw(100),fw(200),fw(0.5),fw(0),fw(1),0xffffffff)
        flat(flat_vertices,3,1000,0);actual_draws+=1;check(status()==0,'actual producers complete');fid=Q();check(submit(C.byref(fid))==0,'real frame submit');actual_frames+=1
        out=P();err=Failure();rc=decoder.frame(fid,16<<20,C.byref(out),C.byref(err))
        if rc:
            size=S();ptr=flist(fid,err.list,C.byref(size));raw=C.string_at(ptr,size.value)
            print('Failing actual list, 32-byte words:',[(at,struct.unpack('<8I',raw[at:at+32])) for at in range(0,len(raw),32)])
        check(rc==0,f'actual frame decode {iteration}: {rc}/{err.list}/{err.offset}')
        plan=Plan.from_address(out.value);check(plan.batch_count>=4 and plan.triangle_count>=4,'actual emitted geometry consumed')
        for k in range(plan.batch_count):
            batch=plan.batches[k];check(batch.list in [0,1,2] and batch.first_vertex+batch.vertex_count<=plan.vertex_count and batch.first_triangle+batch.triangle_count<=plan.triangle_count,'actual batch bounds')
            if batch.header[0]&8:
                size=S();ptr=surface(fid,batch.header[3]&0x1fffff,C.byref(size));check(bool(ptr) and C.string_at(ptr,size.value)==payload,'material references actual retained surface')
        # Decoder owns all geometry, including after the original frame release.
        saved=C.string_at(out.value,plan.allocation_bytes);check(release(fid)==0,'real owner completion');check(C.string_at(out.value,plan.allocation_bytes)==saved,'decoded geometry independently owned')
        decoder.destroy(out);sentinel=P(0x12345678);check(decoder.frame(fid,1<<20,C.byref(sentinel),C.byref(err))==10 and sentinel.value==0x12345678,'stale frame rejected atomically')
        bind(lib,'jp_frames_clear_error',None)()
    qs();t.shutdown();check(fs()==0,'owner cleanup');groups['complete_actual_producers_and_resource_identity']=checks-sum(groups.values())
    report=dict(status='native_ordinary_packet_consumer_verified',checks=checks,constructed_layout_cases=cases,vertex_layouts=sorted(layouts),groups=groups,
        actual_complete_parent_draws=actual_draws,actual_owned_frames=actual_frames,protected_page_boundaries_verified=True,decoded_geometry_survives_frame_release=True,
        DLL_sha256=sha(Path(b['DLL'])),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),
        fixture_scope='all nine ordinary vertex layouts, raw finite RGBA/face/intensity data, strips, ownership, strict errors; not hardware quantization or pixels',
        modifier_hardware_sprite_two_volume_tile_clip_supported=False,rasterizer_implemented=False,game_frame_executed=False,playable=False,overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))

if __name__=='__main__':main()
