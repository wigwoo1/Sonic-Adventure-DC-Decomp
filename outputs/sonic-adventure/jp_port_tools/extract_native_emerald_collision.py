"""Decode static solid Emerald Coast geometry from the supplied Japanese disc."""
import argparse
import hashlib
import json
import math
import re
import struct
import sys
from pathlib import Path

from build_native_stage_start import pinned
from verify_native_read_queue import Disc

LOAD = 0x0C900000
ENTRY = struct.Struct('<6fIII')
TRIANGLE = struct.Struct('<II9f')


def decode_act(image, address):
    def offset(pointer, size):
        start = pointer-LOAD
        assert 0<=start<=len(image)-size,(hex(pointer),size)
        return start

    count, motions, flags, users, _, entries = struct.unpack_from(
        '<hhhhfI',image,offset(address,36))
    assert count>0 and motions==users==0 and not (flags&0x10)
    table = offset(entries,count*ENTRY.size)
    triangles = bytearray()
    objects = 0
    for index in range(count):
        *_, pointer, blocks, flags = ENTRY.unpack_from(image,table+index*ENTRY.size)
        if not flags&1 or flags&0x400002:
            continue
        obj = struct.unpack_from('<II3f3i3fII',image,offset(pointer,52))
        _, model, tx, ty, tz, rx, ry, rz, sx, sy, sz, child, sibling = obj
        assert child==sibling==0,(index,obj)
        points, _, point_count, meshes, _, mesh_count = struct.unpack_from(
            '<5IH',image,offset(model,40))
        point_start = offset(points,point_count*12)
        mesh_start = offset(meshes,mesh_count*24)
        angles = [(value&0xffff)*math.tau/65536 for value in (rx,ry,rz)]
        (sin_x,sin_y,sin_z) = map(math.sin,angles)
        (cos_x,cos_y,cos_z) = map(math.cos,angles)
        vertices = []
        for x,y,z in struct.iter_unpack('<3f',image[point_start:point_start+point_count*12]):
            x,y,z = x*sx,y*sy,z*sz
            y,z = cos_x*y-sin_x*z,sin_x*y+cos_x*z
            x,z = cos_y*x+sin_y*z,cos_y*z-sin_y*x
            x,y = cos_z*x-sin_z*y,sin_z*x+cos_z*y
            vertices.append((tx+x,ty+y,tz+z))
        for mesh_index in range(mesh_count):
            kind, strip_count, indices = struct.unpack_from('<HHI',image,mesh_start+24*mesh_index)
            kind >>= 14
            for _ in range(strip_count):
                reversed_first = 0
                if kind>=2:
                    header, = struct.unpack_from('<H',image,offset(indices,2))
                    reversed_first, corners = header>>15, header&0x7fff
                    indices += 2
                else:
                    corners = kind+3
                assert 3<=corners<=1024
                strip = struct.unpack_from(f'<{corners}H',image,offset(indices,corners*2))
                assert all(i<point_count for i in strip)
                indices += corners*2
                for i in range(corners-2):
                    a,b,c = (vertices[j] for j in strip[i:i+3])
                    if (reversed_first+i)&1:
                        b,c = c,b
                    triangles.extend(TRIANGLE.pack(index,flags,*a,*b,*c))
        objects += 1
    return bytes(triangles), objects


def surfaces(triangles,x,z):
    hits = []
    for index,flags,ax,ay,az,bx,by,bz,cx,cy,cz in TRIANGLE.iter_unpack(triangles):
        ux,uz,vx,vz = bx-ax,bz-az,cx-ax,cz-az
        det = ux*vz-uz*vx
        if abs(det)<1e-8:
            continue
        u = ((x-ax)*vz-(z-az)*vx)/det
        v = (ux*(z-az)-uz*(x-ax))/det
        if u>=-1e-7 and v>=-1e-7 and u+v<=1+1e-7:
            hits.append((ay+u*(by-ay)+v*(cy-ay),index,flags,det<0))
    return sorted(hits)


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace','upstream','track','output'):
        parser.add_argument('--'+name,type=Path,required=True)
    args = parser.parse_args()
    out = args.output.resolve()
    assert out.is_relative_to(args.workspace.resolve()/'work/sonic-adventure')
    sys.path.insert(0,str(args.workspace.resolve()/'outputs/sonic-adventure'))
    from decompress_prs import decompress

    symbols = pinned(args.upstream,'config/stg01/symbols.txt')
    addresses = dict((int(act),int(address,16)) for address,act in
                     re.findall(rb'^(0x[0-9a-f]+)\s+beach_act([012])_land\s*$',symbols,re.M))
    assert set(addresses)=={0,1,2}
    disc = Disc(args.track)
    try:
        fad,size,directory = disc.files()['STG01.PRS']
        assert not directory
        compressed = disc.sectors(fad,(size+2047)//2048)[:size]
    finally:
        disc.f.close()
    image,_ = decompress(compressed)
    out.mkdir()
    sha = lambda payload: hashlib.sha256(payload).hexdigest()
    report = dict(stage='Emerald Coast',module='STG01.PRS',module_sha256=sha(compressed),
                  decoded_module_sha256=sha(image),acts=[],playable=False,overall_completion_percent=10)
    for act,spawn in enumerate(((-9,16,4),(-986.5,1145,-2856),(5815,5,1136))):
        triangles,objects = decode_act(image,addresses[act])
        hits = surfaces(triangles,spawn[0],spawn[2])
        floors = [hit for hit in hits if hit[0]<=spawn[1] and hit[-1]]
        assert floors,(act,spawn,hits)
        floor = floors[-1]
        assert 0<=spawn[1]-floor[0]<64,(act,floor)
        (out/f'act{act}_triangles.bin').write_bytes(triangles)
        report['acts'].append(dict(act=act,solid_objects=objects,triangles=len(triangles)//TRIANGLE.size,
            triangle_sha256=sha(triangles),spawn_floor_y=floor[0],spawn_floor_entry=floor[1]))
    (out/'verification.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print('Decoded original Emerald Coast collision geometry and located floor under all three Sonic starts')


if __name__ == '__main__':
    main()
