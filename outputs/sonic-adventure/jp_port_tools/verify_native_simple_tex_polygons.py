"""Verify textured unlit polygon UVs, clipping, and queue bytes."""
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

I,U,S,P,F=c.c_int32,c.c_uint32,c.c_size_t,c.c_void_p,c.c_float


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
    scratch=c.create_string_buffer(b'\x75'*64,64)
    vertex=P.in_dll(f,'_nj_vertex_buf_')
    count=I.in_dll(f,'_nj_vertex_num_')
    vertex.value,count.value=c.addressof(scratch),4
    indices=(c.c_int16*8)()
    uv=(c.c_int16*10)(-129,1,0,-256,256,128,64,32,80,-80)
    mesh=Mesh(0xc000,1,indices,None,None,None,c.addressof(uv))
    class Span(c.Structure):
        _fields_=[('indices',S),('uvs',S)]
    span=Span()
    mat=Material(0xffffffff,0,1,0,0x02000000)
    points=(Vec*5)()
    model=Model(points,None,5,c.pointer(mesh),c.pointer(mat),1,1,Vec(0,0,2),16)
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
    v0,v1,v2,v3=(256,240,.5),(384,240,.5),(320,176,.5),(384,176,.5)
    clipped=[(320,240,1),(256,176,1),v1,v2]
    cases={}
    fixtures=[
        ('quad',0x4000,[0,1,2,3],False,False,[[v0,v1,v2,v3]],4),
        ('pentagon',0x8000,[5,0,1,2,3,4],False,True,[[v0,v1,v2,v3,(320,144,.5)]],5),
        ('quad_clipped',0x4000,[0,2,1,3],True,False,[clipped,[v2,v1,v3]],4),
        ('ngon_clipped',0x8000,[4,0,2,1,3],True,True,[clipped,[v2,v1,v3]],4),
        ('short_quad',0x4000,[0,1,2],False,False,[],0),
        ('short_ngon',0x8000,[5,0,1,2,3],False,False,[],0),
        ('two_corners',0x8000,[2,0,1],False,False,[],0),
        ('short_uv',0x4000,[0,1,2,3],False,False,[],0),
    ]
    for name,kind,words,near,alpha,runs,calc_count in fixtures:
        assert reset()==0
        calc.value=drawn.value=0
        points[:]=[Vec(-.5,0,0 if near else 2),Vec(.5,0,2),Vec(0,.5,2),Vec(.5,.5,2),Vec(0,.75,2)]
        mat.flags=0x02200000|(0x100000 if alpha else 0)
        indices[:len(words)]=words
        span.indices=len(words)
        span.uvs=3 if name=="short_uv" else 5
        mesh.kind=kind
        result=I(123)
        rc=draw(c.byref(model),5,0,1,c.byref(span),1,c.byref(result))
        assert (rc,result.value,calc.value,drawn.value)==(
            0 if calc_count else 100,0 if calc_count else 123,calc_count,sum(map(len,runs))),(
            name,rc,result.value,calc.value,drawn.value)
        assert vertex.value==c.addressof(scratch) and count.value==4 and bytes(scratch)==b'\x75'*64
        lane=int(alpha)
        assert used(1-lane)==0
        raw=c.string_at(data(lane),used(lane)) if used(lane) else b''
        header=struct.pack('<4I4f',0x82000028 if alpha else 0x80000028,0x20800000,
                           (0x00900000 if alpha else 0x20880000)|(tsp&63),tcw,.5,.75,.25,1)
        original_uv=[(-129,1),(0,-256),(256,128),(64,32),(80,-80)]
        clipped_uv=[[(63,64),(-64,-127),original_uv[2],original_uv[1]],original_uv[1:4]]
        coords=clipped_uv if near else [original_uv]
        expected=header
        for run,uvs in zip(runs,coords):
            for j,(position,(u,v)) in enumerate(zip(run,uvs)):
                expected+=struct.pack('<I7f',0xf0000000 if j==len(run)-1 else 0xe0000000,
                                      *position,u/256,v/256,1,0)
        assert raw==(expected if calc_count else b''),(name,raw.hex(),expected.hex())
        cases[name]=dict(calculated=calc_count,draw_counter=drawn.value,packet_hex=raw.hex())
    with a.output.open('x') as out:
        json.dump(dict(cases=cases,dll_sha256=b['frames_DLL_sha256'],original_SH4_execution_compared=False,
                       actual_GPU_present=False),out,indent=2)
        out.write('\n')
    print(len(cases),'textured quad and N-gon packet cases passed',flush=True)


if __name__=='__main__':
    main()
