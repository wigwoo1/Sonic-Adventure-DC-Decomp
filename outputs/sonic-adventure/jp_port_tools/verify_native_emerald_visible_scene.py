"""Feed the original land visibility order into the Emerald Coast GPU fixture."""
import ctypes as C
import inspect
import struct
from pathlib import Path

import verify_native_emerald_environment_scene as environment
from build_native_land_visibility import Entry
from extract_native_emerald_collision import ENTRY, LOAD
from native_packet_test_support import bind


def select_land(context,image):
    address = 0x0c981904-LOAD
    count, _, flags, _, clip, table = struct.unpack_from('<hhhhfI',image,address)
    start = table-LOAD
    assert 0<=start<=len(image)-count*ENTRY.size
    rows = [ENTRY.unpack_from(image,start+i*ENTRY.size) for i in range(count)]
    F,I,P = C.c_float,C.c_int,C.c_void_p
    entries = (Entry*count)(*(Entry((F*3)(*row[:3]),*row[3:6],None,row[-2],row[-1])
                             for row in rows))
    output = (C.c_uint16*1024)()
    select = bind(context['f'],'jp_land_visible_indices',I,P,I,I,I,F,F,F,P,I,I,P,C.c_size_t)
    camera = (F*3)(-9,16,4)
    selected = select(entries,count,-1,flags,clip,2400,1,camera,0,0,output,len(output))
    indices = list(output[:selected])
    assert 0<selected<count and indices==sorted(set(indices),reverse=True)
    return indices


def draw_land(context,track):
    source = inspect.getsource(environment.draw_land)
    anchor = '    namespace = dict(vars(textured),verify_terminal_mip=vq.verify_terminal_mip)'
    assert source.count(anchor)==1
    source = source.replace(anchor,'''    anchor = '    for entry in (42,91,92,93,126,388,393):'
    swap(anchor,'    draw_order = select_land(context,image)\\n    for entry in draw_order:')
    swap('original_textured_land_entries=sorted(emitted_entries)',
         'original_textured_land_entries=sorted(emitted_entries),visibility_draw_order=draw_order')
    namespace = dict(vars(textured),verify_terminal_mip=vq.verify_terminal_mip,select_land=select_land)''')
    namespace = dict(vars(environment),select_land=select_land)
    exec(compile(source,environment.__file__,'exec'),namespace)
    return namespace['draw_land'](context,track)


def scene_caller(raw):
    raw = environment.scene_caller(raw).replace('from verify_native_emerald_environment_scene import draw_land',
                                               'from verify_native_emerald_visible_scene import draw_land')
    anchor = "    land['distinct_environment_coordinates']=len(env_uvs)"
    assert raw.count(anchor)==1
    raw = raw.replace(anchor,anchor+'''
    check(set(land['original_textured_land_entries'])<=set(land['visibility_draw_order']) and
          len(land['visibility_draw_order'])==13,
          'native visibility-selected land entries feed the GPU in original draw order')
''')
    return raw.replace('sonic_emerald_environment_scene','sonic_emerald_visible_scene').replace(
        'original_Emerald_Coast_environment_mapped_land_and_Sonic_rendered_together',
        'original_Emerald_Coast_visibility_selection_drives_native_GPU_scene')


def exercise(context,inputs,track):
    source = inspect.getsource(environment.exercise)
    namespace = dict(vars(environment),__file__=__file__,scene_caller=scene_caller)
    exec(compile(source,environment.__file__,'exec'),namespace)
    namespace['exercise'](context,inputs,track)


if __name__=='__main__':
    scene = environment.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n",1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor)==1
    runner = runner.replace(anchor,'from verify_native_emerald_visible_scene import exercise')
    exec(compile('if True:\n'+runner,scene.__file__,'exec'),dict(vars(scene)))
