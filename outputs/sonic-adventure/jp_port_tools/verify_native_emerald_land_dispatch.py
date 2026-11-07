"""Verify the scheduled original land callback before bounded model rendering."""
import ctypes as C
import inspect
import struct
from pathlib import Path

import verify_native_emerald_visible_scene as visible
from build_native_land_visibility import Entry
from extract_native_emerald_collision import ENTRY, LOAD
from native_packet_test_support import bind
from verify_native_character_tree import Node
from verify_native_model_preflight import Model


def scheduled_land(context,image,listing):
    expected = visible.select_land(context,image)
    count, _, flags, _, clip, table = struct.unpack_from('<hhhhfI',image,0x0c981904-LOAD)
    rows = [ENTRY.unpack_from(image,table-LOAD+i*ENTRY.size) for i in range(count)]
    F,I,P = C.c_float,C.c_int,C.c_void_p
    objects,models = (Node*count)(),(Model*count)()
    entries = (Entry*count)()
    for i,row in enumerate(rows):
        entries[i] = Entry((F*3)(*row[:3]),*row[3:6],C.addressof(objects[i]),row[-2],row[-1])
        if i in expected:
            obj = struct.unpack_from('<II3f3i3fII',image,row[-3]-LOAD)
            objects[i] = Node(obj[0],C.addressof(models[i]),(F*3)(*obj[2:5]),
                              (I*3)(*obj[5:8]),(F*3)(*obj[8:11]),None,None)
    f = context['f']
    get = bind(f,'njGetMatrix',None,P)
    set_matrix = bind(f,'njSetMatrix',None,P,P)
    move = bind(f,'njTranslate',None,P,F,F,F)
    rotate = bind(f,'njRotateXYZ',None,P,I,I,I)
    before, after = (F*16)(), (F*16)()
    get(before)
    matrices = {}
    for i in expected:
        set_matrix(None,before)
        move(None,*objects[i].pos)
        rotate(None,*objects[i].ang)
        get(after)
        matrices[i] = bytes(after)
    set_matrix(None,before)
    depth = I.in_dll(f,'_stack_size_').value
    model_ids = {C.addressof(models[i]):i for i in expected}
    observed, errors = [], []
    Consume = C.CFUNCTYPE(None,P)

    @Consume
    def consume(address):
        if address not in model_ids:
            errors.append(address)
            return
        i = model_ids[address]
        get(after)
        if bytes(after)!=matrices[i]:
            errors.append(i)
        observed.append(i)

    call = bind(f,'jp_land_dispatch_probe',I,P,I,P,P,I,F,I,I,Consume)
    camera = (F*3)(-9,16,4)
    for shown,step,selected in ((0,0,0),(1,1,0),(1,0,len(expected))):
        assert call(entries,count,C.byref(listing),camera,flags,clip,shown,step,consume)==selected
        assert len(observed)==selected
    get(after)
    assert observed==expected and not errors,(observed,expected,errors)
    assert bytes(after)==bytes(before) and I.in_dll(f,'_stack_size_').value==depth
    return observed


def draw_land(context,track):
    source = inspect.getsource(visible.draw_land)
    source = source.replace('draw_order = select_land(context,image)',
                            'draw_order = select_land(context,image,listing)')
    namespace = dict(vars(visible),select_land=scheduled_land)
    exec(compile(source,visible.__file__,'exec'),namespace)
    result,release = namespace['draw_land'](context,track)
    result.update(original_scheduled_land_callback=True,land_hidden_and_later_tick_skip_verified=True,
                  original_entry_matrices_compared=len(result['visibility_draw_order']))
    return result,release


def scene_caller(raw):
    return visible.scene_caller(raw).replace(
        'from verify_native_emerald_visible_scene import draw_land',
        'from verify_native_emerald_land_dispatch import draw_land').replace(
        'sonic_emerald_visible_scene','sonic_emerald_land_dispatch').replace(
        'original_Emerald_Coast_visibility_selection_drives_native_GPU_scene',
        'original_scheduled_Emerald_Coast_land_dispatch_and_GPU_scene_verified')


def exercise(context,inputs,track):
    source = inspect.getsource(visible.exercise)
    namespace = dict(vars(visible),__file__=__file__,scene_caller=scene_caller)
    exec(compile(source,visible.__file__,'exec'),namespace)
    namespace['exercise'](context,inputs,track)


if __name__=='__main__':
    scene = visible.environment.textured.scene
    runner = Path(scene.__file__).read_text().split("if __name__ == '__main__':\n",1)[1]
    anchor = 'from verify_native_emerald_scene import exercise'
    assert runner.count(anchor)==1
    runner = runner.replace(anchor,'from verify_native_emerald_land_dispatch import exercise')
    exec(compile('if True:\n'+runner,scene.__file__,'exec'),dict(vars(scene)))
