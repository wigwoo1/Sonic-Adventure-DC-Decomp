"""Check mixed materials and mesh types under one original projection lease."""
import argparse
import ctypes as c
import json
import struct
from pathlib import Path

from build_native_simple_material import digest
from native_packet_test_support import bind
from native_texture_test_support import Runtime, Info, Name, List, Record, field
from verify_native_model_preflight import Material, Mesh, Model, Vec
from verify_native_ninja_global import Control

I,U,S,P,F,Q=c.c_int32,c.c_uint32,c.c_size_t,c.c_void_p,c.c_float,c.c_uint64


def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--build',type=Path,required=True)
    ap.add_argument('--output',type=Path,required=True)
    a=ap.parse_args()
    b=json.loads((a.build/'build_verification.json').read_bytes())
    assert digest(b['frames_DLL'])==b['frames_DLL_sha256']
    t=Runtime(b['frames_DLL'])
    f=t.dll
    def api(n,result,*args):
        return bind(f,n,result,*args)
    draw=api('jp_simple_draw_model_bounded',I,P,S,S,S,P,S,P)
    assert api('jp_queue_initialize',I,S)(1<<20)==0
    assert api('jp_video_initialize',I,U,U,U,I)(640,480,0xff000000,1)==0
    assert api('jp_frames_initialize',I,U,U,S)(4,16,1<<20)==0
    assert api('jp_queue_set_context',I,U,P)(0,(U*4)(0x8000000a,0x20800000,0x20800000,0))==0
    assert api('jp_simple_calc_initialize',I,U,S)(8,320)==0
    assert t.init(8,128,1<<20)==0
    payload=bytes(range(128))
    info=Info()
    s=info.texsurface
    s.Type,s.BitDepth,s.PixelFormat,s.nWidth,s.nHeight,s.TextureSize,s.fSurfaceFlags=0x10000,1,0x08000000,8,8,128,4
    assert t.copy(c.byref(info),c.create_string_buffer(payload),128)==0
    names=(Name*1)()
    t.setname(c.byref(names[0]),None,1000,0)
    t.entry(field(names[0]),c.byref(info),1000,2,0xffffffff)
    address=t.resolve(names[0].texaddr)
    assert address and t.status()==0
    t.param(address)
    record=Record.from_address(address)
    tcw,tsp=record.texparambuffer,record.tspparambuffer
    assert tcw&0x1fffff==t.surface_word(c.byref(s))
    listing=List(names,1)
    assert t.select(c.byref(listing))==1
    api('njSetMatrix',None,P,P)(None,(F*16)(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1))
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
    indices=(c.c_int16*3)(0,1,2)
    strip=(c.c_int16*4)(3,0,1,2)
    uv=(c.c_int16*6)(-128,0,256,128,0,-256)
    meshes=(Mesh*4)(Mesh(0,1,indices),Mesh(0xc001,1,strip,None,None,None,c.addressof(uv)),
                     Mesh(1,1,indices,None,None,None,c.addressof(uv)),Mesh(2,1,indices))
    materials=(Material*3)(Material(0xffffffff,0,1,0,0x02000000),
                           Material(0xffffffff,0,1,0,0x00300000),Material(0xffffffff,0,1,0,0))
    points=(Vec*3)(Vec(-.5,0,2),Vec(.5,0,2),Vec(0,.5,2))
    normals=(Vec*3)(Vec(0,1,0),Vec(0,-1,0),Vec(0,0,1))
    model=Model(points,normals,3,meshes,materials,4,3,Vec(0,0,2),16)
    class Span(c.Structure):
        _fields_=[('indices',S),('uvs',S)]
    spans=(Span*4)(Span(3,0),Span(4,3),Span(3,3),Span(3,0))
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
    used=api('jp_queue_committed_bytes',S,U)
    data=api('jp_queue_data',P,U)
    submit=api('jp_frames_submit',I,c.POINTER(Q))
    surface=api('jp_frame_surface',P,Q,U,c.POINTER(S))
    frame_list=api('jp_frame_list',P,Q,U,c.POINTER(S))
    release=api('jp_frame_release',I,Q)
    cases={}
    projected=I.in_dll(f,'_nj_calc_vertex_')
    for name,invalid,expected_status in [('late_bad_index',True,100),('mixed_draw',False,0)]:
        calc.value=drawn.value=projected.value=0
        # Only the last mesh has a bad span. No earlier mesh may emit.
        spans[3].indices=2 if invalid else 3
        result=I(123)
        rc=draw(c.byref(model),3,3,3,spans,4,c.byref(result))
        assert rc==expected_status,(name,rc)
        if invalid:
            assert result.value==123 and not (calc.value or drawn.value or projected.value or used(0) or used(1))
            cases[name]=dict(status=rc,packets=0)
            continue
        assert (result.value,calc.value,drawn.value,projected.value)==(0,12,13,3)
        opaque=struct.pack('<8I',0x80000010,0x20800000,0x20880000,0,0,0,0,0)
        smooth=struct.pack('<8I',0x80000012,0x20800000,0x20880000|(tsp&63),0,0,0,0,0)
        texture=struct.pack('<4I4f',0x8200002a,0x20800000,0x00900000|(tsp&63),tcw,.5,.75,.25,1)
        def vertices(kind):
            packet=b''
            for j,(x,y,u,v,i) in enumerate([(256,240,-.5,0,0),(384,240,1,.5,1),(320,176,0,-1,.5)]):
                extra=(u,v,i,0) if kind=='texture' else (.5,.75,.25,1) if kind=='unlit' else (.5,.75*i,.25*i,i)
                packet+=struct.pack('<I7f',0xf0000000 if j==2 else 0xe0000000,x,y,.5,*extra)
            return packet
        packets=[opaque+vertices('unlit')+smooth+vertices('smooth'),(texture+vertices('texture'))*2]
        for lane,expected in enumerate(packets):
            raw=c.string_at(data(lane),used(lane))
            assert raw==expected,(lane,raw.hex(),expected.hex())
        frame=Q()
        assert submit(c.byref(frame))==0 and frame.value
        for lane,expected in enumerate(packets):
            size=S()
            ptr=frame_list(frame.value,lane,c.byref(size))
            assert ptr and c.string_at(ptr,size.value)==expected
        t.release_list(c.byref(listing))
        size=S()
        ptr=surface(frame.value,tcw&0x1fffff,c.byref(size))
        assert ptr and c.string_at(ptr,size.value)==payload
        assert release(frame.value)==0
        cases[name]=dict(status=rc,projected_vertices=projected.value,packet_hex=[raw.hex() for raw in packets])
    with a.output.open('x') as out:
        json.dump(dict(cases=cases,dll_sha256=b['frames_DLL_sha256'],texture_retained_after_release=True,
                       actual_GPU_present=False,original_SH4_execution_compared=False),out,indent=2)
        out.write('\n')
    print('Mixed model and late-input rejection passed',flush=True)


if __name__=='__main__':
    main()
