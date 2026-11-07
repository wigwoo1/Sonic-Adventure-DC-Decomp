"""Present original Emerald Coast render geometry with Sonic in one GPU frame."""
import argparse
import ctypes as C
import inspect
import json
import sys
from collections import defaultdict
from pathlib import Path

import extract_native_emerald_collision as geometry
import verify_native_sonic_animation_task as animation
import verify_native_sonic_aura_frames as aura
import verify_native_sonic_emerald_task as emerald
from native_packet_test_support import bind
from verify_native_model_preflight import Material, Mesh, Model, Span, Vec

LAND_COLORS = (0xffc19a65,0xff64816a,0xff8d795f,0xff908970,0xff497e9f,0xff786246,0xff567456)
DISPLAY_DROP = 6


def draw_land(context, track):
    source = inspect.getsource(geometry.decode_act)
    anchor = '        *_, pointer, blocks, flags = ENTRY.unpack_from(image,table+index*ENTRY.size)\n        if not flags&1 or flags&0x400002:'
    assert source.count(anchor)==1
    source = source.replace(anchor,'''        x, y, z, radius, _, _, pointer, blocks, flags = ENTRY.unpack_from(image,table+index*ENTRY.size)
        if not flags&0x80000000 or (x+9)**2+(z-4)**2>(abs(radius)+60)**2:''')
    namespace = dict(vars(geometry))
    exec(compile(source,geometry.__file__,'exec'),namespace)
    sys.path.insert(0,str(context['r']/'outputs/sonic-adventure'))
    from decompress_prs import decompress
    disc = geometry.Disc(track)
    try:
        fad, size, directory = disc.files()['STG01.PRS']
        assert not directory
        compressed = disc.sectors(fad,(size+2047)//2048)[:size]
    finally:
        disc.f.close()
    import hashlib
    assert hashlib.sha256(compressed).hexdigest()=='dc78bfb0256cebeb093d45b24fb183390720e7c6984f8bc4f56570a62f4fbbff'
    image,_ = decompress(compressed)
    triangles, entries = namespace['decode_act'](image,0x0c981904)
    groups = defaultdict(list)
    for entry, flags, *v in geometry.TRIANGLE.iter_unpack(triangles):
        groups[entry].extend((v[i]+9,v[i+1]-4-DISPLAY_DROP,v[i+2]-4) for i in (0,3,6))
    assert len(groups)==entries==7
    f = context['f']
    draw = bind(f,'jp_simple_draw_model_bounded',C.c_int,C.c_void_p,C.c_size_t,
                C.c_size_t,C.c_size_t,C.c_void_p,C.c_size_t,C.c_void_p)
    counter = C.c_int32.in_dll(f,'_nj_draw_polygon_')
    before = counter.value
    used = bind(f,'jp_queue_committed_bytes',C.c_size_t,C.c_uint32)
    previous = used(0)
    for color, (entry, vertices) in zip(LAND_COLORS,groups.items()):
        assert len(vertices)<2048
        points = (Vec*len(vertices))(*(Vec(*v) for v in vertices))
        indices = (C.c_int16*len(vertices))(*range(len(vertices)))
        mesh = Mesh(0,len(vertices)//3,indices)
        material = Material(color,0,1,0,0x02800000)
        model = Model(points,None,len(vertices),C.pointer(mesh),C.pointer(material),1,1,Vec(0,0,0),1024)
        span, projection = Span(len(vertices),0), C.c_int32()
        status = draw(C.byref(model),len(vertices),0,1,C.byref(span),1,C.byref(projection))
        assert status==0,(entry,status,projection.value)
    assert used(0)>previous and counter.value>before
    result = dict(original_drawable_land_entries=sorted(groups),source_triangles=len(triangles)//geometry.TRIANGLE.size,
                  opaque_bytes=used(0)-previous,rendered_vertex_words=counter.value-before,
                  diagnostic_land_display_drop=DISPLAY_DROP)
    (context['d']/'emerald_land.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
    return result


def scene_caller(raw):
    raw = emerald.use_emerald_floor(raw)
    anchor = "                  'original charged player draw callback succeeds')"
    assert raw.count(anchor)==1
    raw = raw.replace(anchor,anchor+'''
            from verify_native_emerald_scene import draw_land
            land = draw_land(c,track)
            check(land['opaque_bytes']>0,'original drawable land submitted with Sonic')
''')
    anchor = 'decoded.batch_count==uncharged_batches+3*len(models)'
    assert raw.count(anchor)==1
    raw = raw.replace(anchor,'decoded.batch_count>uncharged_batches+3*len(models)')
    anchor = "    (d/'image_observation.json').write_text"
    assert raw.count(anchor)==1
    raw = raw.replace(anchor,'''    from verify_native_emerald_scene import LAND_COLORS
    land_colors = {tuple(c.to_bytes(4,'big')[1:]) for c in LAND_COLORS}
    land_pixels = sum(tuple(rgba[i:i+3]) in land_colors for i in range(0,len(rgba),4))
    check(land_pixels>1000 and foreground-land_pixels>1000,'land colors and player pixels are visible in the same GPU image')
    land['visible_land_pixels']=land_pixels
    (d/'emerald_land.json').write_text(json.dumps(land,indent=2)+'\\n',encoding='utf-8')
'''+anchor)
    return raw.replace('sonic_emerald_floor_walk','sonic_emerald_stage_scene').replace(
        'original_Sonic_movement_on_Emerald_Coast_floor_and_GPU_draw_verified',
        'original_Emerald_Coast_land_and_Sonic_rendered_together')


def exercise(context, inputs, track):
    source = inspect.getsource(animation.exercise)
    namespace = dict(vars(animation),__file__=__file__,use_task_animation=scene_caller)
    exec(compile(source,animation.__file__,'exec'),namespace)
    namespace['exercise'](context,inputs,track)
    result_path = context['d']/'result.json'
    result = aura.read(result_path)
    result['stage_geometry'] = aura.read(context['d']/'emerald_land.json')
    result_path.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')


if __name__ == '__main__':
    import os
    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument('--geometry',type=Path,required=True)
    args, remaining = parser.parse_known_args()
    os.environ['JP_SONIC_EMERALD_TEST_GEOMETRY'] = str(args.geometry.resolve())
    sys.argv[1:] = remaining
    runner = inspect.getsource(aura.main)
    anchor = 'from verify_native_sonic_aura_frames import exercise'
    assert runner.count(anchor)==1
    runner = runner.replace(anchor,'from verify_native_emerald_scene import exercise')
    anchor = 'prefix=raw.split(marker)[0]'
    assert runner.count(anchor)==1
    runner = runner.replace(anchor,anchor+'''
    sample = b'Sample(1,0,0,0,0,0,0,0)'
    assert prefix.count(sample)==1
    prefix = prefix.replace(sample,b'Sample(1,0,-100,-12,0,0,0,0)')
''')
    namespace = dict(vars(aura))
    exec(compile(runner,aura.__file__,'exec'),namespace)
    namespace['main']()
