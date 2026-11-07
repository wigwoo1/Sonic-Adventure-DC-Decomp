"""Link the pinned land visibility routine and check original Emerald Coast entries."""
import argparse
import ctypes as C
import hashlib
import json
import math
import re
import shlex
import shutil
import subprocess
import sys
from pathlib import Path

from build_native_read_queue import PIN, owner_function
from build_native_simple_material import digest
from build_native_stage_start import pinned
from extract_native_emerald_collision import ENTRY, LOAD, Disc
from native_packet_test_support import bind
from rewrite_sprite_float_operations import rewrite


class Entry(C.Structure):
    _fields_ = [('center',C.c_float*3),('radius',C.c_float),
                ('size_y',C.c_float),('size_z',C.c_float),('object',C.c_void_p),
                ('blocks',C.c_int32),('flags',C.c_uint32)]


PROBE = b'''
__declspec(dllexport) int jp_land_visible_indices(
    struct land_entry *entries, int count, int blocks, int flags,
    Float clip, Float far_clip, Float aspect, NJS_POINT3 *pos,
    Angle yaw, Angle pitch, Uint16 *indices, size_t capacity)
{
    if (!entries || count < 0 || count > 32767 || !pos || !indices ||
        !capacity || !(clip > 0) || !(far_clip > 0) || !(aspect > 0))
        return -1;
    struct land_table land = {0};
    struct task_work view = {0};
    land.count = (Sint16)count;
    land.entries = entries;
    land.flags = (Sint16)flags;
    land.clip = clip;
    view.pos = *pos;
    view.ang.y = yaw;
    view.ang.x = pitch;
    struct task_work *saved_camera = camera_work;
    Float saved_far = _nj_far_clip_, saved_aspect = port_draw_aspect;
    world_land = &land;
    world_land_blocks = blocks;
    camera_work = &view;
    _nj_far_clip_ = far_clip;
    port_draw_aspect = aspect;
    world_land_list_visible();
    camera_work = saved_camera;
    _nj_far_clip_ = saved_far;
    port_draw_aspect = saved_aspect;
    world_land = NULL;
    int selected = world_drawn_count;
    if ((size_t)selected > capacity) selected = -2;
    if (selected > 0) {
        /* The original static draw callback traverses this list backwards. */
        for (int i = 0; i < selected; i++)
            indices[i] = (Uint16)(world_drawn[selected-1-i]-entries);
    }
    world_drawn_count = 0;
    return selected;
}
'''


