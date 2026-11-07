"""Verify the real retained queue receives the original unlit triangle packets."""
import argparse
import ctypes as c
import json
import struct
from pathlib import Path

from build_native_simple_material import digest
from native_packet_test_support import bind
from verify_native_model_preflight import Material, Mesh, Model, Vec

I,U,S,P,F=c.c_int32,c.c_uint32,c.c_size_t,c.c_void_p,c.c_float


def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--build',type=Path,required=True)
    ap.add_argument('--output',type=Path,required=True)
    a=ap.parse_args()
    b=json.loads((a.build/'build_verification.json').read_bytes())
    assert digest(b['frames_DLL'])==b['frames_DLL_sha256']
    f=c.CDLL(b['frames_DLL'])
    def api(n,result,*args):
        return bind(f,n,result,*args)
    draw=api('jp_simple_draw_unlit_triangles',I,P,S,S,S,S,P,F,P)
    qinit=api('jp_queue_initialize',I,S)
    finit=api('jp_frames_initialize',I,U,U,S)
    qreset=api('jp_queue_reset_frame',I)
    used=api('jp_queue_committed_bytes',S,U)
    data=api('jp_queue_data',P,U)
    assert qinit(1<<20)==finit(4,16,1<<20)==0
    context=(U*4)(0x8000000a,0x20800000,0x20800000,0)
    assert api('jp_queue_set_context',I,U,P)(0,context)==0
    assert api('jp_simple_calc_initialize',I,U,S)(8,320)==0
    matrix=(F*16)(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1)
    api('njSetMatrix',None,P,P)(None,matrix)
    (F*5).in_dll(f,'_nj_screen_')[:]=[256,640,480,320,240]
    (F*4).in_dll(f,'_nj_2d_clip_')[:]=[0,0,640,480]
    F.in_dll(f,'_nj_near_clip_').value=1
    F.in_dll(f,'_nj_far_clip_').value=1000
    F.in_dll(f,'port_draw_aspect').value=1
    F.in_dll(f,'_nj_scr_aspect_y_').value=-1
    control=U.in_dll(f,'_nj_control_3d_flag_')
    (F*4).in_dll(f,'_nj_constant_material_')[:]=[.5,.75,.25,1]
    light_state=(U*13).in_dll(f,'_nj_light_material_')
    light_state[:]=[0x3e800000+i for i in range(13)]
    saved_light=list(light_state)
    oldbuf=c.create_string_buffer(b'\x93'*48,48)
    vertex=P.in_dll(f,'_nj_vertex_buf_')
    vertex_count=I.in_dll(f,'_nj_vertex_num_')
    vertex.value,vertex_count.value=c.addressof(oldbuf),3
    calc_count=I.in_dll(f,'_nj_calc_polygon_')
    draw_count=I.in_dll(f,'_nj_draw_polygon_')
    points=(Vec*3)(Vec(-1,0,8),Vec(1,0,8),Vec(0,1,8))
    indices=(c.c_int16*6)(0,1,2,0,2,1)
    mesh=Mesh(0,2,indices)
    mat=Material(0xffffffff,0,1,0,0)
    model=Model(points,None,3,c.pointer(mesh),c.pointer(mat),1,1,Vec(0,0,8),2)
    light=Vec(.5,-1,.75)
    cases={}
    for name,alpha,change,expected_status,expected_value,expected_calc,expected_draw in [
        ('opaque',0,'',0,0,6,3),('alpha',1,'',0,0,6,3),
        ('backface_only',0,'back',0,0,3,0),('culled',0,'culled',0,-1,0,0),
        ('partial_clip',0,'partial',101,123,0,0),('bad_index',0,'bad_index',100,123,0,0),
        ('short_span',0,'short',100,123,0,0),('textured',0,'textured',100,123,0,0)]:
        assert qreset()==0
        control.value=0x110
        mat.flags=0x02000000|(0x100000 if alpha else 0)|(0x200000 if change=='textured' else 0)
        calc_count.value,draw_count.value=0,0
        indices[:]=[0,2,1,0,2,1] if change=='back' else [0,1,2,0,2,1]
        if change=='bad_index':
            indices[5]=-1
        mesh.count=1 if change=='back' else 2
        model.center=Vec(0,0,-10 if change=='culled' else 8)
        points[0]=Vec(-12 if change=='partial' else -1,0,8)
        model.radius=16 if change=='partial' else 2
        result=I(123)
        rc=draw(c.byref(model),3,0,5 if change=='short' else 6,1,c.byref(light),.8,c.byref(result))
        assert (rc,result.value,calc_count.value,draw_count.value)==(
            expected_status,expected_value,expected_calc,expected_draw),name
        assert vertex.value==c.addressof(oldbuf) and vertex_count.value==3 and bytes(oldbuf)==b'\x93'*48
        assert list(light_state)==saved_light
        lane=int(bool(alpha))
        assert used(1-lane)==0
        raw=c.string_at(data(lane),used(lane)) if used(lane) else b''
        assert len(raw)==(128 if expected_draw else 32 if change=='back' else 0),name
        if raw:
            assert struct.unpack_from('<8I',raw)==(
                0x82000010 if alpha else 0x80000010,0x20800000,
                0x00900000 if alpha else 0x20880000,0,0,0,0,0),(name,raw[:32].hex())
        if expected_draw:
            expected=b''.join(struct.pack('<I7f',0xf0000000 if j==2 else 0xe0000000,
                            x,y,.125,.5,.75,.25,1) for j,(x,y) in enumerate([(288,240),(352,240),(320,208)]))
            assert raw[32:]==expected,name
        cases[name]=dict(status=rc,projection=result.value,calculated=expected_calc,drawn=expected_draw,packet_hex=raw.hex())
    with a.output.open('x') as out:
        json.dump(dict(cases=cases,dll_sha256=b['frames_DLL_sha256'],queue_packet_bytes_verified=True,
                       actual_GPU_present=False,original_SH4_execution_compared=False),out,indent=2)
        out.write('\n')
    print(len(cases),'unlit triangle integration cases passed',flush=True)


if __name__=='__main__':
    main()
