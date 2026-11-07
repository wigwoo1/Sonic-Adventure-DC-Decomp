"""Verify original Simple action traverses the SDK draw entry with explicit model spans."""
import argparse
import ctypes as c
import json
import struct
from pathlib import Path

from build_native_simple_material import digest
from native_packet_test_support import bind
from verify_native_model_preflight import Material, Mesh, Model, Vec
from verify_native_ninja_global import Control
from verify_native_character_tree import Node
from verify_native_action_poses import Action, Motion

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
    draw=api('njSimpleDrawModel',None,P)
    action=api('njSimpleAction',None,P,F)
    begin=api('jp_simple_model_scope_begin',I,P,S)
    end=api('jp_simple_model_scope_end',I)
    assert api('jp_queue_initialize',I,S)(1<<20)==0
    assert api('jp_frames_initialize',I,U,U,S)(4,16,1<<20)==0
    assert api('jp_queue_set_context',I,U,P)(0,(U*4)(0x8000000a,0x20800000,0x20800000,0))==0
    assert api('jp_simple_calc_initialize',I,U,S)(8,320)==0
    stack=c.create_string_buffer(32+8*64)
    api('njInitMatrix',None,P,I,I)((c.addressof(stack)+31)&~31,8,0)
    matrix=(F*16)(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1)
    api('njSetMatrix',None,P,P)(None,matrix)
    (F*5).in_dll(f,'_nj_screen_')[:]=[256,640,480,320,240]
    (F*4).in_dll(f,'_nj_2d_clip_')[:]=[0,0,640,480]
    F.in_dll(f,'_nj_near_clip_').value=1
    F.in_dll(f,'_nj_far_clip_').value=1000
    F.in_dll(f,'port_draw_aspect').value=1
    F.in_dll(f,'_nj_scr_aspect_y_').value=-1
    U.in_dll(f,'_nj_control_3d_flag_').value=0x110
    (F*4).in_dll(f,'_nj_constant_material_')[:]=[.5,.75,.25,1]
    calc=I.in_dll(f,'_nj_calc_polygon_')
    drawn=I.in_dll(f,'_nj_draw_polygon_')
    scratch=c.create_string_buffer(b'\x75'*64,64)
    vertex=P.in_dll(f,'_nj_vertex_buf_')
    count=I.in_dll(f,'_nj_vertex_num_')
    vertex.value,count.value=c.addressof(scratch),4
    indices=(c.c_int16*3)(0,1,2)
    mesh=Mesh(0,1,indices)
    class Span(c.Structure):
        _fields_=[('indices',S),('uvs',S)]
    class ModelSpan(c.Structure):
        _fields_=[('model',P),('points',S),('normals',S),('materials',S),('spans',P),('meshsets',S)]
    span=Span(3,0)
    mat=Material(0xffffffff,0,1,0,0x02000000)
    points=(Vec*3)(Vec(-.5,0,2),Vec(.5,0,2),Vec(0,.5,2))
    model=Model(points,None,3,c.pointer(mesh),c.pointer(mat),1,1,Vec(0,0,2),2)
    entry=ModelSpan(c.addressof(model),3,0,1,c.addressof(span),1)
    node=Node(0,c.addressof(model),(F*3)(.25,0,0),(I*3)(0,0,0),(F*3)(1,1,1),None,None)
    motion=Motion(None,4,0,0)
    act=Action(c.addressof(node),None)
    class Attr(c.Structure):
        _fields_=[('source',I),('ambient',F),('diffuse',F),('specular',F),
                  ('near',F),('far',F),('callback',P),('inner',I),('outer',I),('argb',F*4)]
    class Light(c.Structure):
        _fields_=[('matrix',F*16),('position',Vec),('direction',Vec),
                  ('stat',I),('reserved',I),('calculated',F*50),('attr',Attr)]
    assert Light.attr.offset==296
    light=Light()
    light.matrix[:]=[1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1]
    light.direction=Vec(1,0,0)
    light.attr.ambient=.5
    light.attr.diffuse=.5
    (F*16).in_dll(f,'_nj_view_matrix_')[:]=[0,1,0,0,-1,0,0,0,0,0,1,0,0,0,0,1]
    owner=Control.in_dll(f,'_nj_light_control_')
    owner.lights[0]=c.addressof(light)
    direction,scale,ambient=Vec(),F(),F()
    prepare=api('jp_simple_prepare_base_light',I,P,P,P)
    assert prepare(c.byref(direction),c.byref(scale),c.byref(ambient))==0
    observed=(direction.x,direction.y,direction.z,scale.value,ambient.value)
    assert observed==(0,1,0,.5,.5),observed
    reset=api('jp_queue_reset_frame',I)
    used=api('jp_queue_committed_bytes',S,U)
    data=api('jp_queue_data',P,U)
    cases={}
    for name,expected_error,emits in [
        ('direct',0,True),('action',0,True),('hidden',0,False),
        ('outside_motion',0,False),('unregistered',104,False),('bad_span',100,False)
    ]:
        assert reset()==0
        calc.value=drawn.value=0
        node.flags=8 if name=='hidden' else 0
        act.motion=c.addressof(motion) if name=='outside_motion' else None
        span.indices=2 if name=='bad_span' else 3
        assert begin(c.byref(entry),1)==0
        assert begin(c.byref(entry),1)==103
        if name=='direct':
            draw(c.byref(model))
        elif name=='unregistered':
            draw(1)
        else:
            action(c.byref(act),4 if name=='outside_motion' else 0)
        assert end()==expected_error,name
        assert end()==103
        current=(F*16)()
        api('njGetMatrix',None,P)(current)
        assert bytes(current)==bytes(matrix) and I.in_dll(f,'_stack_size_').value==1,name
        assert (calc.value,drawn.value)==((3,3) if emits else (0,0)),name
        assert vertex.value==c.addressof(scratch) and count.value==4 and bytes(scratch)==b'\x75'*64
        raw=c.string_at(data(0),used(0)) if used(0) else b''
        assert used(1)==0
        header=struct.pack('<8I',0x80000010,0x20800000,0x20880000,0,0,0,0,0)
        shift=32 if name=='action' else 0
        packet=header+b''.join(struct.pack('<I7f',0xf0000000 if j==2 else 0xe0000000,
                              x+shift,y,.5,.5,.75,.25,1) for j,(x,y) in enumerate([(256,240),(384,240),(320,176)]))
        assert raw==(packet if emits else b''),(name,raw.hex(),packet.hex())
        cases[name]=dict(status=expected_error,packet_hex=raw.hex())
    with a.output.open('x') as out:
        json.dump(dict(cases=cases,dll_sha256=b['frames_DLL_sha256'],original_SH4_execution_compared=False,
                       actual_GPU_present=False),out,indent=2)
        out.write('\n')
    print(len(cases),'Simple SDK scope and action packet cases passed',flush=True)


if __name__=='__main__':
    main()
