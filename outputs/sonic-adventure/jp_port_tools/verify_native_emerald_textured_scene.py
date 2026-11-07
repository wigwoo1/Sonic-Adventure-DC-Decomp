"""Render original Emerald Coast texture coordinates and surfaces with Sonic."""
import ctypes as C
import hashlib
import inspect
import math
import struct
import sys
from pathlib import Path

import verify_native_emerald_scene as scene
from extract_native_emerald_collision import ENTRY, LOAD, Disc
from native_packet_test_support import bind
from native_pvm_test_support import PVM, destination, index_archive
from native_texture_test_support import Record, Runtime
from verify_native_model_preflight import Material, Mesh, Model, Span, Vec


def draw_textured_land(context, track):
    sys.path.insert(0,str(context['r']/'outputs/sonic-adventure'))
    from decompress_prs import decompress
    disc = Disc(track)
    try:
        files = disc.files()
        def read(name):
            fad,size,directory = files[name]
            assert not directory
            return disc.sectors(fad,(size+2047)//2048)[:size]
        module, compressed = read('STG01.PRS'),read('BEACH01.PRS')
    finally:
        disc.f.close()
    assert hashlib.sha256(module).hexdigest()=='dc78bfb0256cebeb093d45b24fb183390720e7c6984f8bc4f56570a62f4fbbff'
    assert hashlib.sha256(compressed).hexdigest()=='4ff783da89c18313a04d4264ff7551d3aec64d19ed561b0e89bc119b90a07dd1'
    image,_ = decompress(module)
    archive,_ = decompress(compressed)
    _,rows,pictures = index_archive(archive)
    by_slot = {r['number']:p for r,p in zip(rows,pictures)}
    runtime = Runtime(context['b']['frames_DLL'])
    lib = context['f']
    pvm = PVM(lib)
    names, listing, buffers = destination(len(rows))
    source = C.create_string_buffer(archive)
    assert pvm.load(C.byref(listing),len(rows),28,source,len(archive))==1
    assert runtime.select(C.byref(listing))==1
    draw = bind(lib,'jp_simple_draw_model_bounded',C.c_int,C.c_void_p,C.c_size_t,
                C.c_size_t,C.c_size_t,C.c_void_p,C.c_size_t,C.c_void_p)
    used = bind(lib,'jp_queue_committed_bytes',C.c_size_t,C.c_uint32)
    prior_bytes = used(0)
    emitted_slots, emitted_entries, deferred_slots, environment_slots = set(), set(), set(), set()
    source_triangles = 0

    def read_at(layout,address):
        start = address-LOAD
        assert 0<=start<=len(image)-struct.calcsize(layout)
        return struct.unpack_from(layout,image,start)

    for entry in (42,91,92,93,126,388,393):
        *_, pointer, _, flags = read_at(ENTRY.format,0x0c97da70+entry*ENTRY.size)
        assert flags&0x80000000
        _,model,tx,ty,tz,rx,ry,rz,sx,sy,sz,child,sibling = read_at('<II3f3i3fII',pointer)
        assert not (rx or rz or child or sibling) and (sx,sy,sz)==(1,1,1)
        points,_,n,meshes,materials,mesh_count,material_count = read_at('<5I2H',model)
        angle = (ry&0xffff)*math.tau/65536
        cosine,sine = math.cos(angle),math.sin(angle)
        positions = []
        for i in range(n):
            x,y,z = read_at('<3f',points+12*i)
            positions.append(Vec(tx+cosine*x+sine*z+9,ty+y-4-scene.DISPLAY_DROP,tz-sine*x+cosine*z-4))
        for i in range(mesh_count):
            kind, strips, indices, _, _, _, uv = read_at('<HH5I',meshes+24*i)
            mat = kind&0x3fff
            assert mat<material_count
            diffuse,specular,exponent,slot,attr = read_at('<IIfII',materials+20*mat)
            assert attr&0x200000 and uv
            if attr&0x400000:
                environment_slots.add(slot)
                continue
            if by_slot[slot]['kind']&0xff00==0x400:
                deferred_slots.add(slot)
                continue
            order, coords = [], []
            for _ in range(strips):
                mode = kind>>14
                flipped = 0
                if mode>=2:
                    header, = read_at('<H',indices)
                    flipped,count = header>>15,header&0x7fff
                    indices += 2
                else:
                    count = mode+3
                assert 3<=count<=1024
                strip = read_at(f'<{count}H',indices)
                tex = read_at(f'<{count*2}h',uv)
                indices += count*2
                uv += count*4
                for j in range(count-2):
                    corners = (j,j+2,j+1) if (flipped+j)&1 else (j,j+1,j+2)
                    order.extend(strip[k] for k in corners)
                    coords.extend(tex[2*k+l] for k in corners for l in (0,1))
            assert order and all(j<n for j in order)
            indices = (C.c_int16*len(order))(*order)
            uv_array = (C.c_int16*len(coords))(*coords)
            mesh = Mesh(0,len(order)//3,indices,None,None,None,C.addressof(uv_array))
            material = Material(diffuse,specular,exponent,slot,attr|0x02000000)
            vertices = (Vec*n)(*positions)
            native_model = Model(vertices,None,n,C.pointer(mesh),C.pointer(material),1,1,Vec(0,0,0),1024)
            span, projection = Span(len(order),len(order)), C.c_int32()
            before = used(0)+used(1)
            status = draw(C.byref(native_model),n,0,1,C.byref(span),1,C.byref(projection))
            assert status==0,(entry,i,slot,status,projection.value)
            if used(0)+used(1)>before:
                emitted_slots.add(slot)
                emitted_entries.add(entry)
            source_triangles += len(order)//3
    assert used(0)>prior_bytes and emitted_slots
    texture_words = []
    for slot in sorted(emitted_slots):
        record = Record.from_address(runtime.lookup(C.byref(listing),slot))
        texture_words.append(record.texparambuffer&0x1fffff)
    report = dict(original_textured_land_entries=sorted(emitted_entries),texture_slots=sorted(emitted_slots),
                  deferred_vq_mip_slots=sorted(deferred_slots),source_triangles=source_triangles,
                  deferred_environment_slots=sorted(environment_slots),
                  opaque_bytes=used(0)-prior_bytes,diagnostic_land_display_drop=scene.DISPLAY_DROP,
                  texture_words=texture_words)

    def release():
        assert pvm.release(C.byref(listing))==1
        # Keep the texture names and copied metadata alive through retirement.
        assert len(names)==len(buffers)==len(rows)

    return report, release


def textured_caller(raw):
    raw = scene.scene_caller(raw)
    start = '            from verify_native_emerald_scene import draw_land'
    stop = "            check(land['opaque_bytes']>0,'original drawable land submitted with Sonic')"
    assert raw.count(start)==raw.count(stop)==1
    a,b = raw.index(start),raw.index(stop)+len(stop)
    raw = raw[:a]+'''            from verify_native_emerald_textured_scene import draw_textured_land
            land, release_land = draw_textured_land(c,track)
            check(land['opaque_bytes']>0,'original textured land submitted with Sonic')'''+raw[b:]
    start = '    from verify_native_emerald_scene import LAND_COLORS'
    stop = "    (d/'image_observation.json').write_text"
    assert raw.count(start)==raw.count(stop)==1
    raw = raw[:raw.index(start)]+raw[raw.index(stop):]
    anchor = ';decoder.destroy(plan)'
    assert raw.count(anchor)==1
    raw = raw.replace(anchor,'''
    land_words = set(land['texture_words'])
    land_batches = [b for b in decoded.batches[:decoded.batch_count]
                    if b.triangle_count and b.header[3]&0x1fffff in land_words]
    check(bool(land_batches),'nonempty retained batches reference original stage texture surfaces')
    land['nonempty_textured_stage_batches']=len(land_batches)
    decoder.destroy(plan)''')
    anchor = "    check(jobs.release(handle,off_job.value"
    assert raw.count(anchor)==1
    raw = raw.replace(anchor,'''    edge_changes = sum(rgba[i:i+3]!=off_rgba[i:i+3] for i in range(0,len(rgba),4)
                       if i//4%512<120 or i//4%512>390)
    check(edge_changes>512,'stage pixels change when texture sampling is disabled')
    land['outer_frame_texture_pixel_changes']=edge_changes
    (d/'emerald_land.json').write_text(json.dumps(land,indent=2)+'\\n',encoding='utf-8')
'''+anchor)
    anchor = "    check(bind(f,'jp_character_textures_release'"
    assert raw.count(anchor)==1
    raw = raw.replace(anchor,'    release_land()\n'+anchor)
    return raw.replace('sonic_emerald_stage_scene','sonic_emerald_textured_scene').replace(
        'original_Emerald_Coast_land_and_Sonic_rendered_together',
        'original_Emerald_Coast_textured_land_and_Sonic_rendered_together')


def exercise(context,inputs,track):
    source = inspect.getsource(scene.exercise)
    namespace = dict(vars(scene),__file__=__file__,scene_caller=textured_caller)
    exec(compile(source,scene.__file__,'exec'),namespace)
    namespace['exercise'](context,inputs,track)


if __name__=='__main__':
    # Reuse the established isolated GPU runner and controlled input.
    tail = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n",1)[1]
    runner = 'if True:\n'+tail
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor)==1
    runner = runner.replace(anchor,'from verify_native_emerald_textured_scene import exercise')
    exec(compile(runner,scene.__file__,'exec'),dict(vars(scene)))
