"""Check environment strip FTRV, clipping, and shared matrix preservation."""
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
    assert api('jp_frames_initialize',I,U,U,S)(16,16,1<<20)==0
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
    matrix=(F*16)(.5,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1)
    api('njSetMatrix',None,P,P)(None,matrix)
    get_matrix=api('njGetMatrix',None,P)
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
    indices=(c.c_int16*4)(3,0,1,2)
    mesh=Mesh(0xc000,1,indices)
    mat=Material(0xffffffff,0,1,0xc0000000,0x02200000)
    points=(Vec*3)(Vec(-.5,0,2),Vec(.5,0,2),Vec(0,.5,2))
    normals=(Vec*3)(Vec(0,1,0),Vec(0,-1,0),Vec(1,0,0))
    model=Model(points,normals,3,c.pointer(mesh),c.pointer(mat),1,1,Vec(0,0,2),16)
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
    class Span(c.Structure):
        _fields_=[('indices',S),('uvs',S)]
    span=Span(4,0)
    cases={}
    frames=[]
    fixtures=[
        ('near',1,0,0x8003,[(288,176,1,160,192,.25),(320,240,1,128,128,.5),
                            (320,176,.5,192,128,.5),(352,240,.5,128,0,1)],7),
        ('near_backface',0,0,3,[],3),
        ('unclipped',0,2,3,[(288,240,.5,128,256,0),(352,240,.5,128,0,1),
                            (320,176,.5,192,128,.5)],4),
    ]
    for mode,fixture in ((mode,case) for mode in ('unlit','smooth','flat') for case in fixtures):
        name,alpha,z0,header_word,expected,draw_count=fixture
        calc.value=drawn.value=0
        points[:]=[Vec(-.5,0,z0),Vec(.5,0,2),Vec(0,.5,2)]
        indices[0]=header_word
        mat.flags=0x00600000|(0x100000 if alpha else 0)|{"unlit":0x02000000,"smooth":0,"flat":0x01000000}[mode]
        result=I(123)
        rc=draw(c.byref(model),3,3,1,c.byref(span),1,c.byref(result))
        assert rc==0,(name,rc)
        assert used(1-alpha)==0
        raw=c.string_at(data(alpha),used(alpha))
        assert (result.value,calc.value,drawn.value)==(0,3,draw_count),(mode,name,drawn.value)
        observed_matrix=(F*16)()
        get_matrix(observed_matrix)
        assert bytes(observed_matrix)==bytes(matrix),(mode,name)
        mode_bits=(0x82000028 if alpha else 0x80000028)|(2 if mode=='smooth' else 0)
        header=struct.pack('<4I4f',mode_bits,0x20800000,
                           (0x00900000 if alpha else 0x20880000)|(tsp&63),tcw,.5,.75,.25,1)
        assert raw[:32]==header and len(raw)==32+32*len(expected),name
        for j,(x,y,q,u,v,intensity) in enumerate(expected):
            start=32+32*j
            command=0xf0000000 if j==len(expected)-1 else 0xe0000000
            assert raw[start:start+24]==struct.pack('<I5f',command,x,y,q,u/256,v/256),(mode,name,j,raw.hex())
            if mode!='flat' or z0 or j>=2:
                level=1 if mode=='unlit' else .5 if mode=='flat' and not z0 else intensity
                assert raw[start+24:start+28]==struct.pack('<f',level),(mode,name,j)
        frame=Q()
        assert submit(c.byref(frame))==0 and frame.value
        frames.append(frame.value)
        size=S()
        ptr=frame_list(frame.value,alpha,c.byref(size))
        assert ptr and c.string_at(ptr,size.value)==raw
        cases[mode+"_"+name]=dict(status=rc,vertices=len(expected),packet_hex=raw.hex())
    t.clear()
    t.release_list(c.byref(listing))
    assert t.status()==0
    for frame in frames:
        size=S()
        ptr=surface(frame,tcw&0x1fffff,c.byref(size))
        assert ptr and c.string_at(ptr,size.value)==payload
        assert release(frame)==0
    with a.output.open('x') as out:
        json.dump(dict(cases=cases,dll_sha256=b['frames_DLL_sha256'],texture_retained_after_release=True,
                       shared_matrix_unchanged=True,actual_GPU_present=False,original_SH4_execution_compared=False),out,indent=2)
        out.write('\n')
    print(len(cases),'environment strip packet cases passed with retained textures',flush=True)


if __name__=='__main__':
    main()
