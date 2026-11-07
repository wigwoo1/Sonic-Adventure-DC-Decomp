"""Verify original unlit strip winding, clipping, and complete queue bytes."""
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
    draw=api('jp_simple_draw_unlit_strips',I,P,S,S,S,S,P,F,P)
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
    scratch=c.create_string_buffer(b'\x75'*64,64)
    vertex=P.in_dll(f,'_nj_vertex_buf_')
    count=I.in_dll(f,'_nj_vertex_num_')
    vertex.value,count.value=c.addressof(scratch),4
    indices=(c.c_int16*8)()
    mesh=Mesh(0xc000,1,indices)
    mat=Material(0xffffffff,0,1,0,0x02000000)
    points=(Vec*4)()
    model=Model(points,None,4,c.pointer(mesh),c.pointer(mat),1,1,Vec(0,0,2),16)
    light=Vec(.5,-1,.75)
    reset=api('jp_queue_reset_frame',I)
    used=api('jp_queue_committed_bytes',S,U)
    data=api('jp_queue_data',P,U)
    v0,v1,v2,v3=(256,240,.5),(384,240,.5),(256,176,.5),(384,176,.5)
    cases={}
    for name,words,nstrips,near,alpha,runs,calc_count,draw_count in [
        ('continuous', [4,0,1,2,3],1,False,False,[[v0,v1,v2,v3]],4,5),
        ('flipped_backface', [0x8004,0,1,2,3],1,False,False,[],4,0),
        ('split', [3,0,1,2,0x8003,2,3,1],2,False,False,[[v0,v1,v2],[v2,v3,v1]],6,8),
        ('near', [0x8003,0,1,2],1,True,True,[[(256,176,1),(320,240,1),(320,176,.5),v1]],3,7),
        ('near_backface', [3,0,1,2],1,True,False,[],3,3),
        ('short_strip', [2,0,1],1,False,False,[],0,0),
        ('short_span', [4,0,1,2],1,False,False,[],0,0),
        ('bad_index', [3,0,1,65535],1,False,False,[],0,0),
    ]:
        assert reset()==0
        calc.value=drawn.value=0
        points[:]=[Vec(-.5,0,0 if near else 2),Vec(.5,0,2),Vec(0 if near else -.5,.5,2),Vec(.5,.5,2)]
        mat.flags=0x02000000|(0x100000 if alpha else 0)
        indices[:len(words)]=words
        mesh.count=nstrips
        result=I(123)
        rc=draw(c.byref(model),4,0,len(words),1,c.byref(light),.8,c.byref(result))
        assert (rc,result.value,calc.value,drawn.value)==(
            0 if calc_count else 100,0 if calc_count else 123,calc_count,draw_count),(
            name,rc,result.value,calc.value,drawn.value)
        assert vertex.value==c.addressof(scratch) and count.value==4 and bytes(scratch)==b'\x75'*64
        lane=int(alpha)
        assert used(1-lane)==0
        raw=c.string_at(data(lane),used(lane)) if used(lane) else b''
        header=struct.pack('<8I',0x82000010 if alpha else 0x80000010,0x20800000,
                           0x00900000 if alpha else 0x20880000,0,0,0,0,0)
        expected=header+b''.join(struct.pack('<I7f',0xf0000000 if j==len(run)-1 else 0xe0000000,
                                 *v,.5,.75,.25,1) for run in runs for j,v in enumerate(run))
        assert raw==(expected if calc_count else b''),(name,raw.hex(),expected.hex())
        cases[name]=dict(calculated=calc_count,draw_counter=draw_count,packet_hex=raw.hex())
    with a.output.open('x') as out:
        json.dump(dict(cases=cases,dll_sha256=b['frames_DLL_sha256'],original_SH4_execution_compared=False,
                       actual_GPU_present=False),out,indent=2)
        out.write('\n')
    print(len(cases),'unlit strip packet cases passed',flush=True)


if __name__=='__main__':
    main()
