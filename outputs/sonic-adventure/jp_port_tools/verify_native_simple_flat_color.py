"""Verify the real retained queue receives the flat-lit triangle packets."""
import argparse
import ctypes as c
import json
import struct
from pathlib import Path

from build_native_simple_material import digest
from native_packet_test_support import bind
from verify_native_model_preflight import Material, Mesh, Model, Vec
from verify_native_ninja_global import Control

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
    draw=api('jp_simple_draw_model_bounded',I,P,S,S,S,P,S,P)
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
    class Attr(c.Structure):
        _fields_=[('source',I),('ambient',F),('diffuse',F),('specular',F),
                  ('near',F),('far',F),('callback',P),('inner',I),('outer',I),('argb',F*4)]
    class Light(c.Structure):
        _fields_=[('matrix',F*16),('position',Vec),('direction',Vec),
                  ('stat',I),('reserved',I),('calculated',F*50),('attr',Attr)]
    assert Light.attr.offset==296
    light=Light()
    light.matrix[:]=matrix
    light.direction=Vec(1,0,0)
    light.attr.ambient=.5
    light.attr.diffuse=.25
    (F*16).in_dll(f,'_nj_view_matrix_')[:]=[0,1,0,0,-1,0,0,0,0,0,1,0,0,0,0,1]
    owner=Control.in_dll(f,'_nj_light_control_')
    owner.lights[0]=c.addressof(light)
    direction,scale,ambient=Vec(),F(),F()
    prepare=api('jp_simple_prepare_base_light',I,P,P,P)
    assert prepare(c.byref(direction),c.byref(scale),c.byref(ambient))==0
    observed=(direction.x,direction.y,direction.z,scale.value,ambient.value)
    assert observed==(0,1,0,.25,.5),observed
    points=(Vec*3)(Vec(-1,0,8),Vec(1,0,8),Vec(0,1,8))
    normals=(Vec*3)(Vec(0,1,0),Vec(0,1,0),Vec(0,0,0))
    indices=(c.c_int16*6)(0,1,2,0,2,1)
    mesh=Mesh(0,2,indices)
    mat=Material(0xffffffff,0,1,0,0)
    model=Model(points,normals,3,c.pointer(mesh),c.pointer(mat),1,1,Vec(0,0,8),2)
    class Span(c.Structure):
        _fields_=[('indices',S),('uvs',S)]
    span=Span(3,0)
    cases={}
    for name,z0,z2,flip,alpha,expected,culled in [
        ('one_near',0,2,False,False,[(320,240,1,.5),(256,176,1,.25),(384,240,.5,1),(320,176,.5,.5)],False),
        ('two_near_alpha',0,0,False,True,[(320,240,1,.5),(384,176,1,.75),(384,240,.5,1)],False),
        ('near_backface',0,2,True,False,[],False),
        ('all_near',0,0,False,False,[],True),
        ('unclipped',2,2,True,False,[(256,240,.5,0),(384,240,.5,1),(320,176,.5,.5)],False),
        ('partial_side',2,2,True,False,[(-192,240,.5,0),(384,240,.5,1),(320,176,.5,.5)],False),
    ]:
        assert qreset()==0
        control.value=0x110
        calc_count.value=draw_count.value=0
        mat.flags=0x01000000|(0x100000 if alpha else 0)
        mesh.count=1
        indices[:3]=[0,1,2] if flip else [0,2,1]
        model.center=Vec(0,0,2)
        model.radius=16
        points[:]=[Vec(-4 if name=='partial_side' else -.5,0,z0),
                   Vec(.5,0,0 if culled else 2),Vec(0,.5,z2)]
        result=I(123)
        assert draw(c.byref(model),3,3,1,c.byref(span),1,c.byref(result))==0,name
        assert (result.value,calc_count.value,draw_count.value)==(-1 if culled else 0,0 if culled else 3,len(expected)),name
        assert vertex.value==c.addressof(oldbuf) and vertex_count.value==3 and bytes(oldbuf)==b'\x93'*48
        assert list(light_state)==saved_light
        lane=int(alpha)
        assert used(1-lane)==0
        raw=c.string_at(data(lane),used(lane)) if used(lane) else b''
        header=struct.pack('<8I',0x82000010 if alpha else 0x80000010,0x20800000,
                           0x00900000 if alpha else 0x20880000,0,0,0,0,0)
        assert len(raw)==(0 if culled else 32+32*len(expected)),name
        if not culled:
            assert raw[:32]==header,name
        for j,(x,y,q,_) in enumerate(expected):
            command=0xf0000000 if j==len(expected)-1 else 0xe0000000
            assert raw[32+32*j:48+32*j]==struct.pack('<I3f',command,x,y,q),name
            if j>=2:
                color=raw[48+32*j:64+32*j]
                expected_color=struct.pack('<4f',.5,.1875,.0625,.25)
                assert color==expected_color,(name,color.hex(),expected_color.hex())
        cases[name]=dict(vertices=len(expected),model_culled=culled,packet_hex=raw.hex())
    with a.output.open('x') as out:
        json.dump(dict(cases=cases,dll_sha256=b['frames_DLL_sha256'],defined_packet_bytes_verified=True,
                       actual_GPU_present=False,original_SH4_execution_compared=False),out,indent=2)
        out.write('\n')
    print(len(cases),'flat-lit triangle packet cases passed',flush=True)


if __name__=='__main__':
    main()
