"""Verify original Emerald Coast materials against the native texture owner."""
import argparse
import ctypes as C
import hashlib
import json
import struct
import sys
from pathlib import Path

from build_native_stage_start import pinned
from extract_native_emerald_collision import ENTRY, LOAD
from native_packet_test_support import bind
from native_pvm_test_support import PVM, destination, index_archive
from native_texture_test_support import Record, Runtime
from native_video_test_support import Video
from verify_native_read_queue import Disc


def stage_materials(image, table_address):
    def offset(address, size):
        value = address-LOAD
        assert 0<=value<=len(image)-size,(hex(address),size)
        return value

    count, _, _, _, _, entries, _, _, texlist = struct.unpack_from(
        '<hhhhf4I',image,offset(table_address,36))
    texture_names, texture_count = struct.unpack_from('<II',image,offset(texlist,8))
    names = []
    for i in range(texture_count):
        name, _, _ = struct.unpack_from('<III',image,offset(texture_names+i*12,12))
        start = offset(name,1)
        end = image.index(0,start,start+29)
        names.append(image[start:end])
    used = set()
    meshes = corners = objects = 0
    for entry in range(count):
        *_, obj, _, flags = ENTRY.unpack_from(image,offset(entries+entry*ENTRY.size,ENTRY.size))
        if not flags&0x80000000:
            continue
        _, model, *_, child, sibling = struct.unpack_from('<II3f3i3fII',image,offset(obj,52))
        assert not child and not sibling
        _, _, points, meshsets, materials, mesh_count, material_count = struct.unpack_from(
            '<5I2H',image,offset(model,40))
        for i in range(mesh_count):
            kind, strips, indices, _, _, _, uv = struct.unpack_from(
                '<HH5I',image,offset(meshsets+i*24,24))
            material_id = kind&0x3fff
            assert material_id<material_count
            _, _, _, slot, material_flags = struct.unpack_from(
                '<IIfII',image,offset(materials+material_id*20,20))
            if not material_flags&0x200000:
                continue
            assert slot<texture_count and uv
            used.add(slot)
            mesh_corners = 0
            for _ in range(strips):
                mesh_kind = kind>>14
                if mesh_kind>=2:
                    n, = struct.unpack_from('<H',image,offset(indices,2))
                    n &= 0x7fff
                    indices += 2
                else:
                    n = mesh_kind+3
                assert n>=3
                assert all(j<points for j in struct.unpack_from(f'<{n}H',image,offset(indices,n*2)))
                mesh_corners += n
                indices += n*2
            offset(uv,mesh_corners*4)
            corners += mesh_corners
            meshes += 1
        objects += 1
    return names, used, dict(drawable_objects=objects,textured_meshsets=meshes,uv_corners=corners)


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace','upstream','baseline','track','output'):
        parser.add_argument('--'+name,type=Path,required=True)
    args = parser.parse_args()
    root, out = args.workspace.resolve(), args.output.resolve()
    assert out.is_relative_to(root/'work/sonic-adventure')
    sys.path.insert(0,str(root/'outputs/sonic-adventure'))
    from decompress_prs import decompress

    manifest_path = args.baseline/'build_verification.json'
    baseline = json.loads(manifest_path.read_bytes())
    dll = args.baseline.resolve()/Path(baseline['frames_DLL']).name
    sha = lambda raw: hashlib.sha256(raw).hexdigest()
    assert sha(dll.read_bytes())==baseline['frames_DLL_sha256']
    symbols = pinned(args.upstream,'config/stg01/symbols.txt')
    table, = [int(line.split()[0],16) for line in symbols.splitlines()
              if line.split()[-1:]==[b'beach_act0_land']]
    disc = Disc(args.track)
    try:
        files = disc.files()
        def read(name):
            fad,size,directory = files[name]
            assert not directory
            return disc.sectors(fad,(size+2047)//2048)[:size]
        module, compressed = read('STG01.PRS'), read('BEACH01.PRS')
    finally:
        disc.f.close()
    assert sha(module)=='dc78bfb0256cebeb093d45b24fb183390720e7c6984f8bc4f56570a62f4fbbff'
    image,_ = decompress(module)
    archive,_ = decompress(compressed)
    slot_names, used, mesh_stats = stage_materials(image,table)
    flags, rows, pictures = index_archive(archive)
    assert len(slot_names)==len(rows)==len(pictures)
    assert sorted(row['number'] for row in rows)==list(range(len(rows)))
    by_slot = {row['number']:(row,pic) for row,pic in zip(rows,pictures)}
    for i, name in enumerate(slot_names):
        assert by_slot[i][0]['name'].split(b'\0',1)[0].lower()==name.lower(),(i,name,by_slot[i][0]['name'])

    runtime = Runtime(dll)
    lib = runtime.dll
    pvm = PVM(lib)
    assert runtime.init(256,1024,32<<20)==0
    video = Video(lib)
    assert bind(lib,'jp_queue_initialize',C.c_int,C.c_size_t)(8<<20)==0
    assert bind(lib,'jp_frames_initialize',C.c_int,C.c_uint32,C.c_uint32,C.c_size_t)(8,128,32<<20)==0
    assert video.init(64,64,0,1)==0
    names, listing, buffers = destination(len(rows))
    src = C.create_string_buffer(archive)
    loaded = pvm.load(C.byref(listing),len(rows),28,src,len(archive))
    assert loaded==1,(loaded,pvm.status(),runtime.status(),pvm.loaded(C.byref(listing)))
    assert pvm.loaded(C.byref(listing))==len(rows)
    stored_bytes = 0
    for i in range(len(rows)):
        row,pic = by_slot[i]
        pointer = runtime.lookup(C.byref(listing),i)
        assert pointer,i
        record = Record.from_address(pointer)
        surface = record.texinfo.texsurface
        assert record.globalIndex==row['gid'] and (surface.nWidth,surface.nHeight)==(pic['w'],pic['h'])
        assert buffers[i].raw[16:44]==row['name']
        word = runtime.surface_word(C.byref(surface))
        size = runtime.surface_bytes(word)
        prefix = 4 if pic['kind']&0xff00==0x200 else 0
        assert prefix<size<=len(pic['pixels'])+prefix
        expected = bytes(prefix)+pic['pixels'][:size-prefix]
        assert C.string_at(runtime.surface_data(word),size)==expected,i
        stored_bytes += size-prefix
    assert pvm.release(C.byref(listing))==1 and runtime.live()==0
    assert pvm.shutdown()==0
    runtime.shutdown()
    video.shutdown()
    assert bind(lib,'jp_frames_shutdown',C.c_int)()==0
    bind(lib,'jp_queue_shutdown',None)()
    out.mkdir()
    report = dict(status='verified_original_Emerald_Coast_material_slots_and_native_surfaces',
                  module_sha256=sha(module),prs_sha256=sha(compressed),pvm_sha256=sha(archive),
                  texture_slots=len(rows),referenced_slots=len(used),encoded_bytes_compared=stored_bytes,
                  header_flags=flags,build_verification_sha256=sha(manifest_path.read_bytes()),
                  **mesh_stats,playable=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(f'Native texture owner verified all {len(rows)} Emerald Coast texture slots')


if __name__ == '__main__':
    main()