def verify(dll, track, stage_symbols):
    from decompress_prs import decompress
    disc = Disc(track)
    try:
        fad,size,directory = disc.files()['STG01.PRS']
        assert not directory
        packed = disc.sectors(fad,(size+2047)//2048)[:size]
    finally:
        disc.f.close()
    assert hashlib.sha256(packed).hexdigest()=='dc78bfb0256cebeb093d45b24fb183390720e7c6984f8bc4f56570a62f4fbbff'
    image,_ = decompress(packed)
    import struct
    lib = C.CDLL(str(dll))
    I,F,P = C.c_int32,C.c_float,C.c_void_p
    stack = (F*(16*16))()
    bind(lib,'njInitMatrix',None,P,I,I)(stack,16,0)
    screen = (F*5).in_dll(lib,'_nj_screen_')
    screen[:] = (512,640,480,320,240)
    camera = P.in_dll(lib,'camera_work')
    camera.value = 0x12345678
    far_clip = F.in_dll(lib,'_nj_far_clip_')
    far_clip.value = 777
    aspect = F.in_dll(lib,'port_draw_aspect')
    aspect.value = 0.75
    get_matrix = bind(lib,'njGetMatrix',None,P)
    before = (F*16)()
    get_matrix(before)
    probe = bind(lib,'jp_land_visible_indices',I,P,I,I,I,F,F,F,P,I,I,P,C.c_size_t)
    checks = 0
    observed = []
    for act,pos in enumerate(((-9,16,4),(-986.5,1145,-2856),(5815,5,1136))):
        address, = [int(line.split()[0],16) for line in stage_symbols.splitlines()
                    if line.split()[-1:]==[f'beach_act{act}_land'.encode()]]
        count,_,flags,_,clip,entries = struct.unpack_from('<hhhhfI',image,address-LOAD)
        rows = [ENTRY.unpack_from(image,entries-LOAD+i*ENTRY.size) for i in range(count)]
        native = (Entry*count)(*(Entry((F*3)(*row[:3]),*row[3:6],None,row[-2],row[-1])
                                  for row in rows))
        output = (C.c_uint16*1024)()
        pos_native = (F*3)(*pos)
        for block_mask in (0,1,2,-1):
            for yaw,pitch in ((0,0),(0x4000,0),(0x8000,0),(0,0x4000)):
                for view_flags in (flags, flags&~4):
                    for ratio in (1,1.25):
                        far = 2400
                        effective = clip if view_flags&4 else far
                        effective *= 512*math.hypot(512*ratio,320)/(512*ratio*math.hypot(512,320))
                        half = effective/2
                        dy = half if pitch else 0
                        dx,dz = {0:(0,-half),0x4000:(-half,0),0x8000:(0,half)}[yaw]
                        if pitch: dx = dz = 0
                        center = (pos[0]+dx,pos[1]+dy,pos[2]+dz)
                        expected = []
                        for i,row in enumerate(rows):
                            if not row[-1]&0x80000000 or (row[-2] and not row[-2]&block_mask):
                                continue
                            reach = (far if row[-1]&0x20 else half)+row[3]
                            if sum((a-b)**2 for a,b in zip(center,row[:3]))<=reach*reach:
                                expected.append(i)
                        n = probe(native,count,block_mask,view_flags,clip,far,ratio,pos_native,yaw,pitch,output,len(output))
                        assert n==len(expected) and list(output[:n])==expected[::-1],(act,block_mask,yaw,pitch,view_flags,ratio,n,len(expected))
                        checks += 1
                        if block_mask==-1 and yaw==pitch==0 and view_flags==flags and ratio==1:
                            observed.append(dict(act=act,selected=n,land_entries=count))
        assert probe(native,count,-1,flags,clip,2400,1,pos_native,0,0,output,1)==-2
        checks += 1
    after = (F*16)()
    get_matrix(after)
    assert bytes(before)==bytes(after)
    assert camera.value==0x12345678 and far_clip.value==777 and aspect.value==0.75
    return dict(cases=checks,spawn_views=observed,camera_matrix_and_globals_restored=True)


def main():
    parser = argparse.ArgumentParser()
    for name in ('workspace','upstream','baseline','track','output'):
        parser.add_argument('--'+name,type=Path,required=True)
    args = parser.parse_args()
    workspace,base,out = (p.resolve() for p in (args.workspace,args.baseline,args.output))
    assert out.is_relative_to(workspace/'work/sonic-adventure')
    sys.path.insert(0,str(workspace/'outputs/sonic-adventure'))
    parent = base/'build_verification.json'
    manifest = json.loads(parent.read_bytes())
    assert digest(manifest['frames_DLL'])==manifest['frames_DLL_sha256']
    world = pinned(args.upstream,'src/collision/world.c')
    stage_symbols = pinned(args.upstream,'config/stg01/symbols.txt')
    fields = re.findall(rb'^static (?:struct land_(?:table|entry) \*|Sint16 )(?:world_land|world_drawn(?:_count)?|dat_8c764e6c)(?:\[DRAWN\])?;',world,re.M)
    assert len(fields)==4
    limit = re.search(rb'^#define DRAWN \d+',world,re.M).group()
    out.mkdir()
    source = out/'land_visibility.c'
    source.write_bytes(b'''#include <ninja.h>
#include "camera/camera.h"
#include "collision/world.h"
#include "system/task.h"
#include "system/util.h"
#include "jp_native_sprite_numeric.h"
#include "jp_native_primitive_numeric.h"
extern Float port_draw_aspect;
extern NJS_MATRIX _unit_matrix_;
#define njSqrt jp_primitive_sqrt
static Sint32 world_land_blocks;
'''+limit+b'\n'+b'\n'.join(fields)+b'\n#define GCC 1\n'+
        owner_function(world,'world_land_list_visible')+b'\n#undef GCC\n'+PROBE)
    dll,obj = out/'jp_native_frames.dll',out/'land_visibility.obj'
    for dep in base.glob('*.dll'):
        if dep.name!=dll.name:
            shutil.copyfile(dep,out/dep.name)
    recipe = manifest['sonic_emerald_task']['commands'][0]
    driver = manifest['sonic_emerald_task']['commands'][-1][0]
    compiler = recipe[:recipe.index('-c')]+['-Wno-unused-but-set-variable']
    ast = subprocess.run(compiler+['-Xclang','-ast-dump=json','-fsyntax-only',str(source)],capture_output=True)
    (out/'ast.stderr').write_bytes(ast.stderr)
    assert not ast.returncode and not ast.stderr,ast.stderr.decode(errors='replace')
    native,stats = rewrite(source.read_bytes(),json.loads(ast.stdout),['world_land_list_visible'])
    adapted = out/'land_visibility_rz.c'
    adapted.write_bytes(native)
    link = shlex.split((base/'link.rsp').read_text())
    prior = {p:digest(p) for p in link if p.endswith('.obj')}
    link[link.index('-o'):link.index('-o')] = [str(obj)]
    link[-1] = str(dll)
    rsp = out/'link.rsp'
    rsp.write_text(''.join('"'+arg.replace('\\','/')+'"\n' for arg in link),encoding='utf-8')
    commands = [compiler+['-c',str(adapted),'-o',str(obj)],[driver,'@'+str(rsp)]]
    for phase,command in zip(('compile','link'),commands):
        run = subprocess.run(command,capture_output=True)
        (out/(phase+'.stderr')).write_bytes(run.stderr)
        assert not run.returncode and not run.stderr,run.stderr.decode(errors='replace')
    assert all(digest(p)==sha for p,sha in prior.items())
    verification = verify(dll,args.track,stage_symbols)
    manifest.update(frames_DLL=str(dll),frames_DLL_sha256=digest(dll),land_visibility=dict(
        source_commit=PIN,parent_manifest_sha256=digest(parent),object_sha256=digest(obj),
        generated_source_sha256=digest(adapted),arithmetic=stats,commands=commands,**verification))
    (out/'build_verification.json').write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
    print(f'{verification["cases"]} original land visibility and draw order cases passed')


if __name__=='__main__':
    main()
