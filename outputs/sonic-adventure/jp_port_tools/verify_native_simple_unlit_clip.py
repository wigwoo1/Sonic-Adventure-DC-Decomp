"""Check original near-plane topology and complete native queue packets."""
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
    assert api('jp_queue_initialize',I,S)(1<<20)==0
    assert api('jp_frames_initialize',I,U,U,S)(4,16,1<<20)==0
    assert api('jp_queue_set_context',I,U,P)(0,(U*4)(0x8000000a,0x20800000,0x20800000,0))==0
    assert api('jp_simple_calc_initialize',I,U,S)(8,320)==0
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
    scratch=c.create_string_buffer(b'\x75'*48,48)
    vertex=P.in_dll(f,'_nj_vertex_buf_')
    count=I.in_dll(f,'_nj_vertex_num_')
    vertex.value,count.value=c.addressof(scratch),3
    indices=(c.c_int16*3)(0,2,1)
    mesh=Mesh(0,1,indices)
    mat=Material(0xffffffff,0,1,0,0x02000000)
    points=(Vec*3)()
    model=Model(points,None,3,c.pointer(mesh),c.pointer(mat),1,1,Vec(0,0,2),16)
    light=Vec(.5,-1,.75)
    reset=api('jp_queue_reset_frame',I)
    used=api('jp_queue_committed_bytes',S,U)
    data=api('jp_queue_data',P,U)
    cases={}
    for name,z0,z2,flip,alpha,expected,culled in [
        ('one_near',0,2,False,False,[(320,240,1),(256,176,1),(384,240,.5),(320,176,.5)],False),
        ('two_near_alpha',0,0,False,True,[(320,240,1),(384,176,1),(384,240,.5)],False),
        ('near_backface',0,2,True,False,[],False),
        ('all_near',0,0,False,False,[],True),
        ('unclipped',2,2,True,False,[(256,240,.5),(384,240,.5),(320,176,.5)],False),
        ('partial_side',2,2,True,False,[(-192,240,.5),(384,240,.5),(320,176,.5)],False),
    ]:
        assert reset()==0
        calc.value=drawn.value=0
        points[:]=[Vec(-4 if name=='partial_side' else -.5,0,z0),Vec(.5,0,0 if culled else 2),Vec(0,.5,z2)]
        mat.flags=0x02000000|(0x100000 if alpha else 0)
        indices[:]=[0,1,2] if flip else [0,2,1]
        result=I(123)
        assert draw(c.byref(model),3,0,3,1,c.byref(light),.8,c.byref(result))==0,name
        assert (result.value,calc.value,drawn.value)==(-1 if culled else 0,0 if culled else 3,len(expected)),name
        assert vertex.value==c.addressof(scratch) and count.value==3 and bytes(scratch)==b'\x75'*48
        lane=int(alpha)
        assert used(1-lane)==0
        raw=c.string_at(data(lane),used(lane)) if used(lane) else b''
        header=struct.pack('<8I',0x82000010 if alpha else 0x80000010,0x20800000,
                           0x00900000 if alpha else 0x20880000,0,0,0,0,0)
        packet=header+b''.join(struct.pack('<I7f',0xf0000000 if j==len(expected)-1 else 0xe0000000,
                               *v,.5,.75,.25,1) for j,v in enumerate(expected))
        assert raw==(b'' if culled else packet),(name,raw.hex(),packet.hex())
        cases[name]=dict(vertices=len(expected),model_culled=culled,packet_hex=raw.hex())
    with a.output.open('x') as out:
        json.dump(dict(cases=cases,dll_sha256=b['frames_DLL_sha256'],original_SH4_execution_compared=False,
                       actual_GPU_present=False),out,indent=2)
        out.write('\n')
    print(len(cases),'clipped and unclipped packet cases passed',flush=True)


if __name__=='__main__':
    main()
